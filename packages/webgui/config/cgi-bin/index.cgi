#!/bin/sh

DISP="/bin/busybox echo"

UPTIME=`/bin/busybox cat /proc/uptime | /bin/busybox sed 's/\([0-9]*\)\..*/\1/'`
UP_DAYS=$(($UPTIME/24/3600))
UP_HOURS=$(($(($UPTIME-$(($UP_DAYS*24*3600))))/3600))
UP_MIN=$(($(($UPTIME-$(($UP_DAYS*24*3600))-$(($UP_HOURS*3600))))/60))
UP_SEC=$(($UPTIME-$(($UP_DAYS*24*3600))-$(($UP_HOURS*3600))-$(($UP_MIN*60))))

$DISP "Content-type: text/html"
$DISP ""

$DISP "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
$DISP "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">"
$DISP "<html>"
$DISP "  <head>"
$DISP "    <title>GeeXboX Management Center</title>"
$DISP "    <meta http-equiv=\"Content-language\" content=\"en\"/>"
$DISP "    <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=ISO-8859-1\"/>"
$DISP "    <link rel=\"icon\" type=\"images/png\" href=\"../img/icon.png\"/>"
$DISP "  </head>"
$DISP "  <body>"
$DISP "    <center>"
$DISP "    <img src=\"../img/logo.jpeg\" alt=\"GeeXboX Logo\"/>"
$DISP "    <h1>GeeXboX Management Center</h1>"
$DISP "    <br/>"
$DISP "    Uptime : $UP_DAYS days $UP_HOURS hours $UP_MIN minutes $UP_SEC secondes."
$DISP "    </center>"
$DISP "  </body>"
$DISP "</html>"
