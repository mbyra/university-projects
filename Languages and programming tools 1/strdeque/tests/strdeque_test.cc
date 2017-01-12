#if TEST_NUM == 601

#include "cstrdequeconst"
#include "cstrdeque"

#include <cassert>
#include <cstddef>
#include <cstring>

int streq(const char* s1, const char* s2, size_t len) {
  if (s1 == s2)
    return 1;

  if (s1 == NULL || s2 == NULL)
    return 0;

  return ::std::strncmp(s1, s2, len) == 0;
}

namespace {
  unsigned long testuj() {
    unsigned long id;

    id = ::jnp1::strdeque_new();
    ::jnp1::strdeque_insert_at(id, 0, "");

    return id;
  }

  unsigned long id1 = testuj();
  unsigned long id2 = ::jnp1::emptystrdeque();
} // anonymous namespace

int main() {
  assert(streq(::jnp1::strdeque_get_at(id1, 0), "", 1));
  ::jnp1::strdeque_delete(id1);
  return id1 == id2;
}

#endif

#if TEST_NUM == 602

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_new();
}

#endif

#if TEST_NUM == 603

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_delete(0);
}

#endif

#if TEST_NUM == 604

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_size(0);
}

#endif

#if TEST_NUM == 605

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_insert_at(0, 0, "");
}

#endif

#if TEST_NUM == 606

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_remove_at(0, 0);
}

#endif

#if TEST_NUM == 607

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_get_at(0, 0);
}

#endif

#if TEST_NUM == 608

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_clear(0);
}

#endif

#if TEST_NUM == 609

#include "cstrdeque"

int main() {
  // To ma się nie kompilować.
  strdeque_comp(0, 0);
}

#endif

#if TEST_NUM == 610

#include "cstrdequeconst"

int main() {
  // To ma się nie kompilować.
  emptystrdeque();
}

#endif
