#!/bin/sh

/bin/mkdir -p /var/run/lirc

TMPFILE=/tmp/TMP.$$

/bin/mkdir -p /var/run/lirc
if [ -f /etc/lirc/hardware.conf ]; then
   source /etc/lirc/hardware.conf
else
   exit 1
fi

for i in 0 1 2 3 4 5 6 7 8 9; do
  [ -d /sys/class/rc ] && break
  sleep 0.3
done

for i in 0 1 2 3 4 5 6 7 8 9; do
  ls /sys/class/rc/* >/dev/null 2>&1 && break
  sleep 0.3
done

FIRSTIR="yes"
for i in /sys/class/rc/*; do
  [ $FIRSTIR == "yes" ] && DEVEVENT2=$(ls -d $i/lirc*)
  grep -i "$DRV_NAME" $i/uevent >/dev/null 2>&1 && DEVEVENT1=$(ls -d $i/lirc*)
   FIRSTIR="no"
done

if [ "X$DEVEVENT1" == "X" ]; then
  DEVEVENT=$(basename ${DEVEVENT2})
else
  DEVEVENT=$(basename ${DEVEVENT1})
fi

if [ "X$DEVEVENT" == "X" ]; then
  DEVEVENT=lirc0
fi

DEVNUM=${DEVEVENT##*lirc}
if [ -f /sys/class/rc/rc${DEVNUM}/protocols ]; then
  echo "lirc" > /sys/class/rc/rc${DEVNUM}/protocols
fi

sed -i "s/^DEVICE=.*/DEVICE=\"\/dev\/lirc1\"/g" /etc/lirc/hardware.conf
