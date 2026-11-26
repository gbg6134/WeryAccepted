// DEPS: point seg poly hull lineproj mec tangents circlePoly polyCut hullDiam DSU kruskal manhattan
int main(){
    mt19937 rng(3);
    auto rd = [&](int lo, int hi) { return lo + (ll)(rng() % (hi - lo + 1)); };
    // lineProj / reflection
    rep(it, 0, 1000) {
        Pd a(rd(-9, 9), rd(-9, 9)), b(rd(-9, 9), rd(-9, 9)), p(rd(-9, 9), rd(-9, 9));
        if (a == b) continue;
        Pd q = lineProj(a, b, p), r = lineProj(a, b, p, true);
        if (fabsl((b - a).cross(q - a)) > 1e-9 || fabsl((p - q).dot(b - a)) > 1e-9) { puts("lineProj"); return 1; }
        if (((p + r) / 2 - q).dist() > 1e-9) { puts("reflect"); return 1; }
    }
    // angle sort vs atan2 (distinct directions)
    rep(it, 0, 300) {
        vector<Pl> v; rep(i, 0, 20) { Pl p(rd(-50, 50), rd(-50, 50)); if (!(p == Pl(0, 0))) v.push_back(p); }
        sort(all(v), angleLess);
        auto ang = [](Pl p) { ld a = atan2l((ld)p.y, (ld)p.x); return a < 0 ? a + 2 * acosl(-1) : a; };
        rep(i, 1, (ll)v.size()) if (ang(v[i]) < ang(v[i - 1]) - 1e-12) { puts("angleLess"); return 1; }
    }
    // mec: contains all points, and no point set has a smaller circle through 2/3 boundary points
    rep(it, 0, 200) {
        int n = 1 + rng() % 12; vector<Pd> ps; rep(i, 0, n) ps.push_back(Pd(rd(-20, 20), rd(-20, 20)));
        auto [o, r] = mec(ps);
        for (auto p : ps) if ((p - o).dist() > r + 1e-7) { puts("mec not enclosing"); return 1; }
        ld best = 1e18;
        auto test = [&](Pd c, ld rr) { for (auto p : ps) if ((p - c).dist() > rr + 1e-7) return; best = min(best, rr); };
        if (n == 1) best = 0;
        rep(i, 0, n) rep(j, i + 1, n) {
            test((ps[i] + ps[j]) / 2, (ps[i] - ps[j]).dist() / 2);
            rep(k, j + 1, n) if (fabsl((ps[j] - ps[i]).cross(ps[k] - ps[i])) > 1e-9) {
                Pd c = ccCenter(ps[i], ps[j], ps[k]);
                if (fabsl(ccRadius(ps[i], ps[j], ps[k]) - (c - ps[i]).dist()) > 1e-7) { puts("ccRadius"); return 1; }
                test(c, (c - ps[i]).dist());
            }
        }
        if (n > 1 && all_of(all(ps), [&](Pd p){ return p == ps[0]; })) best = 0;
        if (fabsl(best - r) > 1e-6) { printf("mec %Lf %Lf\n", r, best); return 1; }
    }
    // tangents: touching points at right distance and perpendicular
    rep(it, 0, 500) {
        Pd c1(rd(-9, 9), rd(-9, 9)), c2(rd(-9, 9), rd(-9, 9)); ld r1 = rd(1, 5), r2 = rd(0, 5);
        if (rng() % 2) r2 = -r2;
        for (auto [p, q] : tangents(c1, r1, c2, r2)) {
            if (fabsl((p - c1).dist() - r1) > 1e-7 || fabsl((q - c2).dist() - fabsl(r2)) > 1e-7) { puts("tangent dist"); return 1; }
            Pd d = q - p;
            if (d.dist() > 1e-9 && (fabsl(d.dot(p - c1)) > 1e-6 || fabsl(d.dot(q - c2)) > 1e-6)) { puts("tangent perp"); return 1; }
        }
    }
    // circlePoly vs Monte Carlo-free checks: huge circle = polygon area, tiny circle inside convex poly = pi r^2
    rep(it, 0, 200) {
        vector<Pl> pts; rep(i, 0, 10) pts.push_back(Pl(rd(-10, 10), rd(-10, 10)));
        auto h = hull(pts); if (h.size() < 3) continue;
        vector<Pd> hd; for (auto p : h) hd.push_back(Pd(p.x, p.y));
        ld A = area2(h) / 2.0L;
        if (fabsl(circlePoly(Pd(0, 0), 1000, hd) - A) > 1e-6) { puts("circlePoly big"); return 1; }
        // grid integration estimate for a medium circle
        Pd c(rd(-5, 5), rd(-5, 5)); ld r = rd(1, 8);
        ld est = 0; int G = 400; ld step = 40.0L / G;
        rep(i, 0, G) rep(j, 0, G) {
            Pd q(-20 + (i + 0.5L) * step, -20 + (j + 0.5L) * step);
            if ((q - c).dist() > r) continue;
            bool in = true; rep(k, 0, (ll)hd.size()) if (hd[k].cross(hd[(k + 1) % hd.size()], q) < 0) in = false;
            if (in) est += step * step;
        }
        if (fabsl(circlePoly(c, r, hd) - est) > 0.5) { printf("circlePoly %Lf %Lf\n", circlePoly(c, r, hd), est); return 1; }
    }
    // polygonCut: area of right part + left part = whole; centroid of a square
    rep(it, 0, 300) {
        vector<Pl> pts; rep(i, 0, 10) pts.push_back(Pl(rd(-10, 10), rd(-10, 10)));
        auto h = hull(pts); if (h.size() < 3) continue;
        vector<Pd> hd; for (auto p : h) hd.push_back(Pd(p.x, p.y));
        Pd s(rd(-10, 10), rd(-10, 10)), e(rd(-10, 10), rd(-10, 10)); if (s == e) continue;
        auto ar = [](const vector<Pd>& p) { ld a = 0; rep(i, 0, (ll)p.size()) a += p[i].cross(p[(i + 1) % p.size()]); return a / 2; };
        auto R = polygonCut(hd, s, e), L = polygonCut(hd, e, s);
        if (fabsl(ar(R) + ar(L) - ar(hd)) > 1e-6) { puts("polygonCut"); return 1; }
        for (auto p : R) if (s.cross(e, p) > 1e-9) { puts("polygonCut side"); return 1; }
    }
    {
        Pd c = polygonCenter({Pd(0, 0), Pd(2, 0), Pd(2, 2), Pd(0, 2)});
        Pd t = polygonCenter({Pd(0, 0), Pd(3, 0), Pd(0, 3)});
        if ((c - Pd(1, 1)).dist() > 1e-9 || (t - Pd(1, 1)).dist() > 1e-9) { puts("polygonCenter"); return 1; }
    }
    // hullDiameter vs brute
    rep(it, 0, 500) {
        vector<Pl> pts; int n = 1 + rng() % 15; rep(i, 0, n) pts.push_back(Pl(rd(-30, 30), rd(-30, 30)));
        auto h = hull(pts);
        ll best = 0; for (auto a : pts) for (auto b : pts) best = max(best, (a - b).dist2());
        auto d = hullDiameter(h);
        if ((d[0] - d[1]).dist2() != best) { puts("hullDiameter"); return 1; }
    }
    // manhattan MST vs O(n^2) Kruskal
    rep(it, 0, 300) {
        int n = 1 + rng() % 30; vector<Pl> ps; rep(i, 0, n) ps.push_back(Pl(rd(-20, 20), rd(-20, 20)));
        vector<array<ll,3>> all_e;
        rep(i, 0, n) rep(j, i + 1, n) all_e.push_back({llabs(ps[i].x - ps[j].x) + llabs(ps[i].y - ps[j].y), i, j});
        ll want = kruskal(n, all_e).first, got = kruskal(n, manhattanMST(ps)).first;
        if (want != got) { printf("manhattanMST %lld %lld\n", got, want); return 1; }
    }
    puts("OK lineProj 1000, angleLess 300, mec 200 vs brute, tangents 500, circlePoly 200, polygonCut 300, hullDiameter 500, manhattanMST 300");
}
