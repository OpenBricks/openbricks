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

# Configure network interface and parameters before installing GeeXboX to disk.
setup_network () {
  local title phy_type wifi_mode wep essid host_ip gw_ip dns_ip smb_user smb_pwd val f

  title="$BACKTITLE : Network Configuration"
  f="$1/etc/network"

  # Get type of physical interface
  phy_type=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "Network Physical Interface" --menu "\nGeeXboX can only use one network physical interface at a time. If you have more than one NIC, GeeXboX will use the first one. If you have both a traditional Ethernet adapter and a Wireless card, GeeXboX will use the wireless card by default. It is recommended to keep physical interface auto-detection but you may also want to force the use of one kind of interface.\n" 0 0 0 auto "Auto detection (recommanded)" ethernet "Force using Ethernet card" wifi "Force using Wireless card"` || exit 1

  # Get wireless settings only if required
  if [ $phy_type = "auto" -o $phy_type = "wifi" ]; then
    wifi_mode=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "Configuring WiFi Mode" --menu "\nAs you seem to be using your wireless adapter to connect this computer to your network, you will have to setup the networking mode.\n Are you connected to an access point (recommanded) or directly to another computer ?\n" 0 0 0 managed "Connected to an access point (recommanded)" ad-hoc "Direct Connection"` || exit 1

    val=`grep WIFI_WEP $f | cut -d'"' -f2`
    wep=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "Configuring WEP key" --inputbox "\nAs you seem to be using your wireless adapter to connect this computer to your network, you may be using a WEP key. If so, please fill in the following input box with your access point WEP key or let it blank if you do not have one (open network).\n" 0 0 "$val"` || exit 1

    val=`grep WIFI_ESSID $f | cut -d'"' -f2`
    essid=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "Configuring WiFi ESSID" --inputbox "\nAs you seem to be using your wireless adapter to connect this computer to your network, you probably are using an SSID. If so, please fill in the following input box with your SSID indentifier or let it blank if you do not have one (open network).\n" 0 0 "$val"` || exit 1
  fi

  # get GeeXboX IP address
  val=`grep HOST $f | cut -d'"' -f2`
  host_ip=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "GeeXboX IP" --inputbox "\nGeeXboX needs to be allocated an IP address to be present on your network. Please fill in the following input box or leave it as it for using DHCP autoconfiguration\n" 0 0 "$val"` || exit 1

  # do not get more settings if DHCP
  if [ ! -z $host_ip ]; then
    val=`grep GATEWAY $f | cut -d'"' -f2`
    gw_ip=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "GeeXboX GateWay" --inputbox "\nYou may want to connect GeeXboX to the Internet. Please fill in the following input box with your gateway IP address or let it blank if you do not want to set a gateway for this computer.\n" 0 0 "$val"` || exit 1

    val=`grep DNS_SERVER $f | cut -d'"' -f2`
    dns_ip=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "GeeXboX DNS Server" --inputbox "\nYou may want to connect GeeXboX to the Internet. Please fill in the following input box with your DNS Server IP address used for name resolving or let it blank if you do not want to resolve names with this computer.\n" 0 0 "$val"` || exit 1
  fi

  # get samba user name
  val=`grep SMB_USER $f | cut -d'"' -f2`
  smb_user=`$DIALOG --no-cancel --stdout --backtitle "$title" --title "Set Samba User name" --inputbox "\nWhen accessing to remote Samba shares, you may need to be authenticated. Most of Microsoft Windows computers let you anonymously access to remote shares using the guest account (SHARE). Please fill in the following input box with your user name for accesing to remote Samba shares or leave it blank if you do not have one.\n" 0 0 "$val"` || exit 1

  # get samba password
  val=`grep SMB_PWD $f | cut -d'"' -f2`
  smb_pwd=`$DIALOG --no-cancel --stdout --backtitle "$title" --title "Set Samba Password" --inputbox "\nIf user needs to be authenticated through a password, please fill in the following input box with it or leave it blank if you do not have one.\n" 0 0 "$val"` || exit 1

  sed -i "s%^PHY_TYPE=\".*\"\(.*\)%PHY_TYPE=\"$phy_type\"\1%" $f
  sed -i "s%^WIFI_MODE=\".*\"\(.*\)%WIFI_MODE=\"$wifi_mode\"\1%" $f
  sed -i "s%^WIFI_WEP=\".*\"\(.*\)%WIFI_WEP=\"$wep\"\1%" $f
  sed -i "s%^WIFI_ESSID=\".*\"\(.*\)%WIFI_ESSID=\"$essid\"\1%" $f
  sed -i "s%^HOST=.*%HOST=\"$host_ip\"%" $f
  sed -i "s%^GATEWAY=.*%GATEWAY=\"$gw_ip\"%" $f
  sed -i "s%^DNS_SERVER=.*%DNS_SERVER=\"$gw_ip\"%" $f
  sed -i "s%^SMB_USER=.*%SMB_USER=\"$smb_user\"%" $f
  sed -i "s%^SMB_PWD=.*%SMB_PWD=\"$smb_pwd\"%" $f
}

# Configure TV card and scan for channels.
setup_tvscan () {
  MPTVSCAN=/usr/bin/mptvscan
  title="$BACKTITLE : Analog TV Channels Scanner"
  MPLAYER_CONF="$1/etc/mplayer/mplayer.conf"

  for i in `$MPTVSCAN -i`; do
    INPUTS="$INPUTS $i ''"
  done

  for i in `$MPTVSCAN -s`; do
    NORMS="$NORMS $i ''"
  done

  for i in `$MPTVSCAN -c`; do
    CHANLISTS="$CHANLISTS $i ''"
  done

  while [ -z "$DONE" ]; do
    INPUT=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "TV Input Selection" --menu "\nBelow is the list of your TV card's available inputs. Please select the one you want to use for channels scan (should be Television)." 0 0 0 $INPUTS`

    NORM=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "TV Norm Selection" --menu "\nBelow is the list of your TV card's supported video standards. Please select the one you want to use, according to your localization." 0 0 0 $NORMS`

    CHANLIST=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "TV Chanlist Selection" --menu "\nBelow is the list of pre-configured chanlists for scan. Select the one corresponding to your location or choose 'all' for a deep scan (scanning all existing frequencies)." 0 0 0 "all channels" ' ' $CHANLISTS`

    $MPTVSCAN -i$INPUT -s$NORM -c$CHANLIST -p 2>/tmp/chans | $DIALOG --no-cancel --aspect 15 --stdout --backtitle "$title" --title "Scanning Channels" --gauge "\nGeeXboX is currently scanning your channels. This operation may take a while. Please wait while processing ..." 0 0

    CHANNELS=`sed 's/channels=//' /tmp/chans | sed 's/-/ - /g' | sed 's/,/\\\\n/g'`
    $DIALOG --aspect 12 --stdout --yes-label "Accept" --no-label "Retry" --backtitle "$title" --title "Scan Done ..." --yesno "\nCongratulations, the TV channels scan is done. The following channels has been discoverd (if no channel has been found, you can then try again with new card/tuner/norm/chanlist settings).\n\n$CHANNELS" 0 0 && DONE=true
  done

  if [ -s /tmp/chans ]; then
    echo -n "tv=" | cat - /tmp/chans >> $MPLAYER_CONF
  fi
  rm /tmp/chans

  sed -i "s/^TVIN_STANDARD=.*/TVIN_STANDARD=$NORM/" $1/etc/tvcard
  sed -i "s/^CHANLIST=.*/CHANLIST=$CHANLIST/" $1/etc/tvcard
}

# Configure DVB card and scan for channels.
setup_dvbscan () {
  DVB_LIST=/usr/share/dvb
  DVB_FILE=/usr/share/dvb.tar.lzma

  DVBSCAN=/usr/bin/dvbscan
  SCAN_ARGS="-x 0" # Scan FreeToAir channels only

  TITLE="$BACKTITLE : Digital TV Channels Scanner"
  CHANNELS_CONF="$1/etc/mplayer/channels.conf"

  if [ -f $DVB_FILE -a ! -d $DVB_LIST ]; then
    lzmacat $DVB_FILE | tar xf - -C /usr/share 
  fi

  DVB_TYPE=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "DVB Card Type Selection" --menu "\nBelow is the list of available DVB cards type. Please select the one you want to use for channels scan." 0 0 0 dvb-s "DVB Sattelite" dvb-t "DVB Terrestrial" dvb-c "DVB Cable" atsc "ATSC (US)"`

  # DVB Terrestrial cards
  if [ $DVB_TYPE = "dvb-t" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      COUNTRIES="$COUNTRIES $i ''"
    done

    COUNTRY=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "Country Selection" --menu "\nBelow is the list of countries with known DVB-T transponders frequencies. Please select the one where you live." 0 0 0 $COUNTRIES`

    for i in `ls $DVB_LIST/$DVB_TYPE/$COUNTRY`; do
      CITIES="$CITIES $i ''"
    done

    CITY=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "City Selection" --menu "\nBelow is the list of locations from your country with known DVB-T transponders frequencies. If you live in place not present in this list, please contact your DVB provider, asking him for your local transponders frequencies and send this information to the LinuxTV (http://www.linuxtv.org/) team. Otherwise, simply choose the nearest town from the place you live." 0 0 0 $CITIES`

    $DVBSCAN $SCAN_ARGS $DVB_LIST/$DVB_TYPE/$COUNTRY/$CITY > $CHANNELS_CONF
  elif [ $DVB_TYPE = "dvb-s" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      SATS="$SATS $i ''"
    done

    SAT=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "Satellite Selection" --menu "\nBelow is the list of known DVB-S satellite transponders you may be able to be connected to. If you are using another transponder which is not present in this list, please contact your DVB provider, asking him for your transponder frequencies and send this information to the LinuxTV (http://www.linuxtv.org/) team. Otherwise, simply choose the one that fit your needs." 0 0 0 $SATS`

    $DVBSCAN $SCAN_ARGS $DVB_LIST/$DVB_TYPE/$SAT > $CHANNELS_CONF
  elif [ $DVB_TYPE = "dvb-c" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      COUNTRIES="$COUNTRIES $i ''"
    done

    COUNTRY=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "Country Selection" --menu "\nBelow is the list of countries with known DVB-C transponders frequencies. Please select the one where you live." 0 0 0 $COUNTRIES`

    for i in `ls $DVB_LIST/$DVB_TYPE/$COUNTRY`; do
      CITIES="$CITIES $i ''"
    done

    CITY=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "City Selection" --menu "\nBelow is the list of locations from your country with known DVB-C transponders frequencies. If you live in place not present in this list, please contact your DVB provider, asking him for your local transponders frequencies and send this information to the LinuxTV (http://www.linuxtv.org/) team. Otherwise, simply choose the nearest town from the place you live." 0 0 0 $CITIES`

    $DVBSCAN $SCAN_ARGS $DVB_LIST/$DVB_TYPE/$COUNTRY/$CITY > $CHANNELS_CONF
  elif [ $DVB_TYPE = "atsc" ]; then
    for i in `ls $DVB_LIST/$DVB_TYPE`; do
      ATSC="$ATSC $i ''"
    done

    FREQ=`$DIALOG --no-cancel --aspect 15 --stdout --backtitle "$TITLE" --title "ATSC Transponder Selection" --menu "\nBelow is the list of known ATSC transponders you may be able to be connected to. If you are using another transponder which is not present in this list, please contact your ATSC provider, asking him for your transponder frequencies and send this information to the LinuxTV (http://www.linuxtv.org/) team. Otherwise, simply choose the one that fit your needs." 0 0 0 $ATSC`

    $DVBSCAN $SCAN_ARGS $DVB_LIST/$DVB_TYPE/$FREQ > $CHANNELS_CONF
  fi

  if [ -f $CHANNELS_CONF -a -s $CHANNELS_CONF ]; then
    # remove non-coherent detected channels
    grep -v "^\[.*\]:" $CHANNELS_CONF > /tmp/channels.conf
    mv /tmp/channels.conf $CHANNELS_CONF
  fi
}

/bin/busybox mount -t proc none /proc
/bin/busybox mount -t sysfs none /sys
/bin/busybox --install -s

if [ "$1" = geexbox ]; then
  DIALOG=/usr/bin/dialog
  CFDISK=/usr/bin/cfdisk
  SFDISK=/usr/bin/sfdisk
  MKDOSFS=/usr/bin/mkdosfs
  MKE2FS=/sbin/mke2fs
  GRUB=/usr/bin/grub
  SYSLINUX=/usr/bin/syslinux
else
  DIALOG=`which dialog`
  CFDISK=`which cfdisk`
  SFDISK=`which sfdisk`
  MKDOSFS=`which mkdosfs`
  MKE2FS=`which mke2fs`
  GRUB=`which grub`
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

if [ -z "$SFDISK" -o -z "$GRUB" -o -z "$DIALOG" ]; then
  echo ""
  echo "**** You need to have sfdisk, grub and dialog installed to install GeeXboX ****"
  echo ""
  exit 1
fi

if [ ! -d "/sys/block" ]; then
  echo ""
  echo "**** You need to have a mounted sysfs at /sys. try executing: mount -t sysfs none /sys ****"
  echo ""
  exit 1
fi

# disable kernel messages to avoid screen corruption
echo 0 > /proc/sys/kernel/printk

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
    DISK=`$DIALOG --stdout --backtitle "$BACKTITLE" --title "Installation device" --menu "\nYou are going to install GeeXboX. For this you will need an empty partition with about 8 MB of free space.\nBe careful to choose the right disk! We won't take responsibility for any data loss." 0 0 0 $DISKS` || exit 1
    [ $DISK != refresh ] && break
  fi
done

if [ "`cat /sys/block/$DISK/removable`" = 1 ]; then
  BOOTLOADER=`$DIALOG --stdout --aspect 15 --backtitle "$BACKTITLE" --title "Linux partition type" --menu "Which type of boot loader you want ? " 0 0 0 grub "GNU GRUB - Doesn't work with oldest BIOS" syslinux "Syslinux - For oldest BIOS compatbility"` || exit 1
  TYPE=REMOVABLE
else
  BOOTLOADER=grub
  TYPE=HDD
fi

if [ $BOOTLOADER = syslinux ]; then
  PART_MSG="FAT partition (type=06)"
elif [ $BOOTLOADER = grub ]; then
  PART_MSG="FAT partition (type=0B) or Linux ext2/3 partition (type=83)"
fi

if [ -z "$CFDISK" ]; then
  CFDISK_MSG="As you don't have cfdisk installed, the installator won't be able to create the partition for you. You'll have to create it yourself before installing."
else
  CFDISK_MSG="Please edit your partition table to create a $PART_MSG with about 8 MB of free space.\nRemember to write the changes when done. We won't take responsibility for any data loss."
fi
$DIALOG --stdout --backtitle "$BACKTITLE" --title "Installation device" --msgbox "$CFDISK_MSG" 0 0 || exit 1

if [ -n "$CFDISK" ]; then
  $CFDISK /dev/$DISK || exit 1
fi

while [ ! -b "$DEV" ]; do
  DISKS=""
  for i in `$SFDISK -l /dev/$DISK | grep ${DISK%disc} | cut -f1 -d' '`; do
    case `$SFDISK --print-id ${i%%[0-9]*} ${i#${i%%[0-9]*}}` in
      1|11|6|e|16|1e) #FAT12/16 are supported both in syslinux and grub.
        S=`$SFDISK -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
        DISKS="$DISKS $i ${S}MB"
        ;;
      b|c|1b|1c|83) #FAT32 and Linux are supported only in grub.
        if [ $BOOTLOADER = grub ]; then
          S=`$SFDISK -s "$i" | sed 's/\([0-9]*\)[0-9]\{3\}/\1/'`
          DISKS="$DISKS $i ${S}MB"
        fi
        ;;
    esac
  done
  if [ -z "$DISKS" ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nYou don't have any $PART_MSG partition on your system. Please create a partition first using for example cfdisk.\n" 0 0
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
if [ -z "$MKFS_TYPE" ]; then
  FORMAT_MSG="Partition is not formated. "
else
  for type in $SUPPORTED_TYPES; do
    [ $type = $MKFS_TYPE ] && NEED_FORMAT=no
  done

  if [ "$NEED_FORMAT" = yes ]; then
    FORMAT_MSG="Partition format type ($MKFS_TYPE) is not supported in your partition type ($PART_TYPE). "
  else
    FORMAT_MSG="Partition is already formated. "
  fi
fi

$DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Formatting" --defaultno --yesno "$FORMAT_MSG\nDo you want to format '$DEV' ?\n" 0 0 && FORMAT=yes

if [ "$FORMAT" = yes ]; then
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

  if [ -z "$MKFS" -o ! -x $MKFS ]; then
    if [ "$NEED_FORMAT" = yes ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\n'$DEV' must be formated. As you don't have formatting tool installed, I won't be able to format the partition.\n" 0 0
      rmdir di
      exit 1
    else
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Warning" --msgbox "\n'$DEV' needs to be a $MKFS_TYPENAME partition. As you don't have formatting tool installed, I won't be able to format the partition. Hopefully it is already formatted.\n" 0 0
    fi
  else
    $MKFS $MKFS_OPT "$DEV"
  fi
elif [ "$NEED_FORMAT" = yes ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\n'$DEV' needs to be a formated.\n" 0 0
  rmdir di
  exit 1
fi

mount -t $MKFS_TYPE "$DEV" di
if [ $? -ne 0 ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nFailed to mount '$DEV' as $MKFS_TYPENAME partition.\n" 0 0
  rmdir di
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
  cd di/GEEXBOX/boot
  mv vmlinuz initrd.gz isolinux.cfg boot.msg help.msg splash.rle ../../
  cd ../../../
  rm -rf di/GEEXBOX/boot
fi

# Setup network is only available when booting from GeeXboX.
if [ "$1" = geexbox ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Configure Network ?" --yesno "\nDo you want to configure your network parameters before installing GeeXboX to disk ?\n" 0 0 && setup_network "di/GEEXBOX"
fi

# Configure TV card and scan for channels.
# (only available when booting from GeeXboX).
if [ "$1" = geexbox ]; then
  if test -n "`grep 'Class 0400:.*109e:' /proc/pci`" \
       -o -n "`grep 'Class 0480:.*1131:' /proc/pci`" \
       -o -n "`grep 'Class 0480:.*14f1:88' /proc/pci`"; then

    # Only scan if a TV card is detected
    /usr/bin/mptvscan -i >/dev/null 2>&1
    if [ `echo $?` = 0 ]; then
      $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Scan for Analog TV Channels ?" --yesno "\nDo you want to configure your analog tv card and scan for channels before installing GeeXboX to disk ?\n" 0 0 && setup_tvscan "di/GEEXBOX"
    fi
  fi
fi

# Configure DVB card and scan for channels.
# (only available when booting from GeeXboX).
if [ "$1" = geexbox ]; then
  # Only scan if a DVB card is detected
  if [ -f /var/dvbcard ]; then
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Scan for Digital (DVB) TV Channels ?" --yesno "\nDo you want to configure your digital (DVB) tv card and scan for channels before installing GeeXboX to disk ?\n" 0 0 && setup_dvbscan "di/GEEXBOX"
  fi
fi

grubprefix=/boot/grub
grubdir=di$grubprefix
device_map=$grubdir/device.map

rm -rf $grubdir
mkdir -p $grubdir
lzmacat "di/GEEXBOX/usr/share/grub-i386-pc.tar.lzma" | tar xf - -C $grubdir

if [ -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" ]; then
  cp -f "di/GEEXBOX/usr/share/grub-splash.xpm.gz" $grubdir || exit 1
  disable_splashimage=
else
  disable_splashimage="#"
fi
splashimage="$grubprefix/grub-splash.xpm.gz"

if [ $BOOTLOADER = syslinux ]; then
  cp "di/GEEXBOX/usr/share/ldlinux.sys" di
  sed "s/boot=cdrom/boot=${DEV#/dev/}/" di/isolinux.cfg > di/syslinux.cfg
  rm di/isolinux.cfg
elif [ $BOOTLOADER = grub ]; then
  cp $grubdir/stage2 $grubdir/stage2_single
  rm di/isolinux.cfg di/boot.msg di/help.msg di/splash.rle
fi

if [ $TYPE = HDD ]; then
  echo "quit" | $GRUB --batch --no-floppy --device-map=$device_map
elif [ $TYPE = REMOVABLE ]; then
  echo "(hd0) ${DEV%%[0-9]*}" > $device_map
fi

rootdev=$(convert $DEV)

if [ -z "$rootdev" ]; then
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "ERROR" --msgbox "\nCouldn't find my GRUB partition representation\n" 0 0
  umount di
  rmdir di
  exit 1
fi


if [ $BOOTLOADER = syslinux ]; then
  umount di
  $SYSLINUX "$DEV"
  mount -t $MKFS_TYPE "$DEV" di
elif [ $BOOTLOADER = grub ]; then
  if [ $TYPE = HDD ]; then
    rootdev_single=$rootdev
    fake_device=
  elif [ $TYPE = REMOVABLE ]; then
    rootdev_single="(fd0)"
    fake_device="device $rootdev_single $DEV"
  fi

  $GRUB --batch --no-floppy --device-map=$device_map <<EOF
$fake_device
root $rootdev_single
install --stage2=$grubdir/stage2_single $grubprefix/stage1 $rootdev_single $grubprefix/stage2_single p $grubprefix/single.lst
EOF

  cat > $grubdir/single.lst <<EOF
default  0
timeout  2
color cyan/blue white/blue
${disable_splashimage}splashimage=$rootdev_single$splashimage

title	GeeXboX
root	$rootdev_single
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME splash=silent vga=0x315 video=vesafb:ywrap,mtrr
initrd  /initrd.gz
boot

title	GeeXboX (debug)
root	$rootdev_single
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME splash=0 vga=0x315 video=vesafb:ywrap,mtrr debugging
initrd  /initrd.gz
boot
EOF
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
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a GeeXboX partition. To boot from it, you will need to install a bootloader. I can install one for you. If you have any other operating system on your computer, I will also install a multiboot for you. If you do not want me to install a new bootloader, you will need to configure yours alone.\nI have found: $supported_os_list\nDo you want to install me to install the boot loader (GRUB) for you ?\n" 0 0 && MBR=yes
  else
    $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --defaultno --yesno "\n'$DEV' is now a GeeXboX partition. I didn't recognize any other OS on your system, want me to install boot loader on your MBR?\n" 0 0 && MBR=yes
  fi
elif [ $TYPE = REMOVABLE ]; then
  oslist=
  MBR=yes
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
${disable_splashimage}splashimage=$rootdev$splashimage

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
kernel	/vmlinuz root=/dev/ram0 rw init=linuxrc boot=$DEVNAME splash=0 vga=0x315 video=vesafb:ywrap,mtrr debugging
initrd  /initrd.gz
boot
EOF

else
  $DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Bootloader" --msgbox "\nYou must install a boot loader to boot GeeXboX\n" 0 0
fi

umount di
rmdir di

[ -n "$CDROM" ] && eject &

$DIALOG --aspect 15 --backtitle "$BACKTITLE" --title "Have Fun!" --msgbox "\nGeeXboX is now installed on '$DEV'\n" 0 0
