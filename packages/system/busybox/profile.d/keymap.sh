#!/bin/sh

loaded="no"

try_keymap() {
  if [ -n "$1" ] && [ -r /etc/keymaps/$1.kmap ]; then
    cat /etc/keymaps/$1.kmap | /sbin/loadkmap
    loaded="yes"
  fi
}

# load keymap given in /etc/locale.conf
if [ -r /etc/locale.conf ]; then
  try_keymap $(sed -n "s/.*LANG=\([^\.]*\).*/\1/p" /etc/locale.conf)

  [ $loaded = "no" ] && \
    try_keymap $(sed -n "s/.*LANG=\([^_]*\).*/\1/p" /etc/locale.conf)
fi

# load keymap given on kernel cmdline
[ $loaded = "no" ] && \
  try_keymap  $(sed -n "s/.*keymap=\([^ ]*\).*/\1/p" /proc/cmdline)
