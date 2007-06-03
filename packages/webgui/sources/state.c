#include <stdio.h>
#include <stdlib.h>
#include "system.h"


int main(int argc, char *argv[]) {
	sleep(0.25);
	
	printf("Content-type: text/html\n\n") ;

	printf("<html>\n") ;
	printf("<head>\n");
	printf("	<meta http-equiv=\"refresh\" content=\"30\">");
	printf("	<link rel=\"stylesheet\" type=\"text/css\" href=\"/webgui.css\">");
	printf("</head>\n");
	printf("<body>\n") ;
	printPlayerState();
	printf("</body>\n") ;
	printf("</html>\n") ;
	
	return EXIT_SUCCESS;
}
