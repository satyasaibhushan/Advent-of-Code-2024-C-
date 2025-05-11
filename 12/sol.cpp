#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <set>

using namespace std;

int m, n;
vector<pair<int, int>> dir = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

bool isValid(int i, int j)
{
    return i >= 0 && i < n && j >= 0 && j < m;
}

int calc_perimeter(set<tuple<int, int, int>> &p, char c, vector<string> &A)
{
    vector<tuple<int, int, int>> v(p.begin(), p.end());
    int perimeter = p.size();
    for (int idx = v.size() - 1; idx >= 0; idx--)
    {
        auto t = v[idx];
        int x = get<0>(t);
        int y = get<1>(t);
        int d = get<2>(t);

        if (d % 2 == 1)
        {
            int curr_y = y;
            while (p.find({x, curr_y + 1, d}) != p.end())
            {
                perimeter--;
                curr_y++;
                p.erase({x, curr_y + 1, d});
            }
            curr_y = y;
        }
        else
        {
            int curr_x = x;
            while (p.find({curr_x + 1, y, d}) != p.end())
            {
                perimeter--;
                curr_x++;
                p.erase({curr_x + 1, y, d});
            }
            curr_x = x;
        }
    }
    return perimeter;
}

pair<int, int> dfs(vector<string> &A, int i, int j, int &perimeter, int &area, vector<vector<int>> &visited)
{
    char c = A[i][j];
    if (!isValid(i, j))
        return {0, 0};
    if (A[i][j] == '.')
        return {0, 0};

    A[i][j] = '-';
    area += 1;
    visited[i][j] = 1;

    for (auto d : dir)
    {
        int ni = i + d.first;
        int nj = j + d.second;
        if (isValid(ni, nj) && A[ni][nj] == c)
            dfs(A, ni, nj, perimeter, area, visited);
        else if (!isValid(ni, nj) || (!visited[ni][nj] && A[ni][nj] != '-'))
            perimeter += 1;
    }

    A[i][j] = '.';
    return {perimeter, area};
}

void dfs2(vector<string> &A, int i, int j, set<tuple<int, int, int>> &p, int &area, vector<vector<int>> &visited)
{
    char c = A[i][j];
    if (!isValid(i, j))
        return;
    if (A[i][j] == '.')
        return;

    A[i][j] = '-';
    area += 1;
    visited[i][j] = 1;

    for (int d = 0; d < 4; d++)
    {
        int ni = i + dir[d].first;
        int nj = j + dir[d].second;
        if (isValid(ni, nj) && A[ni][nj] == c)
            dfs2(A, ni, nj, p, area, visited);
        else if (!isValid(ni, nj) || (!visited[ni][nj] && A[ni][nj] != '-'))
        {
            p.insert({i, j, d});
        }
    }

    A[i][j] = '.';

    return;
}

pair<long long, long long> solve(vector<string> A)
{
    long long ans = 0;
    long long ans2 = 0;

    vector<string> B = A;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (A[i][j] != '.')
            {
                int perimeter = 0, area = 0;
                vector<vector<int>> visited(n, vector<int>(m, 0));
                pair<int, int> p = dfs(A, i, j, perimeter, area, visited);
                ans += p.first * p.second;
            }
        }
    }

    A = B;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (A[i][j] != '.')
            {
                set<tuple<int, int, int>> perimeter_set;
                int area = 0;
                vector<vector<int>> visited(n, vector<int>(m, 0));
                char c = A[i][j];
                dfs2(A, i, j, perimeter_set, area, visited);
                int perimeter = calc_perimeter(perimeter_set, c, B);
                ans2 += area * perimeter;
            }
        }
    }
    return {ans, ans2};
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");
    string line;

    vector<string> numbers;
    while (getline(file, line))
    {
        numbers.push_back(line);
    }
    n = numbers.size();
    m = numbers[0].size();
    auto ans = solve(numbers);

    cout << ans.first << endl;
    cout << ans.second << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
