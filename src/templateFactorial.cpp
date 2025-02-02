#include <iostream>

template <unsigned N>
struct Factorial
{
    static constexpr unsigned num = Factorial<N - 1>().num * N;
};

template <>
struct Factorial<0>
{
    static constexpr unsigned int num = 1;
};

int main()
{
    static_assert(Factorial<5>::num == 120, "error in Factorial<5> == 120");
    std::cout << Factorial<5>().num << std::endl;
    return 0;
}
