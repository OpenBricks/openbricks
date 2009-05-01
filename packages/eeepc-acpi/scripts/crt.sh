#!/bin/sh

export DISPLAY=:0.0

. /etc/acpi/eee-common

# Fn+F5 (CRT)
xrandr --output VGA --auto --output LVDS --off
restart_mplayer

