// To use this:
// Call precompute() once in code to initialize  
// Then to recieve answer use C(total elements, elements to pick)

const ll MOD = 998244353, MAXN = 500001;
ll fact[MAXN], inv_fact[MAXN];

ll power(ll a, ll b, ll mod) {
    ll res = 1;
    for (; b > 0; b >>= 1, a = a * a % mod)
        if (b & 1) res = res * a % mod;
    return res;
}

void precompute() {
    fact[0] = 1;
    rep(i, 1, MAXN) fact[i] = fact[i-1] * i % MOD;
    inv_fact[MAXN - 1] = power(fact[MAXN - 1], MOD - 2, MOD);
    down(i, MAXN - 1, 0) inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
}

ll C(ll n, ll k) {
    if (n < 0 || k < 0 || k > n || n >= MAXN) return 0;
    return fact[n] % MOD * inv_fact[k] % MOD * inv_fact[n-k] % MOD;
}