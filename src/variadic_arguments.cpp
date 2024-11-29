#include <iostream>

template<typename func, typename... Args>
void functionCaller(func&& f, Args&& ... args)
{
    f(args...);
}

void printNum(int num)
{
    std::cout << num << std::endl;
}

void printSum(int a, int b)
{
    std::cout << a + b << std::endl;
}

void foo(int a, int b, int c, int d)
{
    std::cout << "foo" << std::endl;
}

void bar()
{
    std::cout << "bar" << std::endl;
}

int main()
{
    functionCaller(printNum, 5);
    functionCaller(printSum, 5, 6);
    functionCaller(foo, 1, 2, 3, 4);
    functionCaller(bar);

    return 0;;
}
