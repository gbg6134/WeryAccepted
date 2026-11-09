// ===========================================================================
// SEGTREE (iterative)   build O(n)   set O(log n)   query O(log n)   mem 2n
// Ranges are HALF-OPEN: [l, r) -- r is NOT included.
// POINT assign + RANGE query. No lazy -> for range updates use LazySeg.
//
//   Seg s(n);              // n elements, all = ID
//   Seg s(a);              // build from vector<Seg::T> a, O(n)
//
//   s.set(i, v);           // a[i] = v
//   s[i];                  // read a[i]
//   s.set(i, s[i] + v);    // a[i] += v   (any read-modify-write)
//   s.query(l, r);         // fold a[l..r)   query(i, i+1) = a[i], query(l,l) = ID
//
// CONFIG -- edit the 3 lines below. op must be associative (need NOT commute).
//   query=   T      op                ID
//   sum      ll     a + b             0
//   min      ll     min(a, b)         LLONG_MAX
//   max      ll     max(a, b)         LLONG_MIN
//   gcd      ll     __gcd(a, b)       0
//   xor      ll     a ^ b             0
//   and      ll     a & b             ~0LL
//   or       ll     a | b             0
//   count of min:   T = pair<ll,int>, op = merge keeping smaller .first and
//                   summing .second on ties, ID = {LLONG_MAX, 0}
//
// If T is a struct, change ID to:  inline static const T ID = T{...};
// Need two different trees in one problem? Copy the struct, rename, edit.
// ===========================================================================
struct Seg {
    using T = ll;
    static T op(T a, T b) { return a + b; }
    static constexpr T ID = 0;
 
    int n; vector<T> t;
 
    Seg(int n = 0) : n(n), t(2 * n, ID) {}
    Seg(const vector<T>& a) : n((int)a.size()), t(2 * a.size()) {
        copy(a.begin(), a.end(), t.begin() + n);
        for (int i = n - 1; i > 0; i--) t[i] = op(t[2*i], t[2*i+1]);
    }
    T operator[](int i) const { return t[i + n]; }
 
    void set(int i, T v) {
        for (t[i += n] = v; i /= 2;) t[i] = op(t[2*i], t[2*i+1]);
    }
    T query(int l, int r) {                  // fold a[l..r)
        T x = ID, y = ID;                    // x = left part, y = right part
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) x = op(x, t[l++]);
            if (r & 1) y = op(t[--r], y);
        }
        return op(x, y);
    }
};