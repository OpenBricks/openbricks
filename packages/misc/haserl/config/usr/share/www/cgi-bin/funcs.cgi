<head>
    <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
    <title>SNAKE</title>

<link rel="stylesheet" href="/css/default.css" type="text/css" />
<script language="JavaScript" type="text/javascript" src="/js/funcs.js"></script>
 
<%


set_config(){
	sed -ri "s%^${1}=.*%${1}=${2}%g" /etc/default/config
}

get_config(){
	echo -n $(sed -n "s/^\(${1}\)=\(.*\)$/\2/p" /etc/default/config)
}

set_config_mixer(){
	sed -ri "s%^${1}=.*%${1}=\"${2}\"%g" /etc/audio
}

get_config_mixer(){
	echo -n $(sed -n "s/^\(${1}\)=\(.*\)$/\2/p" /etc/audio) | tr -d \"
}

is_checked(){
    if [ ${1} = 1 ] ; then
      echo -n "checked"                                            
  fi
}

set_hostname(){
    hostname ${1}
    echo ${1} > /etc/hostname
    sed -ri "s,hostname=.*,hostname=${1},g" /etc/hostname
    sed -ri "s,server string = .*,server string = ${1},g" /etc/smb.conf 
    /bin/systemctl restart smbd.service > /dev/null
    echo "127.0.0.1 localhost" > /etc/hosts
    echo $(get_ipaddress) ${1} >> /etc/hosts
}

get_mac_address(){
    echo $(ifconfig eth0 | grep "HWa" | awk -F':' '{print $5":"$6":"$7}')
}

set_ipinfo(){
	set_config ip_address ${1}
	set_config netmask ${2}
	set_config gateway ${3}
	set_config dhcp_address ${4}	
}

set_dnsinfo(){
	set_config nameserver1 ${1}
	set_config nameserver2 ${2}
	set_config dnsdomain ${3}
	set_config dhcp_dns ${4}
}


saveandreload(){
    /usr/share/snake/config save  > /dev/null
    sleep 1
    reboot > /dev/null
}

get_ipaddress(){
    echo -n $(ifconfig eth0 | grep "inet" | tr -s ' ' | sed 's/:/ /g' | cut -d ' ' -f 4)
}

get_netmask(){
    echo -n $(ifconfig eth0 | grep "inet" | tr -s ' ' | sed 's/:/ /g' | cut -d ' ' -f 8)
}

get_gateway(){
    GW=$(route -n | grep "^0.0.0.0" | awk '{ print $2}')
    if [ "${GW:-none}" = "none" ] ; then
        echo -n "0.0.0.0"
    else
        echo -n ${GW}
    fi
}

get_nameserver1(){
    NS=$(grep "^nameserver" /etc/resolv.conf | tail -1 | awk '{ print $2}')
    if [ "${NS:-none}" = "none" ] ; then
        get_gateway
    else
        echo -n ${NS}
    fi
}

get_nameserver2(){
    NS=$(grep "^nameserver" /etc/resolv.conf | tail -2 | head -n 1 | awk '{ print $2}')
    if [ "${NS:-none}" = "none" ] ; then
        get_nameserver1
    else
        echo -n ${NS}
    fi
}

get_dnsdomain(){
    DOM=$(grep "^search" /etc/resolv.conf | tail -1 | awk '{ print $2}')
    if [ "${DOM:-none}" = "none" ] ; then
        echo -n "localdomain"
    else
        echo -n ${DOM}
    fi
}

get_mounted(){
    echo -n $(mount | grep "/usb/" | cut -d ' ' -f 3 | cut -d '/' -f 3)
}

get_part_mounted(){
    echo $(mount | sort | grep "/usb/" | cut -d ' ' -f 1 | cut -d '/' -f 3)
}

get_mounted_dev(){
    echo -n $(mount | sort | grep "/usb/$1" | cut -d ' ' -f 1 | cut -d '/' -f 3)
}

get_alldisks(){
    ls /sys/block/sd* -d | cut -d '/' -f 4
}

get_allparts(){
    ls /sys/block/$1/sd* -d | cut -d '/' -f 5
}

capitalize(){
    echo $(echo "$1" | cut -b 1 | tr a-z A-Z)$(echo "$1" | cut -b 2-)
}

%>

