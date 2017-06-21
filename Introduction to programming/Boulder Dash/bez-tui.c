#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#define ILEWIERSZY 100 /* dlugosc kolumny */
#define ILEKOLUMN 200 /* dlugosc wiersza */


/* Sam ruch Rockforda */
void ruch_rockforda (int ***wiersz, int ***kolumna, int ile_w_pionie, int ile_w_poziomie, int plansza[][ILEKOLUMN])
{
    plansza[***wiersz][***kolumna] = ' ';
    plansza[***wiersz + ile_w_pionie][***kolumna + ile_w_poziomie] = '&';
    ***wiersz = ***wiersz + ile_w_pionie;
    ***kolumna = ***kolumna + ile_w_poziomie;
}

/* W zależności od znaku na polu, na ktore ma ruszyc sie Rockford, wykonuje ruch_rockforda */
void rusz(int ile_w_pionie, int ile_w_poziomie, int **wiersz, int **kolumna, int plansza[][ILEKOLUMN], int **ilosc_diamentow, bool **koniec)
{
    char znak = plansza[**wiersz + ile_w_pionie][**kolumna + ile_w_poziomie];
    if (znak == '.' || znak == ' '){
        ruch_rockforda(&wiersz,&kolumna,ile_w_pionie,ile_w_poziomie,plansza);
    }if (znak == '$'){
        ruch_rockforda(&wiersz,&kolumna,ile_w_pionie,ile_w_poziomie,plansza);
        --**ilosc_diamentow;
    }if (znak == 'O'){
        if (ile_w_poziomie == -1 && **kolumna > 1 && (char) plansza[**wiersz][**kolumna - 2] == ' '){
            plansza[**wiersz][**kolumna] = ' ';
            plansza[**wiersz + ile_w_pionie][**kolumna + ile_w_poziomie] = '&';
            plansza[**wiersz][**kolumna - 2] = 'O';
            **wiersz = **wiersz + ile_w_pionie;
            **kolumna = **kolumna + ile_w_poziomie;
        }if (ile_w_poziomie == 1 && **kolumna < ILEWIERSZY - 1 && (char) plansza[**wiersz][**kolumna + 2] == ' '){
            plansza[**wiersz][**kolumna] = ' ';
            plansza[**wiersz + ile_w_pionie][**kolumna + ile_w_poziomie] = '&';
            plansza[**wiersz][**kolumna + 2] = 'O';
            **wiersz = **wiersz + ile_w_pionie;
            **kolumna = **kolumna + ile_w_poziomie;
        }
    }if (znak == 'X' && **ilosc_diamentow == 0){
        plansza[**wiersz][**kolumna] = ' ';
        **koniec = true;
    }
}
    
/* Wczytuje planszę do tablicy, znajduje Rockforda i liczy diamenty */
void wczytaj_plansze(int plansza[][ILEKOLUMN], int *ilewwierszu, int *ilewkolumnie, int *gdzie_rockford_x, int *gdzie_rockford_y, int *ilosc_diamentow) 
{
    int i;
    int j;
    int ile;
    bool koniecwiersza;
    bool koniecplanszy=false;
    /*wiem, ze troche przekombinowane, JAK BEDZIE CZAS TO ZMIENIC!!! */
    for (i = 0; i < ILEWIERSZY && !koniecplanszy; ++i) {
        koniecwiersza=false;
        for (j = 0; j < ILEKOLUMN+1 && !koniecwiersza; ++j) {
            ile=getchar();
            plansza[i][j]=ile;
            /* jesli napotka eol(wartosc 10),spelnia warunek konca petli wiersza */
            if(ile ==(int) '\n'){ 
                koniecwiersza=true;
            }
            if(ile == (int) '&'){
		*gdzie_rockford_x = i; /* pobiera i, czyli pierwszy argument plansza[i][] */
		*gdzie_rockford_y = j; /* pobiera j, czyli drugi argument plansza[][j] */
	    }
	    if(ile == (int) '$'){
                ++*ilosc_diamentow;
            }
        }
        /* w pierwszym przebiegu zapisuje liczbe znakow w wierszu */
        if(i==0){ 
            *ilewwierszu=j;
        }
        /* jesli po ktoryms zainicjalizowaniu przebiegu petli wiersza po pierwszym obrocie napotkal eol, tzn ze koniec planszy */
        if(j==1){ 
            koniecplanszy=true;
            /*po zakonczeniu czytania planszy zapisuje liczbe znakow w kolumnie */
            *ilewkolumnie=i; 
        }
    }
}

/* Zapełnia tablicę instrukcje[] instrukcjami z pliku wejsciowego */
void wczytaj_instrukcje(int instrukcje[], int *ileinstrukcji)
{
    char kierunek;
    *ileinstrukcji=0;
    /* zakladam, ze po zakonczeniu wiersza polecen jest EOL */
    while( (kierunek = getchar()) != EOF ) {
        instrukcje[*ileinstrukcji] = kierunek;
        ++*ileinstrukcji;
    }
}

/* Sprawdza, czy są kamienie nad powietrzem, i ewentualnie je zrzuca */
void sprawdz_stabilnosc(int plansza[][ILEKOLUMN])
{
    int i;
    int j;
    char tutaj;
    for (i = 0; plansza[0][i] != '\n'; ++i) { /* petla wzdluz wiersza - po numerach kolumn  - */
            for (j = 0; plansza[j][0] != '\n'; ++j) { /* petla wzdluz kolumny - po numerach wierszy  - */
                tutaj = plansza[j][i];
                if ( (tutaj == 'O' || tutaj == '$') && (char) plansza[j+1][i] == ' '){
                    plansza[j][i] = ' ';
                    if ( tutaj == 'O'){
                        plansza[j+1][i] = 'O';
                    }else{
                        plansza[j+1][i] = '$';
                    }
                    j=0;  /* sprawdzam kolumne od gory */
                }
            }
    }
}

/* W zależności od kierunku odczytanego z tablicy instrukcje[], odpowiednio przekierowuje do funkcji rusz */
void wykonaj_instrukcje(int instrukcje[], int ileinstrukcji, int plansza[][ILEKOLUMN], int *gdzie_rockford_x, int *gdzie_rockford_y, int *ilosc_diamentow, bool *koniec)
{
    char komenda;
    int i=0;
    sprawdz_stabilnosc(plansza);
    while ( i < ileinstrukcji && !*koniec ){
        komenda = (char) instrukcje[i];
        if (komenda == 'a') {
            rusz(0,-1, &gdzie_rockford_x, &gdzie_rockford_y, plansza, &ilosc_diamentow, &koniec);
        }
        if (komenda == 'w') {
            rusz(-1,0, &gdzie_rockford_x, &gdzie_rockford_y, plansza, &ilosc_diamentow, &koniec);
        }
        if (komenda == 'd') {
            rusz(0,1, &gdzie_rockford_x, &gdzie_rockford_y, plansza, &ilosc_diamentow, &koniec);
        }
        if (komenda == 's') {
            rusz(1,0, &gdzie_rockford_x, &gdzie_rockford_y, plansza, &ilosc_diamentow, &koniec);
        }
        sprawdz_stabilnosc(plansza);
        ++i;
    }
}

     
     
int main (void) 
{	
	int plansza[ILEWIERSZY][ILEKOLUMN];
        int instrukcje[ILEWIERSZY*ILEKOLUMN];
	int ilewwierszu;
        int ilewkolumnie;
	int gdzie_rockford_x;
	int gdzie_rockford_y;
        int i;
        int j;
        int ileinstrukcji;
        int ilosc_diamentow = 0;
        bool koniec = false;
        
        wczytaj_plansze(plansza, &ilewwierszu, &ilewkolumnie, &gdzie_rockford_x, &gdzie_rockford_y, &ilosc_diamentow);
        wczytaj_instrukcje(instrukcje, &ileinstrukcji);
	wykonaj_instrukcje(instrukcje, ileinstrukcji, plansza, &gdzie_rockford_x, &gdzie_rockford_y, &ilosc_diamentow, &koniec);
        
        for (i = 0; i < ilewkolumnie; ++i) {
            for (j = 0; j < ilewwierszu; ++j) {
                putchar(plansza[i][j]);
            }
        }
        
	return 0;
}
