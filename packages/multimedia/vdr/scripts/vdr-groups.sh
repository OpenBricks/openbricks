#!/bin/sh
#
# This script checks which groups the vdr user should belong to and adds
# it to the necessary groups or removes it from groups which are not needed
# anymore
#
# (c) 2007, Thomas Schmidt <tschmidt@debian.org>
# (c) 2007, Tobias Grimm <tg@e-tobi.net>
#

DIR="/usr/share/vdr/groups.d"
VDR_USER=vdr

ACTUAL_GROUPS=`groups $VDR_USER | cut -d' ' -f3-`

is_group_in_list()
{
    local group=$1
    shift
    local group_list=$*
    local current_group

    for current_group in $group_list; do
        [ "$current_group" = "$group" ] && return
    done

    false
}

read_groups()
{
    cat "$1" | grep -v "^#\|^$" | sed s/"\(.*\)#.*"/"\1"/ | xargs
}

add_to_groups()
{
    local groups_file="$1"
    local groups=`read_groups "$groups_file"`
    local group

    for group in $groups; do
        if ! is_group_in_list $group $ACTUAL_GROUPS; then
            echo "Adding '$VDR_USER' to group '$group'"
            adduser $VDR_USER $group > /dev/null 2>&1
        fi
    done
}

remove_from_groups()
{
    local groups_file="$1"
    local groups=`read_groups "$groups_file"`
    local needed_groups
    local group

    needed_groups=`read_groups $DIR/*`

    for group in $groups; do
        if is_group_in_list $group $ACTUAL_GROUPS; then
            if ! is_group_in_list $group $needed_groups; then
                echo "Removing '$VDR_USER' from group '$group'"
                deluser $VDR_USER $group > /dev/null 2>&1
            fi
        fi
    done
}

show_help()
{
    echo
    echo "vdr-groups.sh"
    echo "-------------"
    echo "Shell script to be used by vdr plugin packages to register/deregister"
    echo "required vdr group memberships."
    echo
    echo "/bin/sh /usr/lib/vdr/vdr-groups.sh --add <GROUP-FILE>"
    echo "/bin/sh /usr/lib/vdr/vdr-groups.sh --remove <GROUP-FILE>"
    echo
    echo "The <GROUP-FILE> is the file in $DIR containing the list of groups"
    echo "vdr should be added to or removed from."
    echo
    exit 127
}

#
# main()
#


if [ $# -ne 2 ]; then
    show_help
fi

action="$1"
groups_file="$DIR/$2.groups"

if [ ! -e $groups_file ]; then
    echo "WARNING: $groups_file does not exist. Can't adjust vdr group membership"
    exit 0
fi

case "$action" in
    --add)
        add_to_groups "$groups_file"
        ;;
    --remove)
        remove_from_groups "$groups_file"
        ;;
    *)
        show_help
        ;;
esac

