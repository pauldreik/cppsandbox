#pragma once

#include <array>

namespace detail {
// the data N=3 to N=168 is from
// http://scott.joviansynth.com/electronics/LFSRtaps.html
// but some were edited (in particular the N=16 entry, to match the wikipedia
// LFSR article)
//
// here is a very long list of taps:
// https://datacipy.cz/lfsr_table.pdf
//
// here is a program for generating taps, it is very slow for sizes over 200:
// https://github.com/hayguen/mlpolygen

template<int N>
constexpr auto
getTapsImpl()
{

  struct Nbits
  {
    int bits;
  };
  using RawTaps = std::array<int, 4>;
  struct Pair
  {
    Nbits nbits;
    RawTaps rawtaps;
  };

  constexpr Pair data[] = { { Nbits{ 3 }, RawTaps{ 3, 2 } },
                            { Nbits{ 4 }, RawTaps{ 4, 3 } },
                            { Nbits{ 5 }, RawTaps{ 5, 3 } },
                            { Nbits{ 6 }, RawTaps{ 6, 5 } },
                            { Nbits{ 7 }, RawTaps{ 7, 6 } },
                            { Nbits{ 8 }, RawTaps{ 8, 6, 5, 4 } },
                            { Nbits{ 9 }, RawTaps{ 9, 5 } },
                            { Nbits{ 10 }, RawTaps{ 10, 7 } },
                            { Nbits{ 11 }, RawTaps{ 11, 9 } },
                            { Nbits{ 12 }, RawTaps{ 12, 6, 4, 1 } },
                            { Nbits{ 13 }, RawTaps{ 13, 4, 3, 1 } },
                            { Nbits{ 14 }, RawTaps{ 14, 5, 3, 1 } },
                            { Nbits{ 15 }, RawTaps{ 15, 14 } },
                            { Nbits{ 16 }, RawTaps{ 16, 14, 13, 11 } },
                            { Nbits{ 17 }, RawTaps{ 17, 14 } },
                            { Nbits{ 18 }, RawTaps{ 18, 11 } },
                            { Nbits{ 19 }, RawTaps{ 19, 6, 2, 1 } },
                            { Nbits{ 20 }, RawTaps{ 20, 17 } },
                            { Nbits{ 21 }, RawTaps{ 21, 19 } },
                            { Nbits{ 22 }, RawTaps{ 22, 21 } },
                            { Nbits{ 23 }, RawTaps{ 23, 18 } },
                            { Nbits{ 24 }, RawTaps{ 24, 23, 22, 17 } },
                            { Nbits{ 25 }, RawTaps{ 25, 22 } },
                            { Nbits{ 26 }, RawTaps{ 26, 6, 2, 1 } },
                            { Nbits{ 27 }, RawTaps{ 27, 5, 2, 1 } },
                            { Nbits{ 28 }, RawTaps{ 28, 25 } },
                            { Nbits{ 29 }, RawTaps{ 29, 27 } },
                            { Nbits{ 30 }, RawTaps{ 30, 6, 4, 1 } },
                            { Nbits{ 31 }, RawTaps{ 31, 28 } },
                            { Nbits{ 32 }, RawTaps{ 32, 22, 2, 1 } },
                            { Nbits{ 33 }, RawTaps{ 33, 2 } },
                            { Nbits{ 34 }, RawTaps{ 34, 27, 2, 1 } },
                            { Nbits{ 35 }, RawTaps{ 35, 33 } },
                            { Nbits{ 36 }, RawTaps{ 36, 25 } },
                            { Nbits{ 37 }, RawTaps{ 37, 36, 33, 31 } },
                            { Nbits{ 38 }, RawTaps{ 38, 6, 5, 1 } },
                            { Nbits{ 39 }, RawTaps{ 39, 35 } },
                            { Nbits{ 40 }, RawTaps{ 40, 38, 21, 19 } },
                            { Nbits{ 41 }, RawTaps{ 41, 38 } },
                            { Nbits{ 42 }, RawTaps{ 42, 41, 20, 19 } },
                            { Nbits{ 43 }, RawTaps{ 43, 42, 38, 37 } },
                            { Nbits{ 44 }, RawTaps{ 44, 43, 18, 17 } },
                            { Nbits{ 45 }, RawTaps{ 45, 44, 42, 41 } },
                            { Nbits{ 46 }, RawTaps{ 46, 45, 26, 25 } },
                            { Nbits{ 47 }, RawTaps{ 47, 42 } },
                            { Nbits{ 48 }, RawTaps{ 48, 47, 21, 20 } },
                            { Nbits{ 49 }, RawTaps{ 49, 4 } },
                            { Nbits{ 50 }, RawTaps{ 50, 49, 24, 23 } },
                            { Nbits{ 51 }, RawTaps{ 51, 50, 36, 35 } },
                            { Nbits{ 52 }, RawTaps{ 52, 49 } },
                            { Nbits{ 53 }, RawTaps{ 53, 52, 38, 37 } },
                            { Nbits{ 54 }, RawTaps{ 54, 53, 18, 17 } },
                            { Nbits{ 55 }, RawTaps{ 55, 31 } },
                            { Nbits{ 56 }, RawTaps{ 56, 55, 35, 34 } },
                            { Nbits{ 57 }, RawTaps{ 57, 5 } },
                            { Nbits{ 58 }, RawTaps{ 58, 39 } },
                            { Nbits{ 59 }, RawTaps{ 59, 58, 38, 37 } },
                            { Nbits{ 60 }, RawTaps{ 60, 59 } },
                            { Nbits{ 61 }, RawTaps{ 61, 60, 46, 45 } },
                            { Nbits{ 62 }, RawTaps{ 62, 61, 6, 5 } },
                            { Nbits{ 63 }, RawTaps{ 63, 62 } },
                            { Nbits{ 64 }, RawTaps{ 64, 63, 61, 60 } },
                            { Nbits{ 65 }, RawTaps{ 65, 47 } },
                            { Nbits{ 66 }, RawTaps{ 66, 65, 57, 56 } },
                            { Nbits{ 67 }, RawTaps{ 67, 66, 58, 57 } },
                            { Nbits{ 68 }, RawTaps{ 68, 59 } },
                            { Nbits{ 69 }, RawTaps{ 69, 67, 42, 40 } },
                            { Nbits{ 70 }, RawTaps{ 70, 69, 55, 54 } },
                            { Nbits{ 71 }, RawTaps{ 71, 65 } },
                            { Nbits{ 72 }, RawTaps{ 72, 66, 25, 19 } },
                            { Nbits{ 73 }, RawTaps{ 73, 48 } },
                            { Nbits{ 74 }, RawTaps{ 74, 73, 59, 58 } },
                            { Nbits{ 75 }, RawTaps{ 75, 74, 65, 64 } },
                            { Nbits{ 76 }, RawTaps{ 76, 75, 41, 40 } },
                            { Nbits{ 77 }, RawTaps{ 77, 76, 47, 46 } },
                            { Nbits{ 78 }, RawTaps{ 78, 77, 59, 58 } },
                            { Nbits{ 79 }, RawTaps{ 79, 7 } },
                            { Nbits{ 80 }, RawTaps{ 80, 79, 43, 42 } },
                            { Nbits{ 81 }, RawTaps{ 81, 77 } },
                            { Nbits{ 82 }, RawTaps{ 82, 79, 47, 44 } },
                            { Nbits{ 83 }, RawTaps{ 83, 82, 38, 37 } },
                            { Nbits{ 84 }, RawTaps{ 84, 71 } },
                            { Nbits{ 85 }, RawTaps{ 85, 84, 58, 57 } },
                            { Nbits{ 86 }, RawTaps{ 86, 85, 74, 73 } },
                            { Nbits{ 87 }, RawTaps{ 87, 74 } },
                            { Nbits{ 88 }, RawTaps{ 88, 87, 17, 16 } },
                            { Nbits{ 89 }, RawTaps{ 89, 51 } },
                            { Nbits{ 90 }, RawTaps{ 90, 89, 72, 71 } },
                            { Nbits{ 91 }, RawTaps{ 91, 90, 8, 7 } },
                            { Nbits{ 92 }, RawTaps{ 92, 91, 80, 79 } },
                            { Nbits{ 93 }, RawTaps{ 93, 91 } },
                            { Nbits{ 94 }, RawTaps{ 94, 73 } },
                            { Nbits{ 95 }, RawTaps{ 95, 84 } },
                            { Nbits{ 96 }, RawTaps{ 96, 94, 49, 47 } },
                            { Nbits{ 97 }, RawTaps{ 97, 91 } },
                            { Nbits{ 98 }, RawTaps{ 98, 87 } },
                            { Nbits{ 99 }, RawTaps{ 99, 97, 54, 52 } },
                            { Nbits{ 100 }, RawTaps{ 100, 63 } },
                            { Nbits{ 101 }, RawTaps{ 101, 100, 95, 94 } },
                            { Nbits{ 102 }, RawTaps{ 102, 101, 36, 35 } },
                            { Nbits{ 103 }, RawTaps{ 103, 94 } },
                            { Nbits{ 104 }, RawTaps{ 104, 103, 94, 93 } },
                            { Nbits{ 105 }, RawTaps{ 105, 89 } },
                            { Nbits{ 106 }, RawTaps{ 106, 91 } },
                            { Nbits{ 107 }, RawTaps{ 107, 105, 44, 42 } },
                            { Nbits{ 108 }, RawTaps{ 108, 77 } },
                            { Nbits{ 109 }, RawTaps{ 109, 108, 103, 102 } },
                            { Nbits{ 110 }, RawTaps{ 110, 109, 98, 97 } },
                            { Nbits{ 111 }, RawTaps{ 111, 101 } },
                            { Nbits{ 112 }, RawTaps{ 112, 110, 69, 67 } },
                            { Nbits{ 113 }, RawTaps{ 113, 104 } },
                            { Nbits{ 114 }, RawTaps{ 114, 113, 33, 32 } },
                            { Nbits{ 115 }, RawTaps{ 115, 114, 101, 100 } },
                            { Nbits{ 116 }, RawTaps{ 116, 115, 46, 45 } },
                            { Nbits{ 117 }, RawTaps{ 117, 115, 99, 97 } },
                            { Nbits{ 118 }, RawTaps{ 118, 85 } },
                            { Nbits{ 119 }, RawTaps{ 119, 111 } },
                            { Nbits{ 120 }, RawTaps{ 120, 113, 9, 2 } },
                            { Nbits{ 121 }, RawTaps{ 121, 103 } },
                            { Nbits{ 122 }, RawTaps{ 122, 121, 63, 62 } },
                            { Nbits{ 123 }, RawTaps{ 123, 121 } },
                            { Nbits{ 124 }, RawTaps{ 124, 87 } },
                            { Nbits{ 125 }, RawTaps{ 125, 124, 18, 17 } },
                            { Nbits{ 126 }, RawTaps{ 126, 125, 90, 89 } },
                            { Nbits{ 127 }, RawTaps{ 127, 126 } },
                            { Nbits{ 128 }, RawTaps{ 128, 126, 101, 99 } },
                            { Nbits{ 129 }, RawTaps{ 129, 124 } },
                            { Nbits{ 130 }, RawTaps{ 130, 127 } },
                            { Nbits{ 131 }, RawTaps{ 131, 130, 84, 83 } },
                            { Nbits{ 132 }, RawTaps{ 132, 103 } },
                            { Nbits{ 133 }, RawTaps{ 133, 132, 82, 81 } },
                            { Nbits{ 134 }, RawTaps{ 134, 77 } },
                            { Nbits{ 135 }, RawTaps{ 135, 124 } },
                            { Nbits{ 136 }, RawTaps{ 136, 135, 11, 10 } },
                            { Nbits{ 137 }, RawTaps{ 137, 116 } },
                            { Nbits{ 138 }, RawTaps{ 138, 137, 131, 130 } },
                            { Nbits{ 139 }, RawTaps{ 139, 136, 134, 131 } },
                            { Nbits{ 140 }, RawTaps{ 140, 111 } },
                            { Nbits{ 141 }, RawTaps{ 141, 140, 110, 109 } },
                            { Nbits{ 142 }, RawTaps{ 142, 121 } },
                            { Nbits{ 143 }, RawTaps{ 143, 142, 123, 122 } },
                            { Nbits{ 144 }, RawTaps{ 144, 143, 75, 74 } },
                            { Nbits{ 145 }, RawTaps{ 145, 93 } },
                            { Nbits{ 146 }, RawTaps{ 146, 145, 87, 86 } },
                            { Nbits{ 147 }, RawTaps{ 147, 146, 110, 109 } },
                            { Nbits{ 148 }, RawTaps{ 148, 121 } },
                            { Nbits{ 149 }, RawTaps{ 149, 148, 40, 39 } },
                            { Nbits{ 150 }, RawTaps{ 150, 97 } },
                            { Nbits{ 151 }, RawTaps{ 151, 148 } },
                            { Nbits{ 152 }, RawTaps{ 152, 151, 87, 86 } },
                            { Nbits{ 153 }, RawTaps{ 153, 152 } },
                            { Nbits{ 154 }, RawTaps{ 154, 152, 27, 25 } },
                            { Nbits{ 155 }, RawTaps{ 155, 154, 124, 123 } },
                            { Nbits{ 156 }, RawTaps{ 156, 155, 41, 40 } },
                            { Nbits{ 157 }, RawTaps{ 157, 156, 131, 130 } },
                            { Nbits{ 158 }, RawTaps{ 158, 157, 132, 131 } },
                            { Nbits{ 159 }, RawTaps{ 159, 128 } },
                            { Nbits{ 160 }, RawTaps{ 160, 159, 142, 141 } },
                            { Nbits{ 161 }, RawTaps{ 161, 143 } },
                            { Nbits{ 162 }, RawTaps{ 162, 161, 75, 74 } },
                            { Nbits{ 163 }, RawTaps{ 163, 162, 104, 103 } },
                            { Nbits{ 164 }, RawTaps{ 164, 163, 151, 150 } },
                            { Nbits{ 165 }, RawTaps{ 165, 164, 135, 134 } },
                            { Nbits{ 166 }, RawTaps{ 166, 165, 128, 127 } },
                            { Nbits{ 167 }, RawTaps{ 167, 161 } },
                            { Nbits{ 168 }, RawTaps{ 168, 166, 153, 151 } } };

  for (const auto& [k, v] : data) {
    if (k.bits == N) {
      return v;
    }
  }
  throw "unsupported value of nbits";
}
}

/**
 * given a LSFR size N, return an std::index_sequece with the taps in falling
 * order, as is customary when working with LSFR.
 */
template<int N>
constexpr auto
getTaps()
{
  constexpr auto rawtaps = detail::getTapsImpl<N>();
  static_assert(rawtaps[0] <= N);
  static_assert(rawtaps[0] != 0);
  static_assert(rawtaps[1] != 0);
  static_assert(rawtaps[0] > rawtaps[1]);
  if constexpr (rawtaps[2] == 0) {
    static_assert(rawtaps[3] == 0);
    return std::index_sequence<rawtaps[0], rawtaps[1]>{};
  } else {
    static_assert(rawtaps[2] != 0);
    static_assert(rawtaps[1] > rawtaps[2]);
    static_assert(rawtaps[3] != 0);
    static_assert(rawtaps[2] > rawtaps[3]);
    return std::
      index_sequence<rawtaps[0], rawtaps[1], rawtaps[2], rawtaps[3]>{};
  }
}
