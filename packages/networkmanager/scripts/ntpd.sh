#!/bin/sh

. /etc/network

for i in $TIMESERVERS; do
  NTP_OPTS="$NTP_OPTS -p $i"
done

echo "Starting ntpd"
/usr/sbin/ntpd $NTP_OPTS