#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">
  
<%in funcs.cgi %>

<meta http-equiv='refresh' content='60'>
</head>
<body>
<center> 
<h2>System Log</h2>
</center>
<p align=right><a href="./syslog-d.cgi">Sort Descending</a></p>
<% tail -n 200 $(get_config syslog_path) | while read line ; do echo $line"<br>" ; done  %>
</body>
</html>

