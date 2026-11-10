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

// Union-find with merge-largest
class UF_SZ {
  vector<ll> p;
  vector<ll> sz;

public:
  UF_SZ(ll n) : p(n, -1), sz(n, 1) {}
  ll size(ll u) { return sz[find(u)]; }
  ll alone(ll u) { return p[u] < 0; }
  ll find(ll u) { return p[u] < 0 ? u : p[u] = find(p[u]); }
  void join(ll u, ll v) {
    ll a = find(v), b = find(u);
    if (a == b)
      return;
    if (sz[a] > sz[b])
      swap(a, b);
    p[a] = b;
    sz[b] += sz[a];
  }
};

// Union-find with merge-random
class UF_RD {
  vector<ll> p;
  random_device rd;
  uniform_int_distribution<mt19937::result_type> dist;
  mt19937 rng;

public:
  UF_RD(ll n) : p(n, -1), dist(0, 1), rng(rd()) {}
  ll find(ll u) { return p[u] < 0 ? u : p[u] = find(p[u]); }
  void join(ll u, ll v) {
    ll a = find(v), b = find(u);
    if (a == b)
      return;
    if (dist(rng) < 0.5)
      swap(a, b);
    p[a] = b;
  }
};

// Union-find with merge-largest and move operation
class UF_MV {
  vector<ll> p;
  vector<ll> sz;

public:
  UF_MV(ll n) : p(n * 2, -1), sz(n * 2, 1) {}
  ll size(ll u) { return sz[find(u)]; }
  ll alone(ll u) { return p[u] < 0; }
  ll find(ll u) { return p[u] < 0 ? u + p.size() : p[u] = find(p[u]); }
  void join(ll u, ll v) {
    ll a = find(v), b = find(u);
    if (a == b)
      return;
    if (sz[a] > sz[b])
      swap(a, b);
    p[a] = b;
    sz[b] += sz[a];
  }
  void move(ll u, ll v) {
    ll a = find(v), b = find(u);
    if (a == b)
      return;
    sz[b]--;
    sz[a]++;
    p[b] = a;
  }
};

// Unionfind (DSU) with rollback
class UF_SZ_ROLL {
  vector<ll> p;
  vector<ll> sz;
  stack<tuple<ll, ll, ll, ll>> op;

public:
  UF_SZ_ROLL(ll n) : p(n, -1), sz(n, 1) {}
  ll size(ll u) { return sz[find(u)]; }
  ll find(ll u) { return (p[u] < 0 || p[u] == u) ? u : find(p[u]); }
  bool join(ll u, ll v) {
    ll a = find(v), b = find(u);
    if (sz[a] > sz[b])
      swap(a, b);
    op.push({a, sz[a], b, sz[b]});
    if (a == b)
      return false;
    p[a] = b;
    sz[b] += sz[a];
    return true;
  }
  void rollback() {
    if (op.empty())
      return;
    auto o = op.top();
    op.pop();

    p[get<0>(o)] = get<0>(o);
    sz[get<0>(o)] = get<1>(o);
    p[get<2>(o)] = get<2>(o);
    sz[get<2>(o)] = get<3>(o);
  }
};

// Union-find (DSU) with priority rollback
// https://gist.github.com/brunomaletta/81ba61f5e1e508e5f7e1d536dfd19958#file-priority_queue_ds-cpp-L40
class UF_PRI_ROLL {
public:
  UF_SZ_ROLL ds;
  vector<tuple<pair<ll, ll>, ll, ll>> upd;
  set<pair<ll, ll>> ops;
  vector<ll> pos;

  UF_PRI_ROLL(ll n) : ds(n) {}
  void join(ll u, ll v, ll p) {
    ds.join(u, v);
    ops.emplace(p, pos.size());
    upd.emplace_back(make_pair(u, v), p, (ll)pos.size());
    pos.push_back(upd.size() - 1);
  }
  ll size(ll u) { return ds.size(u); }
  ll find(ll u) { return ds.find(u); }
  void revert() {
    ll k = 1, min_p = 0;
    vector<tuple<pair<ll, ll>, ll, ll>> small, big;
    auto it = ops.end();
    for (ll qt = 0; qt++ < (k + 1) / 2;) {
      it--;
      min_p = it->fi;
      ll i = pos[it->se];
      if (qt > 1)
        big.push_back(upd[i]);
      k = max(k, (ll)upd.size() - i);
    }

    for (ll i = 0; i < k; i++) {
      ds.rollback();
      auto [u, p, idx] = upd.rbegin()[i];
      if (p < min_p)
        small.emplace_back(u, p, idx);
    }

    ops.erase(prev(ops.end()));
    upd.erase(upd.end() - k, upd.end());

    small.insert(small.end(), big.rbegin(), big.rend());
    for (auto [u, p, idx] : small) {
      ds.join(u.fi, u.se);
      upd.emplace_back(u, p, idx);
      pos[idx] = upd.size() - 1;
    }
  }
};
