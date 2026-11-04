// c6 - quickselect si mediana medianelor (al k-lea cel mai mic element)
// compilare: clang++ -std=c++17 -Wall -Wextra -O2 quickselect.cpp -o quickselect
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

mt19937 rng(42);

int random_int(int lo, int hi) {  // numar aleator din [lo, hi]
    return uniform_int_distribution<int>(lo, hi)(rng);
}

// lomuto cu pivot aleator, returneaza pozitia finala a pivotului
int partition_lomuto(vector<int>& a, int lo, int hi) {
    swap(a[random_int(lo, hi)], a[hi]);
    int p = a[hi];
    int i = lo;
    for (int j = lo; j < hi; j++) {
        if (a[j] < p) {
            swap(a[i], a[j]);
            i++;
        }
    }
    swap(a[i], a[hi]);
    return i;
}

// al k-lea cel mai mic element (k de la 1 la n), O(n) in medie, O(n^2) in cel mai rau caz
// luam vectorul prin copie ca sa nu-l stricam pe cel original
int quickselect(vector<int> a, int k) {
    int target = k - 1, lo = 0, hi = (int)a.size() - 1;
    while (true) {
        int pos = partition_lomuto(a, lo, hi);
        if (pos == target) return a[pos];
        if (target < pos) hi = pos - 1;  // raspunsul e in stanga
        else lo = pos + 1;               // raspunsul e in dreapta
    }
}

// mediana medianelor: O(n) si in cel mai rau caz
// partitionarea face vectori noi (nu e in-place), dar e mai usor de urmarit asa
int select_mom(const vector<int>& a, int k) {
    int n = (int)a.size();
    if (n <= 5) {
        vector<int> b = a;
        sort(b.begin(), b.end());
        return b[k - 1];
    }

    // 1-2. medianele grupelor de cate 5
    vector<int> medians;
    for (int i = 0; i < n; i += 5) {
        vector<int> group(a.begin() + i, a.begin() + min(i + 5, n));
        sort(group.begin(), group.end());
        medians.push_back(group[group.size() / 2]);
    }

    // 3. mediana medianelor, recursiv
    int x = select_mom(medians, ((int)medians.size() + 1) / 2);

    // 4. partitionare in 3 dupa x
    vector<int> less, greater;
    int equal = 0;
    for (int e : a) {
        if (e < x) less.push_back(e);
        else if (e > x) greater.push_back(e);
        else equal++;
    }

    int nless = (int)less.size();
    if (k <= nless) return select_mom(less, k);
    if (k <= nless + equal) return x;
    return select_mom(greater, k - nless - equal);
}

int main() {
    vector<int> v = {7, 10, 4, 3, 20, 15, 8, 1, 12};
    int n = (int)v.size();

    vector<int> sorted_v = v;
    sort(sorted_v.begin(), sorted_v.end());
    cout << "sortat:      ";
    for (int x : sorted_v) cout << x << ' ';
    cout << "\nquickselect: ";
    for (int k = 1; k <= n; k++) cout << quickselect(v, k) << ' ';
    cout << "\nmom:         ";
    for (int k = 1; k <= n; k++) cout << select_mom(v, k) << ' ';
    cout << '\n';

    // testare: comparam cu vectorul sortat
    for (int t = 0; t < 300; t++) {
        int m = random_int(1, 80);
        vector<int> w(m);
        for (int& x : w) x = random_int(-30, 30);
        vector<int> s = w;
        sort(s.begin(), s.end());
        for (int k = 1; k <= m; k++) {
            assert(quickselect(w, k) == s[k - 1]);
            assert(select_mom(w, k) == s[k - 1]);
        }
    }
    cout << "toate testele au trecut\n\n";

    // timpi pentru mediana a 2 milioane de numere
    int big_n = 2'000'000, k = big_n / 2;
    vector<int> big(big_n);
    for (int& x : big) x = random_int(0, 1'000'000'000);

    auto measure = [&](const char* name, auto fn) {
        auto start = chrono::steady_clock::now();
        int result = fn();
        auto end = chrono::steady_clock::now();
        cout << name << result << "  (" << chrono::duration<double, milli>(end - start).count() << " ms)\n";
    };
    cout << "mediana a " << big_n << " de numere:\n";
    measure("  quickselect:        ", [&] { return quickselect(big, k); });
    measure("  mediana medianelor: ", [&] { return select_mom(big, k); });
    measure("  std::nth_element:   ", [&] {
        vector<int> b = big;
        nth_element(b.begin(), b.begin() + (k - 1), b.end());
        return b[k - 1];
    });
    measure("  std::sort:          ", [&] {
        vector<int> b = big;
        sort(b.begin(), b.end());
        return b[k - 1];
    });
    return 0;
}
