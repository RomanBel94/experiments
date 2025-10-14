#include "optional.hpp"
#include <iostream>
#include <string>

int main()
{
    try
    {
        my::optional<int> obj;
        my::optional<int> obj2{my::nullopt};

        obj.emplace(42);
        std::cout << *obj << '\n';

        if (obj2)
            std::cout << *obj2 << '\n';

        std::cout << std::hash<int>::operator()(obj) << '\n';
        std::cout << std::boolalpha << (obj != my::nullopt) << ' '
                  << (obj2 == my::nullopt) << '\n';

        my::optional<std::string> opt_string{"Hello"};
        std::cout << opt_string->size() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}