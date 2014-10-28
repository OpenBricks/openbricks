#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    USE_OPKG=$(echo ${FORM_use_opkg} | cut -d ' ' -f 1)
    OPKGDISK=$(echo ${FORM_opkgdisk} | cut -d ' ' -f 1)

    case "$ACTION" in
      Apply)
 	OPKGLOG=$(/etc/init.d/opkg unlink 2>&1 | tail -c 1000)

	set_config use_opkg ${USE_OPKG:-0}	
	set_config opkg_disk ${OPKGDISK:-sda1}	
	if [ ${USE_OPKG:-0} -eq 1 ] ; then
		OPKGLOG=$(/etc/init.d/opkg link $OPKGDISK 2>&1 | tail -c 1000)
	fi
    	;;
      *)
    esac
fi

OPKGDISK=$(get_config opkg_disk)

%>
<script language="JavaScript">
<!-- //
function checkOpkg(){                                                           
    if ( <% get_config use_opkg %> == 0 ){
        document.forms[0].opkgdisk.disabled = true;
    }
}
function setReadOpkg(obj){
    if(obj.checked)
    {
        document.forms[0].opkgdisk.disabled = false;
        } 
    else {
        document.forms[0].opkgdisk.disabled = true;
        }
} 

function validateAction(form) {                                             
    var action = btnAction.value;
    var use = form.use_opkg.value;
    var question = "";

    if (action == 'Apply' && use == 1)
        if (use == 1)
            question = "All package services will be (re)started.";
	else
            question = "All package services will be stopped.";
    var answer = confirm (question);
    if (answer)
        return true;                                                 
    else                                                            
        return false;                                           
}


// -->
</script>
</head>
<body onload="checkOpkg();">                            

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
     <TABLE border="0" >
     <TR><TH>Enable opkg:</TH><TD><input type="checkbox" name="use_opkg" value="1" <% is_checked $(get_config use_opkg) %> onclick="setReadOpkg(this);"/></TD></TR>
   <TR><TH>Opkg Disk:</TH><TD> <select name="opkgdisk">
    <%

    OPTIONS="<option value="${OPKGDISK}" selected>${OPKGDISK}</option>"
    for MOUNT in $(get_mounted) ; do
        if [ ! $MOUNT = ${OPKGDISK} ] ; then
            OPTIONS=${OPTIONS}"<option value="${MOUNT}">${MOUNT}</option>"        
        fi
    done
    echo ${OPTIONS}
    %>
    </select></TD>
    <TD>(current: <% echo $(get_config opkg_disk)%> ) </TD>	
    </TR>
</TABLE>

   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
</form>
<pre style="width: 80ex; text-align:left">
<% 
   echo $OPKGLOG
%>
</pre>
</center>
</body>
</html>
