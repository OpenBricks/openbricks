#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%
    
CURDIR=$(echo ${FORM_folder} | cut -d ' ' -f 1)

SNAME=$(echo ${FORM_sname} | cut -d ' ' -f 1)
WRITE=$(echo ${FORM_write} | cut -d ' ' -f 1)
AUSER=$(echo ${FORM_auser} | cut -d ' ' -f 1)

SOURCEFORM=$(echo ${FORM_sourceform} | cut -d ' ' -f 1)
        
list_dir(){

	echo "<div id=\"updir-div\" onClick=\"setValue('$(echo /$CURDIR | sed "s,/[^/]*$,," | sed "s,^/,,")');folderlist.submit();\">"
	echo "<font color=blue>..</font>"
	echo "</div>"
		
	echo "$(find /usb/$(echo $CURDIR | sed 's,\.\.,,g') -type d -maxdepth 1 | sed 1d | sed 's,//,/,g' | sed 's,^/usb/,,g' | sort)" | while read LINE
	do
		echo "<div id=\"$LINE-div\" onClick=\"setValue('$LINE');folderlist.submit();\">"
		echo "<font color=blue>"
		echo "${LINE}"
		echo "</font>"
		echo "</div>"
	done
}


%>
<script language="JavaScript">
<!-- //

function setIniFolder(){
	folderlist.folder.value = "<%= ${CURDIR} %>";
	folderlist.sourceform.value= "<%= ${SOURCEFORM} %>";
	return true;
}


function setValue(folder){
	folderlist.folder.value = folder;
	return true;
}

function doneSelect(){
	if ( folderchoosen.efolder.value == 'yes'){
		folderchoosen.shared.value = "/usb";
		return true;
	}
	else{
		if ( folderlist.folder.value == "" )
			folderchoosen.shared.value = "/usb";
		else 
			folderchoosen.shared.value = "/usb/" + folderlist.folder.value;
		return true;
		}
}


function emptyFolder(){
	folderchoosen.efolder.value = 'yes';
	folderlist.folder.value = '';
}

// -->
</script>
</head>
<center>
<%
if [ "${REQUEST_METHOD}" = "POST" ]
	then
%>
<body onload="setIniFolder();">

<b>Current folder: <%= ${CURDIR:-usb_mount_point} %></b><br>
<form id=folderlist name=folderlist action="<%= ${SCRIPT_NAME} %>" method="POST">
<% list_dir %>
<input type=hidden name=folder value="">
<input type=hidden name=sourceform value="">
<input type=hidden name=sname value="<%= $SNAME %>">
<input type=hidden name=write value="<%= $WRITE %>">
<input type=hidden name=auser value="<%= $AUSER %>">
</form>

<form id=folderchoosen name=folderchoosen action="<%= $SOURCEFORM %>" method="POST" onsubmit="return doneSelect();">
<input type=hidden name=efolder value="">
<input type=hidden name=shared value="">
<input type=hidden name=sname value="<%= $SNAME %>">
<input type=hidden name=write value="<%= $WRITE %>">
<input type=hidden name=auser value="<%= $AUSER %>">
<input type="submit" name="action" value="Done" >
<input type="submit" name="action" value="Cancel" onclick="emptyFolder();"/>
</form>
<%
else 
	echo "<body>"
fi
%>
</body>
</html>
