#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
using namespace std;

long long solve(string s)
{
    int start = 0, n = s.size();
    string beginning = "mul(";
    int n1, n2;
    string temp;
    long long ans = 0;
    for (int i = 0; i < n; i++)
    {
        if (s.substr(i, beginning.size()) == beginning)
        {
            int comma = s.find(",", i + beginning.size());
            temp = s.substr(i + beginning.size(), comma - i - beginning.size());
            if (temp.size() > 0 && temp.size() < 4)
            {
                n1 = stoi(temp);
                if (n1 > 1000)
                {
                    i = comma + 1;
                    continue;
                }
                temp = s.substr(comma + 1, s.find(")", comma + 1) - comma - 1);
                if (temp.size() > 0 && temp.size() < 4)
                {
                    n2 = stoi(temp);
                    if (n2 > 1000)
                    {
                        i = comma + 1;
                        continue;
                    }
                    ans += n1 * n2;
                    i = comma + 1;
                }
            }
        }
    }

    return ans;
}

long long solve2(string s)
{
    int start = 0, n = s.size();
    string m1 = "do()";
    string m2 = "don't()";
    int end;
    long long ans = 0;
    bool flag = false;
    while (start < n)
    {
        if (flag)
        {
            start = s.find(m1, start);
            if (start == string::npos)
            {
                return ans;
            }
            flag = false;
        }
        else
        {
            end = s.find(m2, start);
            if (end == string::npos)
            {
                ans += solve(s.substr(start));
                return ans;
            }
            else{
                ans += solve(s.substr(start, end - start));
                start = end + m2.size();
            }
            flag = true;
        }
    }
    return ans;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream file("./question");

    string line;

    int ans = 0;
    long long ans2 = 0;
    string k = "";
    while (getline(file, line))
    {
        ans += solve(line);
        k+=line;
    }
    ans2 = solve2(k);
    cout << ans << endl;
    cout << ans2 << endl;
    return 0;
}
