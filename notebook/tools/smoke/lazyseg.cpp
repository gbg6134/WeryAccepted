// DEPS: LazySeg
int main(){
    mt19937 rng(5150);
    rep(iter, 0, 300) {
        int n = 1 + rng() % 40;
        vi a(n);
        rep(i, 0, n) a[i] = (ll)(rng() % 21) - 10;
        LazySeg s(a);
        rep(q, 0, 200) {
            int l = rng() % n, r = l + 1 + rng() % (n - l);
            if (rng() % 2) {
                ll f = (ll)(rng() % 11) - 5;
                s.update(l, r, f);
                for (int i = l; i < r; i++) a[i] += f;
            } else {
                ll want = 0;
                for (int i = l; i < r; i++) want += a[i];
                if (s.query(l, r) != want) {
                    puts("MISMATCH range sum"); return 1;
                }
                int i = rng() % n;
                if (s[i] != a[i]) { puts("MISMATCH point"); return 1; }
            }
        }
    }
    puts("OK 300 cases x 200 ops: range add + range sum vs naive array");
}
