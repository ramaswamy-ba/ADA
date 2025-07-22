//1. Implement a function to find the smallest missing positive integer from an unsorted array
#include <vector>
#include <iostream>
int firstMissingPositive(std::vector<int>& nums)
{
    int n = nums.size();
    // Place each number in its correct position, i.e., nums[i] = i + 1
    for (int i = 0; i < n; ++i)
    {
        while (
            nums[i] > 0               // Only consider positive numbers
            && nums[i] <= n           // Ignore numbers greater than the array size
            && nums[i] != nums[nums[i] - 1] // Avoid duplicate swaps
          // Without the check → duplicate values cause repeated pointless swaps.
          // With the check → each value is only moved if it's going to a new position with a different value, preserving efficiency.
        )

        std::swap(nums[i], nums[nums[i] - 1]);
    }
    // Find the first index where the number is not correct
    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1)
            return i + 1;
    }
    // If all numbers are in place
    return n + 1;
}
int main()
{
    std::vector<int> arr = {3, 4, -1, 1};
    std::cout << "Smallest missing positive: " << firstMissingPositive(arr) << "\n";  // Output: 2
}

//2.
