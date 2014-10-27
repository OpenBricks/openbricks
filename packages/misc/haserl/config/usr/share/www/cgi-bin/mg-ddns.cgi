#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_ddns_sel(){
    if [ "$CUR_DDNS" = "$1" ] ; then
        echo "selected"
    fi
}

set_ddns_provider(){
	set_config ddns_provider ${1}
    sed -ri "s,^dyndns_system.*,dyndns_system ${1},g" /etc/inadyn.conf 
}

set_ddns_alias(){
	set_config ddns_alias ${1}
    sed -ri "s,^alias.*,alias ${1},g" /etc/inadyn.conf 
}

set_ddns_user(){
	set_config ddns_user ${1}
    sed -ri "s,^username.*,username ${1},g" /etc/inadyn.conf 
}

set_ddns_pass(){
	set_config ddns_pass ${1}
    sed -ri "s,^password.*,password ${1},g" /etc/inadyn.conf 
}


set_ddnsinfo(){
	set_config ddns_enable ${1}
	if [ ${1} = "1" ] ; then
		set_ddns_provider ${2}
		set_ddns_alias ${3}
		set_ddns_user ${4}
		set_ddns_pass ${5}
	fi
}

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    DDNS_ENABLE=$(echo ${FORM_ddns_enable} | cut -d ' ' -f 1)
    DDNS_PROVIDER=$(echo ${FORM_ddns_provider} | cut -d ' ' -f 1)
    DDNS_ALIAS=$(echo ${FORM_ddns_alias} | cut -d ' ' -f 1)
    DDNS_USER=$(echo ${FORM_ddns_user} | cut -d ' ' -f 1)
    DDNS_PASS=$(echo ${FORM_ddns_pass} | cut -d ' ' -f 1)

    case "$ACTION" in
      Apply)
      	set_ddnsinfo ${DDNS_ENABLE:-0} $DDNS_PROVIDER $DDNS_ALIAS $DDNS_USER $DDNS_PASS
		/etc/init.d/ddns restart > /dev/null
		sleep 1
    	;;
      Start)
      	/etc/init.d/ddns start > /dev/null
		sleep 1
    	;;
      Stop)
      	/etc/init.d/ddns stop > /dev/null
		sleep 1
    	;;
      Restart)
      	/etc/init.d/ddns restart > /dev/null
		sleep 1
    	;;
      *)
    esac
fi

CUR_DDNS=$(get_config ddns_provider)

%>
<script language="JavaScript">
<!-- //
function checkDDNS(){                                                           
    if ( <% get_config ddns_enable %> == 0 ){
        document.forms[0].ddns_provider.disabled = true;
        document.forms[0].ddns_alias.disabled = true;
        document.forms[0].ddns_user.disabled = true;
        document.forms[0].ddns_pass.disabled = true;
    }
}
function setReadDDNS(obj){
    if(obj.checked)
    {
        document.forms[0].ddns_provider.disabled = false;
        document.forms[0].ddns_alias.disabled = false;
        document.forms[0].ddns_user.disabled = false;
        document.forms[0].ddns_pass.disabled = false;
        } 
    else {
        document.forms[0].ddns_provider.disabled = true;
        document.forms[0].ddns_alias.disabled = true;
        document.forms[0].ddns_user.disabled = true;
        document.forms[0].ddns_pass.disabled = true;
        }
} 

function validateAction(form) {                                             
    var action = btnAction.value;
    var question = "";
	if (action == 'Apply')
        question = "You will apply the new settings. Do you want to continue?";
	else
		return true;
    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
} 


// -->
</script>
<meta http-equiv='refresh' content='90'>
</head>
<body onload="checkDDNS();">                            

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
	<TABLE border="0" >
	<TR><TH>Enable DDNS:</TH><TD><input type="checkbox" name="ddns_enable" value="1" <% is_checked $(get_config ddns_enable) %> onclick="setReadDDNS(this);"/></TD></TR>
	<TR><TH>DDNS Provider:</TH><TD> <select name="ddns_provider">
		<option value="dyndns@dyndns.org" <%= $(get_ddns_sel "dyndns@dyndns.org") %>>dyndns.org Standard</option>
		<option value="statdns@dyndns.org" <%= $(get_ddns_sel "statdns@dyndns.org") %>>dyndns.org StatDNS</option>
		<option value="custom@dyndns.org" <%= $(get_ddns_sel "custom@dyndns.org") %>>dyndns.org Custom</option>
		<option value="default@zoneedit.com" <%= $(get_ddns_sel "default@zoneedit.com") %>>zoneedit.com</option>
		<option value="default@no-ip.com" <%= $(get_ddns_sel "default@no-ip.com") %>>no-ip.com</option>
    </select></TD></TR>
	<TR><TH>DDNS Alias:</TH><TD><input type="text" name="ddns_alias" size=50 value="<% get_config ddns_alias %>" title="Type the alias of this host on DDNS System"/></TD></TR>
	<TR><TH>DDNS Username:</TH><TD><input type="text" name="ddns_user" size=50 value="<% get_config ddns_user %>" title="Type the username to authenticate on DDNS System"/></TD></TR>
	<TR><TH>DDNS Password:</TH><TD><input type="password" name="ddns_pass" size=50 value="<% get_config ddns_pass %>" title="Type the password to authenticate on DDNS System"/></TD></TR>

    <TR><TH>DDNS status: </TH><TD><% /etc/init.d/ddns webstatus %></TD></TR>

</TABLE>
   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Start" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Restart" onclick="btnAction=this">
</form>
<br><br>                                                                        
<b>DDNS Log</b>                                         
</center>                                                                       
<% grep "^I:\|^W:\|^E:\|^Date:" /var/inadyn.log | tail -n 40 | while read line ; do echo $line"<br>" ; done %> 

</body>
</html>
