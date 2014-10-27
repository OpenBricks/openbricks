#!/bin/bash

shopt -s expand_aliases

liv0=`mktemp -t liv0.XXXXXX`

cd /usr/bin
/usr/bin/svdrpsend REMO off
if [ -f /etc/vdr/plugins.d/50_softhddevice ]; then
        /usr/bin/svdrpsend PLUG softhddevice SUSP
fi

if [ -f /etc/vdr/plugins.d/50_xineliboutput ]; then
        systemctl stop vdr-sxfe.service
fi

if [ -f /etc/vdr/plugins.d/50_xine ]; then
        systemctl stop vdr-xine.service
fi


XX=`xdpyinfo | grep dimensions | awk '{print $2}' | awk 'BEGIN { FS = "x" }; {print $1}'`
	case $XX in
	720)  RIS=3 ;; # NTSC-VGA
	768)  RIS=3 ;; # PAL
	800)  RIS=3 ;; # VIRTUAL MACHINE
	1024) RIS=4 ;; # XGA
	1280) RIS=5 ;; # HD READY
	1440) RIS=5 ;; # ??
	1920) RIS=6 ;; # HD
	*) RIS=3 ;; # ALTRI CASI
	esac

# RIS e’ il fattore moltiplicativo per le dimensioni delle finestre

export AA=$(( $RIS * 6 ))         # Era 18
export BB=$(( $RIS * 15 ))        # Era 45
export CC=$(( $RIS * 7 ))         # Era 21
export DD=$(( $RIS * 24 ))        # Era 72
export EE=$(( $RIS * 35 ))        # Era 105

alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --cancel-label "Exit"'

while [ 1 ]
do
XDIALOG --backtitle "SifBox Configurator" --menubox "Please choose one of this options" $AA $BB $CC \
	"1" "Configuration of the network card" \
	"2" "Configuration of the audio card" \
	"3" "Configuration of the services" \
	"4" "Packages managment" \
	"0" "Exit" 2> $liv0
if [ $? -eq 1 ]; then break; fi
selection=`cat $liv0`
case $selection in
1) /bin/bash /usr/bin/SifBoxNetConf.sh ;;
2) /bin/bash /usr/bin/SifBoxAudioConf.sh ;;
3) /bin/bash /usr/bin/SifBoxSerConf.sh ;;
4) /bin/bash /usr/bin/SifBoxPackagesConf.sh ;;
0) break ;;
esac
done
rm -f liv0
/usr/bin/svdrpsend REMO on
if [ -f /etc/vdr/plugins.d/50_xineliboutput ]; then
        systemctl start vdr-sxfe.service
fi
if [ -f /etc/vdr/plugins.d/50_xine ]; then
        systemctl start vdr-xine.service
fi
if [ -f /etc/vdr/plugins.d/50_softhddevice ]; then
        /usr/bin/svdrpsend PLUG softhddevice RESU
fi
