#!/bin/bash
# Script per il controllo deli files di log e rotazione di essi per risparmiare spazio

INITIALSPACE=`df | grep rootfs | awk '{print $4}'`
echo $INITIALSPACE >> /var/log/percentuale

while [ 1 ]
do
   sleep 500
   ACTUALSPACE=`df | grep rootfs | awk '{print $4}'`
   PERCENT=`expr $ACTUALSPACE \* 100 / $INITIALSPACE`
   echo $PERCENT >> /var/log/percentuale
   echo $ACTUALSPACE >> /var/log/percentuale
   echo $INITIALSPACE >> /var/log/percentuale

   if [ -s "/var/log/syslog" ]; then
      sdim=`ls -al /var/log/syslog | awk '{print $5}'`
      if [ "$sdim" -gt "10240000" ]; then
         if [ -f "/var/log/syslog.gz" ]; then
            cp /var/log/syslog.gz /var/log/syslog.gz.1
            rm /var/log/syslog.gz
         fi
         gzip /var/log/syslog
       fi
   fi
   if [ -s "/var/log/kern.log" ]; then
      kdim=`ls -al /var/log/kern.log | awk '{print $5}'`
      if [ "$kdim" -gt "10240000" ]; then
         if [ -f "/var/log/kern.log.gz" ]; then
            cp /var/log/kern.log.gz /var/log/kern.log.gz.1
            rm /var/log/kern.log.gz
         fi
         gzip /var/log/kern.log
      fi
   fi
   if [ -s "/var/log/messages" ]; then
      mdim=`ls -al /var/log/messages | awk '{print $5}'`
      if [ "$mdim" -gt "10240000" ]; then
         if [ -f "/var/log/messages.gz" ]; then
            cp /var/log/messages.gz /var/log/messages.gz.1
            rm /var/log/messages.gz
         fi
         gzip /var/log/messages
      fi
   fi
   if [ -s "/var/log/debug" ]; then
      ddim=`ls -al /var/log/debug | awk '{print $5}'`
      if [ "$ddim" -gt "10240000" ]; then
         if [ -f "/var/log/debug.gz" ]; then
            cp /var/log/debug.gz /var/log/debug.gz.1
            rm /var/log/debug.gz
         fi
         gzip /var/log/debug
      fi
   fi
   if [ ! -s "/var/log/messages" ]; then
      /bin/systemctl restart rsyslog.service
   fi
   if [ ! -s "/var/log/syslog" ]; then
      /bin/systemctl restart rsyslog.service
   fi
   if [ ! -s "/var/log/kern.log" ]; then
      /bin/systemctl restart rsyslog.service
   fi
   if [ ! -s "/var/log/debug" ]; then
      /bin/systemctl restart rsyslog.service
   fi

done

