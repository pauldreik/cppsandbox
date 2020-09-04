#include <iostream>

#include <cstdint>
#include <cmath>

template<class Rep>
struct Floating {
    Rep mantissa;
    int exponent;
    void print() const {
        std::cout<<mantissa<<'*'<<"2^"<<exponent<<'\n';
    }
    static Floating FromInt(std::int64_t x) {
        // if x is intmin, shift it
        return {x,0};
    }
    static Floating FromDouble(double x) {
        int exp;
        double fraction=std::frexp (x, &exp );
        // |fraction| is in [0.5, 1), get it into mantissa with as many digits preserved as possible
        // if Rep is 64 bit, multiply with 2^63 and truncate towards zero
        const int shift=63;
        Rep mantissa=static_cast<Rep>(std::ldexp(fraction,shift));
        int exponent=exp-shift;
        return {mantissa,exponent};
    }
    double toDouble() {
        return std::ldexp(mantissa,exponent);
    }
};

template<class Rep>
Floating<Rep> operator*(Floating<Rep> a, Floating<Rep> b) {
    const int aisneg=a.mantissa<0;
    const int bisneg=b.mantissa<0;
    Rep aabs=aisneg ? -a.mantissa : a.mantissa;
    Rep babs=bisneg ? -b.mantissa : b.mantissa;
    Rep ahi=aabs>>32;
    Rep alo=(aabs<<32)>>32;
    Rep bhi=babs>>32;
    Rep blo=(babs<<32)>>32;
    Rep res=ahi*bhi+ahi*alo+alo*bhi+alo*blo;
    const bool resultisneg= (aisneg ^ bisneg);
    return {resultisneg?-res:res,a.exponent+b.exponent};
}

void doit(double x) {
    auto f=Floating<std::int64_t>::FromDouble(x);
    f.print();
    std::cout<<"reconstructed: "<<f.toDouble()<<'\n';
}

int main()
{
doit(-1);
using F=Floating<std::int64_t>;
auto one=F::FromInt(1);
auto two=F::FromInt(2);
one.print();
two.print();
(one*two).print();
}
