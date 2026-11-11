// Tutoriat SDA - 11.11.2026
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void print(const vector<int>& v) {
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// 1. countsort pe numere naturale mici
vector<int> countsort(const vector<int>& v) {
    int k = *max_element(v.begin(), v.end()) + 1;
    vector<int> f(k, 0);
    for (int x : v) f[x]++;
    for (int i = 1; i < k; i++) f[i] += f[i - 1];  // f[x] = cate sunt <= x
    vector<int> out(v.size());
    for (int i = v.size() - 1; i >= 0; i--) {
        out[f[v[i]] - 1] = v[i];
        f[v[i]]--;
    }
    return out;
}

// 2. studenti dupa nota, e stabil doar daca mergem de la final
struct Student {
    string nume;
    int nota;
};

vector<Student> dupa_nota(const vector<Student>& s, bool de_la_final) {
    int n = s.size();
    vector<int> f(11, 0);
    for (const Student& st : s) f[st.nota]++;
    for (int i = 1; i <= 10; i++) f[i] += f[i - 1];
    vector<Student> out(n);
    for (int t = 0; t < n; t++) {
        int i = de_la_final ? n - 1 - t : t;
        out[f[s[i].nota] - 1] = s[i];
        f[s[i].nota]--;
    }
    return out;
}

// 3. countsort dupa cifra (x / exp) % 10
void countsort_cifra(vector<int>& v, long long exp, bool de_la_final) {
    int n = v.size();
    vector<int> f(10, 0);
    for (int x : v) f[(x / exp) % 10]++;
    for (int i = 1; i < 10; i++) f[i] += f[i - 1];
    vector<int> out(n);
    for (int t = 0; t < n; t++) {
        int i = de_la_final ? n - 1 - t : t;
        int c = (v[i] / exp) % 10;
        out[f[c] - 1] = v[i];
        f[c]--;
    }
    v = out;
}

void radixsort(vector<int>& v, bool de_la_final = true) {
    int m = *max_element(v.begin(), v.end());
    // exp e long long ca sa nu dea overflow la numere mari
    for (long long exp = 1; m / exp > 0; exp *= 10) {
        countsort_cifra(v, exp, de_la_final);
        cout << "cifra " << exp << ": ";
        print(v);
    }
}

int main() {
    print(countsort({4, 1, 3, 4, 2, 0, 1, 4}));
    cout << '\n';

    vector<Student> studenti = {{"Ana", 9}, {"Bogdan", 7}, {"Cristi", 9}, {"Dana", 10}, {"Elena", 7}, {"Florin", 9}};
    for (bool de_la_final : {true, false}) {
        cout << (de_la_final ? "de la final:   " : "de la inceput: ");
        for (const Student& st : dupa_nota(studenti, de_la_final)) cout << st.nume << ' ' << st.nota << ", ";
        cout << '\n';
    }
    cout << '\n';

    vector<int> v = {170, 45, 75, 90, 802, 24, 2, 66};
    radixsort(v);
    cout << '\n';

    v = {170, 45, 75, 90, 802, 24, 2, 66};
    cout << "cu countsort de la inceput (gresit):\n";
    radixsort(v, false);
    cout << '\n';

    v = {400000000, 5, 8};
    radixsort(v);
    return 0;
}
