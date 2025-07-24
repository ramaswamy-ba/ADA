#include <iostream>
#include <vector>

// initiating a buy, not completing a transaction.
//Think of op_count as counting completed transactions—each one being a buy followed by a sell.
  
int maxProfit(const std::vector<int>& vec, int pos, bool canBuy, int op_count)
{
    if (pos >= vec.size() || op_count == 0 )
        return 0;
    
    int without  = maxProfit(vec, pos + 1, canBuy, op_count);
    int with = 0;
    if (canBuy)
        with = -vec[pos] + maxProfit(vec, pos + 1, false, op_count); // Buy, no op_count change
    else
        with = vec[pos]  + maxProfit(vec, pos + 1, true, op_count - 1); // Sell, op_count decreases
  
    return std::max(without, with);
}

//if we allowed to buy and sell only once
int maxProfit2(std::vector<int>& prices) 
{
    int minPrice = INT_MAX;
    int maxProfit = 0;

    for (int price : prices) 
    {
        minPrice = std::min(minPrice, price);           // Keep track of the lowest buying price
        maxProfit = std::max(maxProfit, price - minPrice); // Try selling today and update max profit
    }

    return maxProfit;
}

int maxProfitMemo(const std::vector<int>& vec, int pos, bool canBuy, int op_count)
{
    static std::vector<std::vector<int>> memo(op_count + 1, std::vector<int>(vec.size(), -1));
    if (pos >= vec.size() || op_count == 0)
        return 0;
    if (memo[op_count][pos] != -1)
    {
        std::cout << "Result from memo for " << pos << "th day " << op_count << ' ' << memo[op_count][pos] << endl;
        return memo[op_count][pos];
    }

    int without  = maxProfit(vec, pos + 1, canBuy, op_count);
    int with = 0;
    if (canBuy)
        with = -vec[pos] + maxProfit(vec, pos + 1, false, op_count);
    else
        with = vec[pos]  + maxProfit(vec, pos + 1, true, op_count - 1);

    memo[op_count][pos] = std::max(without, with);
    return memo[op_count][pos];
}

int main()
{
    std::vector<int> vec{ 3,2,6,5,0,3 };
    //std::vector<int> vec{ 2,3,5 };
    std::vector<int> vec3{7, 10, 1, 3, 6, 9, 2}; // 
    std::vector<int> vec4{7, 10}; //3

    std::cout << "Max Profit " << maxProfit(vec, 0, true, 4) << endl;
    std::cout << "Max Profit " << maxProfitMemo(vec, 0, true, 4) << endl;
}
