// Tutoriat SDA - 28.10.2026
#include <algorithm>
#include <deque>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

void print(const vector<int>& v) {
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// c1 - complexitate: de cate ori se executa a++ si b++
void bucle(int n) {
    long long a = 0, b = 0;
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++) a++;  // n(n+1)/2 => O(n^2)
    for (int i = 1; i < n; i *= 2) b++;   // log2 n => O(log n)
    cout << "n = " << n << ": a = " << a << ", b = " << b << '\n';
}

// c2 - paranteze cu stiva
bool corect(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            char deschisa = c == ')' ? '(' : c == ']' ? '[' : '{';
            if (st.empty() || st.top() != deschisa) return false;
            st.pop();
        }
    }
    return st.empty();
}

// c3 - arbore binar de cautare
struct Node {
    int key;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int k) : key(k) {}
};

Node* insert(Node* node, int key) {
    if (node == nullptr) return new Node(key);
    if (key < node->key) node->left = insert(node->left, key);
    else node->right = insert(node->right, key);
    return node;
}

void preordine(Node* node) {
    if (node == nullptr) return;
    cout << node->key << ' ';
    preordine(node->left);
    preordine(node->right);
}

void inordine(Node* node) {
    if (node == nullptr) return;
    inordine(node->left);
    cout << node->key << ' ';
    inordine(node->right);
}

void postordine(Node* node) {
    if (node == nullptr) return;
    postordine(node->left);
    postordine(node->right);
    cout << node->key << ' ';
}

// in muchii, un singur nod are inaltimea 0
int inaltime(Node* node) {
    if (node == nullptr) return -1;
    return 1 + max(inaltime(node->left), inaltime(node->right));
}

// arborele culcat: dreapta e sus, stanga e jos
void afiseaza(Node* node, int depth = 0) {
    if (node == nullptr) return;
    afiseaza(node->right, depth + 1);
    cout << string(4 * depth, ' ') << node->key << '\n';
    afiseaza(node->left, depth + 1);
}

// c4 - min-heap
void sift_up(vector<int>& h, int i) {
    while (i > 0 && h[i] < h[(i - 1) / 2]) {
        swap(h[i], h[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void sift_down(vector<int>& h, int i) {
    int n = h.size();
    while (true) {
        int st = 2 * i + 1, dr = 2 * i + 2, m = i;
        if (st < n && h[st] < h[m]) m = st;
        if (dr < n && h[dr] < h[m]) m = dr;
        if (m == i) return;
        swap(h[i], h[m]);
        i = m;
    }
}

// c5 - interclasare
vector<int> interclasare(const vector<int>& a, const vector<int>& b, int& comp) {
    vector<int> res;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        comp++;
        if (a[i] <= b[j]) res.push_back(a[i++]);
        else res.push_back(b[j++]);
    }
    // restul fara comparatii
    while (i < a.size()) res.push_back(a[i++]);
    while (j < b.size()) res.push_back(b[j++]);
    return res;
}

// c5 - mergesort, afisam fiecare interclasare
vector<int> mergesort(const vector<int>& v, int depth = 0) {
    if (v.size() <= 1) return v;
    size_t mid = v.size() / 2;
    vector<int> st(v.begin(), v.begin() + mid), dr(v.begin() + mid, v.end());
    st = mergesort(st, depth + 1);
    dr = mergesort(dr, depth + 1);
    int comp = 0;
    vector<int> res = interclasare(st, dr, comp);
    cout << string(3 * depth, ' ') << "interclasez -> ";
    print(res);
    return res;
}

// c5 - inversiuni: sorteaza v si numara inversiunile
long long inversiuni(vector<int>& v) {
    if (v.size() <= 1) return 0;
    size_t mid = v.size() / 2;
    vector<int> a(v.begin(), v.begin() + mid), b(v.begin() + mid, v.end());
    long long inv = inversiuni(a) + inversiuni(b);
    size_t i = 0, j = 0, k = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) {
            v[k++] = a[i++];
        } else {
            inv += a.size() - i;  // b[j] e mai mic decat tot ce a ramas in a
            v[k++] = b[j++];
        }
    }
    while (i < a.size()) v[k++] = a[i++];
    while (j < b.size()) v[k++] = b[j++];
    return inv;
}

int main() {
    cout << "--- c1 ---\n";
    for (int n : {10, 100, 1000}) bucle(n);

    cout << "\n--- c2 ---\n";
    stack<int> s;
    deque<int> q;
    for (string op : {"push 5", "push 3", "pop", "push 8", "push 1", "pop", "push 4"}) {
        if (op == "pop") {
            cout << "pop: stiva scoate " << s.top() << ", coada scoate " << q.front() << '\n';
            s.pop();
            q.pop_front();
        } else {
            int x = stoi(op.substr(5));
            s.push(x);
            q.push_back(x);
        }
    }
    for (string p : {"([]{()})", "([)]", "((", "())"}) {
        cout << p << ' ' << (corect(p) ? "da" : "nu") << '\n';
    }

    cout << "\n--- c3 ---\n";
    Node* root = nullptr;
    for (int x : {8, 3, 10, 1, 6, 14, 4, 7, 13}) root = insert(root, x);
    afiseaza(root);
    cout << "preordine:  ";
    preordine(root);
    cout << "\ninordine:   ";
    inordine(root);
    cout << "\npostordine: ";
    postordine(root);
    cout << "\ninaltime: " << inaltime(root) << '\n';

    cout << "\n--- c4 ---\n";
    vector<int> h;
    for (int x : {6, 4, 5, 1, 2, 8, 7, 3}) {
        h.push_back(x);
        sift_up(h, h.size() - 1);
    }
    print(h);
    for (int t = 0; t < 2; t++) {
        cout << "scot " << h[0] << " -> ";
        h[0] = h.back();
        h.pop_back();
        sift_down(h, 0);
        print(h);
    }

    cout << "\n--- c5 ---\n";
    int comp = 0;
    print(interclasare({1, 4, 7, 9}, {2, 3, 8, 10, 11}, comp));
    cout << comp << " comparatii\n";
    print(mergesort({38, 27, 43, 3, 9, 82, 10}));
    vector<int> v = {2, 4, 1, 3, 5};
    cout << inversiuni(v) << " inversiuni\n";
    return 0;
}
