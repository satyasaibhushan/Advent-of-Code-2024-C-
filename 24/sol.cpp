#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>

using namespace std;

long long solve(vector<tuple<string, string, string, string>> A, map<string, int> given)
{
    map<string, int> values;

    for (auto v : given)
    {
        values[v.first] = v.second;
    }
    // Keep processing gates until no new values are computed
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto [a, b, c, d] : A)
        {
            if (values.find(a) != values.end() && values.find(c) != values.end() && values.find(d) == values.end())
            {
                if (b == "XOR")
                {
                    values[d] = values[a] ^ values[c];
                }
                else if (b == "AND")
                {
                    values[d] = values[a] & values[c];
                }
                else if (b == "OR")
                {
                    values[d] = values[a] | values[c];
                }
                changed = true;
            }
        }
    }

    map<int, int> zValues;
    for (auto v : values)
    {
        if (v.first.size() > 1 && v.first[0] == 'z' && isdigit(v.first[1]))
        {
            int zIndex = stoi(v.first.substr(1));
            zValues[zIndex] = v.second;
        }
    }

    long long result = 0;
    for (auto it = zValues.rbegin(); it != zValues.rend(); ++it)
    {
        result = (result << 1) | it->second;
    }

    return result;
}

string solve2(vector<tuple<string, string, string, string>> A, int outliers_count)
{
    vector<tuple<string, string, string, string>> valid_gates;
    vector<tuple<string, string, string, string>> outliers;
    map<string, bool> outlier_map; // Track outliers by their output to avoid duplicates

    for (auto gate : A)
    {
        auto [a, b, c, d] = gate;
        bool is_outlier = false;

        bool has_x = (a.find("x") != string::npos || c.find("x") != string::npos);
        bool has_y = (a.find("y") != string::npos || c.find("y") != string::npos);
        bool has_z_output = (d.find("z") != string::npos);
        bool is_special_z00 = (d == "z00" && b == "XOR" &&
                               ((a == "y00" && c == "x00") || (a == "x00" && c == "y00")));
        bool is_special_z45 = (d == "z45" && b == "OR");

        // Rule 1: x and y in inputs should not have z output (except special case)
        if (has_x && has_y && has_z_output && !is_special_z00)
        {
            if (outlier_map.find(d) == outlier_map.end())
            {
                outliers.push_back(gate);
                outlier_map[d] = true;
                is_outlier = true;
            }
        }

        // Rule 2: z outputs require XOR operation (except special case)
        if (!is_outlier && has_z_output && b != "XOR" && !is_special_z45)
        {
            if (outlier_map.find(d) == outlier_map.end())
            {
                outliers.push_back(gate);
                outlier_map[d] = true;
                is_outlier = true;
            }
        }

        // Rule 3: XOR must follow valid patterns
        if (!is_outlier && b == "XOR")
        {
            bool is_valid_xy_xor = (has_x && has_y && !has_z_output);
            bool is_valid_z_xor = (has_z_output && !(has_x && has_y));

            if (!is_valid_xy_xor && !is_valid_z_xor && !is_special_z00)
            {
                if (outlier_map.find(d) == outlier_map.end())
                {
                    outliers.push_back(gate);
                    outlier_map[d] = true;
                    is_outlier = true;
                }
            }
        }

        if (!is_outlier)
        {
            valid_gates.push_back(gate);
        }
    }

    cout << "Initial outliers found: " << outliers.size() << endl;

    if (outliers.size() < outliers_count)
    {
        }

    cout << "Found " << outliers.size() << " total outliers:" << endl;
    for (auto [a, b, c, d] : outliers)
    {
        cout << a << " " << b << " " << c << " -> " << d << endl;
    }

    return "";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    vector<int> x;
    vector<int> y;
    map<string, int> given;
    vector<tuple<string, string, string, string>> A;
    ifstream file("./question");
    string line;

    // First pass: Process lines with colon format (key:value)
    while (getline(file, line))
    {
        size_t colon_pos = line.find(':');
        if (colon_pos != string::npos)
        {
            string key = line.substr(0, colon_pos);
            int value = stoi(line.substr(colon_pos + 1));

            given[key] = value;
        }
        else if (line.empty())
            break;
    }

    while (getline(file, line))
    {
        if (!line.empty())
        {
            stringstream ss(line);
            string a, b, c, arrow, d;
            ss >> a >> b >> c >> arrow >> d;

            if (!a.empty() && !b.empty() && !c.empty() && !d.empty())
            {
                A.push_back(make_tuple(a, b, c, d));
            }
        }
    }

    long long ans = solve(A, given);
    string ans2 = solve2(A, 8);

    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
