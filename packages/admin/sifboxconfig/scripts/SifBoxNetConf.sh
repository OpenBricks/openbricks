#!/bin/bash

shopt -s expand_aliases

liv0=`mktemp -t liv0.XXXXXX`
temp=`mktemp -t temp.XXXXXX`

# LEGGI FILE
NETWORK=`cat /etc/network | grep NETWORK | cut -d'"' -f2`
IFACE=`cat /etc/network | grep IFACE | cut -d'"' -f2`
ADDRESS=`cat /etc/network | grep ADDRESS | cut -d'"' -f2`
GATEWAY=`cat /etc/network | grep GATEWAY | cut -d'"' -f2`
DNS_SERVER=`cat /etc/network | grep DNS_SERVER | cut -d'"' -f2`
NET_PREFIX=`cat /etc/network | grep NET_PREFIX | cut -d'=' -f2`
SSID=`cat /etc/network | grep SSID | cut -d'"' -f2`
HIDDEN=`cat /etc/network | grep HIDDEN | cut -d'"' -f2`
SECURITY=`cat /etc/network | grep SECURITY | cut -d'"' -f2`
PASSPHRASE=`cat /etc/network | grep PASSPHRASE | cut -d'"' -f2`

alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --backtitle "SifBox Network Configurator"'

while [ 1 ]
do
XDIALOG --cancel-label "Back" --menubox "Please choose one of this wizards" $AA $BB $CC \
	1 "Cable with static IP" \
	2 "Cable with DHCP" \
	3 "WiFi with static IP" \
	4 "Wifi with DHCP" \
	5 "Save" \
	0 "Exit" 2> $liv0
if [ $? -eq 1 ]; then break; fi
selection=`cat $liv0`
case $selection in
1)	NETWORK="LAN"
	XDIALOG --no-cancel --ok-label "Next" --msgbox "Cable with static IP :\n\n
	Press Next to proceed with the Wizard" $AA $BB $CC
	INTERFACES=`ifconfig -a | sed 's/[ \t].*//;/^$/d' | sed ':l; N; s/\n/ /; tl'`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Please, insert the Interface ID\n\
	Available values : \n$INTERFACES\n\n
	Now the Interface name is $IFACE" $AA $BB $IFACE 2>temp
	IFACE=`sed -n '1p' temp`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "IP Address" $AA $BB $ADDRESS 2>temp
	ADDRESS=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Gateway" $AA $BB $GATEWAY 2>temp
	GATEWAY=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "Subnet Mask" $AA $BB \
	"255.255.255.252 = /30" "255.255.255.248 = /29" "255.255.255.240 = /28" \
	"255.255.255.224 = /27" "255.255.255.192 = /26" "255.255.255.128 = /25" \
	"255.255.255.0   = /24" "255.255.254.0   = /23" "255.255.252.0   = /22" \
	"255.255.248.0   = /21" "255.255.240.0   = /20" "255.255.224.0   = /19" \
	"255.255.192.0   = /18" "255.255.128.0   = /17" "255.255.0.0     = /16" \
	"255.254.0.0     = /15" "255.252.0.0     = /14" "255.248.0.0     = /13" \
	"255.240.0.0     = /12" "255.224.0.0     = /11" "255.192.0.0     = /10" \
	"255.128.0.0     = /9" "255.0.0.0       = /8" "254.0.0.0       = /7" \
	"252.0.0.0       = /6" "248.0.0.0       = /5" "240.0.0.0       = /4" \
	"224.0.0.0       = /3" "192.0.0.0       = /2" "128.0.0.0       = /1" 2> tmp.txt
	NET_PREFIX=$(awk 'BEGIN { FS = "/" } ; { print $2 }' tmp.txt)	
	XDIALOG --no-cancel --ok-label "Next" --inputbox "DNS server" $AA $BB $DNS_SERVER 2>temp
	DNS_SERVER=$(cat temp) ;;
2)	NETWORK="LAN"
	ADDRESS=""
	GATEWAY=""
	NET_PREFIX=""
	DNS_SERVER=""
	INTERFACES=`ifconfig -a | sed 's/[ \t].*//;/^$/d' | sed ':l; N; s/\n/ /; tl'`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Please, insert the Interface ID\n\
	Available values : \n$INTERFACES\n
	Now the Interface name is $IFACE" $AA $BB $IFACE 2>temp
	IFACE=`sed -n '1p' temp` ;;
3)	NETWORK="WLAN"
	XDIALOG --no-cancel --ok-label "Next" --msgbox "WiFi with static IP :\n\n
	Press Next to proceed with the Wizard" $AA $BB $CC
	INTERFACES=`ifconfig -a | sed 's/[ \t].*//;/^$/d' | sed ':l; N; s/\n/ /; tl'`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Please, insert the Interface ID\n\
	Available values : \n$INTERFACES\n
	Now the Interface name is $IFACE" $AA $BB $IFACE 2>temp
	IFACE=`sed -n '1p' temp`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "IP Address" $AA $BB $ADDRESS 2>temp
	ADDRESS=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Gateway" $AA $BB $GATEWAY 2>temp
	GATEWAY=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "Subnet Mask" $AA $BB \
	"255.255.255.252 = /30" "255.255.255.248 = /29" "255.255.255.240 = /28" \
	"255.255.255.224 = /27" "255.255.255.192 = /26" "255.255.255.128 = /25" \
	"255.255.255.0   = /24" "255.255.254.0   = /23" "255.255.252.0   = /22" \
	"255.255.248.0   = /21" "255.255.240.0   = /20" "255.255.224.0   = /19" \
	"255.255.192.0   = /18" "255.255.128.0   = /17" "255.255.0.0     = /16" \
	"255.254.0.0     = /15" "255.252.0.0     = /14" "255.248.0.0     = /13" \
	"255.240.0.0     = /12" "255.224.0.0     = /11" "255.192.0.0     = /10" \
	"255.128.0.0     = /9" "255.0.0.0       = /8" "254.0.0.0       = /7" \
	"252.0.0.0       = /6" "248.0.0.0       = /5" "240.0.0.0       = /4" \
	"224.0.0.0       = /3" "192.0.0.0       = /2" "128.0.0.0       = /1" 2> tmp.txt
	NET_PREFIX=$(awk 'BEGIN { FS = "/" } ; { print $2 }' tmp.txt)
	XDIALOG --no-cancel --ok-label "Next" --inputbox "DNS server" $AA $BB $DNS_SERVER 2>temp
	DNS_SERVER=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --inputbox "SSID" $AA $BB $SSID 2>temp
        SSID=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "HIDDEN" $AA $BB \
	"true" "false" 2> temp
	HIDDEN=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "SECURITY" $AA $BB \
	"WEP" "PSK" "RSN" 2> temp
	SECURITY=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --password --inputbox "PASSPHRASE" $AA $BB $PASSPHRASE 2>temp
	PASSPHRASE=$(cat temp) ;;
4)	NETWORK="WLAN"
	ADDRESS=""
	GATEWAY=""
	NET_PREFIX=""
	DNS_SERVER=""
	XDIALOG --no-cancel --ok-label "Next" --msgbox "WiFi with dynamic IP :\n\n
	Press Next to proceed with the Wizard" $AA $BB $CC
	INTERFACES=`ifconfig -a | sed 's/[ \t].*//;/^$/d' | sed ':l; N; s/\n/ /; tl'`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "Please, insert the Interface ID\n\
	Available values : \n$INTERFACES\n
	Now the Interface name is $IFACE" $AA $BB $IFACE 2>temp
	IFACE=`sed -n '1p' temp`
	XDIALOG --no-cancel --ok-label "Next" --inputbox "SSID" $AA $BB $SSID 2>temp
	SSID=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "HIDDEN" $AA $BB \
	"true" "false" 2> temp
	HIDDEN=$(cat temp)
	XDIALOG --no-cancel --ok-label "Next" --combobox "SECURITY" $AA $BB \
	"WEP" "PSK" "RSN" 2> temp
        SECURITY=$(cat temp)	
	XDIALOG --no-cancel --ok-label "Next" --password --inputbox "PASSPHRASE" $AA $BB $PASSPHRASE 2>temp
	PASSPHRASE=$(cat temp) ;;
5)	sed -i 's/NETWORK="[^"]*"/NETWORK="'$NETWORK'"/' /etc/network
	sed -i 's/IFACE="[^"]*"/IFACE="'$IFACE'"/' /etc/network
	sed -i 's/ADDRESS="[^"]*"/ADDRESS="'$ADDRESS'"/' /etc/network
	sed -i 's/GATEWAY="[^"]*"/GATEWAY="'$GATEWAY'"/' /etc/network
	sed -i 's/DNS_SERVER="[^"]*"/DNS_SERVER="'$DNS_SERVER'"/' /etc/network
	sed -i 's/NET_PREFIX="[^"]*"/NET_PREFIX='$NET_PREFIX'/' /etc/network
	sed -i 's/SSID="[^"]*"/SSID="'$SSID'"/' /etc/network
	sed -i 's/HIDDEN="[^"]*"/HIDDEN="'$HIDDEN'"/' /etc/network
	sed -i 's/SECURITY="[^"]*"/SECURITY="'$SECURITY'"/' /etc/network
	sed -i 's/PASSPHRASE="[^"]*"/PASSPHRASE="'$PASSPHRASE'"/' /etc/network
	systemctl restart connman.service
	XDIALOG --no-cancel --msgbox "Configuration Saved !\n\nServices restarted !" $AA $BB ;;
0) 	break ;;
*) 	XDIALOG --no-cancel --msgbox "Sorry, invalid selection" $AA $BB ;;
esac
done
rm -f temp
