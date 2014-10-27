#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_devinfo(){
    if [ ${1:-All} = "All" ] ; then
        for DISK in $(get_alldisks) ; do
            get_devinfoall $DISK
        done
    else
        get_devinfoall $1
    fi
}

get_devinfoall(){
    DEV=$1
    VENDOR=$(cat /sys/block/$DEV/device/vendor)
    MODEL=$(cat /sys/block/$DEV/device/model)
   
    for PART in $(get_allparts $DEV) ; do
        MOUNTINFO=$(cat /proc/mounts | grep "$PART")
        MOUNTED=$(echo $MOUNTINFO | cut -d ' ' -f 2 | cut -d '/' -f 3)
        MOUNTED=${MOUNTED:-none}
        if [ "$MOUNTED" = "none" ] ; then
            TYPE= && DFSIZE= && DFUSED= && DFAVAIL= && DFPCT= 
			ACTION="<input type=\"submit\" name=\"mount\" value=\"Mount $PART\"  title=\"Click here to mount the disk. If it does not work, try to reformat the disk.\"/>"
        else
            TYPE=$(echo $MOUNTINFO | cut -d ' ' -f 3)
            if [ "$TYPE" = "fuseblk" ] ; then
                TYPE=ntfs
            fi
            DF=$(df -h /usb/$MOUNTED | grep "^/" |  tr -s ' ')
            DFSIZE=$(echo $DF | cut -d ' ' -f 2)
            DFUSED=$(echo $DF | cut -d ' ' -f 3)
            DFAVAIL=$(echo $DF | cut -d ' ' -f 4)
            DFPCT=$(echo $DF | cut -d ' ' -f 5)
            ACTION="<input type=\"submit\" name=\"mount\" value=\"Umount $PART\" title=\"Click here to dismount. If it does not work, may have a service using the disk.\"/>"
			if [ "$TYPE" = "ext2" -o "$TYPE" = "ext3" ] ; then
				ACTION="${ACTION}<input type=\"submit\" name=\"mount\" value=\"Check $PART\" title=\"Check partition integrity. No changes will be done.\"/>"
				ACTION="${ACTION}<input type=\"submit\" name=\"mount\" value=\"Fix $PART\" title=\"Attempt to fix partition integrity. It will umount the partition during this procedure.\"/>"
			fi
        fi
        echo -n "<TR><TD align=center>$DEV</TD><TD align=center>$VENDOR</TD><TD align=center>$MODEL</TD><TD align=center>$PART</TD><TD align=center>$MOUNTED</TD><TD align=center>$TYPE</TD><TD align=center>$DFSIZE</TD><TD align=center>$DFUSED</TD><TD align=center>$DFAVAIL</TD><TD align=center>$DFPCT</TD><TD align=center>$ACTION</TD></TR>"     
    done
}
%>
</head>                                                                                                                                   
<body>                                                                                                                                    
<center>  
<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    ACTION=$(echo ${FORM_mount} | cut -d ' ' -f 1)
    PART=$(echo ${FORM_mount} | cut -d ' ' -f 2)
    if [ "$ACTION" = "Mount" ] ; then
    	/usr/share/snake/automount $PART add
    fi
    if [ "$ACTION" = "Umount" ] ; then
     	/usr/share/snake/automount $PART remove
    fi
    if [ "$ACTION" = "Check" ] ; then
    	echo "<h2>Checking partition $PART integrity</h2>"
    	e2fsck -n /dev/$PART
		echo "<br><b>If an error was found above, Umount the partition to fix it</b><br><br>"
    fi
    if [ "$ACTION" = "Fix" ] ; then
    	echo "<h2>Fixing partition $PART integrity</h2>"
     	/usr/share/snake/automount $PART remove
    	e2fsck -y /dev/$PART
    	/usr/share/snake/automount $PART add
		echo "<br><b>You can mount the disk now</b><br><br>"
    fi
fi
%>

<form action="<%= ${SCRIPT_NAME} %>" method="POST">  
<TABLE border="1" >
<TR>
<TH align=center>Device</TH>
<TH align=center>Vendor</TH>
<TH align=center>Model</TH>
<TH align=center>Partition</TH>
<TH align=center>Mounted</TH>
<TH align=center>Type</TH>
<TH align=center>Size</TH>
<TH align=center>Used</TH>
<TH align=center>Avail</TH>
<TH align=center>Usage</TH>
<TH align=center>Action</TD>
</TR>
<% get_devinfo %>
</TABLE>
</form>
</body>
</html>
