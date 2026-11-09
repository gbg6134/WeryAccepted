// ==================== Min Cost Max Flow (SPFA / Bellman-Ford based) ====================
// O(V*E) per augmenting path (handles negative-cost edges, no negative cycles).
// For all non-negative costs, swap SPFA for Dijkstra + potentials for speed.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = LLONG_MAX / 2;

struct MCMF {
    struct Edge { int to; ll cap, cost; };
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<ll> dist;
    vector<int> parentEdge;
    vector<bool> inQueue;
    int n;

    MCMF(int n_) : adj(n_), n(n_) {}

    void addEdge(int u, int v, ll cap, ll cost) {
        adj[u].push_back(edges.size());
        edges.push_back({v, cap, cost});
        adj[v].push_back(edges.size());
        edges.push_back({u, 0, -cost}); // reverse edge
    }

    bool spfa(int s, int t) {
        dist.assign(n, INF);
        parentEdge.assign(n, -1);
        inQueue.assign(n, false);
        dist[s] = 0;
        queue<int> q;
        q.push(s); inQueue[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop(); inQueue[u] = false;
            for (int id : adj[u]) {
                auto& e = edges[id];
                if (e.cap > 0 && dist[u] + e.cost < dist[e.to]) {
                    dist[e.to] = dist[u] + e.cost;
                    parentEdge[e.to] = id;
                    if (!inQueue[e.to]) { q.push(e.to); inQueue[e.to] = true; }
                }
            }
        }
        return dist[t] < INF;
    }

    // returns {maxflow, mincost}
    pair<ll,ll> run(int s, int t) {
        ll flow = 0, cost = 0;
        while (spfa(s, t)) {
            ll pushed = INF;
            for (int v = t; v != s; ) {
                int id = parentEdge[v];
                pushed = min(pushed, edges[id].cap);
                v = edges[id ^ 1].to;
            }
            for (int v = t; v != s; ) {
                int id = parentEdge[v];
                edges[id].cap -= pushed;
                edges[id ^ 1].cap += pushed;
                v = edges[id ^ 1].to;
            }
            flow += pushed;
            cost += pushed * dist[t];
        }
        return {flow, cost};
    }
};
