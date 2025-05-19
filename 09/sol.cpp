#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include "../helpers/helpers.cpp"

using namespace std;

string solve(string line)
{
    vector<int> curr;
    vector<int> numbers;
    vector<int> gaps;
    for (int i = 0; i < line.size(); i++)
    {
        if (i % 2 == 0)
        {
            numbers.push_back(line[i] - '0');
        }
        else
        {
            gaps.push_back(line[i] - '0');
        }
    }

    int gap = 0;
    int k = numbers.size();
    for (int i = 0; i < k; i++)
    {
        while (gap > 0 && i < numbers.size())
        {
            if (numbers.size() == 0)
            {
                break;
            }

            if (numbers[numbers.size() - 1] > gap)
            {
                for (int j = 0; j < gap; j++)
                {
                    curr.push_back(numbers.size() - 1);
                }
                numbers[numbers.size() - 1] -= gap;
                gap = 0;
            }
            else
            {
                for (int j = 0; j < numbers[numbers.size() - 1]; j++)
                {
                    curr.push_back(numbers.size() - 1);
                }
                gap -= numbers[numbers.size() - 1];
                numbers.pop_back();
                k = numbers.size();
            }
        }
        for (int j = 0; j < numbers[i]; j++)
        {
            curr.push_back(i);
        }
        if (i < gaps.size())
        {
            gap += gaps[i];
        }
    }

    long long ans = 0;
    for (int i = 0; i < curr.size(); i++)
    {
        ans += i * curr[i];
    }
    return to_string(ans);
}

string solve2(string line)
{
    vector<int> disk;
    vector<int> files;
    vector<int> spaces;

    for (int i = 0; i < line.size(); i++)
    {
        if (i % 2 == 0)
            files.push_back(line[i] - '0');
        else
            spaces.push_back(line[i] - '0');
    }

    for (int i = 0; i < files.size(); i++)
    {
        for (int j = 0; j < files[i]; j++)
            disk.push_back(i);
        if (i < spaces.size())
            for (int j = 0; j < spaces[i]; j++)
                disk.push_back(-1);
    }

    for (int fileId = files.size() - 1; fileId >= 0; fileId--)
    {
        int fileSize = files[fileId];
        int filePos = -1;

        for (int i = 0; i < disk.size(); i++)
            if (disk[i] == fileId)
            {
                filePos = i;
                break;
            }

        if (filePos == -1)
            continue;

        int spaceStart = -1;
        int spaceCount = 0;

        for (int i = 0; i < filePos; i++)
        {
            if (disk[i] == -1)
            {
                if (spaceStart == -1)
                    spaceStart = i;
                spaceCount++;
                if (spaceCount >= fileSize)
                {
                    for (int j = 0; j < fileSize; j++)
                        disk[spaceStart + j] = fileId;
                    for (int j = filePos; j < filePos + fileSize; j++)
                        disk[j] = -1;
                    break;
                }
            }
            else
            {
                spaceStart = -1;
                spaceCount = 0;
            }
        }
    }

    long long ans = 0;
    for (int i = 0; i < disk.size(); i++)
        if (disk[i] != -1)
            ans += i * disk[i];
    return to_string(ans);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    ifstream file("./question");
    string line;
    string ans = "";
    string ans2 = "";

    while (getline(file, line))
    {
        ans = solve(line);
        ans2 = solve2(line);
    }
    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
