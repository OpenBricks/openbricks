#!/bin/sh
# script to handle mount of persistent data partition

# persistent data partition already mounted
[ -h /var/data ] && rm /var/data

# parse command line arguments
for arg in $(cat /proc/cmdline); do
  case $arg in
    boot=*)
      BOOT="${arg#boot=}"
      case "$BOOT" in
        UUID=*)
          BOOT=hdd
          ;;
        LABEL=*)
          BOOT=hdd
          ;;
        *)
          BOOT_DEV=$BOOT
          BOOT=hdd
          ;;
      esac
      ;;
    data=*)
      DATA="${arg#data=}"
      case "$DATA" in
	none)
	  DATA=""
	  DATA_DEV=""
	  DATA_UUID=""
	  ;;
        UUID=*)
          # Grab real device name from UUID symlink
          DATA_UUID="${DATA#UUID=}"
          DATA=hdd
          ;;
        *)
          if [ "$DATA" != cdrom -a "$DATA" != nfs -a "$DATA" != smb ]; then
            DATA_DEV=$DATA
            DATA=hdd
          fi
          ;;
      esac
      ;;
    datadir=*)
      DATA_LOCATION="${arg#datadir=}"
      ;;
    waittime=*)
      WAITTIME="${arg#waittime=}"
      ;;
  esac
done

# determine if a data dir was specified or force the default one
[ -z "$DATA_LOCATION" ] && DATA_LOCATION="/geexbox-data"

data_partition=""
data_device=""

if [ -z "$DATA" -a "$BOOT_DEV"="cdrom" ]; then 
  # let the user select a persistent data partition if one was found

  # todo: i18n
  [ -z "$WAITTIME" ] && WAITTIME=3
  echo "Waiting $WAITTIME seconds for (slow) devices to become ready..."
  sleep $WAITTIME;

  for i in /mnt/*; do
    echo "$i" >>/tmp/mount.out
  done

  #todo: i18n
  partitions=""
  for i in /mnt/*; do
    # ensure that this mount point is a disk
    echo "$i" | grep -qi disk || continue;

    if [ -d "$i/$DATA_LOCATION" ]; then
      partitions="$partitions \"$i\""
      partitions="$partitions \"GeeXboX configuration partition\""
    fi
  done

  test -z "$partitions" && exit 0 # no potential data partitions found
  
  #todo: i18n
  echo dialog --timeout 10 --aspect 15 --stdout --backtitle \"$MSG_CFG_TITLE\" \
    --title \"Persistent configuration\" --menu \"Please select a persistent data partition in case you want to use one of the following\" 0 0 0 $partitions >/tmp/partselect
  data_partition=`sh /tmp/partselect`
  [ -z "$data_partition" ] && exit 0 # none selected, working without

  data_device=`mount | grep "$data_partition" | sed 's/\(.*\) on.*/\1/'`
  data_uuid=`blkid -o value -s UUID $data_device`

elif [ -n "$DATA" ]; then
  # data partition was given at kernel cmd line

  if [ -n "$DATA_UUID" ]; then
    DATA_DEV=""
    DATA_DEV_NAME=""
    DEV_PATH=$(find /dev/disk/by-uuid/ -iname ${DATA_UUID})
    if [ -n "$DEV_PATH" ]; then
      DATA_DEV_NAME=$(ls -l ${DEV_PATH} | sed 's#.*\.\.\/##')
      DATA_DEV=/dev/$DATA_DEV_NAME
    else
      exit 0 # partition wasn't found
    fi
  fi

  data_device=$DATA_DEV
fi

if [ -n "$data_device" ]; then
  mkdir -p /mnt/data /var/data
  umount "$data_device" >/dev/null 2>&1
  mount "$data_device" /mnt/data
  mount -o bind /mnt/data/$DATA_LOCATION /var/data

  # doublecheck whether location is writable
  ( echo >/var/data/test && rm /var/data/test ) || exit 1
else
  exit 0
fi

