// DEPS: centroidTree
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
    puts("OK centroidTree 300: one root, pieces connected, each at most half its parent, depth <= log n");
}
