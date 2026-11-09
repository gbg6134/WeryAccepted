// DEPS: point seg poly hull closest circle
// exact oracle for segment intersection, non-degenerate segments only
static bool oracle(Pl a, Pl b, Pl c, Pl d) {
    ll den = (b - a).cross(d - c);
    if (den != 0) {
        ll tn = (c - a).cross(d - c), sn = (c - a).cross(b - a);
        auto in01 = [](ll num, ll de) {
            if (de < 0) { num = -num; de = -de; }
            return num >= 0 && num <= de;
        };
        return in01(tn, den) && in01(sn, den);
    }
    if ((c - a).cross(b - a) != 0) return false;      // parallel, apart
    ll R = (b - a).dot(b - a);
    ll p1 = (c - a).dot(b - a), p2 = (d - a).dot(b - a);
    if (p1 > p2) swap(p1, p2);
    return max((ll)0, p1) <= min(R, p2);
}
int main(){
    mt19937 rng(161803);
    // --- segInter vs exact oracle, tiny coordinates so cases repeat often
    rep(iter, 0, 200000) {
        auto rp = [&]{ return Pl(rng() % 5, rng() % 5); };
        Pl a = rp(), b = rp(), c = rp(), d = rp();
        if (a == b || c == d) continue;
        if (segInter(a, b, c, d) != oracle(a, b, c, d)) {
            puts("MISMATCH segInter"); return 1;
        }
    }
    // --- hull: general position, compare vertex set with O(n^3) oracle
    rep(iter, 0, 2000) {
        int n = 1 + rng() % 9;
        vector<Pl> p;
        rep(i, 0, n) p.push_back(Pl(rng() % 30, rng() % 30));
        sort(all(p)); p.erase(unique(all(p)), p.end());
        n = (int)p.size();
        bool general = true;                  // reject 3 collinear
        rep(i, 0, n) rep(j, i+1, n) rep(k, j+1, n)
            if (ccw(p[i], p[j], p[k]) == 0) general = false;
        if (!general || n < 3) continue;
        vector<Pl> h = hull(p);
        set<pair<ll,ll>> want;
        rep(i, 0, n) rep(j, 0, n) if (i != j) {
            bool allLeft = true;
            rep(k, 0, n) if (k != i && k != j
                             && ccw(p[i], p[j], p[k]) <= 0) allLeft = false;
            if (allLeft) {
                want.insert({p[i].x, p[i].y});
                want.insert({p[j].x, p[j].y});
            }
        }
        set<pair<ll,ll>> got;
        for (Pl q : h) got.insert({q.x, q.y});
        if (got != want) { puts("MISMATCH hull vertex set"); return 1; }
        rep(i, 0, (ll)h.size()) {            // strictly convex, ccw
            Pl x = h[i], y = h[(i+1) % h.size()],
               z = h[(i+2) % h.size()];
            if (ccw(x, y, z) <= 0) { puts("MISMATCH hull not ccw"); return 1; }
        }
        if (area2(h) <= 0) { puts("MISMATCH hull area sign"); return 1; }
        // inConvex must agree with inPoly on the hull
        rep(qx, 0, 30) rep(qy, 0, 30) {
            Pl q(qx, qy);
            int u = inPoly(h, q), v = inConvex(h, q);
            if ((u != 0) != (v != 0) || (u == 2) != (v == 2)) {
                puts("MISMATCH inConvex vs inPoly"); return 1;
            }
        }
    }
    // --- area2 on a known shape, and orientation
    {
        vector<Pl> sq = {Pl(0,0), Pl(4,0), Pl(4,3), Pl(0,3)};
        if (area2(sq) != 24) { puts("MISMATCH area2 square"); return 1; }
        reverse(all(sq));
        if (area2(sq) != -24) { puts("MISMATCH area2 sign"); return 1; }
    }
    // --- closest pair vs O(n^2)
    rep(iter, 0, 2000) {
        int n = 2 + rng() % 40;
        vector<Pl> p;
        rep(i, 0, n) p.push_back(Pl(rng() % 200, rng() % 200));
        auto [best, pr] = closest(p);
        ll want = LLONG_MAX;
        rep(i, 0, n) rep(j, i+1, n)
            want = min(want, (p[i] - p[j]).dist2());
        if (best != want) {
            printf("MISMATCH closest got=%lld want=%lld\n",
                   (long long)best, (long long)want); return 1;
        }
        if ((pr.first - pr.second).dist2() != best) {
            puts("MISMATCH closest returned pair"); return 1;
        }
    }
    // --- circles: every returned point lies on both objects
    rep(iter, 0, 20000) {
        Pd c1((ld)(rng() % 21) - 10, (ld)(rng() % 21) - 10);
        Pd c2((ld)(rng() % 21) - 10, (ld)(rng() % 21) - 10);
        ld r1 = 1 + rng() % 10, r2 = 1 + rng() % 10;
        for (Pd q : circleCircle(c1, r1, c2, r2)) {
            if (fabsl((q - c1).dist() - r1) > 1e-6
                || fabsl((q - c2).dist() - r2) > 1e-6) {
                puts("MISMATCH circleCircle point"); return 1;
            }
        }
        Pd a((ld)(rng() % 21) - 10, (ld)(rng() % 21) - 10);
        Pd b((ld)(rng() % 21) - 10, (ld)(rng() % 21) - 10);
        if (!(a == b)) for (Pd q : circleLine(c1, r1, a, b)) {
            if (fabsl((q - c1).dist() - r1) > 1e-6
                || fabsl(segDist(q, a, b)) > 1e-6) {
                // q may be outside the segment; check the LINE instead
                if (fabsl((b - a).cross(q - a)) > 1e-6 * (b-a).dist()) {
                    puts("MISMATCH circleLine point"); return 1;
                }
            }
        }
    }
    puts("OK segInter vs exact oracle (200k), hull vs O(n^3) oracle, inConvex, area2, closest vs O(n^2), circles");
}
