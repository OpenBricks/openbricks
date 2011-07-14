#!/bin/sh

MODE=on
[ "$1" = on ]  && MODE=on
[ "$1" = off ] && MODE=off

grep -q -v splash /proc/cmdline && exit 1

if [ "$MODE" = on ]; then
  dd if=/dev/zero of=/dev/fb0 >/dev/null 2>&1
  /bin/dietsplash &
elif [ "$MODE" = off ]; then
  /usr/bin/dietsplashctl 100 ""
fi

exit 0
