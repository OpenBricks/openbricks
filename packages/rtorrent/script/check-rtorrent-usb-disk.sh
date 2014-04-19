#!/bin/sh

. /etc/rtorrent.cfg

if [ "$CHECK_USB_DISK" = "no" ] ; then
  exit 0
fi

if [ "x$1" == "x" ]; then
  echo  "Got empty argument"
  exit 1
fi
mp=""
for i in 1 2 3 4 5 6 7 8 9 ; do
	sleep $i
	a=`udisks --show-info /dev/$1 | grep "mount paths" | sed -e "s/^.*: *//"`
	! [ -z "$a" ] && mp="$a" && break
done

cat /proc/mounts > /tmp/$1.txt

#First check that rtorrent config points to /rtorrent; if so then user probably
# didn't hand configure anything; so lets point to the new mount point
CHECK=`cat /etc/rtorrent.rc  | grep "directory =" | cut -f2 -d'=' | head -n1`
if [ "x$CHECK" != "x /rtorrent" ]; then
	exit 0
fi

#Second check if there is already a removable media with download directory.
# If so then don't replace it
CHECK=`cat /tmp/rtorrent-dir`
if [ "x$CHECK" != "x" ]; then
	echo "There is already an rtorrent download area. Exiting"
	exit 0
fi

echo $1 > /tmp/rtorrent-dir

# Set rtorrent to use that new directory
mkdir -p "$mp/rtorrent"
mkdir -p "$mp/rtorrent/session"
mkdir -p "$mp/rtorrent/watch"
\rm /tmp/rtorrent

ln -s "$mp/rtorrent" /tmp/rtorrent
[ -f "$mp/rtorrent/session/rtorrent.lock" ] && rm "$mp/rtorrent/session/rtorrent.lock"
echo "directory = /tmp/rtorrent/" > /tmp/new-rtorrent-dir
echo "session = /tmp/rtorrent/session" >> /tmp/new-rtorrent-dir
echo "schedule = watch_directory,5,5,load_start=/tmp/rtorrent/watch/*.torrent" >> /tmp/new-rtorrent-dir
cat /tmp/new-rtorrent-dir >> /etc/rtorrent.rc
sync

systemctl restart rtorrent
echo "Restarted rtorrent"

# start to check if udisks wants to unmount this device
/usr/bin/check-rtorrent-udisks-remove.sh &

exit 0
