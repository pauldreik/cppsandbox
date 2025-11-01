#include <cassert>

#include <fmt/core.h>

#include "multiplication.h"

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  static_assert(multiply_modulo<3>(1, 1) == 1);
  static_assert(multiply_modulo<3>(0, 1) == 0);
  static_assert(multiply_modulo<3>(4, 1) == 1);
  static_assert(multiply_modulo<3>(1, 4) == 1);
  static_assert(multiply_modulo<3>(2, 4) == 2);
  static_assert(multiply_modulo<3>(4, 2) == 2);

  static_assert(products_equal(0, 0, 0, 0));
  static_assert(products_equal(0, 0, 0, 1));
  static_assert(products_equal(0, 0, 1, 0));
  static_assert(!products_equal(0, 0, 1, 1));
  static_assert(products_equal(0, 1, 0, 0));
  static_assert(products_equal(0, 1, 0, 1));
  static_assert(products_equal(0, 1, 1, 0));
  static_assert(!products_equal(0, 1, 1, 1));
  static_assert(products_equal(1, 0, 0, 0));
  static_assert(products_equal(1, 0, 0, 1));
  static_assert(products_equal(1, 0, 1, 0));
  static_assert(!products_equal(1, 0, 1, 1));
  static_assert(!products_equal(1, 1, 0, 0));
  static_assert(!products_equal(1, 1, 0, 1));
  static_assert(!products_equal(1, 1, 1, 0));
  static_assert(products_equal(1, 1, 1, 1));

  constexpr unsigned interesting_numbers[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    655531,
    655532,
    655533,
    655534,
    655535,
    655536,
    655537,
    655538,
    655539,
    655540,
    UINT_MAX - 5,
    UINT_MAX - 4,
    UINT_MAX - 3,
    UINT_MAX - 2,
    UINT_MAX - 1,
    UINT_MAX,
  };
  for (const unsigned a : interesting_numbers) {
    for (const unsigned b : interesting_numbers) {
      assert(products_equal(a, b, a, b));
      assert(products_equal(a, b, b, a));
      assert(products_equal(b, a, a, b));
      assert(products_equal(b, a, b, a));
    }
  }
  static_assert(!products_equal(4291624960, 4291493888, 0, 0));

  static_assert(!products_equal(4291624960, 4291493888, 0, 0));
  static_assert(!products_equal(4291362816, 4291624960, 3598, 0));
  fmt::println("it works");
}
