#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

set_trans_alt(){
	set_config enable_trans_alt ${1}
	set_config trans_alt_speed_up ${2}
	set_config trans_alt_speed_down ${3}
	set_config trans_alt_time_begin ${4}
	set_config trans_alt_time_end ${5}
	set_config trans_alt_time_days ${6}

	if [ ${1} = 1 ] ; then
		ALTSPEED=true 
	else 
		ALTSPEED=false
	fi

	sed -ri "s/alt-speed-time-enabled\":.*/alt-speed-time-enabled\": ${ALTSPEED},/g" /etc/default/settings.json
	sed -ri "s/alt-speed-up\":.*/alt-speed-up\": ${2},/g" /etc/default/settings.json
	sed -ri "s/alt-speed-down\":.*/alt-speed-down\": ${3},/g" /etc/default/settings.json
	sed -ri "s/alt-speed-time-begin\":.*/alt-speed-time-begin\": ${4},/g" /etc/default/settings.json
	sed -ri "s/alt-speed-time-end\":.*/alt-speed-time-end\": ${5},/g" /etc/default/settings.json
	sed -ri "s/alt-speed-time-day\":.*/alt-speed-time-day\": ${6},/g" /etc/default/settings.json
}

set_trans_speed(){

	set_config trans_speed_up_enable ${1}
	set_config trans_speed_up ${2}
	set_config trans_speed_down_enable ${3}
	set_config trans_speed_down ${4}

	if [ ${1} = 1 ] ; then
		UPSPEED=true 
	else 
		UPSPEED=false
	fi

	if [ ${3} = 1 ] ; then
		DOWNSPEED=true 
	else 
		DOWNSPEED=false
	fi

	sed -ri "s/speed-limit-up-enabled\":.*/speed-limit-up-enabled\": ${UPSPEED},/g" /etc/default/settings.json
	sed -ri "s/speed-limit-down-enabled\":.*/speed-limit-down-enabled\": ${DOWNSPEED},/g" /etc/default/settings.json
	sed -ri "s/speed-limit-up\":.*/speed-limit-up\": ${2},/g" /etc/default/settings.json
	sed -ri "s/speed-limit-down\":.*/speed-limit-down\": ${4},/g" /etc/default/settings.json
}

set_peer_limit(){
	set_config trans_peer_limit_global ${1}
	set_config trans_peer_limit_torrent ${2}

	sed -ri "s/peer-limit-global\":.*/peer-limit-global\": ${1},/g" /etc/default/settings.json
	sed -ri "s/peer-limit-per-torrent\":.*/peer-limit-per-torrent\": ${2},/g" /etc/default/settings.json
}

set_ports(){
	set_config trans_peer_port ${1}
	set_config trans_rpc_port ${2}

	sed -ri "s/peer-port\":.*/peer-port\": ${1},/g" /etc/default/settings.json
	sed -ri "s/rpc-port\":.*/rpc-port\": ${2},/g" /etc/default/settings.json
}


disable_trans_alt(){
	sed -ri "s,^enable_trans_alt=.*,enable_trans_alt=0,g" /etc/default/config
	sed -ri "s/alt-speed-time-enabled\":.*/alt-speed-time-enabled\": false,/g" /etc/default/settings.json
}

get_alt_time_begin_sel(){
	ALT_TIME_BEGIN=$(get_config trans_alt_time_begin)
	HOUR=$(printf "%.2d" $((${ALT_TIME_BEGIN}/60)))
	MIN=$(printf "%.2d" $((${ALT_TIME_BEGIN}%60)))
	echo -n "<option value=${ALT_TIME_BEGIN} selected >${HOUR}:${MIN}</option>"
}

get_alt_time_end_sel(){
	ALT_TIME_END=$(get_config trans_alt_time_end)
	HOUR=$(printf "%.2d" $((${ALT_TIME_END}/60)))
	MIN=$(printf "%.2d" $((${ALT_TIME_END}%60)))
	echo -n "<option value=${ALT_TIME_END} selected >${HOUR}:${MIN}</option>"
}


TRANSDIR=$(get_config transmission_dir)

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
      Apply)
		TRANSDIR=$(echo ${FORM_shared} | cut -d ' ' -f 1)
		USE_TRANSMISSION=$(echo ${FORM_use_transmission} | cut -d ' ' -f 1)
		ENABLE_TRANS_ALT=$(echo ${FORM_enable_trans_alt} | cut -d ' ' -f 1)
		set_config use_transmission ${USE_TRANSMISSION:-0}

      	/etc/init.d/transmission stop > /dev/null
      	sleep 2

		PEER_PORT=$(echo ${FORM_peer_port} | cut -d ' ' -f 1)
		RPC_PORT=$(echo ${FORM_rpc_port} | cut -d ' ' -f 1)
		set_ports ${PEER_PORT} ${RPC_PORT}

		PEER_LIMIT_GLOBAL=$(echo ${FORM_peer_limit_global} | cut -d ' ' -f 1)
		PEER_LIMIT_TORRENT=$(echo ${FORM_peer_limit_torrent} | cut -d ' ' -f 1)
		set_peer_limit ${PEER_LIMIT_GLOBAL:-240} ${PEER_LIMIT_TORRENT:-0}

		SPEED_UP_ENABLE=$(echo ${FORM_speed_up_enable} | cut -d ' ' -f 1)
		SPEED_UP=$(echo ${FORM_speed_up} | cut -d ' ' -f 1)
		SPEED_DOWN_ENABLE=$(echo ${FORM_speed_down_enable} | cut -d ' ' -f 1)
		SPEED_DOWN=$(echo ${FORM_speed_down} | cut -d ' ' -f 1)
		set_trans_speed ${SPEED_UP_ENABLE:-0} ${SPEED_UP:-0} ${SPEED_DOWN_ENABLE:-0} ${SPEED_DOWN:-0}

		if [ "${ENABLE_TRANS_ALT}" = 1 ] ; then
			ALT_SPEED_UP=$(echo ${FORM_alt_speed_up} | cut -d ' ' -f 1)
			ALT_SPEED_DOWN=$(echo ${FORM_alt_speed_down} | cut -d ' ' -f 1)
			ALT_TIME_BEGIN=$(echo ${FORM_alt_time_begin} | cut -d ' ' -f 1)
			ALT_TIME_END=$(echo ${FORM_alt_time_end} | cut -d ' ' -f 1)
			FSUN=$(echo ${FORM_sun} | cut -d ' ' -f 1)
			FMON=$(echo ${FORM_mon} | cut -d ' ' -f 1)
			FTUE=$(echo ${FORM_tue} | cut -d ' ' -f 1)
			FWED=$(echo ${FORM_wed} | cut -d ' ' -f 1)
			FTHU=$(echo ${FORM_thu} | cut -d ' ' -f 1)
			FFRY=$(echo ${FORM_fry} | cut -d ' ' -f 1)
			FSAT=$(echo ${FORM_sat} | cut -d ' ' -f 1)
			ALT_TIME_DAYS=$((${FSUN:-0}+${FMON:-0}+${FTUE:-0}+${FWED:-0}+${FTHU:-0}+${FFRY:-0}+${FSAT:-0}))
			set_trans_alt ${ENABLE_TRANS_ALT} ${ALT_SPEED_UP} ${ALT_SPEED_DOWN} ${ALT_TIME_BEGIN} ${ALT_TIME_END} ${ALT_TIME_DAYS}
		else
			disable_trans_alt
		fi
      	set_config transmission_dir ${TRANSDIR}

      	/etc/init.d/transmission start > /dev/null
      	sleep 2
      	;;
      Start)
      	/etc/init.d/transmission start > /dev/null
      	sleep 2
    	;;
      Stop)
      	/etc/init.d/transmission stop > /dev/null
      	sleep 3
    	;;
      Restart)
      	/etc/init.d/transmission restart > /dev/null
      	sleep 2
    	;;
	  Done)
        TRANSDIR=/usb/$(echo ${FORM_shared} | cut -d '|' -f 1)
        ;;
      *)
    esac
fi

IN=$(get_config trans_alt_time_days)

SUN="$(($IN%2))"
IN="$(($IN/2))"
MON="$(($IN%2))"                               
IN="$(($IN/2))"                                
TUE="$(($IN%2))"                               
IN="$(($IN/2))"                                
WED="$(($IN%2))"                               
IN="$(($IN/2))"                                
THU="$(($IN%2))"                               
IN="$(($IN/2))"                                
FRY="$(($IN%2))"                               
IN="$(($IN/2))"                                
SAT="$(($IN%2))"                               
IN="$(($IN/2))"


%>
<script language="JavaScript">
<!-- //

var http_port = "";

function validateAction(form) {                                             
	http_port = form.http_port.value;
	var action = btnAction.value;
	var question = "";

	if ( action == 'Apply' ){
		if (form.use_transmission.checked){
			if (!validatePort(form.peer_port.value,'p2p port'))
				return false;

			if (!validatePort(form.rpc_port.value,'Web port'))
				return false;

			if (form.peer_port.value == form.rpc_port.value){
				alert("p2p port and web port are the same, change one of them");
				return false;		
			}

			if (!validateNumber(form.peer_limit_global.value,'Peer limit global'))
				return false;		
		
			if (!validateNumber(form.peer_limit_torrent.value,'Peer limit per torrent'))
				return false;

			if (form.speed_down_enable.checked && !validateNumber(form.speed_down.value,'Speed Down'))
				return false;

			if (form.speed_up_enable.checked && !validateNumber(form.speed_up.value,'Speed Up'))
				return false;

			if (form.enable_trans_alt.checked){
				if (!validateNumber(form.alt_speed_down.value,'Alternative Speed Down'))
					return false;

				if (!validateNumber(form.alt_speed_up.value,'Alternative Speed Up'))
					return false;
			}
		}	
		question = "This will restart Transmission service and set the new folder. Current downloads will get lost and files may be incomplete, unless you use the old folder again. Are you sure?";
	}
	else if ( action == 'Stop' )
		question = "Are you sure that you want to stop Transmission service?";
	else if  ( action == 'Restart' )
		question = "Are you sure that you want to restart Transmission service?";
	else
		return true;
	
	var answer = confirm (question);
	if (answer)                                                      
		return true;                                                 
	else                                                            
		return false;                                           
}

function validatePort(port,description){
	var re = /^\d{1,5}$/;

	if (port == '') {
		alert('Please enter a valid port for '+description+'. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	if (!re.test(port)) { 
		alert('Please enter a valid port for '+description+'. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	if (port < 1 || port > 65535) { 
		alert('Please enter a valid port for '+description+'. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	if (port == http_port) { 
		alert('You have to use a different port for '+description+' and web admin interface.');
		return false;
	}
	return true;                                                 
}

function validateNumber(number,description){
	var re = /^\d{1,5}$/;

	if (number == '') {
		alert('Please enter a valid number for '+description+'. Accepted numbers are between 1 and 99999.');
		return false;
	}
	if (!re.test(number)) { 
		alert('Please enter a valid number for '+description+'. Accepted numbers are between 1 and 99999.');
		return false;
	}
	if (number < 1 || number > 99999) { 
		alert('Please enter a valid number for '+description+'. Accepted numbers are between 1 and 99999.');
		return false;
	}
	return true;                                    
}

function checkTrans(){
	trans.shared.value = "<%= ${TRANSDIR} %>";
	if ( <% get_config use_transmission %> == 0 ){
		document.forms[0].shared.disabled = true;
		document.forms[1].enable_trans_alt.checked = false;
		document.forms[1].enable_trans_alt.disabled = true;
	}
	setTransUp(document.forms[1].speed_up_enable);
	setTransDown(document.forms[1].speed_down_enable);
	setReadTransAlt(document.forms[1].enable_trans_alt);
}

function fileBrowserNew(){
	filebrowser.submit();
}

function setReadTrans(obj){
	if(obj.checked){       
		document.forms[0].shared.disabled = false;
		document.forms[1].enable_trans_alt.disabled = false;
		document.forms[1].speed_up_enable.disabled = false;
		document.forms[1].speed_down_enable.disabled = false;
		document.forms[1].peer_limit_global.disabled = false;
		document.forms[1].peer_limit_torrent.disabled = false;
		document.forms[1].peer_port.disabled = false;
		document.forms[1].rpc_port.disabled = false;

	} 
	else { 
		document.forms[0].shared.disabled = true;
		document.forms[1].enable_trans_alt.checked = false;
		document.forms[1].enable_trans_alt.disabled = true;
		document.forms[1].speed_up_enable.checked = false;
		document.forms[1].speed_up_enable.disabled = true;
		document.forms[1].speed_down_enable.checked = false;
		document.forms[1].speed_down_enable.disabled = true;
		document.forms[1].peer_limit_global.disabled = true;
		document.forms[1].peer_limit_torrent.disabled = true;
		document.forms[1].peer_port.disabled = true;
		document.forms[1].rpc_port.disabled = true;

	}
	setTransUp(document.forms[1].speed_up_enable);
	setTransDown(document.forms[1].speed_down_enable);
	setReadTransAlt(document.forms[1].enable_trans_alt);
}

function setTransUp(obj){
	if(obj.checked){       
		document.forms[1].speed_up.disabled = false;
	} 
	else { 
		document.forms[1].speed_up.disabled = true;
	} 
}

function setTransDown(obj){
	if(obj.checked){       
		document.forms[1].speed_down.disabled = false;
	} 
	else { 
		document.forms[1].speed_down.disabled = true;
	} 
}


function setReadTransAlt(obj){
	if(obj.checked){       
		document.forms[1].alt_speed_up.disabled = false;
		document.forms[1].alt_speed_down.disabled = false;
		document.forms[1].alt_time_begin.disabled = false;
		document.forms[1].alt_time_end.disabled = false;
		document.forms[1].sun.disabled = false;
		document.forms[1].mon.disabled = false;
		document.forms[1].tue.disabled = false;
		document.forms[1].wed.disabled = false;
		document.forms[1].thu.disabled = false;
		document.forms[1].fry.disabled = false;
		document.forms[1].sat.disabled = false;
	} 
	else { 
		document.forms[1].alt_speed_up.disabled = true;
		document.forms[1].alt_speed_down.disabled = true;
		document.forms[1].alt_time_begin.disabled = true;
		document.forms[1].alt_time_end.disabled = true;
		document.forms[1].sun.disabled = true;
		document.forms[1].mon.disabled = true;
		document.forms[1].tue.disabled = true;
		document.forms[1].wed.disabled = true;
		document.forms[1].thu.disabled = true;
		document.forms[1].fry.disabled = true;
		document.forms[1].sat.disabled = true;
	} 
}


// -->
</script>
</head>

<body onload="checkTrans();" >

<center>
<TABLE border="0">
<form id=filebrowser name=filebrowser action="./filebrowserd.cgi" method="POST">
	<input type=hidden name=sourceform value="./mg-trans.cgi">
    <TR><TH>Current torrent folder:</TH><TD><input type="text" name="shared" size=20 value="<%= $TRANSDIR %>" readonly onclick="fileBrowserNew()"/></TD></TR>
    <TR><TH>Torrent drop folder:</TH><TD><%= $TRANSDIR %>/torrentwatch</TD></TR>
</form> 
<form id=trans name=trans action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
<input type=hidden name=shared value=""> <input type=hidden name="http_port" value="<% get_config http_port %>"> 
  	<TR><TH COLSPAN=2>Transmission - Main Settings</TH></TR>
	<TR><TH>Transmission enabled:</TH><TD><input type="checkbox" name="use_transmission" value="1" <% is_checked $(get_config use_transmission) %> onclick="setReadTrans(this);"/></TD></TR>

	<TR><TH>p2p Port:</TH><TD><INPUT type="text" name="peer_port" size=4 value="<% get_config trans_peer_port %>"></TD></TR>
	<TR><TH>Web Port:</TH><TD><INPUT type="text" name="rpc_port" size=4 value="<% get_config trans_rpc_port %>"></TD></TR>

	<TR><TH>Peer limit global:</TH><TD><INPUT type="text" name="peer_limit_global" size=4 value="<% get_config trans_peer_limit_global %>"></TD></TR>
	<TR><TH>Peer limit per torrent:</TH><TD><INPUT type="text" name="peer_limit_torrent" size=4 value="<% get_config trans_peer_limit_torrent %>"></TD></TR>

	<TR><TH>Speed Down:</TH><TD><input type="checkbox" name="speed_down_enable" value="1" <% is_checked $(get_config trans_speed_down_enable) %> onclick="setTransDown(this);"/><INPUT type="text" name="speed_down" size=4 value="<% get_config trans_speed_down %>"> (KB/s)</TD></TR>
    <TR><TH>Speed Up:</TH><TD><input type="checkbox" name="speed_up_enable" value="1" <% is_checked $(get_config trans_speed_up_enable) %> onclick="setTransUp(this);"/><INPUT type="text" name="speed_up" size=4 value="<% get_config trans_speed_up %>"> (KB/s)</TD></TR>

	<TR><TH COLSPAN=2>Transmission - Alternate Time</TH></TR>
	<TR><TH>Enable Alternate Time:</TH><TD><input type="checkbox" name="enable_trans_alt" value="1" <% is_checked $(get_config enable_trans_alt) %> onclick="setReadTransAlt(this);"/></TD></TR>
	<TR><TH>Alternate Speed Up:</TH><TD><INPUT type="text" name="alt_speed_up" size=4 value="<% get_config trans_alt_speed_up %>"> (KB/s)</TD></TR>
	<TR><TH>Alternate Speed Down:</TH><TD><INPUT type="text" name="alt_speed_down" size=4 value="<% get_config trans_alt_speed_down %>"> (KB/s)</TD></TR>
    <TR><TH>Alternate Speed Up:</TH><TD><INPUT type="text" name="alt_speed_up" size=4 value="<% get_config trans_alt_speed_up %>"> (KB/s)</TD></TR>
	<TR><TH>Alternate Time Begin:</TH><TD>
		<select name="alt_time_begin">
		<% get_alt_time_begin_sel %>
		<option value="0" >00:00</option>
		<option value="30" >00:30</option>
		<option value="60" >01:00</option>
		<option value="90" >01:30</option>
		<option value="120" >02:00</option>
		<option value="150" >02:30</option>
		<option value="180" >03:00</option>
		<option value="210" >03:30</option>
		<option value="240" >04:00</option>
		<option value="270" >04:30</option>
		<option value="300" >05:00</option>
		<option value="330" >05:30</option>
		<option value="360" >06:00</option>
		<option value="390" >06:30</option>
		<option value="420" >07:00</option>
		<option value="450" >07:30</option>
		<option value="480" >08:00</option>
		<option value="510" >08:30</option>
		<option value="540" >09:00</option>
		<option value="570" >09:30</option>
		<option value="600" >10:00</option>
		<option value="630" >10:30</option>
		<option value="660" >11:00</option>
		<option value="690" >11:30</option>
		<option value="720" >12:00</option>
		<option value="750" >12:30</option>
		<option value="780" >13:00</option>
		<option value="810" >13:30</option>
		<option value="840" >14:00</option>
		<option value="870" >14:30</option>
		<option value="900" >15:00</option>
		<option value="930" >15:30</option>
		<option value="960" >16:00</option>
		<option value="990" >16:30</option>
		<option value="1020" >17:00</option>
		<option value="1050" >17:30</option>
		<option value="1080" >18:00</option>
		<option value="1110" >18:30</option>
		<option value="1140" >19:00</option>
		<option value="1170" >19:30</option>
		<option value="1200" >20:00</option>
		<option value="1230" >20:30</option>
		<option value="1260" >21:00</option>
		<option value="1290" >21:30</option>
		<option value="1320" >22:00</option>
		<option value="1350" >22:30</option>
		<option value="1380" >23:00</option>
		<option value="1410" >23:30</option>
		</select></TD></TR>
	<TR><TH>Alternate Time End:</TH><TD><select name="alt_time_end">
		<% get_alt_time_end_sel %>
		<option value="0" >00:00</option>
		<option value="30" >00:30</option>
		<option value="60" >01:00</option>
		<option value="90" >01:30</option>
		<option value="120" >02:00</option>
		<option value="150" >02:30</option>
		<option value="180" >03:00</option>
		<option value="210" >03:30</option>
		<option value="240" >04:00</option>
		<option value="270" >04:30</option>
		<option value="300" >05:00</option>
		<option value="330" >05:30</option>
		<option value="360" >06:00</option>
		<option value="390" >06:30</option>
		<option value="420" >07:00</option>
		<option value="450" >07:30</option>
		<option value="480" >08:00</option>
		<option value="510" >08:30</option>
		<option value="540" >09:00</option>
		<option value="570" >09:30</option>
		<option value="600" >10:00</option>
		<option value="630" >10:30</option>
		<option value="660" >11:00</option>
		<option value="690" >11:30</option>
		<option value="720" >12:00</option>
		<option value="750" >12:30</option>
		<option value="780" >13:00</option>
		<option value="810" >13:30</option>
		<option value="840" >14:00</option>
		<option value="870" >14:30</option>
		<option value="900" >15:00</option>
		<option value="930" >15:30</option>
		<option value="960" >16:00</option>
		<option value="990" >16:30</option>
		<option value="1020" >17:00</option>
		<option value="1050" >17:30</option>
		<option value="1080" >18:00</option>
		<option value="1110" >18:30</option>
		<option value="1140" >19:00</option>
		<option value="1170" >19:30</option>
		<option value="1200" >20:00</option>
		<option value="1230" >20:30</option>
		<option value="1260" >21:00</option>
		<option value="1290" >21:30</option>
		<option value="1320" >22:00</option>
		<option value="1350" >22:30</option>
		<option value="1380" >23:00</option>
		<option value="1410" >23:30</option>
		</select></TD></TR>
  	<TR><TH COLSPAN=2>Transmission - Alternate Time Days</TH></TR>
	<TR><TH>Sun</TH><TD><input type="checkbox" name="sun" value="1" <% is_checked $SUN %> /></TD></TR>
	<TR><TH>Mon</TH><TD><input type="checkbox" name="mon" value="2" <% is_checked $MON %> /></TD></TR>
	<TR><TH>Tue</TH><TD><input type="checkbox" name="tue" value="4" <% is_checked $TUE %> /></TD></TR>
	<TR><TH>Wed</TH><TD><input type="checkbox" name="wed" value="8" <% is_checked $WED %> /></TD></TR>
	<TR><TH>Thu</TH><TD><input type="checkbox" name="thu" value="16" <% is_checked $THU %> /></TD></TR>
	<TR><TH>Fry</TH><TD><input type="checkbox" name="fry" value="32" <% is_checked $FRY %> /></TD></TR>
	<TR><TH>Sat</TH><TD><input type="checkbox" name="sat" value="64" <% is_checked $SAT %> /></TD></TR>
    <TR><TD COLSPAN=2></TD></TR>
    <TR><TH>Status:</TH><TD><% /etc/init.d/transmission webstatus %></TD></TR>
</TABLE>
	<input type="submit" name="action" value="Apply" onclick="btnAction=this">
	<input type="submit" name="action" value="Start" onclick="btnAction=this">
	<input type="submit" name="action" value="Stop" onclick="btnAction=this">
	<input type="submit" name="action" value="Restart" onclick="btnAction=this">   
</form>


</body>
</html>
