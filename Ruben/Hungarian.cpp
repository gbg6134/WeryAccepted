// ===========================================================================
// Hungarian (assignment problem)   O(n^2 m)   mem n * m
// n rows (workers), m >= n columns (jobs), a[i][j] = cost of giving job j
// to worker i. Every worker gets exactly one job, no job twice; minimises
// the total. The constructor does all the work.
//
//   Hungarian h(a);             // a = n x m cost matrix, n <= m
//   h.cost                      // minimum total cost
//   h.job[i]                    // column given to row i
//
// CONFIG
//   C     cost type. double works.
//
// MAX total: pass -a, negate cost. n > m: transpose (jobs choose workers).
// Forbidden pair: cost BIG with n * BIG far below INF, and BIG larger than
// any real total; if h.cost >= BIG the assignment had to use one.
// Rows allowed to stay idle: append one 0-cost dummy column per such row.
//
// MEASURED at -O2: n = m = 500 0.02 s, n = m = 1000 0.08 s.
// ===========================================================================
struct Hungarian {
    using C = ll;
    static constexpr C INF = numeric_limits<C>::max() / 4;
    C cost; vector<int> job;

    template<class M>                      // vector<vector<C>>
    Hungarian(const M& a) {
        int n = a.size(), m = n ? a[0].size() : 0;
        vector<C> u(n + 1), v(m + 1);      // potentials, 1-idx
        vector<int> p(m + 1), way(m + 1);  // p[j]: row of col j
        for (int i = 1; i <= n; i++) {
            p[0] = i; int j0 = 0;
            vector<C> mn(m + 1, INF);
            vector<char> used(m + 1, 0);
            do {
                used[j0] = 1; int i0 = p[j0], j1 = 0; C dl = INF;
                for (int j = 1; j <= m; j++) if (!used[j]) {
                    C cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < mn[j]) mn[j] = cur, way[j] = j0;
                    if (mn[j] < dl) dl = mn[j], j1 = j;
                }
                for (int j = 0; j <= m; j++)
                    if (used[j]) u[p[j]] += dl, v[j] -= dl;
                    else mn[j] -= dl;
                j0 = j1;
            } while (p[j0]);
            do {
                int j1 = way[j0]; p[j0] = p[j1]; j0 = j1;
            } while (j0);
        }
        job.assign(n, -1); cost = 0;
        for (int j = 1; j <= m; j++)
            if (p[j]) job[p[j] - 1] = j - 1;
        for (int i = 0; i < n; i++) cost += a[i][job[i]];
    }
};