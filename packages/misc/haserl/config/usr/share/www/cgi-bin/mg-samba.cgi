#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_smbsecmode_sel(){
    if [ $SMBSECMODE = $1 ] ; then
        echo -n "selected"
    fi
}

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
      Start)
      	/bin/systemctl start smbd.service > /dev/null
      	sleep 2
    	;;
      Stop)
      	/bin/systemctl stop smbd.service > /dev/null
      	sleep 2
    	;;
      Restart)
      	/bin/systemctl restart smbd.service > /dev/null
      	sleep 2
    	;;
      Apply)
        SMBENABLE=$(echo ${FORM_samba_enable} | cut -d ' ' -f 1)
      	set_config samba_enable ${SMBENABLE:-0}
		if [ ${SMBENABLE:-0} = 1 ] ; then
	      	SECMODE=$(echo ${FORM_security} | cut -d ' ' -f 1)
			set_config smbsecurity ${SECMODE}
			sed -ri "s/security = .*/security = ${SECMODE}/g" /etc/smb.conf
	      	WORKGROUP=$(echo ${FORM_workgroup} | cut -d ' ' -f 1)
			set_config smbworkgroup ${WORKGROUP}
	      	sed -ri "s/workgroup = .*/workgroup = ${WORKGROUP}/g" /etc/smb.conf
		fi
      	/bin/systemctl restart smbd.service > /dev/null
      	sleep 2
    	;;
    	
      *)
    esac
fi

SMBSECMODE=$(get_config smbsecurity)

%>
<script language="JavaScript">
<!-- //

function validateAction(form) {                                             
    var action = btnAction.value;
    var question = "";
    if ( action == 'Start' )
    	return true;
    else if ( action == 'Stop' )
        question = "This action will disconnect remote users. Are you sure that you want to stop Samba service?";
    else if  ( action == 'Restart' )
        question = "This action will disconnect remote users. Are you sure that you want to restart Samba service?";
    else if  ( action == 'Apply' )
        question = "This action will disconnect remote users. Please review your file shares if changing the security mode. Do you want to continue?";

    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
} 
function checkSmb(){              
	if ( <% get_config samba_enable %> == 0 ){
		document.forms[0].workgroup.disabled = true;
		document.forms[0].security.disabled = true;
	}
}

function setReadSmb(obj){
	if(obj.checked){       
		document.forms[0].workgroup.disabled = false;
		document.forms[0].security.disabled = false;
	} 
	else { 
		document.forms[0].workgroup.disabled = true;
		document.forms[0].security.disabled = true;
	}
}   

// -->
</script>
</head>
<body onload="checkSmb();">                 

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
     <TABLE border="0" >
	  <TR><TH>Samba enabled:</TH><TD><input type="checkbox" name="samba_enable" value="1" <% is_checked $(get_config samba_enable) %> onclick="setReadSmb(this);"/></TD></TR>
      <TR><TH>Samba domain/workgroup:</TH><TD><input type="text" name="workgroup" size=20 value=<% get_config smbworkgroup %> </TD>
      <TR><TH>Samba security mode:</TH><TD> <select name="security">
    <%
    OPTIONS="<option value="SHARE" $(get_smbsecmode_sel SHARE)>Password Only or Anonymous</option>"
    OPTIONS=${OPTIONS}"<option value="USER" $(get_smbsecmode_sel USER)>User and Password Required</option>"
    echo ${OPTIONS}
    %>
    </select></TD></TR>
     
     
     <TR><TH>Status:</TH><TD><% /etc/init.d/samba webstatus %></TD></TR>
</TABLE>
   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">   
</form>


</body>
</html>
