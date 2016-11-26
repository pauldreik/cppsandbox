#ifndef PARPARTIAL_HH
#define PARPARTIAL_HH
/*
 * Playing around with parallelizing std::partial_sum
 * by Paul Dreik https://www.pauldreik.se/
 * LICENSE: http://www.boost.org/LICENSE_1_0.txt
 */
#include <array>
#include <cassert>
#include <future>   //for async
#include <iterator> //for iterator traits
#include <numeric>
#include <thread>
#include <type_traits> // for is_same
#include <vector>

namespace whatever {

struct Options {
  std::size_t Nthreads = 4;
  bool execute_in_threads = false;
};

/**
 * extra crappy helper class to simplify the threading code
 */
struct ThreadGroup {
  explicit ThreadGroup(std::size_t Nthreads) : m_threads(Nthreads){};
  /// for debugging - runs blocking instead of in threads
  void runSync() { m_execute_in_threads = false; }
  /// does job c on the i:th thread
  template <typename Callable> void operator()(std::size_t i, Callable c) {
    if (m_execute_in_threads) {
      m_threads.at(i) = std::thread(c);
    } else {
      c();
    }
  }

  void joinAll() {
    if (m_execute_in_threads) {
      for (auto &t : m_threads) {
        if (t.joinable()) {
          t.join();
        }
      }
    }
  }

private:
  std::vector<std::thread> m_threads;
  bool m_execute_in_threads = true;
};

/**
 * helper function to reduce the amount of code elsewhere
 * @param
 * @param
 * @param
 */
template <class InputIt, class OutputIt>
void validateArgumentTypes(InputIt, InputIt, OutputIt) {
  // make sure we got passed random access iterators
  static_assert(
      std::is_same<
          std::random_access_iterator_tag,
          typename std::iterator_traits<InputIt>::iterator_category>::value,
      "input iterator must be random access");

  static_assert(
      std::is_same<
          std::random_access_iterator_tag,
          typename std::iterator_traits<OutputIt>::iterator_category>::value,
      "output iterator must be random access");
}

/**
 * like std::partial_sum, but summation starts at v, not 0.
 * @param first
 * @param last
 * @param d_first
 * @param v initial value
 * @return
 */
template <class InputIt, class OutputIt, class ValueType>
OutputIt partial_sum_from_value(InputIt first, InputIt last, OutputIt d_first,
                                ValueType v) {
  validateArgumentTypes(first, last, d_first);

  if (first == last) {
    return d_first;
  }

  v += *first;
  *d_first = v;
  while (++first != last) {
    v += *first;
    *++d_first = v;
  }
  return ++d_first;
}

/**
 * like std::partial_sum, but executes in parallel.
 * @param first
 * @param last
 * @param d_first
 * @param options
 * @return
 */
template <class InputIt, class OutputIt>
OutputIt par_partial_sum(InputIt first, InputIt last, OutputIt d_first,
                         const Options options = whatever::Options()) {

  // make sure we got passed random access iterators
  validateArgumentTypes(first, last, d_first);

  // the value type
  using value_t = std::decay_t<decltype(*first)>;

  // how many threads are we going to use?
  const std::size_t Nthreads = options.Nthreads;

  // number of data points
  assert(first <= last && "your input sequence is weird");
  const auto N = std::distance(first, last);

  // use a linear algorithm for small data sets. this simplifies the
  // implementation because it avoids empty ranges.
  if (static_cast<std::size_t>(N) <= Nthreads) {
    return std::partial_sum(first, last, d_first);
  }

  // divide the input into one range per thread
  std::vector<InputIt> ranges;
  const std::size_t step = N / Nthreads;
  assert(step > 0);

  // loop Nthreads times instead
  for (std::size_t i = 0; i < Nthreads; ++i) {
    ranges.push_back(first + step * i);
  }
  // add an end marker for the last thread
  ranges.push_back(last);
  assert(ranges.size() == Nthreads + 1);

  // for each range, calculate the sum
  std::vector<value_t> sums(Nthreads);

  ThreadGroup threads(Nthreads);
  // for debugging - execute sync instead of in the threads
  if (!options.execute_in_threads) {
    threads.runSync();
  }

  for (std::size_t i = 0; i < Nthreads; ++i) {
    auto task = [&, i]() {
      sums.at(i) = std::accumulate(ranges.at(i), ranges.at(i + 1), value_t{});
    };
    threads(i, task);
  }

  threads.joinAll();

  // calculating the starting values for the partial sums (one per thread)
  std::vector<value_t> starting_values(Nthreads);
  std::partial_sum(std::cbegin(sums), std::cend(sums) - 1,
                   starting_values.begin() + 1);

  // now call partial_sum on each part, starting from the precomputed value
  for (std::size_t i = 0; i < Nthreads; ++i) {
    auto task = [&, i]() {
      partial_sum_from_value(ranges.at(i), ranges.at(i + 1),
                             d_first + (ranges.at(i) - first),
                             starting_values.at(i));
    };
    threads(i, task);
  }

  threads.joinAll();

  return d_first + (last - first);
}
/******************************************************************************/

/**
 * like par_partial_sum, but tweaked a bit
 * @param first
 * @param last
 * @param d_first
 * @param options
 * @return
 */
template <class InputIt, class OutputIt>
OutputIt par_partial_sum_v2(InputIt first, InputIt last, OutputIt d_first,
                            const Options options = whatever::Options()) {

  // make sure we got passed random access iterators
  validateArgumentTypes(first, last, d_first);

  // the value type
  using value_t = std::decay_t<decltype(*first)>;

  // how many threads are we going to use?
  const std::size_t Nthreads = options.Nthreads;

  // number of data points
  assert(first <= last && "your input sequence is weird");
  const auto N = std::distance(first, last);

  // use a linear algorithm for small data sets. this simplifies the
  // implementation because it avoids empty ranges.
  if (Nthreads <= 1 || static_cast<std::size_t>(N) <= Nthreads) {
    return std::partial_sum(first, last, d_first);
  }

  // divide the input into large blocks.
  const auto Nblocks = Nthreads;
  std::vector<InputIt> ranges;
  const std::size_t step = N / Nblocks;
  assert(step > 0);

  for (std::size_t i = 0; i < Nthreads; ++i) {
    ranges.push_back(first + step * i);
  }
  // add an end marker for the last thread
  ranges.push_back(last);
  assert(ranges.size() == Nthreads + 1);

  // for each range, calculate the sum
  std::vector<value_t> sums(Nthreads);

  ThreadGroup threads(Nthreads);
  // for debugging - execute sync instead of in the threads
  if (!options.execute_in_threads) {
    threads.runSync();
  }

  // for the first block, calculate the sum in this thread. for the others,
  // calculate the sum, ideally being finished at the same time.
  for (std::size_t i = Nthreads - 1; i < Nthreads;
       --i) { //<--sorry for the intended wraparound trick
    if (i == 0) {
      // first block. run in this thread.
      auto ret = std::partial_sum(ranges.at(0), ranges.at(1), d_first);
      sums.at(0) = ret[-1];
    } else {
      auto task = [&, i]() {
        sums.at(i) = std::accumulate(ranges.at(i), ranges.at(i + 1), value_t{});
      };
      threads(i, task);
    }
  }

  threads.joinAll();

  // calculating the starting values for the partial sums (one per thread)
  std::vector<value_t> starting_values(Nthreads);
  std::partial_sum(std::cbegin(sums), std::cend(sums) - 1,
                   starting_values.begin() + 1);

  // now call partial_sum on each part, starting from the precomputed value
  for (std::size_t i = 0; i < Nthreads; ++i) {
    if (i > 0) {
      auto task = [&, i]() {
        partial_sum_from_value(ranges.at(i), ranges.at(i + 1),
                               d_first + (ranges.at(i) - first),
                               starting_values.at(i));
      };
      threads(i, task);
    }
  }

  threads.joinAll();

  return d_first + (last - first);
}
/******************************************************************************/

/**
 * like std::partial_sum, but executes in parallel using std::async
 * @param first
 * @param last
 * @param d_first
 * @param options
 * @return
 */
template <class InputIt, class OutputIt>
OutputIt par_partial_sum_async(InputIt first, InputIt last, OutputIt d_first,
                               std::size_t Nblocks = 4) {

  // make sure we got passed random access iterators
  validateArgumentTypes(first, last, d_first);

  // the value type
  using value_t = std::decay_t<decltype(*first)>;

  // this is the block size of calculation
  const auto Nthreshold = 10000;

  // how many blocks to divide the work into
  // const auto Nblocks=4;

  // number of data points
  assert(first <= last && "your input sequence is weird");
  const auto N = std::distance(first, last);

  // use a linear algorithm for small data sets. this simplifies the
  // implementation because it avoids empty ranges etc.
  if (N <= Nthreshold) {
    return std::partial_sum(first, last, d_first);
  }

  // divide the input into multiple ranges
  std::vector<InputIt> ranges;
  const std::size_t step = N / Nblocks;
  assert(step > 0);

  for (std::size_t i = 0; i < Nblocks; ++i) {
    ranges.push_back(first + step * i);
  }
  // add an end marker for the last block
  ranges.push_back(last);
  assert(ranges.size() == Nblocks + 1);

  // for each range, calculate the sum
  std::vector<value_t> sums(Nblocks);

  std::vector<std::future<void>> tasks;
  tasks.reserve(Nblocks);
  for (std::size_t i = 0; i < Nblocks; ++i) {
    auto task = [&, i]() {
      sums.at(i) = std::accumulate(ranges.at(i), ranges.at(i + 1), value_t{});
    };
    tasks.emplace_back(std::async(std::launch::async, task));
  }
  for (std::size_t i = 0; i < Nblocks; ++i) {
    tasks.at(i).get();
  }

  // calculating the starting values for the partial sums (one per thread)
  std::vector<value_t> starting_values(Nblocks);
  std::partial_sum(std::cbegin(sums), std::cend(sums) - 1,
                   starting_values.begin() + 1);

  // now call partial_sum on each part, starting from the precomputed value
  for (std::size_t i = 0; i < Nblocks; ++i) {
    auto task = [&, i]() {
      partial_sum_from_value(ranges.at(i), ranges.at(i + 1),
                             d_first + (ranges.at(i) - first),
                             starting_values.at(i));
    };
    tasks.at(i) = std::async(std::launch::async, task);
  }
  for (std::size_t i = 0; i < Nblocks; ++i) {
    tasks.at(i).get();
  }

  return d_first + (last - first);
}
}

#endif
