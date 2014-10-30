#!/bin/bash
#
# rememberevent.sh
#
# source: vdr-plugin epgsearch
#
# add this lines to your epgsearchcmds.conf:
# folgende zeilen in die epgsearchcmds.conf eintragen:
#
# remember event               : /usr/local/bin/rememberevent.sh 0
# remember event (inc. switch) : /usr/local/bin/rememberevent.sh 1
# show event list              : /usr/local/bin/rememberevent.sh -ls
# remove all events?           : /usr/local/bin/rememberevent.sh -rm

# CONFIG START
  ATD_SPOOL=/var/spool/atjobs

# default settings
  MINSBEFORE=1  # minutes before event for announcement
  COLUMNS=17    # columns for the epg entry
  FORMAT=MET    # date format, eg.: UTC/MET
# CONFIG END

PATH=/usr/local/bin:$PATH

case $1 in
    -ls)
        grep -s ^'#[0-2][0-9]:[0-5][0-9]#' $ATD_SPOOL/* | sort -t. +1 | cut -d'#' -f3
        ;;
    -rm)
        find $ATD_SPOOL -exec grep -qs ^'#[0-2][0-9]:[0-5][0-9]#' \{} \; -exec rm -f \{} \;
        ;;
      *)
        switch_time=`date -d "1970-01-01 $FORMAT $3 seconds" +"%a.%d %H:%M"`
        entry="#${switch_time#* }#$(printf "%-10s%-0s\n" "${6:0:9}" "$switch_time ${2:0:$COLUMNS}")"
        secs=$(($3-$MINSBEFORE*60))
        secs_now=`date +%s`
        if [ $secs -le $secs_now ]; then
            echo "svdrpsend MESG '$2 already runs!' >/dev/null" | at now
        else
            if [ -z "$(find $ATD_SPOOL -exec grep -qs "^$entry$" \{} \; -exec rm -v \{} \;)" ]; then
                at $(date -d "1970-01-01 $FORMAT $secs seconds" +"%H:%M %m/%d/%Y") <<EOT
                svdrpsend MESG '${switch_time#* }: $2' >/dev/null
                sleep $(($MINSBEFORE*60))s
                if [ $1 -eq 1 ] ; then
                    svdrpsend CHAN $5 >/dev/null
                else
                    svdrpsend MESG '$2 starts!' >/dev/null
                fi
$entry
EOT
            fi
        fi
        ;;
esac

