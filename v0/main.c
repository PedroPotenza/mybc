#include <stdlib.h>
#include <stdio.h>
#include <main.h>

FILE *source;

int main(void)
{
	source = stdin;

	lookahead = gettoken(source);

	E();

	exit(0);
}
