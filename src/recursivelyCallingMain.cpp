#include <iostream>

void mainCaller(int (*f)())
{
    std::cout << "mainCaller" << std::endl;
    f();
}

int main()
{
    std::cout << "main" << std::endl;
    mainCaller(main);

    return 0;
}
