#pragma once

#include <algorithm>

#include "integerselect.h"
#include "lfsr_coefficients.h"

/**
 * SmallLFSR is a linear feedback shift register
 * for smaller types. Small is defined as being able
 * to fit within the largest native integer the platform supports.
 */
template<std::size_t N>
class SmallLFSR
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
  SmallLFSR() {}

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
