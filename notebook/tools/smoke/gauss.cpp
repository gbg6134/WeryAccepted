// DEPS: gauss
int main(){
    mt19937 rng(9001);
    rep(iter, 0, 400) {
        int n = 1 + rng() % 6;
        vector<vector<ld>> a(n, vector<ld>(n));
        vector<ld> xtrue(n), b(n, 0);
        rep(i, 0, n) xtrue[i] = (ld)((ll)(rng() % 21) - 10);
        rep(i, 0, n) rep(j, 0, n)
            a[i][j] = (ld)((ll)(rng() % 21) - 10);
        rep(i, 0, n) rep(j, 0, n) b[i] += a[i][j] * xtrue[j];
        vector<ld> x;
        int r = gauss(a, b, x);
        if (r != n) continue;              // singular, skip
        rep(i, 0, n) {                     // residual must vanish
            ld s = 0;
            rep(j, 0, n) s += a[i][j] * x[j];
            if (fabsl(s - b[i]) > 1e-6) {
                puts("MISMATCH residual"); return 1;
            }
        }
    }
    puts("OK 400 systems: full-rank solutions satisfy A*x == b");
}
