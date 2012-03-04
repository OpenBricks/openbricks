#!/bin/sh

echo "checkurl_dl.sh: Using checkurl output: $1"
echo "Downloading files missing on sources.openbricks.org..."

while read line
do

  if [ ${line##* } != "failed" ]
  then

    url=`echo $line | cut -d \   -f 5`

    echo -n "Downloading $url... "
    wget --no-check-certificate --passive-ftp --dns-timeout=5 --connect-timeout=5 --read-timeout=5 --tries=1 -c -q $url || wget -c -q $url
    if [ $? = 0 ]
    then
      echo OK
    else
      echo FAILED
    fi

  fi

done < $1

