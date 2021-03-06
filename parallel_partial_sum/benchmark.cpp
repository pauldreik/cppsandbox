/*
 * Playing around with parallelizing std::partial_sum
 * by Paul Dreik https://www.pauldreik.se/
 * LICENSE: http://www.boost.org/LICENSE_1_0.txt
 */
#include "include/parpartial.h"

#include <chrono>
#include <iostream>
#include <vector>

// gnu parallel stl - see
// https://gcc.gnu.org/onlinedocs/libstdc++/manual/parallel_mode_using.html
#if defined(__GNUC__)
#include <parallel/numeric>
#endif

// is fast math enabled?
constexpr char isFastMathEnabled() {
#if defined(__GNUC__) || defined(__clang__)
#ifdef __FAST_MATH__
  return 'y';
#else
  return 'n';
#endif
#endif
  return '?';
}
enum AlgoName {
  MY_PARTIAL_SUM = 1,
  STD_PARTIAL_SUM,
  MY_PARTIAL_SUM_STDASYNC,
  MY_PARTIAL_SUM_V2,
  GNU_PARALLEL_PARTIAL_SUM,
};

// function for knowing which of the algorithms need a number of threads/block
// size hint.
static constexpr bool needsBlockSize(const AlgoName algo) {
  return (algo == MY_PARTIAL_SUM || algo == MY_PARTIAL_SUM_STDASYNC ||
          algo == MY_PARTIAL_SUM_V2);
}

template <typename Numeric>
void doBenchmarkCase(const std::size_t Ndata, const std::size_t Nthreads,
                     const std::vector<Numeric> &input,
                     std::vector<Numeric> &output, const int algo) {
  // make sure arrays are valid
  output.at(Ndata - 1) = input.at(Ndata - 1);

  whatever::Options options;
  options.Nthreads = Nthreads;
  options.execute_in_threads = true;
  switch (algo) {
  case AlgoName::MY_PARTIAL_SUM:
    // uses my implementation with threads
    whatever::par_partial_sum(input.cbegin(), input.cbegin() + Ndata,
                              output.begin(), options);
    break;

  case AlgoName::STD_PARTIAL_SUM:
    // uses std implementation, singlethreaded
    std::partial_sum(input.cbegin(), input.cbegin() + Ndata, output.begin());
    break;

  case AlgoName::MY_PARTIAL_SUM_STDASYNC:
    // my implementation, with std::async instead of explicit threads.
    whatever::par_partial_sum_async(input.cbegin(), input.cbegin() + Ndata,
                                    output.begin(), Nthreads);
    break;

  case AlgoName::MY_PARTIAL_SUM_V2:
    // uses my implementation with threads, tweaked a bit
    whatever::par_partial_sum_v2(input.cbegin(), input.cbegin() + Ndata,
                                 output.begin(), options);
    break;

#if defined(__GNUC__)
  case AlgoName::GNU_PARALLEL_PARTIAL_SUM:
    // gnu parallel
    __gnu_parallel::partial_sum(input.cbegin(), input.cbegin() + Ndata,
                                output.begin());
    break;
#endif

  default:
    throw "invalid algo";
  }
}

template <class Duration> double toSeconds(const Duration &d) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(d).count();
}

// runs a single case multiple times
template <typename Numeric>
void runCase(const std::size_t Ndata, const std::size_t Nthreads,
             const std::vector<Numeric> &input, std::vector<Numeric> &output,
             const int algo) {
  const auto Nruns = input.size() / Ndata;
  const auto t1 = std::chrono::steady_clock::now();
  for (std::size_t i = 0; i < Nruns; ++i) {
    doBenchmarkCase(Ndata, Nthreads, input, output, algo);
    (void)&output.back();
  }
  const auto t2 = std::chrono::steady_clock::now();
  auto Nvaluesrun = Nruns * Ndata;

  // ns per element
  const auto ns_per_element = toSeconds(t2 - t1) * 1e9 / Nvaluesrun;
  std::cout << algo << ',' << Nruns << ',' << Nthreads << ',' << Ndata << ','
            << ns_per_element << ',' << isFastMathEnabled() << '\n';
  std::cout.flush();
}

int main() {

  std::vector<float> input;
  const std::size_t N = 160'000'000;
  // fill with garbage
  input.resize(N);
  // touch the memory
  // const auto t0 = std::chrono::steady_clock::now();
  for (auto &e : input) {
    e = 1;
  }
  // also allocate results
  auto output = input;

  // which algos to test
  const auto algos = {
    MY_PARTIAL_SUM,
    STD_PARTIAL_SUM,
    MY_PARTIAL_SUM_STDASYNC,
    MY_PARTIAL_SUM_V2,
#if defined(__GNUC__)
    GNU_PARALLEL_PARTIAL_SUM,
#endif
  };

  // Start of benchmarking
  std::cout << "algo,nruns,nthreads,ndata,nsperelement,fastmath\n";
  for (const auto algo : algos) {
    for (const auto Nblock : {100'000, 1'000'000, 10'000'000, 20'000'000,
                              40'000'000, 160'000'000}) {
      if (needsBlockSize(algo)) {
        for (const auto Nthreads : {1, 2, 4, 8}) {
          runCase(Nblock, Nthreads, input, output, algo);
        }
      } else {
        runCase(Nblock, 1, input, output, algo);
      }
    }
  }
}
