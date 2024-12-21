#include <algorithm>
#include <array>
#include <print>
#include <ranges>

// see https://proofwiki.org/wiki/Polydivisible_Number/Examples/381,654,729

int main() {

  std::array numbers{1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::size_t count = 0;

  do {
    ++count;

    auto partialnumber = 0;
    bool good = true;
    for (auto const [i, digit] : std::views::enumerate(numbers)) {
      partialnumber = partialnumber * 10 + digit;
      if (const auto divider = i + 1; partialnumber % divider) {
        good = false;
      }
    }

    if (good) {
      std::println("found one after {} tries!", count);
      for (const auto d : numbers) {
        std::print(" {}", d);
      }
      std::println();
    }
  } while (std::ranges::next_permutation(numbers).found);

  std::println("found {} candidates, expected {}", count,
               std::ranges::fold_left(numbers, 1, std::multiplies<int>{}));
}
