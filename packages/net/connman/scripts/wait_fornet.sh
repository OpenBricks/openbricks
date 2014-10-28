#!/bin/bash
. /etc/funcs

wait_for_network
if [ "$?" = 1 ]; then
  logger -t automount_smb "No network connection available, exiting..."
  exit 2
fi

