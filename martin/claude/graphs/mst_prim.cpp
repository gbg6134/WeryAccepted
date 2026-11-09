// ==================== Prim's MST ====================
// O(E log V) with a priority queue and adjacency list.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = LLONG_MAX / 2;

ll prim(int n, vector<vector<pair<int,ll>>>& adj, int src = 0) {
    vector<ll> minEdge(n, INF);
    vector<bool> inMST(n, false);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
    minEdge[src] = 0;
    pq.push({0, src});
    ll total = 0;
    int cnt = 0;
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        total += w;
        cnt++;
        for (auto [v, wt] : adj[u]) {
            if (!inMST[v] && wt < minEdge[v]) {
                minEdge[v] = wt;
                pq.push({wt, v});
            }
        }
    }
    return (cnt == n) ? total : -1; // -1 if graph disconnected
}
