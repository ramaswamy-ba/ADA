/* Problem statement
* Given an integer array of coins[ ] of size N representing different types of currency and an integer sum, The task is to find the number of ways to make sum by using different combinations from coins[].  
* Note: Assume that you have an infinite supply of each type of coin. 
*/

/*
This problem can be solved with recursive or dynamic problem.
In every stage,  target sum can reached with using some denomination or not using the some denomination.
*/
// using recursive

/* Time: complexity:
O(2^coins)
  
*/
//recursion        
int count(std::vector<int> &coins, int index, int target_money)
{
  if ( target_money == 0 ) // target money 0 can be reached by one way, without using any coin
    return 1;
  if ( target_money < 0 )
    return 0;
  if ( index == coins.size() ) // no further denomincation of coins
    return 0;
  int denomination = coins[index];
  if ( denomination == target_sum )  // early termination condition - optional
    return 1;
  if ( denomination > target_sum )  // early termination condition - optional
    return 0;
  return count(coins, index, target_money - denomination) // using current denomination
        +
        count(coins, index+1, targe_money) // without using cureent denomination, move to next denomination
}
//Dynamic programming
/* Time complexity: O(coins * target_money) */
int count(std::vector<int> &vec, int target_money)
{
  std::vector<int> money_array(target_money+1, 0);
  money_array[0]=1; 
  for(auto coin: coins)
  {
      for(int money = coin; money <= target_money; ++money)
      {
        int remaining_money = target_money - money;
        money_array[ money ] = money_array[ money ]  /*without the current denomination*/ + money_array[ remaining_money ];  /* with current denomination*/
      }
}
