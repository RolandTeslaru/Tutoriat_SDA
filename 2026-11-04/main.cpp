// Tutoriat SDA - 04.11.2026
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

void print(const vector<int>& v) {
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// 1. lomuto, pivot = ultimul element
int lomuto(vector<int>& a, int lo, int hi) {
    int p = a[hi], i = lo;
    for (int j = lo; j < hi; j++)
        if (a[j] < p) swap(a[i++], a[j]);
    swap(a[i], a[hi]);
    return i;
}

// 2. hoare, pivot = elementul din mijloc
int hoare(vector<int>& a, int lo, int hi) {
    int p = a[(lo + hi) / 2];
    int i = lo - 1, j = hi + 1;
    while (true) {
        do i++; while (a[i] < p);
        do j--; while (a[j] > p);
        if (i >= j) return j;
        swap(a[i], a[j]);
    }
}

// 3. quicksort
void quicksort(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int k = lomuto(a, lo, hi);
    quicksort(a, lo, k - 1);
    quicksort(a, k + 1, hi);
}

// cu hoare: recursiv pe [lo, j] si [j+1, hi]
void quicksort_hoare(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int j = hoare(a, lo, hi);
    quicksort_hoare(a, lo, j);
    quicksort_hoare(a, j + 1, hi);
}

// 4. numaram comparatiile, pivot ultimul sau aleator
long long comp = 0;

void quicksort_count(vector<int>& a, int lo, int hi, bool aleator) {
    if (lo >= hi) return;
    if (aleator) swap(a[lo + rand() % (hi - lo + 1)], a[hi]);
    comp += hi - lo;
    int k = lomuto(a, lo, hi);
    quicksort_count(a, lo, k - 1, aleator);
    quicksort_count(a, k + 1, hi, aleator);
}

// 5. quickselect, k de la 1
int quickselect(vector<int> a, int k) {
    int lo = 0, hi = a.size() - 1;
    while (true) {
        swap(a[lo + rand() % (hi - lo + 1)], a[hi]);
        int pos = lomuto(a, lo, hi);
        if (pos == k - 1) return a[pos];
        if (k - 1 < pos) hi = pos - 1;
        else lo = pos + 1;
    }
}

int main() {
    vector<int> a = {5, 3, 8, 4, 2, 7, 1, 6};
    int k = lomuto(a, 0, a.size() - 1);
    cout << "lomuto: ";
    print(a);
    cout << "pivotul pe pozitia " << k << "\n\n";

    a = {5, 3, 8, 4, 2, 7, 1, 6};
    int j = hoare(a, 0, a.size() - 1);
    cout << "hoare: ";
    print(a);
    cout << "j = " << j << "\n\n";

    a = {38, 27, 43, 3, 9, 82, 10};
    quicksort(a, 0, a.size() - 1);
    print(a);
    a = {38, 27, 43, 3, 9, 82, 10};
    quicksort_hoare(a, 0, a.size() - 1);
    print(a);
    cout << '\n';

    int n = 1000;
    for (bool aleator : {false, true}) {
        vector<int> v(n);
        for (int i = 0; i < n; i++) v[i] = i;  // deja sortat
        comp = 0;
        quicksort_count(v, 0, n - 1, aleator);
        cout << (aleator ? "pivot aleator: " : "pivot ultimul: ") << comp << " comparatii\n";
    }
    cout << "n^2 / 2 = " << n * n / 2 << "\n\n";

    vector<int> v = {7, 10, 4, 3, 20, 15, 8, 1, 12};
    cout << "mediana: " << quickselect(v, (v.size() + 1) / 2) << '\n';
    return 0;
}
