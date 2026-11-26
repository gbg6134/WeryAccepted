// DEPS: kmp zfunc manacher hash sufarr aho
static bool pal(const string& s, int l, int r) {
    for (int i = l, j = r - 1; i < j; i++, j--)
        if (s[i] != s[j]) return false;
    return true;
}
int main(){
    mt19937 rng(20260930);
    rep(iter, 0, 600) {
        int n = 1 + rng() % 14;
        int alpha = 1 + rng() % 3;
        string s;
        rep(i, 0, n) s += char('a' + rng() % alpha);

        // KMP match vs brute force, for every pattern length
        rep(m, 1, min<int>(n, 5) + 1) {
            string p;
            rep(i, 0, m) p += char('a' + rng() % alpha);
            vi got = match(s, p), want;
            rep(i, 0, n - m + 1)
                if (s.compare(i, m, p) == 0) want.push_back(i);
            if (got != want) { puts("MISMATCH kmp"); return 1; }
        }
        // pi vs brute force; shortest period property
        {
            vi p = pi(s);
            rep(i, 0, n) {
                int best = 0;
                rep(k, 1, i + 1)
                    if (s.compare(0, k, s, i - k + 1, k) == 0) best = k;
                if (p[i] != best) { puts("MISMATCH pi"); return 1; }
            }
            int per = n - p.back();
            if (n % per == 0)
                rep(i, 0, n) if (s[i] != s[i % per]) {
                    puts("MISMATCH period"); return 1;
                }
        }
        // Z function vs brute force
        {
            vi z = Z(s);
            if (z[0] != 0) { puts("MISMATCH z0"); return 1; }
            rep(i, 1, n) {
                int k = 0;
                while (i + k < n && s[k] == s[i+k]) k++;
                if (z[i] != k) { puts("MISMATCH zfunc"); return 1; }
            }
        }
        // Manacher: the isPal rule from the entry text, and the count
        {
            auto p = manacher(s);
            ll cnt = 0, want = 0;
            rep(i, 0, n) cnt += p[1][i] + 1 + p[0][i];
            rep(l, 0, n) rep(r, l + 1, n + 1) {
                int len = (int)(r - l), c = (int)l + len / 2;
                bool got = p[len % 2][c] >= len / 2;
                bool ok = pal(s, (int)l, (int)r);
                want += ok;
                if (got != ok) { puts("MISMATCH manacher isPal"); return 1; }
            }
            if (cnt != want) { puts("MISMATCH manacher count"); return 1; }
        }
        // hashing: equal hash iff equal substring
        {
            Hash h(s);
            if (iter < 120) rep(l1, 0, n) rep(r1, l1 + 1, n + 1)
              rep(l2, 0, n) rep(r2, l2 + 1, n + 1) {
                bool same = (r1 - l1 == r2 - l2)
                    && s.compare(l1, r1-l1, s, l2, r2-l2) == 0;
                bool hEq = h.get((int)l1,(int)r1) == h.get((int)l2,(int)r2);
                if (same != hEq) { puts("MISMATCH hash"); return 1; }
              }
        }
        // suffix array vs sorted suffixes (sa[0] = n), lcp vs brute force
        {
            SuffixArray sa(s);
            vi idx(n + 1); iota(all(idx), 0);
            sort(all(idx), [&](ll a, ll b){
                return s.compare(a, string::npos, s, b, string::npos) < 0; });
            if (sa.sa != idx) { puts("MISMATCH suffix array"); return 1; }
            if (sa.lcp[0] != 0) { puts("MISMATCH lcp0"); return 1; }
            rep(i, 1, n + 1) {
                int a = sa.sa[i], b = sa.sa[i-1], k = 0;
                while (a+k < n && b+k < n && s[a+k] == s[b+k]) k++;
                if (sa.lcp[i] != k) { puts("MISMATCH lcp"); return 1; }
            }
        }
        // Aho-Corasick findAll / find vs running match per pattern
        {
            int k = 1 + rng() % 4;
            vector<string> ps;
            rep(j, 0, k) {
                int m = 1 + rng() % 3;
                string p;
                rep(i, 0, m) p += char('a' + rng() % alpha);
                ps.push_back(p);
            }
            AhoCorasick ac(ps);
            set<pair<ll,ll>> got, want;      // (start index, pattern id)
            vector<vi> all_ = ac.findAll(ps, s);
            rep(i, 0, n) for (ll id : all_[i]) got.insert({i, id});
            rep(j, 0, k) for (ll st : match(s, ps[j])) want.insert({st, j});
            if (got != want) { puts("MISMATCH aho findAll"); return 1; }
            vi f = ac.find(s);
            rep(i, 0, n) {          // longest pattern ending at i
                ll best = -1, bl = 0;
                rep(j, 0, k) {
                    ll m = ps[j].size();
                    if (i + 1 >= m && s.compare(i + 1 - m, m, ps[j]) == 0
                        && m > bl) best = j, bl = m;
                }
                bool ok = best == -1 ? f[i] == -1
                        : (f[i] >= 0 && (ll)ps[f[i]].size() == bl);
                if (!ok) { puts("MISMATCH aho find"); return 1; }
            }
        }
    }
    puts("OK 600 strings: kmp, pi, zfunc, manacher, hash, suffix array+lcp, aho");
}
