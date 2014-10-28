#!/bin/bash

. /etc/network

if [ "$IFACE" = "" ]; then
  INTERFACE="eth0"
else
  INTERFACE=$IFACE
fi

logger -t 'wakeonlan init script' enabling wake on lan for $INTERFACE
ethtool -s $INTERFACE wol g
echo enabled > /sys/class/net/$INTERFACE/device/power/wakeup
