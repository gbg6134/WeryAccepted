// ==================== Sparse Table (RMQ, idempotent ops) ====================
// O(n log n) build, O(1) query. Only for idempotent functions (min/max/gcd/and/or).
#include <bits/stdc++.h>
using namespace std;

struct SparseTable {
    vector<vector<int>> table;
    vector<int> logt;
    SparseTable(vector<int>& a) {
        int n = a.size();
        int LOG = 32 - __builtin_clz(max(n,1));
        table.assign(LOG, vector<int>(n));
        table[0] = a;
        logt.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) logt[i] = logt[i/2] + 1;
        for (int k = 1; k < LOG; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                table[k][i] = min(table[k-1][i], table[k-1][i + (1 << (k-1))]);
    }
    // min query on [l, r] inclusive
    int query(int l, int r) {
        int k = logt[r - l + 1];
        return min(table[k][l], table[k][r - (1 << k) + 1]);
    }
};
