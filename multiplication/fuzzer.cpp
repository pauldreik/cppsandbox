#include "multiplication.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

bool
reference_products_equal(const unsigned a,
                         const unsigned b,
                         const unsigned c,
                         const unsigned d)
{
  using U = std::uint64_t;
  return U{ a } * b == U{ c } * d;
}

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size)
{

  unsigned ints[4]{};
  if (Size < sizeof(ints)) {
    return 0;
  }
  std::memcpy(&ints[0], Data, std::min(Size, sizeof(ints)));
  const auto [a, b, c, d] = ints;
  if (reference_products_equal(a, b, c, d) != products_equal(a, b, c, d)) {
    std::cerr << "found test case " << a << " * " << b << " != " << c << " * "
              << d << '\n';
    std::abort();
  }
  return 0;
}
