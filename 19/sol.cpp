#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

map<string, int> pattern_map;

string trim(string s)
{
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

long long solve(vector<string> patterns, string word, bool isSecondPart = false)
{
    if (pattern_map.find(word) != pattern_map.end())
    {
        return pattern_map[word];
    }

    vector<long long> dp(word.length() + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < word.length(); i++)
    {
        if (dp[i] == 0)
            continue;

        for (const string &pattern : patterns)
        {
            if (i + pattern.length() <= word.length() &&
                word.substr(i, pattern.length()) == pattern)
            {
                dp[i + pattern.length()] += dp[i];
            }
        }
    }

    if (dp[word.length()] == 0)
        return 0;
    pattern_map[word] = dp[word.length()];

    return isSecondPart ? dp[word.length()] : 1;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    string line;

    vector<string> patterns;
    vector<string> words;

    getline(file, line);

    stringstream ss(line);
    string pattern;
    while (getline(ss, pattern, ','))
    {
        pattern.erase(0, pattern.find_first_not_of(" "));
        pattern.erase(pattern.find_last_not_of(" ") + 1);
        patterns.push_back(pattern);
    }
    getline(file, line);

    long long ans = 0;
    while (getline(file, line))
    {
        ans += solve(patterns, line, true);
    }

    cout << ans << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
