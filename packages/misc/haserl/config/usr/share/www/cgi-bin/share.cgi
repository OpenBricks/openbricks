#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%
get_shareinfo(){
    for SHARE in $(smbshare -list) ; do
        SNAME=$(echo $SHARE | cut -d '|' -f 2)
        SHARED=$(echo $SHARE | cut -d '|' -f 3)
        WRITE=$(echo $SHARE | cut -d '|' -f 4)
        AUSER=$(echo $SHARE | cut -d '|' -f 5)
		echo "<TR><TD><input type=radio name=sname value=\"$SNAME\" onclick=\"enableButtons();\">"
        echo "<TD>$SNAME</TD>"
        echo "<TD>$SHARED</TD>"
        echo "<TD align=center>$WRITE</TD>"
        echo "<TD>${AUSER:--Everyone-}</TD></TR>"
    done
}

get_userlist(){
	if [ "$AUSER" = "" ] ; then
		echo "<OPTION name=auser value=\" \" selected >-Everyone-</OPTION>"
	else
		echo "<OPTION name=auser value=\" \" >-Everyone-</OPTION>"
	fi
    for UNAME in $(smbpasswd -list) ; do
    	if [ "$UNAME" = "$AUSER" ] ; then
        	echo "<OPTION name=auser value=\"$UNAME\" selected >$UNAME</OPTION>"
        else
        	echo "<OPTION name=auser value=\"$UNAME\" >$UNAME</OPTION>"
        fi
    done
}

get_writeable(){
    	if [ "$WRITE" = "yes" ] ; then
    		echo "<option name=write value=\"yes\" selected>yes</option>"
	    	echo "<option name=write value=\"no\">no</option>"
        else
    		echo "<option name=write value=\"yes\">yes</option>"
	    	echo "<option name=write value=\"no\" selected>no</option>"
        fi
}



initial_shareform(){
    %>
<body onload="disableButtons();">
<center>
	<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);"">
	<TABLE border="1" >
	<TR>
	<TH>&nbsp;</TH>
	<TH align=center>Share Name</TH>
	<TH align=center>Shared Folder</TH>
	<TH align=center>Writable</TH>
	<TH align=center>Allowed User</TH>
	</TR>
	<% get_shareinfo %>
    </TD></TR>
    </TABLE>
	<input type="submit" name="action" value="New" title="Click here to create a new share." onclick="btnAction=this"/>
	<input type="submit" id=actionEdit name="action" value="Edit" title="Click here to edit the share." onclick="btnAction=this"/>
	<input type="submit" id=actionRemove name="action" value="Remove" title="Click here to remove the share." onclick="btnAction=this"/>
	</form>
 <%
}

edit_shareform(){
    %>
<body>
<center>
    <form id=share name=share action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">
    <TABLE border="1" >
	<TR>
	<TH align=center>Share Name</TH>
	<TH align=center>Shared Folder</TH>
	<TH align=center>Writable</TH>
	<TH align=center>Allowed User</TH>
	</TR><TR>
    <TD><input type="text" name="sname"  size=15 value="<%= $SNAME %>"></TD>
    <TD><input type="text" name="shared" size=20 value="<%= $SHARED %>" readonly onclick="fileBrowser()"/></TD>
    <TD align=center>
    	<SELECT NAME="write">
	    <% get_writeable %>
	    </SELECT>
    </TD>
    <TD>
    	<SELECT NAME="auser">
    	<% get_userlist %>
	    </SELECT>
    </TD>
    </TR>
    </TABLE>
        <input type="submit" name="action" value="Save" onclick="btnAction=this"/>
        <input type="submit" name="action" value="Cancel" onclick="history.go(-1);return false;"/>
    </form>
<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
<input type=hidden name=sourceform value="./share.cgi">
<input type=hidden name=efolder value="">
<input type=hidden name=shared value="">
<input type=hidden name=sname value="<%= $SNAME %>">
<input type=hidden name=write value="<%= $WRITE %>">
<input type=hidden name=auser value="<%= $AUSER %>">
</form>
    <%
}

new_shareform(){
    %>
<body>
<center>
    <form id=share name=share action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">
    <TABLE border="1" >
	<TR>
	<TH align=center>Share Name</TH>
	<TH align=center>Shared Folder</TH>
	<TH align=center>Writable</TH>
	<TH align=center>Allowed User</TH>
	</TR>
	<TR>
    <TD><input type="text" name="sname"  size=15 value="<%= $SNAME %>"></TD>
    <TD><input type="text" name="shared" size=20 value="<%= $SHARED %>" readonly onclick="fileBrowserNew()"/></TD>
    <TD align=center>
    	<SELECT NAME="write">
	    <% get_writeable %>
	    </SELECT>
    </TD>
    <TD>
    	<SELECT NAME="auser">
    	<% get_userlist %>
	    </SELECT>
    </TD>
    </TR>
    </TABLE>
        <input type="submit" name="action" value="Save" onclick="btnAction=this"/>
        <input type="submit" name="action" value="Cancel" onclick="history.go(-1);return false;"/>
    </form>
<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
<input type=hidden name=sourceform value="./share.cgi">
<input type=hidden name=efolder value="">
<input type=hidden name=shared value="">
<input type=hidden name=sname value="">
<input type=hidden name=write value="">
<input type=hidden name=auser value="">
</form>
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

function getCheckedValue(radioObj) {
	if(!radioObj)
		return "";
	var radioLength = radioObj.length;
	if(radioLength == undefined)
		if(radioObj.checked)
			return radioObj.value;
		else
			return "";
	for(var i = 0; i < radioLength; i++) {
		if(radioObj[i].checked) {
			return radioObj[i].value;
		}
	}
	return "";
}

function fileBrowser(){
	filebrowser.submit();
}

function fileBrowserNew(){
	filebrowser.sname.value = share.sname.value;
	filebrowser.write.value = share.write.value;
	filebrowser.auser.value = share.auser.value;
	filebrowser.submit();
}


function validateAction(form) {                                             
    var action = btnAction.value;                                               
    var share = getCheckedValue(form.sname);
    var question = "";                                                          
    var answer = true;             
    if ( action == 'New')
        return true; 
    if ( share == '--invalid--')                                                 
        return false;                                                           
    if ( action == 'Remove'){                                                   
        question = "This will erase the share. Do you want to continue?";
        answer = confirm (question);                                            
    }
    if ( action == 'Save'){                                                   
        question = "This action will save the share. Do you want to continue?";
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
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)

    case "$ACTION" in
      Edit)
        SNAME=$(echo ${FORM_sname} | cut -d ' ' -f 1)
        SHARE=$(smbshare -get $SNAME)
        SHARED=$(echo $SHARE | cut -d '|' -f 3)
        WRITE=$(echo $SHARE | cut -d '|' -f 4)
        AUSER=$(echo $SHARE | cut -d '|' -f 5)
        edit_shareform
    	;;
      Save)
        SNAME=$(echo ${FORM_sname} | cut -d ' ' -f 1)
        SHARED=$(echo ${FORM_shared} | cut -d ' ' -f 1)
        WRITE=$(echo ${FORM_write} | cut -d ' ' -f 1)
        AUSER=$(echo ${FORM_auser} | cut -d ' ' -f 1)
        smbshare -a $SNAME $SHARED $WRITE $AUSER    
        initial_shareform
    	;;
      Done)
        SNAME=$(echo ${FORM_sname} | cut -d ' ' -f 1)
        SHARED=$(echo ${FORM_shared} | cut -d '|' -f 1)
        WRITE=$(echo ${FORM_write} | cut -d '|' -f 1)
        AUSER=$(echo ${FORM_auser} | cut -d '|' -f 1)
        edit_shareform
        ;;	
      Remove)
        SNAME=$(echo ${FORM_sname} | cut -d ' ' -f 1)
        smbshare -x $SNAME
        initial_shareform
    	;;
      New)
        new_shareform
        ;;
      *)
        initial_shareform
    esac
else 
    initial_shareform
fi
%>

</body>
</html>
