// ==================== Floyd-Warshall (all-pairs shortest path) ====================
// O(V^3). dist[i][i] should start at 0, dist[i][j] = weight if edge exists, else INF.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = LLONG_MAX / 2;

void floydWarshall(vector<vector<ll>>& dist, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
    // negative cycle exists iff dist[i][i] < 0 for some i
}
