#!/bin/bash

shopt -s expand_aliases

temp=`mktemp -t liv0.XXXXXX`

# LEGGI FILE
ALSA_CARD=`cat /etc/audio | grep ALSA_CARD | cut -d'"' -f2`
SOUNDCARD_MODE=`cat /etc/audio | grep SOUNDCARD_MODE | cut -d'"' -f2`
SOUNDCARD_PT_MODE=`cat /etc/audio | grep SOUNDCARD_PT_MODE | cut -d'"' -f2`
AC97_SPSA=`cat /etc/audio | grep AC97_SPSA | cut -d'"' -f2`
CHANNELS=`cat /etc/audio | grep CHANNELS | cut -d'"' -f2`
SBL_AUDIGY=`cat /etc/audio | grep SBL_AUDIGY | cut -d'"' -f2`
HAVE_AMPLY=`cat /etc/audio | grep HAVE_AMPLY | cut -d'"' -f2`


alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --left --backtitle "SifBox Audio Configurator"'

XDIALOG --cancel-label "Back" --ok-label "Next" \
	--yesno "Press Next to proceed with the Wizard" $AA $DD
case $? in
0) 	XDIALOG --no-cancel --ok-label "Next" --combobox "Please select the soundcard to use
	by ID (0 for first card, 1 for second, ...)\n\n
	ALSA_CARD = $ALSA_CARD\n" $AA $DD "0" "1" "2" "3" 2>temp
	ALSA_CARD=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "Output mode :
	analog
	digital
	hdmi\n\n
	digital is used for SPDIF/IEC958 output\n\n
	SOUNDCARD_MODE = $SOUNDCARD_MODE" $AA $DD "analog" "digital" "hdmi" 2>temp
	SOUNDCARD_MODE=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "Passthrough mode for digital/hdmi outputs
	none : software audio decoding for all streams
	ac3 : enable hardware passthrough for AC-3 streams only
	dts : enable hardware passthrough for DTS streams only
	ac3dts : enable hardware passthrough for both AC-3
	            and DTS streams
	(Note: in analog mode this option is ignored)\n\n
	SOUNDCARD_PT_MODE=$SOUNDCARD_PT_MODE" $AA $DD "none" "ac3" "dts" "ac3dts" 2>temp
	SOUNDCARD_PT_MODE=$(cat temp)

	XDIALOG --no-cancel --ok-label "Next" --combobox "IEC958 Playback AC97-SPSA Mode:
	0 --> PCM1
	1 --> PCM2,PCM1 (rear)
	2 --> Centre and LFE
	3 --> PCM3,Modem,Dedicated S/PDIF
	For SPDIF most users will want 0, but some
	users may need 3\n\n
	AC97_SPSA = $AC97_SPSA" $AA $DD "0" "1" "2" "3" 2>temp
	AC97_SPSA=$(cat temp)

	XDIALOG --no-cancel --ok-label "Next" --combobox "Playback channels
	2 --> Stereo
	4 --> Surround
	6 --> Full 5.1
	note: in SPDIF and hardware AC3 decoder mode 
	this option is ignored\n\n
	CAHANNELS = $CHANNELS" $AA $DD "2" "4" "6" 2>temp
	CHANNELS=$(cat temp)

	XDIALOG --no-cancel --ok-label "Next" --combobox "SB Live/Audigy Analog/Digital
	Output Mode:
	0 --> Suitable for some older SB Live! cards
	1 --> Suitable for newer SB Live! and all Audigy cards\n\n
	SBL_AUDIGY = $SBL_AUDIGY" $AA $DD "0" "1" 2>temp
	SBL_AUDIGY=$(cat temp)

	XDIALOG --no-cancel --ok-label "Next" --combobox "Specify if you have an amplifier
	0 --> No
	1 --> YES\n\n
	HAVE_AMPLY = $HAVE_AMPLY" $AA $DD "0" "1" 2>temp
	HAVE_AMPLY=$(cat temp)
	                                        
	XDIALOG --yesno "Do you want to save the configuration 
	with the following values?\n\n
	ALSA_CARD = $ALSA_CARD
	SOUNDCARD_MODE = $SOUNDCARD_MODE
	SOUNDCARD_PT_MODE = $SOUNDCARD_PT_MODE
	AC97_SPSA = $AC97_SPSA
	CHANNELS = $CHANNELS
	SBL_AUDIGY = $SBL_AUDIGY
	HAVE_AMPLY =$HAVE_AMPLY" $AA $DD

	case $? in
		0) 
		sed -i 's/ALSA_CARD="[^"]*"/ALSA_CARD="'$ALSA_CARD'"/' /etc/audio
		sed -i 's/SOUNDCARD_MODE="[^"]*"/SOUNDCARD_MODE="'$SOUNDCARD_MODE'"/' /etc/audio
		sed -i 's/SOUNDCARD_PT_MODE="[^"]*"/SOUNDCARD_PT_MODE="'$SOUNDCARD_PT_MODE'"/' /etc/audio
		sed -i 's/AC97_SPSA="[^"]*"/AC97_SPSA="'$AC97_SPSA'"/' /etc/audio
		sed -i 's/CHANNELS="[^"]*"/CHANNELS="'$CHANNELS'"/' /etc/audio
		sed -i 's/SBL_AUDIGY="[^"]*"/SBL_AUDIGY="'$SBL_AUDIGY'"/' /etc/audio
		sed -i 's/HAVE_AMPLY="[^"]*"/HAVE_AMPLY="'$HAVE_AMPLY'"/' /etc/audio
		systemctl restart mixer
		XDIALOG --msgbox "Configuration Saved !\n\nServices restarted!" $AA $DD ;;
		1) 
		XDIALOG --yesno "Do you want to restart the Wizard?" $AA $DD
		case $? in
			0) `$0`;;
			1) echo ;;
		esac ;;

	esac 
	rm temp ;;
1)	echo ;;
esac
