#!/bin/sh

LOGFILE=/tmp/install.log
USE_XORG=no

# Detect whether partition ($1) mounted at ($2) with type ($3) is microsoft.
detect_os_microsoft () {
  local longname

  if [ "$3" != ntfs -a "$3" != vfat -a "$3" != msdos ]; then
    return
  fi

  if [ -e "$2/ntldr" -a -e "$2/NTDETECT.COM" ]; then
    longname="Windows NT/2000/XP/Vista"
  elif [ -e "$2/windows/win.com" ]; then
    longname="Windows 95/98/Me"
  elif [ -d "$2/dos" ]; then
    longname="MS-DOS 5.x/6.x/Win3.1"
  else
    return
  fi

  echo "$1:$longname:chain"
}

detect_os () {
  local partname partition type mpoint

  mkdir -p tmpmnt

  for partname in `sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *[0-9][0-9][0-9]*\ \([a-z]*[0-9][0-9]*\)/\1/p" /proc/partitions`; do
    partition="/dev/$partname"

    if ! grep -q "^$partition " /proc/mounts; then
      if mount -o ro $partition tmpmnt >/dev/null 2>&1; then
        type=$(grep "^$partition " /proc/mounts | cut -d " " -f 3)
        detect_os_microsoft $partition tmpmnt $type

        umount tmpmnt >/dev/null || return
      fi
    else
      mpoint=$(grep "^$partition " /proc/mounts | cut -d " " -f 2)
      type=$(grep "^$partition " /proc/mounts | cut -d " " -f 3)

      detect_os_microsoft $partition $mpoint $type
    fi
  done

  rmdir tmpmnt
}

# Usage: convert os_device
# Convert an OS device to the corresponding GRUB drive.
# This part is OS-specific.
# -- taken from `grub-install`
convert () {
  local tmp_drive tmp_disk tmp_part

  if test ! -e "$1"; then
    echo "$1: $MSG_DEV_NO_BLOCK" 1>&2
    exit 1
  fi

  tmp_disk=`echo "$1" | sed -e 's%\([sh]d[a-z]\)[0-9]*$%\1%'`
  tmp_part=`echo "$1" | sed -e 's%.*/[sh]d[a-z]\([0-9]*\)$%\1%'`

  tmp_drive=`grep -v '^#' $device_map | grep "$tmp_disk *$" \
			| sed 's%.*\(([hf]d[0-9][a-g0-9,]*)\).*%\1%'`

  if [ -z "$tmp_drive" ]; then
    echo "$1 $MSG_BIOS_NO_DRIVE" 1>&2
    exit 1
  fi

  if [ -n "$tmp_part" ]; then
    # If a partition is specified, we need to translate it into the
    # GRUB's syntax.
    echo "$tmp_drive" | sed "s%)$%,$(($tmp_part-1)))%"
  else
    # If no partition is specified, just print the drive name.
    echo "$tmp_drive"
  fi
}

# Create grub.conf file
update_grub_conf_bootargs () {
  # don't try to set an item with a non-existing value
  test -z "$3" && return;
  sed -i "s/$2=[^ ]*/$2=$3/g" "$1"
}

append_grub_conf () {
  TMP_GRUB_CONF=/tmp/grub.conf
  cp /etc/grub/grub.conf $TMP_GRUB_CONF

  sed -i "s/_TITLE_/$2/" $TMP_GRUB_CONF
  sed -i "s/_HDTV_/$3/" $TMP_GRUB_CONF
  sed -i "s/_DEBUG_/$4/" $TMP_GRUB_CONF
  sed -i "s/_CONFIG_/$5/" $TMP_GRUB_CONF

  cat $TMP_GRUB_CONF >> $1
  echo "" >> $1
}

setup_grub () {
  # conditional HDTV menu entry if X.org is found
  if [ "$USE_XORG" = yes ]; then
    append_grub_conf $1 "GeeXboX HDTV" hdtv "" ""
    append_grub_conf $1 "GeeXboX HDTV (debug)" hdtv debugging ""
    append_grub_conf $1 "GeeXboX HDTV (reconfigure)" hdtv "" configure
  fi

  # add console mode menu
  append_grub_conf $1 "GeeXboX" "" "" ""
  append_grub_conf $1 "GeeXboX (debug)" "" debugging ""
  append_grub_conf $1 "GeeXboX (reconfigure)" "" "" configure

  # put default options
  update_grub_conf_bootargs $1 lang `cmdline_default lang en`
  update_grub_conf_bootargs $1 remote `cmdline_default remote atiusb`
  update_grub_conf_bootargs $1 receiver `cmdline_default receiver atiusb`
  update_grub_conf_bootargs $1 keymap `cmdline_default keymap qwerty`

  # now setup installation specific options
  sed -i "s/_ROOTDEV_SINGLE_/$rootdev_single/g" $1
  sed -i "s/_DEVNAME_/UUID=${DEV_UUID}/g" $1
}

cmdline_default () {
  RET=`sed -n "s/.*$1=\([^ ]*\).*/\1/p" /proc/cmdline`
  test -z $RET && RET=$2
  echo $RET
}

# Select keymap
setup_keymap () {
  KEYMAP_OLD=`cmdline_default keymap qwerty`

  KEYMAPS="qwerty qwerty"
  for i in `ls /etc/keymaps`; do
    KEYMAPS="$KEYMAPS $i $i"
  done

  KEYMAP=`dialog --no-cancel --stdout --backtitle "$BACKTITLE : $MSG_KEYMAP_CONFIG" --title "$MSG_KEYMAP" --default-item $KEYMAP_OLD --menu "$MSG_KEYMAP_DESC" 0 0 0 $KEYMAPS`

  test -f "/etc/keymaps/$KEYMAP" && loadkmap < "/etc/keymaps/$KEYMAP"
}

configure () {
  IDISK=/mnt/install_disk
  mkdir -p $IDISK
  mount $DEV $IDISK
  configurator
  umount $IDISK
}

VERSION=`cat VERSION`
BACKTITLE="GeeXboX $VERSION installator"

# include language definitions
LANG=`cmdline_default lang en`
. /etc/installator/en.install # default language file, always present
[ -f /etc/installator/${LANG}.install ] && . /etc/installator/${LANG}.install

# disable kernel messages to avoid screen corruption
echo 0 > /proc/sys/kernel/printk

setup_keymap

while true; do
  DISKS=""
  for i in `cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *[0-9][0-9]*\ \([a-z]*\)$/\1/p"`; do
    S=`sfdisk -s /dev/$i | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
    VENDOR=`cat /sys/block/$i/device/vendor`
    MODEL=`cat /sys/block/$i/device/model`
    DISKNAME=`echo $VENDOR $MODEL ${S}MB | sed 's/ /_/g'`
    DISKS="$DISKS $i $DISKNAME"
  done

  if [ -z "$DISKS" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --yesno "\n${MSG_DISK_NOT_FOUND}" 0 0 || exit 1
  else
    DISKS="$DISKS $MSG_DISK_REFRESH"
    DISK=`dialog --stdout --backtitle "$BACKTITLE" --title "$MSG_DISK_DEVICE" --menu "\n${MSG_DISK_DEVICE_DESC}" 0 0 0 $DISKS` || exit 1
    [ $DISK != refresh ] && break
  fi
done

if [ "`cat /sys/block/$DISK/removable`" = 1 ]; then
  TYPE=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_REMOVABLE" --menu "$MSG_DISK_REMOVABLE_DESC" 0 0 0 REMOVABLE "$MSG_DISK_REMOVABLE" HDD "$MSG_DISK_HDD"` || exit 1
else
  TYPE=HDD
fi

if [ $TYPE = REMOVABLE ]; then
  BOOTLOADER=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_PART_LINUX" --menu "$MSG_DISK_PART_LINUX_DESC" 0 0 0 grub "$MSG_DISK_BOOT_GRUB" syslinux "$MSG_DISK_BOOT_SYSLINUX"` || exit 1
else
  BOOTLOADER=grub
fi

if [ $BOOTLOADER = syslinux ]; then
  PART_MSG="$MSG_DISK_PART_FAT"
elif [ $BOOTLOADER = grub ]; then
  PART_MSG="$MSG_DISK_PART_EXT"
fi

CFDISK_MSG="$MSG_CFDISK_BEGIN $PART_MSG $MSG_CFDISK_END"

dialog --stdout --backtitle "$BACKTITLE" --title "$MSG_INSTALL_DEV_CONFIG" --msgbox "$CFDISK_MSG" 0 0 || exit 1

cfdisk /dev/$DISK || exit 1

while [ ! -b "$DEV" ]; do
  DISKS=""
  for i in `sfdisk -l /dev/$DISK | grep ${DISK%disc} | cut -f1 -d' '`; do
    case `sfdisk --print-id ${i%%[0-9]*} ${i#${i%%[0-9]*}}` in
      1|11|6|e|16|1e|b|c|1b|1c) #FAT12/16/32 are supported both in syslinux and grub.
        S=`sfdisk -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
        VENDOR=`cat /sys/block/$DISK/device/vendor`
        MODEL=`cat /sys/block/$DISK/device/model`
        DISKNAME=`echo $VENDOR $MODEL ${S}MB | sed 's/ /_/g'`
        DISKS="$DISKS $i $DISKNAME"
        ;;
      83) #Linux is supported only in grub.
        if [ $BOOTLOADER = grub ]; then
          S=`sfdisk -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
          VENDOR=`cat /sys/block/$DISK/device/vendor`
          MODEL=`cat /sys/block/$DISK/device/model`
          DISKNAME=`echo $VENDOR $MODEL ${S}MB | sed 's/ /_/g'`
          DISKS="$DISKS $i $DISKNAME"
        fi
        ;;
    esac
  done
  if [ -z "$DISKS" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n$MSG_INSTALL_DEV_NOPART_BEGIN $PART_MSG $MSG_INSTALL_DEV_NOPART_END\n" 0 0
    exit 1
  else
    DEV=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_INSTALL_DEV_CONFIG" --menu "$MSG_INSTALL_DEV_DESC" 0 0 0 $DISKS` || exit 1
  fi
  if [ ! -b "$DEV" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n'$DEV' $MSG_INSTALL_DEV_BAD_BLOCK\n" 0 0
  fi
done

DEVNAME="${DEV#/dev/}"

case `sfdisk --print-id ${DEV%%[0-9]*} ${DEV#${DEV%%[0-9]*}}` in
  1|11|6|e|16|1e|14|b|c|1b|1c)
    SUPPORTED_TYPES="vfat"
    PART_TYPE="FAT"
    ;;
  83) # Linux
    SUPPORTED_TYPES="ext3 ext2"
    PART_TYPE="Linux"
    ;;
esac

mkdir di

# Try to guess current partition type.
MKFS_TYPE=
for type in vfat ext3 ext2 auto; do
  if mount -o ro -t $type "$DEV" di; then
    MKFS_TYPE=`grep "^$DEV " /proc/mounts | cut -d " " -f 3`
    umount di
    break
  fi
done

NEED_FORMAT=yes
FORMAT_DEFAULT="--defaultno"
if [ -z "$MKFS_TYPE" ]; then
  FORMAT_MSG="$MSG_INSTALL_DEV_NO_FORMAT"
  FORMAT_DEFAULT=""
else
  for type in $SUPPORTED_TYPES; do
    [ $type = $MKFS_TYPE ] && NEED_FORMAT=no
  done

  if [ "$NEED_FORMAT" = yes ]; then
    FORMAT_MSG="$MSG_INSTALL_DEV_FORMAT_BEGIN ($MKFS_TYPE) $MSG_INSTALL_DEV_FORMAT_END ($PART_TYPE)."
  else
    FORMAT_MSG="$MSG_INSTALL_DEV_FORMATED"
  fi
fi

dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_INSTALL_DEV_FORMAT" $FORMAT_DEFAULT --yesno "${FORMAT_MSG}\n${MSG_INSTALL_DEV_FORMAT_DESC} '$DEV' ?\n" 0 0 && FORMAT=yes

if [ "$FORMAT" = yes ]; then
  case `sfdisk --print-id ${DEV%%[0-9]*} ${DEV#${DEV%%[0-9]*}}` in
    1|11|6|e|16|1e|14) # FAT12 and FAT16
      MKFS=mkdosfs
      MKFS_OPT="-n GEEXBOX"
      MKFS_TYPE=vfat
      MKFS_TYPENAME="FAT"
      ;;
    b|c|1b|1c) # FAT32
      MKFS=mkdosfs
      MKFS_OPT="-n GEEXBOX -F 32"
      MKFS_TYPE=vfat
      MKFS_TYPENAME="FAT"
      ;;
    83) # Linux
      MKFS_TYPE=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_INSTALL_PART_TYPE" --menu "$MSG_INSTALL_PART_TYPE_DESC" 0 0 0 ext2 "Linux ext2" ext3 "Linux ext3"` || exit 1

      case $MKFS_TYPE in
        ext2)
          MKFS=mke2fs
          MKFS_OPT="-L GEEXBOX"
          MKFS_TYPENAME="Linux ext2"
          ;;
        ext3)
          MKFS=mke2fs
          MKFS_OPT="-L GEEXBOX -j"
          MKFS_TYPENAME="Linux ext3"
          ;;
      esac
      ;;
  esac

  if [ -z "$MKFS" ]; then
    if [ "$NEED_FORMAT" = yes ]; then
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n${MSG_INSTALL_DEV_NO_FORMAT} ('$DEV'). ${MSG_INSTALL_FORMAT_NO_TOOLS}\n" 0 0
      rmdir di
      exit 1
    else
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_WARNING" --msgbox "\n'$DEV' $MSG_INSTALL_FORMAT_BAD_TYPE $MKFS_TYPENAME. ${MSG_INSTALL_FORMAT_NO_TOOLS}. ${MSG_INSTALL_FORMAT_ALREADY}\n" 0 0
    fi
  else
    echo "$MKFS $MKFS_OPT \"$DEV\"" >> $LOGFILE
    $MKFS $MKFS_OPT "$DEV" >> $LOGFILE 2>&1
  fi
elif [ "$NEED_FORMAT" = yes ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n${MSG_INSTALL_DEV_NO_FORMAT} ('$DEV')\n" 0 0
  rmdir di
  exit 1
fi

# restart UDEV scan to get device UUID if
# user just created/formatted a new disk/partition
udevadm trigger
udevadm settle --timeout=180

DEV_REALNAME=`ls -l $DEV | sed "s/.*-> \(.*\)/\1/"`
for dev in `ls /dev/storage/by-uuid/*`; do
  NAME=`ls -l "$dev" | sed "s/.*-> \(.*\)/\1/" | sed 's%../../%%'`
  if [ "$NAME" = "$DEV_REALNAME" ]; then
    DEV_UUID=`echo $dev | sed 's%/dev/storage/by-uuid/%%'`
    break
  fi
done

# ensure UDEV hasn't remounted the install partition
umount /dev/$NAME

mount -t $MKFS_TYPE "$DEV" di
if [ $? -ne 0 ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n${MSG_INSTALL_MOUNT_FAILED} '$DEV' ($MKFS_TYPENAME).\n" 0 0
  rmdir di
  exit 1
fi

if [ -n "$NFS" ]; then
  GEEXBOX="$NFS"
else
  GEEXBOX="$CDROM/GEEXBOX"
fi

rm -rf di/GEEXBOX
cp -a "$GEEXBOX" di/GEEXBOX >> $LOGFILE 2>&1 

[ "$PART_TYPE" = "Linux" ] && dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOT_SLEEPLESS" --defaultno --yesno "\n${MSG_BOOT_SLEEPLESS_DESC}\n" 0 0 && FASTBOOT=yes && echo "" > "di/GEEXBOX/var/fastboot"

if [ "$FASTBOOT" = "yes" ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOT_LARGE_HDD" --defaultno --yesno "\n${MSG_BOOT_LARGE_HDD_DESC}\n" 0 0 && UNCOMPRESS_INSTALL=yes && rm di/GEEXBOX/bin.tar.lzma
  [ "$UNCOMPRESS_INSTALL" = "yes" -a -f "$GEEXBOX/bin.tar.lzma" ] && tar xaf "$GEEXBOX/bin.tar.lzma" -C di/GEEXBOX >> $LOGFILE 2>&1
fi

cd di/GEEXBOX/boot
mv vmlinuz initrd.gz isolinux.cfg vesamenu.c32 help.msg splash.png ../../
cd ../../../
rm -rf di/GEEXBOX/boot

# Configure X.Org
if [ -f /etc/X11/X.cfg.sample -o -f /etc/X11/X.cfg ]; then
  USE_XORG=yes # default is to use X if present
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_CFG_HDTV" --yesno "\n${MSG_CFG_HDTV_DESC}\n" 0 0 || USE_XORG=no
fi

if [ "$USE_XORG" = yes ]; then
  [ "$UNCOMPRESS_INSTALL" = "yes" -a -f "$GEEXBOX/X.tar.lzma" ] && rm di/GEEXBOX/X.tar.lzma && tar xaf "$GEEXBOX/X.tar.lzma" -C di/GEEXBOX
else
  # Since X is disabled, remove the files from HDD install to speed up boot
  rm -f di/GEEXBOX/X.tar.lzma
fi

grubprefix=/boot/grub
grubdir=di$grubprefix
device_map=$grubdir/device.map

rm -rf $grubdir
mkdir -p $grubdir
[ -f "di/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" ] && tar xaf "di/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" -C $grubdir >> $LOGFILE 2>&1

if [ -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" ]; then
  cp -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" $grubdir || exit 1
  disable_splashimage=
else
  disable_splashimage="#"
fi
splashimage="$grubprefix/grub-splash.xpm.gz"

if [ $BOOTLOADER = syslinux ]; then
  cp "di/GEEXBOX/usr/share/ldlinux.sys" di
  sed -e "s/boot=cdrom/boot=UUID=${DEV_UUID}/" di/isolinux.cfg > di/syslinux.cfg
  sed -i s%^#CFG#%% di/syslinux.cfg
  rm di/isolinux.cfg

  echo "*** Syslinux.cfg ***" >> $LOGFILE
  cat di/syslinux.cfg >> $LOGFILE
elif [ $BOOTLOADER = grub ]; then
  cp $grubdir/stage2 $grubdir/stage2_single
  rm di/isolinux.cfg di/vesamenu.c32 di/help.msg di/splash.png
fi

if [ $TYPE = HDD ]; then
  echo "quit" | grub --batch --no-floppy --device-map=$device_map >> $LOGFILE 2>&1
elif [ $TYPE = REMOVABLE ]; then
  echo "(hd0) ${DEV%%[0-9]*}" > $device_map
fi

rootdev=$(convert $DEV)

if [ -z "$rootdev" ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n${MSG_GRUB_NO_ROOTDEV}\n" 0 0
  umount di
  rmdir di
  exit 1
fi

if [ $BOOTLOADER = syslinux ]; then
  umount di
  syslinux "$DEV" >> $LOGFILE 2>&1
  mount -t $MKFS_TYPE "$DEV" di
elif [ $BOOTLOADER = grub ]; then
  if [ $TYPE = HDD ]; then
    rootdev_single=$rootdev
    fake_device=
  elif [ $TYPE = REMOVABLE ]; then
    rootdev_single="(fd0)"
    fake_device="device $rootdev_single $DEV"
  fi

  echo "grub --batch --no-floppy --device-map=$device_map" >> $LOGFILE
  echo "$fake_device" >> $LOGFILE
  echo "root $rootdev_single" >> $LOGFILE
  echo "install --stage2=$grubdir/stage2_single $grubprefix/stage1 $rootdev_single $grubprefix/stage2_single p $grubprefix/single.lst" >> $LOGFILE
  echo "" >> $LOGFILE

  grub --batch --no-floppy --device-map=$device_map >> $LOGFILE 2>&1 <<EOF
$fake_device
root $rootdev_single
install --stage2=$grubdir/stage2_single $grubprefix/stage1 $rootdev_single $grubprefix/stage2_single p $grubprefix/single.lst
EOF

  cat > $grubdir/single.lst <<EOF
default  0
timeout  2
color cyan/blue white/blue
${disable_splashimage}splashimage=$rootdev_single$splashimage

EOF

  setup_grub $grubdir/single.lst

  echo "*** GRUB Single.lst ***" >> $LOGFILE
  cat $grubdir/single.lst >> $LOGFILE
fi

if [ $TYPE = HDD ]; then
  oslist=$(detect_os)

  supported_os_list=""
  saveifs=$IFS
  IFS='
'
  for os in $oslist; do
      title=$(echo "$os" | cut -d: -f2)
      if [ -n "$supported_os_list" ]; then
  	supported_os_list="$supported_os_list, $title"
      else
  	supported_os_list="$title"
      fi
  done
  IFS=$saveifs

  if [ -n "$supported_os_list" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --defaultno --yesno "\n'$DEV' $MSG_LOADER_MULTIBOOT_BEGIN $supported_os_list\n${MSG_LOADER_MULTIBOOT_END}\n" 0 0 && MBR=yes
  else
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --yesno "\n'$DEV' ${MSG_LOADER_NONE}\n" 0 0 && MBR=yes
  fi
elif [ $TYPE = REMOVABLE ]; then
  oslist=
  MBR=yes
fi

if [ "$MBR" = "yes" ]; then

  echo "grub --batch --no-floppy --device-map=$device_map" >> $LOGFILE
  echo "root $rootdev" >> $LOGFILE
  echo "setup --stage2=$grubdir/stage2 --prefix=$grubprefix (hd0)" >> $LOGFILE
  echo "" >> $LOGFILE

  grub --batch --no-floppy --device-map=$device_map >> $LOGFILE 2>&1 <<EOF
root $rootdev
setup --stage2=$grubdir/stage2 --prefix=$grubprefix (hd0)
EOF

  cat > $grubdir/menu.lst <<EOF
default  0
timeout  5
color cyan/blue white/blue
${disable_splashimage}splashimage=$rootdev$splashimage

EOF

  saveifs=$IFS
  IFS='
'
  for os in $oslist; do
    partition=$(echo "$os" | cut -d: -f1)
    grubpartition=$(convert $partition)

    [ -z "$grubpartition" ] && continue

    grubdisk=${grubpartition%,*}
    grubdisk="(${grubdisk#(})"
    title=$(echo "$os" | cut -d: -f2)
    type=$(echo "$os" | cut -d: -f3)

    if [ "$type" = chain ]; then
      cat >> $grubdir/menu.lst <<EOF
title	$title
EOF
      if [ $grubdisk != "(hd0)" ]; then
        cat >> $grubdir/menu.lst <<EOF
map (hd0) $grubdisk
map $grubdisk (hd0)
EOF
      fi

      cat >> $grubdir/menu.lst <<EOF
rootnoverify $grubpartition
makeactive
chainloader +1
boot

EOF
    fi
  done
  IFS=$saveifs

  setup_grub $grubdir/menu.lst

  echo "*** GRUB menu.lst ***" >> $LOGFILE
  cat $grubdir/menu.lst >> $LOGFILE
else
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --msgbox "\n${MSG_LOADER_ERROR}\n" 0 0
fi

umount di
rmdir di

[ -n "$CDROM" ] && eject -s &

dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_LOG" --defaultno --yesno "$MSG_LOG_DESC" 0 0 && dialog --textbox $LOGFILE 0 0

dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_SUCCESS" --yesno "\n${MSG_SUCCESS_DESC_BEGIN} '$DEV' !! ${MSG_SUCCESS_DESC_END}\n" 0 0 && configure
