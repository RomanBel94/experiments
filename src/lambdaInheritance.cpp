#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>


template <typename... Lambdas>
struct Visitor : Lambdas...
{
    Visitor(Lambdas... lambdas) : Lambdas(std::forward<Lambdas>(lambdas))... {}
    using Lambdas::operator()...;
};

using var_t = std::variant<int, double, std::string>;

int main()
{
    std::vector<var_t> vec = {10, 1.5, "hello"};
    std::for_each(vec.begin(), vec.end(),
                  [](const auto& v)
                  {
                      std::visit(
                          Visitor{[](int arg) { std::cout << arg << ' '; },
                                  [](double arg) { std::cout << arg << ' '; },
                                  [](const std::string& arg)
                                  { std::cout << std::quoted(arg) << ' '; }},
                          v);
                  });

    Visitor vis{[](int) { std::cout << "\nvis.operator(int);"; },
            [](double) { std::cout << "\nvis.operator(double);"; }};

    vis(1);
    vis(1.0);

    return 0;
}
