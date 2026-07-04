#include "optional.hpp"
#include <cassert>
#include <string>
#include <vector>

int main()
{
    my::optional<int> op_int1, op_int2(42), op_int3{my::nullopt};
    my::optional<std::string> op_string1{my::nullopt}, op_string2,
        op_string3{"Hello"};

    assert(op_int1 == my::nullopt);
    assert(*op_int2 == 42);
    assert(!op_int3.has_value());

    assert(!op_string1);
    assert(!op_string2.has_value());
    assert(op_string3.value() == "Hello");

    my::optional<int> opt_int;
    assert(!opt_int);
    opt_int = 5;
    assert(opt_int == 5);
    opt_int.reset();
    assert(!opt_int.has_value());

    my::optional opt_float{42.f};
    assert(opt_float);
    opt_float = 5.3f;
    assert(opt_float == 5.3f);
    opt_float.reset();
    assert(!opt_int.has_value());

    my::optional<std::vector<int>> opt_vec{{1, 2, 3, 4, 5}};
    opt_vec->push_back(6);
    assert(opt_vec.value()[5] == 6);

    std::clog << "\e[1;32mAll asserts passed\n\e[0m";
    return EXIT_SUCCESS;
}
