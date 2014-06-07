#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ffos.h"

int main(int argc, char **argv) {
	if (argc<2) {
		help();
		return (EXIT_FAILURE);
	}
	
	char *cmd = argv[1];
	
	if (!strcmp(cmd,"create")) {
		create(argc,argv);
	}
	
	else if(!strcmp(cmd,"add")) {
		add(argc,argv);
	}
	
	else if(!strcmp(cmd,"set")) {
		set(argc,argv);
	}
	
	else help();
	
	return (EXIT_SUCCESS);
}

