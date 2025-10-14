#include "optional.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main()
{
    my::optional<int> op_int1, op_int2(42), op_int3{my::nullopt};
    my::optional<std::string> op_string1{my::nullopt}, op_string2,
        op_string3{"Hello"};

    assert(op_int1 == my::nullopt);
    assert(op_int2 == 42);
    assert(!op_int3.has_value());

    std::clog << "All asserts passed\n";
    return EXIT_SUCCESS;
}