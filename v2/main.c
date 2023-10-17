#include <stdlib.h>
#include <stdio.h>
#include <main.h>

FILE *source;

int main(int argc, char *argv[])
{
	if (argc > 1){
	   source = fopen(argv[1], "r");
	   if (source == NULL) {
	       exit(0);	
	   }	
	} else {
	   source = stdin;
	}

	lookahead = gettoken(source);

	//E();

	printf("%d\n",  lookahead);
	exit(0);
}
