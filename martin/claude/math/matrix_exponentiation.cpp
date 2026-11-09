// ==================== Matrix Exponentiation ====================
// O(k^3 log n) for k x k matrix to the n-th power. Great for linear recurrences (Fibonacci etc.)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1000000007;

typedef vector<vector<ll>> Matrix;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Matrix C(n, vector<ll>(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++) {
            if (A[i][j] == 0) continue;
            for (int l = 0; l < m; l++)
                C[i][l] = (C[i][l] + A[i][j] * B[j][l]) % MOD;
        }
    return C;
}

Matrix matpow(Matrix base, ll exp) {
    int n = base.size();
    Matrix result(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1; // identity
    while (exp > 0) {
        if (exp & 1) result = multiply(result, base);
        base = multiply(base, base);
        exp >>= 1;
    }
    return result;
}

// Example: nth Fibonacci number mod MOD using matrix [[1,1],[1,0]]^n
ll fib(ll n) {
    if (n == 0) return 0;
    Matrix base = {{1, 1}, {1, 0}};
    Matrix result = matpow(base, n - 1);
    return result[0][0];
}
