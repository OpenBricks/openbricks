#!/bin/sh

CHANNELMAP=/etc/vdr/plugins/tvm2vdr/tvm2vdr_channelmap.conf

echo "# you've got to map showview numbers to each channel and change the number 000 accordingly" >$CHANNELMAP
echo "# showview numbers may be found at e.g. http://de.wikipedia.org/wiki/ShowView#ShowView-Leitzahlen_f.C3.BCr_Deutschland.2C_.C3.96sterreich_und_die_Schweiz" >>$CHANNELMAP
cat /etc/vdr/channels.conf | while read line; do
  name=`echo $line | cut -d: -f1`
  source=`echo $line | cut -d: -f4`
  sid=`echo $line | cut -d: -f10`
  nid=`echo $line | cut -d: -f11`
  tid=`echo $line | cut -d: -f12`

  echo "$source" | grep -q "^C\|^S\|^T" && \
    echo "${showview_id} = ${source}-${nid}-${tid}-${sid} // ${name}\n" >>$CHANNELMAP
done
