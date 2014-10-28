#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

HTTP_PORT=$(get_config http_port)
WEBROOT=$(get_config webserver_root)

if [ "${REQUEST_METHOD}" = "POST" ]
then
	ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    case "$ACTION" in
		Apply)
			ENABLE=$(echo ${FORM_webserver_enable} | cut -d ' ' -f 1)
		    set_config webserver_enable ${ENABLE:-0}
			if [ ${ENABLE} = "1" ] ; then
			    NEWPORT=$(echo ${FORM_webserver_port} | cut -d ' ' -f 1)
		    	set_config webserver_port $NEWPORT

				WEBROOT=$(echo ${FORM_shared} | cut -d ' ' -f 1)
		    	set_config webserver_root ${WEBROOT}
				
				/etc/init.d/webserver restart > /dev/null
				sleep 1
			fi
		;;
		Start)
	      	/etc/init.d/webserver start > /dev/null
			sleep 1
    	;;
		Stop)
      		/etc/init.d/webserver stop > /dev/null
			sleep 1
		;;
		Restart)
      		/etc/init.d/webserver restart > /dev/null
			sleep 1
		;;
		Done)
			WEBROOT=$(echo ${FORM_shared} | cut -d ' ' -f 1)
		;;
		*)
	esac
fi



%>

<script language="JavaScript">
<!-- //
function checkWebserver(){                                                           
    if ( <% get_config webserver_enable %> == 0 ){
        document.forms[1].webserver_port.disabled = true;
        document.forms[0].shared.disabled = true;
    }
}

function setReadWebserver(obj){
    if(obj.checked)
    {
        document.forms[1].webserver_port.disabled = false;
        document.forms[0].shared.disabled = false;
        } 
    else {
        document.forms[1].webserver_port.disabled = true;
        document.forms[0].shared.disabled = true;
        }
} 

function validateAll(form) {
	var http_port = form.http_port.value;
	var webserver_port = form.webserver_port.value;
	var webserver_enable = form.webserver_enable.value;
	var re = /^\d{1,5}$/;
	if (webserver_enable == 1){
		if (webserver_port == '') {
			alert('Please enter a valid http port for web server. Accepted port numbers are between 1 and 65535.');
			return false;
		}
		if (!re.test(webserver_port)) { 
			alert('Please enter a valid http port for web server. Accepted port numbers are between 1 and 65535.');
			return false;
		}
		if (webserver_port < 1 || webserver_port > 65535) { 
			alert('Please enter a valid http port for web server. Accepted port numbers are between 1 and 65535.');
			return false;
		}
		if (webserver_port == http_port) { 
			alert('You have to use a different http port for web server and web admin interface.');
			return false;
		}
		return true;                                                 
	}
}

function setFolder(){
	webserver.shared.value = "<%= ${WEBROOT} %>";
	return true;
}

function fileBrowserNew(){
	filebrowser.submit();
}

// -->
</script>
</head>
<body onload="setFolder();checkWebserver();">                            

<center>
<TABLE border="0" >
<form id=filebrowser name=filebrowser action="./filebrowser.cgi" method="POST">
	<input type=hidden name=sourceform value="<%= ${SCRIPT_NAME} %>">
	<TR><TH>Web Server folder:</TH><TD><input type="text" name="shared" size=20 value="<%= ${WEBROOT} %>" readonly onclick="fileBrowserNew()"/></TD></TR>
</form> 
<form id=webserver name=webserver action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAll(this);">
	<input type="hidden" name="http_port" value=<%= ${HTTP_PORT} %> >
	<TR><TH>Enable Web Server:</TH><TD><input type="checkbox" name="webserver_enable" value="1" <% is_checked $(get_config webserver_enable) %> onclick="setReadWebserver(this);"/></TD></TR>
	<TR><TH>Web Server port:</TH><TD><input type="text" name="webserver_port" size=4 value=<% get_config webserver_port %> title="Enter a the Web Server port. Accepted port numbers are between 1 and 65535."></TD></TR>
	<TR><TH>Web Server status: </TH><TD><% /etc/init.d/webserver webstatus %></TD></TR>
    </TABLE>
    <input type=hidden name=shared value="">
	<input type="submit" name="action" value="Apply" onclick="btnAction=this">
	<input type="submit" name="action" value="Start" onclick="btnAction=this">
	<input type="submit" name="action" value="Stop" onclick="btnAction=this">
	<input type="submit" name="action" value="Restart" onclick="btnAction=this">
</form>
</body>
</html>
