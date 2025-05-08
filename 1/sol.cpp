#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
using namespace std;

void solve(vector<int> A, vector<int> B) {

    int n = A.size();
    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += abs(A[i] - B[i]);
    }
    cout << "ans1:" << ans << endl;

    long long ans2 = 0;
    map<int, int> freqA, freqB;
    for (int i = 0; i < n; i++) {
        freqA[A[i]]++;
        freqB[B[i]]++;
    }

    for (auto [key, value] : freqA) {
        ans2 += key * value * freqB[key];
    }
    cout << "ans2:" << ans2 << endl;
}

int main() {    
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<pair<int, int>> pairs;
    ifstream file("./question");
    
    vector<int> A, B;
    int a, b;
    while (file >> a >> b) {
        A.push_back(a);
        B.push_back(b);
    }

    solve(A, B);
    
    cout << "Read " << A.size() << " "<<B.size() << " numbers\n";
    
    return 0;
}
