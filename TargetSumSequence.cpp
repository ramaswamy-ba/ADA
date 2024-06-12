#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <climits>
void subArraySlidingWindow(const std::vector<int>& vec, int target)
{
    if (vec.empty())
        return;

    int sum = vec[0];
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, ", ")); std::cout << '\n';
    for (int w_begin = 0, w_end = 1; w_end <= vec.size();)
    {
        if (sum == target)
        {
            std::cout << "Found target " << w_begin << '-' << w_end-1 << " --> ";
            std::copy(vec.begin()+w_begin, vec.begin() + w_end, std::ostream_iterator<int>(std::cout, ", ")); std::cout << '\n';
            sum = sum - vec[w_begin];
            ++w_begin;
        }
        else if (sum > target)
        {
            sum = sum - vec[w_begin];
            ++w_begin;
        }
        else
        {
            if( w_end < vec.size())
                sum = sum + vec[w_end];
            ++w_end;
        }
    }
}

  std::vector<std::vector<int>> target_path;                                                                                                
  void list_targets(std::vector<int> &nums, int target)                                                                                     
  {                                                                                                                                         
      if ( nums.empty())                                                                                                                    
          return;                                                                                                                           
      int sum = nums[0];                                                                                                                    
      for(size_t w_begin =0, w_end=1; w_end <nums.size(); )                                                                                 
      {                                                                                                                                     
          if( sum == target)                                                                                                                
          {                                                                                                                                 
              target_path.push_back(std::vector<int>(nums.begin()+w_begin, nums.begin()+w_end));                                            
              sum  = sum - nums[w_begin];                                                                                                   
              ++w_begin;                                                                                                                    
          }                                                                                                                                 
          else if ( sum > target)                                                                                                           
          {                                                                                                                                 
              sum  = sum - nums[w_begin];                                                                                                   
              ++w_begin;                                                                                                                    
          }                                                                                                                                 
          else                                                                                                                              
          {                                                                                                                                 
                                                                                                                                            
              if( w_end < nums.size())                                                                                                      
                  sum  = sum + nums[w_end];                                                                                                 
              ++w_end;                                                                                                                      
          }                                                                                                                                 
      }                                                                                                                                     
      std::cout<<"Target Path size "<<target_path.size()<<'\n';                                                                             
      size_t min_length = INT_MAX;    
      for(auto v: target_path)    
      {    
          min_length = std::min(v.size(), min_length);    
          for(auto p: v)    
          {                                                                                                                                                                                                                                                                              
              std::cout<<p<<',';    
          }    
          std::cout<<'\n';    
      }    
      std::cout<<"Target exist with min length "<<min_length<<'\n';    
  }

void TestsubArray()
{
    std::vector<int> vec{ 9,1,2,3,4,5,6,7,8,9};
    subArraySlidingWindow(vec, 9);
/* -- output
  9, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  Found target 0-0 --> 9,
  Found target 2-4 --> 2, 3, 4,
  Found target 4-5 --> 4, 5,
  Found target 9-9 --> 9,
*/
}
int main()
{
	std::cout << "Test\n";
    TestsubArray();
	return 0;
}
