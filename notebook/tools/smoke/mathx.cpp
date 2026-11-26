// DEPS: modbasics modLog modSqrt floorsum gaussMod gaussXor xorbasis det bm fst interp simplex quad
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
    // floor sums
    rep(it, 0, 3000) {
        ull to = rng() % 50, c = rng() % 100, k = rng() % 100, m = 1 + rng() % 30;
        ull d = 0; ll ms = 0;
        ll cc = (ll)(rng() % 200) - 100, kk = (ll)(rng() % 200) - 100;
        for (ull i = 0; i < to; i++) { d += (k * i + c) / m; ms += (((kk * (ll)i + cc) % (ll)m) + m) % m; }
        if (divsum(to, c, k, m) != d || modsum(to, cc, kk, m) != ms) { puts("floorsum fail"); return 1; }
    }
    // gaussMod: compare solvability/count with brute force over F_p^m
    rep(it, 0, 500) {
        ll p = vector<ll>{2, 3, 5}[rng() % 3];
        int n = 1 + rng() % 3, m = 1 + rng() % 3;
        vector<vi> a(n, vi(m)); vi b(n);
        rep(i, 0, n) { rep(j, 0, m) a[i][j] = rng() % p; b[i] = rng() % p; }
        int cnt = 0; ll tot = 1; rep(j, 0, m) tot *= p;
        rep(code, 0, tot) {
            vi x(m); ll c = code; rep(j, 0, m) x[j] = c % p, c /= p;
            bool ok = true;
            rep(i, 0, n) { ll s = 0; rep(j, 0, m) s += a[i][j] * x[j]; if (s % p != b[i]) ok = false; }
            cnt += ok;
        }
        vi x; int r = gaussMod(a, b, x, p);
        if (r < 0) { if (cnt) { puts("gaussMod says none"); return 1; } continue; }
        ll expect = 1; rep(j, 0, m - r) expect *= p;
        bool ok = true;
        rep(i, 0, n) { ll s = 0; rep(j, 0, m) s += a[i][j] * x[j]; if (s % p != b[i]) ok = false; }
        if (!ok || cnt != expect) { puts("gaussMod wrong"); return 1; }
    }
    // gaussXor
    rep(it, 0, 500) {
        int n = 1 + rng() % 5, m = 1 + rng() % 5;
        vector<bs> A(n); vector<int> b(n);
        rep(i, 0, n) { rep(j, 0, m) A[i][j] = rng() % 2; b[i] = rng() % 2; }
        auto A0 = A; auto b0 = b;
        int cnt = 0;
        rep(x, 0, 1 << m) { bool ok = true; rep(i, 0, n) { int s = 0; rep(j, 0, m) s ^= A0[i][j] & (x >> j & 1); if (s != b0[i]) ok = false; } cnt += ok; }
        bs x; int r = gaussXor(A, b, x, m);
        if (r < 0) { if (cnt) { puts("gaussXor none"); return 1; } continue; }
        bool ok = true; rep(i, 0, n) { int s = 0; rep(j, 0, m) s ^= A0[i][j] & x[j]; if (s != b0[i]) ok = false; }
        if (!ok || cnt != (1 << (m - r))) { puts("gaussXor wrong"); return 1; }
    }
    // xor basis
    rep(it, 0, 300) {
        int n = rng() % 8; XorBasis B; set<ll> reach{0}; vector<ll> v;
        rep(i, 0, n) {
            ll x = rng() % 64; bool was = reach.count(x);
            if (B.add(x) == was) { puts("xorbasis add"); return 1; }
            set<ll> nr = reach; for (ll y : reach) nr.insert(y ^ x); reach = nr;
        }
        rep(x, 0, 64) if (B.can(x) != (bool)reach.count(x)) { puts("xorbasis can"); return 1; }
        ll q = rng() % 64, mx = 0; for (ll y : reach) mx = max(mx, q ^ y);
        if (B.maxXor(q) != mx) { puts("xorbasis max"); return 1; }
        vector<ll> sorted(all(reach));
        rep(k, 0, (ll)sorted.size() + 2) {
            ll want = k < (ll)sorted.size() ? sorted[k] : -1;
            if (B.kth(k) != want) { puts("xorbasis kth"); return 1; }
        }
    }
    // det vs permutation expansion
    rep(it, 0, 300) {
        int n = 1 + rng() % 5; ll md = 7 + rng() % 3 * 2;   // 7, 9, 11 (9 not prime)
        vector<vi> a(n, vi(n)); vector<vector<ld>> f(n, vector<ld>(n));
        rep(i, 0, n) rep(j, 0, n) { a[i][j] = rng() % md; f[i][j] = a[i][j]; }
        vector<int> pm(n); iota(all(pm), 0); ll s = 0;
        do {
            ll t = 1; rep(i, 0, n) t *= a[i][pm[i]];
            int inv = 0; rep(i, 0, n) rep(j, i + 1, n) inv += pm[i] > pm[j];
            s += inv % 2 ? -t : t;
        } while (next_permutation(all(pm)));
        if (detMod(a, md) != ((s % md) + md) % md) { puts("detMod"); return 1; }
        if (fabsl(det(f) - s) > 1e-6 * max((ld)1, fabsl((ld)s))) { puts("det"); return 1; }
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
    // interpolation
    rep(it, 0, 200) {
        int n = 1 + rng() % 7; vi co(n); rep(i, 0, n) co[i] = rng() % MD;
        auto ev = [&](ll x) { ll r = 0; down(i, n, 0) r = (r * (x % MD) + co[i]) % MD; return r; };
        vi y(n); rep(i, 0, n) y[i] = ev(i);
        ll X = rng() % 1000000000000LL;
        if (lagrange(y, X, MD) != ev(X)) { puts("lagrange"); return 1; }
        vector<ld> xs(n), ys(n); vector<ld> cf(n); rep(i, 0, n) cf[i] = (ld)(rng() % 7) - 3;
        rep(i, 0, n) { xs[i] = i * 0.7L - 1; ld r = 0; down(j, n, 0) r = r * xs[i] + cf[j]; ys[i] = r; }
        auto got = interpolate(xs, ys);
        rep(i, 0, n) if (fabsl(got[i] - cf[i]) > 1e-6) { puts("interpolate"); return 1; }
    }
    // simplex: KACTL example, plus random 2-variable LPs vs vertex enumeration
    {
        vector<LPSolver::vd> A = {{1, -1}, {-1, 1}, {-1, -2}};
        LPSolver::vd b = {1, 1, -4}, c = {-1, -1}, x;
        double v = LPSolver(A, b, c).solve(x);
        if (fabs(v - (-7.0 / 3)) > 1e-6)   // x=2/3, y=5/3
        { printf("simplex example %f\n", v); return 1; }
    }
    rep(it, 0, 500) {
        int m = 1 + rng() % 4;
        vector<LPSolver::vd> A(m, LPSolver::vd(2)); LPSolver::vd b(m), c(2), x;
        rep(i, 0, m) { A[i][0] = (int)(rng() % 7) - 2; A[i][1] = (int)(rng() % 7) - 2; b[i] = (int)(rng() % 10) - 2; }
        c[0] = (int)(rng() % 7) - 3; c[1] = (int)(rng() % 7) - 3;
        rep(i, 0, m) A.push_back({0, 0});  // keep sizes simple
        A.resize(m);
        // add box x,y <= 20 so it is bounded
        A.push_back({1, 0}); b.push_back(20); A.push_back({0, 1}); b.push_back(20);
        double v = LPSolver(A, b, c).solve(x);
        // brute: grid search on fine lattice of rationals is unreliable; check vertices of all line pairs
        vector<array<double,3>> L;
        rep(i, 0, (ll)A.size()) L.push_back({A[i][0], A[i][1], b[i]});
        L.push_back({-1, 0, 0}); L.push_back({0, -1, 0});
        double best = -1e18; bool feas = false;
        auto okp = [&](double px, double py) { for (auto& l : L) if (l[0]*px + l[1]*py > l[2] + 1e-7) return false; return true; };
        rep(i, 0, (ll)L.size()) rep(j, i + 1, (ll)L.size()) {
            double d = L[i][0]*L[j][1] - L[i][1]*L[j][0];
            if (fabs(d) < 1e-12) continue;
            double px = (L[i][2]*L[j][1] - L[i][1]*L[j][2]) / d, py = (L[i][0]*L[j][2] - L[i][2]*L[j][0]) / d;
            if (okp(px, py)) { feas = true; best = max(best, c[0]*px + c[1]*py); }
        }
        if (!feas) { if (v != -LPSolver::inf) { puts("simplex infeasible"); return 1; } continue; }
        if (fabs(v - best) > 1e-6 || !okp(x[0], x[1])) { printf("simplex %f %f\n", v, best); return 1; }
    }
    // quad
    {
        ld pi = acosl(-1);
        ld a1 = quad(0, pi, [](ld x) { return sinl(x); });
        ld a2 = quad(-1, 1, [](ld x) { return sqrtl(max((ld)0, 1 - x * x)); });
        if (fabsl(a1 - 2) > 1e-6 || fabsl(a2 - pi / 2) > 1e-5) { puts("quad"); return 1; }
    }
    puts("OK modLog 3000, modSqrt 3000, floor sums 3000, gaussMod 500 + gaussXor 500 (solution counts), XorBasis 300, det/detMod 300, BM+linearRec 200, AND/OR/XOR conv 600, lagrange/interpolate 200, simplex 501, quad");
}
