#!/bin/bash

if [ $# -lt 2 ]
then
  echo "usage: $0 objectpath interface.method [arguments]"
  exit 1
fi

DEST=de.tvdr.vdr
OBJECT=$1
shift
INTERFACE=$1
shift

dbus-send --system --type=method_call --dest=$DEST --print-reply $OBJECT $DEST.$INTERFACE "$@"

