// DEPS: modbasics modLog modSqrt bm fst
int main(){
    mt19937 rng(5);
    // modLog vs brute
    rep(it, 0, 3000) {
        ll m = 1 + rng() % 200, a = rng() % m, b = rng() % m;
        ll want = -1, e = 1;
        rep(x, 1, 3 * m + 5) { e = e * a % m; if (e == b % m) { want = x; break; } }
        if (modLog(a, b, m) != want) { printf("modLog %lld %lld %lld got %lld want %lld\n", a, b, m, modLog(a, b, m), want); return 1; }
    }
    // modSqrt
    for (ll p : {2LL, 3LL, 5LL, 7LL, 13LL, 17LL, 41LL, 97LL, 998244353LL, 1000000007LL}) rep(it, 0, 300) {
        ll x = rng() % p, a = x * x % p;
        ll r = modSqrt(a, p);
        if (r * r % p != a) { printf("modSqrt fail p=%lld\n", p); return 1; }
    }
    // Berlekamp-Massey + linearRec
    const ll MD = 998244353;
    rep(it, 0, 200) {
        int d = 1 + rng() % 6; vi c(d), s(d);
        rep(i, 0, d) c[i] = rng() % MD, s[i] = rng() % MD;
        vi seq = s;
        rep(i, d, 60) { ll x = 0; rep(j, 0, d) x = (x + c[j] * seq[i - 1 - j]) % MD; seq.push_back(x); }
        vi first(seq.begin(), seq.begin() + 2 * d + 4);
        vi tr = berlekampMassey(first, MD);
        rep(k, 0, 60) if (linearRec(first, tr, k, MD) != seq[k]) { puts("BM/linearRec"); return 1; }
    }
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
    puts("OK modLog, modSqrt, BM+linearRec, AND/OR/XOR conv");
}
