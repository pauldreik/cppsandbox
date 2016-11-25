/*
 * Playing around with parallelizing std::partial_sum
 * by Paul Dreik https://www.pauldreik.se/
 * LICENSE: http://www.boost.org/LICENSE_1_0.txt
 */
#include "include/parpartial.h"

#include <chrono>
#include <iostream>
#include <vector>

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
  case 1:
    // uses my implementation with threads
    whatever::par_partial_sum(input.cbegin(), input.cbegin() + Ndata,
                              output.begin(), options);
    break;

  case 2:
    // uses std implementation, singlethreaded
    std::partial_sum(input.cbegin(), input.cbegin() + Ndata, output.begin());
    break;

  case 3:
    // my implementation, with std::async instead of explicit threads.
    whatever::par_partial_sum_async(input.cbegin(), input.cbegin() + Ndata,
                                    output.begin(), Nthreads);
    break;

  case 4:
    // uses my implementation with threads, tweaked a bit
    whatever::par_partial_sum_v2(input.cbegin(), input.cbegin() + Ndata,
                                 output.begin(), options);
    break;

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
  std::cout << algo << '\t' << Nruns << '\t' << Nthreads << '\t' << Ndata
            << '\t' << ns_per_element << '\n';
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

  // Start of benchmarking
  std::cout << "algo\tnruns\tnthreads\tndata\tnsperelement\n";
  for (const auto algo : {1, 2, 3, 4}) {
    for (const auto Nblock : {100'000, 1'000'000, 10'000'000, 20'000'000,
                              40'000'000, 160'000'000}) {

      for (const auto Nthreads : {1, 2, 4, 8}) {
        runCase(Nblock, Nthreads, input, output, algo);
      }
    }
  }
}
