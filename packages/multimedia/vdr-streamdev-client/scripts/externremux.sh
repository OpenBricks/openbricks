#!/bin/bash
#
# externremux.sh - sample remux script using mencoder for remuxing.
#
# Install this script as VDRCONFDIR/plugins/streamdev-server/externremux.sh
#
# The parameter QUALITY selects the default remux parameters. Adjust
# to your needs and point your web browser to http://servername:3000/ext/
# To select different remux parameters on the fly, insert a semicolon
# followed by the name and value of the requested parameter, e.g:
#   e.g. http://servername:3000/ext;QUALITY=WLAN11;VBR=512/
# The following parameters are recognized:
#
# PROG   actual remux program
# VC     video codec
# VBR    video bitrate (kbit)
# VOPTS  custom video options
# WIDTH  scale video to width
# HEIGHT scale video to height
# FPS    output frames per second
# AC     audio codec
# ABR    audio bitrate (kbit)
# AOPTS  custom audio options
#

##########################################################################

### GENERAL CONFIG START
###
# Pick one of DSL1000/DSL2000/DSL3000/DSL6000/DSL16000/LAN10/WLAN11/WLAN54
QUALITY='DSL3000'
# Program used for logging (logging disabled if empty)
LOGGER=logger
# Path and name of FIFO
FIFO=/tmp/externremux-${RANDOM:-$$}
# Default remux program (cat/mencoder/ogg)
PROG=mencoder
# Use mono if $ABR is lower than this value
ABR_MONO=64
###
### GENERAL CONFIG END

### MENCODER CONFIG START
###
# mencoder binary
MENCODER=mencoder
# verbosity from all=-1 to all=9 (-msglevel ...)
MENCODER_MSGLEVEL=all=1
### video part
# Default video codec (e.g. lavc/x264/copy)
#MENCODER_VC=lavc
MENCODER_VC=x264
# Default video options if lavc is used (-ovc lavc -lavcopts ...)
MENCODER_LAVC_VOPTS=vcodec=mpeg4
# Default video options if x264 is used (-ovc x264 -x264encopts ...)
MENCODER_X264_VOPTS=threads=auto
### audio part
# Audio language to use if several audio PIDs are available (-alang ...)
MENCODER_ALANG=ita
# Default audio codec (e.g. lavc/mp3lame/faac/copy)
MENCODER_AC=mp3lame
#MENCODER_AC=faac
# Default audio options if lavc is used (-oac lavc -lavcopts ...)
MENCODER_LAVC_AOPTS=acodec=mp2
# Default audio options if mp3lame is used (-oac mp3lame -lameopts ...)
MENCODER_LAME_AOPTS=
# Default audio options if faac is used (-oac faac -faacopts ...)
MENCODER_FAAC_AOPTS=
###
### MENCODER CONFIG END

### OGG CONFIG START
###
# ffmpeg2theora binary 
OGG=ffmpeg2theora
# speedlevel - lower value gives better quality but is slower (0..2)
OGG_SPEED=1
# videoquality - higher value gives better quality but is slower (0..10)
OGG_VQUALITY=0
# audioquality - higher value gives better quality but is slower (0..10)
OGG_AQUALITY=0
# aspect ratio used for scaling if only one of HEIGHT/WIDTH given (16/9 or 4/3)
OGG_ASPECT='4 / 3'
###
### OGG CONFIG END

##########################################################################

function hasOpt { echo "$1" | grep -q "\b${2}\b"; }

# $1:    concatenation of already set option=value pairs
# $2-$n: option=value pairs to be echod if the option is not present in $1
function addOpts
{
	local opts="$1"
	shift
	while [ $# -gt 0 ]; do
		hasOpt "$opts" ${1%%=*}= || echo $1
		shift
	done
}

function isNumeric() { echo "$@" | grep -q '^-\?[0-9]\{1,\}$'; }

function remux_cat
{
	startReply
	exec 3<&0
	cat 0<&3 >"$FIFO" &
}

function remux_mencoder
{
	# lavc may be used for video and audio
	LAVCOPTS=()

	# Assemble video options
	VC=${REMUX_PARAM_VC:-$MENCODER_VC}
	VOPTS=${REMUX_PARAM_VOPTS}
	FPS=${REMUX_PARAM_FPS:-$FPS}

	# if only one of HEIGHT/WIDTH given:
	# have mencoder calculate other value depending on actual aspect ratio
	if [ "$HEIGHT" -a -z "$WIDTH" ]; then
	  WIDTH=-3
	elif [ "$WIDTH" -a -z "$HEIGHT" ]; then
	  HEIGHT=-3
	fi

	case "$VC" in
		lavc)
			LAVCOPTS=(
				${VOPTS}
				$(IFS=$IFS:; addOpts "$VOPTS" $MENCODER_LAVC_VOPTS)
				${VBR:+vbitrate=$VBR}
			)
			[ ${#LAVCOPTS[*]} -gt 0 ] && VOPTS=$(IFS=:; echo -lavcopts "${LAVCOPTS[*]}")
			;; 
		x264)
			isNumeric "$HEIGHT" && [ $HEIGHT -lt 0 -a $HEIGHT -gt -8 ] && ((HEIGHT-=8))
			isNumeric "$WIDTH" && [ $WIDTH -lt 0 -a $WIDTH -gt -8 ] && ((WIDTH-=8))
			X264OPTS=(
				${VOPTS}
				$(IFS=$IFS:; addOpts "$VOPTS" $MENCODER_X264_VOPTS)
				${VBR:+bitrate=$VBR}
			)
			[ ${#X264OPTS[*]} -gt 0 ] && VOPTS=$(IFS=:; echo -x264encopts "${X264OPTS[*]}")
			;;
		copy)
			VOPTS=
			;;
		*)
			error "Unknown video codec '$VC'"
			;;
	esac

	# Assemble audio options 
	AC=${REMUX_PARAM_AC:-$MENCODER_AC}
	AOPTS=${REMUX_PARAM_AOPTS}
	case "$AC" in
		lavc)
			LAVCOPTS=(
				${LAVCOPTS[*]}
				${AOPTS}
				$(IFS=$IFS:; addOpts "$AOPTS" $MENCODER_LAVC_AOPTS)
				${ABR:+abitrate=$ABR}
			)
	
			[ ${#LAVCOPTS[*]} -gt 0 ] && AOPTS=$(IFS=:; echo -lavcopts "${LAVCOPTS[*]}")
			# lavc used for video and audio decoding - wipe out VOPTS as video options became part of AOPTS
			[ "$VC" = lavc ] && VOPTS=
			;; 
		mp3lame)
			LAMEOPTS=(
				${AOPTS}
				$(isNumeric "${ABR}" && [ "${ABR}" -lt "$ABR_MONO" ] && ! hasOpt "${AOPTS}" mode ] && echo 'mode=3')
				$(IFS=$IFS:; addOpts "$AOPTS" $MENCODER_LAME_AOPTS)
				${ABR:+preset=$ABR}
			)
			[ ${#LAMEOPTS[*]} -gt 0 ] && AOPTS=$(IFS=:; echo -lameopts "${LAMEOPTS[*]}")
			;;
		faac)
			FAACOPTS=(
				${AOPTS}
				$(IFS=$IFS:; addOpts "$AOPTS" $MENCODER_FAAC_AOPTS)
				${ABR:+br=$ABR}
			)
			[ ${#FAACOPTS[*]} -gt 0 ] && AOPTS=$(IFS=:; echo -faacopts "${FAACOPTS[*]}")
			;;
		copy)
			AOPTS=
			;;
		*)
			error "Unknown audio codec '$AC'"
			;;
	esac


	startReply
	exec 3<&0
	echo $MENCODER \
		${MENCODER_MSGLEVEL:+-msglevel $MENCODER_MSGLEVEL} \
		-ovc $VC $VOPTS \
		-oac $AC $AOPTS \
		${MENCODER_ALANG:+-alang $MENCODER_ALANG} \
		${WIDTH:+-vf scale=$WIDTH:$HEIGHT -zoom} \
		${FPS:+-ofps $FPS} \
		-o "$FIFO" -- - >&2
	$MENCODER \
		${MENCODER_MSGLEVEL:+-msglevel $MENCODER_MSGLEVEL} \
		-ovc $VC $VOPTS \
		-oac $AC $AOPTS \
		${MENCODER_ALANG:+-alang $MENCODER_ALANG} \
		${WIDTH:+-vf scale=$WIDTH:$HEIGHT -zoom} \
		${FPS:+-ofps $FPS} \
		-o "$FIFO" -- - 0<&3 >/dev/null &
}

function remux_ogg
{
	VOPTS=${REMUX_PARAM_VOPTS//[:=]/ }
	AOPTS=${REMUX_PARAM_AOPTS//[:=]/ }

	# if only one of HEIGHT/WIDTH given:
	# calculate other value depending on configured aspect ratio
	# trim to multiple of 8
	if [ "$HEIGHT" -a -z "$WIDTH" ]; then
	  WIDTH=$((HEIGHT * $OGG_ASPECT / 8 * 8))
	elif [ "$WIDTH" -a -z "$HEIGHT" ]; then
	  HEIGHT=$(($WIDTH * $( echo $OGG_ASPECT | sed 's#^\([0-9]\+\) */ *\([0-9]\+\)$#\2 / \1#') / 8 * 8))
	fi

	OGGOPTS=(
		${VOPTS}
		${VBR:+--videobitrate $VBR}
		$(hasOpt "${VOPTS}" videoquality || echo "--videoquality $OGG_VQUALITY")
		$(hasOpt "${VOPTS}" speedlevel || echo "--speedlevel $OGG_SPEED")
		${AOPTS}
		${ABR:+--audiobitrate $ABR}
		$(isNumeric "${ABR}" && [ "${ABR}" -lt "$ABR_MONO" ] && ! hasOpt "${AOPTS}" channels ] && echo '--channels 1')
		$(hasOpt "${AOPTS}" audioquality || echo "--audioquality $OGG_AQUALITY")
		$(hasOpt "${AOPTS}" audiostream || echo '--audiostream 1')
	)

	startReply
	exec 3<&0
	echo $OGG --format ts \
		${OGGOPTS[*]} \
		${WIDTH:+--width $WIDTH --height $HEIGHT} \
                --title "VDR Streamdev: ${REMUX_CHANNEL_NAME}" \
                --output "$FIFO" -- - 0<&3 >&2
	$OGG --format ts \
		${OGGOPTS[*]} \
		${WIDTH:+--width $WIDTH --height $HEIGHT} \
                --title "VDR Streamdev: ${REMUX_CHANNEL_NAME}" \
                --output "$FIFO" -- - 0<&3 >/dev/null &
}

function error
{
	if [ "$SERVER_PROTOCOL" = HTTP ]; then
		echo -ne "Content-type: text/plain\r\n"
		echo -ne '\r\n'
		echo "$*"
	fi

	echo "$*" >&2
	exit 1
}

function startReply
{
	if [ "$SERVER_PROTOCOL" = HTTP ]; then
		# send content-type and custom headers
		echo -ne "Content-type: ${CONTENTTYPE}\r\n"
		for header in "${HEADER[@]}"; do echo -ne "$header\r\n"; done
		echo -ne '\r\n'

		# abort after headers
		[ "$REQUEST_METHOD" = HEAD ] && exit 0
	fi

	# create FIFO and read from it in the background
	mkfifo "$FIFO"
	trap "trap '' EXIT HUP INT TERM ABRT PIPE CHLD; kill -INT 0; sleep 1; fuser -k '$FIFO'; rm '$FIFO'" EXIT HUP INT TERM ABRT PIPE CHLD
	cat "$FIFO" <&- &
}

HEADER=()

[ "$LOGGER" ] && exec 2> >($LOGGER -t "vdr: [$$] ${0##*/}" 2>&-)

# set default content-types
case "$REMUX_VPID" in
	''|0|1) CONTENTTYPE='audio/mpeg';;
	*)      CONTENTTYPE='video/mpeg';;
esac

QUALITY=${REMUX_PARAM_QUALITY:-$QUALITY}
case "$QUALITY" in
	DSL1000|dsl1000)   VBR=96;   ABR=16;  WIDTH=160;;
	DSL2000|dsl2000)   VBR=128;  ABR=16;  WIDTH=160;;
	DSL3000|dsl3000)   VBR=256;  ABR=16;  WIDTH=320;;
	DSL6000|dsl6000)   VBR=378;  ABR=32;  WIDTH=320;;
	DSL16000|dsl16000) VBR=512;  ABR=32;  WIDTH=480;;
	WLAN11|wlan11)     VBR=768;  ABR=64;  WIDTH=640;;
	WLAN54|wlan54)     VBR=2048; ABR=128; WIDTH=;;
	LAN10|lan10)       VBR=4096; ABR=;    WIDTH=;;
	*)                 error "Unknown quality '$QUALITY'";;
esac
ABR=${REMUX_PARAM_ABR:-$ABR}
VBR=${REMUX_PARAM_VBR:-$VBR}
WIDTH=${REMUX_PARAM_WIDTH:-$WIDTH}
HEIGHT=${REMUX_PARAM_HEIGHT:-$HEIGHT}
PROG=${REMUX_PARAM_PROG:-$PROG}

case "$PROG" in
	cat)      remux_cat;;
	mencoder) remux_mencoder;;
	ogg)      remux_ogg;;
	*)        error "Unknown remuxer '$PROG'";;
esac

set -o monitor
wait
