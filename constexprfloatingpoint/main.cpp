#include <iostream>

#include <cstdint>
#include <cmath>
#include <cassert>
#include <type_traits>

#include "FloatingPoint.hpp"

void doit(double x) {
    auto f=Floating::FromDouble(x);
    f.print();
    std::cout<<"reconstructed: "<<f.toDouble()<<'\n';
}

int main()
{
    doit(-1);
    using F=Floating;
    constexpr auto one=F::FromInt(1);
    constexpr auto two=F::FromInt(2);
    one.print();
    two.print();
    (one*two).print();
    static_assert(one+two==one*two+one);
}
