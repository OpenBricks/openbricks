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

/* toDo
+ error on add file to list, reload mplayer ?
+ fix error when play´s files whose name includes ' ( ) & #
+ sort directory content
+ can not get dir atribute´s if dir contents + char
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
	
	while(c) {
		p = strcat(path, "/");
		p = strcat(path, c);
		printf(" / <a href=\"?dir=%s\">%s</a> \n", p, c);

		c = strtok(NULL,"/");
	}
	
	if(search != NULL) {
		printf(" / search for '%s'<br>", search);
	} else {
		printf("<a href=\"?play_dir=%s\"><img src=\"/img/play.png\" title=\"%s\"></a>\n", p, PLAY_DIR);
	}
	printf("<br>\n");
}

const char *printFileIcon(const char *file) {
	if(strcasestr(file, ".mp3") || strcasestr(file, ".wav")) {
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
	
	printf("<img src=\"/img/%s\">&nbsp;", printFileIcon(file));
	printf("%s\n", file);
	// error  // printf("<a href=\"?system=echo '%s/%s' >> /tmp/playlist.pls\"><img src=\"/img/add.png\" title=\"%s\"></a>\n", dir, file, "Add to playlist");
	printf("<a href=\"?%s%s/%s'\"><img src=\"/img/play.png\" title=\"%s\"></a>\n", cmd, dir, file, PLAY_FILE);
	printf("<br>\n");
}

void printDir(const char *dir, const char *file) {
	printf("<img src=\"/img/%s\">&nbsp;", printDirIcon(file));
	printf("<a href=\"?dir=%s/%s\">%s</a>\n", dir, file, file);
	printf("<a href=\"?play_dir=%s/%s\"><img src=\"/img/play.png\" title=\"%s\"></a>\n", dir, file, PLAY_DIR);
	printf("<br>\n");
}

void printCurrentDirContent(const char *dir, const char *name) {
	int dirNamePrinted = 0;
// old //	DIR *pDIR = opendir(dir);
// old //	if(pDIR == NULL) {
// old //		fprintf(stderr, "%s %d: opendir() failed (%s)\n", __FILE__, __LINE__, strerror(errno));
// old //	}
// old //	struct dirent *pDirEnt = readdir(pDIR);
	
	struct dirent** pDirEnt;
        int n = scandir(dir, &pDirEnt, 0, alphasort);
	int i;
        for(i = 0; i < n; i++) {
// old //	while(pDirEnt != NULL) {
// old //		char *filename = pDirEnt->d_name;
		char *filename = pDirEnt[i]->d_name;
		if(strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
			char fullpath[MAX] = "";
			strcat(fullpath, dir);
			strcat(fullpath, "/");
			strcat(fullpath, filename);

			struct stat attributs;
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
// old //	pDirEnt = readdir(pDIR);
	}
// old //	closedir(pDIR);
	free(pDirEnt);
}

void printPlayerControls(const char *play_file, const char *play_dir) {
	printf("<table width=\"100%%\">");
	printf("<tr>");
	
	/* seek */
	printf("<td>");
	printf("<table cellspacing=0 cellpadding=0>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=pt_step -1\" accesskey=\"p\"><img src=\"/img/big/left.png\" title=\"%s\"></a></td>\n", PLAY_PREVIOUS);
	//printf("<td><a href=\"?mplayer=seek -30\"><img src=\"/img/big/back.png\" title=\"%s\"></a>&nbsp;</td>\n", SEEK_BW);
	int i;
	for(i = 0; i <= 100; i += 2) {
		printf("<td align=center bgcolor=#EEEEEE><a href=\"?mplayer=seek %d 1\">", i);
		switch(i) {
			case 0:
				printf("[%d", i);
				break;
			case 100:
				printf("%d]", i);
				break;
			case 50:
				printf("%d", i);
				break;
			case 10:
			case 24:
			case 74:
			case 36:
			case 64:
			case 88:
				break;
			case 12:
			case 26:
			case 76:
			case 38:
			case 62:
			case 86:
				printf(":");
				break;
			default:
				printf(".");
		}
		printf("</a></td>");
	}
	//printf("<td>&nbsp;<a href=\"?mplayer=seek +30\"><img src=\"/img/big/forward.png\" title=\"%s\"></a></td>\n", SEEK_FW);
	printf("<td><a href=\"?mplayer=pt_step +1\" accesskey=\"n\"><img src=\"/img/big/right.png\" title=\"%s\"></a></td>\n", PLAY_NEXT);
	printf("<td>&nbsp;</td>");
	// printf("<td><a href=\"?mplayer=stop\"><img src=\"/img/big/stop.png\" title=\"%s\"></a></td>\n", STOP);
	printf("<td><a href=\"?mplayer=pause\"><img src=\"/img/big/pause.png\" title=\"%s\"></a></td>\n", PAUSE);
	printf("</tr>");
	printf("</table>");
	printf("</td>");
	
	/* player controls */
	//printf("<td>");
	//printf("&nbsp;");


	//printf("&nbsp;");
	//printf("</td>");
	
	/* print current command (state) */
	//printf("<td>");
	//printPlayerState();
	//printf("</td>");
	
	/* print search */
	printf("<td align=\"right\">");
	printf("<form method=\"get\">");
	//printf("<img src=\"/img/big/search.png\">");
	printf("<input type=\"image\" src=\"/img/big/search.png\" alt=\"%s\" title=\"%s\">\n", "Search", "Search");
	//printf("<input type=\"submit\" value=\"GO\">");
	printf("<input type=\"text\" name=\"search\" value=\"\">");
	printf("</form>");
	printf("</td>");
	
	printf("</tr>");
	printf("</table>");
}
