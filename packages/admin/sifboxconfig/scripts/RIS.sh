#!/bin/bash

if [ -e /usr/bin/SifBoxConfigurator.sh ]
	then
	XX=`xdpyinfo | grep dimensions | awk '{print $2}' | awk 'BEGIN { FS = "x" }; {print $1}'`
	case $XX in
	720)  ris=3 ;; # NTSC-VGA
	768)  ris=3 ;; # PAL
	800)  ris=3 ;; # VIRTUAL MACHINE
	1024) ris=4 ;; # XGA
	1280) ris=5 ;; # HD READY
	1920) ris=6 ;; # HD
	esac
	sed -i "s/RIS=[0-9]/RIS=$ris/g" /usr/bin/SifBoxConfigurator.sh
fi


	
