// ==================== Geometry basics: points, vectors, orientation ====================
#include <bits/stdc++.h>
using namespace std;
typedef double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point() : x(0), y(0) {}
    Point(ld x_, ld y_) : x(x_), y(y_) {}
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(ld t) const { return {x * t, y * t}; }
    ld cross(const Point& p) const { return x * p.y - y * p.x; }
    ld dot(const Point& p) const { return x * p.x + y * p.y; }
    ld norm() const { return sqrt(x*x + y*y); }
};

ld cross(const Point& O, const Point& A, const Point& B) {
    return (A - O).cross(B - O);
}
ld dist(const Point& a, const Point& b) { return (a - b).norm(); }

// orientation: >0 counter-clockwise, <0 clockwise, 0 collinear
int orientation(const Point& O, const Point& A, const Point& B) {
    ld c = cross(O, A, B);
    if (c > EPS) return 1;
    if (c < -EPS) return -1;
    return 0;
}

// does segment p1p2 properly intersect segment p3p4?
bool segmentsIntersect(Point p1, Point p2, Point p3, Point p4) {
    int d1 = orientation(p3, p4, p1);
    int d2 = orientation(p3, p4, p2);
    int d3 = orientation(p1, p2, p3);
    int d4 = orientation(p1, p2, p4);
    if (d1 != d2 && d3 != d4) return true;
    // collinear special cases (touching/overlap) omitted for brevity; add on-segment checks if needed
    return false;
}

// area of a simple polygon (shoelace formula), points in order
ld polygonArea(vector<Point>& poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += poly[i].cross(poly[j]);
    }
    return fabs(area) / 2.0;
}

// point in polygon (ray casting), poly need not be convex
bool pointInPolygon(vector<Point>& poly, Point p) {
    int n = poly.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
            (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
            inside = !inside;
    }
    return inside;
}
