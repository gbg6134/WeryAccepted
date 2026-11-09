// DEPS: Dinic MCMF
int main(){
    mt19937 rng(999);
    rep(iter, 0, 400) {
        int n = 2 + rng() % 7;
        Dinic din(n); MCMF mc(n);
        vector<array<int,3>> es; vector<int> ids;
        int m = rng() % 14;
        rep(e, 0, m) {
            int u = rng() % n, v = rng() % n;
            if (u == v) continue;
            int c = 1 + rng() % 9;
            ids.push_back(din.add(u, v, c)); mc.add(u, v, c, 1);
            es.push_back({u, v, c});
        }
        int s = 0, t = n - 1;
        ll f1 = din.maxflow(s, t);
        ll f2 = mc.run(s, t).first;
        ll cutcap = 0, cutids = 0;
        for (auto& x : es)
            if (din.inS(x[0]) && !din.inS(x[1])) cutcap += x[2];
        for (int id : din.cut()) cutids += din.e[id].c;
        vi bal(n, 0);                       // conservation
        rep(i, 0, (ll)es.size()) {
            ll f = din.flow(ids[i]);
            if (f < 0 || f > es[i][2]) { puts("BAD edge flow"); return 1; }
            bal[es[i][0]] -= f; bal[es[i][1]] += f;
        }
        rep(v, 0, n) if (v != s && v != t && bal[v]) {
            puts("BAD conservation"); return 1;
        }
        if (f1 != f2 || cutcap != f1 || cutids != f1 || bal[t] != f1
            || !din.inS(s) || din.inS(t)) {
            printf("MISMATCH dinic=%lld mcmf=%lld cut=%lld\n",
                   (long long)f1, (long long)f2, (long long)cutcap);
            return 1;
        }
        din.reset();
        if (din.maxflow(s, t) != f1) { puts("BAD reset"); return 1; }
    }
    puts("OK 400 cases: dinic flow == mcmf flow == min-cut capacity");
}
