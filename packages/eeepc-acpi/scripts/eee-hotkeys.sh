#!/bin/sh

MP_FIFO=/var/mp_control

export DISPLAY=:0.0

case "$3" in
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
