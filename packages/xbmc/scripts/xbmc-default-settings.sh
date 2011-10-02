#!/bin/sh

#############################################################################
# Setup default XBMC settings files                                         #
# See http://wiki.xbmc.org/index.php?title=Advancedsettings.xml for details #
#############################################################################

USERDATA="$HOME/.xbmc/userdata"
ADDONS="$HOME/.xbmc/addons"
TEMP_DIR="$HOME/.xbmc/temp"
ADV_SETTINGS="$USERDATA/advancedsettings.xml"
GUI_SETTINGS="$USERDATA/guisettings.xml"
SOURCES="$USERDATA/sources.xml"

gpu_guess () {

  GPUDEVICE=$(cat /tmp/pci | grep 0300)
  GPUTYPE="OTHER"
  [ "$(echo $GPUDEVICE | grep 8086)" ] && GPUTYPE="INTEL"
  [ "$(echo $GPUDEVICE | grep 10de)" ] && GPUTYPE="NVIDIA"
  [ "$(echo $GPUDEVICE | grep 1002)" ] && GPUTYPE="AMD"

  export GPUTYPE
}

set_default_gui_settings () {
  [ -f "$GUI_SETTINGS" ] && return

  [ "$GPUTYPE" != "NVIDIA" -a "$GPUTYPE" != "AMD" ] && return

  cat > "$GUI_SETTINGS" << EOF
<settings>
  <!-- Sync to vblank -->
  <videoscreen>
    <vsync>2</vsync>
  </videoscreen>
</settings>
EOF
}

set_default_advanced_settings () {
  [ -f "$ADV_SETTINGS" ] && return

  cat > "$ADV_SETTINGS" << EOF
<advancedsettings>
  <useddsfanart>true</useddsfanart>
  <samba>
    <clienttimeout>10</clienttimeout>
  </samba>
  <gui>
    <algorithmdirtyregions>1</algorithmdirtyregions>
  </gui>
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

# clean temp dir
rm -rf "$TEMP_DIR/*"

mkdir -p "$USERDATA"
gpu_guess
set_default_advanced_settings
set_default_gui_settings
set_default_sources

# remote
[ -f /usr/share/xbmc/system/Lircmap.xml ] && cp /usr/share/xbmc/system/Lircmap.xml "$USERDATA"

# geexbox.network.cfg
mkdir -p $ADDONS
[ -d /usr/share/xbmc/addons/geexbox.network.cfg ]  && cp -R /usr/share/xbmc/addons/geexbox.network.cfg "$ADDONS"

# geexbox.updater
mkdir -p $ADDONS
[ -d /usr/share/xbmc/addons/geexbox.updater ]  && cp -R /usr/share/xbmc/addons/geexbox.updater "$ADDONS"
