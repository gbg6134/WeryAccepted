// ==================== Bellman-Ford ====================
// O(V*E). Handles negative edges, detects negative cycles.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = LLONG_MAX / 2;

struct Edge { int u, v; ll w; };

// returns dist array; hasNegCycle set to true if a negative cycle is reachable from src
vector<ll> bellmanFord(int n, int src, vector<Edge>& edges, bool& hasNegCycle) {
    vector<ll> dist(n, INF);
    dist[src] = 0;
    hasNegCycle = false;
    for (int i = 0; i < n - 1; i++) {
        for (auto& e : edges) {
            if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v])
                dist[e.v] = dist[e.u] + e.w;
        }
    }
    // one more pass: if anything still relaxes, there's a negative cycle
    for (auto& e : edges) {
        if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
            hasNegCycle = true;
            break;
        }
    }
    return dist;
}
