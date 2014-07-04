#!/bin/sh
# Sample script that can be used to start MiniDLNA as a service.
# The script can be customized to wait for certain environmental conditions
# to be established (such as having obtained a DHCP IP address) before
# starting MiniDLNA.

# ------------------------------------------------------------------------------
# If your system uses DHCP to acquire an IP address on the network, you may
# need to delay the start of MiniDLNA until the system has acquired an address
# otherwise MiniDLNA will fail to start.
# Thanks to: https://ask.fedoraproject.org/en/question/10640/how-to-start-a-systemd-service-after-dhcp-has-initialized-the-ip-adress/?answer=27797#post-id-27797
#
# Here is an example:
#  # Wait until we have an IP address with '192' in it
#  ip_row=`ifconfig | grep 192`
#  while [ ! -n "$ip_row" ]
#  do
#    echo Waiting for IP address ...
#    ip_row=`ifconfig | grep 192`
#    sleep 2
#  done

# ------------------------------------------------------------------------------
# If you have your media library on an external drive, you will want to
# wait to start MiniDLNA until the external drive is mounted.  If you
# start MiniDLNA before the external drive is mounted, then MiniDLNA will
# either:
#   (a) not locate any media for the library
#   (b) remove from the library all media on the external drive
#
# Here is an example:
#  # Wait until the USB drive is mounted ...
#  while [ ! -d "/media/Elements/Media" ]
#  do
#    echo Waiting for /media/Elements/Media ...
#    sleep 2
#  done

# ------------------------------------------------------------------------------
# OK, now we are ready to start MiniDLNA!
echo Starting MiniDLNA ...
/usr/sbin/minidlnad -f /etc/minidlna.conf -P /var/run/minidlna/minidlna.pid
