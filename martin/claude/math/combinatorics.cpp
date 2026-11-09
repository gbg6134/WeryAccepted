// ==================== Combinatorics (factorials, nCr mod p, Catalan) ====================
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const ll MOD = 1000000007;
const int MAXN = 1000006;

ll fact[MAXN], invFact[MAXN];

ll modpow(ll base, ll exp, ll mod) {
    base %= mod; ll result = 1;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

void precomputeFactorials() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = fact[i-1] * i % MOD;
    invFact[MAXN-1] = modpow(fact[MAXN-1], MOD - 2, MOD);
    for (int i = MAXN - 2; i >= 0; i--) invFact[i] = invFact[i+1] * (i+1) % MOD;
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n-r] % MOD;
}
ll nPr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[n-r] % MOD;
}
// nth Catalan number mod MOD: C(2n, n) / (n+1)
ll catalan(int n) {
    return nCr(2*n, n) * modpow(n + 1, MOD - 2, MOD) % MOD;
}

// Pascal's triangle (no mod inverse needed, good for small n or non-prime mod)
vector<vector<ll>> pascalTriangle(int n, ll mod = MOD) {
    vector<vector<ll>> C(n+1, vector<ll>(n+1, 0));
    for (int i = 0; i <= n; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = (C[i-1][j-1] + (j <= i-1 ? C[i-1][j] : 0)) % mod;
    }
    return C;
}
