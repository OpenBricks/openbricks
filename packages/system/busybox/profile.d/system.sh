# System Settings

# System Settings

export PATH="/bin:/sbin:/usr/bin:/usr/sbin"
export HOSTNAME=$(cat /etc/hostname)
[ -r /etc/locale.conf ] && . /etc/locale.conf && export LANG

update_ps1() {
  cur_user=$USER
  cur_pwd=$(pwd)
  ech="\$"
  if [ $cur_user = "root" ]; then
    ech="#"
  fi
  if [ $cur_pwd = $HOME ]; then
    cur_pwd="~"
  fi
  PS1="$cur_user@$HOSTNAME:$cur_pwd$ech "
}

my_cd() {
 chdir $@
 update_ps1
}

if ps ax -o pid,comm | grep `echo $$` | grep -q bash ; then
  ## it's bash
  col_bold="\033[1m"
  col_red="\033[1;31m"
  col_green="\033[1;32m"
  col_yellow="\033[1;33m"
  col_blue="\033[1;34m"
  col_purple="\033[1;35m"
  col_cyan="\033[1;36m"
  col_normal="\033[0;39m"

  PS1="\[${col_cyan}\]\u@\h:\[${col_blue}\]\w\[${col_normal}\]\$ "
  export PS1
else
  ## it's sh from busybox
  alias cd=my_cd
  update_ps1
fi

alias ll='ls -l'
