// DEPS: Dinic globalMinCut gomoryHu modbasics matInv generalMatching SCC twosat
int main(){
    mt19937 rng(7);
    // globalMinCut vs all bipartitions
    rep(it, 0, 300) {
        int n = 2 + rng() % 7;
        vector<vi> w(n, vi(n, 0));
        rep(i, 0, n) rep(j, i + 1, n) if (rng() % 2)
            w[i][j] = w[j][i] = rng() % 10;
        ll best = LLONG_MAX;
        rep(m, 1, (1 << n) - 1) {
            ll c = 0;
            rep(i, 0, n) rep(j, 0, n)
                if ((m >> i & 1) && !(m >> j & 1)) c += w[i][j];
            best = min(best, c);
        }
        auto [val, side] = globalMinCut(w);
        vector<int> in(n, 0); for (int v : side) in[v] = 1;
        ll c = 0;
        rep(i, 0, n) rep(j, 0, n) if (in[i] && !in[j]) c += w[i][j];
        if (val != best || c != best || side.empty() || (int)side.size() == n) {
            printf("globalMinCut MISMATCH %lld %lld %lld\n", val, c, best); return 1;
        }
    }
    // gomoryHu vs Dinic for every pair
    rep(it, 0, 200) {
        int n = 2 + rng() % 6;
        vector<array<ll,3>> ed;
        int m = rng() % 12;
        rep(i, 0, m) {
            int a = rng() % n, b = rng() % n;
            if (a != b) ed.push_back({a, b, (ll)(rng() % 9)});
        }
        auto tr = gomoryHu(n, ed);
        vector<vector<pii>> g(n);
        for (auto [a, b, c] : tr) { g[a].push_back({b, c}); g[b].push_back({a, c}); }
        rep(s, 0, n) {
            vi mn(n, -1); mn[s] = LLONG_MAX; vector<int> st{(int)s};
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (auto [v, c] : g[u]) if (mn[v] < 0) { mn[v] = min(mn[u], c); st.push_back(v); }
            }
            rep(t, 0, n) if (t != s) {
                Dinic d(n);
                for (auto [a, b, c] : ed) d.add(a, b, c, c);
                ll f = d.maxflow(s, t);
                if (f != mn[t]) { printf("gomoryHu MISMATCH %lld %lld\n", f, mn[t]); return 1; }
            }
        }
    }
    // generalMatching vs bitmask DP
    rep(it, 0, 300) {
        int n = 1 + rng() % 10;
        vector<pii> ed; vector<vector<int>> adj(n, vector<int>(n, 0));
        rep(i, 0, n) rep(j, i + 1, n) if (rng() % 3 == 0) { ed.push_back({i, j}); adj[i][j] = adj[j][i] = 1; }
        vector<int> dp(1 << n, 0);
        rep(m, 1, 1 << n) {
            int i = __builtin_ctz(m); int r = m & ~(1 << i);
            dp[m] = dp[r];
            rep(j, 0, n) if ((r >> j & 1) && adj[i][j]) dp[m] = max(dp[m], 1 + dp[r & ~(1 << j)]);
        }
        auto res = generalMatching(n, ed);
        vector<int> used(n, 0); bool ok = true;
        for (auto [a, b] : res) {
            if (!adj[a][b] || used[a] || used[b]) ok = false;
            used[a] = used[b] = 1;
        }
        if (!ok || (int)res.size() != dp[(1 << n) - 1]) {
            printf("generalMatching MISMATCH got %d want %d\n", (int)res.size(), dp[(1 << n) - 1]); return 1;
        }
    }
    // 2-SAT vs brute force (either, setValue, implies, atMostOne)
    rep(it, 0, 2000) {
        int n = 1 + rng() % 6;
        TwoSat ts(n);
        vector<array<int,3>> cl;   // type, a, b
        int m = rng() % 8;
        auto lit = [&]() { int v = rng() % n; return rng() % 2 ? v : ~v; };
        auto ev = [&](int l, int mask) { return l >= 0 ? (mask >> l & 1) : !(mask >> ~l & 1); };
        vector<vector<int>> amo;
        rep(i, 0, m) {
            int t = rng() % 3, a = lit(), b = lit();
            if (t == 0) ts.either(a, b);
            else if (t == 1) ts.implies(a, b);
            else ts.setValue(a);
            cl.push_back({t, a, b});
        }
        if (rng() % 2) {
            vector<int> li; int k = rng() % 4;
            rep(i, 0, k) li.push_back(lit());
            ts.atMostOne(li); amo.push_back(li);
        }
        auto okm = [&](int mask) {
            for (auto [t, a, b] : cl) {
                if (t == 0 && !(ev(a, mask) || ev(b, mask))) return false;
                if (t == 1 && ev(a, mask) && !ev(b, mask)) return false;
                if (t == 2 && !ev(a, mask)) return false;
            }
            for (auto& li : amo) { int c = 0; for (int l : li) c += ev(l, mask); if (c > 1) return false; }
            return true;
        };
        bool any = false;
        rep(mask, 0, 1 << n) if (okm(mask)) any = true;
        bool got = ts.solve();
        if (got != any) { printf("2SAT existence MISMATCH\n"); return 1; }
        if (got) {
            int mask = 0; rep(i, 0, n) if (ts.val[i]) mask |= 1 << i;
            if (!okm(mask)) { printf("2SAT assignment invalid\n"); return 1; }
        }
    }
    puts("OK globalMinCut 300, gomoryHu 200 graphs all pairs, generalMatching 300 vs bitmask DP, 2-SAT 2000 vs brute");
}
