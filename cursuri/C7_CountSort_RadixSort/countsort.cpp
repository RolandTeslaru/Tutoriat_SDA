// c7 - countsort (sortare prin numarare), varianta stabila
// compilare: clang++ -std=c++17 -Wall -Wextra countsort.cpp -o countsort
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
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

// countsort stabil pt numere naturale, O(n + k) unde k = max + 1
void countsort(vector<int>& v) {
    if (v.empty()) return;
    int k = *max_element(v.begin(), v.end()) + 1;

    vector<int> f(k, 0);
    for (int x : v) f[x]++;                          // 1. frecvente
    for (int i = 1; i < k; i++) f[i] += f[i - 1];    // 2. sume partiale

    vector<int> out(v.size());
    for (int i = (int)v.size() - 1; i >= 0; i--) {   // 3. de la final => stabil
        out[f[v[i]] - 1] = v[i];
        f[v[i]]--;
    }
    v = out;
}

// pt orice intregi (si negativi): scadem minimul, sortam, adunam inapoi
void countsort_any(vector<int>& v) {
    if (v.empty()) return;
    int lo = *min_element(v.begin(), v.end());
    for (int& x : v) x -= lo;
    countsort(v);
    for (int& x : v) x += lo;
}

// pe inregistrari: sortam studentii dupa nota (0..10)
// la note egale ramane ordinea initiala, asta inseamna stabil
struct Student {
    string nume;
    int nota;
};

vector<Student> countsort_students(const vector<Student>& s) {
    vector<int> f(11, 0);
    for (const Student& st : s) f[st.nota]++;
    for (int i = 1; i <= 10; i++) f[i] += f[i - 1];

    vector<Student> out(s.size());
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        out[f[s[i].nota] - 1] = s[i];
        f[s[i].nota]--;
    }
    return out;
}

int main() {
    vector<int> v = {4, 1, 3, 4, 2, 0, 1, 4};
    countsort(v);
    cout << "countsort:     ";
    print(v);

    vector<int> neg = {3, -2, 7, 0, -5, 3, 1};
    countsort_any(neg);
    cout << "countsort_any: ";
    print(neg);

    vector<Student> studenti = {{"Ana", 9},  {"Bogdan", 7}, {"Cristi", 9},
                                {"Dana", 10}, {"Elena", 7},  {"Florin", 9}};
    cout << "studenti dupa nota:\n";
    for (const Student& st : countsort_students(studenti)) cout << "  " << st.nota << ' ' << st.nume << '\n';

    // testare: comparam cu std::sort si (pentru studenti) cu std::stable_sort
    for (int t = 0; t < 1000; t++) {
        int n = random_int(0, 60);
        vector<int> a(n), b(n);
        for (int& x : a) x = random_int(0, 30);
        for (int& x : b) x = random_int(-30, 30);
        vector<int> ea = a, eb = b;
        sort(ea.begin(), ea.end());
        sort(eb.begin(), eb.end());
        countsort(a);
        countsort_any(b);
        assert(a == ea && b == eb);

        vector<Student> s(n);
        for (int i = 0; i < n; i++) s[i] = {"s" + to_string(i), random_int(0, 10)};
        vector<Student> es = s;
        stable_sort(es.begin(), es.end(), [](const Student& x, const Student& y) { return x.nota < y.nota; });
        vector<Student> cs = countsort_students(s);
        for (int i = 0; i < n; i++) assert(cs[i].nume == es[i].nume);
    }
    cout << "toate testele au trecut\n";
    return 0;
}
