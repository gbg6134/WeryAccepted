// An implicit treap is used for insert in O(logn) and 
// finding with at(index) in O(logn)

// Example usage:
// Treap<long double> t;
// t.insert(0, 3.5);          // insert at front
// t.insert(1, 7.2);          // insert at index 1
// long double v = t.at(0);   // access by index
// t.erase(0);                // delete at index

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<typename T>
struct Treap {
    struct Node {
        T val;
        T agg;          // aggregate over subtree (e.g. sum). Replace as needed.
        ll prio, sz;
        Node *l, *r;
        Node(T v) : val(v), agg(v), prio(rng()), sz(1), l(nullptr), r(nullptr) {}
    };
    Node* root = nullptr;

    static ll sz(Node* t) { return t ? t->sz : 0; }
    static T  ag(Node* t) { return t ? t->agg : T{}; }   // identity for monoid

    static void pull(Node* t) {
        if (!t) return;
        t->sz  = 1 + sz(t->l) + sz(t->r);
        t->agg = ag(t->l) + t->val + ag(t->r);            // change op for other aggregates
    }

    // split: first `k` elements go to `a`, rest to `b`
    static void split(Node* t, ll k, Node*& a, Node*& b) {
        if (!t) { a = b = nullptr; return; }
        if (sz(t->l) >= k) {
            split(t->l, k, a, t->l);
            b = t;
        } else {
            split(t->r, k - sz(t->l) - 1, t->r, b);
            a = t;
        }
        pull(t);
    }

    static Node* merge(Node* a, Node* b) {
        if (!a || !b) return a ? a : b;
        if (a->prio > b->prio) { a->r = merge(a->r, b); pull(a); return a; }
        else                   { b->l = merge(a, b->l); pull(b); return b; }
    }

    // insert val so it ends up at index k (0-indexed)
    void insert(ll k, T val) {
        Node *a, *b;
        split(root, k, a, b);
        root = merge(merge(a, new Node(val)), b);
    }

    // erase element at index k
    void erase(ll k) {
        Node *a, *b, *c;
        split(root, k, a, b);
        split(b, 1, b, c);
        delete b;
        root = merge(a, c);
    }

    // value at index k
    T at(ll k) {
        Node* t = root;
        while (t) {
            ll ls = sz(t->l);
            if (k == ls) return t->val;
            if (k <  ls) t = t->l;
            else { k -= ls + 1; t = t->r; }
        }
        assert(false);
    }

    // aggregate over [l, r)
    T query(ll l, ll r) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l, b, c);
        T res = ag(b);
        root = merge(a, merge(b, c));
        return res;
    }

    ll size() const { return sz(root); }
};