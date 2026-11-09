// DEPS: modbasics crt
int main(){
    mt19937 rng(271828);
    rep(iter, 0, 4000) {
        ll m1 = 1 + rng() % 30, m2 = 1 + rng() % 30;
        ll r1 = rng() % m1, r2 = rng() % m2;
        auto [r, l] = crt(r1, m1, r2, m2);
        ll want = -1, L = m1 / __gcd(m1, m2) * m2;
        rep(x, 0, L) if (x % m1 == r1 && x % m2 == r2) { want = x; break; }
        if (want < 0) {
            if (r != -1) { puts("MISMATCH claimed a solution"); return 1; }
        } else {
            if (r != want || l != L) {
                printf("MISMATCH r1=%lld m1=%lld r2=%lld m2=%lld got=%lld want=%lld\n",
                       (long long)r1,(long long)m1,(long long)r2,(long long)m2,
                       (long long)r,(long long)want);
                return 1;
            }
        }
    }
    puts("OK 4000 cases vs exhaustive search (non-coprime moduli included)");
}
