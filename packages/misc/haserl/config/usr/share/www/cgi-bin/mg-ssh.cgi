#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>

<script language="JavaScript">
<!-- //

var http_port = "";

function validateAction(form) {                                             
	http_port = form.http_port.value;
    var action = btnAction.value;
    var question = "";

	if ( action == 'Apply' ){		
		if (!validatePort(form.ssh_port.value,'SSH Server Port'))
			return false;

		if (!validateTimeout(form.ssh_timeout.value,'SSH Timeout'))
			return false;

		if (!validateTimeout(form.ssh_keepalive.value,'SSH Keepalive'))
			return false;

		question = "This will restart ssh server service. Are you sure?";
	}
    else if ( action == 'Start' )
    	return true;
    else if ( action == 'Stop' )
        question = "This action could disconnect remote users. Are you sure that you want to stop SSH/Telnet service?";
    else if  ( action == 'Restart' )
        question = "This action will invalidate previous ssh keys, and may take a few minutes to complete. Are you sure?";
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

function validateTimeout(time,description){
	var re = /^\d{1,5}$/;

	if (time == '' || !re.test(time) || port > 99999) {
		alert('Please enter a valid timeout for '+description+'. Accepted values are between 0 and 99999.');
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
      	NEWPORT=$(echo ${FORM_ssh_port} | cut -d ' ' -f 1)
      	set_config ssh_port ${NEWPORT}

      	KEEPALIVE=$(echo ${FORM_ssh_keepalive} | cut -d ' ' -f 1)
      	set_config ssh_keepalive ${KEEPALIVE}

      	TIMEOUT=$(echo ${FORM_ssh_timeout} | cut -d ' ' -f 1)
      	set_config ssh_timeout ${TIMEOUT}

      	WEAKKEX=$(echo ${FORM_ssh_weakkex} | cut -d ' ' -f 1)
      	set_config ssh_weakkex ${WEAKKEX:-0}

      	/bin/systemctl restart sshd.socket > /dev/null
      	sleep 2
    	;;
      Start)
      	/bin/systemctl start sshd.socket  > /dev/null
      	sleep 2
    	;;
      Stop)
      	/bin/systemctl stop sshd.socket > /dev/null
      	sleep 2
    	;;
      Restart)
      	/bin/systemctl restart sshd.socket > /dev/null
      	sleep 2
    	;;
      *)
    esac
fi
%>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
     <TABLE border="0" >
	<input type="hidden" name="http_port" value="<% get_config http_port %>" >
	<TR><TH>SSH Server Port:</TH><TD><input type="text" name="ssh_port" size=4 value="<% get_config ssh_port %>" title="Enter a the SSH Server port. Accepted port numbers are between 1 and 65535."></TD></TR>
	<TR><TH>Session timeout:</TH><TD><input type="text" name="ssh_timeout" size=4 value="<% get_config ssh_timeout %>" title="Enter the session timeout. Accepted values are between 0 and 99999 (0 to disable)."></TD></TR>     
	<TR><TH>Keepalive:</TH><TD><input type="text" name="ssh_keepalive" size=4 value="<% get_config ssh_keepalive %>" title="Enter the keepalive delay. Accepted values are between 0 and 99999 (0 to disable)."></TD></TR>     
	<TR><TH>Status:</TH><TD><%= $(/bin/systemctl status sshd.socket | grep active | cut -d ":" -f 2 | cut -d "(" -f 1) %></TD>
</TABLE>

   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">
      
</form>

<form action="<%= Telnet_Services %>" method="POST"  onsubmit="return validateActionT(this);">
     <TABLE border="0" >


<TR><TH>Enalbe Telnet Services:</TH><TD><input type="checkbox" name="telnet_enable" value="1" <% is_checked $(get_config telnet_enable) %> /></TD></TR>

</TABLE>

   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">

</form>

</body>
</html>
