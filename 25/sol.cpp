#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

long long solve(vector<vector<int>> locks, vector<vector<int>> keys)
{
    long long ans = 0;
    for (auto lock : locks)
    {
        for (auto key : keys)
        {
            int flag = 0;
            for (int i = 0; i < 5; i++)
            {
                if (lock[i] + key[i] > 5)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
                ans++;
        }
    }
    return ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<vector<int>> locks;
    vector<vector<int>> keys;
    ifstream file("./question");
    string line;

    int curr = -1;
    vector<int> temp(5, 0);
    while (getline(file, line))
    {
        if (line.empty())
        {
            if (curr == 0)
                locks.push_back(temp);
            else
            {

                for (int i = 0; i < 5; i++)
                {
                    temp[i]--;
                }
                keys.push_back(temp);
            }
            curr = -1;
            temp = vector<int>(5, 0);
            continue;
        }
        if (curr == -1)
        {
            if (line[0] == '#')
                curr = 0;
            else
                curr = 1;
            continue;
        }

        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == '#')
                temp[i]++;
        }
    }
    if (curr == 0)
        locks.push_back(temp);
    else
    {
        keys.push_back(temp);
        for (int i = 0; i < 5; i++)
        {
            temp[i]--;
        }
        keys.push_back(temp);
    }

    long long ans = solve(locks, keys);
    cout << ans << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
