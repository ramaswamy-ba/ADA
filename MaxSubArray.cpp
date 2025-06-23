//Max subarray - Kadane's Algorithm
#include <iostream>
#include <vector>
#include <climits> // for INT_MIN

int maxSubArray(const std::vector<int>& nums) 
{
    int max_sum = INT_MIN;
    int current_sum = 0;

    for (int num : nums) 
    {
        current_sum = std::max(num, current_sum + num);
        max_sum = std::max(max_sum, current_sum);
    }

    return max_sum;
}

int main() 
{
    std::vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    std::cout << "Maximum subarray sum: " << maxSubArray(arr) << std::endl;
    return 0;
}
