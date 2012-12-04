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

  cp /etc/xbmc/generic-advancedsettings.xml $ADV_SETTINGS

  ## Now overwrite with specific config files

  if dmesg | grep "OMAP4 Panda board" -q ; then
    cp /etc/xbmc/panda-advancedsettings.xml $ADV_SETTINGS
    cp /etc/xbmc/panda-playercorefactory.xml $CORE_FACTORY
  fi

  if grep Snowball /proc/cpuinfo -q ; then 
    cp /etc/xbmc/snowball-advancedsettings.xml $ADV_SETTINGS
  fi

  if cat /proc/cpuinfo | grep CuBox -q; then
    cp /etc/xbmc/cubox-advancedsettings.xml $ADV_SETTINGS
    mkdir -p /root/.xbmc/userdata/peripheral_data
    cp /etc/xbmc/builtin_0471_1001.xml /root/.xbmc/userdata/peripheral_data/builtin_0471_1001.xml
  fi 
}

set_default_gui_settings () {
  [ -f "$GUI_SETTINGS" ] && return

  [ "x$TZ" = "x" ] && TZ=$( ls -l /etc/localtime | sed 's/.*\/usr\/share\/zoneinfo\///')
  [ "x$TZ" = "x" ] || TZ_COUNTRY_CODE=$( grep $TZ /usr/share/zoneinfo/zone.tab | cut -f1|head -n 1)
  [ "x$TZ_COUNTRY_CODE" = "x" ] || TZ_COUNTRY=$( grep $TZ_COUNTRY_CODE /usr/share/zoneinfo/iso3166.tab | cut -f2|head -n 1)

  cp /etc/xbmc/guisettings.xml $GUI_SETTINGS

  if cat /proc/cpuinfo | grep CuBox -q; then
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
if [ -f /usr/share/xbmc/system/Lircmap.xml ] && [ ! -f $USERDATA/Lircmap.xml ]; then cp /usr/share/xbmc/system/Lircmap.xml "$USERDATA"; fi
