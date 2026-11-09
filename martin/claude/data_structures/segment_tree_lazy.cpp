// ======== Segment Tree with Lazy Propagation (range add, range sum) ========
// Adapt push_down/merge for range-assign, range-min, etc.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct LazySegTree {
    int n;
    vector<ll> tree, lazy;

    LazySegTree(int n_) : n(n_), tree(4*n_, 0), lazy(4*n_, 0) {}

    void build(vector<ll>& a, int node, int lo, int hi) {
        if (lo == hi) { tree[node] = a[lo]; return; }
        int mid = (lo + hi) / 2;
        build(a, 2*node, lo, mid);
        build(a, 2*node+1, mid+1, hi);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    void push_down(int node, int lo, int hi) {
        if (lazy[node] == 0) return;
        int mid = (lo + hi) / 2;
        int left = 2*node, right = 2*node+1;
        tree[left]  += lazy[node] * (mid - lo + 1);
        tree[right] += lazy[node] * (hi - mid);
        lazy[left]  += lazy[node];
        lazy[right] += lazy[node];
        lazy[node] = 0;
    }

    // add val to every element in [l, r] (inclusive)
    void update(int node, int lo, int hi, int l, int r, ll val) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) {
            tree[node] += val * (hi - lo + 1);
            lazy[node] += val;
            return;
        }
        push_down(node, lo, hi);
        int mid = (lo + hi) / 2;
        update(2*node, lo, mid, l, r, val);
        update(2*node+1, mid+1, hi, l, r, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    ll query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return 0;
        if (l <= lo && hi <= r) return tree[node];
        push_down(node, lo, hi);
        int mid = (lo + hi) / 2;
        return query(2*node, lo, mid, l, r) + query(2*node+1, mid+1, hi, l, r);
    }
};
// Usage: LazySegTree st(n); st.build(a, 1, 0, n-1);
//        st.update(1, 0, n-1, l, r, val); st.query(1, 0, n-1, l, r);
