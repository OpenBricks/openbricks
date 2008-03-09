#!/bin/sh

mute_off () {
    amixer set iSpeaker on > /dev/null
}

mute_on () {
    amixer set iSpeaker off > /dev/null
}

amixer cget numid=1 | grep -q off && mute_off || mute_on
