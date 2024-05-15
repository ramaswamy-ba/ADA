#include <iostream>
using namespace std;

int maxLen(int arr[], int n, int target)
{
    unordered_map<int, int> presum;

    int sum = 0;
    int len = 0;

    for (int i = 0; i < n; i++) 
    {
        sum += arr[i];
        
        if (sum == target)
            ++len; 

        if (presum.find(sum) != presum.end()) 
            len = max(len, i - presum[sum]);
        else 
            presum[sum] = i;
    }
    return len;
}

int main()
{
    int arr[] = {10, -2, 2, -8, 1, 7, 10, 23};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 0;
    auto res  = maxLen(arr, n, target);
    cout << "Length of the longest "<<target<<" sum subarray is " << res<<std::endl;

    return 0;
}
