// DEPS: hungarian
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
        if (cost != best || re != cost || (int)seen.size() != n) {
            printf("MISMATCH n=%d m=%d got=%lld brute=%lld\n",
                   n, m, (long long)cost, (long long)best);
            return 1;
        }
    }
    puts("OK 400 cases vs brute-force permutations (rectangular, negative costs)");
}
