#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>

template<typename T>
double percent_diff(const T& a, const T& b) noexcept
{
    return 100.0 - std::min(a, b) / std::max(a, b) * 100.0;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
        std::cout << "Usage: percentDiff <number> <number>" << std::endl;

    std::cout << std::setprecision(4) << "The difference is: "
        << percent_diff(std::atof(argv[1]), std::atof(argv[2]))<< '%' << std::endl;

    return 0;
}
