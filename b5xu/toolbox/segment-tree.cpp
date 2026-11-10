#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double ld;
#define fi first
#define se second
#define inf 1e18
#ifdef LOCAL
#define dbg(expr)                                                              \
  if (true) {                                                                  \
    cerr << "[" << __FUNCTION__ << ", " << __LINE__ << "] " << #expr << ": "   \
         << expr << endl                                                       \
         << flush;                                                             \
  }
#else
#define dbg(expr)
#endif

// https://codeforces.com/blog/entry/18051
// Segment tree, range query, point update, iterative
// set, min
// RMQ
class ST_RQPU_I {
  typedef ll T;
  typedef ll CHNG_T;
  const T def = inf;
  inline T _comb(T a, T b) { return min(a, b); }
  inline T _chng(T old_v, CHNG_T v) { return v; }

  vector<T> val;
  ll st_n;

public:
  ST_RQPU_I(ll n) : val(2 * n, def) { st_n = n; }
  /*
  ST_RQPU_I(vector<CHNG_T> values) : val(2 * values.size(), def) {
    st_n = values.size();
    for (ll i = 0; i < values.size(); i++)
      val[st_n + i] = _chng(def, values[i]);
    for (ll i = st_n - 1; i > 0; --i)
      val[i] = _comb(val[i << 1], val[i << 1 | 1]);
  }
  ST_RQPU_I(vector<T> values) : val(2 * values.size(), def) {
    st_n = values.size();
    for (ll i = 0; i < values.size(); i++)
      val[st_n + i] = values[i];
    for (ll i = st_n - 1; i > 0; --i)
      val[i] = _comb(val[i << 1], val[i << 1 | 1]);
  }
  */
  void update(ll i, CHNG_T v) {
    i += st_n;
    for (val[i] = _chng(val[i], v); i > 1; i >>= 1)
      val[i >> 1] = _comb(val[i], val[i ^ 1]);
  }
  T query(ll l, ll r) {
    T res = def;
    for (l += st_n, r += st_n + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        res = _comb(res, val[l++]);
      if (r & 1)
        res = _comb(res, val[--r]);
    }
    return res;
  }
};

// Segment tree, range query, point update, recursive
// set, min
// RMQ
class ST_RQPU_R {
  typedef ll T;
  typedef ll CHNG_T;
  const T def = inf;
  inline T _comb(T a, T b) { return min(a, b); }
  inline T _chng(T old_v, CHNG_T v) { return v; }

  vector<T> val;
  ll st_n;

public:
  ST_RQPU_R(ll n) : val(4 * n, def) { st_n = n; }
  void _update(ll x, ll l, ll r, ll i, CHNG_T v) {
    if (l == r)
      val[x] = _chng(val[x], v);
    else {
      ll mid = (l + r) / 2;
      if (i <= mid)
        _update(x * 2, l, mid, i, v);
      else
        _update(x * 2 + 1, mid + 1, r, i, v);
      val[x] = _comb(val[x * 2], val[x * 2 + 1]);
    }
  }
  T _query(ll x, ll l, ll r, ll ql, ll qr) {
    if (r < ql || qr < l)
      return def;
    if (ql <= l && r <= qr)
      return val[x];

    ll mid = (l + r) / 2;
    return _comb(_query(x * 2, l, mid, ql, qr),
                 _query(x * 2 + 1, mid + 1, r, ql, qr));
  }
  void update(ll i, CHNG_T v) { _update(1, 0, st_n - 1, i, v); }
  T query(ll l, ll r) { return _query(1, 0, st_n - 1, l, r); }
};

// Segment tree, range query, range update
// set, max
// RMQ
class ST_RQRU {
  typedef ll T;
  static constexpr T d = -inf;
  vector<T> val;
  vector<T> lazy;

public:
  ST_RQRU(T n) : val(4 * n, d), lazy(4 * n, d) {}
  T f(T a, T b) { return max(a, b); }
  void _update(ll x, ll l, ll r, ll ql, ll qr, T v) {
    if (l > qr || r < ql)
      return;

    if (l > ql && r < qr) {
      lazy[x] = v;
      return;
    }

    if (lazy[x] != d) {
      val[x] = f(val[x], lazy[x]);
      lazy[x * 2] = lazy[x];
      lazy[x * 2 + 1] = lazy[x];
      lazy[x] = d;
    }

    ll mid = (l + r) / 2;
    _update(x * 2, l, mid, ql, qr, v);
    _update(x * 2 + 1, mid, r, ql, qr, v);
  }
  T _query(ll x, ll l, ll r, ll ql, ll qr) {
    if (l > qr || r < ql)
      return 0;
    if (l > ql && r < qr)
      return val[x];

    if (lazy[x] != d) {
      val[x] = f(val[x], lazy[x]);
      lazy[x * 2] = lazy[x];
      lazy[x * 2 + 1] = lazy[x];
      lazy[x] = d;
    }

    ll mid = (l + r) / 2;
    return f(_query(x * 2, l, mid, ql, qr), _query(x * 2 + 1, mid, r, ql, qr));
  }
  void update(ll l, ll r, ll v) { _update(1, l, r, l, r, v); }
  T query(ll l, ll r) { return _query(1, l, r, l, r); }
};

// Sparse lazy segment tree
// add, max
// All ops in O(log N), bad constant factor
class ST_RQRU_S {
  typedef ll T;
  typedef ll CHNG_T;
  const T def = 0;
  const T comb_ident = -inf;
  inline T _comb(T a, T b) { return max(a, b); }
  const CHNG_T chng_ident = 0;
  inline T _chng(T old_v, CHNG_T v) { return v + old_v; }

  ll lb, rb;
  ST_RQRU_S *lt, *rt;
  T val;
  CHNG_T lazy;

public:
  ST_RQRU_S(ll lb, ll rb)
      : lb(lb), rb(rb), lt(nullptr), rt(nullptr), val(def), lazy(chng_ident) {}
  void _grow() {
    if (!lt && !rt && lb < rb) {
      ll mid = (lb + rb) / 2;
      lt = new ST_RQRU_S(lb, mid);
      rt = new ST_RQRU_S(mid + 1, rb);
    }

    if (lt && rt) {
      lt->lazy = lt->_chng(lt->lazy, lazy);
      lt->val = lt->_chng(lt->val, lazy);
      rt->lazy = rt->_chng(rt->lazy, lazy);
      rt->val = rt->_chng(rt->val, lazy);
    }
    lazy = chng_ident;
  }
  void update(ll ql, ll qr, CHNG_T v) {
    ql = max(lb, ql), qr = min(rb, qr);
    if (ql > qr)
      return;
    if (ql <= lb && rb <= qr) {
      lazy = _chng(lazy, v);
      val = _chng(val, v);
      return;
    }

    _grow();
    if (lt && rt) {
      lt->update(ql, qr, v);
      rt->update(ql, qr, v);
      val = _comb(lt->val, rt->val);
    }
  }
  T query(ll ql, ll qr) {
    ql = max(lb, ql), qr = min(rb, qr);
    if (ql > qr)
      return comb_ident;
    if (ql <= lb && rb <= qr) {
      return val;
    }

    _grow();

    ll ans = _comb(lt->query(ql, qr), rt->query(ql, qr));
    return ans;
  }
};
