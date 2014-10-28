#!/bin/haserl --upload-limit=200 --upload-dir=/tmp
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>

<script language="JavaScript">
<!-- //

function validateAction(form) {                                             
	var action = btnAction.value;
	var question = "";
	if ( action == 'Save Config' )
		question = "This action will save the current config. Continue?";
	else if  ( action == 'Reset to Default' )
		question = "This action will return the config back to default settings. The device will restart and get back in a few minutes. ARE YOU REALLY, REALLY SURE?";
        else if  ( action == 'Restore Config' )
		question = "This action will restore the config from a backup from same SNAKE OS version.\nThe device will reboot when done.\nARE YOU SURE?";
	else 
		return false;
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

<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
      Save)
      	/usr/share/snake/config save > /dev/null
      	sleep 2
      	echo "<center><h2>Config saved.</h2></center>"      	
    	;;
      Reset)
      	/usr/share/snake/config setdefault > /dev/null
      	sleep 30
    	;;
      Restore)
        if [ -n "$FORM_uploadfile" ] ; then
        	echo -n "<center>"
       	        dd if=$FORM_uploadfile of=/tmp/cfgpartition.tmp bs=4096 count=32
    	        dd if=$FORM_uploadfile of=/tmp/cfgstring.tmp bs=4096 skip=32
                FILEMD5=$(md5sum /tmp/cfgpartition.tmp | cut -c 1-32)
                EXPMD5=$(cut -c 1-32 /tmp/cfgstring.tmp)
                if [ "$FILEMD5" = "$EXPMD5" ] ; then
                    FILEVER=$(cut -c 33- /tmp/cfgstring.tmp )
                    THISVER=$(cat /etc/version)
                    if [ "$FILEVER" = "$THISVER" ] ; then
                         echo "<b>Restoring SNAKE OS config...</b><br>"
                         dd if=/tmp/cfgpartition.tmp of=/dev/mtdblock1 bs=4096 count=32
                         sync
                         echo "<h2>Config restored.</h2>"
                         echo "<b>Rebooting now, you can try to connect in a few moments...</b><br>"
                         reboot -f
                     else
                          echo -n "<b>ERROR:</b> Unable to restore. Different SNAKE OS version on backup.</br>"
                     fi
                else
                    echo -n "<b>ERROR:</b> Unable to restore. Wrong checksum on backup file.</br>"
                fi
	fi
      ;;
      *)
    esac
fi
%>
<br>
<b>Device configuration:</b><br>
<TABLE border=1 WIDTH="380" STYLE="width: 380px"> 
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
<TR><TD align=center><input type="submit" name="action" value="Save Config" onclick="btnAction=this"></TD>
<TD align=center><input type="submit" name="action" value="Reset to Default" onclick="btnAction=this"></TD></TR>
</form>
</TABLE>
<br>
<b>Backup/Restore:</b><br>
<TABLE border=1 WIDTH="380" STYLE="width: 380px">
<form action="<%= /cgi-bin/backup.cgi %>" method="GET">
<TR><TD colspan=2><input type="submit" name="action" value="Backup Config">&nbsp;Backup last saved configuration to file</TD>
</form>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" enctype="multipart/form-data" onsubmit="return validateAction(this);">
<TR>
<TD><input type=submit name="action" value="Restore Config" onclick="btnAction=this">
<input type=file name=uploadfile></TD> 
</TR>
</form>
</TABLE>
</body>
</html>
