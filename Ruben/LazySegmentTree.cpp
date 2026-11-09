// ===========================================================================
// LAZY SEGTREE     build O(n)   update O(log n)   query O(log n)   mem 4n
// Ranges are HALF-OPEN: [l, r) -- r is NOT included.
// RANGE update + RANGE query. Point ops are just l, l+1 -- not a config choice.
// ~4x slower than Seg: if you only ever update one index, use Seg instead.
//
//   LazySeg s(n);          // n elements, all = ID
//   LazySeg s(a);          // build from vector<LazySeg::T> a
//
//   s.update(l, r, f);     // apply f to a[l..r)     update(i, i+1, f) = point
//   s.query(l, r);         // fold a[l..r)           query(l, l) = ID
//   s[i];                  // read a[i]
//
// WHAT THE FIVE PIECES MEAN
//   Every node stores T = the fold of its segment, plus F = an update that has
//   been applied to this node's T but NOT yet to its children ("pending").
//
//   T            the answer type: what one element is and what a fold gives back.
//   F            the update type: what you pass to update(l, r, f).
//
//   op(a, b)     merge two child folds into the parent's. a is the LEFT child.
//   ID           neutral element of op: op(x, ID) = op(ID, x) = x.
//                Also what query(l, l) returns.
//
//   ap(x, f, k)  apply update f to a WHOLE segment at once, given only that
//                segment's current fold x and its length k. This is the whole
//                trick -- you never touch the k elements individually, so the
//                new fold has to be computable from (x, f, k) alone. If it
//                isn't, lazy propagation doesn't work for that update.
//                k is there for things like sum, where "+= f" on k elements
//                adds f*k to the fold. Ignore k for min/max.
//
//   cm(a, f)     squash two pending updates into one: first a, THEN f.
//                Needed because a node can collect a second update before it
//                has pushed the first one down. Order matters (assign, affine).
//   NOP          "nothing pending": cm(NOP, f) = f and ap(x, NOP, k) = x.
//
//   Sanity check for a new row: applying f to both halves and then merging must
//   equal merging first and then applying f --
//       op(ap(a, f, ka), ap(b, f, kb)) == ap(op(a, b), f, ka + kb)
//   and stacking must match squashing --
//       ap(ap(x, a, k), f, k) == ap(x, cm(a, f), k)
//   If either fails, the config is wrong and it will only show up on big tests.
//
// CONFIG -- edit the 5 lines below.
//   f = the update, k = how many elements sit under the node.
//
//   f does     query=  op          ap(x,f,k)      cm(a,f)      ID     NOP
//   x += f     sum     a + b       x + f*k        a + f        0      0
//   x += f     min     min(a,b)    x + f          a + f        INF    0
//   x += f     max     max(a,b)    x + f          a + f        -INF   0
//   x  = f     sum     a + b       f * k          f            0      NONE
//   x  = f     min     min(a,b)    f              f            INF    NONE
//   x  = f     max     max(a,b)    f              f            -INF   NONE
//   x  = f     gcd     __gcd(a,b)  f              f            0      NONE
//   x ^= f     xor     a ^ b       k&1 ? x^f : x  a ^ f        0      0
//
//   INF = 4e18.  NONE = LLONG_MIN, a fake "nothing pending" value -- for the
//   assign rows the value you assign must never actually be LLONG_MIN.
//
//   AFFINE (only when one problem mixes "multiply a range" and "add a range"):
//     using F = pair<ll,ll>;   f = {p,q} means x -> p*x + q
//     ap(x,f,k) = f.first * x + f.second * k
//     cm(a,f)   = {a.first * f.first, f.first * a.second + f.second}
//     NOP       = {1, 0}
//
// Need two different lazy trees in one problem? Copy the struct, rename, edit.
// ===========================================================================
struct LazySeg {
    using T = ll;                                        // node value
    using F = ll;                                        // pending update
    static T op(T a, T b)        { return a + b; }       // combine children
    static T ap(T x, F f, int k) { return x + f * k; }   // f over k elements
    static F cm(F a, F f)        { return a + f; }       // f applied after a
    static constexpr T ID  = 0;                          // neutral value
    static constexpr F NOP = 0;                          // nothing pending
 
    int n; vector<T> t; vector<F> d;
 
    LazySeg(int n = 0) : n(n), t(4 * n, ID), d(4 * n, NOP) {}
    LazySeg(const vector<T>& a) : LazySeg((int)a.size()) { if (n) build(1, 0, n, a); }
 
    void update(int l, int r, F f) { if (l < r) upd(1, 0, n, l, r, f); }
    T query(int l, int r) { return l < r ? qry(1, 0, n, l, r) : ID; }
    T operator[](int i) { return query(i, i + 1); }
 
    // ---- internals: node v covers [l, r) --------------------------------
    void build(int v, int l, int r, const vector<T>& a) {
        if (r - l == 1) { t[v] = a[l]; return; }
        int m = (l + r) / 2;
        build(2*v, l, m, a); build(2*v+1, m, r, a);
        t[v] = op(t[2*v], t[2*v+1]);
    }
    void put(int v, int k, F f) { t[v] = ap(t[v], f, k); d[v] = cm(d[v], f); }
    void push(int v, int l, int m, int r) {
        if (d[v] == NOP) return;
        put(2*v, m - l, d[v]); put(2*v+1, r - m, d[v]);
        d[v] = NOP;
    }
    void upd(int v, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) { put(v, r - l, f); return; }
        int m = (l + r) / 2;
        push(v, l, m, r);
        if (ql < m) upd(2*v, l, m, ql, qr, f);
        if (m < qr) upd(2*v+1, m, r, ql, qr, f);
        t[v] = op(t[2*v], t[2*v+1]);
    }
    T qry(int v, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[v];
        int m = (l + r) / 2;
        push(v, l, m, r);
        if (qr <= m) return qry(2*v, l, m, ql, qr);
        if (m <= ql) return qry(2*v+1, m, r, ql, qr);
        return op(qry(2*v, l, m, ql, qr), qry(2*v+1, m, r, ql, qr));
    }
};