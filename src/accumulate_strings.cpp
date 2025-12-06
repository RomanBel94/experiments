#include <array>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>

int main()
{
    std::array<std::string, 3> strings{"Hello ", "World", "!"};
    std::string result = std::accumulate(strings.cbegin(), strings.cend(), "",
                                         std::plus<std::string>());

    std::cout << result << std::endl;

    std::array<std::string, 5> strings2{"Hello", "world", "one", "two",
                                        "three"};
    std::string result2 =
        std::accumulate(strings2.cbegin(), strings2.cend(), "",
                        [](const std::string& lhs, const std::string& rhs)
                        { return lhs + rhs + " "; });

    std::cout << result2 << std::endl;

    return EXIT_SUCCESS;
}
