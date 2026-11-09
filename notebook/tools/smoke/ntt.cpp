// DEPS: ntt
int main(){
    mt19937 rng(1618);
    rep(iter, 0, 300) {
        int n = 1 + rng() % 40, m = 1 + rng() % 40;
        vi a(n), b(m);
        rep(i, 0, n) a[i] = rng() % P;
        rep(i, 0, m) b[i] = rng() % P;
        vi got = conv(a, b);
        vi want(n + m - 1, 0);
        rep(i, 0, n) rep(j, 0, m)
            want[i+j] = (want[i+j] + a[i] * b[j]) % P;
        if (got != want) { printf("MISMATCH n=%d m=%d\n", n, m); return 1; }
    }
    // a big one, to be sure the power-of-two padding is right
    int N = 5000;
    vi a(N, 1), b(N, 1);
    vi c = conv(a, b);
    if ((int)c.size() != 2*N - 1 || c[N-1] != N) {
        puts("MISMATCH large case"); return 1;
    }
    puts("OK 300 random convolutions vs naive O(n^2), plus n=5000");
}
