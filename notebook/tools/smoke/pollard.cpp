// DEPS: pollard
int main(){
    // primality against a sieve
    int N = 200000;
    vector<char> comp(N + 1, 0);
    for (int i = 2; (ll)i * i <= N; i++) if (!comp[i])
        for (int j = i*i; j <= N; j += i) comp[j] = 1;
    for (int i = 2; i <= N; i++)
        if (isPrime(i) == (bool)comp[i]) {
            printf("MISMATCH isPrime(%d)\n", i); return 1;
        }
    // factorisation of random values, and of hard semiprimes
    mt19937_64 rng(4021);
    vector<ll> hard = {1000000007LL*1000000009LL, 999999999989LL,
                       (ll)4e18 + 37, 2LL*3*5*7*11*13*17*19*23*29*31*37};
    rep(iter, 0, 300) hard.push_back((ll)(rng() % (ull)4e18) + 2);
    for (ll n : hard) {
        vi f; factor(n, f);
        i128 prod = 1;
        for (ll p : f) {
            if (!isPrime(p)) { printf("MISMATCH non-prime factor\n"); return 1; }
            prod *= p;
        }
        if (prod != (i128)n) { printf("MISMATCH product != n\n"); return 1; }
    }
    puts("OK isPrime vs sieve to 2e5; 304 factorisations multiply back (incl. 4e18)");
}
