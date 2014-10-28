#!/bin/haserl
Content-type: application/octet-stream;
Content-Disposition: attachment; filename=<% echo "$(sed 's/[( )]//g' /etc/version)-config-$(date +%Y%m%d%H%M).bin;" 
dd if=/dev/mtdblock1 of=/tmp/cfgpart 2> /dev/null
echo $(md5sum /tmp/cfgpart | cut -c 1-32)$(cat /etc/version) | cat /tmp/cfgpart - > /tmp/config.bin
echo ""
dd if=/tmp/config.bin
rm -f /tmp/cfgpart
rm -f /tmp/config.bin 
%>
