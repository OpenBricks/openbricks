#!/bin/sh

. /etc/rtorrent.cfg

if [ "$CHECK_USB_DISK" = "no" ] ; then
  exit 0
fi

if [ "x$1" == "x" ]; then
	echo "Got empty unmount point : $1"
	exit -1
fi

# First check that rtorrent config points to /rtorrent; if so then user probably
# didn't hand configure anything; so lets point to the new mount point
CHECK=`cat /etc/rtorrent.rc  | grep "directory =" | cut -f2 -d'=' | head -n1`
if [ "x$CHECK" != "x /rtorrent" ]; then
	exit 0
fi

# Second check if there is already a removable media with download directory.
# If there isn't then there is nothing to do here
CHECK=`cat /tmp/rtorrent-dir`
if [ "x$CHECK" == "x" ]; then
	echo "No removable media with rtorrent attached to it; nothing to do"
	exit 0
fi

if [ "x$1" != "x$CHECK" ]; then
	echo "Called unmount on irrelevant directory."
	exit 0
fi

cat /etc/rtorrent.rc | grep -v "/tmp/rtorrent" > /tmp/rtorrent.rc.tmp
mv /tmp/rtorrent.rc.tmp /etc/rtorrent.rc
systemctl try-restart rtorrent
echo "Restarted rtorrent"
rm -rf /tmp/rtorrent-dir
sleep 1