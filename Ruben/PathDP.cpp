// ===========================================================================
// PathDP (bitmask)   O(2^n * n^2)   mem 2^n * n
// Folds every simple path covering exactly a vertex set into one value,
// n <= ~20. Directed; for undirected fill both directions. No recursion.
// The constructor does all the work, every call below is a lookup.
//
//   PathDP p(w);              // w[u][v] = weight, PathDP::INF = no edge
//   PathDP p(w, 1LL << s);    // 2nd arg = MASK of allowed start vertices,
//                             // -1 (default) = any. NOT a vertex index.
//   PathDP p(w, -1, true);    // 3rd arg = canon, see CYCLES below
//
//   p.val(mask, v)    // fold of the paths covering EXACTLY mask, ending at v
//   p.hamPath()       // fold over v of val(full, v)
//   p.hamPathTo(v)    // val(full, v): covers everything and ends at v
//   p.hamCycle(s)     // fold of val(full, v) extended by w[v][s]
//   p.endingAt(v)     // fold over every mask, ending at v
//   p.bySize(k)       // fold over the masks holding exactly k vertices
//   p.anyPath()       // fold over every mask and every v
//   p.cycles()        // fold over every simple cycle, needs canon
//   p.path(mask, v)   // vertex order behind val(mask, v); MIN / MAX only
//   p.full            // the all-vertices mask, p.dp / p.par are flat
//
//   PathDP::induced(g, vs)  // cut a piece out of a big graph, see below
//
// CONFIG -- edit the 5 lines below, like Seg.
//   T           what one path is worth, and what every call returns.
//   ext(x, wt)  a path worth x is extended by one edge of weight wt;
//               return what the longer path is worth.
//   merge(a,b)  two different paths reach the same (mask, v); return the one
//               value that stands for both. Associative AND commutative.
//   UNIT        what a one-vertex path is worth, before any edge.
//   NONE        "no path at all": merge(x, NONE) = x and ext(NONE, wt) = NONE.
//               Every call returns it when nothing exists.
//   ext must distribute over merge -- ext(merge(a,b),wt) =
//   merge(ext(a,wt),ext(b,wt)) -- or the fold is wrong.
//
//   want                T       UNIT  NONE   ext(x,wt)    merge(a,b)
//   shortest path       ll      0     INF    x + wt       min(a, b)
//   longest path        ll      0     -INF   x + wt       max(a, b)
//   count paths         ll      1     0      x            a + b
//   count mod p         ll      1     0      x            (a + b) % p
//   min bottleneck      ll      0     INF    max(x, wt)   min(a, b)
//   max probability     double  1     0      x * wt       max(a, b)
//   reachable lengths   bitset  1<<0  0      x << wt      a | b
// Counting ignores wt, so w only has to say which edges exist. On an
// undirected graph every path is found in both directions, so counts from
// hamPath / hamCycle / cycles come out 2x -- halve them.
// If T is a struct or a bitset, constexpr will not compile: write
//   inline static const T UNIT = ...;   inline static const T NONE = ...;
// path() needs merge to SELECT one of its arguments, so it is for MIN / MAX
// only; under a summing merge par holds the last contributor, not a path.
// Unmodded counts overflow ll fast -- n = 18 dense already does. Use mod p.
//
// STARTS -- the 2nd argument is a bitmask, not a vertex. 1LL << s pins the
// start to s (what hamCycle(s) needs), -1 lets any vertex start, and any
// other mask restricts the start set, e.g. only sources of a condensation.
//
// CYCLES -- pass canon = true and every path is forced to begin at the
// LOWEST vertex of its own mask, so each vertex set is walked from one
// canonical start instead of every start. cycles() then closes each path
// back to that lowest vertex and folds, skipping masks below 3 vertices.
// With canon the paths are no longer "all paths from anywhere", so only
// cycles() is meaningful; leave canon false for everything else.
//
// INDUCED -- runs the dp on a subset of a big graph. g is the global
// adjacency list (a LIST), vs the global ids you keep, in any order. Back
// comes the k x k weight MATRIX of just those vertices, which is the form
// the constructor wants, renumbered so that local i is vs[i]:
//
//   vi vs = {9, 2, 5};
//   PathDP p(PathDP::induced(g, vs));
//   //  local  0  1  2   ==   global  9  2  5
//
// From there EVERYTHING is local: n is k, bit i of a mask is local i, and
// val / path / hamPath all speak local ids. vs is the way back, vs[i]
// being the global id of local i:
//
//   for (ll x : p.path(bm, bv)) cout << vs[x] << ' ';   // global order
//
// Keep vs in a variable: SCC::members() rebuilds the whole grouping on
// every call, so take it once with auto mem = s.members(), then pass
// mem[c] and translate with mem[c][x]. The global->local scratch is reused
// between calls and wiped in O(k), so running it once per component of a
// big graph costs O(n + m) in total, not O(components * n).
//
// An edge survives only if BOTH ends are in vs; direction is kept, every
// survivor gets weight 1, parallel edges collapse into one. g and vs hold
// any integer type, so SCC::members()[c] and 2ecc vertex lists go in as is.
// vs must hold DISTINCT ids, each < g.size(): a repeat silently becomes two
// vertices that a path may visit both of. A self-loop lands on w[i][i] and
// is never read by the dp, so it is harmless.
//
// GOTCHAS
//   val(mask, v) is EXACTLY mask, not a subset; v must be inside mask.
//   anyPath() and endingAt() also fold in the one-vertex paths -- under a
//   counting config that is +1 each, under MAX it floors them at UNIT.
//   hamCycle(s) means nothing unless the table was built with 1LL << s.
//   induced() hardcodes weight 1; for real weights build w yourself, and
//   remember its output is local while your problem is global.
//   A different start mask changes the whole table, so sweeping starts
//   rebuilds it: k of them cost k * 2^n * n^2.
//
// MEM -- dp is one flat 2^n * n array of T, par the same count of char.
// The inner loop walks nb[u] & ~m, the out-edges not yet used, so it never
// tests a non-edge. Measured at -O2, T = ll, 30% density: n=16 0.01 s
// 13 MB, n=18 0.05 s 44 MB, n=20 0.22 s 184 MB.
// ===========================================================================
struct PathDP {
    using T = ll;
    static T ext(T x, ll wt) { return x + wt; }
    static T merge(T a, T b) { return min(a, b); }
    static constexpr T UNIT = 0;
    static constexpr T NONE = 1e18;
    static constexpr ll INF = 1e18;      // missing edge in w

    int n; ll starts, full;
    matrix w; vector<T> dp; vector<char> par;

    PathDP(const matrix& wt, ll ss = -1, bool canon = false)
            : n(wt.size()), starts(ss), full((1LL << n) - 1),
              w(wt) {
        dp.assign((full + 1) * n, NONE);
        par.assign((full + 1) * n, -1);
        rep(v, 0, n) if (starts >> v & 1)
            dp[(1LL << v) * n + v] = UNIT;
        vector<ll> nb(n, 0);        // out-edges of u as bits
        rep(u, 0, n) rep(v, 0, n)
            if (w[u][v] < INF) nb[u] |= 1LL << v;
        rep(m, 1, full + 1) {
            ll base = m * n;
            for (ll b = m; b; b &= b - 1) {   // set bits of m
                int u = __builtin_ffsll(b) - 1;
                T cur = dp[base + u];
                if (cur == NONE) continue;
                for (ll c = nb[u] & ~m; c; c &= c - 1) {
                    int v = __builtin_ffsll(c) - 1;
                    if (canon && (1LL << v) < (m & -m))
                        continue;
                    ll t = (m | 1LL << v) * n + v;
                    T nv = merge(dp[t], ext(cur, w[u][v]));
                    if (nv != dp[t]) {
                        dp[t] = nv; par[t] = (char)u;
                    }
                }
            }
        }
    }
    T val(ll m, int v) { return dp[m * n + v]; }
    T hamPathTo(int v) { return val(full, v); }
    T hamPath() {
        T r = NONE;
        rep(v, 0, n) r = merge(r, val(full, v));
        return r;
    }
    T hamCycle(int s) {          // build with 1LL << s
        T r = NONE;
        rep(v, 0, n) {
            T d = val(full, v);
            if (d != NONE && w[v][s] < INF)
                r = merge(r, ext(d, w[v][s]));
        }
        return r;
    }
    T endingAt(int v) {
        T r = NONE;
        rep(m, 1, full + 1) r = merge(r, val(m, v));
        return r;
    }
    T bySize(int k) {            // exactly k vertices
        T r = NONE;
        rep(m, 1, full + 1) if (__builtin_popcountll(m) == k)
            rep(v, 0, n) r = merge(r, val(m, v));
        return r;
    }
    T anyPath() {
        T r = NONE;
        rep(m, 1, full + 1) rep(v, 0, n)
            r = merge(r, val(m, v));
        return r;
    }
    T cycles() {                 // build with canon
        T r = NONE;
        rep(m, 1, full + 1) {
            if (__builtin_popcountll(m) < 3) continue;
            int s = 0;
            while (!(m >> s & 1)) s++;   // lowest of m
            rep(v, 0, n) {
                T d = val(m, v);
                if (v != s && d != NONE && w[v][s] < INF)
                    r = merge(r, ext(d, w[v][s]));
            }
        }
        return r;
    }
    vi path(ll m, int v) {               // MIN / MAX only
        vi p;
        while (v >= 0) {
            p.push_back(v);
            int u = par[m * n + v]; m ^= 1LL << v; v = u;
        }
        reverse(p.begin(), p.end());
        return p;
    }
    template<class G, class V>    // g, vs: any integer type
    static matrix induced(const G& g, const V& vs) {
        static vi idx;        // reused, all -1 on entry
        if (idx.size() < g.size()) idx.assign(g.size(), -1);
        int k = (int)vs.size();
        rep(i, 0, k) idx[vs[i]] = i;
        matrix w(k, vi(k, INF));
        rep(i, 0, k) for (ll u : g[vs[i]])
            if (idx[u] >= 0) w[i][idx[u]] = 1;
        rep(i, 0, k) idx[vs[i]] = -1;   // wipe what we set
        return w;
    }
};