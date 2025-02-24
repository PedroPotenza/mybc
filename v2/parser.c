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
 * F -> ID | DEC | ( E ) */

/* E -> T R */
void E(void)
{
	T(); R();
}

/* R -> + T R | - T R | <epsilon> */
void R(void)
{
    switch (lookahead)
    {
	case '+':
	   match('+'); T(); R();
	break;

	case '-':
	   match('-'); T(); R();
	break;

	default:
	   ;
    }
}

/* T -> F Q */
void T(void)
{
	F(); Q();
}

/* Q -> * F Q | / F Q | <epsilon> */
void Q(void)
{
    switch (lookahead)
    {
	case '*':
	   match('*'); F(); Q();
        break;

	case '/':
	   match('/'); F(); Q();
        break;

	default:
	   ;
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
		fprintf(stderr, "token mismatch... exiting with error\n");
		exit(-2);
	}
}


