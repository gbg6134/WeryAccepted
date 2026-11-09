// ==================== Kruskal's MST ====================
// O(E log E). Requires DSU (see data_structures/dsu.cpp).
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct DSU {
    vector<int> parent, sz;
    DSU(int n) : parent(n), sz(n, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; } return x; }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a; sz[a] += sz[b];
        return true;
    }
};

struct Edge { int u, v; ll w; };

// returns {total MST weight, list of edges used}. -1 total if graph disconnected.
pair<ll, vector<Edge>> kruskal(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    DSU dsu(n);
    ll total = 0;
    vector<Edge> used;
    int edgesUsed = 0;
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            used.push_back(e);
            edgesUsed++;
        }
    }
    if (edgesUsed != n - 1) return {-1, {}};
    return {total, used};
}
