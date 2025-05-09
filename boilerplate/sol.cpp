#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

int solve(vector<char> A)
{
    // Your solution here
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    // vector<int> A;
    // int a;
    // while (file >> a ) {
    //     A.push_back(a);
    // }

    // solve(A);
    string line;

    int ans = 0;

    while (getline(file, line))
    {
        vector<char> numbers;
        stringstream ss(line);
        char num;

        while (ss >> num)
        {
            numbers.push_back(num);
        }

        ans += solve(numbers);
    }
    cout << ans << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
