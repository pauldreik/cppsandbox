#include <cstdint>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "lib.hpp"

enum class operation
{
  unknown,
  input,
  invert,
  and_gate,
  or_gate
};

std::string_view to_string(operation o)
{
  switch (o) {
    case operation::unknown:
      return "unknown";
    case operation::input:
      return "input";
    case operation::invert:
      return "invert";
    case operation::and_gate:
      return "and_gate";
    case operation::or_gate:
      return "or_gate";
    default:
      return "???";
  }
}

struct result
{
  // key for the input used to create this element
  std::uint32_t in1 {};
  std::uint32_t in2 {};
  operation m_operation = operation::unknown;
  std::uint32_t out {};
  int steps = 0;
};

using map_t = std::map<std::uint32_t, result>;

void print(const map_t& m)
{
  for (const auto& [key, value] : m) {
    std::cout << "from " << value.in1 << " " << to_string(value.m_operation)
              << " " << value.in2 << ": got " << value.out << '\n';
  }
}

void operate(map_t& data, const operation o)
{
  map_t new_data = data;
  for (const auto& [key1, value1] : data) {
    for (const auto& [key2, value2] : data) {
      if (key2 <= key1) {
        continue;
      }
      // try: key1 op key2
      std::uint32_t newvalue {};
      switch (o) {
        case operation::and_gate:
          newvalue = value1.out bitand value2.out;
          break;
        case operation::or_gate:
          newvalue = value1.out bitor value2.out;
          break;
        default:
          throw std::runtime_error("unhandled operation");
      }
      const auto steps = 1 + std::max(value1.steps, value2.steps);
      if (data.contains(newvalue)) {
        std::cout << "value " << newvalue << " already existed with steps "
                  << data[newvalue].steps << " instead of " << steps << "\n";
      } else {
        std::cout << "value " << newvalue << " found by combining "
                  << value1.out << " " << to_string(o) << " " << value2.out
                  << " with steps=" << steps << "\n";

        new_data.emplace(newvalue,
                         result {.in1 = value1.out,
                                 .in2 = value2.out,
                                 .m_operation = o,
                                 .out = newvalue});
      }
    }
  }
  data = new_data;
}

int main()
{
  map_t data;

  // we have three inputs
  for (unsigned int i = 0; i < 8u; ++i) {
    data.emplace(
        i,
        result {.in1 = i, .in2 = i, .m_operation = operation::input, .out = i});
  }
  std::cout << "after init:\n";
  print(data);

  operate(data, operation::and_gate);
  print(data);
}
