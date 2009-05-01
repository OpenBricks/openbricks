#!/bin/sh

. /etc/acpi/eee-common

# Fn+F8 (Volume Down)
echo "volume -1" > $MP_FIFO

