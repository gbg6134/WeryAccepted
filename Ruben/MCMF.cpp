// ===========================================================================
// Min-cost max-flow (Dijkstra + potentials)   O(augmentations * E log V)
// mem V + E. Directed. Negative costs OK (one SPFA pass first), negative
// CYCLES are not. No recursion.
//
//   MCMF m(n);                         // vertices 0 .. n-1
//   int id = m.add(u, v, cap, cost);   // cost is per unit of flow
//   auto [fl, c] = m.run(s, t);              // max flow, min cost of those
//   auto [fl, c] = m.run(s, t, k);           // at most k units, min cost
//   auto [fl, c] = m.run(s, t, m.INFF, 1);   // ANY amount, min cost: stops
//                                            // once one more unit costs >= 0
//   m.flow(id)                         // flow on edge id
//
// CONFIG
//   F      capacity / flow type.
//   C      cost type. double works; compare the answer with a tolerance.
//   INFF / INFC   "unbounded" capacity / cost, max / 4 so sums survive.
//
// MAX cost / profit: add every edge with -cost, negate the answer.
// Undirected edge with cost: two separate add calls, one per direction.
// Paths are taken cheapest first, so after k units the cost is the min for
// exactly k, and the cost of each next unit never decreases.
// run is incremental like Dinic: a second call continues from the flow
// already there.
// Each augmentation is one Dijkstra, so the time scales with the number of
// augmenting paths (<= total flow) -- fine up to ~1e4..1e5 of them.
//
// MEASURED at -O2: 1e4 vertices 1e5 edges 0.4 s; 300 x 300 assignment
// (9e4 edges, 300 paths) 0.42 s -- Hungarian does 1000 x 1000 in 0.08 s.
// ===========================================================================
struct MCMF {
    using F = ll; using C = ll;
    static constexpr F INFF = numeric_limits<F>::max() / 4;
    static constexpr C INFC = numeric_limits<C>::max() / 4;
    struct E { int to; F c, f; C w; };     // cap, flow, cost
    int n; vector<E> e; vector<vector<int>> g;
    vector<C> pi, dist; vector<int> pe;    // pe = parent edge

    MCMF(int n = 0) : n(n), g(n), pi(n, 0), dist(n), pe(n) {}
    int add(int u, int v, F c, C w) {
        g[u].push_back(e.size()); e.push_back({v, c, 0, w});
        g[v].push_back(e.size()); e.push_back({u, 0, 0, -w});
        return (int)e.size() - 2;
    }
    F flow(int id) { return e[id].f; }

    void spfa(int s) {                     // exact potentials
        fill(pi.begin(), pi.end(), INFC); pi[s] = 0;
        vector<char> inq(n, 0); deque<int> q{s};
        while (!q.empty()) {
            int u = q.front(); q.pop_front(); inq[u] = 0;
            for (int id : g[u]) {
                auto& x = e[id];
                if (x.c - x.f > 0 && pi[u] + x.w < pi[x.to]) {
                    pi[x.to] = pi[u] + x.w;
                    if (!inq[x.to])
                        inq[x.to] = 1, q.push_back(x.to);
                }
            }
        }
        for (auto& p : pi) if (p == INFC) p = 0;  // unreached
    }
    bool dijkstra(int s, int t) {
        fill(dist.begin(), dist.end(), INFC); dist[s] = 0;
        priority_queue<pair<C,int>, vector<pair<C,int>>,
                       greater<pair<C,int>>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (int id : g[u]) {
                auto& x = e[id];
                C nd = d + x.w + pi[u] - pi[x.to];   // >= 0
                if (x.c - x.f > 0 && nd < dist[x.to]) {
                    dist[x.to] = nd; pe[x.to] = id;
                    pq.push({nd, x.to});
                }
            }
        }
        for (int v = 0; v < n; v++)
            if (dist[v] < INFC) pi[v] += dist[v];
        return dist[t] < INFC;
    }
    pair<F,C> run(int s, int t, F lim = INFF, bool neg = 0) {
        F fl = 0; C cost = 0;
        for (auto& x : e)
            if (x.c - x.f > 0 && x.w < 0) { spfa(s); break; }
        while (fl < lim && dijkstra(s, t)) {
            C pc = pi[t] - pi[s];          // cost of one unit
            if (neg && pc >= 0) break;
            F b = lim - fl;
            for (int v = t; v != s; v = e[pe[v] ^ 1].to)
                b = min(b, e[pe[v]].c - e[pe[v]].f);
            for (int v = t; v != s; v = e[pe[v] ^ 1].to)
                e[pe[v]].f += b, e[pe[v] ^ 1].f -= b;
            fl += b; cost += b * pc;
        }
        return {fl, cost};
    }
};