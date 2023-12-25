#include <type_traits>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "lfsr_small.h"

template<std::size_t N>
void
test_lfsr()
{
  SmallLFSR<N> lfsr;
  using State = decltype(lfsr.state());

  // the expected number of entries is 2**N-1
  const std::size_t expected_size = (1ULL << N) - 1;

  std::vector<State> states;
  states.reserve(expected_size);

  const auto initial_state = lfsr.state();
  do {
    states.push_back(lfsr.state());
    lfsr.next();
  } while (lfsr.state() != initial_state);

  // correct size
  REQUIRE(states.size() == expected_size);

  // zero must not be included
  REQUIRE(std::ranges::all_of(states, [](State s) { return s != 0; }));

  // all values must be within (0,2**N)
  REQUIRE(std::ranges::all_of(states, [](State s) {
    constexpr std::common_type_t<State, std::size_t> max = (1ULL << N);
    return s > 0 && s < max;
  }));
}

TEST_CASE("test small LSFR (8 bits) with brute force")
{
  test_lfsr<3>();
  test_lfsr<4>();
  test_lfsr<5>();
  test_lfsr<6>();
  test_lfsr<7>();
  test_lfsr<8>();
}

TEST_CASE("test small LSFR (16 bits) with brute force")
{
  test_lfsr<9>();
  test_lfsr<10>();
  test_lfsr<11>();
  test_lfsr<12>();
  test_lfsr<13>();
  test_lfsr<14>();
  test_lfsr<15>();
  test_lfsr<16>();
}

TEST_CASE("test small LSFR (32 bits) with brute force")
{
  test_lfsr<17>();
}
