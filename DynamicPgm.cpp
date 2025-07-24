// DynamicProgramming.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>
using namespace std;
using Map = std::unordered_map<int, uint64_t>;

template<class T>
concept SmartPointer = std::is_same_v<T, std::shared_ptr<typename T::element_type>> || std::is_same_v<T,  std::unique_ptr<typename T::element_type>>;


bool reachSum(vector<int> vec, int target)
{
    if (target == 0) return true;
    if (target < 0) return false;
    
    for (int i = 0; i < vec.size(); ++i)
    {
        bool res = reachSum(vec, target - vec[i]);
        if (res)
        {
            std::cout << vec[i] << '\t';
            return true;
        }
    }
    std::cout << '\n';
    return false;
}

int  reachSumNoOfWays(vector<int> vec, int target)
{
    if (target == 0) return 1;
    if (target < 0) return 0;
    int ways = 0;

    for (int i = 0; i < vec.size(); ++i)
    {
        ways += reachSum(vec, target - vec[i]);
    }
    return ways;
}

void TestreachSum()
{
    vector<int> vec{ 1,2, 4, 6,7 };
    std::cout << "reachSum 9 " << reachSum(vec, 9) << endl;

    std::cout << "reachSumNoOfWays  9 " << reachSumNoOfWays(vec, 9) << endl;
}

uint64_t fibonacci(int count, Map &map )
{
    if (map.count(count)) return map[count];
    if (count <=  2) return 1;
    map[count]= (uint64_t)(fibonacci(count - 1, map)) + fibonacci(count - 2, map);
    return map[count];
}

void Testfibonacci()
{
    // 1 1 2 3 5 8 13
    Map map;
    map[0] = map[1] = 1;
    cout << "5th num " << fibonacci(5, map) << endl;
    cout << "7th num " << fibonacci(7, map) << endl;
    cout << "20th num " << fibonacci(20, map) << endl;
    cout << "50th num " << fibonacci(50, map) << endl;
}

struct pair_hash
{
    template<typename First, typename Second>
    std::size_t operator() (const std::pair<First, Second>& arg) const
    {
        auto h1 = std::hash<First>{}(arg.first);
        auto h2 = std::hash<Second>{}(arg.second);
        return h1 ^ h2;
    }
};

uint64_t gridTraverse(int row, int col)
{
    static std::unordered_map<std::pair<int, int>, uint64_t, pair_hash> gridmemo;
    if (row == 1 && col == 1)  return 1;
    if (row == 0 || col == 0)  return 0;

    auto key = std::make_pair(row, col);
    if (gridmemo.count(key)) return gridmemo[key];

    gridmemo[key]=  gridTraverse(row - 1, col) + gridTraverse(row, col - 1);
    return gridmemo[key];
}

void TestgridTraverse()
{ 
    std::cout << "2X3 " << gridTraverse(2,3) << endl;
    std::cout << "3X3 " << gridTraverse(3, 3) << endl;
    std::cout << "3X3 " << gridTraverse(18, 18) << endl;
}

std::optional<std::vector<int>> bestSum(const std::vector<int> &vec, int targetsum)
{
    static std::unordered_map<int, std::optional<std::vector<int>>> memo;
    if (targetsum == 0) return std::vector<int> {};
    if (targetsum < 0)  return  {};

    if (auto ite = memo.find(targetsum); ite != memo.end() ) 
        return ite->second;

    std::vector<int> shortest_path;
    for (int i = 0; i < vec.size(); ++i)
    {
        int num = vec[i];
        auto res = bestSum(vec, targetsum - vec[i]);
        //std::cout << num <<" "<<(res ? res.value().size()+1 : 0) << endl;
        if (res)
        {
            res.value().push_back(vec[i]);
            if (shortest_path.empty() || res.value().size() < shortest_path.size())
            {
                shortest_path = res.value();
                memo[targetsum] = shortest_path;
            }
        }
    }
    if (!shortest_path.empty())
        return  shortest_path;
    return {};
}

void TestbestSum()
{
    auto print = [](auto&& vec) { std::cout << '\n'; std::copy(vec.begin(), vec.end(), ostream_iterator<int>(std::cout, " ")); };
    std::vector<int> vec1{ 5,3,4,7 };
    auto res1 = bestSum(vec1, 7);
    print(res1.value());
    
    auto res2 = bestSum({ 5,7,10,15 }, 120); print(res2.value());

}

/*
Fill at 13 10
Fill at 17 13
Fill at 18 17
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 18 17
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 17 13
Fill at 18 17
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 18 17
Fill at 21 18
Fill no 22 21
Fill no 22 21
Fill at 21 18
Fill no 22 21
Fill no 22 21
4
*/

int minfill(int begin, int end, int gap, const std::vector<int>& points, int pos)
{

    for(; pos<points.size() && begin+gap > points[pos]; ++pos)

    if (pos >= points.size())
        return 0;
    --pos;
    int v = 0;
    if (  begin + gap <= end)
    {
        v = 1;
        std::cout << "Fill at " << points[pos] << ' ' << begin << '\n';
    }
    else
    {
        std::cout<<"Fill no "<< points[pos] <<' '<<begin<< '\n';
    }
    int fill   = minfill(points[pos], end, gap, points, pos + 1) + v;
    int nofill = minfill(points[pos], end, gap, points, pos + 1);
    return std::min(fill, nofill);
}

int min_refills1(int start_pos, int finish_pos, int range, const std::vector<int>& tap_positions) 
{
    int num_refills = 0, current_pos = start_pos;
    //auto tap_it = std::lower_bound(tap_positions.begin(), tap_positions.end(), start_pos);
    auto tap_it = tap_positions.begin();

    while (current_pos + range < finish_pos) 
    {
        if (tap_it == tap_positions.end() || *tap_it > current_pos + range) 
        {
            // No tap in range, return -1
            return -1;
        }

        // Move to the farthest tap within range
        auto next_tap_it = std::prev(std::upper_bound(tap_it, tap_positions.end(), current_pos + range));
        current_pos = *next_tap_it;
        tap_it = next_tap_it + 1;
        num_refills++;
    }

    return num_refills;
}


int min_refills(int start_pos, int finish_pos, int range, const std::vector<int>& points)
{
    int fills = 0;
    int cur_pos = start_pos;
    auto tap_it = points.begin();

    while (cur_pos + range < finish_pos)
    {
        if (tap_it == points.end() || *tap_it > cur_pos + range)
            return -1;

        // Move to the farthest tap within range
        auto next_tap_it = std::prev(std::upper_bound(tap_it, points.end(), cur_pos + range));
        cur_pos = *next_tap_it;
        tap_it = next_tap_it + 1;
        fills++;
    }
    
    auto ite = points.begin();
    int end_pos = finish_pos;
    for (int pos = start_pos; pos + range < end_pos;)
    {
        if (ite == points.end() || *ite > pos + range)
            return -1;
        auto next_ite = std::prev(std::upper_bound(ite, points.end(), pos + range));
        pos = *next_ite;
        ite = next_ite + 1;
        ++fills ;
    }
    return fills;
}

int min_fils(int start_pos, int end_pos, int range, const std::vector<int>& points)
{
    auto ite = points.begin();
    int fills = 0;
    for (int pos = start_pos; pos + range < end_pos;)
    {
        if (ite == points.end() || *ite > pos + range)
            return -1;
        auto next_ite = std::prev(std::upper_bound(ite, points.end(), pos + range));
        pos = *next_ite;
        ite = next_ite + 1;
        ++fills;
    }
    return fills;
}
int min_fills(int start_pos, int end_pos, int range, const std::vector<int>& points)
{
    int cur = start_pos;
    int fills = 0;
    for (auto ite = points.begin() ; ite != points.end() && cur <= end_pos;)
    {
        auto next_fill = std::upper_bound(ite, points.end(), cur + range);
        if (next_fill == points.end())
        {
            std::cout << "Not possible case\n";
            break;
        }
        cur = *std::prev(next_fill);
        ++fills;
        std::cout << "Fill at " << cur << ", count " << fills << '\n';
        ite = ++next_fill;
    }
    std::cout << "MinFills " << fills << '\n';
    return fills;
}


void Testminfill()
{
        std::cout << "Result\n" << min_fils(10, 20, 4, std::vector<int>{13, 17, 18, 30}) << '\n';
        //std::cout << "Result\n" << minfill(10, 25, 5, std::vector<int>{13, 17, 18, 21,22,30}, 0) << '\n';
    
        std::cout << "Result\n" << min_fils(10, 25, 5, std::vector<int>{13, 17, 18, 21, 22, 30}) << '\n';
        min_fills(10, 30, 5, std::vector<int>{13, 17, 18, 21, 22, 27});
}

void subArray(const std::vector<int>& vec, int target)
{
    if (vec.empty())
        return;

    int sum = vec[0];
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(cout, ", "));
    for (int w_begin = 0, w_end = 1; w_end <= vec.size();)
    {
        if (sum == target)
        {
            std::cout << "Found target " << w_begin << '-' << w_end << ' ';
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
            sum = sum + vec[w_end];
            ++w_end;
        }
    }
}

void TestsubArray()
{
    std::vector<int> vec{ 9,1,2,3,4,5,6,7,8,9 };
    subArray(vec,9);
}

void test()
{
    //pair_hash p;
    //std::cout <<"Hash result "<< p(std::make_pair(10, 2)) <<"  "<< p(std::make_pair(2, 10))<<endl;
    std::cout << "Hello World!\n";
    //TestreachSum();
    //Testfibonacci();
    //TestgridTraverse();
    //TestbestSum();
    TestmaxProfit();
    //TestsubArray();
    //Testminfill();
}

int main()
{
    test();

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
