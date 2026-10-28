// Tutoriat SDA - 28.10.2026
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {5, 3, 8, 1, 9, 2};
    sort(v.begin(), v.end());

    cout << "Merge C++! Vector sortat: ";
    for (int x : v) {
        cout << x << ' ';
    }
    cout << '\n';
    return 0;
}
