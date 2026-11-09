// ===========================================================================
// SCC (Tarjan, iterative)   O(n + m)   mem n + m
// Strongly connected components of a DIRECTED graph. No recursion, so any
// depth is fine. Self-loops and multi-edges are OK.
//
//   SCC s(g);              // g = adjacency, 0-indexed. Any integer type:
//                          // vector<vector<int>>, matrix, vector<vi>, ...
//
//   s.nc                   // number of components
//   s.comp[v]              // component id of v, 0 .. nc-1
//   s.members()            // vector<vector<int>>, the vertices of each comp
//   s.dag()                // condensation as adjacency, duplicates removed
//
// comp is in TOPOLOGICAL order: for every edge u -> v either comp[u] ==
// comp[v] or comp[u] < comp[v]. A DP over the condensation is then just a
// loop over ids 0, 1, ..., nc-1 (or backwards), no separate toposort.
//
// u and v are in the same comp iff u reaches v AND v reaches u.
// A comp contains a cycle iff it holds >= 2 vertices, or 1 vertex carrying a
// self-loop -- a lone vertex without one is a comp but has no cycle. So the
// graph is a DAG iff nc == n and there is no self-loop.
// Sources of the condensation have in-degree 0 in dag(), sinks out-degree 0;
// comp[v] == 0 is not enough, count degrees.
//
// 2-SAT -- variable i owns nodes 2i (x_i false) and 2i+1 (x_i true), so n =
// 2*vars. Clause (a OR b) adds BOTH edges !a -> b and !b -> a. Forcing a
// true is the clause (a OR a), i.e. the single edge !a -> a.
//   satisfiable  iff  comp[2i] != comp[2i+1] for every i
//   x_i = true   iff  comp[2i+1] > comp[2i]       (larger id = later)
// That last rule depends on comp being topological, as it is here. With raw
// Tarjan ids (reverse topological) the comparison flips.
// ===========================================================================
struct SCC {
    int n, nc = 0;              // nc = number of comps
    vector<vector<int>> g;
    vector<int> comp;           // topological order

    template<class G>          // any vector<vector<intlike>>
    SCC(const G& adj) : n(adj.size()), g(n), comp(n, -1) {
        for (int u = 0; u < n; u++)
            for (auto v : adj[u]) g[u].push_back((int)v);
        vector<int> tin(n, -1), low(n), ptr(n, 0), stk, cs;
        vector<char> on(n, 0);
        int timer = 0;
        for (int r = 0; r < n; r++) {
            if (tin[r] != -1) continue;
            tin[r] = low[r] = timer++;
            stk.push_back(r); on[r] = 1; cs.push_back(r);
            while (!cs.empty()) {
                int u = cs.back();
                if (ptr[u] < (int)g[u].size()) {
                    int v = g[u][ptr[u]++];
                    if (tin[v] == -1) {
                        tin[v] = low[v] = timer++;
                        stk.push_back(v); on[v] = 1;
                        cs.push_back(v);
                    } else if (on[v])
                        low[u] = min(low[u], tin[v]);
                } else {
                    cs.pop_back();
                    if (!cs.empty()) {
                        int p = cs.back();
                        low[p] = min(low[p], low[u]);
                    }
                    if (low[u] == tin[u]) {  // u roots comp
                        int w;
                        do {
                            w = stk.back(); stk.pop_back();
                            on[w] = 0; comp[w] = nc;
                        } while (w != u);
                        nc++;
                    }
                }
            }
        }
        for (int v = 0; v < n; v++)         // -> topological
            comp[v] = nc - 1 - comp[v];
    }
    vector<vector<int>> members() {
        vector<vector<int>> c(nc);
        for (int v = 0; v < n; v++) c[comp[v]].push_back(v);
        return c;
    }
    vector<vector<int>> dag() {      // condensation, dedup
        vector<vector<int>> d(nc);
        for (int u = 0; u < n; u++)
            for (int v : g[u])
                if (comp[u] != comp[v])
                    d[comp[u]].push_back(comp[v]);
        for (auto& a : d) {
            sort(a.begin(), a.end());
            a.erase(unique(a.begin(), a.end()), a.end());
        }
        return d;
    }
};