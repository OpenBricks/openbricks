#!/bin/haserl --upload-limit=10000 --upload-dir=/tmp/upg
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">
<%in funcs.cgi %>
</head>
<body>
<center>
<h2>Please take care with this procedure and don't power off the device during the upgrade.</h2>
<br>
<b>It will revert back to the default configuration when done.</b>
<br><br>
<%

DISKMOUNTED=$(grep "^/dev/sd" /proc/mounts | wc -l)	

upg_done(){
    /tmp/busybox echo "<p><h2>UPGRADE DONE - RESTARTING NOW...</h2></p>"
    /tmp/busybox rm -f /tmp/upg/fw_upg_nt.tmp > /dev/null
    /tmp/busybox rm -f /tmp/upg/fw_upg_et.tmp > /dev/null
    /tmp/busybox rm -f ${FORM_uploadfile} > /dev/null
	for MOUNTED in $(/tmp/busybox/sed -n 's,^\(/dev/sd[a-z][0-9]\).*$,\1,p' /proc/mounts) ; do
		/tmp/busybox/umount -r "$MOUNTED" > /dev/null
	done
    /tmp/busybox reboot -f > /dev/null
}

if [ $DISKMOUNTED -gt 0 ] ; then

	echo "<p><b>Warning:</b> One connected disk will be used for upgrade, don't remove any.</p>"

	if [ "${REQUEST_METHOD}" != "POST" ]
	then
		TMPDISK=$(grep "^/dev/sd.* /usb/" /proc/mounts | tail -n 1 | cut -d ' ' -f 2)
		rm /tmp/upg -f
		ln -s $TMPDISK /tmp/upg
	fi

	if [ -n "$FORM_uploadfile" ] ; then

		FILESIZE=$(cat $FORM_uploadfile | wc -c)

		if [ ${FILESIZE} -eq 4128768 ] ; then

			echo "<p>Full firmware uploaded. Compatibility with legacy firmware ON.</p>"
			echo "<p>Validating file...</p>"
			dd if=/dev/zero of=/tmp/upg/fw_upg_et.tmp bs=16 count=1
			dd if=$FORM_uploadfile of=/tmp/upg/fw_upg_nt.tmp bs=16 count=258047

			FILESUM=$(cat /tmp/upg/fw_upg_nt.tmp /tmp/upg/fw_upg_et.tmp | md5sum | cut -d ' ' -f 1 )
	    	EXPSUM=$(dd if=${FORM_uploadfile} bs=16 skip=258047 | hexdump -C | grep 00000000 | sed "s/00000000//g" | tr -d ' ' | cut -b 1-32)

			echo "<p>File md5sum is: ${FILESUM} , and expected md5sum is: ${EXPSUM} </p>"
			if [ "${FILESUM}" = "${EXPSUM}" ] ; then
				echo "<p>Upgrade in progress...<br>DON'T TURN OFF AND WAIT 5 MINUTES FOR THE DEVICE TO COMPLETE THE TASK.</p>"
				TURN_LED=$(printf "\x52" > /dev/nasleds &)
				/etc/init.d/opkg stop > /dev/null
				/etc/init.d/samba stop > /dev/null
				/etc/init.d/ftpsvc stop > /dev/null
				umount /etc > /dev/null
				cp /bin/busybox /tmp
				/tmp/busybox echo "Writing part 1/3..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock1 bs=65536 skip=4 count=2
				/tmp/busybox echo "done!<br>"
				/tmp/busybox echo "Writing part 2/3..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock2 bs=65536 skip=6 count=15
				/tmp/busybox echo "done!<br>"
				/tmp/busybox echo "Writing part 3/3..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock3 bs=65536 skip=21 count=42
				/tmp/busybox echo "done!<br>"
	            upg_done
			else
			      echo "<p><b>ERROR:</b> Wrong md5sum inside file, try to download the file again.</p>"
			fi
		elif [ ${FILESIZE} -eq 3866624 ] ; then
		
			echo "<p>Incremental SNAKE OS firmware uploaded.</p>"
			echo "<p>Validating file...</p>"
			dd if=/dev/zero of=/tmp/upg/fw_upg_et.tmp bs=16 count=1
			dd if=$FORM_uploadfile of=/tmp/upg/fw_upg_nt.tmp bs=16 count=241663

			FILESUM=$(cat /tmp/upg/fw_upg_nt.tmp /tmp/upg/fw_upg_et.tmp | md5sum | cut -d ' ' -f 1 )
			EXPSUM=$(dd if=${FORM_uploadfile} bs=16 skip=241663 | hexdump -C | grep 00000000 | sed "s/00000000//g" | tr -d ' ' | cut -b 1-32)

			echo "<p>File md5sum is: ${FILESUM} , and expected md5sum is: ${EXPSUM} </p>"
			if [ "${FILESUM}" = "${EXPSUM}" ] ; then
				echo "<p>Upgrade in progress...<br>DON'T TURN OFF AND WAIT 5 MINUTES FOR THE DEVICE TO COMPLETE THE TASK.</p>"
				TURN_LED=$(printf "\x52" > /dev/nasleds &)
				/etc/init.d/opkg stop > /dev/null
				/etc/init.d/samba stop > /dev/null
				/etc/init.d/ftpsvc stop > /dev/null
				umount /etc > /dev/null
				cp /bin/busybox /tmp
				/tmp/busybox echo "Writing config partition..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock1 bs=65536 count=2
				/tmp/busybox echo "done!<br>"
				/tmp/busybox echo "Writing kernel partition..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock2 bs=65536 skip=2 count=15
				/tmp/busybox echo "done!<br>"
				/tmp/busybox echo "Writing root partition..."
				/tmp/busybox dd if=${FORM_uploadfile} of=/dev/mtdblock3 bs=65536 skip=17 count=42
				/tmp/busybox echo "done!<br>"
				upg_done
			else
				echo "<p><b>ERROR:</b> Wrong md5sum inside file, try to download the file again.</p>"
			fi
			
		else
			echo "<p><b>ERROR:</b> Wrong file size (${FILESIZE} bytes), try to download the file again.</p>"
		fi
	else
%>
		<form action="<%= $SCRIPT_NAME %>" method=POST enctype="multipart/form-data" >
		<input type=file name=uploadfile>
		<input type=submit value=GO>
		</form>
<%
	fi

else
	  echo "<p>You need a mounted disk to make firmware changes. It's also STRONGLY advised to have SWAP and SSHD RUNNING, and other services STOPPED.</p>"
fi
%>


</body>
</html>
