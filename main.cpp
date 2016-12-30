#include <iostream>
#include "Vector.h"

int main() {
    TinySTL::vector<int> nums(2);

    for (int i = 0;i < nums.size(); ++i) {
        nums[i] = i;
    }
    std::cout << "vector数值" << std::endl;
    for (int i = 0;i < nums.size(); ++i) {
        std::cout << nums[i] << std::endl;
    }
    std::cout << nums.begin() << std::endl;
    std::cout << "capacity: " << nums.capacity() << std::endl;

    nums.push_back(2);
    std::cout << nums.begin() << std::endl;
    std::cout << "capacity: " << nums.capacity() << std::endl;

    nums.push_back(5);
    std::cout << nums.begin() << std::endl;
    std::cout << "capacity: " << nums.capacity() << std::endl;

    nums.push_back(9);
    std::cout << nums.begin() << std::endl;
    std::cout << "capacity: " << nums.capacity() << std::endl;

    std::cout << "erase"<< std::endl;
    nums.erase(nums.begin(), nums.begin() + 3);
    for (int i = 0;i < nums.size(); ++i) {
        std::cout << nums[i] << std::endl;
    }
    return 0;
}