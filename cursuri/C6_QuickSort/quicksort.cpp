// c6 - quicksort: partitionare lomuto, hoare si in 3 (pt elemente egale)
// compilare: clang++ -std=c++17 -Wall -Wextra -O2 quicksort.cpp -o quicksort
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

void print(const vector<int>& v) {
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// lomuto cu pivot aleator (il mutam pe ultima pozitie)
// returneaza pozitia finala k a pivotului: a[lo..k-1] < a[k] <= a[k+1..hi]
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
    swap(a[i], a[hi]);  // pivotul ajunge pe locul lui final
    return i;
}

// hoare cu pivotul din mijloc
// returneaza j: a[lo..j] <= p <= a[j+1..hi] (pivotul nu e neaparat pe pozitia j!)
int partition_hoare(vector<int>& a, int lo, int hi) {
    int p = a[lo + (hi - lo) / 2];
    int i = lo - 1, j = hi + 1;
    while (true) {
        do i++; while (a[i] < p);
        do j--; while (a[j] > p);
        if (i >= j) return j;
        swap(a[i], a[j]);
    }
}

void quicksort_lomuto(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int k = partition_lomuto(a, lo, hi);
    quicksort_lomuto(a, lo, k - 1);  // a[k] e deja la locul lui
    quicksort_lomuto(a, k + 1, hi);
}

void quicksort_hoare(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int j = partition_hoare(a, lo, hi);
    quicksort_hoare(a, lo, j);  // atentie: j inclus!
    quicksort_hoare(a, j + 1, hi);
}

// partitionare in 3: a[lo..lt-1] < p, a[lt..gt] == p, a[gt+1..hi] > p
// pe zona cu elemente egale cu pivotul nu mai facem recursivitate
void quicksort_3way(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int p = a[random_int(lo, hi)];
    int lt = lo, i = lo, gt = hi;
    while (i <= gt) {
        if (a[i] < p) swap(a[lt++], a[i++]);
        else if (a[i] > p) swap(a[i], a[gt--]);  // nu crestem i: a[i] e nou
        else i++;
    }
    quicksort_3way(a, lo, lt - 1);
    quicksort_3way(a, gt + 1, hi);
}

// apel recursiv doar pe partea mai mica, pe cea mare continuam cu while
// asa stiva are adancime O(log n) garantat, deci nu luam stack overflow
void quicksort_small_stack(vector<int>& a, int lo, int hi) {
    while (lo < hi) {
        int j = partition_hoare(a, lo, hi);
        if (j - lo < hi - j) {
            quicksort_small_stack(a, lo, j);
            lo = j + 1;
        } else {
            quicksort_small_stack(a, j + 1, hi);
            hi = j;
        }
    }
}

double time_ms(void (*sort_fn)(vector<int>&, int, int), vector<int> a) {
    auto start = chrono::steady_clock::now();
    sort_fn(a, 0, (int)a.size() - 1);
    auto end = chrono::steady_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

int main() {
    vector<int> v = {5, 3, 8, 4, 2, 7, 1, 6};
    cout << "initial:          ";
    print(v);

    vector<int> a = v;
    int j = partition_hoare(a, 0, (int)a.size() - 1);
    cout << "partitie Hoare:   ";
    print(a);
    cout << "                  a[0.." << j << "] <= pivot <= a[" << j + 1 << "..]\n";

    a = v;
    quicksort_lomuto(a, 0, (int)a.size() - 1);
    cout << "quicksort Lomuto: ";
    print(a);

    a = v;
    quicksort_hoare(a, 0, (int)a.size() - 1);
    cout << "quicksort Hoare:  ";
    print(a);

    // testare pe vectori aleatori: comparam cu std::sort
    for (int t = 0; t < 2000; t++) {
        int n = random_int(0, 100);
        vector<int> w(n);
        for (int& x : w) x = random_int(-20, 20);
        vector<int> expected = w;
        sort(expected.begin(), expected.end());

        vector<int> a1 = w, a2 = w, a3 = w, a4 = w;
        quicksort_lomuto(a1, 0, n - 1);
        quicksort_hoare(a2, 0, n - 1);
        quicksort_3way(a3, 0, n - 1);
        quicksort_small_stack(a4, 0, n - 1);
        assert(a1 == expected && a2 == expected && a3 == expected && a4 == expected);
    }
    cout << "toate testele au trecut\n\n";

    // elemente egale: lomuto ajunge la O(n^2), hoare si 3-way nu
    int n = 20000;
    vector<int> equal(n, 7);
    cout << n << " elemente egale:\n";
    cout << "  Lomuto: " << time_ms(quicksort_lomuto, equal) << " ms\n";
    cout << "  Hoare:  " << time_ms(quicksort_hoare, equal) << " ms\n";
    cout << "  3-way:  " << time_ms(quicksort_3way, equal) << " ms\n";
    return 0;
}
