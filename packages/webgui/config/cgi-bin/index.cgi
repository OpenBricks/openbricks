#!/bin/sh

PATH=/bin:/usr/bin

UPTIME=`cat /proc/uptime | cut -f1 -d.`
UP_DAYS=$(($UPTIME/24/3600))
UP_HOURS=$(($(($UPTIME-$(($UP_DAYS*24*3600))))/3600))
UP_MIN=$(($(($UPTIME-$(($UP_DAYS*24*3600))-$(($UP_HOURS*3600))))/60))
UP_SEC=$(($UPTIME-$(($UP_DAYS*24*3600))-$(($UP_HOURS*3600))-$(($UP_MIN*60))))

cat <<EOF
Content-type: text/html

<?xml version="1.0" encoding="iso-8859-1"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
  <head>
    <title>GeeXboX Management Center</title>
    <meta http-equiv="Content-language" content="en"/>
    <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=ISO-8859-1"/>
    <link rel="icon" type="images/png" href="../img/icon.png"/>
  </head>
  <body>
    <center>
    <img src="../img/logo.jpeg" alt="GeeXboX Logo"/>
    <h1>GeeXboX Management Center</h1>
    <br/>
    Uptime : $UP_DAYS days $UP_HOURS hours $UP_MIN minutes $UP_SEC secondes.
    </center>
  </body>
</html>
EOF
