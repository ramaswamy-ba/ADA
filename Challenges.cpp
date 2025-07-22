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

//2. Use of undeclared identifier 'reversseHalf'; did you mean 'reversedHalf'? (fix available)
#include <iostream>
#include <string>
#include <unordered_map>

std::string longestPalindromicRearrangement(const std::string& s)
{
    std::unordered_map<char, int> freq;
    for (char c : s)
        freq[c]++;

    std::string half = "";
    std::string mid="";

    for (auto& [ch, count] : freq)
    {
        // Add half of the characters to the half string
        half += std::string(count / 2, ch);
        // If there's an odd count, save one possible center character
        if (count % 2 != 0 && mid.empty() )
            mid = ch;
        std::cout<<half<<'\n';
    }

    std::string reversedHalf(half.rbegin(), half.rend());

    return half + mid + reversedHalf;

}
int main()
{
    std::string input = "ABCDEABD";
    std::cout << longestPalindromicRearrangement(input) << "\n";  // Output: "DBAEABD" or "DBACABD"
}
