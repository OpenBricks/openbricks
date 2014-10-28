#!/bin/sh
PARAMETER1="$1"
PORT="$2"
exec vlc "http://127.0.0.1/video.cgi" --sout "#transcode{vcodec=mp2v,acodec=mpga,vb=2400,ab=320}:standard{access=udp,mux=ts{pid-video=1,pid-audio=2,pid-spu=3},dst=127.0.0.1:$PORT}" --intf dummy 


