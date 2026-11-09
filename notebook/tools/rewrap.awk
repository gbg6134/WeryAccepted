# Exact full-line rewrites to bring every code line inside the 62-character
# column (A4 landscape, 3 columns, 8pt). Purely reformatting -- no semantics
# change except `closest` and `hungarian`, whose loop bodies are restructured
# identically; both are covered by tools/smoke.
#
# Usage: awk -f tools/rewrap.awk FILE > FILE.new
# Keys are compared as exact strings, so no regex escaping is involved.

BEGIN {
    DEL = "\001DELETE"

    # --- src/02-range.tex
    R["    int kth(ll k) {          // min i with sum(i+1) >= k, else n"] \
        = "    int kth(ll k) {     // min i with sum(i+1) >= k, else n"
    R["    Seg(const vector<T>& a) : n((int)a.size()), t(2*a.size()) {"] \
        = "    Seg(const vector<T>& a)\n            : n((int)a.size()), t(2*a.size()) {"
    R["                t[k][i] = op(t[k-1][i], t[k-1][i + (1 << (k-1))]);"] \
        = "                t[k][i] = op(t[k-1][i],\n                             t[k-1][i + (1 << (k-1))]);"
    R["        else { split(t->r, k - sz(t->l) - 1, t->r, b); a = t; }"] \
        = "        else { split(t->r, k-sz(t->l)-1, t->r, b); a = t; }"

    # --- src/03-dsu.tex
    R["    bool join(int a, int b) {        // false if already joined"] \
        = "    bool join(int a, int b) {   // false if already joined"

    # --- src/07-flow.tex  (hungarian: else-branch pulled onto the brace line)
    R["                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }"] \
        = "                if (used[j]) {\n                    u[p[j]] += delta; v[j] -= delta;\n                } else minv[j] -= delta;"
    R["                else minv[j] -= delta;"] = DEL

    # --- src/09-math.tex
    R["        auto f = [&](ll v) { return (mulm(v, v, n) + c) % n; };"] \
        = "        auto f = [&](ll v){ return (mulm(v,v,n)+c) % n; };"
    R["            if (fabsl(a[i][col]) > fabsl(a[sel][col])) sel = i;"] \
        = "            if (fabsl(a[i][col]) > fabsl(a[sel][col]))\n                sel = i;"
    R["            for (int j = col; j < m; j++) a[i][j] -= c*a[rank][j];"] \
        = "            for (int j = col; j < m; j++)\n                a[i][j] -= c * a[rank][j];"

    # --- src/10-strings.tex
    R["            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);"] \
        = "            int k = i > r ? 1 : min(d1[l+r-i], r-i+1);"
    R["            while (i - k >= 0 && i + k < n && s[i-k] == s[i+k])"] \
        = "            while (i-k >= 0 && i+k < n && s[i-k] == s[i+k])"
    R["            int k = (i > r) ? 0"] \
        = "            int k = i > r ? 0 : min(d2[l+r-i+1], r-i+1);"
    R["                            : min(d2[l + r - i + 1], r - i + 1);"] = DEL
    R["        return len % 2 ? 2 * d1[c] - 1 >= len : 2 * d2[c] >= len;"] \
        = "        return len % 2 ? 2*d1[c]-1 >= len : 2*d2[c] >= len;"
    R["                    || c[(a + d) % n] != c[(b + d) % n]) cls++;"] \
        = "                    || c[(a+d) % n] != c[(b+d) % n]) cls++;"
    R["    void add(const string& s, int id) {   // s must be nonempty"] \
        = "    void add(const string& s, int id) {  // nonempty s"
    R["    template<class F>                    // f(end index, pattern id)"] \
        = "    template<class F>       // f(end index, pattern id)"
    R["            if (ch[u][c] >= 0) { r |= 1LL << b; u = ch[u][c]; }"] \
        = "            if (ch[u][c] >= 0) { r |= 1LL<<b; u = ch[u][c]; }"

    # --- src/11-geometry.tex
    R["    P unit()   const { return *this / (T)dist(); }   // Pd only"] \
        = "    P unit() const { return *this / (T)dist(); } // Pd only"
    R["    P rotate(ld a) const {                           // Pd only"] \
        = "    P rotate(ld a) const {             // Pd only"
    R["    bool operator< (P p) const { return tie(x,y) <  tie(p.x,p.y); }"] \
        = "    bool operator<(P p) const {\n        return tie(x, y) < tie(p.x, p.y);\n    }"
    R["    bool operator==(P p) const { return tie(x,y) == tie(p.x,p.y); }"] \
        = "    bool operator==(P p) const {\n        return tie(x, y) == tie(p.x, p.y);\n    }"
    # closest pair: hoist dx out of the while condition
    R["        while (j < i"] \
        = "        while (j < i) {\n            ll dx = p[i].x - p[j].x;\n            if (dx * dx <= best) break;"
    R["               && (p[i].x - p[j].x) * (p[i].x - p[j].x) > best) {"] = DEL
}

{
    if ($0 in R) {
        if (R[$0] != DEL) print R[$0]
        hits++
    } else print
}

END { if (hits) printf "  %s: %d lines rewritten\n", FILENAME, hits > "/dev/stderr" }
