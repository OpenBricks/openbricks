#!/bin/sh

. /etc/rtorrent.cfg

if [ "$CHECK_USB_DISK" = "no" ] ; then
  exit 0
fi

CHECK=`cat /tmp/rtorrent-dir`

udisks --monitor | while read -r line ; do
  a=`echo $line | grep $CHECK | grep "changed"`
  if ! [ -z "$a" ] ; then
    echo "udisks wants to remove $CHECK"
    systemctl stop rtorrent
    udisks --unmount /dev/$CHECK
  fi
done
