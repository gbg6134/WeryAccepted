// ==================== Fenwick Tree (Binary Indexed Tree) ====================
// 1-indexed internally. Point update, prefix sum query, O(log n).
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Fenwick {
    int n;
    vector<ll> bit;
    Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}

    void update(int i, ll delta) { // add delta at position i (1-indexed)
        for (; i <= n; i += i & (-i)) bit[i] += delta;
    }
    ll query(int i) { // prefix sum [1, i]
        ll s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }
    ll query(int l, int r) { return query(r) - query(l - 1); } // range sum [l, r]

    // find smallest index with prefix sum >= target (bit must be non-negative)
    int lower_bound(ll target) {
        int pos = 0;
        int logn = 32 - __builtin_clz(max(n,1));
        for (int pw = 1 << logn; pw > 0; pw >>= 1) {
            if (pos + pw <= n && bit[pos + pw] < target) {
                pos += pw;
                target -= bit[pos];
            }
        }
        return pos + 1; // first index where prefix sum >= original target
    }
};

// ---------------- Range update, range query Fenwick (via two BITs) ----------------
struct RangeFenwick {
    int n;
    Fenwick B1, B2; // B1: add tag, B2: for prefix sum computation

    RangeFenwick(int n_) : n(n_), B1(n_), B2(n_) {}

    void rangeAdd(int l, int r, ll val) {
        B1.update(l, val);       B1.update(r + 1, -val);
        B2.update(l, val * (l - 1)); B2.update(r + 1, -val * r);
    }
    ll prefixSum(int i) { return B1.query(i) * i - B2.query(i); }
    ll rangeSum(int l, int r) { return prefixSum(r) - prefixSum(l - 1); }
};

// ---------------- 2D Fenwick (point update, rectangle sum) ----------------
struct Fenwick2D {
    int n, m;
    vector<vector<ll>> bit;
    Fenwick2D(int n_, int m_) : n(n_), m(m_), bit(n_ + 1, vector<ll>(m_ + 1, 0)) {}
    void update(int x, int y, ll delta) {
        for (int i = x; i <= n; i += i & (-i))
            for (int j = y; j <= m; j += j & (-j))
                bit[i][j] += delta;
    }
    ll query(int x, int y) {
        ll s = 0;
        for (int i = x; i > 0; i -= i & (-i))
            for (int j = y; j > 0; j -= j & (-j))
                s += bit[i][j];
        return s;
    }
    ll query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};
