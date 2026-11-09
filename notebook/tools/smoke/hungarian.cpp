// DEPS: hungarian MCMF
int main(){
    mt19937 rng(12345);
    rep(iter, 0, 400) {
        int n = 1 + rng() % 6, m = n + rng() % 3;
        matrix a(n, vi(m));
        rep(i, 0, n) rep(j, 0, m) a[i][j] = (ll)(rng() % 201) - 100;
        auto [cost, asg] = hungarian(a);
        vector<int> cols(m); iota(all(cols), 0);
        ll best = INF;
        do {
            ll c = 0;
            rep(i, 0, n) c += a[i][cols[i]];
            best = min(best, c);
        } while (next_permutation(all(cols)));
        set<int> seen; ll re = 0;
        rep(i, 0, n) { re += a[i][asg[i]]; seen.insert(asg[i]); }
        // MCMF on the same assignment: negative costs -> SPFA path
        MCMF mc(n + m + 2); int S = n + m, T = S + 1;
        rep(i, 0, n) mc.add(S, (int)i, 1, 0);
        rep(j, 0, m) mc.add(n + (int)j, T, 1, 0);
        rep(i, 0, n) rep(j, 0, m) mc.add((int)i, n + (int)j, 1, a[i][j]);
        auto [fl, mcost] = mc.run(S, T);
        if (cost != best || re != cost || (int)seen.size() != n
            || fl != n || mcost != best) {
            printf("MISMATCH n=%d m=%d got=%lld mcmf=%lld brute=%lld\n",
                   n, m, (long long)cost, (long long)mcost,
                   (long long)best);
            return 1;
        }
    }
    puts("OK 400 cases: hungarian == MCMF == brute-force permutations (rectangular, negative costs)");
}
