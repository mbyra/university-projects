#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint8_t __executable_start;
extern uint8_t __etext;
extern void *__libc_stack_end;

// Funkcja stack_show nie ma żadnych parametrów. Deklarujemy je tu,
// aby powrzucać śmieci na stos.
void stack_show(uint64_t, ...);

// Podobnie tu
void recursive_function(int, ...);
void dummy_function(int);

int main(int argc, char* args[]) {
  int recursion_level;

  if (argc != 3) {
    return 1;
  }

  recursion_level = atoi(args[1]) - 1;

  if (recursion_level < 0) {
    return 2;
  }

  if (recursion_level > 0) {
    recursive_function(recursion_level);
  }
  else {
    stack_show(0);
  }

  FILE *f = fopen(args[2], "w");
  int i;

  if (f == NULL) {
    return 3;
  }
  // Wypisz, w jakiej (odwrotnej) kolejności oczekujemy wywołań funkcji.
  fprintf(f, "%016lx\n", (uint64_t)stack_show);
  for (i = 0; i < recursion_level; ++i) {
    fprintf(f, "%016lx\n", (uint64_t)recursive_function);
    if ((i & 1) == 0 && i + 1 < recursion_level) {
      fprintf(f, "%016lx\n", (uint64_t)dummy_function);
    }
  }
  fclose(f);

  return 0;
}

// Zminiejszamy poziom optymalizacji, aby coś się zadziało na stosie.
#pragma GCC optimize ("-O1")

void recursive_function(int recursion_level, ...) {
  if (recursion_level == 3) {
    recursive_function(recursion_level - 1, 0, 0, 0, 0, 0,
                       &__executable_start,
                       &__etext,
                       &__executable_start - 1,
                       &__etext - 1,
                       __libc_stack_end);
  }
  else if (recursion_level > 1) {
    if (recursion_level & 1) {
      recursive_function(recursion_level - 1);
    }
    else {
      dummy_function(recursion_level - 1);
    }
  }
  else {
    stack_show(0, 0, 0, 0, 0, 0,
               (uint64_t)main,
               (uint64_t)stack_show,
               (uint64_t)dummy_function,
               (uint64_t)recursive_function);
  }
}

// Jeszcze bardziej zabraniamy optymalizacji, aby nie rozwijać wywołań funkcji.
#pragma GCC optimize ("-O0")

void dummy_function(int recursion_level) {
  recursive_function(recursion_level);
}
