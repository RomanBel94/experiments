#include <iostream>

template<unsigned N>
struct Factorial
{
    static constexpr unsigned num = Factorial<N - 1>().num * N;
};

template<>
struct Factorial<0>
{
    static constexpr unsigned int num = 1;
};

int main()
{
    std::cout << Factorial<5>().num << std::endl;
    return 0;
}
