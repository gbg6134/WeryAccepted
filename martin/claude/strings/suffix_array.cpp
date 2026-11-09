// ==================== Suffix Array (O(n log n)) + LCP array (Kasai) ====================
#include <bits/stdc++.h>
using namespace std;

// returns suffix array: sa[i] = starting index of the i-th smallest suffix
vector<int> buildSuffixArray(const string& s) {
    int n = s.size();
    vector<int> sa(n), rnk(n), tmp(n);
    for (int i = 0; i < n; i++) { sa[i] = i; rnk[i] = s[i]; }

    for (int k = 1; k < n; k <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rnk[a] != rnk[b]) return rnk[a] < rnk[b];
            int ra = (a + k < n) ? rnk[a + k] : -1;
            int rb = (b + k < n) ? rnk[b + k] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
        rnk = tmp;
        if (rnk[sa[n-1]] == n - 1) break;
    }
    return sa;
}

// Kasai's algorithm: lcp[i] = longest common prefix of sa[i] and sa[i+1], size n-1
vector<int> buildLCP(const string& s, vector<int>& sa) {
    int n = s.size();
    vector<int> rnk(n), lcp(n > 0 ? n - 1 : 0);
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rnk[i] > 0) {
            int j = sa[rnk[i] - 1];
            while (i + h < n && j + h < n && s[i+h] == s[j+h]) h++;
            lcp[rnk[i] - 1] = h;
            if (h > 0) h--;
        } else h = 0;
    }
    return lcp;
}
// Note: O(n log^2 n) due to sort comparator; fine for n up to ~1e5-2e5 in contest time limits.
// For n up to 1e6, use radix-sort based O(n log n) construction if TLE.
