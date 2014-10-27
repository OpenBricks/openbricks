#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    PRINTER_BIDI=$(echo ${FORM_printer_bidi} | cut -d ' ' -f 1)
    case "$ACTION" in
      Apply)
      	set_config printer_bidi ${PRINTER_BIDI:-0}
    	;;
      *)
    esac
fi


get_printerinfo(){
	for DEVICE in $(ls /dev/lp* -d | cut -d '/' -f 3) ; do
		MFG=$(cat /sys/class/usb/${DEVICE}/device/ieee1284_id | tr ';' '\n' | grep ^MFG | cut -d : -f 2)
		MDL=$(cat /sys/class/usb/${DEVICE}/device/ieee1284_id | tr ';' '\n' | grep ^MDL | cut -d : -f 2)
		RAWTCP=$(pgrep -fl "/dev/${DEVICE}" | cut -d ' ' -f 2 | cut -c 2-5) 
		echo -n "<TR><TD align=center>$MFG&nbsp;$MDL</TD><TD align=center>$DEVICE</TD><TD align=center>$RAWTCP</TD></TR>"    
	done
}
%>
</head>
<body>
<center>
<b>Printer Sharing</b><br>
<form action="<%= ${SCRIPT_NAME} %>" method="POST">
<TABLE border="0" >
<TR><TH>Bidirectional Support:</TH><TD><input type="checkbox" name="printer_bidi" value="1" <% is_checked $(get_config printer_bidi) %> title="You will have to disconnect and reconnect the printer for this config to take effect"/></TD></TR>
</TABLE> 
<TABLE border="1" >
<TR>
<TH align=center>Model</TH>
<TH align=center>Logical device</TH>
<TH align=center>RAW TCP port</TH>
</TR>
<% get_printerinfo %>
</TABLE>
<br>
   <input type="submit" name="action" value="Apply">
</form>
</body>
</html>

