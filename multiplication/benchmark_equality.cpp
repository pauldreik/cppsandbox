#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <random>

#include "multiplication.h"

namespace {
bool
reference_products_equal(const unsigned a,
                         const unsigned b,
                         const unsigned c,
                         const unsigned d)
{
  using U = std::uint64_t;
  return U{ a } * b == U{ c } * d;
}
}

TEST_CASE("Benchmark equality", "[!benchmark]")
{
  BENCHMARK_ADVANCED("64 bit multiplication")(
    Catch::Benchmark::Chronometer meter)
  {
    // prevent optimization by getting these at runtime
    std::random_device rd;

    meter.measure([=, &rd](unsigned int i) {
      const unsigned b = rd();
      const unsigned c = rd();
      const unsigned d = rd();
      return reference_products_equal(i, b, c, d);
    });
  };
  BENCHMARK_ADVANCED("chinese remainder")(Catch::Benchmark::Chronometer meter)
  {
    // prevent optimization by getting these at runtime
    std::random_device rd;

    meter.measure([=, &rd](unsigned int i) {
      const unsigned b = rd();
      const unsigned c = rd();
      const unsigned d = rd();
      return products_equal(i, b, c, d);
    });
  };
}
