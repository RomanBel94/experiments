#include <format>
#include <iostream>

class Dummy
{
    int val{0};

public:
    Dummy(int val) : val(val)
    {
        std::cout << std::format("Dummy created with val {}\n", val);
    }

    ~Dummy() { std::cout << std::format("Dummy with val {} destroyed\n", val); }
};

int main()
{
    alignas(Dummy) unsigned char buf[sizeof(Dummy)];
    std::cout << std::format("Buffer created with size: {}\n", sizeof(buf));

    Dummy* ptr{nullptr};

    ptr = new (buf) Dummy(5);
    ptr->~Dummy();
    ptr = nullptr;

    ptr = new (buf) Dummy(50);
    ptr->~Dummy();
    ptr = nullptr;

    return EXIT_SUCCESS;
}
