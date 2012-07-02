# System Settings

export PATH="/bin:/sbin:/usr/bin:/usr/sbin"
export HOSTNAME=$(cat /etc/hostname)
[ -r /etc/locale.conf ] && . /etc/locale.conf && export LANG

alias ll='ls -l'
