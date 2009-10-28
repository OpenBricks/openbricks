#!/bin/sh

# Check http://www.sandpile.org/ia32/cpuid.htm for info on CPUID

# Request:
# http://www.geexbox.org/php/stats/stat_request.php?mac=%s&arch=%s&cpu_vendor=%s&cpu_type=%s&cpu_cores=%s&cpu_freq=%s&ram=%s&gpu=%s&hdtv=%s

# See Results on:
# http://www.geexbox.org/php/stats/stats.php

parse_cpuinfo () {
  echo `cat /proc/cpuinfo | grep "^$1" | head -n 1 | sed 's%.*: \(.*\)%\1%'`
}

parse_intel_cpu () {
  case "$CPU_FAMILY" in
    3)
      type="386"
      ;;
    4)
      type="486"
      ;;
    5)
      type="Pentium"
      ;;
    6)
      case "$CPU_MODEL" in
        0|1)
          type="Pentium MMX"
          ;;
        3|5|6)
          type="Pentium 2"
          ;;
        7|8|10|11)
          type="Pentium 3"
          ;;
        9|13|14)
          type="Pentium M"
          ;;
        15|22|23)
          type="Core 2"
          ;;
        26)
          type="Core i7"
          ;;
        28)
          type="ATOM"
          ;;
        29)
          type="Xeon MP"
          ;;
        *)
          type="Other"
          ;;
      esac
      ;;
    7)
      type="Itanium"
      ;;
    15)
      type="Pentium 4"
      ;;
    16|17)
      type="Itanium 2"
      ;;
    *)
      type="Other"
      ;;
  esac

  CPU_TYPE="$type"
}

parse_amd_cpu () {
  case "$CPU_FAMILY" in
    4)
      type="486"
      ;;
    5)
      case "$CPU_MODEL" in
        0|1|2|3)
          type="K5"
          ;;
        6|7|8|9|13)
          type="K6"
          ;;
        *)
          type="Other"
          ;;
      esac
      ;;
    6)
      case "$CPU_MODEL" in
        1|2|4|6|8|10)
          type="K7 (Athlon)"
          ;;
        3|7)
          type="K7 (Duron)"
          ;;
        *)
          type="Other"
          ;;
      esac
      ;;
    15|31)
      type="K8 (Athlon 64)"
      ;;
    *)
      type="Other"
      ;;
  esac

  CPU_TYPE="$type"
}

# CPU Architecure
CPU_ARCH=`uname -m`
CPU_VENDOR_ID=`parse_cpuinfo "vendor_id"`
CPU_FAMILY=`parse_cpuinfo "cpu family"`
CPU_MODEL=`parse_cpuinfo "model"`
CPU_MODEL_NAME=`parse_cpuinfo "model name"`
CPU_CORES=`cat /proc/cpuinfo | grep '^processor' | wc -l`
MAX_FREQ="/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
if [ -e "$MAX_FREQ" ]; then
  FREQ=`cat "$MAX_FREQ"`
  CPU_FREQ="$(($FREQ/1000))"
else
  FREQ=`parse_cpuinfo "cpu MHz"`
  CPU_FREQ=`echo $FREQ | sed 's%\(.*\)\..*%\1%'`
fi

case "$CPU_VENDOR_ID" in
  GenuineIntel)
    CPU_VENDOR="Intel"
    parse_intel_cpu
    ;;
  AuthenticAMD)
    CPU_VENDOR="AMD"
    parse_amd_cpu
    ;;
  CentaurHauls)
    CPU_VENDOR="VIA"
    CPU_TYPE="C3"
    ;;
  *)
    CPU_VENDOR="Other"
    ;;
esac

# Memory
RAM_TOTAL=`sed -n 's/MemTotal:[ \t]*\([0-9]*\).*/\1/p' /proc/meminfo`
RAM=$(($RAM_TOTAL/1024))

# HDTV
[ -f /var/use_xorg ] && HDTV=1 || HDTV=0

# MAC Address
ETH=`ifconfig | grep HWaddr | head -n 1 | sed 's%.*HWaddr \(.*\)%\1%' | cut -d ' ' -f 1`

# GPU
GPU_ID=`lspci -n | grep 0300: | sed 's%.*: \(.*\):.*%\1%'`
case "$GPU_ID" in
  1002|1022)
    GPU="ATI/AMD"
    ;;
  10de)
    GPU="nVidia"
    ;;
  1106)
    GPU="VIA"
    ;;
  15ad)
    GPU="VMware"
    ;;
  8086)
    GPU="Intel"
    ;;
  *)
    GPU="Other"
    ;;
esac

# Compute request URL
URL="http://www.geexbox.org/php/stats/stat_request.php?mac=${ETH}&arch=${CPU_ARCH}&cpu_vendor=${CPU_VENDOR}&cpu_type=${CPU_TYPE}&cpu_cores=${CPU_CORES}&cpu_freq=${CPU_FREQ}&ram=${RAM}&gpu=${GPU}&hdtv=${HDTV}"

if [ "$1" = debug ]; then
  # Display Informations:
  echo "Architecture: \"$CPU_ARCH\""
  echo "CPU Vendor: \"$CPU_VENDOR\""
  echo "CPU Type: \"$CPU_TYPE\""
  echo "CPU Cores: \"$CPU_CORES\""
  echo "CPU Frequency: \"$CPU_FREQ\""
  echo "Memory: \"$RAM\""
  echo "HDTV: \"$HDTV\""
  echo "MAC Address: \"$ETH\""
  echo "GPU: \"$GPU\""
  echo ""
  echo "URL: $URL"
else
  # send request to geexbox.org web server
  wget "$URL" -o /dev/null
fi
