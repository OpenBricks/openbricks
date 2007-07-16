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
	printf("<table width=\"100%%\">");
	printf("<tr>");
	printf("<td>");
	printf("	<a href=\"?system=/usr/bin/tvswitch\"><img src=\"/img/big/tv.png\" title=\"%s\"></a>\n", "Switch TV-Out");
	printf("</td>");
	printf("<td align=\"right\">");
	// test // printf("	<a href=\"?system=/mnt/nfs/video/cpWebgui\"><img src=\"/img/big/update.png\" title=\"%s\"></a>\n", "Update");
	// test // printf("	&nbsp;");
	printf("	<a href=\"?system=reboot\"><img src=\"/img/big/reboot.png\" title=\"%s\"></a>\n", REBOOT);
	printf("	&nbsp;");
	printf("	<a href=\"?system=poweroff\"><img src=\"/img/big/off.png\" title=\"%s\"></a>\n", POWEROFF);
	printf("</td>");
	printf("</table>");
	printf("<br>");
	
	printf("<h3>/proc/asound/pcm</h3>");

	FILE *ptr = fopen("/proc/asound/pcm","r");
	char data[255];
	while (feof(ptr) == 0) {
		fgets(data, 255, ptr);
		printf("%s<br>", data);
	}
	
	fclose(ptr);	
}

void printPlayerState() {
	char data[MAX];
	FILE *ptr;

	ptr = fopen("/tmp/mp_streaminfo","r");
	fgets(data, MAX, ptr);
	
	char* x = strchr(data, '=') + (1 * sizeof(char));

	printf("%s", x);

	fclose(ptr);	
}
