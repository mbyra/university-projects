#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_THREADS 50

void proberen(int32_t *);
void verhogen(int32_t *);

static int global_count;
static int32_t semaphore;
static volatile int global_value;

static void * inc_thread(void *dummy) {
  int i, j;

  for (i = 0; i < global_count; ++i) {
    proberen(&semaphore);
    for (j = 0; j < global_count; ++j) {
      ++global_value;
    }
    verhogen(&semaphore);
  }

  return NULL;
}

int init(size_t);
void producer(void);
void consumer(void);

static int64_t max_value;
static int64_t produced_value;
static int64_t consumed_value;

int produce(int64_t *data) {
  if (produced_value == max_value) {
    return 0;
  }
  else {
    produced_value++;
    *data = produced_value;
    // jakieś opóźnienie od czasu do czasu
    if (produced_value % 256 == 0)
      usleep(90000);
    return 1;
  }
}

int consume(int64_t data) {
  consumed_value++;
  assert(data == consumed_value);
  // jakieś opóźnienie od czasu do czasu
  if (consumed_value % 128 == 64)
    usleep(75000);
  return consumed_value != max_value;
}

static void * producer_thread(void *dummy) {
  producer();
  return NULL;
}

int main(int argc, char *args[]) {
  assert(argc >= 2);

  if (strncmp(args[1], "-s", 2) == 0) {
    int i, thread_count;
    pthread_t tid[MAX_THREADS];

    assert(argc == 4);
    assert(sscanf(args[2], "%d", &thread_count) == 1);
    assert(sscanf(args[3], "%d", &global_count) == 1);
    assert(thread_count > 1 && thread_count <= MAX_THREADS);
    assert(global_count > 0);

    global_value = 0;
    semaphore = 1;

    for (i = 1; i < thread_count; ++i)
      assert(pthread_create(&tid[i], NULL, &inc_thread, NULL) == 0);

    inc_thread(NULL); // Wątek 0 już mamy uruchomiony.

    for (i = 1; i < thread_count; ++i)
      assert(pthread_join(tid[i], NULL) == 0);

    assert(global_value = global_count * global_count * thread_count);
  }
  else if (strncmp(args[1], "-w", 2) == 0) {
    int result;
    size_t bufer_size;
    pthread_t tid;

    assert(argc == 4);
    assert(sscanf(args[2], "%lu", &bufer_size) == 1);
    assert(sscanf(args[3], "%ld", &max_value) == 1);

    result = init(bufer_size);
    if (bufer_size > (1UL << 31) - 1UL)
      assert(result == -1);
    else if (bufer_size == 0)
      assert(result == -2);
    else if (bufer_size >= 2000000000UL) // Ta stała jest dobrana do testu.
      assert(result == -3);
    else
      assert(result == 0);

    if (result == 0) {
      // Producent działa w osobnym wątku.
      assert(pthread_create(&tid, NULL, &producer_thread, NULL) == 0);
      consumer();
      assert(pthread_join(tid, NULL) == 0);
    }

    assert(produced_value == max_value);
    assert(consumed_value == max_value);
  }
  else {
    assert(0);
  }

  return 0;
}
