#!/bin/sh

NM_IFACE="$1"
NM_STATE="$2"

if [ "$NM_STATE" = "up" ]; then
  . /etc/network

  if [ -n "$TIMESERVERS" ] && [ "$IFACE" = "$NM_IFACE" ]; then
    for i in $TIMESERVERS; do
      NTP_OPTS="$NTP_OPTS -p $i"
    done

    echo "Starting ntpd"
    /usr/sbin/ntpd $NTP_OPTS
  fi
fi

exit 0
