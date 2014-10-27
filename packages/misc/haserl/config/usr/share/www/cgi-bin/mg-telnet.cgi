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
      	/etc/init.d/telnet start > /dev/null
      	sleep 1
    	;;
      Stop)
      	/etc/init.d/telnet stop > /dev/null
      	sleep 1
    	;;
      Restart)
      	/etc/init.d/telnet restart > /dev/null
      	sleep 1
    	;;
      Apply)
        TELNETENABLE=$(echo ${FORM_telnet_enable} | cut -d ' ' -f 1)
      	set_config telnet_enable ${TELNETENABLE:-0}
      	/etc/init.d/telnet restart > /dev/null
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
        question = "This action may disconnect remote users. Are you sure that you want to stop Telnet service?";
    else if  ( action == 'Restart' )
        question = "This action may disconnect remote users. Are you sure that you want to restart Telnet service?";
    else if  ( action == 'Apply' )
        question = "This action may disconnect remote users. Do you want to continue?";

    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
} 


// -->
</script>
</head>
<body>                 

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
     <TABLE border="0" >
	  <TR><TH>Telnet enabled:</TH><TD><input type="checkbox" name="telnet_enable" value="1" <% is_checked $(get_config telnet_enable) %> /></TD></TR>
     <TR><TH>Status:</TH><TD><% /etc/init.d/telnet webstatus %></TD></TR>
</TABLE>
   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">   
</form>


</body>
</html>
