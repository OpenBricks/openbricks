#!/bin/sh
PARAMETER1="$1"
PORT="$2"
URL="http://127.0.0.1/video.cgi"
TITLE="webcam"
IMAGE="bild.jpg"

FIFO=/tmp/web.avi
LOG=/dev/null

{
rm -f "$FIFO"
mkfifo "$FIFO"
mplayer -dumpstream "$URL" \
-quiet -nolirc -noautosub -noconsolecontrols -novideo -nojoystick \
-dumpfile "$FIFO" &
# Time to connect and fill pipe
sleep 3 

# Build stream from audiodump with cycle image as video (e.g. webcam)
# PID 0x100/256 = Video 0x101/257 = Audio
ffmpeg -v -1 \
 -i "$FIFO" -r 0.5 -loop_input -i '$IMAGE' \
 -title "$TITLE" \
 -f mpegts -intra -r 24 -vcodec mpeg2video -b 500k -s 352x288 \
 -acodec mp2 -ac 2 -ab 96k -ar 48000 \
  "udp://127.0.0.1:${PORT}?pkt_size=16356"
rm -f "$FIFO"
} > $LOG 2>&1

