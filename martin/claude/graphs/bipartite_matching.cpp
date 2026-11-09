// ==================== Bipartite Matching (Kuhn's algorithm) ====================
// O(V*E). Left side has n1 nodes, right side n2 nodes, adj[u] lists right-side neighbors.
#include <bits/stdc++.h>
using namespace std;

struct BipartiteMatching {
    int n1, n2;
    vector<vector<int>> adj;
    vector<int> matchL, matchR;
    vector<bool> visited;

    BipartiteMatching(int n1_, int n2_) : n1(n1_), n2(n2_), adj(n1_),
        matchL(n1_, -1), matchR(n2_, -1) {}
    void addEdge(int u, int v) { adj[u].push_back(v); } // u in [0,n1), v in [0,n2)

    bool tryKuhn(int u) {
        for (int v : adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            if (matchR[v] == -1 || tryKuhn(matchR[v])) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        return false;
    }
    int maxMatching() {
        int result = 0;
        for (int u = 0; u < n1; u++) {
            visited.assign(n2, false);
            if (tryKuhn(u)) result++;
        }
        return result;
    }
};
