// c7 - radixsort: countsort stabil pe fiecare cifra, incepand cu cea mai putin semnificativa
// compilare: clang++ -std=c++17 -Wall -Wextra -O2 radixsort.cpp -o radixsort
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

mt19937 rng(42);

void print(const vector<int>& v) {
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// countsort stabil dupa cifra (x / exp) % 10
// exp e long long pt ca la numere aproape de 2^31 ajunge la 10^10 si nu mai incape in int
void countsort_digit(vector<int>& v, long long exp) {
    vector<int> f(10, 0);
    for (int x : v) f[(x / exp) % 10]++;
    for (int i = 1; i < 10; i++) f[i] += f[i - 1];

    vector<int> out(v.size());
    for (int i = (int)v.size() - 1; i >= 0; i--) {  // de la final => stabil
        int d = (v[i] / exp) % 10;
        out[f[d] - 1] = v[i];
        f[d]--;
    }
    v = out;
}

// radixsort in baza 10, doar numere naturale, O(d * (n + 10))
void radixsort(vector<int>& v, bool verbose = false) {
    if (v.empty()) return;
    int m = *max_element(v.begin(), v.end());
    for (long long exp = 1; m / exp > 0; exp *= 10) {
        countsort_digit(v, exp);
        if (verbose) {
            cout << "[exp = " << exp << "] ";
            print(v);
        }
    }
}

// baza 256 pt unsigned pe 32 de biti => exact 4 treceri
// cifra i e (x >> (8 * i)) & 255, deci doar operatii pe biti
void radixsort_256(vector<unsigned>& v) {
    vector<unsigned> out(v.size());
    for (int shift = 0; shift < 32; shift += 8) {
        vector<int> f(256, 0);
        for (unsigned x : v) f[(x >> shift) & 255]++;
        for (int i = 1; i < 256; i++) f[i] += f[i - 1];
        for (int i = (int)v.size() - 1; i >= 0; i--) {
            unsigned d = (v[i] >> shift) & 255;
            out[f[d] - 1] = v[i];
            f[d]--;
        }
        swap(v, out);  // dupa 4 treceri (numar par) rezultatul e in v
    }
}

int main() {
    vector<int> v = {170, 45, 75, 90, 802, 24, 2, 66};
    cout << "initial:    ";
    print(v);
    radixsort(v, true);

    // testare: comparam cu std::sort
    for (int t = 0; t < 1000; t++) {
        int n = uniform_int_distribution<int>(0, 100)(rng);
        vector<int> a(n);
        for (int& x : a) x = uniform_int_distribution<int>(0, 2'147'483'647)(rng);  // pana la INT_MAX
        vector<unsigned> b(n);
        for (unsigned& x : b) x = rng();  // toti cei 32 de biti
        vector<int> ea = a;
        vector<unsigned> eb = b;
        sort(ea.begin(), ea.end());
        sort(eb.begin(), eb.end());
        radixsort(a);
        radixsort_256(b);
        assert(a == ea && b == eb);
    }
    cout << "toate testele au trecut\n\n";

    // timpi: compilati cu -O2, altfel comparatia nu e relevanta
    int n = 5'000'000;
    vector<unsigned> big(n);
    for (unsigned& x : big) x = rng();
    vector<int> big_int(n);
    for (int i = 0; i < n; i++) big_int[i] = (int)(big[i] >> 1);  // numere naturale < 2^31

    auto measure = [](const char* name, auto fn) {
        auto start = chrono::steady_clock::now();
        fn();
        auto end = chrono::steady_clock::now();
        cout << name << chrono::duration<double, milli>(end - start).count() << " ms\n";
    };
    cout << n << " numere aleatoare:\n";
    measure("  std::sort:          ", [&] { vector<unsigned> a = big; sort(a.begin(), a.end()); });
    measure("  radixsort baza 10:  ", [&] { vector<int> a = big_int; radixsort(a); });
    measure("  radixsort baza 256: ", [&] { vector<unsigned> a = big; radixsort_256(a); });
    return 0;
}
