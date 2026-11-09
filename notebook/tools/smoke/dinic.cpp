// DEPS: Dinic MCMF
int main(){
    mt19937 rng(999);
    rep(iter, 0, 400) {
        int n = 2 + rng() % 7;
        Dinic din(n); MCMF mc(n);
        vector<array<int,3>> es;
        int m = rng() % 14;
        rep(e, 0, m) {
            int u = rng() % n, v = rng() % n;
            if (u == v) continue;
            int c = 1 + rng() % 9;
            din.addEdge(u, v, c); mc.addEdge(u, v, c, 1);
            es.push_back({u, v, c});
        }
        int s = 0, t = n - 1;
        ll f1 = din.maxflow(s, t);
        ll f2 = mc.run(s, t).first;
        auto c = din.cut();
        ll cutcap = 0;
        for (auto& x : es) if (c[x[0]] && !c[x[1]]) cutcap += x[2];
        if (f1 != f2 || cutcap != f1 || !c[s] || c[t]) {
            printf("MISMATCH dinic=%lld mcmf=%lld cut=%lld\n",
                   (long long)f1, (long long)f2, (long long)cutcap);
            return 1;
        }
    }
    puts("OK 400 cases: dinic flow == mcmf flow == min-cut capacity");
}
