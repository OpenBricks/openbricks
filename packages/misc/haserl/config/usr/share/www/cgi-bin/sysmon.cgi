#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">
  
<%in funcs.cgi %>
<%

TOTALRAM=$(free | grep Mem | awk '{print $2}')
USEDRAM=$(free | grep Mem | awk '{print $3}')
PCT_RAM=$((${USEDRAM}00/$TOTALRAM))
FREE_PCT_RAM=$((100-$PCT_RAM))
RAMBAR=$(($FREE_PCT_RAM*5/4))

                             
SWAPON=$(cat /proc/swaps | grep swapfile | wc -l)
if [ $SWAPON -eq 1 ] ; then  
    TOTALSWAP=$(free | grep Swap | awk '{print $2}')
    USEDSWAP=$(free | grep Swap | awk '{print $3}')
    PCT_SWAP=$((${USEDSWAP}00/$TOTALSWAP))
    FREE_PCT_SWAP=$((100-$PCT_SWAP))
    SWAPBAR=$(($FREE_PCT_SWAP*5/4))
fi

FREE_CPU=$(top -b -d 1 -n 3 | grep "^CPU" | tail -n -1 | tr -s ' ' | cut -d ' ' -f 8 | tr -d % | cut -d "." -f 1)
CPU_LOAD=$((100-$FREE_CPU))
CPUBAR=$(($FREE_CPU*5/4))

get_diskusage(){
    for DISK in $(get_alldisks) ; do
        get_diskusagerow $DISK
    done
}

get_diskusagerow(){
    DEV=$1
    for PART in $(get_allparts $DEV) ; do
        MOUNTINFO=$(cat /proc/mounts | grep "$PART")
	MOUNTED=$(echo $MOUNTINFO | cut -d ' ' -f 2)
        MOUNTED=${MOUNTED:-none}
        if [ "$MOUNTED" != "none" ] ; then
	    DFPCT=$(df -h $MOUNTED | cut -d 'U' -f 4 | tr -s ' ' | cut -d ' ' -f 5 | tr -d %)
            PIENUM=$((${DFPCT}/5))
            echo -n "<TR><TH align=center>$MOUNTED</TH><TD align=center>$DFPCT %</TD><TD><center><div class=\"pie f$PIENUM\"></div></TD></TR>"
        fi
    done
}

print_swap(){
    if [ $SWAPON -eq 1 ] ; then  
        echo -n "<TD>$PCT_SWAP %</TD><TD><img src=\"/img/pctImg.png\" alt=\"$PCT_SWAP %\" title=\"$PCT_SWAP %\" class=\"pctImg\" style=\"background-position: -$SWAPBAR.0px 0pt;\" /></TD>"
    else
        echo -n "<TD>- %</TD><TD>Swap Off</TD>"
    fi
}

%> 
<meta http-equiv='refresh' content='30'>
</head>
<body>
<center> 
<TABLE border="0" align=center>
<TR><TH COLSPAN=3>System Monitor</TH></TR>
<TR><TH>RAM Usage:</TH><TD><%= $PCT_RAM %>%</TD><TD><img src="/img/pctImg.png" alt="<%= $PCT_RAM %>%" title="<%= $PCT_RAM %>%" class="pctImg" style="background-position: -<%= $RAMBAR %>.0px 0pt;" /></TD></TR>
<TR><TH>Swap Usage:</TH><% print_swap %></TR>
<TR><TH>CPU Load:</TH><TD><%= $CPU_LOAD %>%</TD><TD><img src="/img/pctImg.png" alt="<%= $CPU_LOAD %>%" title="<%= $CPU_LOAD %>%" class="pctImg" style="background-position: -<%= $CPUBAR %>.0px 0pt;" /></TD></TR>
<TR><TH COLSPAN=3>Disk Usage Summary</TH></TR>
<TR><TH>Mounted</TH><TH>Pct Used</TH><TH>Usage Graph</TH></TR>
<% get_diskusage %>
</TABLE>
</body>
</html>

