#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
using namespace std;

pair<int, int> get_start(vector<string> input)
{
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == '^')
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

static const vector<pair<int, int>> DIR = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

bool in_bounds(int x, int y, int R, int C)
{
    return x >= 0 && x < R && y >= 0 && y < C;
}

int solve(const vector<string> &grid, pair<int, int> start)
{
    int R = grid.size(), C = grid[0].size();
    vector<vector<bool>> seen(R, vector<bool>(C, false));
    int x = start.first, y = start.second;
    int dir = 0;
    seen[x][y] = true;
    int count = 1;

    while (true)
    {
        int nx = x + DIR[dir].first;
        int ny = y + DIR[dir].second;
        if (!in_bounds(nx, ny, R, C))
            break;
        if (grid[nx][ny] == '#')
        {
            dir = (dir + 1) & 3;
            continue;
        }
        x = nx;
        y = ny;
        if (!seen[x][y])
        {
            seen[x][y] = true;
            count++;
        }
    }
    return count;
}

int solve2(const vector<string> &grid, pair<int, int> start)
{
    int R = grid.size(), C = grid[0].size();
    int base = solve(grid, start);
    int traps = 0;
    auto mod = grid;

    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if ((i == start.first && j == start.second) || grid[i][j] == '#')
                continue;
            mod[i][j] = '#';
            vector<vector<array<bool, 4>>> seen2(R, vector<array<bool, 4>>(C, {false, false, false, false}));
            int x = start.first, y = start.second, dir = 0;
            bool loop = false;
            while (true)
            {
                int nx = x + DIR[dir].first;
                int ny = y + DIR[dir].second;
                if (!in_bounds(nx, ny, R, C))
                    break;
                if (mod[nx][ny] == '#')
                {
                    dir = (dir + 1) & 3;
                }
                else
                {
                    x = nx;
                    y = ny;
                }
                if (seen2[x][y][dir])
                {
                    loop = true;
                    break;
                }
                seen2[x][y][dir] = true;
            }
            if (loop)
                traps++;
            mod[i][j] = grid[i][j];
        }
    }
    return traps;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();
    ifstream file("./question");
    string line;
    vector<string> k;
    while (getline(file, line))
    {
        k.push_back(line);
    }

    pair<int, int> startPoint = get_start(k);
    int ans = solve(k, startPoint);
    int ans2 = solve2(k, startPoint);
    // <2171
    // <2007
    // <2006

    cout << ans << endl;
    cout << ans2 << endl;
    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
    return 0;
}
