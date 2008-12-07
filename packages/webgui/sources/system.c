#include <stdio.h>
#include <errno.h>
#include "command.h"
#include "language_en.h"
#include <stdlib.h>
#include <string.h>
#define MAX 512

/* toDo
+ write getUptime function
+ /usr/bin/tvswitch - not tested (need nvidia tv-card!
*/

void printGeexboxControls() {
	char data[255];
	FILE *ptr;

	ptr = fopen("/proc/asound/pcm","r");
	if (!ptr)
		return;

	printf("\t\t<div id=\"geexboxControls\">\n");
	printf("\t\t\t<a id=\"geexboxControlsTvSwitch\" href=\"?system=/usr/bin/tvswitch\"><img src=\"/style/geexbox/icons/tv.png\" title=\"%s\" alt=\"TV-out Switch\" /></a>\n", "Switch TV-Out");
	printf("\t\t\t<a id=\"geexboxControlsReboot\" href=\"?system=reboot\"><img src=\"/style/geexbox/icons/reboot.png\" title=\"%s\" alt=\"Reboot\" /></a>\n", REBOOT);
	printf("\t\t\t<a id=\"geexboxControlsShutdown\" href=\"?system=poweroff\"><img src=\"/style/geexbox/icons/shutdown.png\" title=\"%s\" alt=\"Shutdown\" /></a>\n", POWEROFF);
	printf("\t\t</div>\n");

	printf("<h3>/proc/asound/pcm</h3>");
	printf("<p>");
	while (feof(ptr) == 0) {
		fgets(data, 255, ptr);
		printf("%s<br />", data);
	}
	printf("</p>");
	fclose(ptr);
}

void printPlayerState() {
	char data[MAX], *x;
	FILE *ptr;

	ptr = fopen("/tmp/mp_streaminfo","r");
	if (!ptr)
		return;

	fgets(data, MAX, ptr);

	x = strchr(data, '=')+1;

	printf("%s", x);
	fclose(ptr);
}
