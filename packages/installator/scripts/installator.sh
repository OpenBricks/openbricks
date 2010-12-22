#!/bin/sh

LOGFILE=/tmp/install.log
BOOTDISK_MNT=/mnt/BootDisk

# Acts just like echo cmd, with automatic redirection
dbglg () {
  echo "$@" >> $LOGFILE
}

# Detect whether partition ($1) mounted at ($2) with type ($3) is microsoft.
detect_os_microsoft () {
  local LONGNAME

  if [ "$3" != ntfs -a "$3" != vfat -a "$3" != msdos -a "$3" != fuseblk ]; then
    return
  fi

  if [ -e "$2/ntldr" -a -e "$2/NTDETECT.COM" ]; then
    LONGNAME="Windows NT/2000/XP/Vista"
  elif [ -e "$2/windows/win.com" ]; then
    LONGNAME="Windows 95/98/Me"
  elif [ -e "$2/bootmgr" -a -d "$2/Boot" ]; then
    LONGNAME="Windows 7"
  elif [ -d "$2/dos" ]; then
    LONGNAME="MS-DOS 5.x/6.x/Win3.1"
  else
    return
  fi

  echo "$1:$LONGNAME:chain"
}

detect_os () {
  local PARTNAME PARTITION TYPE MPOINT

  mkdir -p tmpmnt

  for PARTNAME in `sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *[0-9][0-9][0-9]*\ \([a-z]*[0-9][0-9]*\)/\1/p" /proc/partitions`; do
    PARTITION="/dev/$PARTNAME"

    if ! grep -q "^$PARTITION " /proc/mounts; then
      if mount -o ro $PARTITION tmpmnt >/dev/null 2>&1; then
        TYPE=$(grep "^$PARTITION " /proc/mounts | cut -d " " -f 3)
        detect_os_microsoft $PARTITION tmpmnt $TYPE

        umount tmpmnt >/dev/null || return
      fi
    else
      MPOINT=$(grep "^$PARTITION " /proc/mounts | cut -d " " -f 2)
      TYPE=$(grep "^$PARTITION " /proc/mounts | cut -d " " -f 3)

      detect_os_microsoft $PARTITION $MPOINT $TYPE
    fi
  done

  rmdir tmpmnt
}

# Usage: convert os_device
# Convert an OS device to the corresponding GRUB drive.
# This part is OS-specific.
# -- taken from `grub-install`
# $1 is DEV
# $2 is DEVICE_MAP
convert () {
  local TMP_DRIVE TMP_DISK TMP_PART

  if test ! -e "$1"; then
    echo "$1: $MSG_DEV_NO_BLOCK" 1>&2
    exit 1
  fi

  TMP_DISK=`echo "$1" | sed -e 's%\([sh]d[a-z]\)[0-9]*$%\1%'`
  TMP_PART=`echo "$1" | sed -e 's%.*/[sh]d[a-z]\([0-9]*\)$%\1%'`

  TMP_DRIVE=`grep -v '^#' $2 | grep "$TMP_DISK *$" \
			| sed 's%.*\(([hf]d[0-9][a-g0-9,]*)\).*%\1%'`

  if [ -z "$TMP_DRIVE" ]; then
    echo "$1 $MSG_BIOS_NO_DRIVE" 1>&2
    exit 1
  fi

  if [ -n "$TMP_PART" ]; then
    # If a partition is specified, we need to translate it into the
    # GRUB's syntax.
    echo "$TMP_DRIVE" | sed "s%)$%,$(($TMP_PART)))%"
  else
    # If no partition is specified, just print the drive name.
    echo "$TMP_DRIVE"
  fi
}

# Create grub.conf file helper functions

# Replace original value ($2) with new value ($3) in GRUB config file ($1)
update_grub_conf_bootargs () {
  # don't try to set an item with a non-existing value
  test -z "$3" && return;
  sed -i "s/$2=[^ ]*/$2=$3/g" "$1"
}

# Use the replacement values ($2-$5) in GRUB config file ($1)
append_grub_conf () {
  local TMP_GRUB_CONF=/tmp/grub.cfg
  cp /etc/grub/grub.cfg $TMP_GRUB_CONF

  sed -i "s/_TITLE_/$2/" $TMP_GRUB_CONF
  sed -i "s/_DEBUG_/$4/" $TMP_GRUB_CONF
  sed -i "s/_CONFIG_/$5/" $TMP_GRUB_CONF

  [ "$4" = debugging ] && sed -i "s/loglevel=[0-9]//" $TMP_GRUB_CONF

  cat $TMP_GRUB_CONF >> $1
  echo "" >> $1
}

# Setup of GRUB config file using initial base config file ($1) and the
# boot device UUID ($2) and boot device GRUB descriptor (ROOTDEV as $3)
# and an option according to the type of FS ($4).
setup_grub () {
  local OPT_FS_TYPE

  append_grub_conf $1 "GeeXboX $VERSION " "" "" ""
  append_grub_conf $1 "GeeXboX $VERSION (debug)" "" debugging ""
  append_grub_conf $1 "GeeXboX $VERSION (reconfigure)" "" "" configure

  # put default options
  update_grub_conf_bootargs $1 lang `cmdline_default lang en`
  update_grub_conf_bootargs $1 remote `cmdline_default remote atiusb`
  update_grub_conf_bootargs $1 receiver `cmdline_default receiver atiusb`
  update_grub_conf_bootargs $1 keymap `cmdline_default keymap qwerty`

  # now setup installation specific options
  sed -i "s/_ROOTDEV_/$3/g" $1
  sed -i "s/_DEVNAME_/UUID=${2}/g" $1
}

# Returns the value to use for a given variable ($1) as was found
# in the boot arguments, otherwise returns a default value ($2)
cmdline_default () {
  local RET=`sed -n "s/.*$1=\([^ ]*\).*/\1/p" /proc/cmdline`
  test -z $RET && RET=$2
  echo $RET
}

# Select language definitions
setup_lang () {
  LANG=`cmdline_default lang en`
  . /etc/installator/en.install # default language file, always present
  [ -f /etc/installator/${LANG}.install ] && . /etc/installator/${LANG}.install
}

# Select keymap: Prompts users for available keymaps, and loads the selected one
setup_keymap () {
  local i 
  local KEYMAP_OLD=`cmdline_default keymap qwerty`
  local KEYMAPS="qwerty qwerty"
  for i in `ls /etc/keymaps`; do
    KEYMAPS="$KEYMAPS $i $i"
  done

  KEYMAP=`dialog --no-cancel --stdout \
    --backtitle "$BACKTITLE : $MSG_KEYMAP_CONFIG" --title "$MSG_KEYMAP" \
    --default-item $KEYMAP_OLD --menu "$MSG_KEYMAP_DESC" 0 0 0 $KEYMAPS ` \
    || exit 1

  test -f "/etc/keymaps/$KEYMAP" && loadkmap < "/etc/keymaps/$KEYMAP"
}

# Offer a list of possible disks on which to install to the user,
# and return with the selected disk name
choose_disk () {
  local DISK_LIST SELECTED_DISK SIZE VENDOR MODEL DISKNAME i
  while true; do
    DISK_LIST=""
    for i in `cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *[0-9][0-9]*\ \([a-z]*\)$/\1/p"`; do
      SIZE=`sfdisk -s /dev/$i | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
      VENDOR=`[ -f /sys/block/$i/device/vendor ] \
              && cat /sys/block/$i/device/vendor`
      MODEL=`[ -f /sys/block/$i/device/model ] \
             && cat /sys/block/$i/device/model`
      DISKNAME=`echo $VENDOR $MODEL ${SIZE}MB | sed 's/ /_/g'`
      DISK_LIST="$DISK_LIST $i $DISKNAME"
    done

    # add volume groups to list
    [ -x /sbin/lvm ] && for i in `lvm vgs --noheadings --separator ":" --nosuffix --units M | sed "s/^\ *//g"`; do
      SIZE=`echo $i | cut -d\: -f6 | sed "s/\.[0-9]*//g"`
      VENDOR="LVM"
      MODEL="Volume Group"
      VGNAME=`echo $i | cut -d\: -f1 | sed 's/ /_/g'`
      DISKNAME=`echo $VENDOR $MODEL ${SIZE}MB | sed 's/ /_/g'`
      DISK_LIST="$DISK_LIST $VGNAME $DISKNAME"
    done

    if [ -z "$DISK_LIST" ]; then
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
        --yesno "\n${MSG_DISK_NOT_FOUND}" 0 0 1>&2 || exit 1
    else
      DISK_LIST="$DISK_LIST $MSG_DISK_REFRESH"
      SELECTED_DISK=`dialog --stdout --backtitle "$BACKTITLE" \
                       --title "$MSG_DISK_DEVICE" \
                       --menu "\n${MSG_DISK_DEVICE_DESC}" 0 0 0 $DISK_LIST`
      [ -z "$SELECTED_DISK" ] && break
      [ $SELECTED_DISK != refresh ] && break
    fi
  done

  echo $SELECTED_DISK
}

# Prompt and get the desired partition name from the selected disk ($1),
# and returns the device name of that partition
choose_partition_dev () {
  local LOC_DISK="$1"
  local DEV_SEL DEV_LIST SIZE VENDOR MODEL DEVNAME i
  dbglg "Input arg for DISK is $1"
  while [ ! -b "$DEV_SEL" ]; do
    DEV_LIST=""
    for i in `sfdisk -lq /dev/$LOC_DISK 2>/dev/null | grep ${LOC_DISK%disc} | \
              cut -f1 -d' ' | grep dev`; do
      case `sfdisk --print-id ${i%%[0-9]*} ${i#${i%%[0-9]*}}` in
        1|11|6|e|16|1e|b|c|1b|1c|83)
          SIZE=`sfdisk -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
          VENDOR=`cat /sys/block/$LOC_DISK/device/vendor`
          MODEL=`cat /sys/block/$LOC_DISK/device/model`
          DEVNAME=`echo $VENDOR $MODEL ${SIZE}MB | sed 's/ /_/g'`
          DEV_LIST="$DEV_LIST $i $DEVNAME"
          ;;
      esac
    done
    
    [ -x /sbin/lvm ] && for i in `lvm lvs --noheadings --nosuffix --units M --separator ":" $LOC_DISK 2>/dev/null | sed "s/^\ *//g"`; do
      SIZE=`echo $i | cut -d\: -f4 | sed "s/\.[0-9]*//g"`
      VENDOR="LVM"
      MODEL="Logical Volume"
      LVNAME=`echo $i | cut -d\: -f1 | sed 's/ /_/g'`
      DEVNAME=`echo $VENDOR $MODEL ${SIZE}MB | sed 's/ /_/g'`
      DEV_LIST="$DEV_LIST /dev/${LOC_DISK}/${LVNAME} $DEVNAME"
    done

    if [ -z "$DEV_LIST" ]; then
      dbglg "DEV_LIST var empty!"
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
        --msgbox "\n$MSG_INSTALL_DEV_NOPART_BEGIN $MSG_DISK_PART $MSG_INSTALL_DEV_NOPART_END\n" 0 0 1>&2
      exit 1
    else
      DEV_SEL=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" \
        --title "$MSG_INSTALL_DEV_CONFIG" --menu "$MSG_INSTALL_DEV_DESC" \
        0 0 0 $DEV_LIST`
    fi
    [ -z "$DEV_SEL" ] && exit 1
    if [ ! -b "$DEV_SEL" ]; then
      dbglg "DEV_SEL $DEV_SEL is not a valid block device!"
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
        --msgbox "\n'$DEV_SEL' $MSG_INSTALL_DEV_BAD_BLOCK\n" 0 0 1>&2 \
        || exit 1
    fi
  done

  echo "$DEV_SEL"
}

# Runs configurator on device to configure ($1)
configure () {
  local IDISK=/mnt/install_disk
  mkdir -p $IDISK
  mount $1 $IDISK
  configurator
  umount $IDISK
}

# Try to guess current partition fs type of dev ($1).
guess_partition_type () {
  local type FS_TYPE=""
  for type in vfat ext4 ext3 ext2 auto; do
    if mount -o ro -t $type "$1" $BOOTDISK_MNT 2>/dev/null; then
      FS_TYPE=`grep "^$1 " /proc/mounts | cut -d " " -f 3`
      umount $BOOTDISK_MNT
      break
    fi
  done
  dbglg "guess_partition_type() returned \"$FS_TYPE\""
  echo $FS_TYPE
}

# Decides if a format is needed (or desired) and manages the process
# $1 is global MKFS_TYPE, and also updates MKFS_TYPE variable
# $2 is DEV
format_if_needed () {
  local NEED_FORMAT=yes
  local FORMAT_DEFAULT="--defaultno"
  local LOC_MKFS_TYPE="$1"
  local LOC_DEV="$2"
  local FORMAT_MSG FORMAT MKFS MKFS_OPT MKFS_TYPENAME FORMAT
  local SUPPORTED_TYPES PART_TYPE

  # Set valid FS types based on selected install partition
  if ( [ -x /sbin/lvm ] && lvm lvdisplay $LOC_DEV >/dev/null 2>&1 ); then
    SUPPORTED_TYPES="vfat ext4 ext3 ext2"
    PART_TYPE="LVM"
  else
    case `sfdisk --print-id ${LOC_DEV%%[0-9]*} ${LOC_DEV#${LOC_DEV%%[0-9]*}}` in
      1|11|6|e|16|1e|14|b|c|1b|1c)
        SUPPORTED_TYPES="vfat"
        PART_TYPE="FAT"
        ;;
      83) # Linux
        SUPPORTED_TYPES="ext4 ext3 ext2"
        PART_TYPE="Linux"
        ;;
    esac
  fi

  dbglg "SUPPORTED_TYPES $SUPPORTED_TYPES PART_TYPE $PART_TYPE"
  dbglg "LOC_MKFS_TYPE \"$LOC_MKFS_TYPE\" LOC_DEV \"$LOC_DEV\""

  if [ -z "$LOC_MKFS_TYPE" ]; then
    FORMAT_MSG="$MSG_INSTALL_DEV_NO_FORMAT"
    FORMAT_DEFAULT=""
  else
    for type in $SUPPORTED_TYPES; do
      [ $type = $LOC_MKFS_TYPE ] && NEED_FORMAT=no
    done

    if [ "$NEED_FORMAT" = yes ]; then
      FORMAT_MSG="$MSG_INSTALL_DEV_FORMAT_BEGIN ($LOC_MKFS_TYPE) $MSG_INSTALL_DEV_FORMAT_END ($PART_TYPE)."
      FORMAT_DEFAULT=""
    else
      FORMAT_MSG="$MSG_INSTALL_DEV_FORMATED"
    fi
  fi

  dialog --aspect 15 --backtitle "$BACKTITLE" \
    --title "$MSG_INSTALL_DEV_FORMAT" $FORMAT_DEFAULT \
    --yesno "${FORMAT_MSG}\n${MSG_INSTALL_DEV_FORMAT_DESC} '$LOC_DEV' ?\n" \
    0 0 1>&2 \
    && FORMAT=yes

  if [ "$FORMAT" = yes ]; then
    if ( [ -x /sbin/lvm ] && lvm lvdisplay $LOC_DEV >/dev/null 2>&1 ); then
      LOC_MKFS_TYPE=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" \
        --title "$MSG_INSTALL_PART_TYPE" --menu "$MSG_INSTALL_PART_TYPE_DESC"\
        0 0 0 ext2 "Linux ext2" ext3 "Linux ext3" ext4 "Linux ext4" vfat "Dos vfat"` \
        || exit 1
    else
      case `sfdisk --print-id ${LOC_DEV%%[0-9]*} ${LOC_DEV#${LOC_DEV%%[0-9]*}}` in
        1|11|6|e|16|1e|14) # FAT12 and FAT16
          LOC_MKFS_TYPE="vfat"
          ;;
        b|c|1b|1c) # FAT32
          LOC_MKFS_TYPE="vfat"
          ;;
        83) # Linux
          LOC_MKFS_TYPE=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" \
            --title "$MSG_INSTALL_PART_TYPE" --menu "$MSG_INSTALL_PART_TYPE_DESC"\
            0 0 0 ext2 "Linux ext2" ext3 "Linux ext3" ext4 "Linux ext4"` \
            || exit 1
          ;;
      esac
    fi
    case $LOC_MKFS_TYPE in
      vfat)
        MKFS=mkfs.vfat
        MKFS_OPT="-n GEEXBOX"
        LOC_MKFS_TYPE=vfat
        MKFS_TYPENAME="FAT"
        ;;
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
      ext4)
        MKFS=mke2fs
        MKFS_OPT="-L GEEXBOX -t ext4"
        MKFS_TYPENAME="Linux ext4"
    esac

    if [ -z "$MKFS" ]; then
      if [ "$NEED_FORMAT" = yes ]; then
        dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
          --msgbox "\n${MSG_INSTALL_DEV_NO_FORMAT} ('$LOC_DEV'). ${MSG_INSTALL_FORMAT_NO_TOOLS}\n" 0 0 1>&2
        rmdir $BOOTDISK_MNT
        exit 1
      else
        dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_WARNING"\
          --msgbox "\n'$LOC_DEV' $MSG_INSTALL_FORMAT_BAD_TYPE $MKFS_TYPENAME. ${MSG_INSTALL_FORMAT_NO_TOOLS}. ${MSG_INSTALL_FORMAT_ALREADY}\n" 0 0 1>&2 \
          || exit 1
      fi
    else
      dbglg "$MKFS $MKFS_OPT \"$LOC_DEV\""
      dialog --backtitle "$BACKTITLE" \
        --infobox "$MSG_INSTALL_DEV_FORMATTING_WAIT_BEGIN '$LOC_DEV'$MSG_INSTALL_DEV_FORMATTING_WAIT_END" 0 0
      $MKFS $MKFS_OPT "$LOC_DEV" >> $LOGFILE 2>&1
    fi

  elif [ "$NEED_FORMAT" = yes ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
      --msgbox "\n${MSG_INSTALL_DEV_NO_FORMAT} ('$LOC_DEV')\n" 0 0 1>&2
    rmdir $BOOTDISK_MNT
    exit 1
  fi

  dbglg "format_if_needed() returned \"$LOC_MKFS_TYPE\""
  # Update the global variable
  MKFS_TYPE="$LOC_MKFS_TYPE"
}

#FIXME: function's obsolete now
# Get the uuid of the device given by input $1
get_uuid () {
  local DEV_REALNAME NAME LOC_DEV LOC_UUID

  # restart UDEV scan to get device UUID if
  # user just created/formatted a new disk/partition
  udevadm trigger
  udevadm settle --timeout=180

  DEV_REALNAME=`echo ${1##/dev/}`
  for LOC_DEV in `ls /dev/disk/by-uuid/*`; do
    NAME=`ls -l "$LOC_DEV" | sed "s/.*-> \(.*\)/\1/" | sed 's%../../%%'`
    if [ "$NAME" = "$DEV_REALNAME" ]; then
      LOC_UUID="`echo $LOC_DEV | sed 's%/dev/disk/by-uuid/%%'`"
      dbglg "get_uuid() returned \"$LOC_UUID\""
      DEV_UUID="$LOC_UUID"
      break
    fi
  done
}

# Installs and configures the GRUB bootloader
# $1 is DEV
# $2 is MKFS_TYPE
install_grub (){
  local ROOTDEV
  local MBRDEV
  local PARTDEV
  local GRUBPREFIX=/boot/grub
  local GRUBDIR=$BOOTDISK_MNT/$GRUBPREFIX
  local DEVICE_MAP=$GRUBDIR/device.map
  local SPLASHIMAGE="grub-splash.png"
  local LOC_DEV=$1
  local LOC_MKFS_TYPE=$2
  local ARCH=i386-pc

  TMP_DISK=`echo "$LOC_DEV" | sed -e 's%\([sh]d[a-z]\)[0-9]*$%\1%'`
  TMP_DISKNAME="${TMP_DISK#/dev/}"

  rm -rf $GRUBDIR
  mkdir -p $GRUBDIR

  [ -f "$BOOTDISK_MNT/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" ] \
    && tar xaf "$BOOTDISK_MNT/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" -C /usr \
    >> $LOGFILE 2>&1

  [ -f "$BOOTDISK_MNT/GEEXBOX/usr/share/${SPLASHIMAGE}" ] && cp -f "$BOOTDISK_MNT/GEEXBOX/usr/share/${SPLASHIMAGE}" $GRUBDIR 

  dbglg "grub-mkdevicemap --no-floppy --device-map=$DEVICE_MAP"
  grub-mkdevicemap --no-floppy --device-map=$DEVICE_MAP \
      >> $LOGFILE 2>&1
  ROOTDEV=$(grub-probe --target=drive --device-map=${DEVICE_MAP} $BOOTDISK_MNT)

  dbglg "ROOTDEV \"$ROOTDEV\" DEV \"$LOC_DEV\""

  if [ -z "$ROOTDEV" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
      --msgbox "\n${MSG_GRUB_NO_ROOTDEV}\n" 0 0 1>&2
    umount $BOOTDISK_MNT && rmdir $BOOTDISK_MNT
    exit 1
  fi

  DEV_UUID=$(grub-probe --target=fs_uuid --device-map=${DEVICE_MAP} $BOOTDISK_MNT)
  BOOT_DRV="(UUID=${DEV_UUID})"

  setup_grub $GRUBDIR/grub.cfg $DEV_UUID $BOOT_DRV $LOC_MKFS_TYPE

  dbglg "boot drive: $BOOT_DRV"

  dbglg "*** Start GRUB grub.cfg ***"
  cat $GRUBDIR/grub.cfg >> $LOGFILE
  dbglg "*** End GRUB grub.cfg ***"

  # Detect others OS and ask for MBR only in the case where GeeXboX
  # is not installed on a removable device.
  # Note: lvm is not recognized as removable no matter what it's installed on!
  # FIXME: should be possible to have lvm boot on removable device as well, but with less priority
  if [ ! -f /sys/block/$TMP_DISKNAME/removable -o "`cat /sys/block/$TMP_DISKNAME/removable 2>&1`" = 0 ]; then
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
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" \
        --defaultno --yesno "\n'$LOC_DEV' $MSG_LOADER_MULTIBOOT_BEGIN $supported_os_list\n${MSG_LOADER_MULTIBOOT_END}\n" 0 0 1>&2 \
        && MBR=yes
    else
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" \
        --yesno "\n'$LOC_DEV' ${MSG_LOADER_NONE}\n" 0 0 1>&2 \
        && MBR=yes
    fi

    PARTDEV=$(grub-probe --target=drive --device-map=${DEVICE_MAP} $BOOTDISK_MNT)
    MBRDEV="(hd0)"
  else
    oslist=
    MBR=yes
    PARTDEV=$(grub-probe --target=drive --device-map=${DEVICE_MAP} $BOOTDISK_MNT)
    MBRDEV=$(echo $PARTDEV | sed 's/,[0-9]*//g')
  fi

  # install to boot record of geexbox partition either way
  # order is critical, must be: other, disk, fs, partmap, devabstraction
  dbglg "grub probing..."
  MOD_OTHER="fs_uuid"
  MOD_DISK="biosdisk"
  MOD_FS=$(grub-probe --target=fs --device-map=${DEVICE_MAP} $BOOTDISK_MNT 2>>$LOGFILE)
  MOD_PARTMAP=$(grub-probe --target=partmap --device-map=${DEVICE_MAP} $BOOTDISK_MNT 2>>$LOGFILE)
  MOD_DEVABSTRACTION=$(grub-probe --target=abstraction --device-map=${DEVICE_MAP} $BOOTDISK_MNT 2>>$LOGFILE)
  MODULES="$MOD_DISK $MOD_FS $MOD_PARTMAP $MOD_DEVABSTRACTION $MOD_OTHER"
  dbglg "grub disk: $MOD_DISK"
  dbglg "grub fs: $MOD_FS"
  dbglg "grub dev abstraction: $MOD_DEVABSTRACTION"
  dbglg "grub partmap: $MOD_PARTMAP"
  dbglg "grub modules: $MODULES"
  dbglg "cp -R /usr/lib/grub/fonts /usr/lib/grub/${ARCH}/* ${GRUBDIR}/"
  cp -R /usr/lib/grub/fonts /usr/lib/grub/${ARCH}/* ${GRUBDIR}/


  if [ "$MBR" = "yes" ]; then
    if ! ( dbglg "grub-mkimage --output=${GRUBDIR}/core.img --prefix=${BOOT_DRV}${GRUBPREFIX} $MODULES" && \
    grub-mkimage --output=${GRUBDIR}/core.img --prefix="${BOOT_DRV}${GRUBPREFIX}" $MODULES \
      >>$LOGFILE 2>&1 && \
    dbglg "grub-setup --device-map=${DEVICE_MAP} --directory=${GRUBDIR} $MBRDEV" && \
    grub-setup --device-map=${DEVICE_MAP} --directory=${GRUBDIR} $MBRDEV \
      >> $LOGFILE 2>&1 ); then
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --msgbox "$MSG_GRUB_SETUP_ERROR" 0 0
      dialog --textbox $LOGFILE 0 0
      exit 1
    fi
  else #try to install into partition
    # workaround needed
    ln -s ${GRUBDIR} ${GRUBPREFIX}

    if ! ( dbglg "grub-mkimage --output=${GRUBDIR}/core.img --prefix=${BOOT_DRV}${GRUBPREFIX} $MODULES" && \
    grub-mkimage --output=${GRUBDIR}/core.img --prefix="${BOOT_DRV}${GRUBPREFIX}" $MODULES \
      >>$LOGFILE 2>&1 && \
    dbglg "grub-setup --device-map=${DEVICE_MAP} --directory=${GRUBDIR} $PARTDEV" && \
    grub-setup --force --device-map=${DEVICE_MAP} --directory=${GRUBPREFIX} $PARTDEV \
      >> $LOGFILE 2>&1 ); then
      dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --msgbox "$MSG_GRUB_SETUP_ERROR" 0 0
      dialog --textbox $LOGFILE 0 0
      exit 1
    fi
  fi

    cat >$GRUBDIR/grub.cfg <<EOF
set default=0
set timeout=5
set menu_color_normal=cyan/blue
set menu_color_highlight=white/blue

if [ -f /boot/grub/$SPLASHIMAGE ]; then
  if loadfont /boot/grub/fonts/8x13.pf2 ; then
    set gfxmode="640x480@24;640x480@32;640x480@16"
    insmod gfxterm
    insmod vbe
    insmod png
    terminal_output gfxterm
    if background_image /boot/grub/$SPLASHIMAGE ; then
        set menu_color_normal=cyan/black
        set menu_color_highlight=white/black
    fi
  fi
fi

EOF

    saveifs=$IFS
    IFS='
'
    for os in $oslist; do
      partition=$(echo "$os" | cut -d: -f1)
      grubpartition=$(convert $partition $DEVICE_MAP)

      [ -z "$grubpartition" ] && continue

      grubdisk=${grubpartition%,*}
      grubdisk="(${grubdisk#(})"
      title=$(echo "$os" | cut -d: -f2)
      type=$(echo "$os" | cut -d: -f3)

      if [ "$type" = chain ]; then
        cat >> $GRUBDIR/grub.cfg <<EOF
menuentry "$title" {
EOF
        if [ $grubdisk != "(hd0)" ]; then
          cat >> $GRUBDIR/grub.cfg <<EOF
    drivemap (hd0) $grubdisk
    drivemap $grubdisk (hd0)
EOF
        fi

        cat >> $GRUBDIR/grub.cfg <<EOF
    set root=$grubpartition
    parttool $grubpartition boot+
    parttool $grubpartition hidden-
    chainloader +1
}

EOF
      fi
  done
  IFS=$saveifs

  setup_grub $GRUBDIR/grub.cfg $DEV_UUID $BOOT_DRV $LOC_MKFS_TYPE

  dbglg "*** Start GRUB grub.cfg ***"
  cat $GRUBDIR/grub.cfg >> $LOGFILE
  dbglg "*** End GRUB grub.cfg ***"

  if [ "$MBR" != "yes" ]; then
    # fixme: i18n strings should be updated. in fact, if not installed into mbr, geexbox is installed into respective partition.
    # so, if this is the first one and is bootable, booting works fine. otherwise, the (maybe) existing bootloader just has to be configured to chainload
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" \
      --msgbox "\n${MSG_LOADER_ERROR}\n" 0 0 1>&2
  fi

  # Special care for removable devices
  # if one intend to boot from removable disk, it'll be considered as
  # primary disk for BIOS
  if [ "`cat /sys/block/$TMP_DISKNAME/removable 2>/dev/null`" = 1 ]; then
     for file in grub.cfg; do
       [ -f "$GRUBDIR/$file" ] && sed -i 's%(hd[0-9],%(hd0,%g' "$GRUBDIR/$file"
     done
  fi
}

VERSION=`cat VERSION`
BACKTITLE="GeeXboX $VERSION installator"

# should not be present in install mode, but in case of ...
initctl stop automountd >/dev/null 2>&1
killall -9 automountd >/dev/null 2>&1

setup_lang

# disable kernel messages to avoid screen corruption
echo 0 > /proc/sys/kernel/printk

setup_keymap

DISK="`choose_disk`"
[ -z "$DISK" ] && exit 1

# Make sure disk partitions are not already mounted in case it's no VG
if ( [ -x /sbin/lvm ] && vgdisplay /dev/$DISK >/dev/null 2>&1 ); then
  umount /dev/$DISK/* 2>/dev/null
else
  umount /dev/${DISK}* 2>/dev/null
fi
for d in /mnt/*; do rmdir $d >/dev/null 2>&1; done

# Create directory for the install partition to be mounted
mkdir -p $BOOTDISK_MNT

CFDISK_MSG="$MSG_CFDISK_BEGIN $MSG_DISK_PART $MSG_CFDISK_END"

# Guide user on how to setup with cfdisk tool in the next step only if no VG was selected
if ( ! [ -x /sbin/lvm ] || ! lvm vgdisplay /dev/$DISK >/dev/null 2>&1 ); then
  dialog --stdout --backtitle "$BACKTITLE" --title "$MSG_INSTALL_DEV_CONFIG" \
    --msgbox "$CFDISK_MSG" 0 0 \
    || exit 1

  cfdisk /dev/$DISK
fi

DEV="`choose_partition_dev $DISK`"
[ -z "$DEV" ] && exit 1

MKFS_TYPE="`guess_partition_type $DEV`"

format_if_needed "$MKFS_TYPE" "$DEV"

# Attempt to mount the prepared partition using the given partition fs type
dbglg "mount -t $MKFS_TYPE $DEV $BOOTDISK_MNT"
mount -t $MKFS_TYPE "$DEV" $BOOTDISK_MNT
ret=$?
if [ $ret -ne 0 ]; then
  # FS is not mountable! Return an error msg and exit
  dbglg "mount returned $ret"
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" \
    --msgbox "\n${MSG_INSTALL_MOUNT_FAILED} '$DEV' ($MKFS_TYPENAME).\n" 0 0
  rmdir $BOOTDISK_MNT
  exit 1
fi

dialog --backtitle "$BACKTITLE" --infobox "$MSG_INSTALLING_WAIT" 0 0

# Cleanup if was left in a messy state previously- remove previous installs
rm -rf $BOOTDISK_MNT/GEEXBOX 2>&1 >> $LOGFILE

# Copy the main files to the install partition
OS_RELEASE=$(uname -r)
INITRD_ELEMS="/bin/busybox /bin/mount /bin/umount /etc/profile /lib/ /bin/sh /sbin/init /sbin/blkid /sbin/modprobe /sbin/udevd /sbin/udevadm /linuxrc /lib/modules/${OS_RELEASE}/modules.dep /lib/modules/${OS_RELEASE}/kernel/drivers/md/dm-mod.ko"
INITRD_ELEMS="$INITRD_ELEMS $(find /etc/udev)"
INITRD_ELEMS="$INITRD_ELEMS $(find /lib -maxdepth 1)"
INITRD_ELEMS="$INITRD_ELEMS $(find /lib/udev)"
INITRD_ELEMS="$INITRD_ELEMS $(find /usr/lib/ -maxdepth 1 -name libdevmapper\*)"
INITRD_ELEMS="$INITRD_ELEMS $(find /usr/lib/ -maxdepth 1 -name libgcc\*)"
[ -x /bin/ldd ]  && INITRD_ELEMS="$INITRD_ELEMS /bin/ldd"
[ -x /sbin/lvm ] && INITRD_ELEMS="$INITRD_ELEMS /sbin/lvm"

#TODO: not every empty directory is needed on initial ramdisk. List of needed directories probably depends on included software, so maybe some of these should be created on startup of the very daemon?
EMPTY_DIRS="/proc /dev /dev/shm /sys /etc /tmp /bin /usr /usr/lib /usr/bin /sbin /usr/sbin /var /var/log /var/lib /var/lib/dbus /var/run /var/run/dbus /lib /lib/modules /lib/modules/${OS_RELEASE} /lib/modules/${OS_RELEASE}/kernel /lib/modules/${OS_RELEASE}/kernel/drivers /lib/modules/${OS_RELEASE}/kernel/drivers/md /root"

DISK_ELEMS="/bin /etc /lib /firmware /usr /codecs /sbin /dev"

for e in $EMPTY_DIRS $INITRD_ELEMS; do
  echo "$e" >>/tmp/initrd.install
done
for e in $EMPTY_DIRS; do
  mkdir -p $BOOTDISK_MNT/GEEXBOX/$e
done
for e in $DISK_ELEMS; do
  find $e -xdev | sed 's/^\///g' | cpio -pd $BOOTDISK_MNT/GEEXBOX 2>&1 | grep -v "newer or same age file exists" >> $LOGFILE
done
# Fix install with cdrom
CDROM=`grep boot=cdrom /proc/cmdline`
if [ -n "$CDROM" ] ; then 
 mkdir -p /mnt/cd 
 mount /dev/cdrom /mnt/cd
 mkdir -p $BOOTDISK_MNT/boot 
 cp /mnt/cd/GEEXBOX/boot/vmlinuz $BOOTDISK_MNT/boot/
 umount /mnt/cd
else
 mkdir -p $BOOTDISK_MNT/boot && cp /boot/vmlinuz $BOOTDISK_MNT/boot/
fi

# opkg
mkdir -p $BOOTDISK_MNT/GEEXBOX/var/lib/opkg/info
cp /var/lib/opkg/info/* $BOOTDISK_MNT/GEEXBOX/var/lib/opkg/info
cp /var/lib/opkg/status $BOOTDISK_MNT/GEEXBOX/var/lib/opkg/        

cat /tmp/initrd.install | sed 's/^\///g' | cpio -o -H newc | gzip -9 > $BOOTDISK_MNT/boot/initrd.gz


install_grub "$DEV" "$MKFS_TYPE"

# Softlink grub.cfg
rm -f $BOOTDISK_MNT/GEEXBOX/etc/grub/grub.cfg
ln -s /boot/grub/grub.cfg $BOOTDISK_MNT/GEEXBOX/etc/grub/grub.cfg

# Remove unneeded boot dir from mounted install drive
rm -rf $BOOTDISK_MNT/GEEXBOX/boot

# Eject CD if it was the boot media
[ -n "$CDROM" ] && eject -s /dev/cdrom &

# Prompt to view logging file
dialog --aspect 15 --backtitle "$BACKTITLE" \
                             --title "$MSG_LOG" --defaultno \
                             --yesno "$MSG_LOG_DESC" 0 0 \
                             && dialog --textbox $LOGFILE 0 0

# Prompt to run the configurator to adjust the config of the new installation
dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_SUCCESS" \
  --yesno "\n${MSG_SUCCESS_DESC_BEGIN} '$DEV' !! ${MSG_SUCCESS_DESC_END}\n" \
  0 0 \
  && echo >/var/do_configure

# Cleanup
umount $BOOTDISK_MNT && rmdir $BOOTDISK_MNT

# Exit cleanly
return 0
