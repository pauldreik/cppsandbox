#pragma once

#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <limits>
#include <utility>

/**
 * this is a bignum class with very limited functionality, just
 * the bare minimum to be able to use it for implementing large LFSR
 */
template<int Nbits, typename Limb = unsigned int>
struct BigNum
{
  static constexpr std::size_t bitcount() { return Nbits; }
  static inline constexpr int BitsPerLimb = std::numeric_limits<Limb>::digits;
  static inline constexpr int LimbCount =
    (Nbits + (BitsPerLimb - 1)) / BitsPerLimb;
  static inline constexpr int ExcessBits = LimbCount * BitsPerLimb - Nbits;

  template<std::size_t... bits>
  constexpr int parity(std::index_sequence<bits...>) const
  {
    return ((ith_bit<bits>() + ...) & 0x1);
  }

  constexpr int popcount() const
  {
    int ret = 0;
    for (auto& d : m_data) {
      ret += std::popcount(d);
    }
    return ret;
  }

  template<std::size_t bit>
  constexpr bool ith_bit() const
  {
    static_assert(bit <= Nbits);
    constexpr auto limb = bit / BitsPerLimb;
    constexpr auto bitwithinlimb = bit - (bit / BitsPerLimb) * BitsPerLimb;
    constexpr auto limbmask = 1U << bitwithinlimb;
    return (m_data[limb] & limbmask);
  }

  constexpr bool ith_bit(std::size_t bit) const
  {
    assert(bit <= Nbits);
    const auto limb = bit / BitsPerLimb;
    const auto bitwithinlimb = bit - (bit / BitsPerLimb) * BitsPerLimb;
    const auto limbmask = 1U << bitwithinlimb;
    return (m_data[limb] & limbmask);
  }

  constexpr void set_bit_to(std::size_t bit, bool value)
  {
    assert(bit < Nbits);
    const auto limb = bit / BitsPerLimb;
    const auto bitwithinlimb = bit - (bit / BitsPerLimb) * BitsPerLimb;
    const Limb limbmask = 1U << bitwithinlimb;
    if (value) {
      m_data[limb] |= limbmask;
    } else {
      m_data[limb] &= ~limbmask;
    }
  }

  // flips all bits
  void complement_in_place()
  {
    for (auto& d : m_data) {
      d = ~d;
    }
    if constexpr (ExcessBits > 0) {
      constexpr Limb mask = (Limb{ 1U } << (BitsPerLimb - ExcessBits)) - 1U;
      m_data.back() &= mask;
    }
  }

  // right shift one bit
  void shr_one_bit()
  {
    for (std::size_t i = 0; i < m_data.size(); ++i) {
      m_data[i] >>= 1;
      if (i + 1 < m_data.size()) {
        // get the top bit from the bottom bit of the next limb
        m_data[i] |= (m_data[i + 1] << (BitsPerLimb - 1));
      }
    }
  }

  /// lsb is in the beginning. the topmost excess bits at m_data.back() are kept
  /// zero.
  std::array<Limb, LimbCount> m_data{};
};

template<int Nbits, typename Limb>
requires requires
{
  Nbits <= 64;
}
std::uint64_t
to_uint64(const BigNum<Nbits, Limb>& bignum)
{
  std::uint64_t ret{};
  if constexpr (bignum.BitsPerLimb < 64) {
    for (std::size_t i = 0; i < bignum.LimbCount; ++i) {
      ret <<= bignum.BitsPerLimb;
      ret |= bignum.m_data[bignum.LimbCount - 1 - i];
    }
  } else {
    ret = bignum.m_data[0];
  }
  return ret;
}
