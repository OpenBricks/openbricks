#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

set_ftp_anonymous(){
	set_config ftp_anonymous ${1}
	if [ "${1}" -eq 0 ] ; then
		sed -ri "s,^anonymous_enable=.*,anonymous_enable=NO,g" /etc/vsftpd.conf
	else
		sed -ri "s,^anonymous_enable=.*,anonymous_enable=YES,g" /etc/vsftpd.conf
	fi	 
}

set_ftp_anondir(){
	set_config ftp_anondir ${1}
	sed -ri "s,^anon_root=.*,anon_root=${1},g" /etc/vsftpd.conf
}

set_ftp_anononly(){
	set_config ftp_anononly ${1}
	if [ "${1}" -eq 0 ] ; then
		sed -ri "s,^local_enable=.*,local_enable=YES,g" /etc/vsftpd.conf
	else
		sed -ri "s,^local_enable=.*,local_enable=NO,g" /etc/vsftpd.conf
	fi	 

}

set_ftp_anonrw(){
	set_config ftp_anonrw ${1}
	if [ "${1}" -eq 0 ] ; then
		sed -ri "s,^anon_upload_enable=.*,anon_upload_enable=NO,g" /etc/vsftpd.conf
		sed -ri "s,^anon_mkdir_write_enable=.*,anon_mkdir_write_enable=NO,g" /etc/vsftpd.conf
	else
		sed -ri "s,^anon_upload_enable=.*,anon_upload_enable=YES,g" /etc/vsftpd.conf
		sed -ri "s,^anon_mkdir_write_enable=.*,anon_mkdir_write_enable=YES,g" /etc/vsftpd.conf
	fi	
}


FTPDIR=$(get_config ftp_anondir)

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
      Apply)
        FTPDIR=$(echo ${FORM_shared} | cut -d ' ' -f 1)
        FTPENABLE=$(echo ${FORM_ftp_enable} | cut -d ' ' -f 1)
        FTPANON=$(echo ${FORM_ftp_anon} | cut -d ' ' -f 1)
        FTPANONRW=$(echo ${FORM_ftp_anonrw} | cut -d ' ' -f 1)
        FTPANONONLY=$(echo ${FORM_ftp_anononly} | cut -d ' ' -f 1)
      	/etc/init.d/ftpsvc stop > /dev/null
      	sleep 1
      	set_ftp_anondir ${FTPDIR}
      	set_config ftp_enable ${FTPENABLE:-0}
      	set_ftp_anonymous ${FTPANON:-0}
      	set_ftp_anonrw ${FTPANONRW:-0}
      	set_ftp_anononly ${FTPANONONLY:-0}
      	sleep 1
      	/etc/init.d/ftpsvc start > /dev/null
      	sleep 1
      	;;
      Start)
      	/etc/init.d/ftpsvc start > /dev/null
      	sleep 2
    	;;
      Stop)
      	/etc/init.d/ftpsvc stop > /dev/null
      	sleep 2
    	;;
      Restart)
      	/etc/init.d/ftpsvc restart > /dev/null
      	sleep 2
    	;;
      Done)
        FTPDIR=$(echo ${FORM_shared} | cut -d '|' -f 1)
        ;;
      *)
    esac
fi

%>
<script language="JavaScript">
<!-- //
function checkFtpSetup(){              
	if ( <% get_config ftp_enable %> == 0 ){
		document.forms[0].shared.disabled = true;
		document.forms[1].ftp_anon.disabled = true;
		document.forms[1].ftp_anonrw.disabled = true;
		document.forms[1].ftp_anononly.disabled = true;
	}
	if ( <% get_config ftp_anonymous %> == 0 ){
		document.forms[1].ftp_anonrw.disabled = true;
		document.forms[1].ftp_anononly.disabled = true;
	}
}                                                      

function setReadFtp(obj){
	if(obj.checked){       
		document.forms[0].shared.disabled = false;
		document.forms[1].ftp_anon.disabled = false;
		document.forms[1].ftp_anonrw.disabled = false;
		document.forms[1].ftp_anononly.disabled = false;
		setAnonFtp(document.forms[1].ftp_anon);
	} 
	else { 
		document.forms[0].shared.disabled = true;
		document.forms[1].ftp_anon.disabled = true;
		document.forms[1].ftp_anonrw.disabled = true;
		document.forms[1].ftp_anononly.disabled = true;
	}
}

function setAnonFtp(obj){
	if(obj.checked){       
		document.forms[1].ftp_anonrw.disabled = false;
		document.forms[1].ftp_anononly.disabled = false;
	} 
	else { 
		document.forms[1].ftp_anonrw.disabled = true;
		document.forms[1].ftp_anononly.disabled = true;
	} 
}
 
function validateAction(form) {                                             
    var action = btnAction.value;
    var question = "";
    if ( action == 'Apply' )
    	 question = "This will restart FTP service and set the new folder. Are you sure?";
    else if ( action == 'Stop' )
        question = "Are you sure that you want to stop FTP service?";
    else if  ( action == 'Restart' )
        question = "Are you sure that you want to restart FTP service?";
    else
    	return true;
    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
}

function setFolder(){
	ftp.shared.value = "<%= ${FTPDIR} %>";
	return true;
}

function fileBrowserNew(){
	filebrowser.submit();
}

// -->
</script>
<meta http-equiv='refresh' content='90'>
</head>

<body onload="setFolder();checkFtpSetup();">                 
<center>
<TABLE border="0">
<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
  <input type=hidden name=sourceform value="<%= ${SCRIPT_NAME} %>">
  <TR><TH>Anonymous FTP folder:</TH><TD><input type="text" name="shared" size=20 value="<%= $FTPDIR %>" readonly onclick="fileBrowserNew()"/></TD></TR>
</form> 
<form id=ftp name=ftp action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
  <TR><TH>FTP enabled:</TH><TD><input type="checkbox" name="ftp_enable" value="1" <% is_checked $(get_config ftp_enable) %> onclick="setReadFtp(this);"/></TD></TR>
  <TR><TH>Allow Anonymous:</TH><TD><input type="checkbox" name="ftp_anon" value="1" <% is_checked $(get_config ftp_anonymous) %> onclick="setAnonFtp(this);"/></TD></TR>
  <TR><TH>Anonymous Can Write:</TH><TD><input type="checkbox" name="ftp_anonrw" value="1" <% is_checked $(get_config ftp_anonrw) %> /></TD></TR>
  <TR><TH>Only Anonymous:</TH><TD><input type="checkbox" name="ftp_anononly" value="1" <% is_checked $(get_config ftp_anononly) %> /></TD></TR>
  <TR><TH>Status:</TH><TD><% /etc/init.d/ftpsvc webstatus %></TD></TR>
</TABLE>
    <input type=hidden name=shared value="">     
	<input type="submit" name="action" value="Apply" onclick="btnAction=this">
	<input type="submit" name="action" value="Start" onclick="btnAction=this">
	<input type="submit" name="action" value="Stop" onclick="btnAction=this">
	<input type="submit" name="action" value="Restart" onclick="btnAction=this">   
</form>
<br>
<br>
<b>If config <u>Only Anonymous</u> is <u>not</u> checked, all <u>users set in Sharing->Users</u> will have FTP access.</b>
</body>
</html>
