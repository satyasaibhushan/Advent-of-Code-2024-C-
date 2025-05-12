#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <climits>

using namespace std;

vector<pair<int, int>> dir = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // right, down, left, up

bool valid(int x, int y, int size)
{
    return x >= 0 && x < size && y >= 0 && y < size;
}

int dfs(vector<vector<int>> graph, int i, int j)
{
    int minDist = INT_MAX;

    if (i == graph.size() - 1 && j == graph[0].size() - 1)
    {
        return 0;
    }

    for (int k = 0; k < 4; k++)
    {
        int x = i + dir[k].first;
        int y = j + dir[k].second;
        if (valid(x, y, graph.size()) && graph[x][y] == 0)
        {
            graph[i][j] = 1;
            int dist = dfs(graph, x, y);
            if (dist != INT_MAX)
            {
                minDist = min(minDist, dist + 1);
            }
            graph[i][j] = 0;
        }
    }

    return minDist;
}

int bfs(vector<vector<int>> graph, int i, int j)
{
    queue<pair<pair<int, int>, int>> q; // ((x,y), distance)
    q.push({{i, j}, 0});
    graph[i][j] = 1; // Mark start as visited

    while (!q.empty())
    {
        auto current = q.front();
        int curr_i = current.first.first;
        int curr_j = current.first.second;
        int distance = current.second;
        q.pop();

        // If we reached the target
        if (curr_i == graph.size() - 1 && curr_j == graph[0].size() - 1)
        {
            return distance;
        }

        for (int k = 0; k < 4; k++)
        {
            int x = curr_i + dir[k].first;
            int y = curr_j + dir[k].second;
            if (valid(x, y, graph.size()) && graph[x][y] == 0)
            {
                graph[x][y] = 1; // Mark as visited
                q.push({{x, y}, distance + 1});
            }
        }
    }
    return INT_MAX; // No path found
}

int solve(vector<pair<int, int>> pairs, int limit = 12, int size = 7)
{
    vector<vector<int>> graph(size, vector<int>(size, 0));
    for (int i = 0; i < limit; i++)
    {
        graph[pairs[i].second][pairs[i].first] = 2;
    }

    graph[0][0] = 1;
    // int ans = dfs(graph, 0, 0);
    int ans = bfs(graph, 0, 0);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    return ans;
}

class UnionFind
{
    vector<int> parent, rank;
    int size;

public:
    UnionFind(int sizes)
    {
        int n = sizes * sizes;
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;
        size = sizes;
    }

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(pair<int, int> point_x, pair<int, int> point_y)
    {
        int x = point_x.first * size + point_x.second;
        int y = point_y.first * size + point_y.second;

        int px = find(x), py = find(y);
        if (px == py)
            return;

        if (rank[px] < rank[py])
        {
            parent[px] = py;
        }
        else if (rank[px] > rank[py])
        {
            parent[py] = px;
        }
        else
        {
            parent[py] = px;
            rank[px]++;
        }
    }

    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }
};

bool canReachEnd(vector<pair<int, int>> &pairs, int limit, int size)
{
    UnionFind uf(size);
    vector<vector<bool>> blocked(size, vector<bool>(size, false));

    for (int i = 0; i < limit; i++)
        blocked[pairs[i].second][pairs[i].first] = true;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (blocked[i][j])
                continue;
            for (const auto &d : dir)
            {
                int ni = i + d.first;
                int nj = j + d.second;
                if (valid(ni, nj, size) && !blocked[ni][nj])
                {
                    uf.unite({i, j}, {ni, nj});
                }
            }
        }
    }

    return !blocked[0][0] && !blocked[size - 1][size - 1] &&
           uf.connected(0, (size - 1) * size + (size - 1));
}

int solve2(vector<pair<int, int>> &pairs, int size = 7)
{
    int left = 0, right = pairs.size() - 1;
    int result = -1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (canReachEnd(pairs, mid + 1, size))
        {
            left = mid + 1;
            result = mid;
        }
        else
        {
            right = mid - 1;
        }
    }

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
    while (getline(file, line))
    {
        int a = stoi(line.substr(0, line.find(",")));
        int b = stoi(line.substr(line.find(",") + 1, line.length()));
        pairs.push_back(make_pair(a, b));
    }

    int ans = solve2(pairs, 71);
    if (ans >= 0)
    {
        cout << "Breaking pair: " << pairs[ans + 1].first << "," << pairs[ans + 1].second << endl;
    }

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3 << " ms."
         << endl;

    return 0;
}
