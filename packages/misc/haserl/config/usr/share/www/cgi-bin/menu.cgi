#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<head>
	<title>SIFBOX</title>

	<link rel="StyleSheet" href="../css/dtree.css" type="text/css" />
	<script type="text/javascript" src="../js/dtree.js"></script>

</head>

<body>

<div class="dtree">
	<p><a href="javascript: d.openAll();">open all</a> | <a href="javascript: d.closeAll();">close all</a></p>

	<script type="text/javascript">
		<!--
		d = new dTree('d');
		d.config.target = "main";
		d.config.useCookies = true;
		d.add(0,-1,'SifBoX','status.cgi','SifBoX Open Box Mediacenter status','','');
		
		d.add(1,0,'Device','','','','');
			d.add(2,1,'Hostname','hostname.cgi','Change hostname','','');
			d.add(3,1,'LAN Settings','network.cgi','Network settings','','');
			d.add(4,1,'Disk Info','diskinfo.cgi','Disk Information','','');
			d.add(5,1,'Status','status.cgi','System status','','');
			d.add(6,1,'Log','syslog.cgi','System log','','');
			d.add(7,1,'Monitor','sysmon.cgi','System monitoring','','');
			
		d.add(20,0,'Services','','','','');
			d.add(21,20,'Vdr','mg-vdr.cgi','Vdr Service','','');
			d.add(22,20,'SSH','mg-ssh.cgi','Manage SSH daemon','','');
			d.add(23,20,'Alsa Mixer','mg-alsamixer.cgi','Manage Alsa Mixer service','','');
			d.add(24,20,'Syslog','mg-syslog.cgi','Manage System Logging','','');
			
		document.write(d);

		//-->
	</script>

</div>

</body>

</html>
