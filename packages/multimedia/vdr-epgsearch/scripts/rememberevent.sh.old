#
# rememberevent.sh - v.0.1
#
# source: http://vdrportal.de/board/thread.php?threadid=27095
#
# add this lines to your epgsearchcmds.conf:
# folgende zeilen in die epgsearchcmds.conf eintragen:
#
# remember event?               : /usr/local/bin/rememberevent.sh 0
# remember event? (inc. switch) : /usr/local/bin/rememberevent.sh 1

# CONFIG START
  MINSBEFORE=1 # minutes before event for announcement
# CONFIG END

PATH=$PATH:/usr/bin

# translate to format HH:MM
switch_time=`date -d "1970-01-01 UTC $3 seconds" +"%H:%M"`

# seconds since 1970-01-01 minus minutes before remembering
secs=$(($3-$MINSBEFORE*60))

# now in seconds since 1970-01-01
secs_now=`date +%s`

if [ $secs -le $secs_now ]; then
    if grep -qs "^OSDLanguage = 1" /etc/vdr/setup.conf ; then
        echo "/usr/lib/vdr/svdrpsend.pl MESG '$2 lät schon!'" | at now
    else
        echo "/usr/lib/vdr/svdrpsend.pl MESG '$2 already runs!'" | at now
    fi
    exit
fi

# translate to format HH:MM
announce_time=`date -d "1970-01-01 UTC $secs seconds" +"%H:%M"`

# add command to at queue for announcement
echo "/usr/lib/vdr/svdrpsend.pl MESG '$switch_time: $2'" | at $announce_time

# also switch at event begin?
if [ $1 -eq 1 ]; then
    # add command to at queue
    echo "/usr/lib/vdr/svdrpsend.pl CHAN $5" | at $switch_time
else
    # announce again
    if grep -qs "^OSDLanguage = 1" /etc/vdr/setup.conf ; then
        echo "/usr/lib/vdr/svdrpsend.pl MESG '$2 beginnt!'" | at $switch_time
    else
        echo "/usr/lib/vdr/svdrpsend.pl MESG '$2 starts!'" | at $switch_time
    fi
fi

