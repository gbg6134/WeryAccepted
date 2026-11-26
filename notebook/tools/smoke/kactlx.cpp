// DEPS: rmq bellmanFord topoSort lis ternSearch
int main(){
    mt19937 rng(777);
    // RMQ vs naive
    rep(it, 0, 300) {
        int n = 1 + rng() % 40;
        vi a(n); for (ll& x : a) x = (ll)(rng() % 100) - 50;
        RMQ<ll> r(a);
        rep(l, 0, n) rep(rr, l + 1, n + 1)
            if (r.query(l, rr) != *min_element(a.begin()+l, a.begin()+rr)) {
                puts("MISMATCH rmq"); return 1; }
    }
    // Bellman-Ford vs Floyd (with -inf through negative cycles)
    rep(it, 0, 1500) {
        int n = 1 + rng() % 7, m = rng() % 14, s = rng() % n;
        vector<Ed> eds;
        rep(i, 0, m) eds.push_back({(int)(rng() % n), (int)(rng() % n),
                                    (int)(rng() % 21) - 6});
        vector<Node> nodes(n);
        vector<Ed> cp = eds;
        bellmanFord(nodes, cp, s);
        const ll I = 1e15;
        vector<vi> d(n, vi(n, I));
        rep(i, 0, n) d[i][i] = 0;
        for (auto& e : eds) d[e.a][e.b] = min(d[e.a][e.b], (ll)e.w);
        rep(k, 0, n) rep(i, 0, n) rep(j, 0, n)
            if (d[i][k] < I && d[k][j] < I)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        rep(v, 0, n) {
            bool reach = d[s][v] < I, neg = false;
            rep(c, 0, n) if (d[s][c] < I && d[c][c] < 0 && d[c][v] < I) neg = true;
            ll want = !reach ? inf : neg ? -inf : d[s][v];
            if (nodes[v].dist != want) { puts("MISMATCH bellmanFord"); return 1; }
        }
    }
    // topoSort: valid order on DAGs, short on graphs with a cycle
    rep(it, 0, 1000) {
        int n = 1 + rng() % 8;
        vector<vi> g(n);
        bool dag = rng() % 2;
        vi perm(n); iota(all(perm), 0); shuffle(all(perm), rng);
        rep(e, 0, rng() % 15) {
            int a = rng() % n, b = rng() % n;
            if (dag) { if (a == b) continue; if (a > b) swap(a, b);
                       g[perm[a]].push_back(perm[b]); }
            else g[a].push_back(b);
        }
        vi q = topoSort(g);
        // cycle exists iff Floyd finds i->...->i
        vector<vector<bool>> R(n, vector<bool>(n));
        rep(u, 0, n) for (ll v : g[u]) R[u][v] = true;
        rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) if (R[i][k] && R[k][j]) R[i][j] = true;
        bool cyc = false; rep(i, 0, n) cyc |= R[i][i];
        if (cyc != ((int)q.size() < n)) { puts("MISMATCH topo cycle"); return 1; }
        if (!cyc) {
            vi pos(n); rep(i, 0, n) pos[q[i]] = i;
            rep(u, 0, n) for (ll v : g[u]) if (pos[u] >= pos[v]) {
                puts("MISMATCH topo order"); return 1; }
        }
    }
    // LIS: strictly increasing, correct length (vs O(n^2) DP)
    rep(it, 0, 2000) {
        int n = rng() % 12;
        vi a(n); for (ll& x : a) x = rng() % 6;
        vi idx = lis(a);
        vi dp(n, 1); ll best = 0;
        rep(i, 0, n) { rep(j, 0, i) if (a[j] < a[i]) dp[i] = max(dp[i], dp[j] + 1);
                       best = max(best, dp[i]); }
        if ((ll)idx.size() != best) { puts("MISMATCH lis length"); return 1; }
        rep(i, 1, (ll)idx.size())
            if (idx[i] <= idx[i-1] || a[idx[i]] <= a[idx[i-1]]) {
                puts("MISMATCH lis seq"); return 1; }
    }
    // ternSearch on strictly unimodal arrays (max)
    rep(it, 0, 2000) {
        int n = 1 + rng() % 30, peak = rng() % n;
        vi a(n); a[peak] = 1000;
        down(i, peak, 0) a[i] = a[i+1] - 1 - rng() % 5;
        rep(i, peak + 1, n) a[i] = a[i-1] - rng() % 5;   // non-increasing after
        int got = ternSearch(0, n - 1, [&](int i){ return a[i]; });
        int want = (int)(max_element(all(a)) - a.begin());
        if (got != want) { puts("MISMATCH ternSearch"); return 1; }
    }
    puts("OK rmq, bellmanFord, topoSort, lis, ternSearch");
}
