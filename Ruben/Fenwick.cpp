// ===========================================================================
// FENWICK (BIT)      add O(log n)   sum O(log n)   kth O(log n)   mem n
// Ranges are HALF-OPEN: [l, r) -- r is NOT included.
// ~3x faster than Seg and ~15x faster than LazySeg, but sum only (needs an inverse).
//
//   BIT f(n);              // n elements, all 0
//   BIT f(a);              // build from vector<ll> a, O(n)
//
//   f.add(i, x);           // a[i] += x
//   f.sum(r);              // a[0] + ... + a[r-1]      sum(0) = 0
//   f.sum(l, r);           // a[l] + ... + a[r-1]      sum(i, i+1) = a[i]
//   f.kth(k);              // smallest i with sum(i+1) >= k, or n if none
//                          // needs every a[i] >= 0 and k >= 1
//
//   RANGE add + POINT get: f.add(l, x), f.add(r, -x);  then f.sum(i+1) IS a[i].
//   RANGE add + RANGE sum: two BITs, or just use LazySeg.
//   min / max / gcd have no inverse -> use Seg / LazySeg.
//   xor instead of sum: swap every += for ^= and drop the - in sum(l, r).
// ===========================================================================
struct BIT {
    int n; vector<ll> t;
 
    BIT(int n = 0) : n(n), t(n + 1, 0) {}
    BIT(const vector<ll>& a) : BIT((int)a.size()) {
        for (int i = 1; i <= n; i++) {
            t[i] += a[i-1];
            int j = i + (i & -i);
            if (j <= n) t[j] += t[i];
        }
    }
    void add(int i, ll x) {
        for (++i; i <= n; i += i & -i) t[i] += x;
    }
    ll sum(int r) {                          // a[0..r)
        ll s = 0;
        for (; r > 0; r -= r & -r) s += t[r];
        return s;
    }
    ll sum(int l, int r) {                   // a[l..r)
        return l < r ? sum(r) - sum(l) : 0;
    }
    int kth(ll k) {                          // smallest i with sum(i+1) >= k
        int p = 0;
        for (int w = 1 << __lg(max(n, 1)); w; w >>= 1)
            if (p + w <= n && t[p + w] < k) p += w, k -= t[p];
        return p;
    }
};