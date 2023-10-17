#include <stdio.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

/* ID = [A-Za-z][A-Za-z0-9]* */
int isID(FILE *tape)
{
	int head;

	head = getc(tape);

	if (isalpha(head)) {
		while ( isalnum(head = getc(tape)) );
		ungetc(head, tape);
		return ID;
	}

	ungetc(head, tape);
	return 0;
}

/* DEC = [1-9][0-9]* | 0 */
int isDEC(FILE *tape)
{
	int head;

	head = getc(tape);

	if (isdigit(head)) {
		if (head == '0') {
			return DEC;
		}
		while (isdigit(head = getc(tape)));
		ungetc(head, tape);
		return DEC;
	}

	ungetc(head, tape);
	return 0;
}

int isOCT(FILE *tape)
{
	//le o primeiro
	int head = getc(tape);

	//se tiver lido 0, ele vai continua
	if(head == '0') {
		//le o segundo
		head = getc(tape);
		//valida se o que leu é OCT
		if(head < '0' || head > '7') {
			ungetc(head, tape); //se nao for, retorna o que leu
			ungetc('0', tape); //retorna o 0 lido anteriormente
			return 0;
		}
		//desse ponto para frente temos a certeza que o numero lido eh OCT
		do {
			head = getc(tape);
		} while(head >= '0' && head <= '7');
		ungetc(head, tape); //apos ler algo que nao seja OCT, retorna o valor lido
		return OCT; //o que deveria retornar?? #define OCT ?
	}
	//se nao tiver lido 0, retorna o valor lido
	ungetc(head, tape);
	return 0;
}

// 0[x|X][a-fA-F0-9]+
int isHEX(FILE *tape) {
	
	//recebe o primeiro
	int head = getc(tape);

	//se ele for diferente de 0, significa que nao eh hex
	if(head != '0') {
		ungetc(head, tape); //retorna o valor lido
		return 0;
	}

	//recebe o segundo
	head = getc(tape);

	//se nao eh x e nem X, significa que nao eh hex
	if(head != 'x' && head != 'X') {
		ungetc(head, tape); //retorna o valor lido 
		ungetc('0', tape); //retorna o 0 lido anteriomente
		return 0;
	}
	
	int headHEX = getc(tape); //foi necessario outra variavel para nao perder o valor anterior

	if(isxdigit(headHEX) == 0) { //verifica se é a-f, A-F, 0-9
		ungetc(headHEX, tape); //retorna o valor lido
		ungetc(head, tape); // retorna o x ou o X
		ungetc('0', tape); // retorna o 0
		return 0;
	}

	//desse ponto para frente, temos a certeza que é um valor HEX
	while(isxdigit(headHEX)) {
		headHEX = getc(tape);
	}
	ungetc(headHEX, tape); //apos ler algo que nao seja HEX, retorna o valor lido
	return HEX;
}

int isEE(FILE *tape){

	int head = getc(tape);

	//verifico se a primeira coisa é um 'e' ou 'E'
	if(head != 'e' && head != 'E') { //se nao veio, retorno o que eu li
		ungetc(head, tape);
		return 0;
	}

	int signal = getc(tape); //leio o sinal ou o valor da exponencial direto

	if(signal == '+' || signal =='-') { //se era sinal 

		int numberExp = getc(tape); //recebo os numeros da exponencial
 
		if(!isdigit(numberExp)) { //se nao veio nenhum numero, exemplo: e+T ou E-. 
			ungetc(numberExp, tape); //devolvo o que li
			ungetc(signal, tape); //devolvo o sinal
			ungetc(head, tape); //devolvo o e ou E
			return 0;
		}

		while (isdigit(head = getc(tape))); //se veio 1 digito no minimo, recebo quantos digitos quiser
		ungetc(head, tape);//apos sair do while significa q ele leu algo q n era digito, logo devolve o que leu
		return 1;

	} else { //se nao veio sinal 
		if(!isdigit(signal)) { //verifico se o que foi lido era um digito
			
			ungetc(signal, tape); //se nao era, devolvo o que li
			ungetc(head, tape); //devolvo o e ou E
			return 0;
		}
		while (isdigit(head = getc(tape))); //se veio 1 digito no minimo, recebo quantos digitos quiser
		ungetc(head, tape);//apos sair do while significa q ele leu algo q n era digito, logo devolve o que leu
		return 1;
	}
}

int isFLT(FILE *tape){

	int head;

	//Verifica toda a parte decimal
	if(isDEC(tape)) {

		head = getc(tape); //le o que vem em seguida, pode ser um '.' ou um [eE]
		
		//verifico primeiro o ponto e depois verifico o eE
		if(head == '.') {

			while (isdigit(head = getc(tape))); // leio um ou mais decimais

			ungetc(head,tape);
			
			if(isEE(tape)) {
				return FLT;
			} else {
				ungetc(head, tape);
				return FLT; //mesmo sem o eE ele tendo '.' torna ele um numero float
			}

		} else {
			//se ele nao possui '.' entao verifico se ele é Float ou se ele é Decimal
			ungetc(head,tape);
			if(isEE(tape)){ //se tem notacao exponencial, promovo para float
				return FLT;
			} else {
				return DEC;
			}
		}

	} else {
		head = getc(tape); //le o que vem em seguida, TEM que ser um .
		if(head == '.') {

			if(!isdigit(head = getc(tape))) { //se ele ler ponto e depois não ter um digito retorna erro
				return 0; //ERROR --> perguntar para o professor o que deve acontecer.
			}

			//se tiver pelo menos um digito apos o ponto
			while (isdigit(head = getc(tape))); // leio um ou mais decimais
			
			ungetc(head, tape);

			if(isEE(tape)) {
				return FLT;
			} else {
				ungetc(head, tape);
				return FLT;
			}
		} else {
			//se ele nao possui '.'
			ungetc(head,tape);
			return 0;
		}
	}
}

void skipspaces(FILE *tape) //e comentarios
{
	int head;

	ISSPACE:
	//Retira todos os espaços
	while ( isspace( head = getc(tape) ) );

	// Verifica um ou mais comentarios em sequencia
	if (head == '{'){
		while ( (head = getc(tape)) != '}' && head != EOF);

		goto ISSPACE;
	}
	
	//Devolve o que leu
	ungetc(head, tape);
}

int gettoken(FILE *source)
{
	int token;

	skipspaces(source);

	if ( (token = isID(source)) ) return token;

	if ( (token = isOCT(source)) ) return token;

	if ( (token = isHEX(source)) ) return token;

	//dentro do isFLT ele retorna DEC se for DEC
	if ( (token = isFLT(source)) ) return token; 

	token = getc(source);

	return token;
}

