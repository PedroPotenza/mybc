#include <stdlib.h>
#include <stdio.h>
#include <main.h>

FILE *source;

int main(int argc, char *argv[])
{
	if(argc > 1) {
	    source = fopen(argv[1], "r");
	    if(source == NULL){
		fprintf(stderr, "the file must exist");
		exit(-1);
	    }
	} else {
	    source = stdin;
	}

	lookahead = gettoken(source);

	E();

	if(argc > 1) { 
	    fclose(source);
	}

	exit(0);
}
