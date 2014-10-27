MAKE_CONFIG="Make.config"

toggle_extension_patch()
{
    local patchname="$1"
    local patchaction="$2"
    local tmp
    
    case "$patchaction" in
        -patch) 
            echo "$patchname = 1" >>$MAKE_CONFIG
            echo "DEFINES += -DUSE_$patchname" >>$MAKE_CONFIG
            ;;
        -unpatch) 
            tmp=$MAKE_CONFIG.$$
            sed "/$patchname/d" $MAKE_CONFIG >$tmp
            mv $tmp $MAKE_CONFIG
            ;;
         *)
            echo >&2 "script expects -patch or -unpatch as argument"
            exit 1
            ;;
    esac
}
