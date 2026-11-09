// ==================== KMP (Knuth-Morris-Pratt) ====================
// O(n+m). failure[i] = length of longest proper prefix of pattern[0..i] that's also a suffix.
#include <bits/stdc++.h>
using namespace std;

vector<int> computeFailure(const string& pattern) {
    int m = pattern.size();
    vector<int> fail(m, 0);
    int k = 0;
    for (int i = 1; i < m; i++) {
        while (k > 0 && pattern[i] != pattern[k]) k = fail[k-1];
        if (pattern[i] == pattern[k]) k++;
        fail[i] = k;
    }
    return fail;
}

// returns all starting indices (0-indexed) where pattern occurs in text
vector<int> kmpSearch(const string& text, const string& pattern) {
    vector<int> result;
    if (pattern.empty()) return result;
    vector<int> fail = computeFailure(pattern);
    int n = text.size(), m = pattern.size();
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k > 0 && text[i] != pattern[k]) k = fail[k-1];
        if (text[i] == pattern[k]) k++;
        if (k == m) {
            result.push_back(i - m + 1);
            k = fail[k-1];
        }
    }
    return result;
}
