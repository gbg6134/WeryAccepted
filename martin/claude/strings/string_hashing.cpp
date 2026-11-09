// ==================== Polynomial String Hashing (double hash to avoid collisions) ====================
// O(n) precompute, O(1) substring hash query.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

struct StringHash {
    int n;
    vector<ll> h1, h2, p1, p2;
    const ll MOD1 = 1000000007, MOD2 = 998244353;
    const ll BASE1 = 131, BASE2 = 137;

    StringHash(const string& s) {
        n = s.size();
        h1.assign(n+1, 0); h2.assign(n+1, 0);
        p1.assign(n+1, 1); p2.assign(n+1, 1);
        for (int i = 0; i < n; i++) {
            h1[i+1] = (h1[i] * BASE1 + s[i]) % MOD1;
            h2[i+1] = (h2[i] * BASE2 + s[i]) % MOD2;
            p1[i+1] = (p1[i] * BASE1) % MOD1;
            p2[i+1] = (p2[i] * BASE2) % MOD2;
        }
    }
    // hash of s[l..r] inclusive, 0-indexed
    pair<ll,ll> getHash(int l, int r) {
        ll r1 = ((h1[r+1] - h1[l] * p1[r-l+1]) % MOD1 + MOD1) % MOD1;
        ll r2 = ((h2[r+1] - h2[l] * p2[r-l+1]) % MOD2 + MOD2) % MOD2;
        return {r1, r2};
    }
};
// Compare getHash(l1,r1) == getHash(l2,r2) for O(1) substring equality checks.
