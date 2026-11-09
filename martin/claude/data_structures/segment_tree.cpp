// ============ Segment Tree (point update, range query) ============
// Generic, works for sum/min/max/gcd/etc. Change merge() and IDENTITY.
// 0-indexed array, build in O(n), update/query in O(log n).
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct SegTree {
    int n;
    vector<ll> t;
    ll IDENTITY = 0; // 0 for sum, LLONG_MAX for min, LLONG_MIN for max...

    ll merge(ll a, ll b) { return a + b; } // change for min/max/gcd etc.

    SegTree(int n_) : n(n_), t(2 * n_, IDENTITY) {}
    SegTree(vector<ll>& a) : n(a.size()), t(2 * a.size()) {
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; i--) t[i] = merge(t[2*i], t[2*i+1]);
    }

    void update(int pos, ll val) { // set a[pos] = val
        pos += n;
        t[pos] = val;
        for (pos /= 2; pos >= 1; pos /= 2)
            t[pos] = merge(t[2*pos], t[2*pos+1]);
    }

    // query on [l, r) 0-indexed, half-open
    ll query(int l, int r) {
        ll resL = IDENTITY, resR = IDENTITY;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) resL = merge(resL, t[l++]);
            if (r & 1) resR = merge(t[--r], resR);
        }
        return merge(resL, resR);
    }
};

// ---------------- Recursive version (easier to modify per-problem) ----------------
struct SegTreeRec {
    int n;
    vector<ll> tree;
    SegTreeRec(int n_) : n(n_), tree(4 * n_, 0) {}

    void build(vector<ll>& a, int node, int lo, int hi) {
        if (lo == hi) { tree[node] = a[lo]; return; }
        int mid = (lo + hi) / 2;
        build(a, 2*node, lo, mid);
        build(a, 2*node+1, mid+1, hi);
        tree[node] = tree[2*node] + tree[2*node+1];
    }
    void update(int node, int lo, int hi, int pos, ll val) {
        if (lo == hi) { tree[node] = val; return; }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(2*node, lo, mid, pos, val);
        else update(2*node+1, mid+1, hi, pos, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }
    // query inclusive range [l, r]
    ll query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return 0; // identity
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return query(2*node, lo, mid, l, r) + query(2*node+1, mid+1, hi, l, r);
    }
};
