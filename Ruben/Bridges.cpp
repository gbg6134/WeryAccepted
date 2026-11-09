// ===========================================================================
// BRIDGES + CUT VERTICES + 2ECC (iterative)   O(n + m)   mem n + m
// Undirected graph given as an EDGE LIST, which is what makes multi-edges
// safe: a doubled edge is never a bridge, a self-loop never matters. No
// recursion, so any depth is fine. Disconnected input is fine.
//
//   Bridges b(n, es);      // es = edge list, 0-indexed. Any integer type
//                          // and pair / array<_,2> / tuple.
//
//   b.bri[i]               // is INPUT EDGE i a bridge
//   b.art[v]               // is v a cut vertex (articulation point)
//   b.comp[v]              // 2-edge-connected comp of v, 0 .. nc-1
//   b.nc                   // number of 2ecc
//   b.tree()               // bridge forest on the comps, adjacency
//
// comp merges everything reachable without crossing a bridge, so every cycle
// sits inside one comp and the bridges are exactly the edges between comps.
// tree() is a FOREST, one tree per connected component of the input; feed it
// to LCA / HLD for "how many bridges on the u-v path" questions. Adding an
// edge inside a comp changes nothing; adding one between comps kills every
// bridge on the path joining them.
//
// The parent edge is skipped by EDGE ID, never by vertex -- skipping by
// vertex reports parallel edges as bridges and is the usual wrong answer.
//
// Cut vertices are NOT the vertex version of bridges: an endpoint of a
// bridge need not be a cut vertex (degree 1), and a cut vertex need not
// carry one (two cycles sharing one vertex). The root of a DFS tree is a cut
// vertex iff it has >= 2 children, which is why it is patched separately.
// For the vertex-side decomposition you want biconnected components, a
// different stack -- comp here is the EDGE-side one.
// ===========================================================================
struct Bridges {
    int n, m, nc = 0;                  // nc = number of 2ecc
    vector<vector<pair<int,int>>> g;   // (to, edge id)
    vector<int> comp;                  // 2ecc id per vertex
    vector<char> bri, art;          // per edge / vertex
    vector<pair<int,int>> es;

    template<class E>      // list of pair / array / tuple
    Bridges(int n, const E& edges)
            : n(n), m(edges.size()), g(n), comp(n, -1),
              bri(m, 0), art(n, 0) {
        es.reserve(m);
        for (auto& e : edges)
            es.push_back({(int)get<0>(e), (int)get<1>(e)});
        vector<int> deg(n, 0);
        for (int i = 0; i < m; i++) {
            deg[es[i].first]++; deg[es[i].second]++;
        }
        for (int v = 0; v < n; v++) g[v].reserve(deg[v]);
        for (int i = 0; i < m; i++) {
            g[es[i].first].push_back({es[i].second, i});
            g[es[i].second].push_back({es[i].first, i});
        }
        vector<int> tin(n, -1), low(n), ptr(n, 0), cs;
        vector<int> pe(n, -1);
        int timer = 0;
        for (int r = 0; r < n; r++) {
            if (tin[r] != -1) continue;
            int kids = 0;
            tin[r] = low[r] = timer++; cs.push_back(r);
            while (!cs.empty()) {
                int u = cs.back();
                if (ptr[u] < (int)g[u].size()) {
                    auto [v, id] = g[u][ptr[u]++];
                    if (id == pe[u]) continue;   // skip by ID
                    if (tin[v] == -1) {
                        pe[v] = id; tin[v] = low[v] = timer++;
                        cs.push_back(v); kids += (u == r);
                    } else low[u] = min(low[u], tin[v]);
                } else {
                    cs.pop_back();
                    if (cs.empty()) break;
                    int p = cs.back();
                    low[p] = min(low[p], low[u]);
                    if (low[u] > tin[p]) bri[pe[u]] = 1;
                    if (low[u] >= tin[p]) art[p] = 1;
                }
            }
            art[r] = kids >= 2;                  // root rule
        }
        vector<int> q;                // 2ecc: drop bridges
        for (int r = 0; r < n; r++) {
            if (comp[r] != -1) continue;
            comp[r] = nc; q.clear(); q.push_back(r);
            while (!q.empty()) {
                int u = q.back(); q.pop_back();
                for (auto [v, id] : g[u])
                    if (!bri[id] && comp[v] == -1) {
                        comp[v] = nc; q.push_back(v);
                    }
            }
            nc++;
        }
    }
    Bridges(int n, initializer_list<pair<int,int>> e)
            : Bridges(n, vector<pair<int,int>>(e)) {}

    vector<vector<int>> tree() {     // bridge forest
        vector<vector<int>> t(nc);
        for (int i = 0; i < m; i++) if (bri[i]) {
            int a = comp[es[i].first], b = comp[es[i].second];
            t[a].push_back(b); t[b].push_back(a);
        }
        return t;
    }
};