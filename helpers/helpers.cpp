#include <string>
#include <algorithm>
#include <vector>

using namespace std;

string removeLeadingZeros(string s)
{
    size_t pos = s.find_first_not_of('0');
    if (pos == string::npos)
        return "0";
    return s.substr(pos);
}

string addStrings(string num1, string num2)
{
    string result = "";
    int carry = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0 || j >= 0 || carry)
    {
        int digit1 = (i >= 0) ? num1[i] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;
        int sum = digit1 + digit2 + carry;

        carry = sum / 10;
        result += to_string(sum % 10);

        i--;
        j--;
    }

    reverse(result.begin(), result.end());
    return result;
}

string multiplyStrings(string num1, string num2)
{
    if (num1 == "0" || num2 == "0")
        return "0";
    int n = num1.size(), m = num2.size();
    vector<int> result(n + m, 0);

    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = m - 1; j >= 0; --j)
        {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string res = "";
    for (int num : result)
    {
        if (!(res.empty() && num == 0))
            res += to_string(num);
    }
    return res;
}