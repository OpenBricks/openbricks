#!/bin/sh

#############################################################################
# Setup default XBMC settings files                                         #
# See http://wiki.xbmc.org/index.php?title=Advancedsettings.xml for details #
#############################################################################

USERDATA="/root/.xbmc/userdata"
ADV_SETTINGS="$USERDATA/advancedsettings.xml"
SOURCES="$USERDATA/sources.xml"

set_default_advanced_settings () {
  [ -f "$ADV_SETTINGS" ] && return


  cat > "$ADV_SETTINGS" << EOF
<advancedsettings>
  <useddsfanart>true</useddsfanart>
  <cputempcommand>sed -e 's/\([0-9]*\)[0-9]\{3\}.*/\1 C/' /sys/class/thermal/thermal_zone0/temp</cputempcommand>
  <samba>
    <clienttimeout>10</clienttimeout>
  </samba>
  <gui>
    <algorithmdirtyregions>1</algorithmdirtyregions>
  </gui>
  <fullscreen>true</fullscreen>
</advancedsettings>
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
set_default_sources

# remote
[ -f /usr/share/xbmc/system/Lircmap.xml ] && cp /usr/share/xbmc/system/Lircmap.xml "$USERDATA"
