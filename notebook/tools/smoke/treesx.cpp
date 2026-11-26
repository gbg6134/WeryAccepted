// DEPS: euler LCA centroidTree vtree
int main(){
    mt19937 rng(11);
    // centroid tree: every piece's child pieces are at most half its size, depth <= log
    rep(it, 0, 300) {
        int n = 1 + rng() % 60;
        vector<vector<int>> g(n);
        rep(i, 1, n) { int p = rng() % i; g[i].push_back(p); g[p].push_back(i); }
        auto cp = centroidTree(g);
        int roots = 0; rep(i, 0, n) roots += cp[i] == -1;
        vector<int> dep(n, -1);
        function<int(int)> D = [&](int v) { return dep[v] >= 0 ? dep[v] : dep[v] = (cp[v] < 0 ? 0 : 1 + D(cp[v])); };
        // piece of c = vertices whose centroid-ancestor chain contains c; must be connected in g and size <= half of parent piece
        vector<int> sz(n, 0);
        rep(v, 0, n) for (int c = v; c >= 0; c = cp[c]) sz[c]++;
        bool ok = roots == 1;
        rep(v, 0, n) {
            if (cp[v] >= 0 && 2 * sz[v] > sz[cp[v]]) ok = false;
            if ((1 << D(v)) > n) ok = false;
            // connectivity of piece v
            vector<int> in(n, 0); int cnt = 0;
            rep(u, 0, n) for (int c = u; c >= 0; c = cp[c]) if (c == v) { in[u] = 1; cnt++; break; }
            vector<int> st{(int)v}, seen(n, 0); seen[v] = 1; int r = 0;
            while (!st.empty()) { int u = st.back(); st.pop_back(); r++; for (int w : g[u]) if (in[w] && !seen[w]) seen[w] = 1, st.push_back(w); }
            if (r != cnt) ok = false;
        }
        if (!ok) { puts("centroidTree FAIL"); return 1; }
    }
    // virtual tree: parent relation equals nearest kept ancestor
    rep(it, 0, 300) {
        int n = 1 + rng() % 40;
        vector<vector<int>> g(n); vector<int> par(n, -1);
        rep(i, 1, n) { int p = rng() % i; par[i] = p; g[i].push_back(p); g[p].push_back(i); }
        vector<int> tin, tout; euler(g, 0, tin, tout);
        LCA L(g, 0);
        vector<int> sub; rep(v, 0, n) if (rng() % 3 == 0) sub.push_back(v);
        if (sub.empty()) sub.push_back(rng() % n);
        auto vt = virtualTree(L, tin, sub);
        set<int> keep; for (auto [p, v] : vt) keep.insert(v);
        for (int v : sub) if (!keep.count(v)) { puts("vtree missing vertex"); return 1; }
        rep(i, 1, (ll)vt.size()) {
            int v = vt[i].second, p = vt[vt[i].first].second;
            int a = par[v]; while (!keep.count(a)) a = par[a];
            if (a != p || vt[i].first >= i) { puts("vtree wrong parent"); return 1; }
        }
        if ((int)vt.size() > 2 * (int)sub.size()) { puts("vtree too big"); return 1; }
    }
    puts("OK centroidTree, virtualTree");
}
