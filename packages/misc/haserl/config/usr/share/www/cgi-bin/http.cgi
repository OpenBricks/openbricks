#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>

<script language="JavaScript">
<!-- //

function validateAll(form) {
	var http_port = form.http_port.value;
	var re = /^\d{1,5}$/;
	if (http_port == '') {
		alert('Please enter a valid http port for web admin interface. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	if (!re.test(http_port)) { 
		alert('Please enter a valid http port for web admin interface. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	if (http_port < 1 || http_port > 65535) { 
		alert('Please enter a valid http port for web admin interface. Accepted port numbers are between 1 and 65535.');
		return false;
	}
	var answer = confirm ("You are about to change http web admin port. Confirm that choosen tcp port is a unused one. Are you sure?");
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
    NEWPORT=$(echo ${FORM_http_port} | cut -d ' ' -f 1)
    set_config http_port $NEWPORT
    echo "Web Admin port set to ${NEWPORT}. Save the config and restart your NAS for this change to take effect.<br>"
fi
%>

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAll(this);">
     <TABLE border="0" >
     <TR><TH>HTTP Web Admin port:</TH><TD><input type="text" name="http_port" size=4 value=<% get_config http_port %> title="Enter a new HTTP Web Admin port. Accepted port numbers are between 1 and 9999."></TD></TR>
     </TABLE>
   <input type="submit" name="action" value="Apply">
</form>

</body>
</html>
