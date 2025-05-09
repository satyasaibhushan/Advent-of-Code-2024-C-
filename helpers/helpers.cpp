#include <string>
#include <algorithm>

using namespace std;

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
