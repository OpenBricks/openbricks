#!/bin/sh

if [ ! -s /etc/bind/rndc.key ]; then
    echo -n "Generating /etc/bind/rndc.key:"
    /usr/sbin/rndc-confgen -a -c /etc/bind/rndc.key -b 512 -r /dev/urandom
    #chown root:bind /etc/bind/rndc.key
    chmod 0640 /etc/bind/rndc.key
fi
