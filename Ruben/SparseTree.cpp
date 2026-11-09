// ===========================================================================
// SPARSE SEGTREE (dynamic)   set / add / query O(log C)   mem O(V log C)
// Ranges are HALF-OPEN: [l, r) -- r is NOT included.
// Indices are ll and may be huge or negative. The tree covers [lo, hi) and
// nodes are made only where you touch; every position you never set reads
// as ID, so the untouched part of the universe costs nothing.
//
//   SparseSeg s(0, 1e9);          // covers [0, 1e9), all = ID
//   SparseSeg s(-1e18, 1e18, q);  // 3rd arg = how many set/add you will do
//                                 //   (reserve hint only, may be 0 or off)
//
//   s.set(i, v);           // a[i] = v
//   s.add(i, v);           // a[i] = op(a[i], v)   ONE descent, prefer it
//   s[i];                  // read a[i]   (ID if never set)
//   s.query(l, r);         // fold a[l..r)   query(i,i+1) = a[i], query(l,l) = ID
//   s.kth(k);              // SUM CONFIG ONLY, see below
//
// CONFIG -- edit the 3 lines below, same as Seg. op must be associative
// (need NOT commute). ID is also what every untouched position holds, so it
// must be the neutral element you actually want sitting there.
//   query=   T      op                ID              add(i,v) means
//   sum      ll     a + b             0               a[i] += v
//   count    ll     a + b             0               a[i] += v
//   min      ll     min(a, b)         LLONG_MAX       a[i] = min(a[i], v)
//   max      ll     max(a, b)         LLONG_MIN       a[i] = max(a[i], v)
//   gcd      ll     __gcd(a, b)       0               a[i] = gcd(a[i], v)
//   xor      ll     a ^ b             0               a[i] ^= v
//   or       ll     a | b             0               a[i] |= v
//
// If T is a struct, change ID to:  inline static const T ID = T{...};
// Need two different trees in one problem? Copy the struct, rename, edit.
//
// WHEN -- only when you cannot see every position in advance: interactive,
// or an update that depends on an earlier answer. If you can read all input
// first, coordinate-compress and use BIT / Seg: same complexity but 3.5x
// faster and 4.5x less memory. Multiset over a value axis = count config,
// add(v, +1) / add(v, -1), query(a, b+1), kth(k) = k-th smallest.
//
// MEM -- V = distinct positions ever touched, C = hi - lo. Nodes are about
// 2V + V*log2(C/V), NOT V*log2(C): the top levels saturate. 16 B per node.
// V = 3e5 over C = 1e9 is ~3.9M nodes ~ 62 MB. Tight limit? use T = int
// (12 B/node), or compress and use BIT.
//
// kth -- SUM/COUNT CONFIG ONLY, needs every a[i] >= 0 and k >= 1. Smallest i
// with query(lo, i+1) >= k, or hi if the total is < k.
// ===========================================================================
struct SparseSeg {
    using T = ll;
    static T op(T a, T b) { return a + b; }
    static constexpr T ID = 0;

    struct Nd { int l, r; T v; };
    ll lo, hi;
    vector<Nd> nd;               // node 0 = null, 1 = root

    SparseSeg(ll lo = 0, ll hi = 1, int hint = 0)
            : lo(lo), hi(hi) {
        ll C = max(hi - lo, 1LL), h = max(hint, 1);
        size_t cap = 2 + h * (3 + (C > h ? __lg(C / h) : 0));
        nd.reserve(cap);
        node(); node();
    }
    int node() {
        nd.push_back({0, 0, ID});
        return (int)nd.size() - 1;
    }
    void set(ll i, T v) { pt(i, v, true);  }
    void add(ll i, T v) { pt(i, v, false); }
    T operator[](ll i) { return query(i, i + 1); }
    T query(ll l, ll r) {                      // fold a[l..r)
        return l < r ? qry(1, lo, hi, l, r) : ID;
    }
    ll kth(T k) {
        if (nd[1].v < k) return hi;
        int x = 1; ll l = lo, r = hi;
        while (r - l > 1) {
            ll m = l + (r - l) / 2;
            if (nd[nd[x].l].v >= k) { x = nd[x].l; r = m; }
            else { k -= nd[nd[x].l].v; x = nd[x].r; l = m; }
        }
        return l;
    }
    void pt(ll i, T v, bool assign) {
        int st[64], d = 0, x = 1; ll l = lo, r = hi;
        while (r - l > 1) {
            st[d++] = x;
            ll m = l + (r - l) / 2;
            if (i < m) {
                if (!nd[x].l) { int c = node(); nd[x].l = c; }
                x = nd[x].l; r = m;
            } else {
                if (!nd[x].r) { int c = node(); nd[x].r = c; }
                x = nd[x].r; l = m;
            }
        }
        nd[x].v = assign ? v : op(nd[x].v, v);
        while (d--) {
            int p = st[d];
            nd[p].v = op(nd[nd[p].l].v, nd[nd[p].r].v);
        }
    }
    T qry(int x, ll l, ll r, ll ql, ll qr) {
        if (!x || qr <= l || r <= ql) return ID;
        if (ql <= l && r <= qr) return nd[x].v;
        ll m = l + (r - l) / 2;
        return op(qry(nd[x].l, l, m, ql, qr),
                  qry(nd[x].r, m, r, ql, qr));
    }
};