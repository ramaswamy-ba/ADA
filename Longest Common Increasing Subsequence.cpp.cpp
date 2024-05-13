#include<bits/stdc++.h>
#include <iostream>
using namespace std;

int longestIncreasingSubsequence(vector<int>& nums)
{
    int n = nums.size();
    vector<int> dp(n, 1);
    int max_len = 1;

    for(int i = 1; i < n; i++) 
    {
        for(int j = 0; j < i; j++) 
        {
            if(nums[i] > nums[j]) 
                dp[i] = max(dp[i], dp[j] + 1);
        }
        max_len = max(max_len, dp[i]);
    }

    return max_len;
}

int test1() 
{
    vector<int> nums = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    cout << "Length of Longest Increasing Subsequence is " << longestIncreasingSubsequence(nums) << endl;
    return 0;
}

vector<int> LCIS(vector<int>& arr1, vector<int>& arr2)
{
    int n = arr1.size(), m = arr2.size();
    vector<int> table(m, 0), parent(m, -1);
    for (int i=0; i<n; i++) 
    {
        int current = 0, last = -1;
        for (int j=0; j<m; j++) 
        {
            if (arr1[i] == arr2[j]) 
            {
                if (current + 1 > table[j]) 
                {
                    table[j] = current + 1;
                    parent[j] = last;
                }
            }
            if (arr1[i] > arr2[j])
            {
                if (table[j] > current) 
                {
                    current = table[j];
                    last = j;
                }
            }
        }
    }
    int idx = max_element(table.begin(), table.end()) - table.begin();
    vector<int> lcis;
    for (int i=idx; i>=0; i=parent[i])
        lcis.push_back(arr2[i]);
    reverse(lcis.begin(), lcis.end());
    return lcis;
}

int main() {
    vector<int> arr1 = {3, 4, 9, 1};
    vector<int> arr2 = {5, 3, 8, 9, 10, 2, 1};
    vector<int> lcis = LCIS(arr1, arr2);
    cout << "Length of LCIS: " << lcis.size() << "\n";
    cout << "LCIS: ";
    for (int x : lcis)
        cout << x << " ";
    cout << "\n";
    return 0;
}
