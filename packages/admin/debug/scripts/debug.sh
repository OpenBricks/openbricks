########################
# GeeXboX Debug Traces #
########################

#!/bin/sh

DBG_FILE=/tmp/geexbox-debug.txt

clean_log () {
  echo "" > $DBG_FILE
}

log () {
  echo "$1" >> $DBG_FILE
}

section_log () {
  log " "
  log "################################"
  log "# $1 "
  log "################################"
  log " "
}

clean_log

# System
section_log "Kernel Messages"
log "`dmesg`"

section_log "Hardware Description"
log "`cat /tmp/pci`"

section_log "CPU Model"
log "`cat /proc/cpuinfo`"

section_log "Memory Information"
log "`cat /proc/meminfo`"

section_log "Command Line"
log "`cat /proc/cmdline`"

section_log "Loaded Modules"
log "`lsmod`"

section_log "Disks and Partitions"
log "`cat /proc/partitions`"

section_log "Mount Points"
log "`mount`"

# Sound
section_log "Sound Cards"
log "`cat /proc/asound/cards`"

section_log "Sound Devices"
log "`cat /proc/asound/devices`"

section_log "Audio Configuration"
log "`cat /etc/audio`"

# Network
section_log "Network Interfaces"
log "`ifconfig`"

section_log "Wireless Interfaces"
iwconfig >> $DBG_FILE 2>&1

# X.org related stuff
pidof Xorg 2>&1 > /dev/null
if [ $? = 0 ]; then
  if [ -f /tmp/modes ]; then
    section_log "Video Resolution"
    log "`cat /tmp/modes`"
  fi
  if [ -f /tmp/edid-xorg ]; then
    section_log "EDID Info"
    log "`cat /tmp/edid-xorg`"
  fi
  if [ -f /etc/X/X.cfg ]; then
    section_log "X User Configuration"
    log "`cat /etc/X/X.cfg`"
  fi
  if [ -f /xorg.conf.new ]; then
    section_log "X.org autodetection"
    log "`cat /xorg.conf.new`"
  fi
  if [ -f /etc/X11/xorg.conf ]; then
    section_log "X.org configuration file"
    log "`cat /etc/X11/xorg.conf`"
  fi
  if [ -f /tmp/xrandr ]; then
    section_log "XRandR Resolutions"
    log "`cat /tmp/xrandr`"
  fi
  if [ -f /var/log/Xorg.0.log ]; then
    section_log "X.Org Log status"
    log "`cat /var/log/Xorg.0.log`"
  fi
fi

# MPlayer
section_log "MPlayer Configuration"
log "`cat /etc/mplayer/mplayer.conf`"

echo "File $DBG_FILE has been generated"
