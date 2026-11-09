// ==================== Trie (prefix tree) ====================
// Supports insert, search exact word, and prefix count.
#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct Node {
        int child[26];
        int cnt = 0;      // number of words passing through / ending here
        bool end = false;
        Node() { fill(child, child + 26, -1); }
    };
    vector<Node> nodes;
    Trie() { nodes.emplace_back(); }

    void insert(const string& s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].child[idx] == -1) {
                nodes[cur].child[idx] = nodes.size();
                nodes.emplace_back();
            }
            cur = nodes[cur].child[idx];
            nodes[cur].cnt++;
        }
        nodes[cur].end = true;
    }
    bool search(const string& s) {
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].child[idx] == -1) return false;
            cur = nodes[cur].child[idx];
        }
        return nodes[cur].end;
    }
    int countPrefix(const string& s) { // number of inserted words with this prefix
        int cur = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (nodes[cur].child[idx] == -1) return 0;
            cur = nodes[cur].child[idx];
        }
        return nodes[cur].cnt;
    }
};

// 01-Trie for XOR queries (e.g. max XOR pair), fixed bit width
struct XorTrie {
    static const int BITS = 30;
    struct Node { int child[2] = {-1, -1}; };
    vector<Node> nodes;
    XorTrie() { nodes.emplace_back(); }
    void insert(int x) {
        int cur = 0;
        for (int b = BITS; b >= 0; b--) {
            int bit = (x >> b) & 1;
            if (nodes[cur].child[bit] == -1) {
                nodes[cur].child[bit] = nodes.size();
                nodes.emplace_back();
            }
            cur = nodes[cur].child[bit];
        }
    }
    int maxXor(int x) { // max value of x ^ (something inserted)
        int cur = 0, res = 0;
        for (int b = BITS; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;
            if (nodes[cur].child[want] != -1) {
                res |= (1 << b);
                cur = nodes[cur].child[want];
            } else {
                cur = nodes[cur].child[bit];
            }
        }
        return res;
    }
};
