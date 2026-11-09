// ==================== Number Theory Toolkit ====================
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// ---- gcd/lcm (std::gcd/lcm available in <numeric> since C++17) ----
ll gcd_(ll a, ll b) { while (b) { a %= b; swap(a, b); } return a; }
ll lcm_(ll a, ll b) { return a / gcd_(a, b) * b; }

// ---- Extended Euclidean: finds x, y such that a*x + b*y = gcd(a,b) ----
ll extGcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// ---- Modular exponentiation ----
ll modpow(ll base, ll exp, ll mod) {
    base %= mod; if (base < 0) base += mod;
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// ---- Modular inverse (mod must be prime for Fermat; else use extGcd) ----
ll modInverse(ll a, ll mod) { return modpow(a, mod - 2, mod); } // prime mod
ll modInverseExtGcd(ll a, ll mod) { // works for any mod with gcd(a,mod)=1
    ll x, y;
    extGcd(a, mod, x, y);
    return ((x % mod) + mod) % mod;
}

// ---- Sieve of Eratosthenes ----
vector<int> sieve(int n) { // returns list of primes up to n
    vector<bool> isComposite(n + 1, false);
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (!isComposite[i]) {
            primes.push_back(i);
            for (ll j = (ll)i * i; j <= n; j += i) isComposite[j] = true;
        }
    }
    return primes;
}

// ---- Smallest prime factor sieve (fast factorization) ----
vector<int> smallestPrimeFactor(int n) {
    vector<int> spf(n + 1);
    iota(spf.begin(), spf.end(), 0);
    for (int i = 2; (ll)i * i <= n; i++)
        if (spf[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
    return spf;
}
vector<pair<int,int>> factorizeWithSpf(int x, vector<int>& spf) {
    vector<pair<int,int>> factors;
    while (x > 1) {
        int p = spf[x], cnt = 0;
        while (x % p == 0) { x /= p; cnt++; }
        factors.push_back({p, cnt});
    }
    return factors;
}

// ---- Trial division factorization (no sieve needed, O(sqrt(n))) ----
vector<pair<ll,int>> factorize(ll n) {
    vector<pair<ll,int>> factors;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) { n /= p; cnt++; }
            factors.push_back({p, cnt});
        }
    }
    if (n > 1) factors.push_back({n, 1});
    return factors;
}

// ---- Euler's totient function ----
ll phi(ll n) {
    ll result = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// ---- Chinese Remainder Theorem: x = r1 mod m1, x = r2 mod m2 ----
// returns {x, lcm(m1,m2)}; x == -1 if no solution exists
pair<ll,ll> crt(ll r1, ll m1, ll r2, ll m2) {
    ll x, y;
    ll g = extGcd(m1, m2, x, y);
    if ((r2 - r1) % g != 0) return {-1, -1};
    ll lcm = m1 / g * m2;
    ll mult = (r2 - r1) / g % (m2 / g);
    ll result = (r1 + m1 * ((x * mult % (m2/g) + m2/g) % (m2/g))) % lcm;
    if (result < 0) result += lcm;
    return {result, lcm};
}

// ---- Primality test (Miller-Rabin, deterministic for ll range) ----
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2,3,5,7,11,13,17,19,23,29,31,37}) {
        if (n % p == 0) return n == p;
    }
    ll d = n - 1; int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    for (ll a : {2,3,5,7,11,13,17,19,23,29,31,37}) {
        if (a >= n) continue;
        ll x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < r - 1; i++) {
            x = (lll)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}
