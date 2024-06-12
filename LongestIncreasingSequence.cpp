#include <iostream>
#include <iterator>
#include <algorithm>
#include <climits>
#include <vector>
//taking previous_num as argument
int lis_recursive1(std::vector<int>& nums, size_t n, int prev = INT_MAX)
{
    if (n == 0)
        return 0;
    int inclusive =0, exclusive =0;
    if (nums[n-1] < prev)
    {
        inclusive = 1 + lis_recursive1(nums, n-1, nums[n-1]);
        //std::cout<<"Incl "<<inclusive<<' '<<n<<' '<<nums[n-1]<< ' '<<prev<<'\n';
    }
    
    exclusive = lis_recursive1(nums, n-1, prev);
    //std::cout<<"Incl "<<inclusive<<" Excl "<<exclusive<<' '<<n<<' '<<nums[n-1]<< ' '<<prev<<'\n';
    return std::max(inclusive, exclusive);
}

//taking previous_num pos as argument
//still getting the wrong result
int lis_recursive2(const std::vector<int> &nums, size_t cur_pos, size_t prev_pos)
{
    if (cur_pos == 0 ) 
    {
        if ( nums[cur_pos] < nums[prev_pos] ) // case of handling in case of only two numbers, or scenario like this
            return 2; // current and prev numbers are the part of the seq
        return 1;     // only the current number is part of the seq
    }

    int inclusive=0, exclusive=0;

    if ( nums[cur_pos] < nums[prev_pos])
    {
        inclusive = 1 + lis_recursive2(nums, cur_pos-1, cur_pos);
        //std::cout<<"Incl "<<inclusive<<' '<<cur_pos<<' '<<nums[cur_pos]<< ' '<<prev_pos<<' '<<nums[prev_pos]<<'\n';
    }

    exclusive = lis_recursive2(nums, cur_pos-1, prev_pos);
    //std::cout<<"Incl "<<inclusive<<" Excl "<<exclusive<<' '<<cur_pos<<' '<<nums[cur_pos]<< ' '<<prev_pos<<' '<<nums[prev_pos]<<'\n';
    return std::max(inclusive, exclusive);
}

int lis_dp(const std::vector<int> &nums)
{
    std::vector<int> dp(nums.size(), 1); //minimum length is 1, so all position initialized with 1
    for(size_t i = 1; i<nums.size(); ++i)
    {
        for(size_t j=0; j<i; ++j)
        {
            if ( nums[j] < nums[i] )
                dp[i] = std::max(dp[i], dp[j] + 1);
        }
    }
    return dp[nums.size()-1];
}
//using sequence remembering

int lis_seq(const std::vector<int> &nums)
{
    std::vector<int> seq;
    seq.push_back(nums[0]);
    for(size_t i = 1; i<nums.size(); ++i)
    {
        if(seq.back() < nums[i])
            seq.push_back(nums[i]);
        else 
        {
            auto ite_pos_min_element = std::lower_bound(seq.begin(), seq.end(), nums[i]);
            *ite_pos_min_element = nums[i];
        }
    }
    return seq.size();
}

int main()
{
    std::vector<int> nums = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    nums = {-1,2,-3,-1,-2,9};
    auto res1 = lis_recursive1(nums, nums.size());
    auto res2 = lis_recursive2(nums, nums.size()-2,nums.size()-1);
    auto res3 = lis_dp(nums);
    auto res4 = lis_seq(nums);
    std::cout<<"Result1 = "<<res1<<'\n';
    std::cout<<"Result2 = "<<res2<<'\n';
    std::cout<<"Result3 = "<<res3<<'\n';
    std::cout<<"Result4 = "<<res4<<'\n';
    return 0;
}
