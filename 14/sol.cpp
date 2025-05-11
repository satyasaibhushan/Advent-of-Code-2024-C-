#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;
int row, col;

void dfs_util(vector<vector<int>> &grid, int i, int j, vector<vector<int>> &visited, int &area)
{
    if (i < 0 || i >= row || j < 0 || j >= col || grid[i][j] == 0 || visited[i][j] == 1)
        return;

    visited[i][j] = 1; // Mark as visited
    area++;            // Increment area for this component

    // Check all 8 directions
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++)
    {
        dfs_util(grid, i + dx[k], j + dy[k], visited, area);
    }
}

int dfs(vector<vector<int>> &grid)
{
    int max_area = 0;
    vector<vector<int>> visited(row, vector<int>(col, 0));

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] > 0 && !visited[i][j])
            {
                int area = 0;
                dfs_util(grid, i, j, visited, area);
                max_area = max(max_area, area);
            }
        }
    }

    return max_area;
}

int solve(vector<pair<int, int>> &pos, vector<pair<int, int>> &vel, int t = 100, int compression = 1)
{
    int max_areas = 0;
    for (int i = 0; i < t; i++)
    {
        vector<vector<int>> grid(row, vector<int>(col, 0));
        for (int j = 0; j < pos.size(); j++)
        {
            pos[j].first = (pos[j].first + vel[j].first + row) % row;
            pos[j].second = (pos[j].second + vel[j].second + col) % col;
            grid[pos[j].first][pos[j].second]++;
        }

        int max_area = dfs(grid);
        max_areas = max(max_areas, max_area);

        if (max_area > 100)
        {
            cout << "Time " << i << ": Maximum component area = " << max_area << endl;
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if (i % compression == 0 && j % compression == 0)
                        cout << grid[i][j];
                }
                if (i % compression == 0)
                    cout << endl;
            }
            cout << endl;
        }
    }
    cout << "Maximum number of robots = " << max_areas << endl;
    return 0;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    ifstream file("./question");
    string line;

    int ans = 0;
    vector<pair<int, int>> pos;
    vector<pair<int, int>> vel;

    row = 101;
    col = 103;

    while (getline(file, line))
    {
        stringstream ss(line);
        string p_part, v_part;
        ss >> p_part >> v_part;

        int x, y;
        sscanf(p_part.c_str(), "p=%d,%d", &x, &y);
        pos.push_back({x, y});

        int dx, dy;
        sscanf(v_part.c_str(), "v=%d,%d", &dx, &dy);
        vel.push_back({dx, dy});
    }
    ans = solve(pos, vel, 10000, 1);
    cout << ans << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
