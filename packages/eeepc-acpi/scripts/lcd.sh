#!/bin/sh

export DISPLAY=:0.0

. /etc/acpi/eee-common

# Fn+F5 (LCD)
xrandr --output LVDS --preferred --output VGA --off
restart_mplayer

