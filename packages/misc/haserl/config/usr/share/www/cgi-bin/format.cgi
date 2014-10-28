#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_diskinfo(){
    if [ ${1:-All} = "All" ] ; then
        for DISK in $(get_alldisks) ; do
            get_diskinfoall $DISK
        done
    else
        get_diskinfoall $1
    fi
}

get_alldisks(){
    ls /sys/block/sd* -d | cut -d '/' -f 4
}

get_diskinfoall(){
    DEV=$1
    VENDOR=$(cat /sys/block/$DEV/device/vendor)
    MODEL=$(cat /sys/block/$DEV/device/model)
    SIZE="$(expr $(cat /sys/block/$DEV/size) / 2048) MB"
    ACTION="<input type=\"submit\" name=\"format\" value=\"Format $DEV\"  title=\"Click here to reformat the disk.\"/>"
    echo -n "<TR><TD align=center>$DEV</TD><TD align=center>$VENDOR</TD><TD align=center>$MODEL</TD><TD align=center>$SIZE</TD><TD align=center>$ACTION</TD></TR>"     
}

%>
<script language="JavaScript">
<!-- //

function validateAction(form) {
    var question = "This action will completely erase all data on the selected disk. ALL DATA WILL BE LOST. Do you want to continue?";
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

<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_format} | cut -d ' ' -f 1)
    DISK=$(echo ${FORM_format} | cut -d ' ' -f 2)
    if [ "$ACTION" = "Format" ] ; then
    	echo "<center><h2>Format of disk $DISK in progress... Please wait, it may take up to several minutes...</h2></center>"
		echo "<center>If the page stops without showing the disk status, reload and try again.</center>"
    	/usr/share/snake/formatdisk $DISK
    	echo "<center><h2>Format of disk $DISK done!</h2></center>"
    fi
fi
%>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">  
<center>
<TABLE border="1" >
<TR>
<TH align=center>Device</TH>
<TH align=center>Vendor</TH>
<TH align=center>Model</TH>
<TH align=center>Size</TH>
<TH align=center>Action</TD>
</TR>
<% get_diskinfo %>
</TABLE>
</form>
</body>
</html>
