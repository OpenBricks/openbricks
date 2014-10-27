#!/bin/sh

#
# Automounted
#

. /etc/automountd.conf

if [ "$AUTOMOUNT" = "on" ]; then
  /usr/bin/automountd
fi 

exit 0

