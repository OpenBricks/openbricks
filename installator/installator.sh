#!/bin/sh

/bin/busybox mount -t proc none /proc
/bin/busybox --install -s

if [ "$1" = geexbox ]; then
  DIALOG=/usr/bin/dialog
  CFDISK=/usr/bin/cfdisk
  SFDISK=/usr/bin/sfdisk
  MKDOSFS=/usr/bin/mkdosfs
  SYSLINUX=/usr/bin/syslinux
else
  DIALOG=""
  [ "$1" != --nodialog ] && DIALOG=`which dialog`
  CFDISK=`which cfdisk`
  SFDISK=`which sfdisk`
  MKDOSFS=`which mkdosfs`
  SYSLINUX=`which syslinux`
fi
VERSION=`cat VERSION`
BACKTITLE="GeeXboX $VERSION installator"

if [ "$UID" != "0" ]; then
  if [ -n "$DIALOG" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYour need to be root to install the GeeXboX\n" 0 0
  else
    echo ""
    echo "**** Your need to be root to install the GeeXboX ****"
    echo ""
  fi
  exit 1
fi

if [ -z "$SFDISK" -o -z "$SYSLINUX" ]; then
  if [ -n "$DIALOG" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYou need to have syslinux and sfdisk installed to install the GeeXboX\n" 0 0
  else
    echo ""
    echo "**** You need to have syslinux installed to install the GeeXboX ****"
    echo ""
  fi
  exit 1
fi

if [ -n "$DIALOG" ]; then
  while true; do
    if [ -e /dev/.devfsd ]; then
      DISKS=`cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *\([0-9][0-9]*\)\ \([a-z0-9/]*disc\).*$/\2 (\1_blocks)/p"`
    else
      DISKS=`cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *\([0-9][0-9]*\)\ \([a-z]*\)$/\2 (\1_blocks)/p"`
    fi
    if [ -z "$DISKS" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --yesno "\nNo disks found on this system.\nRecheck ?" 0 0 || exit 1
    else
      DISKS="$DISKS refresh list"
      if [ -z "$CFDISK" ]; then
        CFDISK_MSG="As you don't have cfdisk installed, the installator wont be able to create the partition for you. You have to do it yourself before installing."
      else
        CFDISK_MSG="You can now edit your partition table to create this FAT16 partition (type=06). Be carefull to choose the right disk ! We wont be responsible for any data loss."
      fi
      DISK=`$DIALOG --stdout --backtitle "$BACKTITLE" --title "Installation device" --menu "\nYou are going to install the GeeXboX. For this you will need a PRIMARY FAT16 partition (hdX1 to hdX4) with about 8 MB of free space (max. 1 GB). It WONT work with FAT32 or ext2 partitions.\n$CFDISK_MSG" 0 0 0 $DISKS` ||exit 1
      [ $DISK != refresh ] && break
    fi
  done
  $CFDISK /dev/$DISK || exit 1
else
  echo ""
  echo "You are going to install the GeeXboX. For this you will need a PRIMARY"
  echo "FAT16 partition (hdX1 to hdX4) with about 8 MB of free space (max 1 GB)"
  echo "It WONT work with FAT32 or ext2 partitions."
  echo "This installator wont create the partition. You have to do it yourself"
  echo "before installing. And be carefull when you choose the partition"
  echo "where to install ! We wont be responsible for any data loss."
  echo ""
fi

while [ ! -b "$DEV" ]; do
  if [ -n "$DIALOG" ]; then
    DISKS=""
    for i in `$SFDISK -l | grep FAT16 | grep ${DISK%disc} | cut -f1 -d' '`; do
      S=`$SFDISK -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
      DISKS="$DISKS $i ${S}MB"
    done
    if [ -z "$DISKS" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYou don't have any FAT16 partition on your system. Please first create a FAT16 partition usising cfdisk for example.\n" 0 0
      exit 1
    else
      DEV=`$DIALOG --stdout --aspect 15 --backtitle "$BACKTITLE" --title "Installation device" --menu "Where do you want to install GeeXboX ?" 0 0 0 $DISKS` || exit 1
    fi
  else
    read -p "Where do you want to install GeeXboX ? (eg: /dev/hda1) " DEV
    echo ""
  fi
  if [ ! -b "$DEV" ]; then
    if [ -n "$DIALOG" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\n'$DEV' is not a valid block device\n" 0 0
    else
      echo ""
      echo "**** '$DEV' is not a valid block device ****"
      echo ""
      exit 1
    fi
  fi
done

if [ -z "$MKDOSFS" ]; then
  if [ -n "$DIALOG" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Warning" --msgbox "\n'$DEV' need to be a FAT16 partition. As you don't have mkdosfs installed, I won't be able to format the partition. Hope the partition is already formated.\n" 0 0
  else
    echo "'$DEV' need to be a FAT16 partition."
    echo "As you don't have mkdosfs installed, I won't be able to format the"
    echo "partition Hope the partition is already formated."
  fi
else
  if [ -n "$DIALOG" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Formating" --defaultno --yesno "\nDo you want to format '$DEV' in FAT16 ?\n" 0 0 && FORMAT=yes
  else
    read -p "Do you want to format $DEV in FAT16 ? (yes/no) " FORMAT
  fi
fi
echo ""

[ "$FORMAT" = yes ] && $MKDOSFS -n GEEXBOX "$DEV"
mkdir di
mount -t vfat "$DEV" di
if [ -d disk ]; then
  cp -a disk/* di
else
  cp -a "$CDROM/GEEXBOX" di
  mv di/GEEXBOX/boot/* di
  rm di/isolinux.bin
  sed 's/TIMEOUT.*//' di/isolinux.cfg > di/syslinux.cfg
  sed 's/PROMPT.*//' di/syslinux.cfg > di/isolinux.cfg
  sed "s/boot=cdrom/boot=${DEV#/dev/}/" di/isolinux.cfg > di/syslinux.cfg
  rm di/isolinux.cfg
fi
umount di
rmdir di
syslinux "$DEV"

if [ -n "$DIALOG" ]; then
  `$DIALOG --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a bootable partition. To boot on it, you will need to install a bootloader. If you don't have any other operating system on this hard disk, I can install a bootloader for you. Else, you will need to configure yourself a boot menu such as lilo.\n\nDo you want to install a single system bootloader ?\n" 0 0` && MBR=yes
else
  echo ""
  echo "'$DEV' is now a bootable partition. To boot on it, you will need to"
  echo "install a bootloader. If you don't have any other operating system on"
  echo "this hard disk, I can install a bootloader for you. Else, you will"
  echo "need to configure yourself a boot menu such as lilo."
  echo ""
  read -p "Do you want to install a single system bootloader ? (yes/no) " MBR
fi

if [ "$MBR" = yes ]; then
  dd if=mbr.bin of="/dev/$DISK"
  PART="${DEV#${DEV%%[0-9]*}}"
  echo ",,,*" | $SFDISK "/dev/$DISK" -N$PART
else
  if [ -n "$DIALOG" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --msgbox "\nYou can configure lilo to boot the GeeXboX simply by adding thoses lines at the end of your /etc/lilo.conf :\n\n    other=$DEV\n          label=GeeXboX\n\nDon't forget to execute lilo after doing this modification.\n\nIf you only use windows, you may have a look at a boot menu such as XOSL (http://www.xosl.org/)." 0 0
  else
    echo ""
    echo "You can configure lilo to boot the GeeXboX simply by adding thoses"
    echo "lines at the end of your /etc/lilo.conf :"
    echo "    other=$DEV"
    echo "          label=GeeXboX"
    echo "Don't forget to execute lilo after doing this modification."
  fi
fi

[ -n "$CDROM" ] && eject &

if [ -n "$DIALOG" ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Have Fun" --msgbox "\nThe GeeXboX is now installed on '$DEV'\n" 0 0
else
  echo ""
  echo "**** The GeeXboX is now installed on $DEV ****"
  echo ""
fi
