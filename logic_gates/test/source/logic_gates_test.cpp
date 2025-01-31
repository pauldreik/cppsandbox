#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "logic_gates" ? 0 : 1;
}
