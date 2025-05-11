#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <queue>
#include <algorithm>

using namespace std;

struct State
{
    int v;
    int h;
    char c;
};

bool valid(int x, int y, vector<string> grid)
{
    if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size())
        return false;
    return true;
}

// Part A solution
int solve(vector<string> grid, pair<int, int> start, vector<pair<int, int>> moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        int x = start.first + moves[i].first;
        int y = start.second + moves[i].second;

        if (grid[x][y] == '#' || !valid(x, y, grid))
        {
            continue;
        }
        else if (grid[x][y] == 'O')
        {
            int last_x = x;
            int last_y = y;
            while (valid(last_x + moves[i].first, last_y + moves[i].second, grid) &&
                   grid[last_x + moves[i].first][last_y + moves[i].second] == 'O')
            {
                last_x += moves[i].first;
                last_y += moves[i].second;
            }
            int next_x = last_x + moves[i].first;
            int next_y = last_y + moves[i].second;
            if (valid(next_x, next_y, grid) && grid[next_x][next_y] == '.')
            {
                grid[next_x][next_y] = 'O';
                grid[x][y] = '@';
                grid[start.first][start.second] = '.';
                start = {x, y};
            }
            else
            {
                grid[start.first][start.second] = '@';
            }
        }
        else
        {
            grid[x][y] = '@';
            grid[start.first][start.second] = '.';
            start = {x, y};
        }
    }

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }

    // Calculate sum of GPS coordinates
    int sum = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'O')
            {
                sum += 100 * i + j;
            }
        }
    }
    return sum;
}

// Part B solution
long long solve2(vector<string> grid, pair<int, int> start, vector<pair<int, int>> moves)
{
    // Check if we need to transform the grid from part 1 format to part 2 format
    bool needs_transform = false;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'O')
            {
                needs_transform = true;
                break;
            }
        }
        if (needs_transform)
            break;
    }

    // Transform grid if needed
    if (needs_transform)
    {
        vector<string> new_grid;
        for (int i = 0; i < grid.size(); i++)
        {
            string row;
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (grid[i][j] == '#')
                {
                    row += "##";
                }
                else if (grid[i][j] == '.')
                {
                    row += "..";
                }
                else if (grid[i][j] == 'O')
                {
                    row += "[]";
                }
                else if (grid[i][j] == '@')
                {
                    row += "@.";
                    // Update start position
                    start = {i, row.size() - 2};
                }
            }
            new_grid.push_back(row);
        }
        grid = new_grid;
    }

    // Convert start position to (v, h) coordinates
    int v = start.first;
    int h = start.second;

    // Keep track of box positions as a map of box centers
    map<pair<int, int>, bool> box_positions;

    // Initialize box positions
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size() - 1; j++)
        {
            if (grid[i][j] == '[' && grid[i][j + 1] == ']')
            {
                // Record box position by its left bracket
                box_positions[{i, j}] = true;
            }
        }
    }

    // Process each move
    for (int i = 0; i < moves.size(); i++)
    {
        int dx = moves[i].first;
        int dy = moves[i].second;

        // Store move for debugging
        cout << "Move " << i << ": (" << dx << "," << dy << ")" << endl;

        // Up
        if (dx == -1 && dy == 0)
        {
            // Check if robot can move up
            if (v > 0 && grid[v - 1][h] != '#')
            {
                // If there's a box above, check if we can push it
                if (grid[v - 1][h] == '[' && grid[v - 1][h + 1] == ']')
                {
                    int row = v - 1;
                    // Try to push boxes upward
                    bool can_push = true;
                    vector<pair<int, int>> boxes_to_push;

                    // Find all boxes that need to be pushed
                    while (row >= 0 && grid[row][h] == '[' && grid[row][h + 1] == ']')
                    {
                        boxes_to_push.push_back({row, h});
                        row--;
                        if (row < 0 || grid[row][h] == '#' || grid[row][h + 1] == '#')
                        {
                            can_push = false;
                            break;
                        }
                    }

                    if (can_push && !boxes_to_push.empty())
                    {
                        // Push all boxes up one position (starting from topmost box)
                        for (int i = 0; i < boxes_to_push.size(); i++)
                        {
                            auto box = boxes_to_push[i];
                            // Erase old box
                            grid[box.first][box.second] = '.';
                            grid[box.first][box.second + 1] = '.';
                            // Mark new position
                            grid[box.first - 1][box.second] = '[';
                            grid[box.first - 1][box.second + 1] = ']';
                            // Update box position
                            box_positions.erase({box.first, box.second});
                            box_positions[{box.first - 1, box.second}] = true;
                        }
                        // Move robot up
                        grid[v][h] = '.';
                        v--;
                        grid[v][h] = '@';
                    }
                }
                else if (grid[v - 1][h] == '.')
                {
                    // Empty space, move robot
                    grid[v][h] = '.';
                    v--;
                    grid[v][h] = '@';
                }
            }
        }
        // Right
        else if (dx == 0 && dy == 1)
        {
            // Check if robot can move right
            if (h < grid[v].size() - 1 && grid[v][h + 1] != '#')
            {
                // If there's a box to the right, check if we can push it
                if (grid[v][h + 1] == '[' && h + 2 < grid[v].size() && grid[v][h + 2] == ']')
                {
                    // Find all consecutive boxes starting at this position
                    vector<pair<int, int>> boxes_to_push;
                    int col = h + 1;
                    bool can_push = true;

                    // First box
                    boxes_to_push.push_back({v, col});
                    col += 2;

                    // Look for more boxes
                    while (col < grid[v].size() - 1 && grid[v][col] == '[' && grid[v][col + 1] == ']')
                    {
                        boxes_to_push.push_back({v, col});
                        col += 2;
                    }

                    // Check if there's enough space after the last box
                    if (col < grid[v].size() && grid[v][col] == '#')
                    {
                        can_push = false;
                    }

                    if (can_push && !boxes_to_push.empty())
                    {
                        // Push all boxes right (starting from rightmost box)
                        for (int i = boxes_to_push.size() - 1; i >= 0; i--)
                        {
                            auto box = boxes_to_push[i];
                            // Erase old box
                            grid[box.first][box.second] = '.';
                            grid[box.first][box.second + 1] = '.';
                            // Mark new position
                            grid[box.first][box.second + 2] = '[';
                            grid[box.first][box.second + 3] = ']';
                            // Update box position
                            box_positions.erase({box.first, box.second});
                            box_positions[{box.first, box.second + 2}] = true;
                        }
                        // Move robot right
                        grid[v][h] = '.';
                        h++;
                        grid[v][h] = '@';
                    }
                }
                else if (grid[v][h + 1] == '.')
                {
                    // Empty space, move robot
                    grid[v][h] = '.';
                    h++;
                    grid[v][h] = '@';
                }
            }
        }
        // Down
        else if (dx == 1 && dy == 0)
        {
            // Check if robot can move down
            if (v < grid.size() - 1 && grid[v + 1][h] != '#')
            {
                // If there's a box below, check if we can push it
                if (grid[v + 1][h] == '[' && grid[v + 1][h + 1] == ']')
                {
                    int row = v + 1;
                    // Try to push boxes downward
                    bool can_push = true;
                    vector<pair<int, int>> boxes_to_push;

                    // Find all boxes that need to be pushed
                    while (row < grid.size() && grid[row][h] == '[' && grid[row][h + 1] == ']')
                    {
                        boxes_to_push.push_back({row, h});
                        row++;
                        if (row >= grid.size() || grid[row][h] == '#' || grid[row][h + 1] == '#')
                        {
                            can_push = false;
                            break;
                        }
                    }

                    if (can_push && !boxes_to_push.empty())
                    {
                        // Push all boxes down one position (starting from bottommost box)
                        for (int i = boxes_to_push.size() - 1; i >= 0; i--)
                        {
                            auto box = boxes_to_push[i];
                            // Erase old box
                            grid[box.first][box.second] = '.';
                            grid[box.first][box.second + 1] = '.';
                            // Mark new position
                            grid[box.first + 1][box.second] = '[';
                            grid[box.first + 1][box.second + 1] = ']';
                            // Update box position
                            box_positions.erase({box.first, box.second});
                            box_positions[{box.first + 1, box.second}] = true;
                        }
                        // Move robot down
                        grid[v][h] = '.';
                        v++;
                        grid[v][h] = '@';
                    }
                }
                else if (grid[v + 1][h] == '.')
                {
                    // Empty space, move robot
                    grid[v][h] = '.';
                    v++;
                    grid[v][h] = '@';
                }
            }
        }
        // Left
        else if (dx == 0 && dy == -1)
        {
            // Check if robot can move left
            if (h > 0 && grid[v][h - 1] != '#')
            {
                // If there's a box to the left, check if we can push it
                if (h > 1 && grid[v][h - 2] == '[' && grid[v][h - 1] == ']')
                {
                    // Find all consecutive boxes starting at this position
                    vector<pair<int, int>> boxes_to_push;
                    int col = h - 2;
                    bool can_push = true;

                    // First box
                    boxes_to_push.push_back({v, col});
                    col -= 2;

                    // Look for more boxes
                    while (col > 0 && grid[v][col] == '[' && grid[v][col + 1] == ']')
                    {
                        boxes_to_push.push_back({v, col});
                        col -= 2;
                    }

                    // Check if there's enough space after the last box
                    if (col < 0 || grid[v][col] == '#')
                    {
                        can_push = false;
                    }

                    if (can_push && !boxes_to_push.empty())
                    {
                        // Push all boxes left (starting from leftmost box)
                        for (int i = boxes_to_push.size() - 1; i >= 0; i--)
                        {
                            auto box = boxes_to_push[i];
                            // Erase old box
                            grid[box.first][box.second] = '.';
                            grid[box.first][box.second + 1] = '.';
                            // Mark new position
                            grid[box.first][box.second - 2] = '[';
                            grid[box.first][box.second - 1] = ']';
                            // Update box position
                            box_positions.erase({box.first, box.second});
                            box_positions[{box.first, box.second - 2}] = true;
                        }
                        // Move robot left
                        grid[v][h] = '.';
                        h--;
                        grid[v][h] = '@';
                    }
                }
                else if (grid[v][h - 1] == '.')
                {
                    // Empty space, move robot
                    grid[v][h] = '.';
                    h--;
                    grid[v][h] = '@';
                }
            }
        }
    }

    // Debug: Final grid check
    int left_brackets = 0;
    int right_brackets = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j];
            if (grid[i][j] == '[')
                left_brackets++;
            if (grid[i][j] == ']')
                right_brackets++;
        }
        cout << endl;
    }

    if (left_brackets != right_brackets)
    {
        cout << "WARNING: Bracket mismatch! Grid may be corrupted." << endl;
    }

    // Calculate sum of GPS coordinates by searching the final grid for left brackets
    long long sum = 0;
    int box_count = 0;

    // Search the entire grid for '[' characters
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '[')
            {
                box_count++;

                // Calculate GPS coordinate using the left bracket position
                int original_col = j / 2;
                int gps = 100 * i + original_col;
                cout << "Box at (" << i << ", " << j << ") Original col: " << original_col << " GPS: " << gps << endl;
                sum += gps;
            }
        }
    }

    cout << "Total boxes found: " << box_count << endl;

    return sum;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question2");

    string line;

    long long ans = 0;

    vector<string> grid;
    pair<int, int> startp;
    vector<pair<int, int>> moves;
    bool done = false;
    int count = 0;

    while (getline(file, line))
    {
        vector<char> numbers;
        stringstream ss(line);
        char num;
        string s;

        if (line.size() == 0)
        {
            done = true;
            continue;
        }

        if (done)
        {
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == '<')
                    moves.push_back(make_pair(0, -1));
                if (line[i] == '>')
                    moves.push_back(make_pair(0, 1));
                if (line[i] == '^')
                    moves.push_back(make_pair(-1, 0));
                if (line[i] == 'v')
                    moves.push_back(make_pair(1, 0));
            }
        }

        else
        {

            for (int i = 0; i < line.size(); i++)
            {

                if (line[i] == '#')
                    s += '#';
                if (line[i] == 'O')
                    s += 'O';
                if (line[i] == '@')
                {
                    startp = make_pair(count, i);
                    s += '.';
                }
                if (line[i] == '.')
                    s += '.';
                if (line[i] == '[')
                    s += '[';
                if (line[i] == ']')
                    s += ']';
            }
            count++;
            grid.push_back(s);
        }
    }

    ans = solve2(grid, startp, moves);
    cout << ans << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
