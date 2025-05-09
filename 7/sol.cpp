#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <set>

using namespace std;

long long solve(vector<int> numbers, long long sum)
{
    set<long long> seen = {numbers[0]};

    for (int i = 1; i < numbers.size(); i++)
    {
        set<long long> curr;
        for (auto x : seen)
        {
            long long s1 = x + numbers[i];
            long long s2 = x * numbers[i];
            if (s1 == sum || s2 == sum)
                return sum;
            if (s1 < sum)
                curr.insert(s1);
            if (s2 < sum)
                curr.insert(s2);
        }
        seen = curr;
    }

    return 0;
}

long long solve2(vector<int> numbers, long long sum)
{
    set<long long> seen = {numbers[0]};

    for (int i = 1; i < numbers.size(); i++)
    {
        set<long long> curr;
        for (auto x : seen)
        {
            long long s1 = x + numbers[i];
            long long s2 = x * numbers[i];
            long long s3 = x * pow(10, to_string(numbers[i]).length()) + numbers[i];
            if (s1 == sum || s2 == sum || s3 == sum)
                return sum;
            if (s1 < sum)
                curr.insert(s1);
            if (s2 < sum)
                curr.insert(s2);
            if (s3 < sum)
                curr.insert(s3);
        }
        seen = curr;
    }

    return 0;
}

string addStrings(string num1, string num2)
{
    string result = "";
    int carry = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0 || j >= 0 || carry)
    {
        int digit1 = (i >= 0) ? num1[i] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;
        int sum = digit1 + digit2 + carry;

        carry = sum / 10;
        result += to_string(sum % 10);

        i--;
        j--;
    }

    reverse(result.begin(), result.end());
    return result;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    string line;

    string ans = "0";
    string ans2 = "0";

    while (getline(file, line))
    {
        vector<int> numbers;
        long long sum;
        sum = stoll(line.substr(0, line.find(':')));
        line = line.substr(line.find(':') + 1);
        stringstream ss(line);
        int num;

        while (ss >> num)
        {
            numbers.push_back(num);
        }
        ans = addStrings(ans, to_string(solve(numbers, sum)));
        ans2 = addStrings(ans2, to_string(solve2(numbers, sum)));
    }
    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
