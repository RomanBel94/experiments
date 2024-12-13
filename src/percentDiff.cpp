#include <algorithm>
#include <iostream>
#include <iomanip>

template<typename T>
double percent_diff(const T& a, const T& b) noexcept
{
    return std::min(a, b) / std::max(a, b) * 100.0;
}

int main()
{
    std::cout << "Enter two numers to get the percent difference between them: ";
    double num_a, num_b;
    std::cin >> num_a >> num_b;

    std::cout << std::setprecision(4)
        << "The difference is: " << percent_diff(num_a, num_b)<< '%' << std::endl;

    return 0;
}
