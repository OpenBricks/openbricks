#!/bin/sh

loaded=""

try_keymap() {
  if [ -z "$loaded" ] && [ -n "$1" ] && [ -r /etc/keymaps/$1.kmap ]; then
    /sbin/loadkmap < /etc/keymaps/$1.kmap
    loaded=1
  fi
}

# load keymap given in /etc/locale.conf
if [ -r /etc/locale.conf ]; then
  try_keymap $(sed -n "s/.*LANG=\([^\.]*\).*/\1/p" /etc/locale.conf)
  try_keymap $(sed -n "s/.*LANG=\([^_]*\).*/\1/p" /etc/locale.conf)
fi

# load keymap given on kernel cmdline
try_keymap $(sed -n "s/.*keymap=\([^ ]*\).*/\1/p" /proc/cmdline)
