#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>

<script language="JavaScript">
<!-- //

function validateAction(form) {                                             
	var action = btnAction.value;
    var question = "";

	if ( action == 'Apply' ){		
		if (!validateCard(form.ALSA_CARD.value,'Alsa Card Value'))
			return false;
		if (!validateOutput(form.SOUNDCARD_MODE.value,'Soundcard Mode Value'))
			return false;
		if (!validateOutput(form.SOUNDCARD_PT_MODE.value,'Passthrough Mode Value'))
			return false;
		if (!validateOutput(form.AC97_SPSA.value,'AC97 Value'))
			return false;
		if (!validateOutput(form.CHANNELS.value,'CHANNELS Value'))
			return false;
		if (!validateOutput(form.SBL_AUDIGY.value,'SB Live/Audigy Analog/Digital Output Mode'))
			return false;
		if (!validateOutput(form.HAVE_AMPLY.value,'HAVE Amply'))
			return false;
			
		question = "This will restart Alsa Mixer service. Are you sure?";
	}
    else if ( action == 'Start' )
    	return true;
    else if ( action == 'Stop' )
        question = "This action stop Alsa Mixer service?";
    else if  ( action == 'Restart' )
        question = "This action restart Alsa Mixer service?";
    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
} 

function validateCard(card,description){
	var re = /^\d{0,10}$/;

	if (card == '') {
		alert('Please enter a valid card for '+description+'. Accepted card numbers are between 0 and 10.');
		return false;
	}
	if (!re.test(card)) { 
		alert('Please enter a valid card for '+description+'. Accepted card numbers are between 0 and 10.');
		return false;
	}
	if (card < 0 || card > 10) { 
		alert('Please enter a valid card for '+description+'. Accepted card numbers are between 0 and 10.');
		return false;
	}
	return true;                                                 
}

function validateOutput(output,description){
	var re = /^\w{hdmi||analog||digital}$/;

	if (output == '') {
		alert('Please enter a valid output for '+description+'. Accepted mode are analog, digital or hdmi.');
		return false;
	}
	if (!re.test(output)) { 
		alert('Please enter a valid output for '+description+'. Accepted mode are analog, digital or hdmi.');
		return false;
	}
	if (output == hdmi || output == analog || output == digital) { 
		alert('Please enter a valid output for '+description+'. Accepted mode are analog, digital or hdmi.');
		return false;
	} 
	return true;
}

function validatePassthrough(passthrough,description){
	var re = /^\w{none||ac3||dts||ac3dts}$/;

	if (passthrough == '') {
		alert('Please enter a valid passthrough for '+description+'. Accepted mode are none, ac3, dts or ac3dts.');
		return false;
	}
	if (!re.test(passthrough)) { 
		alert('Please enter a valid passthrough for '+description+'. Accepted mode are none, ac3, dts or ac3dts.');
		return false;
	}
	if (passthrough == none || passthrough == ac3 || passthrough == dts || passthrough == ac3dts) { 
		alert('Please enter a valid passthrough for '+description+'. Accepted mode are none, ac3, dts or ac3dts.');
		return false;
	}
	return true;                                                 
}

function validateAc97(ac97,description){
	var re = /^\d{0,3}$/;

	if (ac97 == '') {
		alert('Please enter a valid ac97 for '+description+'. Accepted ac97 numbers are between 0 and 3.');
		return false;
	}
	if (!re.test(ac97)) { 
		alert('Please enter a valid ac97 for '+description+'. Accepted ac97 numbers are between 0 and 3.');
		return false;
	}
	if (ac97 < 0 || ac97 > 3) { 
		alert('Please enter a valid ac97 for '+description+'. Accepted ac97 numbers are between 0 and 3.');
		return false;
	}
	return true;                                                 
}

function validateChannels(channels,description){
	var re = /^\d{2,6}$/;

	if (channels == '') {
		alert('Please enter a valid channels for '+description+'. Accepted channels numbers are between 2 and 6.');
		return false;
	}
	if (!re.test(channels)) { 
		alert('Please enter a valid channels for '+description+'. Accepted channels numbers are between 2 and 6.');
		return false;
	}
	if (channels < 2 || channels > 6) { 
		alert('Please enter a valid channels for '+description+'. Accepted channels numbers are between 2 and 6.');
		return false;
	}
	return true;                                                 
}

function validateSblaudigy(sblaudigy,description){
	var re = /^\d{0,1}$/;

	if (sblaudigy == '') {
		alert('Please enter a valid sblaudigy for '+description+'. Accepted sblaudigy numbers are between 0 and 1.');
		return false;
	}
	if (!re.test(sblaudigy)) { 
		alert('Please enter a valid sblaudigy for '+description+'. Accepted sblaudigy numbers are between 0 and 1.');
		return false;
	}
	if (sblaudigy < 0 || sblaudigy > 1) { 
		alert('Please enter a valid sblaudigy for '+description+'. Accepted sblaudigy numbers are between 0 and 1.');
		return false;
	}
	return true;                                                 
}

function validateHaveamply(haveamply,description){
	var re = /^\d{0,1}$/;

	if (haveamply == '') {
		alert('Please enter a valid haveamply for '+description+'. Accepted haveamply numbers are between 0 and 1.');
		return false;
	}
	if (!re.test(haveamply)) { 
		alert('Please enter a valid haveamply for '+description+'. Accepted haveamply numbers are between 0 and 1.');
		return false;
	}
	if (haveamply < 0 || haveamply > 1) { 
		alert('Please enter a valid haveamply for '+description+'. Accepted haveamply numbers are between 0 and 1.');
		return false;
	}
	return true;                                                 
}

// -->
</script>
</head>
<meta http-equiv='refresh' content='90'>
<body>                            

<center>

<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
      Apply)
      	NEWCARD=$(echo ${FORM_ALSA_CARD} | cut -d ' ' -f 1)
      	set_config_mixer ALSA_CARD ${NEWCARD}

		NEWOUTPUT=$(echo ${FORM_SOUNDCARD_MODE} | cut -d ' ' -f 1)
      	set_config_mixer SOUNDCARD_MODE ${NEWOUTPUT}
		
		NEWPASSTHROUGH=$(echo ${FORM_SOUNDCARD_PT_MODE} | cut -d ' ' -f 1)
      	set_config_mixer SOUNDCARD_PT_MODE ${NEWPASSTHROUGH}
		
		NEWAC97=$(echo ${FORM_AC97_SPSA} | cut -d ' ' -f 1)
      	set_config_mixer AC97_SPSA ${NEWAC97}
		
		NEWCHANNELS=$(echo ${FORM_CHANNELS} | cut -d ' ' -f 1)
      	set_config_mixer CHANNELS ${NEWCHANNELS}
		
		NEWSBLAUDIGY=$(echo ${FORM_SBL_AUDIGY} | cut -d ' ' -f 1)
      	set_config_mixer SBL_AUDIGY ${NEWSBLAUDIGY}
		
		NEWHAVEAMPLY=$(echo ${FORM_HAVE_AMPLY} | cut -d ' ' -f 1)
      	set_config_mixer HAVE_AMPLY ${NEWHAVEAMPLY}
		
      	/bin/systemctl restart mixer.service > /dev/null
      	sleep 1
    	;;
      Start)
      	/bin/systemctl start mixer.service > /dev/null
      	sleep 1
    	;;
      Stop)
      	/bin/systemctl stop mixer.service > /dev/null
      	sleep 1
    	;;
      Restart)
      	/bin/systemctl restart mixer.service > /dev/null
      	sleep 1
    	;;
      *)
    esac
fi
%>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
    <TABLE border="0" >
		<TR><TH>Alsa Card ID:</TH><TD><input type="text" name="ALSA_CARD" size=4 value="<% get_config_mixer ALSA_CARD %>" title="Enter a valid card ID. Accepted card numbers are between 0 and 10."></TD></TR>
		<TR><TH>Soundcard Mode:</TH><TD><input type="text" name="SOUNDCARD_MODE" size=4 value="<% get_config_mixer SOUNDCARD_MODE %>" title="Enter a valid Soundcard Mode. Accepted mode are analog, digital or hdmi."></TD></TR>
		<TR><TH>Passthrough Mode:</TH><TD><input type="text" name="SOUNDCARD_PT_MODE" size=4 value="<% get_config_mixer SOUNDCARD_PT_MODE %>" title="Enter a valid Soundcard Mode. Accepted mode are analog, digital or hdmi."></TD></TR>
		<TR><TH>Ac97 ID:</TH><TD><input type="text" name="AC97_SPSA" size=4 value="<% get_config_mixer AC97_SPSA %>" title="Enter a valid ac97 ID. Accepted ac97 numbers are between 0 and 3."></TD></TR>
		<TR><TH>CHANNELS Value:</TH><TD><input type="text" name="CHANNELS" size=4 value="<% get_config_mixer CHANNELS %>" title="Enter a valid Channels ID. Accepted Channels numbers are between 2 and 6."></TD></TR>
		<TR><TH>SB Live/Audigy Analog/Digital Output Mode:</TH><TD><input type="text" name="SBL_AUDIGY" size=4 value="<% get_config_mixer SBL_AUDIGY %>" title="Enter a valid SBL_AUDIGY ID. Accepted SBL_AUDIGY numbers are between 0 and 1."></TD></TR>
		<TR><TH>HAVE Amply:</TH><TD><input type="text" name="HAVE_AMPLY" size=4 value="<% get_config_mixer HAVE_AMPLY %>" title="Enter a valid HAVE_AMPLY ID. Accepted HAVE_AMPLY numbers are between 0 and 1."></TD></TR>
		
	<TR><TH>Status:</TH><TD><%= $(/bin/systemctl status mixer.service | grep active | cut -d ":" -f 2 | cut -d "(" -f 1) %></TD></TR>
</TABLE>

   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">
      
</form>


</body>
</html>
