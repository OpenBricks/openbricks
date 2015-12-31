# System Settings

export PATH="/bin:/sbin:/usr/bin:/usr/sbin"
export HOSTNAME=$(cat /etc/hostname)

[ -r /etc/locale.conf ] && . /etc/locale.conf && export LANG

update_ps1() {
  cur_user=$USER
  cur_pwd=$PWD
  ech="\$"
  [ "$cur_user" = "root" ] && ech="#"
  [ "$cur_pwd" = "$HOME" ] && cur_pwd="~"
  PS1="$cur_user@$HOSTNAME:$cur_pwd$ech "
}

my_cd() {
  chdir "$1"
  update_ps1
}

if ps ax -o pid,comm | grep `echo $$` | grep -q bash ; then
  ## it's bash
  [ -r /etc/bash.bashrc ] && . /etc/bash.bashrc
else
  ## it's sh from busybox
  alias cd=my_cd
  update_ps1
fi

alias ll='ls -l'
