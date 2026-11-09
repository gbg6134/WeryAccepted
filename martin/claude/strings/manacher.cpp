// ==================== Manacher's Algorithm ====================
// Finds all palindromic substrings in O(n). Returns radius arrays for odd & even lengths.
// d1[i] = number of odd-length palindromes centered at i (radius including center).
// d2[i] = number of even-length palindromes centered between i-1 and i.
#include <bits/stdc++.h>
using namespace std;

pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    vector<int> d1(n), d2(n);
    // odd length palindromes
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i-k] == s[i+k]) k++;
        d1[i] = k--;
        if (i + k > r) { l = i - k; r = i + k; }
    }
    // even length palindromes
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i-k-1] == s[i+k]) k++;
        d2[i] = k--;
        if (i + k > r) { l = i - k - 1; r = i + k; }
    }
    return {d1, d2};
}
// Longest palindromic substring length:
int longestPalindrome(const string& s) {
    auto [d1, d2] = manacher(s);
    int best = 0;
    for (int x : d1) best = max(best, 2*x - 1);
    for (int x : d2) best = max(best, 2*x);
    return best;
}
