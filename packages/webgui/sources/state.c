#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include <unistd.h>


int main(int argc, char *argv[]) {
	sleep(0.25);

	printf("Content-type: text/html\n") ;
	printf("Expires: Wed, 11 Jan 1984 05:00:00 GMT\n");
	printf("Cache-Control: no-cache, must-revalidate, max-age=0\n");
	printf("Pragma: no-cache\n\n");

	printPlayerState();
	return EXIT_SUCCESS;
}
