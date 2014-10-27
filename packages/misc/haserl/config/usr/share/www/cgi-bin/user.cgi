#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_userlist(){
    for UNAME in $(smbpasswd -list) ; do
		SHARED=$(grep "^$UNAME:" /etc/passwd | cut -d : -f 6)
        echo "<OPTION name=ulist value=\"$UNAME\">$UNAME - $SHARED</OPTION>"
    done
}

initial_userform(){

    %>
<body onload="disableButtons();">

<center>
    <form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">  
    <TABLE border="0" >
    <TR>
    <TH align=center>FTP/Samba Users</TH>
    </TR>
    <TD align=center>
    <SELECT NAME = "ulist" SIZE=5  WIDTH="200" STYLE="width: 200px" onchange="enableButtons();">
    <% get_userlist %>
    </SELECT>
    </TD></TR><TR><TD>
       <input type="submit" name="action" value="New" title="Click here to add a new user." onclick="btnAction=this"/>
       <input type="submit" id=actionEdit name="action" value="Edit" title="Click here to edit the user." onclick="btnAction=this"/>
       <input type="submit" id=actionRemove name="action" value="Remove" title="Click here to remove the user."  onclick="btnAction=this"/>
    </TD></TR></TABLE>
    </form>
    <%

}

new_userform(){
    %>
<body>
<center>
    <form id=user name=user action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateNewUser(this);">
    <TABLE border="1" >
    <TR>
    <TH align=center>User Name</TH>
    <TH align=center>Password</TH>
    <TH align=center>Repeat password</TH>
    <TH align=center>FTP Homedir</TH>
    <TH align=center>Directory access</TH>
    </TR>
    <TR>
    <TD align=center><input type="text" name="uname" maxsize=15 size=15 /></TD>
    <TD align=center><input type="password" name="pass1" maxsize=14 size=14 /></TD>
    <TD align=center><input type="password" name="pass2" maxsize=14 size=14 /></TD>
    <TD><input type="text" name="shared" size=20 value="<%= $SHARED %>" readonly onclick="fileBrowserNew()"/></TD>
    <TD>
    <SELECT NAME="permissions">
        <OPTION value=current selected >Default</OPTION>
        <OPTION value=user >User only</OPTION>
        <OPTION value=allread >All (readonly)</OPTION>
        <OPTION value=all >All</OPTION>
    </SELECT>
    </TD>
    </TR>
    </TABLE>
        <input type=hidden name=permissions_recursive value="">
        <input type="submit" name="action" value="Save" onclick="btnAction=this"/>
        <input type="submit" name="action" value="Cancel" onclick="history.go(-1);return false;"/>
    </form>
	<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
	<input type=hidden name=sourceform value="./user.cgi">
	<input type=hidden name=shared value="">
	<input type=hidden name=auser value="">
	<input type=hidden name=write value="0">
	</form>
    </body>
    </html>
    <%
}

edit_userform(){
    %>
<body>
<center>
    <form id=user name=user action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateEdit(this);">
    <TABLE border="1" >
    <TR>
    <TH align=center>User Name</TH>
    <TH align=center>Password</TH>
    <TH align=center>Repeat password</TH>
    <TH align=center>FTP Homedir</TH>
    <TH align=center>Directory access</TH>
    </TR>
    <TR>
    <TD align=center><input type="text" name=uname  maxsize=15 size=15 value="<%= $UNAME %>" readonly></TD>
    <TD align=center><input type="password" name="pass1" maxsize=14 size=14 /></TD>
    <TD align=center><input type="password" name="pass2" maxsize=14 size=14 /></TD>
    <TD><input type="text" name="shared" size=20 value="<%= $SHARED %>" readonly onclick="fileBrowser()"/></TD>
    <TD>
    <SELECT NAME="permissions">
        <OPTION value=current selected >Default</OPTION>
        <OPTION value=user >User only</OPTION>
        <OPTION value=allread >All (readonly)</OPTION>
        <OPTION value=all >All</OPTION>
    </SELECT>
    </TD>
    </TR>
    </TABLE>
        <input type=hidden name=permissions_recursive value="">
        <input type="submit" name="action" value="Save" onclick="btnAction=this"/>
        <input type="submit" name="action" value="Cancel" onclick="history.go(-1);return false;"/>
    </form>
	<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
	<input type=hidden name=sourceform value="./user.cgi">
	<input type=hidden name=shared value="">
	<input type=hidden name=auser value="<%= $UNAME %>">
	<input type=hidden name=write value="<%= $WRITE %>">
	</form>
    </body>
    </html>
    <%
}
%>
<script language="JavaScript">
<!-- //

function disableButtons(){
	document.getElementById("actionEdit").disabled = true;
	document.getElementById("actionRemove").disabled = true;
}

function enableButtons(){                          
	document.getElementById("actionEdit").disabled = false;
	document.getElementById("actionRemove").disabled = false;
}

function fileBrowserNew(){
	filebrowser.auser.value = user.uname.value;
	filebrowser.submit();
}

function fileBrowser(){
	filebrowser.submit();
}

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
    return true;
}

function validateEdit(form){
	var pwd1 = form.pass1.value;
	var pwd2 = form.pass2.value;
	var write = document.filebrowser.write.value;
	if ( ( pwd1 == "" ) && ( pwd2 == "" ) )
		if ( write == 1 )
			return true;
		else{
			alert("Please enter a valid password! The password must have a minimum of 5 and a maximum of 14 alphanumeric chars.");
        	return false;
		}
	if (validatePass(pwd1,pwd2)){
		var valform = validateAction(form);
		return valform;
	}
	else
		return false;
}

function validateNewUser(form){
	var re = /^\w{5,15}$/;
	var user = form.uname.value;
	var home = form.shared.value;
	if ( home == "" ){
		alert("Please enter a FTP directory for the user!");	
		return false;
	}
	if (!re.test(user)) { 
		alert("Please enter a valid username! The username must have a minimum of 5 and a maximum of 15 chars.");
		return false;
	}
	var pwd1 = form.pass1.value;
	var pwd2 = form.pass2.value;
	if (validatePass(pwd1,pwd2)){
		var valform = validateAction(form);
		return valform;
	}
	else
		return false;
}

function validateAction(form) {
    var action = btnAction.value;                                               
    var user = form.ulist && form.ulist.value || form.uname.value;                                                
    var question = "";                                                          
    var answer = true;             
    if ( action == 'New')
        return true; 
    if ( user == '--invalid--')                                                 
        return false;                                                           
    if ( action == 'Remove'){                                                   
        question = "This will erase the user and associated shares. Do you want to continue?";
        answer = confirm (question);                                            
    }
    if ( action == 'Save'){
        var perms = form.permissions.value;
        if ( perms != "current" ) {
            question = "You have chosen to change the permissions on this users home directory. Do you want to apply these settings to existing files in the directory?"
            if ( confirm(question) ) {
                form.permissions_recursive.value = "1"
            } else {
                form.permissions_recursive.value = ""		    
            }
	}
        question = "This action will save the user. Do you want to continue?";
        var answer = confirm (question);
    }
    if (answer)                                                                 
        return true;                                                            
    else                                                                        
        return false; 
} 


// -->
</script>
</head>

<%

set_permissions(){
    UNAME=${1}
    if ! grep -q ^"${UNAME}": /etc/passwd; then
        echo "set_permissions: invalid username"
        return
    fi

    HOMEDIR=$(readlink -f "${3}")
    if [ -z ${HOMEDIR} ] || [ ! -d "${HOMEDIR}" ] || ! echo "${HOMEDIR}" | grep -q "^/usb/."; then
        echo "set_permissions: invalid directory"
        return    
    fi

    REFLAG=""
    if [ "${4}" == "1" ]; then
	RFLAG="-R"
    fi


    case "${2}" in
      current)
        ;;
      user)
        chown ${RFLAG} ${UNAME}:${UNAME} ${HOMEDIR}
        chmod ${RFLAG} 700 ${HOMEDIR}
        ;;
      allread)
        chown ${RFLAG} ${UNAME}:${UNAME} ${HOMEDIR}
        chmod ${RFLAG} 755 ${HOMEDIR}
        ;;
      all)
        chown ${RFLAG} ${UNAME}:${UNAME} ${HOMEDIR}
        chmod ${RFLAG} 777 ${HOMEDIR}
        ;;
      *)
        echo "set_permissions: invalid permissions"
    esac
}

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)

    case "$ACTION" in
      Edit)
        UNAME=$(echo ${FORM_ulist} | cut -d ' ' -f 1)
        SHARED=$(grep "^$UNAME:" /etc/passwd | cut -d : -f 6)
        WRITE=$(echo ${FORM_write} | cut -d ' ' -f 1)
        WRITE=${WRITE:-1}
        edit_userform
    	;;
      Done)
        UNAME=$(echo ${FORM_auser} | cut -d ' ' -f 1)
        SHARED=$(echo ${FORM_shared} | cut -d '|' -f 1)
        WRITE=$(echo ${FORM_write} | cut -d ' ' -f 1)
        EDIT=$(echo ${FORM_edit} | cut -d ' ' -f 1)
        edit_userform
        ;;
      Save)
        UNAME=$(echo ${FORM_uname} | cut -d ' ' -f 1)
        SHARED=$(echo ${FORM_shared} | cut -d ' ' -f 1)
        PASSWORD=$(echo ${FORM_pass1} | cut -d ' ' -f 1)
        PERMISSIONS=$(echo ${FORM_permissions} | cut -d ' ' -f 1)
        RECURSIVE=$(echo ${FORM_permissions_recursive} | cut -d ' ' -f 1)


        smbpasswd -a $UNAME $SHARED $PASSWORD

        set_permissions ${UNAME} ${PERMISSIONS} ${SHARED} ${RECURSIVE}

        initial_userform
    	;;
      Remove)
        UNAME=$(echo ${FORM_ulist} | cut -d ' ' -f 1)
        smbpasswd -x $UNAME
        smbshare -xuser $UNAME
        initial_userform
    	;;
      New)
        new_userform
        ;;
      *)
        initial_userform
    esac
else 
    initial_userform
fi
%>

</body>
</html>
