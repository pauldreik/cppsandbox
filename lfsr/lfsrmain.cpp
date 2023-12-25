#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "integerselect.h"
#include "lfsr.h"
#include "lfsr_coefficients.h"

// list of taps
// https://datacipy.cz/lfsr_table.pdf
//
// here is a program for generating taps, it is very slow for sizes over 200:
// https://github.com/hayguen/mlpolygen

// this is from https://en.wikipedia.org/wiki/Linear-feedback_shift_register
class LFSR_wikipedia
{
public:
  LFSR_wikipedia() {}

  void next()
  {
    auto lfsr = m_state;
    /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    auto bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
    // taps:
    lfsr = (lfsr >> 1) | (bit << 15);
    m_state = lfsr;
  }

  /// observe the state
  std::uint16_t state() const { return m_state; }

private:
  std::uint16_t m_state = 1;
};

class LFSR16_v2
{
  // the size of the shift register
  inline static constexpr std::size_t N = 16;

  using State = std::uint16_t;

  static_assert(
    std::numeric_limits<State>::digits >= N,
    "the state must be able to represent the size of the shift register");

  template<std::size_t... ints>
  static constexpr State mask_impl(std::index_sequence<ints...>)
  {
    constexpr std::size_t v[] = { ints... };
    static_assert(std::adjacent_find(std::cbegin(v), std::cend(v)) ==
                    std::cend(v),
                  "taps must not contain duplicate elements");
    static_assert(
      std::is_sorted(std::cbegin(v), std::cend(v), std::greater<>()),
      "taps must be sorted in descending order");
    static_assert(
      std::all_of(std::cbegin(v), std::cend(v), [](auto x) { return x <= N; }),
      "all taps must be lower or equal than N");
    return ((State{ 1U } << (N - ints)) | ...);
  }

public:
  LFSR16_v2() {}

  // taps numbered according to LFSR convention
  using taps = std::index_sequence<16, 14, 13, 11>;
  static constexpr State mask() { return mask_impl(taps{}); }

  void next()
  {
    constexpr auto m = mask();
    const auto bit = __builtin_parity(m_state & m);
    m_state = (m_state >> 1) | (bit << (N - 1));
  }

  /// observe the state
  State state() const { return m_state; }

private:
  State m_state = 1;
};

// the size of the shift register
inline static constexpr std::size_t Nbits = 16;
template<std::size_t N>
class LFSR16_v3
{

  using State = SelectInteger_t<N>;

  static_assert(
    std::numeric_limits<State>::digits >= N,
    "the state must be able to represent the size of the shift register");

  template<std::size_t... ints>
  static constexpr State mask_impl(std::index_sequence<ints...>)
  {
    constexpr std::size_t v[] = { ints... };
    static_assert(std::adjacent_find(std::cbegin(v), std::cend(v)) ==
                    std::cend(v),
                  "taps must not contain duplicate elements");
    static_assert(
      std::is_sorted(std::cbegin(v), std::cend(v), std::greater<>()),
      "taps must be sorted in descending order");
    static_assert(
      std::all_of(std::cbegin(v), std::cend(v), [](auto x) { return x <= N; }),
      "all taps must be lower or equal than N");
    return ((State{ 1U } << (N - ints)) | ...);
  }

public:
  LFSR16_v3() {}

  // taps numbered according to LFSR convention
  using taps = decltype(getTaps<N>());
  static constexpr State mask() { return mask_impl(taps{}); }

  void next()
  {
    constexpr auto m = mask();
    const auto bit = __builtin_parity(m_state & m);
    m_state = (m_state >> 1) | (bit << (N - 1));
  }

  /// observe the state
  State state() const { return m_state; }

private:
  State m_state = 1;
};

template<typename LFSR>
int
run_impl()
{
  LFSR x;
  const auto initial = x.state();
  do {
    std::cout << x.state() << '\n';
    x.next();
  } while (x.state() != initial);
  return 0;
}

int
main(int argc, char* argv[])
{

  int impl = 0;
  if (argc > 1) {
    std::stringstream x(argv[1]);
    if (!(x >> impl)) {
      std::cout << "failed parse\n";
      std::abort();
    }
  }

  switch (impl) {
    case 0:
      return run_impl<LFSR_wikipedia>();
    case 1:
      return run_impl<LFSR16_v2>();
    case 2: {
      using LFSR = LFSR16_v3<16>;
      return run_impl<LFSR>();
    }
    case 3: {
      using LFSR = BigLFSR<16>;
      //  return run_impl<LFSR>();
    }
    default:
      std::cout << "unknown implementation\n";
      std::abort();
  }
}
