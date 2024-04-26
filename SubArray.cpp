#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
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
