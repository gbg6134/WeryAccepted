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

        // KMP vs brute force, for every pattern length
        rep(m, 1, min<int>(n, 5) + 1) {
            string p;
            rep(i, 0, m) p += char('a' + rng() % alpha);
            vector<int> got = kmp(s, p), want;
            rep(i, 0, n - m + 1)
                if (s.compare(i, m, p) == 0) want.push_back((int)i);
            if (got != want) { puts("MISMATCH kmp"); return 1; }
        }
        // failure == shortest period property
        {
            vector<int> f = failure(s);
            int per = n - f.back();
            if (n % per == 0) {
                rep(i, 0, n) if (s[i] != s[i % per]) {
                    puts("MISMATCH period"); return 1;
                }
            }
        }
        // Z function vs brute force
        {
            vector<int> z = zfunc(s);
            rep(i, 1, n) {
                int k = 0;
                while (i + k < n && s[k] == s[i+k]) k++;
                if (z[i] != k) { puts("MISMATCH zfunc"); return 1; }
            }
        }
        // Manacher isPal vs brute force
        {
            Manacher mn(s);
            rep(l, 0, n) rep(r, l + 1, n + 1)
                if (mn.isPal((int)l, (int)r) != pal(s, (int)l, (int)r)) {
                    puts("MISMATCH manacher isPal"); return 1;
                }
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
        // suffix array vs sorted suffixes, lcp vs brute force
        {
            SuffixArray sa(s);
            vector<string> suf;
            rep(i, 0, n) suf.push_back(s.substr(i));
            vector<int> idx(n); iota(all(idx), 0);
            sort(all(idx), [&](int a, int b){
                return s.compare(a, string::npos, s, b, string::npos) < 0; });
            if (sa.sa != idx) { puts("MISMATCH suffix array"); return 1; }
            rep(i, 0, n - 1) {
                int a = sa.sa[i], b = sa.sa[i+1], k = 0;
                while (a+k < n && b+k < n && s[a+k] == s[b+k]) k++;
                if (sa.lcp[i] != k) { puts("MISMATCH lcp"); return 1; }
            }
            rep(i, 0, n) if (sa.rnk[sa.sa[i]] != (int)i) {
                puts("MISMATCH rnk"); return 1;
            }
        }
        // Aho-Corasick vs running kmp per pattern
        {
            int k = 1 + rng() % 4;
            vector<string> ps;
            Aho ah;
            rep(j, 0, k) {
                int m = 1 + rng() % 3;
                string p;
                rep(i, 0, m) p += char('a' + rng() % alpha);
                ps.push_back(p); ah.add(p, (int)j);
            }
            ah.build();
            set<pair<int,int>> got, want;   // (end index, pattern id)
            ah.match(s, [&](int i, int id){ got.insert({i, id}); });
            rep(j, 0, k) for (int st : kmp(s, ps[j]))
                want.insert({st + (int)ps[j].size() - 1, (int)j});
            if (got != want) { puts("MISMATCH aho"); return 1; }
        }
    }
    puts("OK 600 strings: kmp, period, zfunc, manacher, hash, suffix array+lcp, aho");
}
