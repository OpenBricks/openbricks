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
#define VERSION "0.00.1"

/* toDo 
+ add to list function
+ fix printControls function to css
+ fix html to xhtml
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
	printf("<table width=\"100%%\">");
	printf("<tr>");
	printf("<td> | </td>");
	
	printf("<td>");
	if(strcmp(l1, "player")) {
		printf("<a href=\"/cgi-bin/index.cgi?l1=player\">PLAYER</a>");
	} else {
		printf("PLAYER");
	}
	printf("</td>");
	
	printf("<td> | </td>");
	
	printf("<td>");
	if(strcmp(l1, "system")) {
		printf("<a href=\"/cgi-bin/index.cgi?l1=system\">SYSTEM</a>");
	} else {
		printf("SYSTEM");
	}
	printf("</td>");
	
	printf("<td> | </td>");
	
	printf("<td align=\"center\" width=\"99%%\">");
	printf("<iframe src=\"state.cgi\" name=\"state\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" frameborder=\"0\" width=\"700\" height=\"18\" align=\"center\"></iframe>");
	printf("</td>");
	
	printf("</tr>");
	printf("</table>");
	printf("<hr>\n");
}

void printMenu2system(const char *l2) {
}

void printMenu2player(const char *l2) {
	printf("<table>");
	printf("<tr>");
	printf("<td> | </td>");
	
	printf("<td>");
	if(strcmp(l2, "player")) {
		printf("<a href=\"/cgi-bin/index.cgi?l2=browser\">BROWSER</a>");
	} else {
		printf("BROWSER");
	}
	printf("</td>");
	
	printf("<td> | </td>");
	
	printf("<td>");
	if(strcmp(l2, "control")) {
		printf("<a href=\"/cgi-bin/index.cgi?l2=control\">CONTROL</a>");
	} else {
		printf("CONTROL");
	}
	printf("</td>");
	printf("<td> | </td>");
	printf("</tr>");
	printf("</table>");
	printf("<hr>\n");
}


void printMenu2(const char *l2) {
	if(!strcmp(l2, "system")) {
		printMenu2system(l2);
		printPageSystem();
	} else if(!strcmp(l2, "control")) {
		printMenu2player(l2);
		printf("<table width=\"100%%\">");
		printf("<tr>");
		printf("<td>");
		printMplayerVolumeControl();
		printf("</td>");
		printf("<td>");
		printMplayerMenuControl();
		printf("</td>");
		printf("<td>");
		printDVDMenuControl();
		printf("</td>");
		printf("</tr>");
		printf("</table>");
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
	printf("Content-type: text/html\n\n") ;

	printf("<html>\n") ;
	printf("<head>\n");
	printf("	<title>GeexBox Webgui %s</title>\n", VERSION);
	printf("	<link rel=\"stylesheet\" type=\"text/css\" href=\"/webgui.css\">");
	printf("</head>\n");
	printf("<body>\n") ;
}

void printPageFoot() {
	printf("</body>\n") ;
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
