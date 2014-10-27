#!/bin/sh

if [ -f "/tmp/ecm.info" ]; then
  ECMFROM=`cat /tmp/ecm.info | grep from`
  ECMCAID=`cat /tmp/ecm.info | grep caid`
  ECMHOPS=`cat /tmp/ecm.info | grep hops`
  ECMTIME=`cat /tmp/ecm.info | grep time`
  MEX=$ECMFROM' '$ECMCAID' '$ECMHOPS' '$ECMTIME
  cat /tmp/ecm.info
else
  ECMFROM=""
  ECMCAID=""
  ECMHOPS=""
  ECMTIME=""
fi

