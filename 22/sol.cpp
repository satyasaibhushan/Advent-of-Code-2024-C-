#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

static uint64_t NewSecret(uint64_t x)
{
    x = (x ^ (x << 6)) & 0xffffff;
    x = (x ^ (x >> 5));
    return (x ^ (x << 11)) & 0xffffff;
}

uint64_t solve(vector<uint64_t> A)
{
    uint64_t result = 0;
    for (const uint64_t &initial : A)
    {
        uint64_t secret = initial;

        for (int i = 0; i < 2000; i++)
        {
            secret = NewSecret(secret);
        }
        result += secret;
    }

    return result;
}

uint64_t solve2(vector<uint64_t> A)
{
    map<vector<int>, uint64_t> sequence_totals;

    for (uint64_t start_num : A)
    {
        vector<int> prices;
        uint64_t a = start_num;
        prices.push_back(a % 10);

        for (int i = 0; i < 2000; i++)
        {
            a = NewSecret(a);
            prices.push_back(a % 10);
        }

        vector<int> changes;
        for (int i = 1; i < prices.size(); i++)
        {
            changes.push_back(prices[i] - prices[i - 1]);
        }

        map<vector<int>, int> first_occurrences;

        for (int i = 0; i <= changes.size() - 4; i++)
        {
            vector<int> seq = {changes[i], changes[i + 1], changes[i + 2], changes[i + 3]};

            if (first_occurrences.find(seq) == first_occurrences.end())
            {
                first_occurrences[seq] = i;
            }
        }

        for (const auto &[seq, pos] : first_occurrences)
        {
            sequence_totals[seq] += prices[pos + 4];
        }
    }

    uint64_t max_bananas = 0;
    vector<int> best_seq;

    for (const auto &[seq, total] : sequence_totals)
    {
        if (total > max_bananas)
        {
            max_bananas = total;
            best_seq = seq;
        }
    }

    return max_bananas;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<uint64_t> A;
    ifstream file("./question");

    string line;

    while (getline(file, line))
    {
        A.push_back(stoul(line));
    }

    uint64_t ans = solve(A);
    uint64_t ans2 = solve2(A);

    cout << "Part 1: " << ans << endl;
    cout << "Part 2: " << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
