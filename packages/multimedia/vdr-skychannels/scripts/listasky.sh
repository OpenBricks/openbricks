#!/bin/bash
getchannels () {
	
	# cerco tid, nid e sid nel file channels.conf
	FOUND=0
	
	TEMPVDR=`cat "$VDRPATH"channels.conf | grep "${3}:${2}:${1}"`
	IFS=':' read -a array <<< "$TEMPVDR"
  	if [ ${#array[@]} == 13 ]; then
  		# se il file ha lo stesso , tid, nid, sid, allora è lo stsso canale e lo copio dal file channles.conf 
		if [ ${array[9]} == $3 ] && [ ${array[10]} == $2 ] && [ ${array[11]} == $1 ] ; then
			# creo i gruppi di canali
			option="${1}" 
			case ${6} in 
			   101) echo ":@101 TV NAZIONALI" >> $CHANNELTEMP
			      ;; 
			   108) echo ":@108 SKY - INTRATTENIMENTO" >> $CHANNELTEMP
			      ;; 
			   200) echo ":@200 SKY - SPORT" >> $CHANNELTEMP
			      ;;
			   250) echo ":@250 SKY - CALCIO" >> $CHANNELTEMP
			      ;;
			   301) echo ":@301 SKY - CINEMA" >> $CHANNELTEMP
			      ;;
			   351) echo ":@351 SKY - PRIMAFILA" >> $CHANNELTEMP
			      ;;
			   401) echo ":@401 SKY - DOCUMENTARI" >> $CHANNELTEMP
			      ;;
			   500) echo ":@500 SKY - NEWS" >> $CHANNELTEMP
			      ;;
			   601) echo ":@601 SKY - BAMBINI" >> $CHANNELTEMP
			      ;;
			   700) echo ":@700 SKY - RADIO" >> $CHANNELTEMP
			      ;;
			   770) echo ":@770 SKY - MUSICA" >> $CHANNELTEMP
			      ;;
			   804) echo ":@804 SKY - ALTRE TV" >> $CHANNELTEMP
			      ;;
			   *) echo ":@$6" >> $CHANNELTEMP
			      ;;                         
			esac 				
			echo "${array[0]}:${4}:${array[2]}:${array[3]}:${5}:${array[5]}:${array[6]}:${array[7]}:${array[8]}:${3}:${2}:${1}:${array[12]}" >> $CHANNELTEMP
			# il canale è stato trovato setto FOUND a 1
			FOUND=1
		fi	
	fi;	
	
	# stampo i dati dei canali non trovati	
	#if [ $FOUND == 0 ] ; then
		#echo $1 $2 $3 $4 $5 $6
	#fi	
	
}

VDRPATH='/etc/vdr/'
PLUGINSPATH='/etc/vdr/plugins/channelssky/'
CHANNELTEMP="$PLUGINSPATH"channels.temp.conf


svdrpsend chan S13.0E-64511-6600-11101
logger -s "inizia la procedura aggiornamento canali SKY"
# grabbo LCN di sky
svdrpsend MESG "Acquisizione LCN SKY"

get_channels_sky

svdrpsend MESG "Acquisizione LCN SKY"

CHANNELSKY=$(</tmp/channels.sky) 

logger -s "salvato file LCN di sky"

svdrpsend MESG "Creazione Lista Canali Attendere"

# leggo il file LCN di sky riga per riga
while read line           
do           	
	IFS='|' read -a array <<< "$line"
  
	if [ ${array[12]} == 1 ] && [ ${array[0]} -lt 65535 ] ; then
		# se il canale non HD cerco i suoi datti in channels conf
		getchannels ${array[3]} ${array[4]} ${array[5]} ${array[6]} ${array[13]} ${array[0]}
	fi	      
done< <(echo "$CHANNELSKY")

logger -s "lista canali temporanea completata"   


#[ $# -eq 0 ] && { echo "Usage: $0 filename"; }
[ ! -f "$CHANNELTEMP" ] && { echo "Error: $0 file not found."; }
 
if [ -s "$CHANNELTEMP" ]
then
	cat "$PLUGINSPATH"channels.pre.conf "$PLUGINSPATH"channels.temp.conf "$PLUGINSPATH"channels.post.conf > "$PLUGINSPATH"channels.sky.conf
	logger -s "lista canali pronta per essere aggiornata"
	svdrpsend MESG "Aggiornamento Completato Riavvio VDR dal MENU"
	
	logger -s "elimino file temporanei"
	`rm -rf "$PLUGINSPATH"channels.temp.conf`
	`rm -rf /tmp/channels.sky`
	
	logger -s "aggiornamento terminato"
	sleep 5  && killall -9 vdr
fi
exit $RET

