#!/bin/sh

export DISPLAY=:0.0

. /etc/acpi/eee-common

# Fn+F5 (LCD-CRT)
xrandr --output VGA --mode 800x480 --output LVDS --mode 800x480
restart_mplayer

