// ==================== Max Flow (Dinic's algorithm) ====================
// O(V^2 * E) general, O(E*sqrt(V)) for unit-capacity graphs. Very fast in practice.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = LLONG_MAX / 2;

struct Dinic {
    struct Edge { int to; ll cap, flow; };
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> level, it;
    int n, s, t;

    Dinic(int n_) : adj(n_), n(n_) {}

    void addEdge(int u, int v, ll cap, ll rcap = 0) {
        adj[u].push_back(edges.size());
        edges.push_back({v, cap, 0});
        adj[v].push_back(edges.size());
        edges.push_back({u, rcap, 0}); // reverse edge (0 cap unless graph is undirected)
    }

    bool bfs() {
        level.assign(n, -1);
        queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) {
                auto& e = edges[id];
                if (level[e.to] == -1 && e.cap - e.flow > 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(int u, ll pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int& i = it[u]; i < (int)adj[u].size(); i++) {
            int id = adj[u][i];
            auto& e = edges[id];
            if (level[e.to] != level[u] + 1 || e.cap - e.flow <= 0) continue;
            ll d = dfs(e.to, min(pushed, e.cap - e.flow));
            if (d > 0) {
                e.flow += d;
                edges[id ^ 1].flow -= d;
                return d;
            }
        }
        return 0;
    }

    ll maxflow(int s_, int t_) {
        s = s_; t = t_;
        ll flow = 0;
        while (bfs()) {
            it.assign(n, 0);
            while (ll pushed = dfs(s, INF)) flow += pushed;
        }
        return flow;
    }
};
// Usage: Dinic din(n); din.addEdge(u, v, cap); din.maxflow(source, sink);
