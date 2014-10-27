#!/bin/sh

SERVICE=$(echo $QUERY_STRING | cut -d "=" -f 2)
if [ \( $(echo -n $SERVICE | sed s/[a-zA-Z0-9]*/""/g | wc -c) -eq 0 \) -a \( -f /opt/share/www/$SERVICE \) ]; then
   /bin/haserl /opt/share/www/$SERVICE
else
    echo "Content-type: text/html"
    echo ""
    echo "<html>opkg-menu: bad parameter</html>"
fi
