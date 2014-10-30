#!/bin/bash
#
# externremux.sh

# CONFIG START
  STREAMQUALITY="UMTS" # COPY, UMTS, DSL{1,2,3,6}000, DSL3500, LAN10, WLAN{11,54}, IPAQ
  RECDIR=/media/video/
  TMP=/tmp/externremux-${RANDOM:-$$}
# CONFIG END

# DVD abspielen:
# /Extern;STREAMQUALITY:dvd:dvdfile

# Aufzeichnungen abspielen
# /Extern;STREAMQUALITY:rec:dirname to search
# /Extern;STREAMQUALITY:file:filename to search

mkdir -p $TMP
mkfifo $TMP/out.avi
(trap "rm -rf $TMP" EXIT HUP INT TERM ABRT; cat $TMP/out.avi) &

case ${1:0:1} in
     [A-Z]) IFS=: CMD=($1) IFS= ;;
         *) IFS=: CMD=("$STREAMQUALITY" $1) IFS= ;;
esac

case ${CMD[1]} in
     d*) RECORDING="dvd://${CMD[2]}"
         ;;
     r*) REC=$(eval find $RECDIR -name 20*.rec | grep -m 1 -i "${CMD[2]}")
         if [ -d "$REC" ] ; then
             RECORDING="$REC/[0-9][0-9][0-9].vdr"
         fi
         ;;
     f*) REC=$(eval find $RECDIR | grep -m 1 -i "${CMD[2]}")
         if [ -f "$REC" ] ; then
             RECORDING="$REC"
         fi
         ;;
esac

case $CMD in
        H265) exec mencoder $RECORDING -ovc x264  \
                -x264encopts bitrate=200:vbv_maxrate=250:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac faac -faacopts br=32 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_150) exec mencoder $RECORDING -ovc x264 -srate 22050 \
                -x264encopts bitrate=150:vbv_maxrate=180:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

 H264_200_15) exec mencoder $RECORDING -ovc x264 -srate 22050 -ofps 15 \
                -x264encopts bitrate=200:vbv_maxrate=250:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_200) exec mencoder $RECORDING -ovc x264 -srate 22050 \
                -x264encopts bitrate=200:vbv_maxrate=250:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_250) exec mencoder $RECORDING -ovc x264 -srate 22050 \
                -x264encopts bitrate=250:vbv_maxrate=280:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_400) exec mencoder $RECORDING -ovc x264 -srate 22050 \
                -x264encopts bitrate=400:vbv_maxrate=450:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_500) exec mencoder $RECORDING -ovc x264 -srate 44100 \
                -x264encopts bitrate=500:vbv_maxrate=550:vbv_bufsize=600:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 480 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_600) exec mencoder $RECORDING -ovc x264 -srate 44100 \
                -x264encopts bitrate=600:vbv_maxrate=650:vbv_bufsize=600:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 480 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    H264_800) exec mencoder $RECORDING -ovc x264 -srate 22050 \
                -x264encopts bitrate=800:vbv_maxrate=850:vbv_bufsize=300:ratetol=0.1:threads=3 \
                -oac mp3lame -lameopts cbr:br=16:q=2:mode=3 -vf pp=ci,scale -zoom -xy 320 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

        UMTS) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=150 \
                -oac lavc -vf scale=320:240 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

       HANDY) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=350 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=320:240 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

     DSL1000) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=100 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=160:104 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

     DSL2000) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=128 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=160:104 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

     DSL3000) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=250 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=320:208 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

     DSL3500) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=300 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=320:208 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

     DSL6000) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=350 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=320:208 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

    DSL16000) exec mencoder $RECORDING [21~-ovc lavc -lavcopts vcodec=mpeg4:vbitrate=500 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale -zoom -xy 480 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

       LAN10) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=4096 \
                -oac mp3lame -lameopts preset=standard \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

      WLAN11) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=768 \
                -oac mp3lame -lameopts preset=standard -vf scale=640:408 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

      WLAN54) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=2048 \
                -oac mp3lame -lameopts preset=standard \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

        IPAQ) exec mencoder $RECORDING -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=350 \
                -oac mp3lame -lameopts preset=15:mode=3 -vf scale=320:208 \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;

        COPY) exec mencoder $RECORDING -of mpeg -ovc copy -oac copy -mpegopts format=mpeg \
                -o $TMP/out.avi -- - &>$TMP/out.log ;;
           *) touch $TMP/out.avi ;;
esac

