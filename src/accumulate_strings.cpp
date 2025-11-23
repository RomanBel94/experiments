#include <array>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>

int main()
{
    std::array<std::string, 3> strings{"Hello ", "World", "!"};

    std::string result =
        std::accumulate(strings.cbegin(), strings.cend(), std::string(),
                        std::plus<std::string>());

    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}
