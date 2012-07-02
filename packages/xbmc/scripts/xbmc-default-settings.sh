#!/bin/sh

#############################################################################
# Setup default XBMC settings files                                         #
# See http://wiki.xbmc.org/index.php?title=Advancedsettings.xml for details #
#############################################################################

USERDATA="/root/.xbmc/userdata"
ADV_SETTINGS="$USERDATA/advancedsettings.xml"
GUI_SETTINGS="$USERDATA/guisettings.xml"
SOURCES="$USERDATA/sources.xml"

set_default_advanced_settings () {
  [ -f "$ADV_SETTINGS" ] && return

FULLSCREEN=true
if dmesg | grep "OMAP4 Panda board" -q ; then 
  FULLSCREEN=false
fi

if grep Snowball /proc/cpuinfo -q ; then 
  FULLSCREEN=false
fi


  cat > "$ADV_SETTINGS" << EOF
<advancedsettings>
  <useddsfanart>true</useddsfanart>
  <cputempcommand>sed -e 's/\([0-9]*\)[0-9]\{3\}.*/\1 C/' /sys/class/thermal/thermal_zone0/temp</cputempcommand>
  <samba>
    <clienttimeout>10</clienttimeout>
  </samba>
  <fullscreen>$FULLSCREEN</fullscreen>
</advancedsettings>
EOF
}

set_default_gui_settings () {
  [ -f "$GUI_SETTINGS" ] && return
  [ "x$TZ" = "x" ] && TZ=$( ls -l /etc/localtime | sed 's/.*\/usr\/share\/zoneinfo\///')
  [ "x$TZ" = "x" ] || TZ_COUNTRY_CODE=$( grep $TZ /usr/share/zoneinfo/zone.tab | cut -f1|head -n 1)
  [ "x$TZ_COUNTRY_CODE" = "x" ] || TZ_COUNTRY=$( grep $TZ_COUNTRY_CODE /usr/share/zoneinfo/iso3166.tab | cut -f2|head -n 1)

  cat > "$GUI_SETTINGS" << EOF
<settings>
  <locale>
    <timezone>$TZ</timezone>
    <timezonecountry>$TZ_COUNTRY</timezonecountry>
  </locale>
</settings>
EOF
}

set_default_sources () {
  [ -f "$SOURCES" ] && return

  cat > "$SOURCES" << EOF
<sources>
  <video>
    <default pathversion="1"></default>
    <source>
      <name>Default Locations</name>
      <path pathversion="1">/mnt/</path>
    </source>
  </video>
  <music>
    <default pathversion="1"></default>
    <source>
      <name>Default Locations</name>
      <path pathversion="1">/mnt/</path>
    </source>
  </music>
  <pictures>
    <default pathversion="1"></default>
    <source>
      <name>Default Locations</name>
      <path pathversion="1">/mnt/</path>
    </source>
  </pictures>
</sources>
EOF
}

mkdir -p "$USERDATA"
set_default_advanced_settings
set_default_gui_settings
set_default_sources

# remote
[ -f /usr/share/xbmc/system/Lircmap.xml ] && cp /usr/share/xbmc/system/Lircmap.xml "$USERDATA"
