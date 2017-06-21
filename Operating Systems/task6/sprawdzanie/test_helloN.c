/** Systemy operacyjne, testy zadania 6, 2017 **/

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test_helloN_default.h"

#define size(t) ((ssize_t)(sizeof(t)/sizeof((t)[0])))

typedef void (*test_function_t)(int test);

static const char init_012[] =
  "012345678901234567890123456789012345678901234567890123456789";
static const char init_aaa[] =
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char init_abc[] =
  "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefgh";
static const char chck_abc[] =
  "abcdefghijklmnopabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqr";
static const char init_cut[] =
  "=cutoff words again=";
static const char init_big[1000];

static char io_buffer[DEVICE_SIZE]; /* bufor odczytu z urządzenia */

static int fp = -1; /* deskryptor urządzenia */

static void error(int test, const char *fmt, ...) {
  va_list fmt_args;

  if (test)
    fprintf(stdout, "TEST %d FAIL: ", test);
  else
    fprintf(stdout, "TEST FAIL: ");

  va_start(fmt_args, fmt);
  vfprintf(stdout, fmt, fmt_args);
  va_end(fmt_args);

  if (errno)
    fprintf(stdout, " (%d; %s)\n", errno, strerror(errno));
  else
    fprintf(stdout, "\n");

  exit(EXIT_FAILURE);
}

/** Funkcje pomocnicze, z których składane są testy. **/

static void test_open(int test) {
  fp = open("/dev/helloN", O_RDWR);
  if (fp < 0)
    error(test, "cannot open device");
}

static void test_close(int test) {
  if (close(fp) < 0)
    error(test, "cannot close device");
}

static void test_read(int test, ssize_t count, ssize_t expected) {
  ssize_t bytes;

  bytes = read(fp, io_buffer, count);
  if (bytes < 0)
    error(test, "cannot read device");
  if (bytes != expected)
    error(test, "read %ld bytes, expected %ld bytes", bytes, expected);
}

static void test_write(int test, const char *buffer, ssize_t count, ssize_t expected) {
  ssize_t bytes;

  bytes = write(fp, buffer, count);
  if (bytes < 0)
    error(test, "cannot write device");
  if (bytes != expected)
    error(test, "write %ld bytes, expected %ld bytes", bytes, expected);
}

static void test_seek(int test, off_t offset) {
  if (lseek(fp, offset, SEEK_SET) < 0)
    error(test, "cannot seek device");
}

static void test_check(int test, const char *expected, ssize_t count) {
  ssize_t i;

  for (i = 0; i < count; ++i)
    if (io_buffer[i] != expected[i])
      error(test, "read %c at index %ld, expected %c", io_buffer[i], i, expected[i]);
}

/** Reinicjowanie zawartość bufora do predefiniowanej wartości */

static void buffer_init(int test) {
  test_open(test);
  test_write(test, init_012, DEVICE_SIZE, DEVICE_SIZE);
  test_close(test);
}

/** Właściwe testy **/

static void first_read(int test) {
  test_open(test);
  test_read(test, DEVICE_SIZE, DEVICE_SIZE);
  test_check(test, init_aaa, DEVICE_SIZE);
  test_close(test);
}

static void seek_and_read(int test) {
  test_open(test);
  test_seek(test, 10);
  test_read(test, DEVICE_SIZE, DEVICE_SIZE - 10);
  test_check(test, init_aaa + 10, DEVICE_SIZE - 10);
  test_close(test);
}

static void first_write(int test) {
  test_open(test);
  test_write(test, init_cut, size(init_cut) - 1, size(init_cut) - 1);
  test_close(test);
}

static void big_write(int test) {
  test_open(test);
  test_write(test, init_big, size(init_big) - 1, DEVICE_SIZE);
  test_close(test);
}

static void many_operations(int test) {
  test_open(test);
  test_write(test, init_abc, DEVICE_SIZE, DEVICE_SIZE);
  test_close(test);
  test_open(test);
  test_seek(test, 16);
  test_write(test, init_abc, DEVICE_SIZE, DEVICE_SIZE - 16);
  test_seek(test, 0);
  test_read(test, DEVICE_SIZE, DEVICE_SIZE);
  test_check(test, chck_abc, DEVICE_SIZE);
  test_close(test);
}

static void after_update(int test) {
  test_open(test);
  test_read(test, DEVICE_SIZE, DEVICE_SIZE);
  test_check(test, init_012, DEVICE_SIZE);
  test_close(test);
}

static test_function_t test_function_table[] = {
  buffer_init,
  first_read,
  seek_and_read,
  first_write,
  big_write,
  many_operations,
  after_update,
};

int main(int argc, char *argv[]) {
  assert(argc == 2);
  int test = atoi(argv[1]);
  assert(test >= 0 && test < size(test_function_table));
  if (test)
    fprintf(stdout, "TEST %d\n", test);
  test_function_table[test](test);
  if (test)
    fprintf(stdout, "TEST %d PASS\n", test);
  return 0;
}
