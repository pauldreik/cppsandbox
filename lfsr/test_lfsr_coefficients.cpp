#include <algorithm>

#include <catch2/catch_test_macros.hpp>

#include "lfsr_coefficients.h"

template<std::size_t... ints>
void
verify_taps(std::index_sequence<ints...>)
{
  const std::size_t taps[] = { ints... };
  CHECK(std::ranges::is_sorted(taps, std::greater<>()));
  const auto [min, max] = std::ranges::minmax_element(taps);
  CHECK(*min < *max);
}

TEST_CASE("coefficients are ok")
{
  // will fail to compile
  // auto taps = getTaps<0>();

  verify_taps(getTaps<3>());
  verify_taps(getTaps<123>());
}
