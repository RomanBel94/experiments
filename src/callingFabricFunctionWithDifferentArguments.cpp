#include <iostream>
#include <memory>

class Foo
{
    int num;

public:
    Foo(int n) : num(n) {}

    static auto get_instance(int n = 0) -> std::shared_ptr<Foo>
    {
        static auto ptr = std::make_shared<Foo>(n);
        return ptr;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Foo& foo)
    {
        stream << foo.num;
        return stream;
    }
};

int main()
{
    auto sptr1 = Foo::get_instance(5);
    auto sptr2 = Foo::get_instance();

    std::cout << "sptr1: " << *sptr1 << std::endl;
    std::cout << "sptr2: " << *sptr2 << std::endl;
}
