#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "browser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#define MAX 512
#include "language_en.h"

/* ToDo
+ error on add file to list, reload mplayer ?
+ sort directory content
+ can not get dir atributes if dir contains + char
+ view what is currently playing
+ view position in track ?
*/

typedef enum {
  ENCODE_TYPE_HTML,
  ENCODE_TYPE_URL,
} encode_type_t;

/*
 * This function encodes some characters for HTML code or URL in order to
 * prevent error messages with web browsers and malformed XHTML tags.
 *
 * With URL type, the characters can be protected with '\'. If the same
 * character appears many times in protect string, more '\' will be added,
 * for example:
 *  str = foo'bar  and protect = ''   : result = foo\\\%27bar
 *  str = foo"bar' and protect = "''  : result = foo\%22bar\\\%27
 */
static char *escape_string(const char *str, encode_type_t type, const char *protect) {
	char c;
	int length = 0;
	char *buf = NULL;
	char it[32];

	while ((c = *str++)) {
		if (type == ENCODE_TYPE_HTML) {
			switch (c) {
			default:
				snprintf(it, sizeof(it), "%c", c);
				break;
			case '&':
				snprintf(it, sizeof(it), "&amp;");
				break;
			case '"':
				snprintf(it, sizeof(it), "&quot;");
				break;
			case '\'':
				snprintf(it, sizeof(it), "&#039;");
				break;
			case '<':
				snprintf(it, sizeof(it), "&lt;");
				break;
			case '>':
				snprintf(it, sizeof(it), "&gt;");
				break;
			}
		} else {
			char bs[16];
			int i = 0;
			if (protect) {
				const char *p = strchr(protect, c);
				while (p && i < (sizeof(bs) - 2)) {
					bs[i++] = '\\';
					if (i > 1) /* only one '\' with the first escape character */
						bs[i++] = '\\';
					p = strchr(p + 1, c);
				}
			}
			bs[i] = '\0';

			switch (c) {
			default:
				snprintf(it, sizeof(it), "%s%c", bs, c);
				break;
			case '&':
			case '"':
			case '\'':
			case '<':
			case '>':
			case '+':
			case '#':
				snprintf(it, sizeof(it), "%s%%%2X", bs, c);
				break;
			}
		}

		buf = realloc(buf, length + strlen(it) + 1);
		if (!buf)
			return NULL;
		length += snprintf(buf + length, strlen(it) + 1, "%s", it);
	}

	return buf;
}

const char *getCurrentDir(const char *dir) {
	if(dir) {
		setDir(dir);

		return dir;
	} else {
		return getDir();
	}
}

void printCurrentDir(const char *dir, const char *search) {
	char path[MAX] = "", *p = NULL;
	char name[MAX] = "", *c = NULL;
	char *p_e = NULL, *c_e;

	strcpy(name, dir);
	c = strtok(name, "/");

	printf("\t\t\t<div class=\"browserDirHead\">\n");
	while(c) {
		p = strcat(path, "/");
		p = strcat(path, c);
		if (p_e)
			free(p_e);
		p_e = escape_string(p, ENCODE_TYPE_URL, NULL);
		c_e = escape_string(c, ENCODE_TYPE_HTML, NULL);
		printf("\t\t\t\t / <a href=\"?dir=%s\">%s</a> \n", p_e ? p_e : "", c_e ? c_e : "");
		c = strtok(NULL,"/");
		if (c_e)
			free(c_e);
	}

	if(search) {
		char *search_e = escape_string(search, ENCODE_TYPE_HTML, NULL);
		printf("\t\t\t\t / search for '%s'<br />\n", search_e ? search_e : "?");
		if (search_e)
			free(search_e);
	} else {
		printf("\t\t\t\t<a href=\"?play_dir=%s\">\n\t\t\t\t\t<img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"Play Directory\" />\n\t\t\t\t</a>\n", p_e ? p_e : "", PLAY_DIR);
	}
	printf("\t\t\t</div>\n");

	if (p_e)
		free(p_e);
}

static const char *printFileIcon(const char *file) {
	if(strcasestr(file, ".mp3") || strcasestr(file, ".wav") || strcasestr(file, ".ogg") || strcasestr(file, ".flac")) {
		return "sound.png";
	} else if(strcasestr(file, ".avi") || strcasestr(file, ".mpg") || strcasestr(file, ".mpeg") || strcasestr(file, ".mpe") || strcasestr(file, ".mov") || strcasestr(file, ".wmv")) {
		return "video.png";
	} else if(strcasestr(file, ".jpg") || strcasestr(file, ".gif") || strcasestr(file, ".png") || strcasestr(file, ".bmp")) {
		return "image.png";
	} else if(strcasestr(file, ".iso") || strcasestr(file, ".cue")) {
		return "iso.png";
	} else if(strcasestr(file, ".pls") || strcasestr(file, ".m3u")) {
		return "playlist.png";
	} else {
		return "file.png";
	}
}

static const char *printDirIcon(const char *file) {
	if(strcasestr(file, "cdrom")) {
		return "cdrom.png";
	} else if(strcasestr(file, "shares")) {
		return "share.png";
	} else if(strcasestr(file, "nfs")) {
		return "nfs.png";
	} else if(strcasestr(file, "streams")) {	
		return "stream.png";
	} else {
		return "folder.png";
	}
}

static void printFile(const char *dir, const char *file) {
	char cmd[MAX] = "";
	char *dir_e, *file_e;
	if(strcasestr(file, ".pls") || strcasestr(file, ".m3u")) {
		strcpy(cmd, "mplayer=loadlist '");
	} else {
		strcpy(cmd, "mplayer=load '");
	} 

	file_e = escape_string(file, ENCODE_TYPE_HTML, NULL);

	printf("\t\t\t<div class=\"browserDirContent\">\n");
	printf("\t\t\t\t<img src=\"/style/geexbox/icons/%s\" alt=\"Icon\" />\n", printFileIcon(file));
	printf("\t\t\t\t%s\n", file_e ? file_e : "");

	if (file_e)
		free(file_e);

	dir_e = escape_string(dir, ENCODE_TYPE_URL, "'");
	file_e = escape_string(file, ENCODE_TYPE_URL, "'");

	printf("\t\t\t\t<a href=\"?%s%s/%s'\"><img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"%s\" /></a>\n", cmd, dir_e ? dir_e : "", file_e ? file_e : "", PLAY_FILE, PLAY_FILE);
	printf("\t\t\t</div>\n");

	if (dir_e)
		free(dir_e);
	if (file_e)
		free(file_e);
}

static void printDir(const char *dir, const char *file) {
	char *dir_e, *file_e, *file2_e;

	printf("\t\t\t<div class=\"browserDirContent\">\n");
	printf("\t\t\t\t<img src=\"/style/geexbox/icons/%s\" alt=\"Icon\"/>\n",printDirIcon(file));

	dir_e = escape_string(dir, ENCODE_TYPE_URL, NULL);
	file_e = escape_string(file, ENCODE_TYPE_URL, NULL);
	file2_e = escape_string(file, ENCODE_TYPE_HTML, NULL);

	printf("\t\t\t\t<a href=\"?dir=%s/%s\">%s</a>\n", dir_e ? dir_e : "", file_e ? file_e : "", file2_e ? file2_e : "");

	if (file2_e)
		free(file2_e);

	printf("\t\t\t\t<a href=\"?play_dir=%s/%s\"><img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"%s\" /></a>\n", dir_e ? dir_e : "", file_e ? file_e : "", PLAY_DIR, PLAY_DIR);
	printf("\t\t\t</div>\n");

	if (dir_e)
		free(dir_e);
	if (file_e)
		free(file_e);
}

void printCurrentDirContent(const char *dir, const char *name) {
	int dirNamePrinted = 0;
	struct dirent** pDirEnt;
	int n = scandir(dir, &pDirEnt, 0, alphasort);
	int i;
        for(i = 0; i < n; i++) {
		char *filename = pDirEnt[i]->d_name;
		if(strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
			char fullpath[MAX] = "";
			struct stat attributs;
			strcat(fullpath, dir);
			strcat(fullpath, "/");
			strcat(fullpath, filename);

			if(stat(fullpath, &attributs) == -1) {
				fprintf(stderr, "[!] Fehler beim auslesen der Attribute von \"%s\"\n", fullpath);
			}

			if(attributs.st_mode & S_IFDIR) {
				/* directory found */
				if(!name || strcasestr(filename, name)) {
					printDir(dir, filename);
				}
				
				/* recursive search */
				if(name) {
					printCurrentDirContent(fullpath, name);
				}
			} else {
				/* regular file found */
				if(!name || strcasestr(filename, name)) {
					if(name && dirNamePrinted == 0 && strcasestr(filename, name)) {
						printCurrentDir(dir, NULL);
						dirNamePrinted = 1;
					}
					printFile(dir, filename);
				}
			}
		}
		free(pDirEnt[i]);
	}
	free(pDirEnt);
}

static void printSeekBar(){
	printf("\t\t\t<div id=\"seekBar\">\n");
	printf("\t\t\t\t<a href=\"?mplayer=volume -10\"><img id=\"seekBarButtonVolumeDecrease\" src=\"/style/geexbox/seekBar/volumeDecreaseButton.png\" alt=\"miniWebmote\" /></a>\n");
	printf("\t\t\t\t<a href=\"?mplayer=volume +10\"><img id=\"seekBarButtonVolumeIncrease\" src=\"/style/geexbox/seekBar/volumeIncreaseButton.png\" alt=\"miniWebmote\" /></a>\n");
	printf("\t\t\t\t<a href=\"?mplayer=pt_step -1\"><img id=\"seekBarButtonPreviousTrack\" src=\"/style/geexbox/seekBar/previousTrackButton.png\" alt=\"miniWebmote\" /></a>\n");
	int i;
	for(i = 0; i <= 100; i += 2) {
		printf("\t\t\t\t<a href=\"?mplayer=seek %d 1\">", i);
		if(!(i % 10)) {
			printf("<img class=\"seekBarSeekField\" id=\"seekBar%d\" src=\"/style/geexbox/seekBar/2.png\" alt=\"Seekbar\" />", i);
		}
		else {
			printf("<img class=\"seekBarSeekField\" id=\"seekBar%d\" src=\"/style/geexbox/seekBar/1.png\" alt=\"Seekbar\" />", i);
		}
		printf("</a>\n");
	}
	printf("\t\t\t\t<a href=\"?mplayer=pt_step +1\"><img id=\"seekBarButtonNextTrack\" src=\"/style/geexbox/seekBar/nextTrackButton.png\" alt=\"miniWebmote\" /></a>\n");
	printf("\t\t\t\t<a href=\"?mplayer=pause\"><img id=\"seekBarButtonPause\" src=\"/style/geexbox/seekBar/pauseButton.png\" alt=\"miniWebmote\" /></a>\n");
	printf("\t\t\t\t<a href=\"?mplayer=mute\"><img id=\"seekBarButtonMute\" src=\"/style/geexbox/seekBar/muteButton.png\" alt=\"miniWebmote\" /></a>\n");
	printf("\t\t\t</div>\n");
}

void printPlayerControls(const char *play_file, const char *play_dir) {
	printf("\t\t<div id=\"browserBar\">\n");
	/* print seek */
	printSeekBar();

	/* print search */
	printf("\t\t\t<form id=\"searchBox\" action=\"?\" method=\"get\">\n");
	printf("\t\t\t\t<fieldset>\n");
	printf("\t\t\t\t\t<input type=\"image\" id=\"searchBoxButton\" src=\"/style/geexbox/searchBox/searchbutton.png\" alt=\"%s\" title=\"%s\" />\n", "Search", "Search");
	printf("\t\t\t\t\t<input type=\"text\" id=\"searchBoxTextField\" name=\"search\" value=\"\" />\n");
	printf("\t\t\t\t</fieldset>\n");
	printf("\t\t\t</form>\n");
	printf("\t\t</div>\n");
}
