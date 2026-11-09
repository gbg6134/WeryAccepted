// DEPS: Matching Dinic
int main(){
    mt19937 rng(4242);
    rep(iter, 0, 400) {
        int a = 1 + rng() % 6, b = 1 + rng() % 6;
        Matching mm(a, b);
        Dinic din(a + b + 2);
        int S = a + b, T = S + 1;
        rep(i, 0, a) din.addEdge(S, (int)i, 1);
        rep(j, 0, b) din.addEdge(a + (int)j, T, 1);
        rep(i, 0, a) rep(j, 0, b) if (rng() % 3 == 0) {
            mm.addEdge((int)i, (int)j);
            din.addEdge((int)i, a + (int)j, 1);
        }
        int k = mm.maxMatching();
        ll f = din.maxflow(S, T);
        if (k != f) {
            printf("MISMATCH kuhn=%d dinic=%lld\n", k, (long long)f);
            return 1;
        }
        rep(i, 0, a) if (mm.matchL[i] != -1
                         && mm.matchR[mm.matchL[i]] != (int)i) {
            puts("MISMATCH matchL/matchR inconsistent"); return 1;
        }
    }
    puts("OK 400 cases: Kuhn == unit-capacity Dinic, matchL/matchR consistent");
}
