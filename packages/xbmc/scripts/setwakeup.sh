#!/bin/bash
# acpi wakeup script called by xbmc; first and only parameter is UTC time

LOGFILE="/dev/null"
TIMER="$1"
echo "$0 was called with: $@" >>$LOGFILE
echo "wakeup time is: $(date -d @$TIMER)" >>$LOGFILE

#check whether system clock is utc or local time
DIFF_SECS=$(expr $(cat /sys/class/rtc/rtc0/since_epoch) - $(date -u +%s))
[ "$DIFF_SECS" -lt 0 ] && DIFF_SECS=$(expr $DIFF_SECS \* -1)

if [ "$DIFF_SECS" -gt 1680 ]; then # more than ca. 30min difference, assume local time
  echo "RTC is local time" >>$LOGFILE
  TIMER=$(date -u -d "$(date -d @$TIMER +%F" "%T)" +%s)
else # assume utc
  echo "RTC is in UTC" >>$LOGFILE
fi

echo 0 > /sys/class/rtc/rtc0/wakealarm
echo $TIMER >/sys/class/rtc/rtc0/wakealarm

cat /proc/driver/rtc >>$LOGFILE

exit 0
