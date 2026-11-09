// ==================== LCA with Binary Lifting ====================
// O(n log n) preprocessing, O(log n) per query. Also gives distance between nodes.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct LCA {
    int n, LOG;
    vector<vector<int>> up;
    vector<int> depth;
    vector<vector<pair<int,ll>>> adj; // (neighbor, weight)
    vector<ll> distFromRoot;

    LCA(int n_) : n(n_), adj(n_) {
        LOG = 32 - __builtin_clz(max(n, 2));
        up.assign(LOG, vector<int>(n, 0));
        depth.assign(n, 0);
        distFromRoot.assign(n, 0);
    }
    void addEdge(int u, int v, ll w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    void dfs(int u, int parent) {
        up[0][u] = parent;
        for (int k = 1; k < LOG; k++)
            up[k][u] = up[k-1][up[k-1][u]];
        for (auto [v, w] : adj[u]) {
            if (v != parent) {
                depth[v] = depth[u] + 1;
                distFromRoot[v] = distFromRoot[u] + w;
                dfs(v, u);
            }
        }
    }
    void build(int root = 0) { dfs(root, root); }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int k = 0; k < LOG; k++) if ((diff >> k) & 1) u = up[k][u];
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--) {
            if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
        }
        return up[0][u];
    }
    ll dist(int u, int v) {
        int a = lca(u, v);
        return distFromRoot[u] + distFromRoot[v] - 2 * distFromRoot[a];
    }
};
