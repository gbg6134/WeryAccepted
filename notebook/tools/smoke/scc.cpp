// DEPS: SCC
int main(){
    mt19937 rng(606);
    rep(iter, 0, 400) {
        int n = 1 + rng() % 8;
        vector<vector<int>> g(n);
        vector<vector<char>> r(n, vector<char>(n, 0));
        rep(i, 0, n) r[i][i] = 1;
        int m = rng() % 18;
        rep(e, 0, m) {
            int u = rng() % n, v = rng() % n;
            g[u].push_back(v); r[u][v] = 1;
        }
        rep(k, 0, n) rep(i, 0, n) rep(j, 0, n)
            if (r[i][k] && r[k][j]) r[i][j] = 1;
        SCC s(g);
        rep(a, 0, n) rep(b, 0, n) {
            bool mutual = r[a][b] && r[b][a];
            if (mutual != (s.comp[a] == s.comp[b])) {
                puts("MISMATCH comp != mutual reachability"); return 1;
            }
        }
        rep(u, 0, n) for (int v : g[u])
            if (s.comp[u] > s.comp[v]) {
                puts("MISMATCH comp not topological"); return 1;
            }
    }
    puts("OK 400 cases: comps == mutual reachability, order is topological");
}
