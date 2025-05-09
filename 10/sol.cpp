#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

vector<pair<int, int>> dir = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int row, col;
bool isValid(int x, int y)
{
    return x >= 0 && x < row && y >= 0 && y < col;
}

int getScore(vector<vector<int>> A, int x, int y, int curr, vector<vector<bool>> &visited, bool part2 = true)
{
    int ans = 0;
    if (curr == 9)
    {
        if (!part2)
            visited[x][y] = true;
        return 1;
    }
    if (curr > 9)
        return 0;

    ans = 0; // Count current point

    for (int i = 0; i < dir.size(); i++)
    {
        int nx = x + dir[i].first;
        int ny = y + dir[i].second;
        if (isValid(nx, ny) && A[nx][ny] == curr + 1 && !visited[nx][ny])
        {
            ans += getScore(A, nx, ny, curr + 1, visited, part2);
        }
    }
    return ans;
}

int solve(vector<vector<int>> A, bool part2 = false)
{
    int ans = 0;
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A[i].size(); j++)
        {
            vector<vector<bool>> visited(A.size(), vector<bool>(A[0].size(), false));
            if (A[i][j] == 0)
            {
                ans += getScore(A, i, j, 0, visited, part2);
            }
        }
    }
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
    vector<vector<int>> A;
    while (getline(file, line))
    {
        vector<int> numbers;
        stringstream ss(line);
        char num;

        while (ss >> num)
        {
            numbers.push_back(num - '0');
        }
        A.push_back(numbers);
    }
    row = A.size();
    col = A[0].size();
    int ans = solve(A);
    int ans2 = solve(A, true);

    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
