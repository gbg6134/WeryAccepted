// ==================== Bridges and Articulation Points ====================
// O(V+E). Undirected graph, may have multi-edges (pass edge id to skip parent edge correctly).
#include <bits/stdc++.h>
using namespace std;

struct BridgesArticulation {
    int n, timer = 0;
    vector<vector<pair<int,int>>> adj; // (neighbor, edge_id)
    vector<int> disc, low;
    vector<bool> visited, isArticulation;
    vector<pair<int,int>> bridges;

    BridgesArticulation(int n_) : n(n_), adj(n_), disc(n_), low(n_),
        visited(n_, false), isArticulation(n_, false) {}

    void addEdge(int u, int v, int id) {
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
    }

    void dfs(int u, int parentEdge) {
        visited[u] = true;
        disc[u] = low[u] = timer++;
        int children = 0;
        for (auto [v, id] : adj[u]) {
            if (id == parentEdge) continue;
            if (visited[v]) {
                low[u] = min(low[u], disc[v]);
            } else {
                children++;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > disc[u]) bridges.push_back({u, v});
                if (disc[u] <= low[v] && parentEdge != -1) isArticulation[u] = true;
            }
        }
        if (parentEdge == -1 && children > 1) isArticulation[u] = true; // root case
    }

    void run() {
        for (int i = 0; i < n; i++) if (!visited[i]) dfs(i, -1);
    }
};
