#!/bin/sh

# Detect whether partition ($1) mounted at ($2) with type ($3) is microsoft.
detect_os_microsoft () {
  local longname

  if [ "$3" != ntfs -a "$3" != vfat -a "$3" != msdos ]; then
    return
  fi

  if [ -e "$2/ntldr" -a -e "$2/NTDETECT.COM" ]; then
    longname="Windows NT/2000/XP"
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
	
        umount $partition >/dev/null 2>&1
      fi
    else
      mpoint=$(grep "^$partition " /proc/mounts | cut -d " " -f 2)
      type=$(grep "^$partition " /proc/mounts | cut -d " " -f 3)

      detect_os_microsoft $partition $mpoint $type
    fi
  done

  rm -r tmpmnt
}

# Usage: convert os_device
# Convert an OS device to the corresponding GRUB drive.
# This part is OS-specific.
# -- taken from `grub-install`
convert () {
  local tmp_drive tmp_disk tmp_part

  if test ! -e "$1"; then
    echo "$1: Not found or not a block device." 1>&2
    exit 1
  fi

  tmp_disk=`echo "$1" | sed -e 's%\([sh]d[a-z]\)[0-9]*$%\1%'`
  tmp_part=`echo "$1" | sed -e 's%.*/[sh]d[a-z]\([0-9]*\)$%\1%'`

  tmp_drive=`grep -v '^#' $device_map | grep "$tmp_disk *$" \
			| sed 's%.*\(([hf]d[0-9][a-g0-9,]*)\).*%\1%'`

  if [ -z "$tmp_drive" ]; then
    echo "$1 does not have any corresponding BIOS drive." 1>&2
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

/bin/busybox mount -t proc none /proc
/bin/busybox --install -s

if [ "$1" = geexbox ]; then
  DIALOG=/usr/bin/dialog
  CFDISK=/usr/bin/cfdisk
  SFDISK=/usr/bin/sfdisk
  MKDOSFS=/usr/bin/mkdosfs
  MKE2FS=/usr/bin/mke2fs
  GRUB=/usr/bin/grub
else
  DIALOG=`which dialog`
  CFDISK=`which cfdisk`
  SFDISK=`which sfdisk`
  MKDOSFS=`which mkdosfs`
  MKE2FS=`which mke2fs`
  GRUB=`which grub`
fi
VERSION=`cat VERSION`
BACKTITLE="GeeXboX $VERSION installator"

if [ "$UID" != "0" ]; then
  echo ""
  echo "**** You need to be root to install GeeXboX ****"
  echo ""
  exit 1
fi

if [ -z "$SFDISK" -o -z "$GRUB" -o -z "$DIALOG" ]; then
    echo ""
    echo "**** You need to have sfdisk, grub and dialog installed to install GeeXboX ****"
    echo ""
fi

while true; do
    if [ -e /dev/.devfsd ]; then
      DISKS=`cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *\([0-9][0-9]*\)\ \([a-z0-9/]*disc\).*$/\2 (\1_blocks)/p"`
    else
      DISKS=`cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *\([0-9][0-9]*\)\ \([a-z]*\)$/\2 (\1_blocks)/p"`
    fi
    if [ -z "$DISKS" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --yesno "\nNo disks found on this system.\nCheck again?" 0 0 || exit 1
    else
      DISKS="$DISKS refresh list"
      if [ -z "$CFDISK" ]; then
        CFDISK_MSG="As you don't have cfdisk installed, the installator won't be able to create the partition for you. You'll have to create it yourself before installing."
      else
        CFDISK_MSG="You can now edit your partition table to create a FAT partition (type=0B) or Linux ext2/3 partition (type=83). Be careful to choose the right disk! We won't take responsibility for any data loss."
      fi
      DISK=`$DIALOG --stdout --backtitle "$BACKTITLE" --title "Installation device" --menu "\nYou are going to install GeeXboX. For this you will need an empty FAT or Linux ext2/3 partition with about 8 MB of free space.\n$CFDISK_MSG" 0 0 0 $DISKS` || exit 1
      [ $DISK != refresh ] && break
    fi
done
if [ -n "$CFDISK" ]; then
    $CFDISK /dev/$DISK || exit 1
fi

while [ ! -b "$DEV" ]; do
    DISKS=""
    for i in `$SFDISK -l /dev/$DISK | grep ${DISK%disc} | cut -f1 -d' '`; do
      case `$SFDISK --print-id ${i%%[0-9]*} ${i#${i%%[0-9]*}}` in
        1|11|6|e|16|1e|14|b|c|1b|1c|83)
          S=`$SFDISK -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
          DISKS="$DISKS $i ${S}MB"
          ;;
      esac
    done
    if [ -z "$DISKS" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYou don't have any FAT or Linux ext2/3 partition on your system. Please create a FAT or Linux ext2/3 partition first using for example cfdisk.\n" 0 0
      exit 1
    else
      DEV=`$DIALOG --stdout --aspect 15 --backtitle "$BACKTITLE" --title "Installation device" --menu "Where do you want to install GeeXboX?" 0 0 0 $DISKS` || exit 1
    fi
    if [ ! -b "$DEV" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\n'$DEV' is not a valid block device.\n" 0 0
    fi
done

DEVNAME="${DEV#/dev/}"

case `$SFDISK --print-id ${DEV%%[0-9]*} ${DEV#${DEV%%[0-9]*}}` in
  1|11|6|e|16|1e|14) # FAT12 and FAT16
    MKFS=$MKDOSFS
    MKFS_OPT="-n GEEXBOX"
    MKFS_TYPE=vfat
    MKFS_TYPENAME="FAT"
    ;;
  b|c|1b|1c) # FAT32
    MKFS=$MKDOSFS
    MKFS_OPT="-n GEEXBOX -F 32"
    MKFS_TYPE=vfat
    MKFS_TYPENAME="FAT"
    ;;
   83) # Linux
     MKFS_TYPE=`$DIALOG --stdout --aspect 15 --backtitle "$BACKTITLE" --title "Linux partition type" --menu "Which type of Linux partition you want ?" 0 0 0 ext2 "Linux ext2" ext3 "Linux ext3"` || exit 1

     case $MKFS_TYPE in
       ext2)
         MKFS=$MKE2FS
         MKFS_OPT="-L GEEXBOX"
         MKFS_TYPENAME="Linux ext2"
         ;;
       ext3)
         MKFS=$MKE2FS
         MKFS_OPT="-L GEEXBOX -j"
         MKFS_TYPENAME="Linux ext3"
         ;;
     esac
     ;;
esac

if [ -z "$MKFS" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Warning" --msgbox "\n'$DEV' needs to be a $MKFS_TYPENAME partition. As you don't have formatting tool installed, I won't be able to format the partition. Hopefully it is already formatted.\n" 0 0
else
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Formatting" --defaultno --yesno "\nDo you want to format '$DEV' ?\n" 0 0 && FORMAT=yes
fi
echo ""

[ "$FORMAT" = yes ] && $MKFS $MKFS_OPT "$DEV"
mkdir di
mount -t $MKFS_TYPE "$DEV" di
if [ $? -ne 0 ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nFailed to mount '$DEV' as $MKFS_TYPENAME partition.\n" 0 0
  rm -r di
  exit 1
fi

if [ -d disk ]; then
  cp -a disk/* di 2>/dev/null
else
  if [ -n "$NFS" ]; then
    GEEXBOX="$NFS"
  else
    GEEXBOX="$CDROM/GEEXBOX"
  fi
  cp -a "$GEEXBOX" di/GEEXBOX 2>/dev/null
  mv di/GEEXBOX/boot/* di
  rm -f di/isolinux.bin di/isolinux.cfg di/boot.catalog
fi

grubprefix=/boot/grub
grubdir=di$grubprefix
device_map=$grubdir/device.map

rm -rf $grubdir	
mkdir -p $grubdir
tar xjf "di/GEEXBOX/usr/share/grub-i386-pc.tar.bz2" -C $grubdir

cp $grubdir/stage2 $grubdir/stage2_single

# Detect BIOS devices
$GRUB --batch --no-floppy --device-map=$device_map <<EOF
quit
EOF

rootdev=$(convert $DEV)

if [ -z "$rootdev" ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nCouldn't find my GRUB partition representation\n" 0 0
  umount di
  rm -r di
  exit 1
fi

$GRUB --batch --no-floppy --device-map=$device_map <<EOF
root $rootdev
install --stage2=$grubdir/stage2_single $grubprefix/stage1 $rootdev $grubprefix/stage2_single p /boot/grub/single.lst
EOF

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

if [ -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" ]; then
  splashimage="splashimage=$rootdev/GEEXBOX/usr/share/grub-splash.xpm.gz"
else
  splashimage=""
fi

if [ -n "$supported_os_list" ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a GeeXboX partition. To boot from it, you will need to install a bootloader. I can install one for you. If you have any other operating system on your computer, I will also install a multiboot for you. If you do not want me to install a new bootloader, you will need to configure yours alone.\nI have found: $supported_os_list\nDo you want to install me to install the boot loader (GRUB) for you ?\n" 0 0 && MBR=yes
else
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a GeeXboX partition. I didn't recognize any other OS on your system, want me to install boot loader on your MBR?\n" 0 0 && MBR=yes
fi

if [ "$MBR" = "yes" ]; then
  $GRUB --batch --no-floppy --device-map=$device_map <<EOF
root $rootdev
setup --stage2=$grubdir/stage2 --prefix=$grubprefix (hd0)
EOF

  cat > $grubdir/menu.lst <<EOF
default  0
timeout  5
color cyan/blue white/blue
$splashimage

EOF

  saveifs=$IFS
  IFS='
'
  for os in $oslist; do
    partition=$(echo "$os" | cut -d: -f1)
    grubpartition=$(convert $partition)
    
    if [ -z "$grubpartition" ]; then
	continue
    fi
    
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

  cat >> $grubdir/menu.lst <<EOF
title	GeeXboX
root	$rootdev
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME splash=silent vga=0x315 video=vesafb:ywrap,mtrr
initrd  /initrd.gz
boot

title	GeeXboX (debug)
root	$rootdev
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME debugging
initrd  /initrd.gz
boot
EOF

else
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --msgbox "\nYou must install a boot loader to boot GeeXboX\n" 0 0
fi

cat > $grubdir/single.lst <<EOF
default  0
timeout  2
color cyan/blue white/blue
$splashimage

title	GeeXboX
root	$rootdev
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME splash=silent vga=0x315 video=vesafb:ywrap,mtrr
initrd  /initrd.gz
boot

title	GeeXboX (debug)
root	$rootdev
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME debugging
initrd  /initrd.gz
boot
EOF

umount di
rm -r di

[ -n "$CDROM" ] && eject &

$DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Have Fun!" --msgbox "\nGeeXboX is now installed on '$DEV'\n" 0 0
