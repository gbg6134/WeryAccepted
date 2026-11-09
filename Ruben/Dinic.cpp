// ===========================================================================
// Dinic (max flow + min cut, iterative)   O(V^2 E), far less in practice
// mem ~50 B per edge. Directed, capacities. No recursion, so path length
// never matters. Multi-edges, self-loops, 0-capacity edges are all fine.
//
//   Dinic d(n);                 // vertices 0 .. n-1
//   int id = d.add(u, v, c);    // edge u -> v, capacity c; returns its id
//   d.add(u, v, c, c);          // UNDIRECTED edge: 4th arg = reverse cap
//   F f = d.maxflow(s, t);      // pushes as much MORE flow as possible
//
//   d.flow(id)                  // flow on edge id; negative = it runs v -> u
//   d.inS(v)                    // after maxflow: v on the s side of a min cut
//   d.cut()                     // edge ids crossing S -> T, all saturated
//   d.reset()                   // all flow back to 0, edges kept
//
// CONFIG
//   F     capacity / flow type. int saves memory, double works if EPS is set.
//   EPS   a residual <= EPS counts as saturated: 0 for integers, 1e-9 double.
//   INF   capacity that may never be cut. It is max / 4, so up to 3 INF can
//         add up; a maxflow result >= INF means no finite cut exists.
//
// maxflow is INCREMENTAL: it keeps the flow already there. Add edges or raise
// d.e[id].c, call it again, and it returns only the extra flow. Another
// (s, t) on the same object also continues from the old flow -- reset()
// first unless that is what you want.
//
// inS / cut describe the LAST maxflow call and give the min cut closest to s.
// cut() returns an odd id when an undirected edge crosses in its reverse
// direction; id & ~1 is always the id add returned. Sum of d.e[id].c over
// cut() equals the flow.
// Edge ids: add returns an even id, id ^ 1 is its paired reverse edge;
// d.e[id].to / .c / .f are public, edit them between calls.
//
// MEASURED at -O2: random 1e5 vertices 1e6 edges 0.4 s; unit-capacity
// bipartite 2e5 vertices 1.2e6 edges 0.8 s 121 MB; 300 x 300 grid with
// random two-way caps 4.5 s (a bad case for every Dinic, KACTL's included).
// ===========================================================================
struct Dinic {
    using F = ll;
    static constexpr F EPS = 0;            // double: 1e-9
    static constexpr F INF = numeric_limits<F>::max() / 4;
    struct E { int to; F c, f; };                  // cap, flow
    int n; vector<E> e; vector<vector<int>> g;
    vector<int> lv, it;                    // level, edge ptr

    Dinic(int n = 0) : n(n), g(n), lv(n), it(n) {}
    int add(int u, int v, F c, F rc = 0) {     // id of u->v
        g[u].push_back(e.size()); e.push_back({v, c, 0});
        g[v].push_back(e.size()); e.push_back({u, rc, 0});
        return (int)e.size() - 2;
    }
    F res(int id) { return e[id].c - e[id].f; }
    F flow(int id) { return e[id].f; }
    bool inS(int v) { return lv[v] >= 0; }
    void reset() { for (auto& x : e) x.f = 0; }

    bool bfs(int s, int t) {
        fill(lv.begin(), lv.end(), -1); lv[s] = 0;
        vector<int> q{s};
        for (int i = 0; i < (int)q.size() && lv[t] < 0; i++)
            for (int id : g[q[i]]) {
                int v = e[id].to;
                if (lv[v] < 0 && res(id) > EPS)
                    lv[v] = lv[q[i]] + 1, q.push_back(v);
            }
        return lv[t] >= 0;
    }
    F maxflow(int s, int t) {
        F tot = 0; vector<int> st;         // path, edge ids
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            int u = s;
            while (true) {
                if (u == t) {                  // augment
                    F b = INF; int k = 0;
                    for (int id : st) b = min(b, res(id));
                    for (int i = st.size(); i--;) {
                        e[st[i]].f += b; e[st[i] ^ 1].f -= b;
                        if (res(st[i]) <= EPS) k = i;  // 1st sat.
                    }
                    tot += b; st.resize(k);
                    u = k ? e[st.back()].to : s;
                } else if (it[u] < (int)g[u].size()) {
                    int id = g[u][it[u]], v = e[id].to;
                    if (lv[v] == lv[u] + 1 && res(id) > EPS)
                        st.push_back(id), u = v;
                    else it[u]++;
                } else {                       // dead end
                    if (u == s) break;
                    lv[u] = -1;
                    u = e[st.back() ^ 1].to;
                    st.pop_back(); it[u]++;
                }
            }
        }
        return tot;
    }
    vector<int> cut() {                    // after maxflow
        vector<int> r;
        for (int id = 0; id < (int)e.size(); id++) {
            int u = e[id ^ 1].to, v = e[id].to;
            if (e[id].c > 0 && inS(u) && !inS(v))
                r.push_back(id);
        }
        return r;
    }
};