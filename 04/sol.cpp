#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
using namespace std;

vector<vector<pair<int, int>>> dir = {{{1, 0}, {2, 0}, {3, 0}}, {{0, 1}, {0, 2}, {0, 3}}, {{1, 1}, {2, 2}, {3, 3}}, {{1, -1}, {2, -2}, {3, -3}}, {{-1, 0}, {-2, 0}, {-3, 0}}, {{0, -1}, {0, -2}, {0, -3}}, {{-1, -1}, {-2, -2}, {-3, -3}}, {{-1, 1}, {-2, 2}, {-3, 3}}};

int row = 0, col = 0;

bool check(int i, int j)
{
    return i >= 0 && i < row && j >= 0 && j < col;
}

int solve(vector<string> k, string req, vector<vector<pair<int, int>>> dir)
{
    int ans = 0;
    for (int i = 0; i < k.size(); i++)
    {
        for (int j = 0; j < k[i].size(); j++)
        {
            if (k[i][j] == req[0])
            {
                int found = 0;
                for (int d = 0; d < dir.size(); d++)
                {
                    bool match = true;
                    for (int l = 0; l < req.size() - 1; l++)
                    {
                        int x = i + dir[d][l].first;
                        int y = j + dir[d][l].second;
                        if (!check(x, y) || k[x][y] != req[l + 1])
                        {
                            match = false;
                            break;
                        }
                    }
                    if (match)
                        found++;
                }
                ans += found;
            }
        }
    }
    return ans;
}
bool checkDirection(const vector<string>& grid, int row, int col, const string& pattern, int rowDir, int colDir) {
    int n = grid.size();
    int m = grid[0].size();
    int len = pattern.size();

    for (int i = 0; i < len; i++) {
        int newRow = row + i * rowDir;
        int newCol = col + i * colDir;
        if (!check(newRow, newCol) || grid[newRow][newCol] != pattern[i]) {
            return false;
        }
    }

    return true;
}

bool isXPattern(const vector<string>& grid, int row, int col, const string& pattern) {
    int len = pattern.size();

    bool dir1 = checkDirection(grid, row - (len / 2), col - (len / 2), pattern, 1, 1); // Down-right
    bool dir2 = checkDirection(grid, row - (len / 2), col + (len / 2), pattern, 1, -1); // Down-left
    bool dir3 = checkDirection(grid, row + (len / 2), col - (len / 2), pattern, -1, 1); // Up-right
    bool dir4 = checkDirection(grid, row + (len / 2), col + (len / 2), pattern, -1, -1); // Up-left
    return (dir1 + dir2 + dir3 + dir4) >= 2;
}

int solve2(const vector<string>& grid, const string& pattern) {
    int count = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (isXPattern(grid, i, j, pattern)) {
                count++;
            }
        }
    }

    return count;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream file("./question");
    string line;
    vector<string> k;
    while (getline(file, line))
    {
        k.push_back(line);
    }
    row = k.size();
    col = k[0].size();

    int ans = solve(k, "XMAS", dir);
    int ans2 = solve2(k, "MAS");
    cout << ans << endl;
    cout << ans2 << endl;
    return 0;
}
