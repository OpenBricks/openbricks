#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

if [ "${REQUEST_METHOD}" = "POST" ]
then
	ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
		Start)
	      	/bin/systemctl start rsyslog.service > /dev/null
			sleep 1
    	;;
		Stop)
      		/bin/systemctl stop  rsyslog.service > /dev/null
			sleep 1
		;;
		Restart)
      		/bin/systemctl restart rsyslog.service > /dev/null
			sleep 1
		;;
		*)
	esac
fi

%>

<script language="JavaScript">
<!-- //

function validateAction(form) {
    var action = btnAction.value;
    var question = "";
    if ( action == 'Start' )
        return true;
    else if ( action == 'Stop' )
        question = "This action stop Syslog service. Do you want to continue?";
    else if  ( action == 'Restart' )
        question = "This action restart Syslog service. Do you want to continue?";
    var answer = confirm (question);
    if (answer)
        return true;
    else
        return false;
}

// -->
</script>
</head>
<body onload="checkSyslogNet();">                            

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
<TABLE border="0" >

	<TR><TH>Syslog status: <TD><%= $(/bin/systemctl status rsyslog.service | grep active | cut -d ":" -f 2 | cut -d "(" -f 1) %></TD></TH>
    </TABLE>
	<input type="submit" name="action" value="Start" onclick="btnAction=this">
	<input type="submit" name="action" value="Stop" onclick="btnAction=this">
	<input type="submit" name="action" value="Restart" onclick="btnAction=this">
</form>
</body>
</html>
