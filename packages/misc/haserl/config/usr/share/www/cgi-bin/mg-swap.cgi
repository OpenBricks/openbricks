#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%
set_swapinfo(){
    /etc/init.d/swapsvc stop > /dev/null
	set_config use_swap ${1}
    if [ $1 -eq 1 ] ; then
       	set_config swapfile /usb/${2}/swapfile
        /etc/init.d/swapsvc start > /dev/null
    fi
}

if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)
    USE_SWAP=$(echo ${FORM_use_swap} | cut -d ' ' -f 1)
    SWAPDISK=$(echo ${FORM_swapdisk} | cut -d ' ' -f 1)

    case "$ACTION" in
      Apply)
      	set_swapinfo ${USE_SWAP:-0} $SWAPDISK
    	;;
      Stop)
      	/etc/init.d/swapsvc stop > /dev/null
    	;;
      Delete)
      	/etc/init.d/swapsvc remove > /dev/null
    	;;
      *)
    esac
fi


SWAPFILE=$(get_config swapfile)
SWAPDISK=$(echo ${SWAPFILE} | cut -d '/' -f 3)

%>
<script language="JavaScript">
<!-- //
function checkSwap(){                                                           
    if ( <% get_config use_swap %> == 0 ){
        document.forms[0].swapdisk.disabled = true;
    }
}
function setReadSwap(obj){
    if(obj.checked)
    {
        document.forms[0].swapdisk.disabled = false;
        } 
    else {
        document.forms[0].swapdisk.disabled = true;
        }
} 

function validateAction(form) {                                             
    var action = btnAction.value;
    var use = form.use_swap.value;
    var question = "";
    if ( action == 'Stop' || use != '1' )
        question = "You are about to stop using swapfile. This can cause instability due to low memory. Do you want to continue?";
    else if (action == 'Delete')
        question = "You are about to delete the current swapfile. Swapping will stop. This can cause instability due to low memory. Do you want to continue?";
    else if (action == 'Apply')
        question = "You will apply the new settings. If swapfile has to be recreated, it will take some time to create and activate it. Do you want to continue?";
    var answer = confirm (question);
    if (answer)                                                      
        return true;                                                 
    else                                                            
        return false;                                           
} 


// -->
</script>
</head>
<body onload="checkSwap();">                            

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST"  onsubmit="return validateAction(this);">
     <TABLE border="0" >
     <TR><TH>Use Swapfile:</TH><TD><input type="checkbox" name="use_swap" value="1" <% is_checked $(get_config use_swap) %> onclick="setReadSwap(this);"/> (256MB required)</TD></TR>
   <TR><TH>Swapfile Disk:</TH><TD> <select name="swapdisk">
    <%

    OPTIONS="<option value="${SWAPDISK}" selected>${SWAPDISK}</option>"
    for MOUNT in $(get_mounted) ; do
        if [ ! $MOUNT = ${SWAPDISK} ] ; then
            OPTIONS=${OPTIONS}"<option value="${MOUNT}">${MOUNT}</option>"        
        fi
    done
    echo ${OPTIONS}
    %>
    </select></TD></TR>
    <TR><TH>Swap status:</TH><TD>
<%                                                                             
 	SWAP_MOUNTED=$(echo $(get_mounted) | grep ${SWAPDISK} | wc -l)
	if [ $(cat /proc/swaps | grep usb | wc -l) -eq 1 ]; then                        
		echo "Active on disk ${SWAPDISK}"                                                       
	else                                                                            
		if [ $SWAP_MOUNTED -eq 0 ]  ; then                                          
			echo "Disk ${SWAPDISK} not mounted"                                             
	    else                                                                        
			echo "Stopped"                       
		fi                                                                          
	fi                                                                              
%> 
</TD></TR>
</TABLE>

   <input type="submit" name="action" value="Apply" onclick="btnAction=this">
   <input type="submit" name="action" value="Stop" onclick="btnAction=this">
   <input type="submit" name="action" value="Delete" onclick="btnAction=this">
</form>


</body>
</html>
