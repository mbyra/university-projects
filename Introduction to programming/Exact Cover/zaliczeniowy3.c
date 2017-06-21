/**
 * "Program X"
 *
 * Program wczytuje z wejścia ciąg wierszy, reprezentujący instancję problemu
 * dokładnego pokrycia, i wypisuje reprezentację wszystkich poprawnych
 * rozwiązań.
 * Program jest dostosowany do obsługi danych rzadkich, tzn. takich, w których
 * zdecydowaną większość znaków w wierszu stanowią spacje - przy wczytywaniu
 * danych do tablicy przed i po każdym znaku umieszcza ilość spacji.
 *
 * autor: Marcin Byra
 * wersja: 1.0
 * data: 05.01.2015
 */


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROZMIAR_POCZ 10

#define STR_PRZEPELNIENIE_PAMIECI "Blad: przepelnienie pamieci"

/**
 * Stała informująca,że dana pozycja w w danych wejściowych jest pusta, bądź że
 * dana pozycja w rozwiazaniu jest jeszcze niezapełniona.
 */
#define PUSTO ' '


/**
 * Wypisuje komunikat o przepełnieniu pamięci i przerywa wykonanie programu.
 */
void blad_przepelnienie_pamieci(void)
{
    fprintf(stderr, "%s\n", STR_PRZEPELNIENIE_PAMIECI);
    exit(EXIT_FAILURE);
}

/**
 * Wypisuje na wyjście zakodowaną tablicę @a rozmiaru @rozmiar
 */
void wypisz_zakodowana(int a[], int rozmiar)
{
	int i = 0;
	int j = 0;
	while(i + 2 < rozmiar){
			if (i % 2 == 0){
				while(j++ < a[i]){
					printf(" ");
				}
				j = 0;
			}else {
				printf("%c",a[i]);
			}
			i++;
			if(a[i] == '\n' && a[i+1] == 0 && a[i+2] == 0){
				i = rozmiar;
			}
	}
}

/**
 * Wypisuje na wyjście tablicę @a rozmiaru @rozmiar
 */
void wypisz(int tablica[], int rozmiar)
{
	int i = 0;
	while (i < rozmiar) {
		printf("%c", tablica[i++]);
	}
}

/**
 * Zeruje (rozszerzoną) tablicę @a, od pozycji @rozmiar_stary do końca podanego
 * przez zmienną @rozmiar_nowy.
 */
void zeruj(int a[], int *rozmiar_stary, int rozmiar_nowy)
{
		while((*rozmiar_stary) < rozmiar_nowy){
			a[(*rozmiar_stary)++] = 0;
		}
}

/**
 * spacjuje (rozszerzoną) tablicę @a, od pozycji @rozmiar_starej do końca podanego
 * przez zmienną @rozmiar.
 */
void spacjuj(int a[], int *rozmiar_stary, int rozmiar_nowy)
{
		while((*rozmiar_stary) < rozmiar_nowy){
			a[(*rozmiar_stary)++] = PUSTO;
		}
}

/**
 * Dwukrotnie powieksza tablicę @a rozmiaru @rozmiar.
 */
void powieksz(int **a, int *rozmiar)
{
	int *b;
	int rozmiar_nowy = 2 * *rozmiar;

	if((b = realloc(*a, rozmiar_nowy * sizeof *b)) == NULL) {
		free(*a);
		blad_przepelnienie_pamieci();
	}
	*a = b;
	zeruj(*a, &*rozmiar, rozmiar_nowy);
}

/**
 * Zwraca ilość wczytanych spacji, zapisuje do zmiennej @znak pierwszy wczytany
 * znak niebędący spacją.
 */
int ile_spacji(char *znak)
{
	int ile_spacji = 0;

	*znak = getchar();
	while(*znak != EOF && *znak == PUSTO){
		ile_spacji++;
		*znak = getchar();
	}

	return ile_spacji;
}

/**
 * Jeśli wartość @ile_w_wierszu jest większa niż @max, przypisuje zmiennej @max
 * wartość @ile_w_wierszu. Następnie zeruje @ile_w_wierszu.
 */
void aktualizuj_max(int *ile_w_wierszu, int *max)
 {
	 if (*ile_w_wierszu > *max){
		 *max = *ile_w_wierszu;
	 }
	 *ile_w_wierszu = 0;
 }

/**
 * Wczytuje z wejścia dane do tablicy @a o początkowym rozmiarze @rozmiar.
 * Na miejscach nieparzystych zostają zapisane symbole wczytanych znaków, a na
 * parzystych liczby reprezentujące ilości spacji między znakami.
 * Zapisuje ilość znaków w najdłuższym wierszu do zmiennej @max.
 * Zapisuje ilość wierszy do zmiennej @wiersze.
 */
void wczytaj(int **a, int *rozmiar, int *max, int *ilosc_wierszy)
{
	char znak;
	int i = 0;
	int ile_w_wierszu = 0;

	*ilosc_wierszy = 0;
	*max = 0;

	(*a)[i] = ile_spacji(&znak);
	ile_w_wierszu += (*a)[i++]; /* bo teraz na pewno wczytałem ilość spacji */
	while(znak != EOF){

		if (i + 1 >= *rozmiar){ /* bo będę potrzebował jeszcze minimum 2 miejsc */
			powieksz(&*a, &*rozmiar);
		}

		(*a)[i++] = znak;
		ile_w_wierszu++; /* bo teraz na pewno wczytałem tylko symbol literki */

		if (znak == '\n'){
			(*ilosc_wierszy)++;
			aktualizuj_max(&ile_w_wierszu, &*max);
		}

		(*a)[i] = ile_spacji(&znak);
		ile_w_wierszu += (*a)[i++]; /* bo teraz na pewno wczytałem ilość spacji */

	}
}

/**
 * Uzupelnia wszystkie fragmenty tablicy @a zakończone wartością symbolu EOL
 * do rozmiaru @max, dodając odpowiednią liczbę do liczby symbolizującej ilość
 * spacji na końcu wiersza.
 */
void uzupelnij(int a[], int rozmiar, int max)
 {
	 int ile = 0;
	 int i = 0;
	 int roznica;
	 while(i < rozmiar){

		 if (i % 2 == 0){
			 ile += a[i]; /* bo w parzystych argumentach wpisane są ilości spacji */
		 }else{
			 ile++; /* bo w nieparzystych wpisane są znaczki */
		 }

		 if(a[i] == '\n'){
			 if ((roznica = max - ile) != 0){
				 a[i - 1] += roznica; /* w a[i-1] jest ilość spacji na końcu wiersza */
			 }
			 ile = 0;
		 }
		 i++;

	 }
 }

 /**
  * Funkcja rozkodowuje dane z tablicy @a, w której dane wejściowe zapisane są
  * w formacie: na nieparzystych miejscach ilości spacji między znakami, których
  * symbole zakodowane są na miejscach parzystych. W przypadku końca wiersza
  * zakodowany jest znak końca linii. Funkcja dla podanych @dlugosc_wiersza,
  * @numer_wiersza oraz pozycji w wierszu @i zwraca prawdziwą wartość danych
  * wejściowych w tym miejscu.
  */
int rozkoduj(int a[], int i, int numer_wiersza)
 {
 	int pozycja_ogolna = 0;
 	int pozycja_w_wierszu = 0;
 	int wiersz = 0;
	int spacje;
	int zwroc = -1;

 	while (wiersz < numer_wiersza) {
		if (pozycja_ogolna % 2 == 0){
			/*tzn trafiłem na oznaczenie ilości spacji, nieistotne tutaj */
		}
		else{
			/* gdy trafię na znak nowej linii zwiększam numer aktualnego wiersza */
			if( a[pozycja_ogolna] == '\n' ) {
				wiersz++;
			}
		}
		pozycja_ogolna++;
 	}

 	/* aktualnie jestem na pierwszej nieparzystej pazycji (informującej o
 		 ilości początkowych spacji) w porządanym wierszu, tzn. że
 		 @pozycja_w_wierszu = 0.
 	*/

 	while ( pozycja_w_wierszu <= i && zwroc < 0) {
		if ( pozycja_ogolna % 2 == 0) {
			/* tzn ze jestem w wierszu na ilości spacji */

			for (spacje = 0; spacje < a[pozycja_ogolna]; spacje++) {
				if( pozycja_w_wierszu == i ) {
					zwroc = PUSTO;
				}
				pozycja_w_wierszu++;
			}
		}

		else {
			/* tzn, ze trafilem na symbol znaku */
			if (pozycja_w_wierszu == i) {
				zwroc = a[pozycja_ogolna];
			}
			pozycja_w_wierszu++;
		}

		/* jak nic nie zwróciło, to do następnej pozycji: */
		pozycja_ogolna++;
	}

	return zwroc;
}

/**
 * Sprawdza, czy dana kolumna rozwiazania jest już zajeta;
 * Jeśli jest zajęta, zwraca true;
 * Jeśli nie jest zajęta zwraca false;
 */
bool pozycja_juz_zajeta(int rozwiazanie[], int kolumna)
{
	if(rozwiazanie[kolumna] == PUSTO){
		return false;
	}else{
		return true;
	}
}

/**
 * Sprawdza, czy można scalić wiersz z rozwiązaniem.
 * Jeśli da się scalić, zwraca true.
 * Jeśli nie da się scalić, zwraca false.
 */
bool mozna_scalic(int a[], int rozwiazanie[], int wiersz, int dlugosc_wiersza)
{
	int pozycja = 0;

	while(pozycja < dlugosc_wiersza-1) {
		if(rozkoduj(a, pozycja, wiersz) != PUSTO) {
			if(rozwiazanie[pozycja] != PUSTO) {
				return false;
			}
		}
		pozycja++;
	}

	return true;
}

/**
 * Kopiuje tablicę @z_tej[] na tablicę @na_ta[],pierwsze @dlugosc_wiersza znakow
 */
void kopiuj(int z_tej[], int na_ta[], int dlugosc_wiersza)
{
	int i = 0;

	while (i < dlugosc_wiersza) {
		na_ta[i] = z_tej[i];
		i++;
	}
}

/**
 * Scala dany wiersz tablicy @a z tablica @rozwiazanie.
 */
void scal(int a[], int rozwiazanie[], int wiersz, int dlugosc_wiersza)
{
	int pozycja = 0;

	while (pozycja < dlugosc_wiersza) {
		if(rozkoduj(a, pozycja, wiersz) != PUSTO) {
			rozwiazanie[pozycja] = rozkoduj(a, pozycja, wiersz);
		}
		pozycja++;
	}
}

/**
 * Sprawdza, czy wypełniona została już cała tablica @rozwiązanie o długości
 * @dlugosc_wiersza. Jeśli tak, to zwraca true. Jeśli są w nim puste miejsca,
 * to zwraca false.
 */
bool cale_rozwiazanie(int rozwiazanie[], int dlugosc_wiersza)
{
	int i = 0;

	while ( i < dlugosc_wiersza) {
		if (rozwiazanie[i] == PUSTO) {
			return false;
		}
		i++;
	}

	return true;
}


/**
 * Rekurencyjna funkcja, rozwiązująca problem całkowitego przykrycia. Wypisuje
 * pełne, poprawne rozwiązania na wyjście.
 */
void szukaj_rozwiazan(int a[], int rozwiazanie[], int kolumna,
	int dlugosc_wiersza, int ilosc_wierszy)
{
	int wiersz = -1;
	int *kopia;

	kopia = malloc(dlugosc_wiersza * sizeof *kopia);

	if (kolumna < dlugosc_wiersza) {

		if (pozycja_juz_zajeta(rozwiazanie,kolumna)) {
			/* pozycja w rozwiązaniu już zajęta, od razu przejdzie do kolejnej */

			kopiuj(rozwiazanie, kopia, dlugosc_wiersza);
			szukaj_rozwiazan(a, rozwiazanie, kolumna + 1, dlugosc_wiersza,
				 ilosc_wierszy);
			kopiuj(kopia, rozwiazanie, dlugosc_wiersza);

		}

		else {
			/* pozycja w rozwiązaniu jest dostępna, szuka pasującego wiersza */
			wiersz++;
			while (wiersz < ilosc_wierszy) {

				if (rozkoduj(a, kolumna, wiersz) != PUSTO) {

					if (mozna_scalic(a, rozwiazanie, wiersz, dlugosc_wiersza)) {

						kopiuj(rozwiazanie, kopia, dlugosc_wiersza);
						scal(a, rozwiazanie, wiersz, dlugosc_wiersza);
						if (cale_rozwiazanie(rozwiazanie, dlugosc_wiersza)) {
							wypisz(rozwiazanie, dlugosc_wiersza);
						}
						szukaj_rozwiazan(a, rozwiazanie, kolumna + 1, dlugosc_wiersza,
							ilosc_wierszy);
						kopiuj(kopia, rozwiazanie, dlugosc_wiersza);
					}

				}

				wiersz++;
			}

		}

	}

	free(kopia);
}

/**
 * Szuka rozwiązania problemu dokładnego przykrycia dla danych w postaci tablicy
 * @a podzielonej na wiersze znakami \n, z reprezentacją literek jako wartości
 * liczbowych ich symboli i ilościami spacji między znakami reprezentowanymi
 * przez liczby całkowite.
 * Długość "wiersza" z wliczonym \n przekazywana w zmiennej @dlugosc
 */
void rozwiaz_problem(int a[], int dlugosc_wiersza, int ilosc_wierszy)
{
	int *rozwiazanie; /* tu będzie tworzony wynikowy wiersz */
	int rozmiar_stary = 0; /* tylko do przekazania 0 do funkcji zeruj */
	int kolumna = 0; /* informuje o aktualnej kolumnie przy wywołaniu rekursji */

	rozwiazanie = malloc(dlugosc_wiersza * sizeof *rozwiazanie);

	spacjuj(rozwiazanie, &rozmiar_stary, dlugosc_wiersza);
	szukaj_rozwiazan(a, rozwiazanie, kolumna, dlugosc_wiersza, ilosc_wierszy);

	free(rozwiazanie);

}


int main(void)
{
	int *a; /* tablica z danymi wejściowymi w formie skompresowanej */
	int rozmiar = ROZMIAR_POCZ;
	int rozmiar_stary = 0; /* tylko do przekazania 0 do funkcji zeruj */
	int max;
	int ilosc_wierszy;

	if ((a = malloc(rozmiar * sizeof *a)) == NULL) {
        blad_przepelnienie_pamieci();
  }

	zeruj(a, &rozmiar_stary, rozmiar);
	wczytaj(&a, &rozmiar, &max, &ilosc_wierszy);
	uzupelnij(a, rozmiar, max);

	rozwiaz_problem(a, max, ilosc_wierszy);

	free(a);

	return EXIT_SUCCESS;
}
