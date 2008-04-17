#!/bin/sh

MP_FIFO=/var/mp_control
XRANDR_INFO=/tmp/xrandr

export DISPLAY=:0.0

restart_mplayer () {
  xrandr -display :0.0 -q > $XRANDR_INFO
  RES=`cat $XRANDR_INFO | grep "Screen 0" | sed 's/.* current \([0-9]*\) x \([0-9]*\),.*/\1x\2/'`
  RESX=`echo $RES | cut -f1 -dx`
  RESY=`echo $RES | cut -f2 -dx`

  mp_set_option screenw "$RESX"
  mp_set_option screenh "$RESY"
  mp_set_option monitoraspect "${RESX}/${RESY}"

  # check for screen aspect ratio
  # usually found values are 1.25, 1.33 (4:3), 1.6 and 1.77 (16:9)
  ASPECT=$((${RESX}*1000/${RESY}))
  if [ "$ASPECT" -gt 1333 ]; then
    # use the wide HD background screen
    echo "" > /tmp/widescreen
  else
    rm /tmp/widescreen
  fi

  killall mplayer
}

case "$3" in
  # Fn+F5
  00000030) # LCD
    xrandr --output LVDS --preferred --output VGA --off
    restart_mplayer
    ;;
  00000031) # CRT
    xrandr --output VGA --auto --output LVDS --off
    restart_mplayer
    ;;
  00000032) # LCD-CRT
    xrandr --output VGA --mode 800x480 --output LVDS --mode 800x480
    restart_mplayer
    ;;
  # Fn+F7 (Mute)
  00000013)
    echo "mute" > $MP_FIFO
    ;;
  # Fn+F8 (Volume Down)
  00000014)
    echo "volume -1" > $MP_FIFO
    ;;
  # Fn+F9 (Volume Up)
  00000015)
    echo "volume +1" > $MP_FIFO
    ;;
esac
