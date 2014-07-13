#!/bin/sh

if grep -q "quiet" /proc/cmdline ; then
  rm /dev/console
  ln -s /dev/null /dev/console
fi

