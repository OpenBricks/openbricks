#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

remove_diskname(){
	sed -ri "/^$1:.*/d" /etc/default/diskname
	sleep 1
}

set_diskname(){
	sed -ri "/^$1:.*/d" /etc/default/diskname
	echo ${1}:${2}:${3} >> /etc/default/diskname
	sleep 1
}

get_names(){
	blkid | grep "^/dev/sd" | sed 's/[ "]//g' | tr '=' '_' | while read BLKID ; do
		DISK=$(echo "$BLKID" | cut -d ':' -f 1 | cut -c 6-8)
		PART=$(echo "$BLKID" | cut -d ':' -f 1 | cut -c 9)
		VENDOR=$(cat /sys/block/$DISK/device/vendor)
		MODEL=$(cat /sys/block/$DISK/device/model)
		DISKINFO=$(echo ${VENDOR}-${MODEL}Part${PART} | tr ' ' '_')
		DISKID=$(echo "$BLKID" | cut -d ':' -f 2)
		NAME=$(grep "^${DISKID}:" /etc/default/diskname| cut -d ':' -f 2)
		echo ${DISKID}:${NAME}:${DISKINFO}:${DISK}${PART}:1
	done | cat /etc/default/diskname - | sort -t ':' -k 1,1 -ur | sort -r -t ':' -k 5,5
}



show_fields(){

	get_names | while read DISK ; do
		DISKID=$(echo "$DISK" | cut -d ':' -f 1)
		NAME=$(echo "$DISK" | cut -d ':' -f 2) 2> /dev/null
		DISKINFO=$(echo "$DISK" | cut -d ':' -f 3)
		DEV=$(echo "$DISK" | cut -d ':' -f 4) 2> /dev/null
		MOUNTED=$(echo "$DISK" | cut -d ':' -f 5) 2> /dev/null

		%>
		<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateName(this);">
		<%
		if [ -n "$MOUNTED" ] ; then
			echo "${DISK}" | awk -F':' -v name=${NAME} '{ print "<TR><TD align=center title=\"Signature :"$1"\"><b>"$3"</b></TD><TD align=center><input type=\"text\" name=\"diskname\" size=15 value=\""name"\" title=\"Enter the disk preferred name here\" /></TD>"}' 
		else
			echo "${DISK}" | awk -F':' -v name=${NAME} '{ print "<TR><TD align=center title=\"Signature :"$1"\">"$3"</TD><TD align=center><input type=\"text\" name=\"diskname\" size=15 value=\""name"\" title=\"Enter the disk preferred name here\" /></TD>"}'
		fi
		%>
		<TD>
		<input type="hidden" name="diskid" value="<%= $DISKID %>">
		<input type="hidden" name="diskinfo" value="<%= $DISKINFO %>">
		<input type="hidden" name="dev" value="<%= $DEV %>">
		<input type="hidden" name="mounted" value="<%= $MOUNTED %>">
		<input type="submit" name="action" value="Save"  title="Click here to save the disk name. If mounted, it will immediately remount the disk."  onclick="btnAction=this"/>
		<input type="submit" name="action" value="Remove"  title="Click here to remove the disk name assignment. If mounted, it will immediately remount the disk."  onclick="btnAction=this"/></form>
		<%
		if [ -n "$MOUNTED" ] ; then
			echo Connected
		fi

		echo "</TD></TR>"

	done
}


if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_action} | cut -d ' ' -f 1)

    case "$ACTION" in
      Save)
        DISKID=$(echo ${FORM_diskid} | cut -d ' ' -f 1)
        DISKNAME=$(echo ${FORM_diskname} | cut -d ' ' -f 1)
        DISKINFO=$(echo ${FORM_diskinfo} | cut -d ' ' -f 1)
        DEV=$(echo ${FORM_dev} | cut -d ' ' -f 1)
        MOUNTED=$(echo ${FORM_mounted} | cut -d ' ' -f 1)
		if [ -n "$MOUNTED" ] ; then
			/usr/share/snake/automount $DEV remove 2> /dev/null
			set_diskname ${DISKID} ${DISKNAME} ${DISKINFO}
			/usr/share/snake/automount $DEV add 2> /dev/null			
		else
			set_diskname ${DISKID} ${DISKNAME} ${DISKINFO}
		fi
    	;;
      Remove)
        DISKID=$(echo ${FORM_diskid} | cut -d ' ' -f 1)
        DEV=$(echo ${FORM_dev} | cut -d ' ' -f 1)
        MOUNTED=$(echo ${FORM_mounted} | cut -d ' ' -f 1)
		if [ -n "$MOUNTED" ] ; then
			/usr/share/snake/automount $DEV remove 2> /dev/null
			remove_diskname ${DISKID}
			/usr/share/snake/automount $DEV add 2> /dev/null			
		else
			remove_diskname ${DISKID}
		fi
    	;;
      *)
		;;
	esac
fi


%>


<script language="JavaScript">
<!-- //

function validateName(form){
	var action = btnAction.value;
	var answer = true;
	if ( action == 'Save'){
		var re = /^\w{3,15}$/;
		var diskname = form.diskname.value;
		if (!re.test(diskname)) { 
	        alert("Please enter a valid disk name! The disk name must have a minimum 3 and a maximum of 15 chars.");
	        return false;
   		}
	}
    question = "If mounted, it will immediately remount the disk. Do you want to continue?";
    answer = confirm (question);                                            
	return answer;
}




// -->
</script>
</head>
<body>
<center>
<TABLE border="1" >
<TR><TH>Partition</TH><TH>Mount Name</TH><TH>Action</TH></TR>
<% show_fields %>
</TABLE>
</body>
</html>
