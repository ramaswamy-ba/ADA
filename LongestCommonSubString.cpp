#include <stdio.h>
#include <iostream>
#include <array>

using namespace std;

int lcs(string X, string Y, int m, int n)
{
    if (m == 0 || n == 0)
        return 0;
    if (X[m - 1] == Y[n - 1])
        return 1 + lcs(X, Y, m - 1, n - 1);
    else
        return max(lcs(X, Y, m, n - 1), lcs(X, Y, m - 1, n));
}
int Dlcs(string X, string Y, const int m, const int n)
{
    int L[m + 1][n + 1];
    for(int i =0; i<=m; ++i)
    for (int j = 0; j <= n; ++j)
    {
        if (i == 0 || j == 0)
            L[i][j] = 0;
        else if (X[i - 1] == Y[j - 1])
            L[i][j] = L[i - 1][j - 1] + 1;
        else
            L[i][j] = std::max(L[i - 1][j], L[i][j - 1]);
    }
    return L[m][n];
}

void testlcs()
{
    string S1 = "AGGTAB";
    string S2 = "GXTXAYB";
    int m = S1.size();
    int n = S2.size();

    cout << "Length of LCS is " << lcs(S1, S2, m, n) << '\n';
    cout << "Length of LCS is " << Dlcs(S1, S2, m, n)<<'\n';
}

int main()
{
    testlcs();
    return 0;
    
}
