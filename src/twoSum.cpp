#include <cstddef>
#include <iostream>
#include <vector>

void twoSum(std::vector<int>& arr, int target)
{
    for(size_t left_index = 0; left_index < arr.size() - 1; ++left_index) {
        for(size_t right_index = 1; right_index < arr.size(); ++right_index) {
            if(arr[left_index] + arr[right_index] == target) {
                std::cout << left_index << ' ' << right_index << std::endl;
                return;
            }
        }
    }
    std::cout << "not found" << std::endl;
}

int main()
{
    std::vector<int> arr{ 1, 5, 4, 7, 12, 3, 8 };
    twoSum(arr, 8);
    return 0;
}
