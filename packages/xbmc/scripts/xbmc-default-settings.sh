#!/bin/sh

#############################################################################
# Setup default XBMC settings files                                         #
# See http://wiki.xbmc.org/index.php?title=Advancedsettings.xml for details #
#############################################################################

USERDATA="/root/.xbmc/userdata"
ADV_SETTINGS="$USERDATA/advancedsettings.xml"
GUI_SETTINGS="$USERDATA/guisettings.xml"
CORE_FACTORY="$USERDATA/playercorefactory.xml"
SOURCES="$USERDATA/sources.xml"

set_default_advanced_settings () {
  [ -f "$ADV_SETTINGS" ] && return

  if grep -q "OMAP4 Panda board" /proc/cpuinfo; then
    cp /etc/xbmc/panda-advancedsettings.xml $ADV_SETTINGS
    cp /etc/xbmc/panda-playercorefactory.xml $CORE_FACTORY
  elif grep -q Snowball /proc/cpuinfo; then
    cp /etc/xbmc/snowball-advancedsettings.xml $ADV_SETTINGS
  elif grep -q CuBox /proc/cpuinfo; then
    cp /etc/xbmc/cubox-advancedsettings.xml $ADV_SETTINGS
  elif grep -q BCM2708 /proc/cpuinfo; then
    cp /etc/xbmc/rpi-advancedsettings.xml $ADV_SETTINGS
  elif grep -q sun4i /proc/cpuinfo; then
    cp /etc/xbmc/a10-advancedsettings.xml $ADV_SETTINGS
  else
    cp /etc/xbmc/generic-advancedsettings.xml $ADV_SETTINGS
  fi
}

set_default_gui_settings () {
  [ -f "$GUI_SETTINGS" ] && return

  ZONEINFO="/usr/share/zoneinfo"
  if [ -z "$TZ" ]; then
    TZ=`readlink /etc/localtime`
    TZ=${TZ#${ZONEINFO}}
    TZ_COUNTRY_CODE=`grep $TZ $ZONEINFO/zone.tab | cut -f1 | head -1`
  fi
  [ -n "$TZ_COUNTRY_CODE" ] \
    TZ_COUNTRY=`grep $TZ_COUNTRY_CODE $ZONEINFO/iso3166.tab | cut -f2 | head -1`

  cp /etc/xbmc/guisettings.xml $GUI_SETTINGS

  if grep -q CuBox /proc/cpuinfo; then
    cp /etc/xbmc/cubox-guisettings.xml $GUI_SETTINGS
  fi 

  sed -i -e "s,TZ_COUNTRY,$TZ_COUNTRY," \
         -e "s,TZ,$TZ," \
         $GUI_SETTINGS
}

set_default_sources () {
  [ -f "$SOURCES" ] && return
  cp /etc/xbmc/sources.xml $SOURCES
}

mkdir -p "$USERDATA"
set_default_advanced_settings
set_default_gui_settings
set_default_sources

# remote
if [ -f /usr/share/xbmc/system/Lircmap.xml ] && [ ! -f $USERDATA/Lircmap.xml ]
then
  cp /usr/share/xbmc/system/Lircmap.xml "$USERDATA"
fi

exit 0
