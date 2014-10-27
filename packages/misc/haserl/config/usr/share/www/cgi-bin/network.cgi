#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%
if [ "${REQUEST_METHOD}" = "POST" ]
then
    NEWIPADD=$(echo ${FORM_ip} | cut -d ' ' -f 1)
    NEWMASK=$(echo ${FORM_mask} | cut -d ' ' -f 1)
    NEWGW=$(echo ${FORM_gw} | cut -d ' ' -f 1)
    NEWDHCP=$(echo ${FORM_dhcp_address} | cut -d ' ' -f 1)
    NEWDNS1=$(echo ${FORM_nameserver1} | cut -d ' ' -f 1)
    NEWDNS2=$(echo ${FORM_nameserver2} | cut -d ' ' -f 1)
    NEWDNSDOM=$(echo ${FORM_dnsdomain} | cut -d ' ' -f 1)
    NEWDHCPDNS=$(echo ${FORM_dhcp_dns} | cut -d ' ' -f 1)
    NEWMACADD=$(echo ${FORM_mac} | cut -d ' ' -f 1)
	set_config mac_address "AC:DE:48:${NEWMACADD}"
    set_ipinfo $NEWIPADD $NEWMASK $NEWGW ${NEWDHCP:-0}
    set_dnsinfo $NEWDNS1 $NEWDNS2 $NEWDNSDOM ${NEWDHCPDNS:-0}
    saveandreload
fi
%>
<script language="JavaScript">
<!-- //
function checkDhcp(){                                                           
    if ( <% get_config dhcp_address %> == 1 ){                                         
        setReadIP(document.forms[0].dhcp_address);                              
        setReadDNS(document.forms[0].dhcp_dns);                                 
    }                                                                           
    else {                                                                      
        document.forms[0].dhcp_dns.checked = false;                             
        document.forms[0].dhcp_dns.disabled = true;                             
    }                                                                           
} 

function setReadIP(obj){
    if(obj.checked)
    {
        document.forms[0].ip.readOnly = 1;
        document.forms[0].mask.readOnly = 1;
        document.forms[0].gw.readOnly = 1;
        } 
    else {
        document.forms[0].ip.readOnly = 0;
        document.forms[0].mask.readOnly = 0;
        document.forms[0].gw.readOnly = 0;    
        }
}

function setReadAll(obj){                                                       
    if(obj.checked)                                                             
    {                                                                           
        document.forms[0].ip.readOnly = 1;                                      
        document.forms[0].mask.readOnly = 1;                                    
        document.forms[0].gw.readOnly = 1;                                      
        document.forms[0].dhcp_dns.disabled = false;                            
        document.forms[0].dhcp_dns.checked = true;                              
        document.forms[0].nameserver1.readOnly = 1;
        document.forms[0].nameserver2.readOnly = 1;                              
        document.forms[0].dnsdomain.readOnly = 1;                               
                                                                                
        }                                                                       
    else {                                                                      
        document.forms[0].ip.readOnly = 0;                                      
        document.forms[0].mask.readOnly = 0;                                    
        document.forms[0].gw.readOnly = 0;                                      
        document.forms[0].dhcp_dns.checked = false;                             
        document.forms[0].dhcp_dns.disabled = true;                             
        document.forms[0].nameserver1.readOnly = 0;
        document.forms[0].nameserver2.readOnly = 0;                              
        document.forms[0].dnsdomain.readOnly = 0; 
    }
}


function setReadDNS(obj){
    if(obj.checked)
    {
        document.forms[0].nameserver1.readOnly = 1;
        document.forms[0].nameserver2.readOnly = 1;
        document.forms[0].dnsdomain.readOnly = 1;
        } 
    else {
        document.forms[0].nameserver1.readOnly = 0;
        document.forms[0].nameserver2.readOnly = 0;
        document.forms[0].dnsdomain.readOnly = 0;
   }
}

    function validateAllIP (form) {                                             
        var ip = validateIP(form.ip.value);                                     
        var netmask = validateNetmask(form.mask.value);                       
        var gateway = validateIPz(form.gw.value);  
        var nameserver1 = validateIPz(form.nameserver1.value);
        var nameserver2 = validateIPz(form.nameserver2.value);
		var mac = validateMAC(form.mac.value);
        if ( ip && netmask && gateway && nameserver1 && nameserver2 && mac){                                                     
               var answer = confirm ("You have changed network settings and this device will reboot now. Do you want to continue?");
               if (answer)                                                      
                   return true;                                                 
                else                                                            
                    return false;                                               
                 }                                                              
        else                                                                
            return false;                                                   
    } 

    function validateNetmask(mask) {
    	if (validateMask(mask))
            return true;
    	else{
             alert("Invalid Network Mask: " + mask);
             return false;
        }
    }

// -->
</script>
</head>
<body onload="checkDhcp();">                            

<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAllIP(this);">
<TABLE border="0">
	<TR><TH>Use DHCP Address:</TH><TD><input type="checkbox" name="dhcp_address" value="1" <% is_checked $(get_config dhcp_address) %> onclick="setReadAll(this);"/></TD></TR>

	<TR><TH>IP Address:</TH><TD><input type="text" name="ip" size=15 value=<% get_ipaddress %> title="Enter a new IP Address here. Format: nnn.nnn.nnn.nnn" /></TD></TR>
	<TR><TH>Netmask:</TH><TD><input type="text" name="mask" size=15 value=<% get_netmask %> title="Enter a new Netmask here. Format: nnn.nnn.nnn.nnn" /></TD></TR>
	<TR><TH>Gateway:</TH><TD><input type="text" name="gw" size=15 value=<% get_gateway %> title="Enter a new Gateway here. Format: nnn.nnn.nnn.nnn" /></TD></TR>

	<TR><TH>Use DHCP for DNS:</TH><TD><input type="checkbox" name="dhcp_dns" value="1" <% is_checked $(get_config dhcp_dns) %> onclick="setReadDNS(this);"/></TD></TR>

	<TR><TH>Primary DNS Server:</TH><TD><input type="text" name="nameserver1" size=15 value=<% get_nameserver1 %> title="Enter a DNS IP Address here. Format: nnn.nnn.nnn.nnn" /></TD></TR>
	<TR><TH>Secondary DNS Server:</TH><TD><input type="text" name="nameserver2" size=15 value=<% get_nameserver2 %> title="Enter a DNS IP Address here. Format: nnn.nnn.nnn.nnn" /></TD></TR>
	<TR><TH>Local DNS Domain:</TH><TD><input type="text" name="dnsdomain" size=15 value=<% get_dnsdomain %> title="Enter a local domain here." /></TD></TR>

	<TR><TH>Mac Address:</TH><TD>AC:DE:48:<input type="text" name="mac" size=8 value=<% get_mac_address %> title="Enter a new Mac Address here. Format: AC:DE:48:01:23:45"></TD></TR>
</TABLE>
   <input type="submit" name="action" value="Apply">
</form>

</body>
</html>
