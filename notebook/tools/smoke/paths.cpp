// DEPS: dijkstra
int main(){
    mt19937 rng(31337);
    rep(iter, 0, 300) {
        int n = 1 + rng() % 7;
        matrix d(n, vi(n, INF));
        rep(i, 0, n) d[i][i] = 0;
        vector<vector<pii>> g(n);
        int m = rng() % 15;
        rep(e, 0, m) {
            int u = rng() % n, v = rng() % n;
            ll w = rng() % 10;
            g[u].push_back({v, w});
            d[u][v] = min(d[u][v], w);
        }
        rep(k, 0, n) rep(i, 0, n) if (d[i][k] < INF)
            rep(j, 0, n) if (d[k][j] < INF)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        rep(s, 0, n) {
            vector<int> par;
            vi got = dijkstra(g, (int)s, &par);
            rep(t, 0, n) {
                if (got[t] != d[s][t]) {
                    puts("MISMATCH dijkstra vs floyd"); return 1;
                }
                if (t != s && got[t] < INF) {
                    ll v = t, steps = 0;
                    while (v != s && steps <= n) { v = par[v]; steps++; }
                    if (v != s) { puts("MISMATCH parent chain"); return 1; }
                }
            }
        }
    }
    puts("OK 300 cases: dijkstra == floyd, parent chains reach the source");
}
