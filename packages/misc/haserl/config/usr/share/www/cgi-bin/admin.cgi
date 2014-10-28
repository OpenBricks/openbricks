#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<script language="JavaScript">
<!-- //
function validatePass(pwd1,pwd2) {
    var re = /^\w{5,14}$/;
    if (pwd1 == '' || pwd2 == '') {
        alert('Please enter your password twice.');
        return false;
    }

    if (pwd1 != pwd2) {
        alert ("You did not enter the same new password twice. Please re-enter your password.");
        return false;
    }
   
    if (!re.test(pwd1)) { 
        alert("Please enter a valid password! The password must have a minimum of 5 and a maximum of 14 alphanumeric chars.");
        return false;
   }
   var answer = confirm ("You are about to change web admin, transmission and ssh passwords. Are you sure?");
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;
}

// -->
</script>
</head>
<body>                            
<%

set_admin_password(){
    sed -ri "s/rpc-password\":.*/rpc-password\": \"${1}\",/g" /etc/default/settings.json
    sed -ri "s,/:admin:.*,/:admin:${1},g" /etc/httpd.conf 
	set_config password ${1}
    echo root:${1} | chpasswd -m > /dev/null                            
    killall -hup httpd > /dev/null
}


if [ "${REQUEST_METHOD}" = "POST" ]
then
    NEWPWD=$(echo ${FORM_pass1} | cut -d ' ' -f 1)
    set_admin_password $NEWPWD
    echo "New password set.<br>"
fi
%>

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validatePass(this.pass1.value, this.pass2.value);">
     <TABLE border="0" >
     <TR><TH>New Password:</TH><TD><input type="password" name="pass1" size=20 title="Enter a new Admin Password. Minimum of 5 and a maximum of 15 alphanumeric chars."></TD></TR>
   	 <TR><TH>Repeat Password:</TH><TD><input type="password" name="pass2" size=20 title="Repeat your new Admin Password here."></TD></TR>
     </TABLE>
   <input type="submit" name="newpwd" value="Apply">
</form>

</body>
</html>
