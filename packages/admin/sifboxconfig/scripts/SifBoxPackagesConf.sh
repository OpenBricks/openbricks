#!/bin/bash

shopt -s expand_aliases

liv0=`mktemp -t liv0.XXXXXX`

alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --backtitle "SifBox Packages installation Tool"'
alias XTERM='/usr/bin/xterm -fg blue -cr gray10 -selfg blue3 -bd blue3 -bg gray15'

while [ 1 ]
do
XDIALOG --cancel-label "Exit" \
	--menubox "Please, choose one of this options" $CC $DD $AA \
	"1" "Update the packages list (opkg update)" \
	"2" "Installation of one or more packages" \
	"3" "Removal of one or more packages" \
	"4" "List of all packages installed" \
	"5" "List of all packages available" \
	"6" "Check for available updates" \
	"7" "Upgrade one or more packages" \
	"8" "Perform a distro upgrade (opkg upgrade)" \
	"0" "Exit" 2> $liv0
if [ $? -eq 1 ]; then break; fi
selection=`cat $liv0`         # Assegna alla variabile selection il valore scelto dal menu 
case $selection in
1) # AGGIORNAMENTO DELLA LISTA DEI PACCHETTI
        XDIALOG --backtitle "SifBox Packages installation Tool" \
        --yesno "Do you really want to perform 
        the packages list update ? 
        (opkg update)" $AA $BB
                case $? in
                        0) XTERM -e opkg update 
                        XDIALOG --msgbox "Packages list updated!" $AA $BB ;;
                        1) echo ;;
                esac ;;
2) # INSTALLAZIONE DI UNO O PIU' PACCHETTI 
	XDIALOG --no-cancel \
	--inputbox "Please insert a filter\n\nBlank for all packages" $AA $BB 2>temp
	temp=`cat temp` # Assegna il valore del filtro alla variabile temp
	if [ -z "$temp" ]; then temp="-" ; fi # Se il filtro e' vuoto gli viene assegnato il valore - che e' presente in tutti i pacchetti
	# Inizio creazione dello script dinamico
	echo '#!/bin/bash' >TS.sh
	echo 'shopt -s expand_aliases' >>TS.sh
	echo "alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --backtitle "SifBox Packages installation Tool"'" >>TS.sh
	echo 'XDIALOG --no-cancel --backtitle "SifBox Packages installation Tool" --separator "\n" --buildlist "Installation of one or more packages" $BB $EE $CC \' >>TS.sh
	opkg info | awk '/Package:|Version:|Status:/' \
	| sed 's/Package: //g' | sed 's/Version://g' | sed 's/Status://g' \
	| sed 'N;N;s/\n/\t/g;' | grep "unknown ok not-installed" | sed 's/unknown ok not-installed//g' \
	| sort | awk '{ printf "\"%s\" \"%s--->ver_%s\" \"on\" \\\n",$1,$1,$2 }'| grep $temp >>TS.sh
	echo '2> ListaPacchetti' >>TS.sh
	wc -l TS.sh | awk '{print $1}' > NumLines  # Assegno a al file il numero delle righe di TS.sh
	NumLines=`cat NumLines`                    # Trasformo il file NumLines in una variabile
	if [ $NumLines -gt 3 ] 
		then 
			chmod +x TS.sh
			bash ./TS.sh
			if [ `ls -l ListaPacchetti | awk '{print $5}'` -eq 0 ]
				then
				XDIALOG --msgbox "There are no packages to be installed !" $AA $BB
				else
				echo $(tr '\n' ' ' < ListaPacchetti) > ListaPacchetti
				XDIALOG --left --fill --yesno "Do you want to install the following packages??\n\n
`cat ListaPacchetti`" $AA $DD
				case $? in
					0) XTERM -e opkg install $(cat ListaPacchetti) 
			   		restart vdr ;;
					1) echo ;;
				esac 
			fi 
		else 
			XDIALOG --msgbox "The list is empty" $AA $BB		
	fi ;;
3) # DISINSTALLAZIONE DI UNO O PIU' PACCHETTI
	XDIALOG --no-cancel \
	--inputbox "Please insert a filter\n\nBlank for all packages" $AA $BB 2>temp
	temp=`cat temp`
	if [ -z "$temp" ]; then temp="-" ; fi # SE IL FILTRO E' VUOTO
	echo '#!/bin/bash' >TS.sh
	echo 'shopt -s expand_aliases' >>TS.sh
	echo "alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --backtitle "SifBox Packages installation Tool"'" >>TS.sh
	echo 'XDIALOG --no-cancel --backtitle "SifBox Packages installation Tool" --separator "\n" --buildlist "Removal of one or more packages" $BB $EE $CC \' >>TS.sh
	opkg list-installed | awk 'BEGIN{ FS=" - " }{printf "\"%s\" \"%s---->ver_%s\" \"on\" \\\n",$1,$1,$2}' \
	| grep $temp >>TS.sh
	echo '2> ListaPacchetti' >>TS.sh
	wc -l TS.sh | awk '{print $1}' > NumLines  # Assegno a al file il numero delle righe di TS.sh
	NumLines=`cat NumLines`                    # Trasformo il file NumLines in una variabile
	if [ $NumLines -gt 3 ]
		then
			chmod +x TS.sh
			bash ./TS.sh
			if [ `ls -l ListaPacchetti | awk '{print $5}'` -eq 0 ]
				then
				XDIALOG --msgbox "There are no packages to be removed !" $AA $BB
				else
				echo $(tr '\n' ' ' < ListaPacchetti) > ListaPacchetti
				XDIALOG --left --fill --yesno "Do you want to remove the following packages??\n\n
`cat ListaPacchetti`" $AA $DD
				case $? in
					0) XTERM -e opkg remove  $(cat ListaPacchetti) ;;
				1) echo ;; 
				esac 
			fi 
		else 
			XDIALOG --msgbox "The list is empty" $AA $BB
	fi ;;
4) # LISTA DI TUTTI I PACCHETTI INSTALLATI
	XDIALOG --no-cancel --inputbox "Please insert a filter\n\nBlank for all packages" $AA $BB 2>temp
	temp=`cat temp`
	if [ -z "$temp" ]; then temp="-" ; fi
	opkg list-installed | grep $temp | sort > ListaPacchetti
	if [ `ls -l ListaPacchetti | awk '{print $5}'` -eq 0 ]
		then
		XDIALOG --msgbox "The list with the given filter is empty" $AA $BB
		else
		XDIALOG --backtitle "SifBox Packages installation Tool
	
List of all installed packages" --no-cancel --textbox ListaPacchetti $BB $BB
	fi;;

5) # LISTA DI TUTTI I PACCHETTI DISPONIBILI
	XDIALOG --no-cancel --inputbox "Please insert a filter\n\nBlank for all packages" $AA $BB 2>temp
	temp=`cat temp`
	if [ -z "$temp" ]; then temp=$temp ; else temp=*$temp* ; fi
        opkg info $temp > ListaPacchetti
        XDIALOG --backtitle "SifBox Packages installation Tool
        
List of all available packages with detailed info" --no-cancel --textbox ListaPacchetti $BB $DD ;;

6) # LISTA DI TUTTI I PACCHETTI CON AGGIORNAMENTI DISPONIBILI 
	echo "The following packages have available updates :
" > ListaPacchetti
	opkg list-upgradable >> ListaPacchetti
	XDIALOG --backtitle "SifBox Packages installation Tool" --no-cancel --textbox ListaPacchetti $BB $BB ;;
7) # UPGRADE DI UNA LISTA DI PACCHETTI;;
	opkg list-upgradable > temp
	if [ `ls -l temp | awk '{print $5}'` -eq 0 ]; then 
	XDIALOG --msgbox "There are no packages to be updated !" $AA $BB 
	else
	echo '#!/bin/bash' >TS.sh
	echo 'shopt -s expand_aliases' >>TS.sh
	echo "alias XDIALOG='Xdialog --rc-file SifBox.rc --buttons-style text --backtitle "SifBox Packages installation Tool"'" >>TS.sh
	echo 'XDIALOG --separator "\n" --buildlist "Upgrade one or more packages" $BB $EE $CC \' >>TS.sh
	opkg list-upgradable \
	| awk 'BEGIN {FS = " - "}; {printf "\"%s\" \"%s  fromVer:%s  toVer:%s\" \"on\" \\\n",$1,$1,$2,$3}' >>TS.sh
	echo '2> ListaPacchetti' >>TS.sh
	chmod +x TS.sh
	bash ./TS.sh
	echo $(tr '\n' ' ' < ListaPacchetti) > ListaPacchetti
	XDIALOG --no-cancel --backtitle "SifBox Packages installation Tool" --yesno "Do you want to upgrade the following packages??\n\n
	`cat ListaPacchetti`" $AA $DD
	case $? in
		0) XTERM -e opkg upgrade $(cat ListaPacchetti) ;;
	        1) echo ;;
	esac 
	fi ;;
8) # DISTRO UPGRADE
	XDIALOG --backtitle "SifBox Packages installation Tool" --no-cancel --fill \
	--yesno "Do you really want to perform a distro upgrade?" $AA $BB
	case $? in
	        0) XTERM -e opkg upgrade ;;
	        1) echo ;;
	esac ;;

0) break ;;
esac
done

rm -f ListaPacchetti
rm -f TS.sh
rm -f liv0
rm -f temp
rm -f NumLines
