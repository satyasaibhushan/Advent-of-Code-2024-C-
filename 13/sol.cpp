#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

typedef long long ll;

int solve(pair<int, int> A, pair<int, int> B, pair<int, int> P)
{
    if (((A.first + B.first) * 100 < P.first) || ((A.second + B.second) * 100 < P.second))
    {
        return 0;
    }
    map<pair<int, int>, int> m;
    m[make_pair(0, 0)] = 0;
    vector<tuple<int, int, int, int>> v;
    v.push_back(make_tuple(A.first, A.second, 1, 0));
    v.push_back(make_tuple(B.first, B.second, 0, 1));

    for (int i = 0; i < 200; i++)
    {
        vector<tuple<int, int, int, int>> v1 = v;
        int x, y;
        for (int j = 0; j < v.size(); j++)
        {
            if (get<2>(v[j]) <= 300)
            {

                x = get<0>(v[j]) + A.first;
                y = get<1>(v[j]) + A.second;
                if (x == P.first && y == P.second)
                {
                    return get<2>(v[j]) + 3 + get<3>(v[j]);
                }
                if (x < P.first && y < P.second && m.find({x, y}) == m.end())
                {
                    v1.push_back(make_tuple(x, y, get<2>(v[j]) + 3, get<3>(v[j])));
                    m[{x, y}] = 1;
                }
            }

            if (get<3>(v[j]) <= 100)
            {
                x = get<0>(v[j]) + B.first;
                y = get<1>(v[j]) + B.second;
                if (x == P.first && y == P.second)
                {
                    return get<2>(v[j]) + 3 + get<3>(v[j]);
                }
                if (x < P.first && y < P.second && m.find({x, y}) == m.end())
                {
                    v1.push_back(make_tuple(x, y, get<2>(v[j]), get<3>(v[j]) + 1));
                    m[{x, y}] = 1;
                }
            }
            v = v1;
        }
    }
    return 0;
}

pair<ll, ll> solve2(pair<int, int> A, pair<int, int> B, pair<long long, long long> P)
{
    ll A_x = A.first, A_y = A.second, B_x = B.first, B_y = B.second, P_x = P.first, P_y = P.second;

    ll D = A_x * B_y - A_y * B_x;

    ll a = (P_x * B_y - P_y * B_x);
    ll b = (A_x * P_y - A_y * P_x);

    if (D == 0 || a % D != 0 || b % D != 0)
    {
        return make_pair(0, 0);
    }
    a /= D;
    b /= D;

    return make_pair(a, b);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<pair<int, int>> pairs;
    ifstream file("./question");

    string line;

    long long ans = 0;
    long long ans2 = 0;

    int i = 0;
    pair<int, int> A, B, P;
    while (getline(file, line))
    {
        stringstream ss(line);
        if (line.size() == 0)
        {
            i = 0;
            continue;
        }
        if (i % 3 == 0)
        {
            A.first = stoi(line.substr(line.find("X") + 2, line.find(",") - line.find("X") - 2));
            A.second = stoi(line.substr(line.find("Y") + 2));
        }
        else if (i % 3 == 1)
        {
            B.first = stoi(line.substr(line.find("X") + 2, line.find(",") - line.find("X") - 2));
            B.second = stoi(line.substr(line.find("Y") + 2));
        }
        else
        {
            P.first = stoi(line.substr(line.find("X") + 2, line.find("=") - line.find("X") - 2));
            P.second = stoi(line.substr(line.find("Y") + 2));
            const long long shift = 10000000000000LL;
            // int k = solve(A, B, P);
            int k = 0;

            pair<ll, ll> k1 = solve2(A, B, {P.first + shift, P.second + shift});
            ans += k;
            ans2 += k1.first * 3 + k1.second;
        }
        i++;
    }

    cout << ans << endl;
    cout << ans2 << endl;
    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
