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

// https://cp-algorithms.com/graph/lca_binary_lifting.html
// Binary lifted lowest common ancestor (LCA) for a tree
// Construction: O(N log N)
//
// anc(u, v): true if u is ancestor of v (O(1))
// lca(u, v): LCA of u and v (O(log N))
//
// TODO: Can be reduced by a log factor
class LCA_BL {
  const static ll max_2_n = 20; // Maximum number of nodes: 20 -> n < 1e6
  vector<vector<ll>> adj;
  ll n;
  vector<ll[max_2_n + 1]> binlif;
  vector<ll> tin, tout;
  ll timer;

public:
  LCA_BL(vector<vector<ll>> adj_, ll root = 0)
      : binlif(adj_.size()), tin(adj_.size()), tout(adj_.size()) {
    adj = adj_;
    n = adj.size();
    timer = 0;
    _init_dfs(root, root);
  }
  void _init_dfs(ll u, ll p) {
    tin[u] = ++timer;
    binlif[u][0] = p;
    for (int i = 1; i <= max_2_n; i++)
      binlif[u][i] = binlif[binlif[u][i - 1]][i - 1];
    for (ll v : adj[u])
      if (v != p)
        _init_dfs(v, u);
    tout[u] = ++timer;
  }
  bool anc(ll u, ll v) { return tin[u] <= tin[v] && tout[u] >= tout[v]; }
  ll lca(ll u, ll v) {
    if (anc(u, v))
      return u;
    if (anc(v, u))
      return v;
    for (int i = max_2_n; i >= 0; i--) {
      if (!anc(binlif[u][i], v))
        u = binlif[u][i];
    }
    return binlif[u][0];
  }
};

// https://robert1003.github.io/2020/01/16/centroid-decomposition.html
// Constructs a centroid decomposition tree
// Construction: O(N log N)
class CD_tree {
  vector<set<ll>> adj;
  vector<ll> par;
  vector<ll> sz;
  ll n;

public:
  CD_tree(vector<vector<ll>> adj_) : sz(adj_.size()), par(adj_.size()) {
    for (ll u = 0; u < adj_.size(); u++)
      for (auto v : adj_[u])
        adj[u].insert(v);
    n = adj.size();
  }
  ll _centroid(ll u, ll p, ll n) {
    for (ll v : adj[u])
      if (v != p)
        if (sz[v] > n / 2)
          return _centroid(v, u, n);
    return u;
  }
  ll _init_dfs(ll u, ll p) {
    sz[u] = 1;
    for (ll v : adj[u])
      if (v != p)
        sz[u] += _init_dfs(v, u);
    return sz[u];
  }
  void _build(ll u, ll p) {
    ll n = _init_dfs(u, p);
    ll c = _centroid(u, p, n);
    if (p == -1)
      p = c;
    par[c] = p;

    vector<ll> tmp(adj[c].begin(), adj[c].end());
    for (ll v : tmp) {
      adj[c].erase(v);
      adj[v].erase(c);
      _build(v, c);
    }
  }
};
