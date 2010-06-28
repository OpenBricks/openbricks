#!/bin/sh

echo "Still needs to be ported to OPKG:"

for i in packages/*; do
  [ ! -f $i/meta ] && echo $i
done

exit 0
