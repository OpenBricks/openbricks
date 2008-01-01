#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgi-util.h"
#include "command.h"
#include "system.h"
#include "browser.h"
#include "control.h"
#include "language_en.h"
#define MENU1_DIR "/tmp/webgui_menu_1"
#define MENU2_DIR "/tmp/webgui_menu_2"
#define VERSION "0.00.2"

/* toDo 
+ add to list function
+ fix printMenu1, at first time displays all links
+ change play_file to mplayer parameter (print state)
+ change play_dir to system paramete (print state)
*/

void printPageHead();
void printPageFoot();
void printMenu1(const char *);
void printMenu2(const char *);
void modSystem();
void modPlayer();
const char *getLevel1(const char *);
const char * getMenuLevel1();
void setMenuLevel1(const char *);
const char * getMenuLevel2();
void setMenuLevel2(const char *);
void printMenu2system();
void printMenu2player();
void printPageSystem();
void printPagePlayer();


int main(int argc, char *argv[]) {
	/* Initialize the CGI and send out an HTTP header: */
	int res = cgi_init();
	
	/* Was there an error initializing the CGI??? */
	if(res != CGIERR_NONE) {
		printf("Error # %d: %s<p>\n", res, cgi_strerror(res));
		exit(0);
	}
	
	/* get parameter */
	const char *cgi_level_1 = getLevel1(cgi_getentrystr("l1"));
	const char *cgi_level_2 = getLevel1(cgi_getentrystr("l2"));
	
	cmdMplayer(cgi_getentrystr("mplayer"));
	cmdSystem(cgi_getentrystr("system"));
	mplayerMenu(cgi_getentrystr("menu"));
	
	printPageHead();
	printMenu1(cgi_level_1);
	printMenu2(cgi_level_2);
	printPageFoot();
	
	/* Close up the CGI: */
	cgi_quit();
	
	return EXIT_SUCCESS;
}

const char *getLevel1(const char *cgi_param) {
	if(cgi_param != NULL) {
		setMenuLevel1(cgi_param);
		
		return cgi_param;
	} else {
		const char *l1 = getMenuLevel1();
		if(l1 != NULL) {
			return l1;
		} else {
			setMenuLevel1("player");
			return "player";
		}
	}
}

const char *getLevel2(const char *cgi_param) {
	if(cgi_param != NULL) {
		setMenuLevel2(cgi_param);
		
		return cgi_param;
	} else {
		const char *l2 = getMenuLevel2();
		if(l2 != NULL) {
			return l2;
		} else {
			setMenuLevel2("browser");
			return "browser";
		}
	}
}

void printMenu1(const char *l1) {
	printf("\t\t<div id=\"menuL1\">\n");
	if(strcmp(l1, "player")) {
		printf("\t\t\t<a class=\"menuL1Element\" href=\"/cgi-bin/index.cgi?l1=player\">PLAYER</a>\n");
	} else {
		printf("\t\t\t<a class=\"menuL1Element\" href=\"#nothing\">PLAYER</a>\n");
	}
	
	if(strcmp(l1, "system")) {
		printf("\t\t\t<a class=\"menuL1Element\" href=\"/cgi-bin/index.cgi?l1=system\">SYSTEM</a>\n");
	} else {
		printf("\t\t\t<a class=\"menuL1Element\" href=\"#nothing\">SYSTEM</a>\n");
	}
	printf("\t\t</div>\n");
}

void printMenu2system(const char *l2) {
}

void printMenu2player(const char *l2) {
	printf("\t\t<div id=\"menuL2\">\n");

	if(strcmp(l2, "player")) {
		printf("\t\t\t<a class=\"menuL2Element\" href=\"/cgi-bin/index.cgi?l2=browser\">BROWSER</a>\n");
	} else {
		printf("\t\t\t<a class=\"menuL2Element\" href=\"#nothing\">BROWSER</a>\n");
	}
	
	if(strcmp(l2, "control")) {
		printf("\t\t\t<a class=\"menuL2Element\" href=\"/cgi-bin/index.cgi?l2=control\">CONTROL</a>\n");
	} else {
		printf("\t\t\t<a class=\"menuL2Element\" href=\"#nothing\">CONTROL</a>\n");
	}

	printf("\t\t</div>\n");
}


void printMenu2(const char *l2) {
	if(!strcmp(l2, "system")) {
		printMenu2system(l2);
		printPageSystem();
	} else if(!strcmp(l2, "control")) {
		printMenu2player(l2);
		printGeexboxWebmote();
	} else {
		printMenu2player(l2);
		printPagePlayer();
	}
}

void printPageSystem() {
	printGeexboxControls();
}

void printPagePlayer() {
	const char *cgi_dir = getCurrentDir(cgi_getentrystr("dir"));		
	const char *cgi_play_file = cgi_getentrystr("play_file");
	const char *cgi_play_dir = cgi_getentrystr("play_dir");
	const char *cgi_search = cgi_getentrystr("search");

	printPlayerControls(cgi_play_file, cgi_play_dir);
	printCurrentDir(cgi_dir, cgi_search);
	printCurrentDirContent(cgi_dir, cgi_search);
	playFile(cgi_play_file);
	playDir(cgi_play_dir);
}

void printPageHead() {
	printf("Content-type: application/xhtml+xml\n\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
	printf("<html xmlns=\"http://www.w3.org/1999/xhtml\"\n");
     printf("      xml:lang=\"en\" >\n");
	printf("\t<head>\n");
	printf("\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	printf("\t\t<title>GeeXboX &#8250; WebGui %s</title>\n", VERSION);
	printf("\t\t<link rel=\"StyleSheet\" href=\"/style/geexbox/style.css\" type=\"text/css\" />\n");
	printf("\t\t<link rel=\"shortcut icon\" href=\"/style/geexbox/icon.png\" type=\"image/x-icon\" />\n");
	printf("\t\t<script type=\"text/javascript\" src=\"/js/state.js\"></script>\n");
	printf("\t</head>\n");
	printf("\t<body onload=\"getState();\">\n");
	printf("\t\t<div id=\"headPlayerState\"></div>\n");
	printf("\t\t<div id=\"head\"><img src=\"/style/geexbox/logo.png\" alt=\"GeeXboX WebGui\" /></div>\n");
}

void printPageFoot() {
	printf("\t</body>\n") ;
	printf("</html>\n") ;
}

const char * getMenuLevel1() {
	return getFileContent(MENU1_DIR);
}

void setMenuLevel1(const char *data) {
	setFileContent(MENU1_DIR, data);
}

const char * getMenuLevel2() {
	return getFileContent(MENU2_DIR);
}

void setMenuLevel2(const char *data) {
	setFileContent(MENU2_DIR, data);
}
