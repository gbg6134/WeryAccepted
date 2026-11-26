// DEPS: mo intervals dnc knuth cover minrot
int main(){
    mt19937 rng(9);
    // Mo: distinct count vs brute
    rep(it, 0, 50) {
        int n = 1 + rng() % 200; a.assign(n, 0); rep(i, 0, n) a[i] = rng() % 20;
        fill(all(cnt), 0); distinct = 0;
        vector<pii> Q; rep(i, 0, 100) { ll l = rng() % (n + 1), r = rng() % (n + 1); if (l > r) swap(l, r); Q.push_back({l, r}); }
        auto res = mo(Q);
        rep(i, 0, (ll)Q.size()) { set<int> s(a.begin() + Q[i].first, a.begin() + Q[i].second); if ((ll)s.size() != res[i]) { puts("mo"); return 1; } }
    }
    // intervals vs boolean array
    rep(it, 0, 300) {
        set<pii> is; vector<int> cov(40, 0);
        rep(op, 0, 30) {
            ll L = rng() % 40, R = rng() % 41; if (L > R) swap(L, R);
            if (rng() % 2) { addInterval(is, L, R); rep(i, L, R) cov[i] = 1; }
            else { removeInterval(is, L, R); rep(i, L, R) cov[i] = 0; }
            vector<int> c2(40, 0); ll prevR = -1;
            for (auto [l, r] : is) { if (l >= r || l <= prevR) { puts("intervals not disjoint/merged"); return 1; } prevR = r; rep(i, l, r) c2[i] = 1; }
            if (c2 != cov) { puts("intervals"); return 1; }
        }
    }
    // dnc DP: split into k groups minimising sum of (group sum)^2 vs O(k n^2)
    rep(it, 0, 200) {
        int n = 1 + rng() % 30, k = 1 + rng() % n;
        vi a2(n), pre(n + 1, 0); rep(i, 0, n) a2[i] = rng() % 10, pre[i + 1] = pre[i] + a2[i];
        auto C = [&](int l, int r) { ll s = pre[r] - pre[l]; return s * s; };
        vi prv(n + 1, INF), cur(n + 1, INF); prv[0] = 0;
        vi bp = prv, bc(n + 1);
        rep(layer, 0, k) {
            fill(all(cur), INF); dnc(1, n + 1, 0, n, prv, cur, C); prv = cur;
            fill(all(bc), INF);
            rep(i, 1, n + 1) rep(j, 0, i) if (bp[j] < INF) bc[i] = min(bc[i], bp[j] + C(j, i));
            bp = bc;
        }
        if (prv[n] != bp[n]) { printf("dnc %lld %lld\n", prv[n], bp[n]); return 1; }
    }
    // knuth: optimal merge of adjacent piles vs O(n^3)
    rep(it, 0, 200) {
        int n = 1 + rng() % 25; vi pre(n + 1, 0); rep(i, 0, n) pre[i + 1] = pre[i] + rng() % 20;
        auto w = [&](int i, int j) { return pre[j] - pre[i]; };
        auto dp = knuth(n, w);
        vector<vi> b(n + 1, vi(n + 1, 0));
        for (int len = 2; len <= n; len++) for (int i = 0; i + len <= n; i++) {
            int j = i + len; b[i][j] = INF;
            rep(k, i + 1, j) b[i][j] = min(b[i][j], b[i][k] + b[k][j] + w(i, j));
        }
        if (dp[0][n] != b[0][n]) { puts("knuth"); return 1; }
    }
    // interval cover vs brute over subsets
    rep(it, 0, 500) {
        int m = rng() % 8; vector<pii> I; rep(i, 0, m) { ll l = rng() % 10, r = rng() % 11; if (l > r) swap(l, r); I.push_back({l, r}); }
        ll gl = rng() % 10, gr = gl + 1 + rng() % 4;
        int best = 100;
        rep(mask, 0, 1 << m) {
            vector<int> c(20, 0); rep(i, 0, m) if (mask >> i & 1) rep(x, I[i].first, I[i].second) c[x] = 1;
            bool ok = true; rep(x, gl, gr) if (!c[x]) ok = false;
            if (ok) best = min(best, __builtin_popcount(mask));
        }
        auto R = cover(pii(gl, gr), I);
        if ((best == 100) != R.empty() || (best < 100 && (int)R.size() != best)) { puts("cover"); return 1; }
    }
    // minRotation vs brute
    rep(it, 0, 2000) {
        int n = 1 + rng() % 10; string s; rep(i, 0, n) s += char('a' + rng() % 3);
        string best = s; rep(i, 0, n) best = min(best, s.substr(i) + s.substr(0, i));
        int r = minRotation(s);
        if (s.substr(r) + s.substr(0, r) != best) { puts("minRotation"); return 1; }
    }
    puts("OK mo, interval set, D&C DP, Knuth, interval cover, minRotation");
}
