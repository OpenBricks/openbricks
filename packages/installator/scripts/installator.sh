#!/bin/sh

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

# Configure TV card and scan for channels.
setup_tvscan () {
  MPTV="mplayer tv:// -really-quiet -msglevel tv=4 -ao null -vo null"
  title="$BACKTITLE : $MSG_TV_CONFIG"

  INPUTS=`$MPTV -frames 0 2>/dev/null | grep "inputs:" | sed -e 's/ inputs: //' -e 's/= //g' -e 's/;//g'`

  NORMS=`$MPTV -frames 0 2>/dev/null | grep "supported norms:" | sed -e 's/ supported norms: //' -e 's/= //g' -e 's/;//g'`

  CHANLISTS="us-bcast '' us-cable '' us-cable-hrc '' japan-bcast '' japan-cable '' europe-west '' europe-east '' italy '' newzealand '' australia '' ireland '' france '' china-bcast '' southafrica '' argentina '' russia ''"

  while [ -z "$DONE" ]; do
    INPUT=`dialog --no-cancel --aspect 15 --stdout --backtitle "$title" --title "$MSG_TV_INPUT" --menu "\n${MSG_TV_INPUT_DESC}\n" 0 0 0 $INPUTS`

    NORM=`dialog --no-cancel --aspect 15 --stdout --backtitle "$title" --title "$MSG_TV_NORM" --menu "\n${MSG_TV_NORM_DESC}\n" 0 0 0 $NORMS`

    CHANLIST=`dialog --no-cancel --aspect 15 --stdout --backtitle "$title" --title "$MSG_TV_CHANLIST" --menu "\n${MSG_TV_CHANLIST_DESC}\n" 0 0 0 $CHANLISTS`

    CHANNELS_MPLAYER_PARAM=`$MPTV -tvscan autostart -frames 600 -tv driver=v4l2:input=$INPUT:norm=$NORM:chanlist=$CHANLIST 2>/dev/null | grep "^channels="`  | dialog --no-cancel --aspect 15 --stdout --backtitle "$title" --title "$MSG_TV_SCAN" --gauge "\n${MSG_TV_SCAN_DESC}\n" 0 0

    CHANNELS=`echo $CHANNELS_MPLAYER_PARAM | sed -e 's/channels=//g' -e 's/-/ - /g' -e 's/,/\\\\n/g' -e 's/$/\\\\n/g'`
    dialog --aspect 12 --stdout --yes-label "$MSG_TV_ACCEPT" --no-label "MSG_TV_RETRY" --backtitle "$title" --title "$MSG_TV_SCAN_DONE" --yesno "\n${MSG_TV_SCAN_DONE_DESC}\n\n$CHANNELS" 0 0 && DONE=true
  done

  [ `echo $CHANNELS_MPLAYER_PARAM | grep -c "channels="` -eq 1 ] && echo "tv=$CHANNELS_MPLAYER_PARAM" >> /etc/mplayer/mplayer.conf
  sed -i "s/^TVIN_STANDARD=.*/TVIN_STANDARD=$NORM/" $1/etc/tvcard
  sed -i "s/^CHANLIST=.*/CHANLIST=$CHANLIST/" $1/etc/tvcard
}

# Configure DVB card and scan for channels.
dvb_do_scan() {
  # Scan FreeToAir channels only
  dvbscan -x 0 "$1" > "$2" 2> /dev/null
}

setup_dvbscan () {
  DVB_LIST=/usr/share/dvb

  TITLE="$BACKTITLE : $MSG_DVB_CONFIG"
  CHANNELS_CONF="$1/etc/mplayer/channels.conf"

  [ -f /usr/share/dvb.tar.lzma -a ! -d $DVB_LIST ] && tar xaf /usr/share/dvb.tar.lzma -C /usr/share

  DVB_TYPE=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_DVB_TYPE" --menu "\n${MSG_DVB_TYPE_DESC}\n" 0 0 0 dvb-s "$MSG_DVB_SAT" dvb-t "$MSG_DVB_TER" dvb-c "$MSG_DVB_CABLE" atsc "$MSG_DVB_ATSC"`

  # DVB Terrestrial cards
  if [ $DVB_TYPE = "dvb-t" -o $DVB_TYPE = "dvb-c" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      COUNTRIES="$COUNTRIES $i ''"
    done

    COUNTRY=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_DVB_COUNTRY" --menu "\n${MSG_DVB_COUNTRY_DESC}\n" 0 0 0 $COUNTRIES`

    for i in `ls $DVB_LIST/$DVB_TYPE/$COUNTRY`; do
      CITIES="$CITIES $i ''"
    done

    CITY=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_DVB_CITY" --menu "\n${MSG_DVB_CITY_DESC}\n" 0 0 0 $CITIES`

    dvb_do_scan "$DVB_LIST/$DVB_TYPE/$COUNTRY/$CITY" "$CHANNELS_CONF"
  elif [ $DVB_TYPE = "dvb-s" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      SATS="$SATS $i ''"
    done

    SAT=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_DVB_SAT_SEL" --menu "\n${MSG_DVB_SAT_SEL_DESC}\n" 0 0 0 $SATS`

    dvb_do_scan "$DVB_LIST/$DVB_TYPE/$SAT" "$CHANNELS_CONF"
  elif [ $DVB_TYPE = "atsc" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      ATSC="$ATSC $i ''"
    done

    FREQ=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_DVB_ATSC_SEL" --menu "\n${MSG_DVB_ATSC_SEL_DESC}" 0 0 0 $ATSC`

    dvb_do_scan "$DVB_LIST/$DVB_TYPE/$FREQ" "$CHANNELS_CONF"
  fi

  if [ -s $CHANNELS_CONF ]; then
    # remove non-coherent detected channels
    grep -v "^\[.*\]:" $CHANNELS_CONF > /tmp/channels.conf
    mv /tmp/channels.conf $CHANNELS_CONF
  fi
}

# Configure X.Org
setup_xorg () {
  XORG_CONFIG=/usr/bin/xorgconfig

  TITLE="$BACKTITLE : $MSG_XORG_CONFIG"
  DRIVERS_FILE="$1/etc/X11/drivers"
  USER_RESOLUTION_LABEL="custom"
  USER_RESOLUTION_AUTO="auto"
  USER_DRIVERS_AUTO="auto"
  X_CFG_SAMPLE="$1/etc/X11/X.cfg.sample"
  X_CFG="$1/etc/X11/X.cfg"

  # retrieve current X settings
  test -f $X_CFG_SAMPLE && . $X_CFG_SAMPLE
  test -f $X_CFG && . $X_CFG

  OLD_RES=auto
  if [ "$XORG_RESX" != auto -a "$XORG_RESY" != auto ]; then
    OLD_RES="${XORG_RESX}x${XORG_RESY}"
  fi
  
  RES=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_XORG_RES" --inputbox "\n${MSG_XORG_RES_DESC}\n" 0 0 $OLD_RES`

  if [ "$RES" = "$USER_RESOLUTION_AUTO" ]; then
    NEW_RESX="auto"
    NEW_RESY="auto"
  else
    NEW_RESX=`echo $RES | sed 's%\(.*\)x.*%\1%'`
    NEW_RESY=`echo $RES | sed 's%.*x\(.*\)%\1%'`
  fi

  NEW_RATE=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_XORG_RATE" --inputbox "\n${MSG_XORG_RATE_DESC}\n" 0 0 $XORG_RATE`

  DRIVERS="$USER_DRIVERS_AUTO ''"
  for i in `cat $DRIVERS_FILE`; do
    DRIVERS="$DRIVERS $i ''"
  done

  NEW_DRIVER=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_XORG_DRIVER" --menu "\n${MSG_XORG_DRIVER_DESC}\n" 0 0 0 $DRIVERS`

  NEW_HORIZSYNC=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_XORG_HORIZSYNC" --inputbox "\n${MSG_XORG_HORIZSYNC_DESC}\n" 0 0 $XORG_HORIZSYNC`

  NEW_VERTREFRESH=`dialog --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "$MSG_XORG_VERTREFRESH" --inputbox "\n${MSG_XORG_VERTREFRESH_DESC}\n" 0 0 $XORG_VERTREFRESH`

  # write settings to config file
  echo "XORG_RESX=\"$NEW_RESX\"" > $X_CFG
  echo "XORG_RESY=\"$NEW_RESY\"" >> $X_CFG
  echo "XORG_RATE=\"$NEW_RATE\"" >> $X_CFG
  echo "XORG_DRIVER=\"$NEW_DRIVER\"" >> $X_CFG
  echo "XORG_HORIZSYNC=\"$NEW_HORIZSYNC\"" >> $X_CFG
  echo "XORG_VERTREFRESH=\"$NEW_VERTREFRESH\"" >> $X_CFG

  cp $X_CFG /etc/X11 # for xorgconfig to work with new params
  $XORG_CONFIG > /dev/null 2>&1 # create xorg.conf file
  cp /etc/X11/xorg.conf $1/etc/X11/ # save back new xorg.conf to HDD
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
  sed -i "s/_DEVNAME_/$DEVNAME/g" $1
}

# Choose default language
setup_lang () {
  LANGS=`ls di/GEEXBOX/etc/mplayer/*.lang | sed -e 's$di/GEEXBOX/etc/mplayer/\(.*\).lang$\1$g'`
  for l in $LANGS; do
    LLANGS="$LLANGS $l $l"
  done
  MENU_LANG=`dialog --no-cancel --stdout --backtitle "$BACKTITLE : $MSG_LANG_CONFIG" --title "$MSG_LANG" --default-item $LANG --menu "$MSG_LANG_DESC" 0 0 0 $LLANGS` || exit 1
}

cmdline_default () {
  RET=`sed -n "s/.*$1=\([^ ]*\).*/\1/p" /proc/cmdline`
  test -z $RET && RET=$2
  echo $RET
}

# Choose default remote
setup_remote () {
  REMOTE_OLD=`cmdline_default remote atiusb`

  REMOTES=`ls di/GEEXBOX/etc/lirc/lircrc_* | sed -e 's/.*lircrc_//g'`
  for r in $REMOTES; do
   LREMOTES="$LREMOTES $r $r"
  done
  REMOTE=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_REMOTE" --default-item $REMOTE_OLD --menu "$MSG_REMOTE_DESC" 000 0 0 $LREMOTES`
}

# Choose default receiver
setup_receiver () {
  RECEIVER_OLD=`cmdline_default receiver atiusb`

  RECEIVERS=`ls di/GEEXBOX/etc/lirc/lircd_* | grep -v ".conf" | sed -e 's/.*lircd_//g'`
  for r in $RECEIVERS; do
    LRECEIVERS="$LRECEIVERS $r $r"
  done
  RECEIVER=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_RECEIVER" --default-item $RECEIVER_OLD --menu "$MSG_RECEIVER_DESC" 000 0 0 $LRECEIVERS`
}

# Select keymap
setup_keymap () {
  KEYMAP_OLD=`cmdline_default keymap qwerty`

  KEYMAPS="qwerty qwerty"
  for i in `ls /etc/keymaps`; do
    KEYMAPS="$KEYMAPS $i $i"
  done

  KEYMAP=`dialog --no-cancel --stdout --backtitle "$BACKTITLE : $MSG_KEYMAP_CONFIG" --title "$MSG_KEYMAP" --default-item $KEYMAP_OLD --menu "$MSG_KEYMAP_DESC" 0 0 0 $KEYMAPS` || exit 1

  test -f "/etc/keymaps/$KEYMAP" && loadkmap < "/etc/keymaps/$KEYMAP"
}

# Setup VESA properties
setup_vesa () {
  VESA_MODE_OLD=`cmdline_default vga 789`

  VESA_RES=$((($VESA_MODE_OLD - 784) / 3))
  VESA_DEPTH=$((($VESA_MODE_OLD - 784) % 3))

  if [ $VESA_DEPTH != 0 -a $VESA_DEPTH != 1 -a $VESA_DEPTH != 2 ] ||
     [ $VESA_RES != 0 -a $VESA_RES != 1 -a $VESA_RES != 2 -a $VESA_RES != 3 ]; then
    VESA_RES=1
    VESA_DEPTH=2
  fi

  if [ "$USE_XORG" = no ]; then
    VESA_RES=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_SCREEN_RES" --default-item $VESA_RES --menu "$MSG_SCREEN_DESC" 000 0 0 0 "640x480" 1 "800x600" 2 "1024x768" 3 "1280x1024" 4 "1600x1200"`

    VESA_DEPTH=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_SCREEN_DEPTH" --default-item $VESA_DEPTH --menu "$MSG_SCREEN_DESC" 000 0 0 0 "15 bits" 1 "16 bits" 2 "24 bits"`
  fi

  VESA_MODE=$((784 + VESA_RES*3 + VESA_DEPTH))
  [ $VESA_MODE -ge 796 ] && VESA_MODE=$((VESA_MODE + 1))
}

# Setup BootSplash
setup_bootsplash () {
  if grep -q "splash=silent" di/isolinux.cfg; then
    SPLASH_ARGUMENT="--defaultno"
    SPLASH_OLD="silent"
  else
    SPLASH_ARGUMENT=""
    SPLASH_OLD="0"
  fi

  dialog --aspect 15 --backtitle "$BACKTITLE" --title "" $SPLASH_ARGUMENT --yesno "\n${MSG_SPLASH_DESC}\n" 0 0 && SPLASH="0" || SPLASH="silent"
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
  DISKS=`cat /proc/partitions | sed -n "s/\ *[0-9][0-9]*\ *[0-9][0-9]*\ *\([0-9][0-9]*\)\ \([a-z]*\)$/\2 (\1_blocks)/p"`
  if [ -z "$DISKS" ]; then
    dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --yesno "\n${MSG_DISK_NOT_FOUND}" 0 0 || exit 1
  else
    DISKS="$DISKS $MSG_DISK_REFRESH"
    DISK=`dialog --stdout --backtitle "$BACKTITLE" --title "$MSG_DISK_DEVICE" --menu "\n${MSG_DISK_DEVICE_DESC}" 0 0 0 $DISKS` || exit 1
    [ $DISK != refresh ] && break
  fi
done

if [ "`cat /sys/block/$DISK/removable`" = 1 ]; then
  BOOTLOADER=`dialog --stdout --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_PART_LINUX" --menu "$MSG_DISK_PART_LINUX_DESC" 0 0 0 grub "$MSG_DISK_BOOT_GRUB" syslinux "$MSG_DISK_BOOT_SYSLINUX"` || exit 1
  TYPE=REMOVABLE
else
  BOOTLOADER=grub
  TYPE=HDD
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
        DISKS="$DISKS $i ${S}MB"
        ;;
      83) #Linux is supported only in grub.
        if [ $BOOTLOADER = grub ]; then
          S=`sfdisk -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
          DISKS="$DISKS $i ${S}MB"
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
    $MKFS $MKFS_OPT "$DEV"
  fi
elif [ "$NEED_FORMAT" = yes ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_DISK_ERROR" --msgbox "\n${MSG_INSTALL_DEV_NO_FORMAT} ('$DEV')\n" 0 0
  rmdir di
  exit 1
fi

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
cp -a "$GEEXBOX" di/GEEXBOX

[ "$PART_TYPE" = "Linux" ] && dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOT_SLEEPLESS" --defaultno --yesno "\n${MSG_BOOT_SLEEPLESS_DESC}\n" 0 0 && FASTBOOT=yes && echo "" > "di/GEEXBOX/var/fastboot"

if [ "$FASTBOOT" = "yes" ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOT_LARGE_HDD" --defaultno --yesno "\n${MSG_BOOT_LARGE_HDD_DESC}\n" 0 0 && UNCOMPRESS_INSTALL=yes && rm di/GEEXBOX/bin.tar.lzma
  [ "$UNCOMPRESS_INSTALL" = "yes" -a -f "$GEEXBOX/bin.tar.lzma" ] && tar xaf "$GEEXBOX/bin.tar.lzma" -C di/GEEXBOX
fi

cd di/GEEXBOX/boot
mv vmlinuz initrd.gz isolinux.cfg boot.msg help.msg splash.rle ../../
cd ../../../
rm -rf di/GEEXBOX/boot

# Configure TV card and scan for channels.
[ -f /var/tvcard ] && dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_CFG_TV" --yesno "\n${MSG_CFG_TV_DESC}\n" 0 0 && setup_tvscan "di/GEEXBOX"

# Configure DVB card and scan for channels.
[ -f /var/dvbcard ] &&  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_CFG_DVB" --yesno "\n${MSG_CFG_DVB_DESC}\n" 0 0 && setup_dvbscan "di/GEEXBOX"

# Configure X.Org
if [ -f /etc/X11/X.cfg.sample -o -f /etc/X11/X.cfg ]; then
  USE_XORG=yes # default is to use X if present
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_CFG_HDTV" --yesno "\n${MSG_CFG_HDTV_DESC}\n" 0 0 || USE_XORG=no
fi

if [ "$USE_XORG" = yes ]; then
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_CFG_XORG" --defaultno --yesno "\n${MSG_CFG_XORG_DESC}\n" 0 0 && setup_xorg "di/GEEXBOX"
  [ "$UNCOMPRESS_INSTALL" = "yes" -a -f "$GEEXBOX/X.tar.lzma" ] && rm di/GEEXBOX/X.tar.lzma && tar xaf "$GEEXBOX/X.tar.lzma" -C di/GEEXBOX
else
  # Since X is disabled, remove the files from HDD install to speed up boot
  rm -f di/GEEXBOX/X.tar.lzma
fi

setup_vesa
setup_lang
setup_remote
setup_receiver
setup_bootsplash

grubprefix=/boot/grub
grubdir=di$grubprefix
device_map=$grubdir/device.map

rm -rf $grubdir
mkdir -p $grubdir
[ -f "di/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" ] && tar xaf "di/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" -C $grubdir

if [ -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" ]; then
  cp -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" $grubdir || exit 1
  disable_splashimage=
else
  disable_splashimage="#"
fi
splashimage="$grubprefix/grub-splash.xpm.gz"

if [ $BOOTLOADER = syslinux ]; then
  cp "di/GEEXBOX/usr/share/ldlinux.sys" di
  sed -e "s/boot=cdrom/boot=${DEV#/dev/}/" -e "s/lang=.*/lang=$MENU_LANG/" -e "s/vga=$VESA_MODE_OLD/vga=$VESA_MODE/" -e "s/splash=$SPLASH_OLD/splash=$SPLASH/" -e "s/keymap=.*/keymap=$KEYMAP/" di/isolinux.cfg > di/syslinux.cfg
  rm di/isolinux.cfg
elif [ $BOOTLOADER = grub ]; then
  cp $grubdir/stage2 $grubdir/stage2_single
  rm di/isolinux.cfg di/boot.msg di/help.msg di/splash.rle
fi

if [ $TYPE = HDD ]; then
  echo "quit" | grub --batch --no-floppy --device-map=$device_map 2>&1 > /dev/null
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
  syslinux "$DEV" 2>&1 > /dev/null
  mount -t $MKFS_TYPE "$DEV" di
elif [ $BOOTLOADER = grub ]; then
  if [ $TYPE = HDD ]; then
    rootdev_single=$rootdev
    fake_device=
  elif [ $TYPE = REMOVABLE ]; then
    rootdev_single="(fd0)"
    fake_device="device $rootdev_single $DEV"
  fi

  grub --batch --no-floppy --device-map=$device_map <<EOF
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
  grub --batch --no-floppy --device-map=$device_map <<EOF
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
else
  dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_BOOTLOADER" --msgbox "\n${MSG_LOADER_ERROR}\n" 0 0
fi

umount di
rmdir di

[ -n "$CDROM" ] && eject &

dialog --aspect 15 --backtitle "$BACKTITLE" --title "$MSG_SUCCESS" --msgbox "\n${MSG_SUCCESS_DESC} '$DEV'\n" 0 0
