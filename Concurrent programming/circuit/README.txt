Programowanie Współbieżne 2016/2017
Zadanie 3 - Obwód Matematyczny

treść: https://www.mimuw.edu.pl/~mp249046/teaching/pw2016z/zadania/zadanie3/zadanie3.html

Zadaniem było stworzenie obwodu matematycznego, tzn na podstawie serii definicji zmiennych stworzenie procesów, które
będą się komunikowały wg schematu grafu wyznaczonego przez definicje. Następnie wczytywane są z wejścia listy
inicjalizacyjne zmiennych. Procesy, komunikując się ze sobą, mają obliczyć wartość obwodu dla danego wartościowania lub
wypisać stosowną informacje w przypadku niemożliwości.

W zadaniu tworzę odpowiednią ilość procesów realizujących zadanie. Główny program wysyła wartości zmiennej i (z listy
inicjalizacji lub -1, gdy wartość nie była podana) do procesu-odbiorcy, który przekazuje wartość zmiennej do wszystkich
węzłów trzymających daną zmienną. Wartości są propagowane i obliczane. Pierwszy bit wiadomości przekazywanej między
procesami stanowi informację, czy wartość jest "uszkodzona" - tzn "poniżej" w grafie pojawiła się niemożliwość
obliczenia wartości danej zmiennej. Nie implikuje to niemożliwości obliczenia wartości układu - być może została podana
wartość zmiennej "wyżej", a niezainicjalizowana zmienna nie będzie potrzebna.
Po wykonaniu obliczenia przez obwód, proces odpowiadający korzeniowi grafu, tzn pierwszej zdefiniowanej zmiennej,
przekazuje wiadomość (zaczynającą się od bitu 0, jeśli nie było możliwości obliczenia) do głównego programu, który
z kolei drukuje odpowiedni komunikat na standardowe wyjście.

Ostatnim etapem jest zwolnienie zaalokowanej przez program pamięci.

Program był testowany na przykładach dostarczonych wraz z treścią zadania oraz własnych; wykonano także sprawdzenie
wycieków pamięci za pomocą programu valgrind.

