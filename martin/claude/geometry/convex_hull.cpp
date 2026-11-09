// ==================== Convex Hull (Andrew's monotone chain) ====================
// O(n log n). Returns hull points in counter-clockwise order, no duplicate endpoint.
#include <bits/stdc++.h>
using namespace std;
typedef double ld;

struct Point {
    ld x, y;
    bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
};

ld cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convexHull(vector<Point> pts) {
    int n = pts.size(), k = 0;
    if (n <= 2) return pts;
    sort(pts.begin(), pts.end());
    vector<Point> hull(2 * n);

    // build lower hull
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k-2], hull[k-1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    // build upper hull
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(hull[k-2], hull[k-1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    hull.resize(k - 1); // last point == first point, drop it
    return hull;
}
