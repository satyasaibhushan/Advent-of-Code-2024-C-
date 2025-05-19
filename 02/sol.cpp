#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
using namespace std;

int solve(vector<int> A)
{
    int n = A.size();
    int inc = 1;

    if (n == 1 || n == 0)
        return 1;

    if (A[0] > A[1])
        inc = -1;
    if (A[0] == A[1])
        return 0;

    for (int i = 1; i <= n - 1; i++)
    {
        int diff = A[i] - A[i - 1];
        if (diff == 0)
            return 0;
        if (diff != inc && diff != inc * 2 && diff != inc * 3)
            return 0;
    }

    return 1;
}
bool is_valid(const vector<int>& v, int l, int r) {
    if (r-l < 1) return true;
    bool inc = true, dec = true;
    for (int i = l+1; i <= r; ++i) {
        int d = v[i] - v[i-1];
        if (abs(d) < 1 || abs(d) > 3)
            return false;
        if (d <= 0) inc = false;
        if (d >= 0) dec = false;
    }
    return inc || dec;
}

int solve2(vector<int> v)
{
    int n = v.size();
    // Check original
    if (is_valid(v, 0, n-1)) return true;

    // Find the first bad index
    for (int i = 1; i < n; ++i) {
        int d = v[i] - v[i-1];
        if (abs(d) < 1 || abs(d) > 3 || (v[i] == v[i-1])) {
            // Try removing v[i]
            vector<int> tmp = v;
            tmp.erase(tmp.begin() + i);
            if (is_valid(tmp, 0, n-2)) return true;
            // Try removing v[i-1]
            tmp = v;
            tmp.erase(tmp.begin() + i - 1);
            if (is_valid(tmp, 0, n-2)) return true;
            return false; // Can't fix with one removal
        }
        // Also check monotonicity break
        if (i >= 2) {
            // If removing v[i-2] or v[i] helps
            if (!is_valid(v, i-2, i)) {
                // Try removing v[i-2]
                vector<int> tmp = v;
                tmp.erase(tmp.begin() + i - 2);
                if (is_valid(tmp, 0, n-2)) return true;
                // Try removing v[i]
                tmp = v;
                tmp.erase(tmp.begin() + i);
                if (is_valid(tmp, 0, n-2)) return true;
                return false;
            }
        }
    }
    return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream file("./question");
    string line;

    int ans = 0;
    int ans2 = 0;

    while (getline(file, line))
    {
        vector<int> numbers;
        stringstream ss(line);
        int num;

        while (ss >> num)
        {
            numbers.push_back(num);
        }

        ans += solve(numbers);
        ans2 += solve2(numbers);
    }
    cout << ans << endl;
    cout << ans2 << endl;

    return 0;
}
