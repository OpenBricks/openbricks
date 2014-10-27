export DISPLAY=:0
export HOME=/var/lib/vdr

XINELIBOUTPUTOPTS="--reconnect --syslog --config /etc/vdr-sxfe/config_xineliboutput"


if [ "x$(dbget system.hardware.nvidia.detected)" = "x1" ]; then                 
  vdpauinfo 2>/dev/null >/dev/null && XINELIBOUTPUTOPTS="$XINELIBOUTPUTOPTS --video=vdpau"
fi

while  ! netcat -z localhost 37890; do sleep 0.1; done;
/usr/bin/vdr-sxfe $XINELIBOUTPUTOPTS xvdr://127.0.0.1:37890
