#!/bin/sh

# load keymap given on kernel cmdline
keymap=$(sed -n "s/.*keymap=\([^ ]*\).*/\1/p" /proc/cmdline)
[ "x$keymap" != "x" ] && [ -r /etc/keymaps/$keymap ] && cat /etc/keymaps/$keymap | /sbin/loadkmap

