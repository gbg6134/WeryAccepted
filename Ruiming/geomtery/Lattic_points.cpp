//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll INF = 1e18;
const ll MAXN = 1006; // UPDATERA ARRAY STORLEKEN!!!!!!!!!!

const ld pi = acos(-1.0L);
ld eps = 1e-9;

// 浮点比较
ll ldcmp(ld x, ld y) {
    if (fabs(x - y) < eps) return 0;
    return x < y ? -1 : 1;
}

// ==================== 整数点 ====================
struct PointLL {
    ll x, y;
    PointLL() : x(0), y(0) {}
    PointLL(ll x, ll y) : x(x), y(y) {}

    PointLL operator+(const PointLL& B) const { return PointLL(x + B.x, y + B.y); }
    PointLL operator-(const PointLL& B) const { return PointLL(x - B.x, y - B.y); }
    PointLL operator*(ll k) const { return PointLL(x * k, y * k); }
    PointLL operator/(ll k) const { return PointLL(x / k, y / k); }
    bool operator==(const PointLL& B) const { return x == B.x && y == B.y; }
    bool operator!=(const PointLL& B) const { return !(*this == B); }
};

Point POF=Point(INF,INF);

typedef PointLL VectorLL;

// 点积
ll Dot(const VectorLL& A, const VectorLL& B) {
    return A.x * B.x + A.y * B.y;
}

// 叉积
ll Cross(const VectorLL& A, const VectorLL& B) {
    return A.x * B.y - A.y * B.x;
}

// 距离平方
ll Dist2(const PointLL& A, const PointLL& B) {
    ll dx = A.x - B.x, dy = A.y - B.y;
    return dx * dx + dy * dy;
}

// 有向面积的两倍
ll Area2(const PointLL& A, const PointLL& B, const PointLL& C) {
    return Cross(B - A, C - A);
}

// 判断平行
bool Parallel(const VectorLL& A, const VectorLL& B) {
    return Cross(A, B) == 0;
}

// 直线（整数点）
struct LineLL {
    PointLL p1, p2;
    LineLL() {}
    LineLL(const PointLL& p1, const PointLL& p2) : p1(p1), p2(p2) {}
};

// 点与直线关系：1 左侧，2 右侧，0 在直线上
ll Point_line_relation(const PointLL& P, const LineLL& v) {
    ll c = Cross(P - v.p1, v.p2 - v.p1);
    if (c < 0) return 1;
    if (c > 0) return 2;
    return 0;
}

// 点是否在线段上
bool Point_on_seg(const PointLL& P, const LineLL& v) {
    return Cross(P - v.p1, v.p2 - v.p1) == 0 &&
           min(v.p1.x, v.p2.x) <= P.x && P.x <= max(v.p1.x, v.p2.x) &&
           min(v.p1.y, v.p2.y) <= P.y && P.y <= max(v.p1.y, v.p2.y);
}

// 两直线关系：0 平行，1 重合，2 相交
ll Line_relation(const LineLL& v1, const LineLL& v2) {
    if (Cross(v1.p2 - v1.p1, v2.p2 - v2.p1) == 0) {
        if (Cross(v1.p1 - v2.p1, v2.p2 - v2.p1) == 0) return 1;
        return 0;
    }
    return 2;
}

// 点与多边形关系（整数精确）
// 返回：3 顶点，2 边，1 内部，0 外部
ll Point_in_polygon(const PointLL& pt, const PointLL* p, ll n) {
    for (int i = 1; i <= n; i++) {
        if (p[i] == pt) return 3;
    }
    for (int i = 1; i <= n; i++) {
        int nxt = (i % n) + 1;
        LineLL v(p[i], p[nxt]);
        if (Point_on_seg(pt, v)) return 2;
    }
    bool inside = false;
    for (int i = 1, j = n; i <= n; j = i++) {
        if ((p[i].y > pt.y) != (p[j].y > pt.y)) {
            ll dy = p[j].y - p[i].y;
            ll dx = p[j].x - p[i].x;
            ll lhs = (pt.x - p[i].x) * dy;
            ll rhs = (pt.y - p[i].y) * dx;
            if (dy > 0) {
                if (lhs < rhs) inside = !inside;
            } else {
                if (lhs > rhs) inside = !inside;
            }
        }
    }
    return inside ? 1 : 0;
}

// 多边形面积的两倍（有向），返回 ll
ll Polygon_area2(const PointLL* p, ll n) {
    ll area = 0;
    for (int i = 1; i <= n; i++) {
        int nxt = (i % n) + 1;
        area += p[i].x * p[nxt].y - p[i].y * p[nxt].x;
    }
    return area;
}

bool Cross_Segment(Point a,Point b,Point c,Point d){
    Point P=Cross_point(a,b,c,d);
    if(P==POF) return 0;

    //special case if all share a line, in line case whatever works, not in segment case
    if(Point_on_seg(a,Line(c,d))||Point_on_seg(b,Line(c,d))||Point_on_seg(c,Line(a,b))||Point_on_seg(d,Line(a,b))) return 1;

    if(Point_on_seg(P,Line(a,b))&&Point_on_seg(P,Line(c,d))) return 1;
    else return 0;
}

// ==================== 浮点点 ====================
struct Point{
    ld x,y;
    Point(): x(0),y(0){}
    Point(ld x,ld y): x(x),y(y) {}

    Point operator + (Point B){return Point(x+B.x,y+B.y);}
    Point operator - (Point B){return Point(x-B.x,y-B.y);}
    Point operator * (ld k){return Point(x*k,y*k);}
    Point operator / (ld k){return Point(x/k,y/k);}
    bool operator == (Point B){return ((ldcmp(x-B.x,0)==0)&&(ldcmp(y-B.y,0)==0));}
    bool operator < (Point B){
        if (x != B.x) return x < B.x;
        return y < B.y;
    }
};

typedef Point Vector;

// 距离
ld Dist(const PointLL& A, const PointLL& B) {
    return sqrt((ld)Dist2(A, B));
}

ld Dot(Vector A,Vector B){return A.x*B.x+A.y*B.y;}
ld Len(Vector A){return sqrt(Dot(A,A));}
ld Cross(Vector A,Vector B){return A.x*B.y-A.y*B.x;}
bool Parallel(Vector A,Vector B){return ldcmp(Cross(A,B),0)==0;}
ld Angle(Vector A,Vector B){
    if(Parallel(A,B)) return 0;
    return acos(Dot(A,B)/Len(A)/Len(B));
}
ld DirectedAngle(Vector A, Vector B) {
    return atan2l(Cross(A,B), Dot(A,B));
}
ld Area(Point A,Point B,Point C){return Cross(B-A,C-A)/2;}
Vector Rotate(Vector A,ld rad){
    return Vector(A.x*cos(rad)-A.y*sin(rad),A.x*sin(rad)+A.y*cos(rad));
}

// 点到直线距离
ld Dis_point_line(const PointLL& P, const LineLL& v) {
    return fabs((ld)Cross(P - v.p1, v.p2 - v.p1)) / Dist(v.p1, v.p2);
}

// 点到线段距离
ld Dis_point_seg(const PointLL& P, const LineLL& v) {
    if (Dot(P - v.p1, v.p2 - v.p1) < 0 || Dot(P - v.p2, v.p1 - v.p2) < 0) {
        return min(Dist(P, v.p1), Dist(P, v.p2));
    }
    return Dis_point_line(P, v);
}

// 点在直线上的投影（返回浮点点）
Point Point_line_proj(const PointLL& P, const LineLL& v) {
    ld k = (ld)Dot(v.p2 - v.p1, P - v.p1) / Dot(v.p2 - v.p1, v.p2 - v.p1);
    return Point(v.p1.x + (v.p2.x - v.p1.x) * k, v.p1.y + (v.p2.y - v.p1.y) * k);
}

// 点关于直线的对称点（返回浮点点）
Point Point_line_symmetry(const PointLL& P, const LineLL& v) {
    Point q = Point_line_proj(P, v);
    return Point(2 * q.x - P.x, 2 * q.y - P.y);
}

// 两直线交点（返回浮点点）。要求两直线不平行。
Point Cross_point(const PointLL& a, const PointLL& b, const PointLL& c, const PointLL& d) {
    ld s1 = (ld)Cross(b - a, c - a);
    ld s2 = (ld)Cross(b - a, d - a);
    ld denom = s2 - s1;
    return Point(
        (c.x * s2 - d.x * s1) / denom,
        (c.y * s2 - d.y * s1) / denom
    );
}

// 向量旋转（返回浮点点）
Point Rotate(const PointLL& A, ld rad) {
    ld c = cos(rad), s = sin(rad);
    return Point(A.x * c - A.y * s, A.x * s + A.y * c);
}

// 全局数组和读入示例
PointLL poly[MAXN];

ll n,m;

void solve() {
    cin>>n>>m;
    eps = 1e-6;
    for (int i = 1; i <= n; i++) {
        cin >> poly[i].x >> poly[i].y;
    }
    for(int i=1;i<=m;i++){
        PointLL P;
        cin>>P.x>>P.y;
        ll x=Point_in_polygon(P,poly,n);

        if(x>=2) cout<<"BOUNDARY"<<'\n';
        else if(x==1){
            cout<<"INSIDE"<<'\n';
        }else cout<<"OUTSIDE"<<'\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
