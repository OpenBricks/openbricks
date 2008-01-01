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
+ fix error when playing files whose name includes ' ( ) & #
+ sort directory content
+ can not get dir atributes if dir contains + char
+ view what is currently playing
+ view position in track ?
*/

const char *getCurrentDir(const char *dir) {
	if(dir != NULL) {
		setDir(dir);
		
		return dir;
	} else {
		return getDir();
	}
}

void printCurrentDir(const char *dir, const char *search) {
	char path[MAX] = "", *p = '\0';
	char name[MAX] = "", *c = '\0';

	strcpy(name, dir);
	c = strtok(name, "/");
	
	printf("\t\t\t<div class=\"browserDirHead\">\n");
	while(c) {
		p = strcat(path, "/");
		p = strcat(path, c);
		printf("\t\t\t\t / <a href=\"?dir=%s\">%s</a> \n", p, c);
		c = strtok(NULL,"/");
	}
	
	if(search != NULL) {
		printf("\t\t\t\t / search for '%s'<br />\n", search);
	} else {
		printf("\t\t\t\t<a href=\"?play_dir=%s\">\n\t\t\t\t\t<img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"Play Directory\" />\n\t\t\t\t</a>\n", p, PLAY_DIR);
	}
	printf("\t\t\t</div>\n");
}

const char *printFileIcon(const char *file) {
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

const char *printDirIcon(const char *file) {
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

void printFile(const char *dir, const char *file) {
	char cmd[MAX] = "";
	if(strcasestr(file, ".pls") || strcasestr(file, ".m3u")) {
		strcpy(cmd, "mplayer=loadlist '");
	} else {
		strcpy(cmd, "mplayer=load '");
	} 
	
	printf("\t\t\t<div class=\"browserDirContent\">\n");
	printf("\t\t\t\t<img src=\"/style/geexbox/icons/%s\" alt=\"Icon\" />\n", printFileIcon(file));
	printf("\t\t\t\t%s\n", file);
	printf("\t\t\t\t<a href=\"?%s%s/%s'\"><img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"%s\" /></a>\n", cmd, dir, file, PLAY_FILE, PLAY_FILE);
	printf("\t\t\t</div>\n");
}

void printDir(const char *dir, const char *file) {
	printf("\t\t\t<div class=\"browserDirContent\">\n");
	printf("\t\t\t\t<img src=\"/style/geexbox/icons/%s\" alt=\"Icon\"/>\n",printDirIcon(file));
	printf("\t\t\t\t<a href=\"?dir=%s/%s\">%s</a>\n", dir, file, file);
	printf("\t\t\t\t<a href=\"?play_dir=%s/%s\"><img src=\"/style/geexbox/icons/play.png\" title=\"%s\" alt=\"%s\" /></a>\n", dir, file, PLAY_DIR, PLAY_DIR);
	printf("\t\t\t</div>\n");
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
				if(name == NULL || strcasestr(filename, name)) {
					printDir(dir, filename);
				}
				
				/* recursive search */
				if(name != NULL) {
					printCurrentDirContent(fullpath, name);
				}
			} else {
				/* regular file found */
				if(name == NULL || strcasestr(filename, name)) {
					if(name != NULL && dirNamePrinted == 0 && strcasestr(filename, name)) {
						printCurrentDir(dir, NULL);
						dirNamePrinted = 1;
					}
					printFile(dir, filename);
				}
			}
		}
	}
	free(pDirEnt);
}

void printSeekBar(){
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
