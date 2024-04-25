/* we have number a and b. find number b can be reached from a with following operation only
    if even 
        * can multiply by 2 
        * can divide   by 2 
        * can increase by 1
    else
        * can multiply by 2 
        * can decrese  by 1
*/
#include <iostream>
#include <set>
#include <queue>
#include <list>

struct Node
{
    int num;
    int step;
    std::list<int> seq;
    Node(int n, int s, std::list<int> l): num(n), step(s), seq(l) { }
};

int minOperationSeq(int x, int y)
{
    std::set<int> visit;
    auto isNotVisited = [&visit](int n) { return visit.find(n) == visit.end(); };

    std::queue<Node> q;
    q.push(Node(x,0,{}));
    while(!q.empty())
    {
        auto [val,step, seq] = q.front(); q.pop();
        std::cout<<val<<',';
        if( val == y )
        {
            std::cout<<"\nSequence --> ";
            for(auto &s: seq)
                std::cout<<s<<',';
            std::cout<<val<<" -  Steps: "<<step<<'\n';
            return 1;
        }
        visit.insert(val);
        seq.push_back(val);
        // val*2+1 <= y reduces the number crossing y
        if ( val%2 ==  0 )
        {
            if ( isNotVisited(val*2) && val*2+1 <= y )
                q.push(Node(val*2, step+1, seq));
            if ( isNotVisited(val/2) )
                q.push(Node(val/2, step+1, seq));
            if ( isNotVisited(val+1) )
                q.push(Node(val+1, step+1, seq));
        }
        else
        {
            if ( isNotVisited(val*2) && val*2+1 <= y )
                q.push(Node(val*2, step+1, seq));
            if ( isNotVisited(val-1) )
                q.push(Node(val-1, step+1, seq));
        }
    }
    std::cout<<"Path not possible\n";
    return 0;
}
 
int minOperation(int x, int y)
{
    std::set<int> visit;
    auto isNotVisited = [&visit](int n) { return visit.find(n) == visit.end(); };
    
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(x, 0));
    while(!q.empty())
    {
        auto [val,step] = q.front(); q.pop();
        std::cout<<val<<',';
        if( val == y )
            return step;
        visit.insert(val);
        if ( val%2 ==  0 )
        {
            if ( isNotVisited(val*2) )
                q.push(std::make_pair(val*2, step+1));
            if ( isNotVisited(val/2) )
                q.push(std::make_pair(val/2, step+1));
            if ( isNotVisited(val+1) )
                q.push(std::make_pair(val+1, step+1));
        }
        else
        {
            if ( isNotVisited(val*2) )
                q.push(std::make_pair(val*2, step+1));
            if ( isNotVisited(val-1) )
                q.push(std::make_pair(val-1, step+1));
        }
    }
    return 0;
}

int main()
{
    auto res = minOperation(12,21);
    std::cout<<"\n"<<res<<"\n";
    minOperationSeq(12,21);
    minOperationSeq(4,7);
    return 0;
}
