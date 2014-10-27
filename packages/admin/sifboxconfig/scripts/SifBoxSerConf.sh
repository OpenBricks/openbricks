#!/bin/bash

shopt -s expand_aliases

temp=`mktemp -t temp.XXXXXX`
# LEGGI FILE
TELNET_SERVER=`cat /etc/network | grep TELNET_SERVER | cut -d'=' -f2`
FTP_SERVER=`cat /etc/network | grep FTP_SERVER | cut -d'=' -f2`
HTTP_SERVER=`cat /etc/network | grep HTTP_SERVER | cut -d'=' -f2`
SAMBA_SERVER=`cat /etc/network | grep SAMBA_SERVER | cut -d'=' -f2`

alias XDIALOG='Xdialog --rc-file SifBox.rc --backtitle "SifBox Services Configurator" --buttons-style text'

XDIALOG --ok-label "Next" --cancel-label "Back" --yesno "Press Next to proceed with the Wizard" $AA $BB

case $? in
0)      XDIALOG --left --no-cancel --ok-label "Next" --combobox "Do you want to start TELNET service?\n\n
	TELNET_SERVER = $TELNET_SERVER" $AA $BB "true" "false" 2>temp
	TELNET_SERVER=$(cat temp)

	XDIALOG --left --no-cancel --ok-label "Next" --combobox "Do you want to start FTP service?\n\n
	FTP_SERVER = $FTP_SERVER" $AA $BB "true" "false" 2>temp
	FTP_SERVER=$(cat temp)

	XDIALOG --left --no-cancel --ok-label "Next" --combobox "Do you want to start HTTP service?\n\n
	HTTP_SERVER = $HTTP_SERVER" $AA $BB "true" "false" 2>temp
	HTTP_SERVER=$(cat temp)

	XDIALOG --left --no-cancel --ok-label "Next" --combobox "Do you want to start SAMBA service?\n\n
	SAMBA_SERVER = $SAMBA_SERVER" $AA $BB "true" "false" 2>temp
	SAMBA_SERVER=$(cat temp)

	XDIALOG --left --yesno "Do you want to save the configuration
	with the following values?\n\n
	TELNET_SERVER = $TELNET_SERVER
	FTP_SERVER = $FTP_SERVER
	HTTP_SERVER = $HTTP_SERVER
	SAMBA_SERVER = $SAMBA_SERVER" $AA $BB
	case $? in
		0)
		sed -i 's/\(TELNET_SERVER=true\|TELNET_SERVER=false\)/TELNET_SERVER='"$TELNET_SERVER"'/' /etc/network
		sed -i 's/\(FTP_SERVER=true\|FTP_SERVER=false\)/FTP_SERVER='"$FTP_SERVER"'/' /etc/network
		sed -i 's/\(HTTP_SERVER=true\|HTTP_SERVER=false\)/HTTP_SERVER='"$HTTP_SERVER"'/' /etc/network
		sed -i 's/\(SAMBA_SERVER=true\|SAMBA_SERVER=false\)/SAMBA_SERVER='"$SAMBA_SERVER"'/' /etc/network
		systemctl restart connman.service smbd.service
		XDIALOG --msgbox " Configuration Saved !\n\nServices restarted !" $AA $BB ;;
		1)
		XDIALOG --left --yesno "Do you want to restart the Wizard?" $AA $BB
		case $? in
			0) `$0` ;;
			1) echo ;;
		esac ;;
	esac 
	rm -f temp ;;
1) echo ;; 
esac
