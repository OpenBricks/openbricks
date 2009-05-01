#!/bin/sh

. /etc/acpi/eee-common

# Fn+F9 (Volume Up)
echo "volume +1" > $MP_FIFO

