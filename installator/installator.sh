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
  DIALOG=`which dialog`
  CFDISK=`which cfdisk`
  SFDISK=`which sfdisk`
  MKDOSFS=`which mkdosfs`
  SYSLINUX=`which syslinux`
fi
VERSION=`cat VERSION`
BACKTITLE="GeeXboX $VERSION installator"

if [ "$UID" != "0" ]; then
  echo ""
  echo "**** You need to be root to install GeeXboX ****"
  echo ""
  exit 1
fi

if [ -z "$SFDISK" -o -z "$SYSLINUX" -o -z "$DIALOG" ]; then
    echo ""
    echo "**** You need to have syslinux, sfdisk and dialog installed to install GeeXboX ****"
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
        CFDISK_MSG="You can now edit your partition table to create a FAT16 partition (type=06). Be careful to choose the right disk! We won't take responsibility for any data loss."
      fi
      DISK=`$DIALOG --stdout --backtitle "$BACKTITLE" --title "Installation device" --menu "\nYou are going to install GeeXboX. For this you will need a PRIMARY FAT16 partition (hdX1 to hdX4) with about 8 MB of free space (max. 1 GB). It WON'T work with FAT32 or ext2 partitions.\n$CFDISK_MSG" 0 0 0 $DISKS` || exit 1
      [ $DISK != refresh ] && break
    fi
done
if [ -n "$CFDISK" ]; then
    $CFDISK /dev/$DISK || exit 1
fi

while [ ! -b "$DEV" ]; do
    DISKS=""
    for i in `$SFDISK -l /dev/$DISK | grep FAT16 | grep ${DISK%disc} | cut -f1 -d' '`; do
      S=`$SFDISK -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
      DISKS="$DISKS $i ${S}MB"
    done
    if [ -z "$DISKS" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYou don't have any FAT16 partitions on your system. Please create a FAT16 partition first using for example cfdisk.\n" 0 0
      exit 1
    else
      DEV=`$DIALOG --stdout --aspect 15 --backtitle "$BACKTITLE" --title "Installation device" --menu "Where do you want to install GeeXboX?" 0 0 0 $DISKS` || exit 1
    fi
    if [ ! -b "$DEV" ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\n'$DEV' is not a valid block device.\n" 0 0
    fi
done

if [ -z "$MKDOSFS" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Warning" --msgbox "\n'$DEV' needs to be a FAT16 partition. As you don't have mkdosfs installed, I won't be able to format the partition. Hopefully it is already formatted.\n" 0 0
else
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Formatting" --defaultno --yesno "\nDo you want to format '$DEV' in FAT16?\n" 0 0 && FORMAT=yes
fi
echo ""

[ "$FORMAT" = yes ] && $MKDOSFS -n GEEXBOX "$DEV"
mkdir di
mount -t vfat "$DEV" di
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
  rm di/isolinux.bin
fi
sed "s/boot=cdrom/boot=${DEV#/dev/}/" di/isolinux.cfg > di/syslinux.cfg
rm di/isolinux.cfg
umount di
$SYSLINUX "$DEV"

mount -t vfat "$DEV" di
dd if="$DEV" of=di/geexbox.lnx count=1 bs=512
umount di
rmdir di

$DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a bootable partition. To boot from it, you will need to install a bootloader. If you don't have any other operating systems on this hard disk, I can install a bootloader for you. Else, you will need to configure yourself a boot loader, such as LILO or GRUB.\n\nDo you want to install a single system bootloader?\n" 0 0 && MBR=yes

PART="${DEV#${DEV%%[0-9]*}}"
if [ "$MBR" = yes ]; then
  if [ -f mbr.bin ]; then
    dd if=mbr.bin of="/dev/$DISK"
  elif [ -f /usr/share/syslinux/mbr.bin ]; then
    dd if=/usr/share/syslinux/mbr.bin of="/dev/$DISK"
  fi
  echo ",,,*" | $SFDISK "/dev/$DISK" -N$PART
else
  GRUBDISK=`echo $DISK | sed 's/.*\(.\)$/\1/ ; y/abcdefghij/0123456789/'`
  GRUBPART=`echo $PART | sed 'y/12345678/01234567/'`
  GRUBDEV="(hd$GRUBDISK,$GRUBPART)"

  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --msgbox "\nYou can configure LILO to boot GeeXboX simply by adding these lines at the end of your /etc/lilo.conf :\n    other=$DEV\n          label=GeeXboX\nDon't forget to execute lilo after doing this modification.\n\nOr if you use GRUB, add something along these lines to your /boot/grub/menu.lst:\n    title GeeXboX\n        rootnoverify $GRUBDEV\n        chainloader /geexbox.lnx\n\nWindows users must copy geexbox.lnx to their C:\ drive and add the\nfollowing line to the boot.ini file to use with the NT Loader.\n    c:\geexbox.lnx=\"GeeXboX\"\n\nOr you can have a look at a separate boot loader such as XOSL (http://www.ranish.com/part/xosl.htm)." 0 0
fi

[ -n "$CDROM" ] && eject &

$DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Have Fun!" --msgbox "\nGeeXboX is now installed on '$DEV'\n" 0 0
