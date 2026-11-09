// DEPS: Matching Dinic
int main(){
    mt19937 rng(4242);
    rep(iter, 0, 400) {
        int a = 1 + rng() % 6, b = 1 + rng() % 6;
        Matching mm(a, b);
        Dinic din(a + b + 2);
        int S = a + b, T = S + 1;
        vector<pii> es;
        rep(i, 0, a) din.add(S, (int)i, 1);
        rep(j, 0, b) din.add(a + (int)j, T, 1);
        rep(i, 0, a) rep(j, 0, b) if (rng() % 3 == 0) {
            mm.add((int)i, (int)j); es.push_back({i, j});
            din.add((int)i, a + (int)j, 1);
        }
        int k = mm.run();
        ll f = din.maxflow(S, T);
        if (k != f) {
            printf("MISMATCH hopcroft-karp=%d dinic=%lld\n", k, (long long)f);
            return 1;
        }
        rep(i, 0, a) if (mm.ml[i] != -1 && mm.mr[mm.ml[i]] != (int)i) {
            puts("MISMATCH ml/mr inconsistent"); return 1;
        }
        auto [L, R] = mm.cover();
        if ((int)(L.size() + R.size()) != k) { puts("BAD cover size"); return 1; }
        vector<char> inL(a, 0), inR(b, 0);
        for (int x : L) inL[x] = 1;
        for (int x : R) inR[x] = 1;
        for (auto [i, j] : es) if (!inL[i] && !inR[j]) {
            puts("BAD cover misses an edge"); return 1;
        }
    }
    puts("OK 400 cases: Hopcroft-Karp == unit-capacity Dinic, ml/mr consistent, Konig cover valid");
}
