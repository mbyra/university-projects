/*
DEKODER MASZYNY NOC

12.2015
Marcin Byra
mb347056@okwf.fuw.edu.pl
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ROZMIAR_NOC 256

/* Dla wygody rejestry maszyny umieszczone w pamieci, bezposrednio za
instrukcjami */
#define AC 256
#define SP 257
#define IP 258

#define MAX_ROZM_KODU 10000

/* Aby użyć niekończącej się pętli */
#define TRUE 1


/* Wypelnia tablice pamiec[] zerami */
void zeruj_pamiec(int pamiec[])
{
	int i;
	for (i = 0; i < ROZMIAR_NOC + 3; ++i)
	{
		pamiec[i] = 0;
	}
}

/* Sprowadzenie rezultatu wykonania operacji do przedzialu 0 - ROZMIAR_NOC-1 */
int mod(int W)
{
	return (W % ROZMIAR_NOC < 0) ? (W % ROZMIAR_NOC + ROZMIAR_NOC) :
	(W % ROZMIAR_NOC);
}

/* Z podanej wartosci z przedzialu 0-255 odczytuje numer trybu adresowania */
int czytaj_tryb(int W)
{
	return mod(W) % 8;
}

/* Z podanej wartosci z przedzialu 0-255 odczytuje kod operacji */
int czytaj_operacja(int W)
{
	return (W - czytaj_tryb(W)) / 8;
}

/* Do sprawdzenia adresowania w operacjach, w których nie można używać acc */
void czy_nie_acc(int tryb)
{
	if (tryb == 0){
		exit(EXIT_SUCCESS);
	}
}

/* Wczytuje program dany na wejscie w konsoli w postaci liczb */
void wczytaj_program(char* argv[], int pamiec[], int *ilosc_slow)
{
	/*int i;*/
	int dlugoscargv; /* dlugosc argv[1], czyli programu podawanego na wejscie */
	int znaki = 0; /* mowi, o ile przesunac poczatek stringa w sscanfie */
	int ilewczytalo = 0; /* mowi, ile dodac do zmiennej znaki */
	*ilosc_slow = 0;
	dlugoscargv=strlen(argv[1]);

	while(znaki < dlugoscargv){
		sscanf(argv[1] + znaki, "%d%n", &pamiec[(*ilosc_slow)++], &ilewczytalo);
		znaki += ilewczytalo;
		if (pamiec[*ilosc_slow] == ';')
		{
			(*ilosc_slow)--;
		}
	}
	/*
	for(i = 0; i < *ilosc_slow; i++){
		printf("pamiec[%d] = %d\n", i, pamiec[i]);
	}
	*/
}

/* Stosuje odpowiednie adresowanie dla zadanego trybu */
void wczytaj_do_operandu(int argument, int tryb, int *operand, int pamiec[])
{
	switch ( tryb )
	{
		case 0: /* @  -  acc  -  accumulator */
			*operand = AC;
			break;
		case 1: /* ^  -  ind  -  indirect */
			*operand = pamiec[AC];
			break;
		case 2: /* }  -  pop  -  postincrement (pop) */
			*operand = pamiec[SP];
			(pamiec[SP])++;
			pamiec[SP] = mod(pamiec[SP]);
			break;
		case 3: /* {  -  psh  -  predecrement (push) */
			(pamiec[SP])--;
			pamiec[SP] = mod(pamiec[SP]);
			*operand = pamiec[SP];
			break;
		case 4: /* #  -  imm  -  immediate */
			*operand = argument;
			(pamiec[IP])++;
			break;
		case 5: /*       abs  -  absolute */
			*operand = pamiec[argument];
			(pamiec[IP])++;
			break;
		case 6: /* $  -  dis  - displacement */
			*operand = pamiec[argument] + pamiec[SP];
			(pamiec[IP])++;
			break;
		case 7: /* &  -  rel  -  relative */
			*operand = pamiec[argument] + mod(argument + 1);
			(pamiec[IP])++;
			break;
	}
	(pamiec[IP])++;
}

/* Wykonuje instrukcje dla zadanej operacji */
void stosuj_operacja(int tryb, int operacja, int *operand, int pamiec[])
{
	switch ( operacja ) {
		case 0: /* \_  -  JSR  -  jump to subroutine */
			czy_nie_acc(tryb);
			pamiec[AC] = pamiec[IP];
			pamiec[IP] = *operand;
			break;
		case 1: /* _  -  JMP  -  jump */
			czy_nie_acc(tryb);
			pamiec[IP] = *operand;
			break;
		case 2: /* ~  -  JPZ  -  jump if AC is zero */
			czy_nie_acc(tryb);
			if ( pamiec[AC] == 0 ){
				pamiec[IP] = *operand;
			}
			break;
		case 3: /* \~  -  JNZ  -  jump if AC is not zero */
			czy_nie_acc(tryb);
			if ( (pamiec[AC]) != 0 ){
				pamiec[IP] = *operand;
			}
			break;
		case 4: /* |  -  NND  -  nand */
			pamiec[AC] = ( pamiec[AC] == 1 && pamiec[*operand] == 1 ) ? 0 : 1;
			break;
		case 5: /* \|  -  DNN  -  reverse nand */
			pamiec[*operand] = ( pamiec[*operand] == 1 && pamiec[AC] == 1 ) ? 0 : 1;
			break;
		case 6: /* \}  -  INC  -  increment */
			(pamiec[*operand])++;
			break;
		case 7: /* \{  -  DEC  -  decrement */
			(pamiec[*operand])--;
			break;
		case 8: /* \+  -  DDA  -  reverse add */
		  pamiec[*operand] += pamiec[AC];
			break;
		case 9: /* \-  -  BUS  -  reverse substract */
			pamiec[*operand] -= pamiec[AC];
			break;
		case 10: /* \*  -  LUM  -  reverse multiply */
			pamiec[*operand] *= (pamiec[AC]);
			break;
		case 11: /* \/  -  VID  -  reverse divide */
			if ( (pamiec[AC]) == 0 ){
				exit(EXIT_FAILURE);
			}
			pamiec[*operand] /= pamiec[AC];
			break;
		case 12: /* \%  -  DOM  -  reverse modulo */
			if ( (pamiec[AC]) == 0 ){
				exit(EXIT_FAILURE);
			}
			pamiec[*operand] = pamiec[*operand] % pamiec[AC];
			break;
		case 13: /* \=  -  SNE  -  skip if not equal */
			if ( (pamiec[AC]) != pamiec[*operand] ) {
				(pamiec[IP]) += 2;
			}
			break;
		case 14: /* \<  -  SGE  - skip if greater or equal */
			if ( (pamiec[AC]) >= pamiec[*operand] ){
				(pamiec[IP]) += 2;
			}
			break;
		case 15: /* \>  -  SLE  -  skip if less or equal */
		if ( (pamiec[AC]) <= pamiec[*operand] ){
			(pamiec[IP]) += 2;
		}
		break;
		case 16: /* +  -  ADD  -  add */
			pamiec[AC] += pamiec[*operand];
			break;
		case 17: /* -  -  SUB  -  substract */
			pamiec[AC] -= pamiec[*operand];
			break;
		case 18: /* *  -  MUL  -  multiply */
			pamiec[AC] *= pamiec[*operand];
			break;
		case 19: /* /  -  DIV  -  divide */
			if ( pamiec[*operand] == 0 ){
				exit(EXIT_FAILURE);
			}
			pamiec[AC] /= pamiec[*operand];
			break;
		case 20: /* %  -  MOD  -  modulo */
			if ( pamiec[*operand] == 0 ){
				exit(EXIT_FAILURE);
			}
			pamiec[AC] = pamiec[AC] % pamiec[*operand];
			break;
		case 21: /* =  -  SEQ  -  skip if equal */
			if ( pamiec[AC] == pamiec[*operand] ){
				(pamiec[IP]) += 2;
			}
			break;
		case 22: /* <  -  SLT  -  skip if less than */
			if ( (pamiec[AC]) < pamiec[*operand] ){
				(pamiec[IP]) += 2;
			}
			break;
		case 23: /* >  -  SGT  -  skip if greater than */
			if ( (pamiec[AC]) > pamiec[*operand] ){
				(pamiec[IP]) += 2;
			}
			break;
		case 24: /* ,  -  LAA  -  load address to AC */
			czy_nie_acc(tryb);
			pamiec[AC] = *operand;
			break;
		case 25: /* `  -  LAS  -  load address to SP */
			czy_nie_acc(tryb);
			pamiec[SP] = *operand;
			break;
		case 26: /* .  -  LDA  -  load */
			pamiec[AC] = pamiec[*operand];
			break;
		case 27: /* :  -  STA  -  store */
			pamiec[*operand] = pamiec[AC];
			break;
		case 28: /* ?  -  ICH  -  input character */
			pamiec[*operand] = getchar();
			if (pamiec[*operand]  ==  EOF){
				pamiec[*operand] = ROZMIAR_NOC - 1;
			}
			break;
		case 29: /* !  -  OCH  -  output character */
			putchar(pamiec[*operand]);
			break;
		case 30: /* \?  -  INU  -  input number */
			scanf("%d", &pamiec[*operand]);
			if ( (pamiec[*operand]) == EOF ){
				pamiec[*operand] = ROZMIAR_NOC - 1;
			}
			break;
		case 31: /* \!  -  ONU  - output number */
			printf("%d", pamiec[*operand]);
			break;
	}
}

/* Instrukcja traktuje swoje argumenty zdefiniowana wyzej operacja modulo */
void wszystko_modul(int *operand, int *adres, int *argument, int pamiec[])
{
	*adres = mod(*adres);
	*operand = mod(*operand);
	*argument = mod(*argument);
	pamiec[AC] = mod(pamiec[AC]);
	pamiec[SP] = mod(pamiec[SP]);
	pamiec[IP] = mod(pamiec[IP]);
}

/* Wczytuje instrukcje (rozkodowuje ja na operacje i tryb) oraz wartosc
argumentu do osobnch zmiennych */
void zaladuj_rejestr_instrukcji(int pamiec[], int *operacja, int *tryb,
	int *argument, int adres)
{
	*operacja = czytaj_operacja(mod(pamiec[adres]));
	*tryb = czytaj_tryb(mod(pamiec[adres]));
	*argument = mod(adres + 1);
}

int main(int argc, char *argv[])
{
	int adres = 0; /* odpowiada za adres aktualnego operandu */
	int operand = 0; /* wartosc aktualnego operandu */
	int ilosc_slow; /* ilosc liczb calkowitych, ktore zawiera wczytany program */
	int operacja; /* odczytany z numeru instrukcji numer operacji */
	int tryb; /* odczytany z numeru instrukcji numer trybu adresowania */
	int argument; /* wartosc aktualnego argumentu( kolejnego slowa w pamieci ) */
	int pamiec[ROZMIAR_NOC + 3 ]; /* Pamiec maszyny Noc wraz z jej rejestrami: */

	assert( argc > 0 );
	zeruj_pamiec( pamiec );
	wczytaj_program( argv, pamiec, &ilosc_slow );


	while(TRUE) {
		zaladuj_rejestr_instrukcji(pamiec, &operacja, &tryb, &argument, adres);
		wczytaj_do_operandu(argument, tryb, &operand, pamiec);
		stosuj_operacja(tryb, operacja, &operand, pamiec);
		wszystko_modul(&operand, &adres, &argument, pamiec);
		adres = pamiec[IP];
	}
	return 0;
}
