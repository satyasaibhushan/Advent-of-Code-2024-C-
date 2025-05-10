#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include "../helpers/helpers.cpp"

using namespace std;

pair<string, string> split(string a)
{
    if (a.size() == 1)
    {
        return {"0", a};
    }
    return {removeLeadingZeros(a.substr(0, a.size() / 2)), removeLeadingZeros(a.substr(a.size() / 2))};
}

string check(string a, int n, map<pair<string, int>, string> &dp)
{
    if (dp.count({a, n}) > 0)
    {
        return dp[{a, n}];
    }

    if (n == 0)
    {
        return dp[{a, n}] = "1";
    }

    if (a == "0")
    {
        return dp[{a, n}] = check("1", n - 1, dp);
    }

    if (a.size() % 2 == 0)
    {
        pair<string, string> p = split(a);
        return dp[{a, n}] = addStrings(check(p.first, n - 1, dp), check(p.second, n - 1, dp));
    }

    string multiplied = multiplyStrings(a, "2024");
    return dp[{a, n}] = check(multiplied, n - 1, dp);
}

string solve(vector<int> A, map<pair<string, int>, string> &dp, int n = 25)
{
    string ans = "0";
    for (auto i : A)
    {
        ans = addStrings(ans, check(to_string(i), n, dp));
    }
    return ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    string line;

    string ans;
    string ans2;

    while (getline(file, line))
    {
        vector<int> numbers;
        stringstream ss(line);
        int num;

        while (ss >> num)
        {
            numbers.push_back(num);
        }
        map<pair<string, int>, string> dp;
        ans = solve(numbers, dp);
        ans2 = solve(numbers, dp, 75);
    }
    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
