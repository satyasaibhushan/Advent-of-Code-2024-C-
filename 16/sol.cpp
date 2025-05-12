#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <queue>
#include <set>

using namespace std;

vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

bool is_valid(pair<int, int> start, vector<string> A)
{
    int n = A.size();
    int m = A[0].size();

    if (start.first < 0 || start.first >= n || start.second < 0 || start.second >= m)
    {
        return false;
    }
    return true;
}

bool optimiser(vector<string> &A)
{
    int n = A.size();
    int m = A[0].size();
    bool changed = false;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (A[i][j] == '.')
            {
                int val = 0;
                for (int k = 0; k < 4; k++)
                {
                    int new_i = i + dirs[k].first;
                    int new_j = j + dirs[k].second;

                    if (is_valid({new_i, new_j}, A) && A[new_i][new_j] != '#')
                    {
                        val++;
                    }
                }
                if (val < 2)
                {
                    A[i][j] = '#';
                    changed = true;
                }
            }
        }
    }
    return changed;
}

struct State
{
    pair<int, int> pos;
    pair<int, int> dir;
    int cost;
    vector<pair<int, int>> path; // Store path directly

    bool operator>(const State &other) const
    {
        return cost > other.cost;
    }
};

pair<int, int> bfs(pair<int, int> &start, pair<int, int> &end, vector<string> &A)
{
    int COST = 1000;
    int n = A.size();
    int m = A[0].size();

    vector<vector<vector<int>>> visited(n, vector<vector<int>>(m, vector<int>(4, INT_MAX)));
    priority_queue<State, vector<State>, greater<State>> pq;

    // Initialize with start position
    vector<pair<int, int>> initial_path = {start};
    pq.push({start, {0, 1}, 0, initial_path});

    int best_cost = INT_MAX;
    set<pair<int, int>> all_best_path_points;

    while (!pq.empty())
    {
        State curr = pq.top();
        pq.pop();

        pair<int, int> pos = curr.pos;
        pair<int, int> dir = curr.dir;
        int cost = curr.cost;

        if (pos == end)
        {
            if (cost <= best_cost)
            {
                if (cost < best_cost)
                {
                    best_cost = cost;
                    all_best_path_points.clear();
                }
                // Add all points from this path to our set of best path points
                all_best_path_points.insert(curr.path.begin(), curr.path.end());
            }
            continue;
        }

        int dir_idx = -1;
        for (int i = 0; i < 4; i++)
        {
            if (dirs[i] == dir)
            {
                dir_idx = i;
                break;
            }
        }

        if (dir_idx == -1 || !is_valid(pos, A) || A[pos.first][pos.second] == '#' || cost > best_cost)
            continue;

        if (visited[pos.first][pos.second][dir_idx] < cost)
            continue;

        visited[pos.first][pos.second][dir_idx] = cost;

        for (int i = 0; i < 4; i++)
        {
            pair<int, int> new_dir = dirs[i];
            pair<int, int> new_pos = {pos.first + new_dir.first, pos.second + new_dir.second};

            int new_cost = cost;

            // first move from the start cell should never pay a turn-penalty
            bool is_first_move = curr.path.size() == 1;

            if (new_dir == dir)
            {
                // straight
                new_cost += 1;
            }
            else if (new_dir.first == -dir.first && new_dir.second == -dir.second)
            {
                // don’t reverse
                continue;
            }
            else
            {
                // a turn: only pay COST on subsequent moves, not on the very first
                new_cost += 1 + (is_first_move ? 0 : COST);
            }

            if (is_valid(new_pos, A) && A[new_pos.first][new_pos.second] != '#' && new_cost <= best_cost)
            {
                vector<pair<int, int>> new_path = curr.path;
                new_path.push_back(new_pos);
                pq.push({new_pos, new_dir, new_cost, new_path});
            }
        }
    }

    // Print the grid with all best paths
    if (!all_best_path_points.empty())
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (all_best_path_points.count({i, j}))
                {
                    cout << "O";
                }
                else
                {
                    cout << A[i][j];
                }
            }
            cout << endl;
        }
        cout << endl;

        return {best_cost, all_best_path_points.size()};
    }

    return {-1, -1};
}

pair<int, int> solve(vector<string> A)
{
    int n = A.size();
    int m = A[0].size();
    pair<int, int> start;
    pair<int, int> end;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (A[i][j] == 'S')
            {
                start = {i, j};
            }
            else if (A[i][j] == 'E')
            {
                end = {i, j};
            }
        }
    }

    // Optimize until no more changes
    // bool changed = true;
    // while (changed)
    // {
    //     changed = optimiser(A);
    // }

    pair<int, int> ans = bfs(start, end, A);
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

    vector<string> a;

    while (getline(file, line))
    {
        a.push_back(line);
    }
    pair<int, int> ans = solve(a);

    cout << ans.first << " " << ans.second << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
