#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>

int lookahead;

/* LR(1) expression grammar:
 * Initial symbol: E
 *
 * E -> E + T | E - T | T
 * T -> T * F | T / F | F
 * F -> ID | DEC | ( E )
 *
 * LL(1) grammar
 *
 * E -> T R
 * R -> + T R | - T R | <epsilon>
 * T -> F Q
 * Q -> * F Q | / F Q | <epsilon>
 * F -> ID | DEC | ( E ) 
 *
 * LL(1) grammar extended normal form
 * E -> T { + T | - T }
 * T -> F { * F | / F }
 * F -> (E) | ID | NUM 
 */

/* E -> [+-] T { +- T } */
void E(void)
{

	//verifica o sinal
	switch (lookahead)
	{
		//caso for positivo, match no sinal de mais
		case '+': 
			match('+');
		break;

        //caso for negativo, match no sinal de menos
		case '-':
			match('-');
		break;

		// como ele é opcional, caso não for um dos dois sinais ele continua
		default:
			;
	}

	T();

	// Verifica enquanto vier os sinais de + e - 
	while(lookahead == '+' || lookahead == '-') {
		match(lookahead); T(); 
	}
	
}

// T -> F { */ F }
void T(void)
{
	F(); 

    // Verifica enquanto vier os sinais de * e / 
	while(lookahead == '*' || lookahead == '/') {
		match(lookahead); F(); 	
	}
}

/* F -> ID | DEC | ( E ) */
void F(void)
{
    switch(lookahead)
    {
		case ID:
			match(ID);
		break;

		case DEC:
			match(DEC);
		break;

		case OCT:
			match(OCT);
		break;

		case HEX:
			match(HEX);
		break;

		case FLT:
			match(FLT);
		break;

		default:
			match('('); E(); match(')');
    }
}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		if(lookahead == EOF) {
			fprintf(stderr, "premature EOF found\n");
			exit(-3);
		} else {
			fprintf(stderr, "token mismatch... exiting with error\n");
			exit(-2);
		}
	}
}


