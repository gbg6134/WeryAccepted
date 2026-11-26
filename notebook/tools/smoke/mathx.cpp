// DEPS: fst
int main(){
    mt19937 rng(5);
    // FST vs brute
    rep(it, 0, 200) {
        int k = rng() % 6, n = 1 << k; vi a(n), b(n);
        rep(i, 0, n) a[i] = (ll)(rng() % 21) - 10, b[i] = (ll)(rng() % 21) - 10;
        rep(mode, 0, 3) {
            vi c = bitConv(a, b, mode), w(n, 0);
            rep(x, 0, n) rep(y, 0, n) { ll z = mode == 0 ? (x & y) : mode == 1 ? (x | y) : (x ^ y); w[z] += a[x] * b[y]; }
            if (c != w) { printf("FST mode %d\n", (int)mode); return 1; }
        }
    }
    puts("OK AND/OR/XOR convolution 600 vs brute force");
}
