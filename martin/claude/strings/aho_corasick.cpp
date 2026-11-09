// ==================== Aho-Corasick Automaton ====================
// Build once from a dictionary of patterns, then scan text in O(|text| + total matches).
#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick {
    struct Node {
        int child[26];
        int fail = 0;
        vector<int> patternIdx; // indices of patterns ending here
        Node() { fill(child, child + 26, -1); }
    };
    vector<Node> nodes;
    AhoCorasick() { nodes.emplace_back(); }

    void addPattern(const string& s, int idx) {
        int cur = 0;
        for (char c : s) {
            int b = c - 'a';
            if (nodes[cur].child[b] == -1) {
                nodes[cur].child[b] = nodes.size();
                nodes.emplace_back();
            }
            cur = nodes[cur].child[b];
        }
        nodes[cur].patternIdx.push_back(idx);
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < 26; c++) {
            if (nodes[0].child[c] == -1) nodes[0].child[c] = 0;
            else { nodes[nodes[0].child[c]].fail = 0; q.push(nodes[0].child[c]); }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int c = 0; c < 26; c++) {
                int v = nodes[u].child[c];
                if (v == -1) {
                    nodes[u].child[c] = nodes[nodes[u].fail].child[c];
                } else {
                    nodes[v].fail = nodes[nodes[u].fail].child[c];
                    q.push(v);
                }
            }
        }
    }

    // returns, for each position i in text, all pattern indices ending at i
    vector<vector<int>> search(const string& text) {
        vector<vector<int>> matches(text.size());
        int cur = 0;
        for (int i = 0; i < (int)text.size(); i++) {
            cur = nodes[cur].child[text[i] - 'a'];
            for (int node = cur; node != 0; node = nodes[node].fail) {
                for (int idx : nodes[node].patternIdx) matches[i].push_back(idx);
                if (nodes[node].fail == node) break; // safety for root self-loop edge case
            }
        }
        return matches;
    }
};
