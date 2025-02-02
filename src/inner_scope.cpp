#include <ctime>
#include <iostream>

class A
{
public:
    A() { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
};

class B
{
public:
    B() { std::cout << "B()\n"; }
    ~B() { std::cout << "~B()\n"; }
};

int main()
{
    A a;
    {
        B b;
    }
    A a2;

    return 0;
}
