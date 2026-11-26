// DEPS: dnc knuth
int main(){
    mt19937 rng(9);
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
    puts("OK D&C DP 200 vs O(k n^2), Knuth 200 vs O(n^3)");
}
