/*
 * MPlayer-Plugin for LCD4Linux, based on XMMS-Plugin by Markus Keil
 * Copyright (C) 2006-2007 Guillaume LECERF <foxcore@gmail.com>
 * Copyright (C) 2003 Markus Keil <markus_keil@t-online.de>
 */

/*
 * exported functions:
 *
 * int plugin_init_mplayer (void)
 *  adds parser for /tmp/mp_streaminfo
 *
 */


/*
 * The Argument 'arg1' must be one of these Things (without quotes):
 *
 * 'ID_FILENAME'
 * 'ID_DEMUXER'
 * 'ID_LENGTH'
 *
 * For video stream :
 *   'ID_VIDEO_FORMAT'
 *   'ID_VIDEO_BITRATE'
 *   'ID_VIDEO_WIDTH'
 *   'ID_VIDEO_HEIGHT'
 *   'ID_VIDEO_FPS'
 *   'ID_VIDEO_ASPECT'
 *
 * For audio stream
 *   'ID_AUDIO_FORMAT'
 *   'ID_AUDIO_BITRATE'
 *   'ID_AUDIO_RATE'
 *   'ID_AUDIO_NCH'
 *   'ID_AUDIO_CODEC'
 *
 * MP3 Tags if available
 *   'ID_AUDIO_TITLE'
 *   'ID_AUDIO_ARTIST'
 *   'ID_AUDIO_ALBUM'
 *   'ID_AUDIO_YEAR'
 *   'ID_AUDIO_COMMENT'
 *   'ID_AUDIO_TRACK'
 *   'ID_AUDIO_GENRE'
 *
 * These arguments are case-sensitive
 */


#include "config.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#include "hash.h"
#include "debug.h"
#include "plugin.h"

const char STREAMINFO_FIFO[] = "/tmp/mp_streaminfo";

static HASH mplayer;


static int parse_mplayer_info(const char *tmpfile)
{
    int age;
    FILE *streaminfo_fifo;
    char line[200];

    /* reread every 100msec only */
    age = hash_age(&mplayer, NULL);
    if (age >= 0 && age <= 100)
	return 0;

    /* Open Filestream for tmpfile */
    streaminfo_fifo = fopen(tmpfile, "r");

    /* Check for File */
    if (!streaminfo_fifo)
      return 0;

    /* Read lines from the fifo */
    while (fgets(line, sizeof(line), streaminfo_fifo)) {
        char *c, *key, *val;
        c = strchr(line, '=');
        if (c == NULL)
            continue;
        key = line;
        val = c + 1;
        /* strip leading blanks from key */
        while (isspace(*key))
            *key++ = '\0';
        /* strip trailing blanks from key */
        do
            *c = '\0';
        while (isspace(*--c));
        /* strip leading blanks from value */
        while (isspace(*val))
            *val++ = '\0';
        /* strip trailing blanks from value */
        for (c = val; *c != '\0'; c++);
        while (isspace(*--c))
            *c = '\0';
        hash_put(&mplayer, key, val);
    }

    fclose(streaminfo_fifo);
    return 0;

}

static void my_mplayer(RESULT * result, RESULT * arg1)
{
    char *key, *val;

    if (parse_mplayer_info(STREAMINFO_FIFO) < 0) {
        SetResult(&result, R_STRING, "");
        return;
    }

    key = R2S(arg1);
    val = hash_get(&mplayer, key, NULL);

    if (val == NULL)
        val = "";

    SetResult(&result, R_STRING, val);
}


int plugin_init_mplayer(void)
{
    hash_create(&mplayer);

    /* register mplayer info */
    AddFunction("mplayer", 1, my_mplayer);

    return 0;
}

void plugin_exit_mplayer(void)
{
    hash_destroy(&mplayer);
}
