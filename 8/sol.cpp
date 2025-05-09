#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <set>

using namespace std;

int row, col;

bool isValid(int x, int y)
{
    return x >= 0 && x < row && y >= 0 && y < col;
}

int solve(map<char, vector<pair<int, int>>> m)
{
    set<pair<int, int>> antinodes;
    for (auto it : m)
    {
        vector<pair<int, int>> points = it.second;
        for (int i = 0; i < points.size(); i++)
        {
            for (int j = i + 1; j < points.size(); j++)
            {
                int x1 = points[i].first;
                int y1 = points[i].second;
                int x2 = points[j].first;
                int y2 = points[j].second;

                int x = 2 * x1 - x2;
                int y = 2 * y1 - y2;
                if (isValid(x, y))
                {
                    antinodes.insert({x, y});
                }
                x = 2 * x2 - x1;
                y = 2 * y2 - y1;
                if (isValid(x, y))
                {
                    antinodes.insert({x, y});
                }
            }
        }
    }
    return antinodes.size();
}

int solve2(map<char, vector<pair<int, int>>> m)
{
    set<pair<int, int>> antinodes;
    for (auto it : m)
    {
        vector<pair<int, int>> points = it.second;
        for (int i = 0; i < points.size(); i++)
        {
            for (int j = i + 1; j < points.size(); j++)
            {
                int x1 = points[i].first;
                int y1 = points[i].second;
                int x2 = points[j].first;
                int y2 = points[j].second;
                antinodes.insert({x1, y1});
                antinodes.insert({x2, y2});

                int x = 2 * x1 - x2;
                int y = 2 * y1 - y2;
                while (isValid(x, y))
                {
                    antinodes.insert({x, y});
                    x += x1 - x2;
                    y += y1 - y2;
                }
                x = 2 * x2 - x1;
                y = 2 * y2 - y1;
                while (isValid(x, y))
                {
                    antinodes.insert({x, y});
                    x += x2 - x1;
                    y += y2 - y1;
                }
            }
        }
    }
    return antinodes.size();
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    string line;
    int i = 0;
    int j = 0;
    map<char, vector<pair<int, int>>> m;

    while (getline(file, line))
    {
        for (j = 0; j < line.size(); j++)
        {
            if (line[j] != '.')
                m[line[j]].push_back({i, j});
        }
        i++;
    }
    row = i;
    col = j;
    int ans = solve(m);
    // < 429
    // > 390
    int ans2 = solve2(m);
    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
