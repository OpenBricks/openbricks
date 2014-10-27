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
      	/bin/systemctl start vdr.service > /dev/null
      	sleep 1
    	;;
      Stop)
      	/bin/systemctl stop vdr.service > /dev/null
      	sleep 1
    	;;
      Restart)
      	/bin/systemctl restart vdr.service > /dev/null
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
        question = "This action stop Vdr service. Do you want to continue?";
    else if  ( action == 'Restart' )
        question = "This action restart Vdr service. Do you want to continue?";
    
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
	  <TR><TD>Vdr Service</TD><TD><input type="submit" name="action" value="Start" onclick="btnAction=this"></TD><TD><input type="submit" name="action" value="Stop" onclick="btnAction=this"></TD><TD><input type="submit" name="action" value="Restart" onclick="btnAction=this"></TD><TH>Status:</TH><TD><%= $(/bin/systemctl status vdr.service | grep active | cut -d ":" -f 2 | cut -d "(" -f 1) %></TD></TR>
	 </TABLE> 
</form>


</body>
</html>
