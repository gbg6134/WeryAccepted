// ==================== Strongly Connected Components (Tarjan) ====================
// O(V+E). comp[] holds SCC id for each vertex (SCCs numbered in reverse topological order).
#include <bits/stdc++.h>
using namespace std;

struct Tarjan {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> disc, low, comp;
    vector<bool> onStack;
    stack<int> st;
    int sccCount = 0;

    Tarjan(int n_) : n(n_), adj(n_), disc(n_, -1), low(n_, -1), comp(n_, -1), onStack(n_, false) {}
    void addEdge(int u, int v) { adj[u].push_back(v); }

    void dfs(int u) {
        disc[u] = low[u] = timer++;
        st.push(u); onStack[u] = true;
        for (int v : adj[u]) {
            if (disc[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (onStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }
        if (low[u] == disc[u]) {
            while (true) {
                int v = st.top(); st.pop(); onStack[v] = false;
                comp[v] = sccCount;
                if (v == u) break;
            }
            sccCount++;
        }
    }
    void run() {
        for (int i = 0; i < n; i++) if (disc[i] == -1) dfs(i);
    }
};
