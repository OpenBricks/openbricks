#!/bin/bash
export DISPLAY=0:0
ASPECT_RATIO="anamorphic"

GEOMETRY=`xrandr | grep "\*" | cut -d" " -f4`


USE_AUTOCROP="1"

test -f /etc/default/xine && . /etc/default/xine
XINEOPTS="-G $GEOMETRY -A alsa --config /etc/xine/config --keymap=file:/etc/xine/keymap --post vdr --post vdr_video --post vdr_audio --verbose=2 --no-gui --no-logo --no-splash --deinterlace -pq"

if cat /proc/devices | grep -i nvidia; then
  vdpauinfo 2>/dev/null >/dev/null && XINEOPTS="$XINEOPTS -V vdpau"
fi

if [ -n "$ASPECT_RATIO" ]; then
  XINEOPTS="$XINEOPTS --aspect-ratio=$ASPECT_RATIO"
fi 

if [ "$USE_AUTOCROP" = "1" ]; then
  XINEOPTS="$XINEOPTS --post autocrop:enable_autodetect=1,enable_subs_detect=1,soft_start=1,stabilize=1"
fi 

while [ ! -e /tmp/vdr-xine/stream ] ; do sleep 0.1 ; done
#sleep 10
/usr/bin/xine $XINEOPTS vdr:/tmp/vdr-xine/stream#demux:mpeg_pes

