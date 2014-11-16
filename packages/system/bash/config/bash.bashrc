# /etc/bash.bashrc

col_bold="\033[1m"
col_red="\033[1;31m"
col_green="\033[1;32m"
col_yellow="\033[1;33m"
col_blue="\033[1;34m"
col_purple="\033[1;35m"
col_cyan="\033[1;36m"
col_normal="\033[0;39m"

PS1="\[${col_cyan}\]\u@\h:\[${col_blue}\]\w\[${col_normal}\]\$ "
