#!/bin/sh

DISTRO=dummy
VERSION=dummy
ARCH=dummy
BACKTITLE_TXT="$DISTRO $ARCH $VERSION installator"

LOGFILE="/tmp/install.log"
ANSWER="/tmp/answer"


main_menu() {
  TXT="\nThis program installs \Zb$DISTRO\Zn on your computer \
       \n\n\Z1\ZbWARNING:\Zn The contents of the selected device will be erased! \
       \n\n\nPlease choose:"

  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- Main Menu -" \
    --cancel-label "Reboot" \
    --menu "$TXT" 15 70 2 \
      1 "Start installation" \
      2 "Show logfile" 2> ${ANSWER}_main

  case $? in
    0)
      case $(cat "${ANSWER}_main") in
        1) do_install; break;;
        2) show_logfile; break;;
      esac
      ;;
    1)
      do_reboot
      ;;
    255)
      do_shutdown
      ;;
  esac
}


do_install() {
  device_menu || main_menu

  efi_menu
  INSTALL_EFI=$?

  has_ssd_with_tirm
  INSTALL_SSD=$?

  start_install_menu || main_menu

  echo "### $BACKTITLE_TXT" > $LOGFILE

  INSTALL_FSYS=ext4
  # Make sure the ESP size is big enough to get FAT32!
  INSTALL_ESP_SIZE=200MB
  INSTALL_PART_TABLE=msdos
  [ "$INSTALL_EFI" = 0 ] && INSTALL_PART_TABLE=gpt


  progress_install 10 "Erasing device"
  parted -s $INSTALL_DEV mktable $INSTALL_PART_TABLE >> $LOGFILE 2>&1
  INSTALL_PART=${INSTALL_DEV}1


  progress_install 20 "Setting up partitions"
  if [ "$INSTALL_EFI" = 0 ]; then
    # Create ESP (EFI System Partition) and root partition if an EFI based installation requested/possible
    INSTALL_EFI_PART=${INSTALL_DEV}1
    parted -s --align=opt $INSTALL_DEV mkpart primary 0% $INSTALL_ESP_SIZE >> $LOGFILE 2>&1
    mkfs.vfat $INSTALL_EFI_PART >> $LOGFILE 2>&1

    INSTALL_PART=${INSTALL_DEV}2
    parted -s --align=opt $INSTALL_DEV mkpart primary $INSTALL_ESP_SIZE 100% >> $LOGFILE 2>&1
  else
    # Otherwise create a full partition on whole device
    parted -s --align=opt $INSTALL_DEV mkpart primary 0% 100% >> $LOGFILE 2>&1
  fi
  # Setting boot flag is important especially for the ESP with GPT table
  parted -s $INSTALL_DEV set 1 boot on >> $LOGFILE 2>&1
  # Log the new device layout
  parted -s $INSTALL_DEV print >> $LOGFILE 2>&1


  progress_install 30 "Formatting"
  mkfs.$INSTALL_FSYS -L $DISTRO -m 0 $INSTALL_PART  >> $LOGFILE 2>&1
  tune2fs -c 0 -i 0 $INSTALL_PART  >> $LOGFILE 2>&1
  mkdir -p /tmp/installator
  blkid -o udev $INSTALL_PART > /tmp/installator/blkid
  . /tmp/installator/blkid


  progress_install 40 "Copying files"
  INSTALL_MOUNT="/tmp/installator/target"
  mkdir -p $INSTALL_MOUNT >> $LOGFILE 2>&1
  mount -t $INSTALL_FSYS $INSTALL_PART $INSTALL_MOUNT >> $LOGFILE 2>&1
  cp -PR /.squashfs/* $INSTALL_MOUNT >> $LOGFILE 2>&1
  

  progress_install 70 "Creating fstab"
  echo "proc /proc proc defaults 0 0" > $INSTALL_MOUNT/etc/fstab
  echo "UUID=${ID_FS_UUID} / $INSTALL_FSYS relatime,errors=remount-ro 0 1" >> $INSTALL_MOUNT/etc/fstab


  progress_install 80 "Installing the kernel"
  mkdir -p $INSTALL_MOUNT/boot >> $LOGFILE 2>&1
  cp -P /.root/vmlinuz $INSTALL_MOUNT/boot/ >> $LOGFILE 2>&1
  cp -P /.root/initrd $INSTALL_MOUNT/boot/ >> $LOGFILE 2>&1


  progress_install 90 "Installing the boot loader"
  if [ "$INSTALL_EFI" = 0 ]; then
    INSTALL_EFI_MOUNT="/tmp/installator/efi"
    mkdir -p $INSTALL_EFI_MOUNT >> $LOGFILE 2>&1
    mount -t vfat $INSTALL_EFI_PART $INSTALL_EFI_MOUNT >> $LOGFILE 2>&1
    cp -PR /.root/efi $INSTALL_EFI_MOUNT >> $LOGFILE 2>&1

    cp /usr/share/installator/grub.cfg $INSTALL_EFI_MOUNT/efi/boot >> $LOGFILE 2>&1
    sed -i $INSTALL_EFI_MOUNT/efi/boot/grub.cfg \
      -e "s/ID_FS_UUID/$ID_FS_UUID/" \
      -e "s/INSTALL_FSYS/$INSTALL_FSYS/" \
      -e "s/DISTRO/$DISTRO/" \
      -e "s/ARCH/$ARCH/" \
      -e "s/VERSION/$VERSION/" >> $LOGFILE 2>&1

    umount $INSTALL_EFI_MOUNT >> $LOGFILE 2>&1
    umount $INSTALL_MOUNT >> $LOGFILE 2>&1
  else
    cp -P /.root/isolinux/splash.png $INSTALL_MOUNT/boot >> $LOGFILE 2>&1
    cp -P /.root/isolinux/vesamenu.c32 $INSTALL_MOUNT/boot >> $LOGFILE 2>&1
    cp -P /.root/isolinux/help.msg $INSTALL_MOUNT/boot >> $LOGFILE 2>&1

    cp /usr/share/installator/syslinux.cfg $INSTALL_MOUNT/boot >> $LOGFILE 2>&1
    sed -i $INSTALL_MOUNT/boot/syslinux.cfg \
      -e "s/ID_FS_UUID/$ID_FS_UUID/" \
      -e "s/DISTRO/$DISTRO/" \
      -e "s/ARCH/$ARCH/" \
      -e "s/VERSION/$VERSION/" >> $LOGFILE 2>&1

    extlinux --install $INSTALL_MOUNT/boot >> $LOGFILE 2>&1
    umount $INSTALL_MOUNT >> $LOGFILE 2>&1
    cat /usr/share/syslinux/mbr.bin > $INSTALL_DEV
  fi
  progress_install 100 "Done"


  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- Installation -" \
    --msgbox "\nInstallation finished\n" 0 0

  main_menu
}


device_menu() {
  get_device_list
  MENU_ITEMS=""
  for i in $DEVICES; do
    get_device_details $i
    MENU_ITEMS="$MENU_ITEMS $i $DEVICE_DETAILS"
  done

  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- Device Menu -" \
    --cancel-label "Back" \
    --ok-label "Continue" \
    --menu "\nHere's a list of the hard drives I could find on your system:" 15 70 5 \
      $MENU_ITEMS 2> ${ANSWER}_device
  RETVAL=$?

  INSTALL_DEV=$(cat "${ANSWER}_device")

  return $RETVAL
}


efi_menu() {
  is_efi_capable || return 2

  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- EFI Menu -" \
    --yesno "\nDo you want an EFI based installation?\n" 0 0
  RETVAL=$?

  return $RETVAL
}


start_install_menu() {
  TXT="\nDo you want to start the installtation with this parameters? \n \
       \n\ZbDevice:\Zn            $INSTALL_DEV \
       \n\ZbEFI Support:\Zn       $([ "$INSTALL_EFI" = 0 ] && echo "Yes" || echo "No") \
       \n\ZbSSD Trim Support:\Zn  $([ "$INSTALL_SSD" = 0 ] && echo "Yes" || echo "No") \
       \n\n\Z1\ZbWARNING:\Zn The contents of the selected device will be erased! "

  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- Start Installation -" \
    --yesno "$TXT" 0 0
  RETVAL=$?

  return $RETVAL
}


get_device_details() {
  DEVICE_MODEL=$(parted -s $1 -m print | grep $1 | cut -f7 -d ":" | sed "s/;//")
  DEVICE_SIZE=$(parted -s $1 -m print | grep $1 | cut -f2 -d ":")
  DEVICE_DETAILS=$(echo $DEVICE_MODEL ${DEVICE_SIZE} | sed 's/ /_/g')
}


get_device_list() {
  DEVICES=$(parted -s -m -l | grep /dev/sd | cut -f1 -d ":")

  for i in $(cat /proc/mounts | grep /dev/sd | cut -f1 -d " " | sed "s/[0-9]$//"); do
    DEVICES=$(echo $DEVICES | sed -e "s|$i||")
  done
}


is_efi_capable() {
  efibootmgr > /dev/null 2>&1
  RETVAL=$?

  return $RETVAL
}


has_ssd_with_tirm() {
  [ -z "$1" -o ! -r $1 ] && return 2

  hdparm -I $1 | grep TRIM > /dev/null 2>&1
  RETVAL=$?

  return $RETVAL
}


progress_install() {
  echo "\n### $2" >> $LOGFILE 2>&1

  dialog --colors \
    --backtitle "$BACKTITLE" \
    --title "\Z7- Installation -" \
    --gauge "$2" 6 70 $1 &

  [ $1 = 100 ] && sleep 1 && dialog --clear
}


show_logfile() {
  dialog --colors \
    --backtitle "$BACKTITLE_TXT" \
    --title "\Z7- Log File -" \
    --exit-label "Back" \
    --textbox "$LOGFILE" 20 70

  main_menu
}


do_reboot() {
  echo "Rebooting..."
  reboot
}


do_shutdown() {
  echo "Shuting down..."
  poweroff
}


while true; do
  main_menu
done


