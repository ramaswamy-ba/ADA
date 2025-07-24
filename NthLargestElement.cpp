#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int kthElement(const std::vector<int> &vec, int k)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    for(auto v:vec)
    {
        heap.push(v);
        if( heap.size() > k)
            heap.pop();
    }

    return heap.top();
}

int main()
{
    std::vector<int> vec{1,5,6,10,21,9};
    auto res = kthElement(vec, 3);
    std::cout<<"Result = "<<res <<'\n';
    return 0;
}
