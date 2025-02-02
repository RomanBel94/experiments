#include <iostream>
#include <vector>

int findUniqueWithXOR(const std::vector<int>& vec)
{
    int result = 0;
    for (auto num : vec)
    {
        result ^= num;
    }
    return result;
}

int main()
{
    std::vector<int> nums{2, 2, 1, 1, 3};
    std::cout << findUniqueWithXOR(nums) << std::endl;
    return 0;
}
