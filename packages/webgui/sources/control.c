#include <stdlib.h>
#include <stdio.h>

/*
+ volume control on SPDIF ?
*/

void printMplayerMenuControl() {
	printf("GeexBox Menu");
	printf("<table>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=menu hide\"><img src=\"/img/big/close.png\" title=\"%s\"></a></td>\n", "Hide menu");
	printf("<td><a href=\"?mplayer=menu up\"><img src=\"/img/big/up.png\" title=\"%s\"></a></td>\n", "Menu up");
	printf("<td>&nbsp;</td>");
	printf("</tr>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=menu cancel\"><img src=\"/img/big/left.png\" title=\"%s\"></a></td>\n", "Menu left");
	printf("<td><a href=\"?mplayer=menu ok\"><img src=\"/img/big/ok.png\" title=\"%s\"></a></td>\n", "Menu OK");
	printf("<td><a href=\"?mplayer=menu ok\"><img src=\"/img/big/right.png\" title=\"%s\"></a></td>\n", "Menu right");
	printf("</tr>");
	printf("<tr>");
	printf("<td>&nbsp;</td>");
	printf("<td><a href=\"?mplayer=menu down\"><img src=\"/img/big/down.png\" title=\"%s\"></a></td>\n", "Menu down");
	printf("<td>&nbsp;</td>");
	printf("</tr>");
	printf("</table>");
}

void printDVDMenuControl() {
	printf("DVD Menu");
	printf("<table>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=dvdnav 5\"><img src=\"/img/big/menu.png\" title=\"%s\"></a></td>\n", "View menu");
	printf("<td><a href=\"?mplayer=dvdnav 1\"><img src=\"/img/big/up.png\" title=\"%s\"></a></td>\n", "Up");
	printf("<td>&nbsp;</td>");
	printf("</tr>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=dvdnav 3\"><img src=\"/img/big/left.png\" title=\"%s\"></a></td>\n", "Left");
	printf("<td><a href=\"?mplayer=dvdnav 6\"><img src=\"/img/big/ok.png\" title=\"%s\"></a></td>\n", "OK");
	printf("<td><a href=\"?mplayer=dvdnav 4\"><img src=\"/img/big/right.png\" title=\"%s\"></a></td>\n", "Right");
	printf("</tr>");
	printf("<tr>");
	printf("<td>&nbsp;</td>");
	printf("<td><a href=\"?mplayer=dvdnav 2\"><img src=\"/img/big/down.png\" title=\"%s\"></a></td>\n", "Down");
	printf("<td>&nbsp;</td>");
	printf("</tr>");
	printf("</table>");
}

void printMplayerVolumeControl() {
	printf("Volume");
	printf("<table>");
	printf("<tr>");
	printf("<td><a href=\"?mplayer=volume -10\"><img src=\"/img/remove.png\" title=\"%s\"></a></td>\n", "Volume -10%");
	printf("<td><a href=\"?mplayer=mute\"><img src=\"/img/big/volume_mute.png\" title=\"%s\"></a></td>\n", "Mute");
	printf("<td><a href=\"?mplayer=volume +10\"><img src=\"/img/add.png\" title=\"%s\"></a></td>\n", "Volume +10%");
	printf("</tr>");
	printf("</table>");	
}
