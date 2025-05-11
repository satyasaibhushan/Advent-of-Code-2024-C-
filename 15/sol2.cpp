#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

void part_one()
{
}

struct State
{
    int v;
    int h;
    char c;
};

void part_two()
{

    vector<string> input;
    ifstream file("./question");
    string line;
    while (getline(file, line))
    {
        input.push_back(line);
    }

    vector<string> grid;

    // Create the grid
    int counter = 0;
    while (input[counter] != "")
    {
        string s;
        for (int i = 0; i < input[counter].size(); i++)
        {
            if (input[counter][i] == '.' || input[counter][i] == '#')
            {
                s += input[counter][i];
                s += input[counter][i];
            }
            else if (input[counter][i] == 'O')
            {
                s += "[]";
            }
            else if (input[counter][i] == '@')
            {
                s += "@.";
            }
        }
        grid.push_back(s);
        counter++;
    }

    counter++;

    // Create the instructions
    string instructions;
    while (counter < input.size())
    {
        instructions += input[counter++];
    }

    // Find initial position
    int v, h;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '@')
            {
                v = i;
                h = j;
            }
        }
    }

    cout << v << endl;
    cout << h << endl;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < instructions.size(); i++)
    {

        // cout << instructions[i] << endl;

        if (instructions[i] == '^')
        {

            auto initial_state = State{.v = v, .h = h, .c = '@'};
            vector<State> to_move;
            queue<State> state_queue;

            state_queue.push(initial_state);

            while (!state_queue.empty())
            {

                auto state = state_queue.front();
                state_queue.pop();
                to_move.push_back(state);

                if (grid[state.v - 1][state.h] == ']')
                {
                    state_queue.push(State{.v = state.v - 1, .h = state.h - 1, .c = '['});
                    state_queue.push(State{.v = state.v - 1, .h = state.h, .c = ']'});
                }
                else if (grid[state.v - 1][state.h] == '[')
                {
                    state_queue.push(State{.v = state.v - 1, .h = state.h, .c = '['});
                    state_queue.push(State{.v = state.v - 1, .h = state.h + 1, .c = ']'});
                }
                else if (grid[state.v - 1][state.h] == '.')
                {
                }
                else if (grid[state.v - 1][state.h] == '#')
                {
                    // If we hit a wall we can't move any of the boxes up,
                    // so just stop
                    queue<State> empty_queue;
                    swap(state_queue, empty_queue);
                    vector<State> empty_vector;
                    swap(to_move, empty_vector);
                }
                else
                {
                    cout << "Panic!" << endl;
                }
            }

            if (!to_move.empty())
            {
                for (auto state : to_move)
                {
                    grid[state.v][state.h] = '.';
                }
                for (auto state : to_move)
                {
                    grid[state.v - 1][state.h] = state.c;
                }

                v = v - 1;
            }
        }
        // Sideways should be the same
        if (instructions[i] == '>')
        {

            int curr_h = h + 1;
            while (curr_h < grid[v].size() && (grid[v][curr_h] == '[' || grid[v][curr_h] == ']'))
            {
                curr_h++;
            }

            if (curr_h < grid[v].size() && grid[v][curr_h] == '.')
            {
                while (curr_h > h)
                {
                    grid[v][curr_h] = grid[v][curr_h - 1];
                    curr_h--;
                }
                grid[v][h] = '.';
                h = h + 1;
            }
        }
        if (instructions[i] == 'v')
        {

            auto initial_state = State{.v = v, .h = h, .c = '@'};
            vector<State> to_move;
            queue<State> state_queue;

            state_queue.push(initial_state);

            while (!state_queue.empty())
            {

                auto state = state_queue.front();
                state_queue.pop();
                to_move.push_back(state);

                if (grid[state.v + 1][state.h] == ']')
                {
                    state_queue.push(State{.v = state.v + 1, .h = state.h - 1, .c = '['});
                    state_queue.push(State{.v = state.v + 1, .h = state.h, .c = ']'});
                }
                else if (grid[state.v + 1][state.h] == '[')
                {
                    state_queue.push(State{.v = state.v + 1, .h = state.h, .c = '['});
                    state_queue.push(State{.v = state.v + 1, .h = state.h + 1, .c = ']'});
                }
                else if (grid[state.v + 1][state.h] == '.')
                {
                }
                else if (grid[state.v + 1][state.h] == '#')
                {
                    // If we hit a wall we can't move any of the boxes up,
                    // so just stop
                    queue<State> empty_queue;
                    swap(state_queue, empty_queue);
                    vector<State> empty_vector;
                    swap(to_move, empty_vector);
                }
                else
                {
                    cout << "Panic!" << endl;
                }
            }

            if (!to_move.empty())
            {
                for (auto state : to_move)
                {
                    grid[state.v][state.h] = '.';
                }
                for (auto state : to_move)
                {
                    grid[state.v + 1][state.h] = state.c;
                }

                v = v + 1;
            }
        }
        if (instructions[i] == '<')
        {

            int curr_h = h - 1;
            while (curr_h >= 0 && (grid[v][curr_h] == '[' || grid[v][curr_h] == ']'))
            {
                curr_h--;
            }

            // cout << curr_h << endl;

            if (curr_h >= 0 && grid[v][curr_h] == '.')
            {
                while (curr_h < h)
                {
                    grid[v][curr_h] = grid[v][curr_h + 1];
                    curr_h++;
                }
                grid[v][h] = '.';
                h = h - 1;
            }
        }

        // for(int i = 0; i < grid.size(); i++) {
        //     for(int j = 0; j < grid[i].size(); j++) {
        //         cout << grid[i][j];
        //     }
        //     cout << endl;
        // }
        // cout << endl;
    }

    long total = 0;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '[')
            {
                total += (100 * i) + j;
            }
        }
        // cout << endl;
    }

    cout << "Part two: " << total << endl;
}

int main()
{

    part_one();
    part_two();

    return 0;
}