// Modular multiplicative inverse via extended Euclidean algorithm.
// Returns x in [0, mod) such that (number * x) % mod == 1.
// Requires gcd(number, mod) == 1; returns -1 if no inverse exists.
// Runs in O(log(min(number, mod))).

ll ext_gcd(ll a, ll b, ll &x, ll &y){
    if(b == 0){ x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll inv(ll a, ll m){
    ll x, y;
    ll g = ext_gcd(a, m, x, y);
    if(g != 1) return -1;            // inverse doesn't exist
    return ((x % m) + m) % m;        // normalize to [0, m)
}