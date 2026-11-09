// DEPS: eulerPath
int main(){
    mt19937 rng(77);
    rep(iter, 0, 3000) {
        int n = 2 + rng() % 5, m = 1 + rng() % 7;
        bool dir = rng() % 2;
        vector<pii> es;
        rep(e, 0, m) es.push_back({(ll)(rng() % n), (ll)(rng() % n)});
        auto p = eulerPath(n, es, dir);
        vi od(n, 0), id(n, 0);
        vector<int> par(n); iota(all(par), 0);
        function<int(int)> f = [&](int x){
            return par[x] == x ? x : par[x] = f(par[x]); };
        for (auto& e : es) {
            od[e.first]++; id[e.second]++;
            par[f((int)e.first)] = f((int)e.second);
        }
        int roots = 0;
        rep(v, 0, n) if ((od[v] || id[v]) && f((int)v) == (int)v) roots++;
        bool okDeg;
        if (dir) {
            int plus = 0, minus = 0, bad = 0;
            rep(v, 0, n) { ll d = od[v] - id[v];
                if (d == 1) plus++; else if (d == -1) minus++;
                else if (d) bad++; }
            okDeg = !bad && plus <= 1 && minus <= 1;
        } else {
            int odd = 0;
            rep(v, 0, n) if ((od[v] + id[v]) % 2) odd++;
            okDeg = (odd == 0 || odd == 2);
        }
        bool should = okDeg && roots == 1;
        if (p.empty() == should) {
            printf("MISMATCH dir=%d oracle=%d empty=%d\n",
                   (int)dir, (int)should, (int)p.empty());
            return 1;
        }
        if (p.empty()) continue;
        if ((int)p.size() != m + 1) { puts("MISMATCH length"); return 1; }
        map<pii,int> avail;
        for (auto& e : es) {
            avail[e]++;
            if (!dir && e.first != e.second)
                avail[{e.second, e.first}]++;
        }
        rep(i, 0, (ll)p.size() - 1) {
            pii k = {p[i], p[i+1]};
            if (!avail[k]) { puts("MISMATCH edge reused"); return 1; }
            avail[k]--;
            if (!dir) {
                pii r = {p[i+1], p[i]};
                if (r != k && avail[r]) avail[r]--;
            }
        }
    }
    puts("OK 3000 cases: existence matches degree+connectivity, each edge used once");
}
