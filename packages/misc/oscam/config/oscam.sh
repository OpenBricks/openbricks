#!/bin/bash
. /etc/funcs

wait_for_network
/usr/bin/oscam -d 128 -c /usr/local/etc
