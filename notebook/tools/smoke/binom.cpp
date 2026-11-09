// DEPS: modbasics binom
int main(){
    precompute();
    int N = 300;
    vector<vi> p(N + 1, vi(N + 1, 0));
    rep(i, 0, N + 1) {
        p[i][0] = 1;
        rep(j, 1, i + 1)
            p[i][j] = (p[i-1][j-1] + (j <= i-1 ? p[i-1][j] : 0)) % MOD;
    }
    rep(i, 0, N + 1) rep(j, 0, N + 1)
        if (C(i, j) != (j <= i ? p[i][j] : 0)) {
            printf("MISMATCH C(%lld,%lld)\n", i, j); return 1;
        }
    if (C(-1, 0) || C(5, 7) || C(MAXN, 1)) {
        puts("MISMATCH out-of-range guard"); return 1;
    }
    puts("OK C(n,k) == Pascal for n,k <= 300; out-of-range returns 0");
}
