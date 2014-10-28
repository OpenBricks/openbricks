#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<script language="JavaScript">
<!-- //

function validateAction(form) {
    var question = "This will reboot the system. Before this, please make sure that the configuration is saved. Do you want to continue?";
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
<body>
<center> 
<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_reboot} | cut -d ' ' -f 1)
    if [ "$ACTION" = "Reboot" ] ; then
    	echo "<center><h2>System reboot in progress... Please wait and try to connect again in a few moments...</h2></center>"
    	/sbin/reboot > /dev/null 2>&1
    	sleep 30
	fi
fi
%>
<TABLE border="0">
<TR><TH>Kernel version:</TH><TD><%= $(uname -r)%></TD></TR>
<TR><TH>Sifbox version:</TH><TD><%= $(cat /etc/version | cut -d "-" -f 1) %></TD></TR>
<TR><TH>Vdr Version:</TH><TD><%= $(vdr -V | grep "^vdr " | cut -d "-" -f 1 | cut -d "/" -f 2 | cut -d ")" -f 1)%></TD></TR>
<TR><TH>LAN IP Address:</TH><TD><% get_ipaddress %></TD></TR>
<TR><TH>LAN MAC Address:</TH><TD><%= $(ifconfig eth0 | grep HWa | awk '{print $5}')%></TD></TR>
<TR><TH>Physical RAM:</TH><TD><%= $(cat /proc/meminfo | tr '\n' ' ' | awk '{print "Total: "$2"KB - Free: "$5"KB - Buffers: "$8"KB - Cached: "$11"KB"}')%></TD></TR>
<TR><TH>Uptime:</TH><TD><%= $(uptime | cut -d p -f 2 | awk -F', l' '{ print $1}' | cut -d "," -f 1)%></TD></TR>
<TR><TH>Vdr Plugin:</TH><TD><%= $(opkg list-installed | grep vdr- | cut -d "-" -f 2)%></TD></TR>
</TABLE>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">
<input type="submit" name="reboot" value="Reboot"  title="Click here to reboot this device."/>
</form>
</body>
</html>

