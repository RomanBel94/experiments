#include <cmath>
#include <iostream>

int main()
{
    double track;
    std::cout << "Enter track number to know your platform: ";
    std::cin >> track;

    std::cout << "Your platform is: " << std::ceil(track / 2) << std::endl;

    return 0;
}
