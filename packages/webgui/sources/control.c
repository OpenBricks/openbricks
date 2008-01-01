#include <stdlib.h>
#include <stdio.h>

/*
+ volume control on SPDIF ?
*/

void printGeexboxWebmote(){
	printf("\t\t<div id=\"webmoteControl\">\n");
	printf("\t\t\t<a href=\"?mplayer=play\"><img id=\"webmoteControlButtonPlay\" src=\"/style/geexbox/webmote/playButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=pause\"><img id=\"webmoteControlButtonPause\" src=\"/style/geexbox/webmote/pauseButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=quit\"><img id=\"webmoteControlButtonStop\" src=\"/style/geexbox/webmote/stopButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu toggle\"><img id=\"webmoteControlButtonMenu\" src=\"/style/geexbox/webmote/menuButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=pt_step -1\"><img id=\"webmoteControlButtonPreviousTrack\" src=\"/style/geexbox/webmote/previousTrackButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=pt_step +1\"><img id=\"webmoteControlButtonNextTrack\" src=\"/style/geexbox/webmote/nextTrackButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=volume -10\"><img id=\"webmoteControlButtonVolumeDecrease\" src=\"/style/geexbox/webmote/volumeDecreaseButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=volume +10\"><img id=\"webmoteControlButtonVolumeIncrease\" src=\"/style/geexbox/webmote/volumeIncreaseButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=mute\"><img id=\"webmoteControlButtonMute\" src=\"/style/geexbox/webmote/muteButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu up\"><img id=\"webmoteControlButtonMenuUp\" src=\"/style/geexbox/webmote/menuButtonUp.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu down\"><img id=\"webmoteControlButtonMenuDown\" src=\"/style/geexbox/webmote/menuButtonDown.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu cancel\"><img id=\"webmoteControlButtonMenuLeft\" src=\"/style/geexbox/webmote/menuButtonLeft.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu ok\"><img id=\"webmoteControlButtonMenuRight\" src=\"/style/geexbox/webmote/menuButtonRight.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=menu ok\"><img id=\"webmoteControlButtonMenuSelect\" src=\"/style/geexbox/webmote/menuButtonSelect.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=seek -10\"><img id=\"webmoteControlButtonSeekBack\" src=\"/style/geexbox/webmote/seekBackwardButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=seek -60\"><img id=\"webmoteControlButtonSeekBackFast\" src=\"/style/geexbox/webmote/seekFastBackwardButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=seek +60\"><img id=\"webmoteControlButtonSeekFastForward\" src=\"/style/geexbox/webmote/seekFastForwardButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=seek +10\"><img id=\"webmoteControlButtonSeekForward\" src=\"/style/geexbox/webmote/seekForwardButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 1\"><img id=\"webmoteControlButtonDvdnavUp\" src=\"/style/geexbox/webmote/menuButtonUp.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 2\"><img id=\"webmoteControlButtonDvdnavDown\" src=\"/style/geexbox/webmote/menuButtonDown.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 3\"><img id=\"webmoteControlButtonDvdnavLeft\" src=\"/style/geexbox/webmote/menuButtonLeft.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 4\"><img id=\"webmoteControlButtonDvdnavRight\" src=\"/style/geexbox/webmote/menuButtonRight.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 5\"><img id=\"webmoteControlButtonDvdnavSelect\" src=\"/style/geexbox/webmote/menuButtonSelect.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=dvdnav 6\"><img id=\"webmoteControlButtonDvdnavMenu\" src=\"/style/geexbox/webmote/menuButtonDvd.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("\t\t\t<a href=\"?mplayer=set_menu aspect\"><img id=\"webmoteControlAspectRatioButton\" src=\"/style/geexbox/webmote/aspectRatioButton.png\" alt=\"GeeXboX Webmote Control\" /></a>\n");
	printf("<img id=\"webmoteControlMarkerDvd\" src=\"/style/geexbox/webmote/dvdMarker.png\" alt=\"GeeXboX Webmote Control\" />\n");
	printf("\t\t</div>\n");
}
