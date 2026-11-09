// DEPS: WDSU
int main(){
    mt19937 rng(8191);
    rep(iter, 0, 500) {
        int n = 2 + rng() % 8;
        vi truth(n);
        rep(i, 0, n) truth[i] = (ll)(rng() % 41) - 20;
        WDSU w(n);
        rep(q, 0, 40) {
            int a = rng() % n, b = rng() % n;
            bool honest = rng() % 4;
            ll claim = honest ? truth[a] - truth[b]
                              : truth[a] - truth[b] + 1 + rng() % 3;
            bool known = w.same(a, b);
            ll before = known ? w.diff(a, b) : 0;
            bool ok = w.join(a, b, claim);
            if (known && ok != (before == claim)) {
                puts("MISMATCH contradiction not caught"); return 1;
            }
            if (!known && !ok) {
                puts("MISMATCH fresh join refused"); return 1;
            }
        }
        rep(a, 0, n) rep(b, 0, n) if (w.same(a, b))
            if (w.diff(a, b) != w.pot(a) - w.pot(b)) {
                puts("MISMATCH diff != pot difference"); return 1;
            }
    }
    puts("OK 500 cases: contradictions detected, differences consistent");
}
