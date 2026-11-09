//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include<bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll t,n,ans;

const ld pi=acos(-1.0);
ld eps=1e-6;//MIGHT GET WA PGA EPS TOO SMALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!

ll ldcmp(ld x,ld y){
    if(fabs(x-y)<eps) return 0;
    else return x<y?-1:1;
    //is x<y return -1, x>y return 1;
}

struct Point{
    ld x,y;
    Point(): x(0),y(0){}
    Point(ld x,ld y): x(x),y(y) {}

    Point operator + (Point B){return Point(x+B.x,y+B.y);}
    Point operator - (Point B){return Point(x-B.x,y-B.y);}
    Point operator * (ld k){return Point(x*k,y*k);}
    Point operator / (ld k){return Point(x/k,y/k);}
    bool operator == (Point B){return ((ldcmp(x-B.x,0)==0)&&(ldcmp(y-B.y,0)==0));}
    bool operator<(Point B) {
        if (ldcmp(x, B.x) != 0) return x < B.x;
        return y < B.y;
    }
};

Point POF=Point(INF,INF);

ld Dist(Point A,Point B){
    return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));
}

typedef Point Vector;

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

struct Line {
    Point p1, p2;
    Vector v;
    ld ang;

    Line() : p1(0,0), p2(0,0), v(0,0), ang(0) {}

    Line(Point p1, Point p2) : p1(p1), p2(p2) {
        v = p2 - p1;
        ang = atan2l(v.y, v.x);
    }

    Line(Point P, ld angle) : p1(P) {
        p2 = p1 + Point(cosl(angle), sinl(angle));
        v = p2 - p1;
        ang = atan2l(v.y, v.x);
    }

    Line(ld a, ld b, ld c) {
        if (ldcmp(a, 0) == 0) {
            p1 = Point(0, -c/b);
            p2 = Point(1, -c/b);
        } else if (ldcmp(b, 0) == 0) {
            p1 = Point(-c/a, 0);
            p2 = Point(-c/a, 1);
        } else {
            p1 = Point(0, -c/b);
            p2 = Point(1, (-c-a)/b);
        }
        v = p2 - p1;
        ang = atan2l(v.y, v.x);
    }

    bool operator<(Line L) {
        if (ldcmp(ang, L.ang) != 0) return ang < L.ang;
        return ldcmp(Cross(L.p1 - p1, L.p2 - p1), 0) > 0;
    }
};

//for directed lines from p1 to p2
ll Point_line_relation(Point P, Line v){
    ll c=ldcmp(Cross(P-v.p1,v.p2-v.p1),0);
    if(c<0) return 1;//P on the leftside of v
    if(c>0) return 2;//P on the rightside of v
    return 0; //P on v
}

bool Point_on_seg(Point P, Line v){
    return ldcmp(Cross(P-v.p1,v.p2-v.p1),0)==0&&ldcmp(Dot(P-v.p1,P-v.p2),0)<=0;
}

ld Dis_point_line(Point P, Line v){
    return fabs(Cross(P-v.p1,v.p2-v.p1)/Dist(v.p1,v.p2));
}

Point Point_line_proj(Point P, Line v){
    ld k=Dot(v.p2-v.p1,P-v.p1)/Dot(v.p2-v.p1,v.p2-v.p1);
    return v.p1+(v.p2-v.p1)*k;
}

Point Point_line_symmetry(Point P, Line v){
    Point q=Point_line_proj(P,v);
    return Point(2*q.x-P.x,2*q.y-P.y);
}

ld Dis_point_seg(Point P, Line v){
    if(ldcmp(Dot(P-v.p1,v.p2-v.p1),0)<0||ldcmp(Dot(P-v.p2,v.p1-v.p2),0)<0){
        return min(Dist(P,v.p1),Dist(P,v.p2));
    }
    return Dis_point_line(P,v);
}

//Directed lines
ll Line_relation(Line v1, Line v2){
    if(ldcmp(Cross(v1.p2-v1.p1,v2.p2-v2.p1),0)==0){
        //overlapp
        if(Point_line_relation(v1.p1,v2)==0) return 1;

        //parallell
        else return 0;
    }
    //intersect
    return 2;
}

bool OnLeft(Line L, Point p) {
    return ldcmp(Cross(L.v, p - L.p1), 0) >= 0;
}

Point Cross_point(Line a, Line b) {
    Vector u = a.p1 - b.p1;
    ld t = Cross(b.v, u) / Cross(a.v, b.v);
    return a.p1 + a.v * t;
}

vector<Point> HPI(vector<Line> L) {
    int n = L.size();
    if (n == 0) return {};

    sort(L.begin(), L.end());

    // 极角去重，只保留最靠左的一条
    vector<Line> lines;
    for (int i = 0; i < n; i++) {
        if (i > 0 && ldcmp(L[i].ang, L[i-1].ang) == 0) continue;
        lines.push_back(L[i]);
    }
    n = lines.size();
    if (n <= 2) return {};

    vector<Line> q(n);
    vector<Point> p(n);
    int first = 0, last = 0;
    q[0] = lines[0];

    for (int i = 1; i < n; i++) {
        while (first < last && !OnLeft(lines[i], p[last-1])) last--;
        while (first < last && !OnLeft(lines[i], p[first])) first++;

        q[++last] = lines[i];

        if (ldcmp(Cross(q[last].v, q[last-1].v), 0) == 0) {
            last--;
            if (OnLeft(q[last], lines[i].p1)) q[last] = lines[i];
        }

        if (first < last) {
            p[last-1] = Cross_point(q[last-1], q[last]);
        }
    }

    while (first < last && !OnLeft(q[first], p[last-1])) last--;
    if (last - first <= 1) return {};

    p[last] = Cross_point(q[last], q[first]);

    vector<Point> ans;
    for (int i = first; i <= last; i++) ans.push_back(p[i]);
    return ans;
}

void solve() {
    int numPoly;
    cin >> numPoly;          // 多边形数量
    vector<Line> L;
    for (int i = 0; i < numPoly; i++) {
        int m;
        cin >> m;            // 当前多边形的点数
        vector<Point> poly(m);
        for (int j = 0; j < m; j++) {
            cin >> poly[j].x >> poly[j].y;
        }
        // 将多边形的每条边转换为有向直线（逆时针，左侧为内部）
        for (int j = 0; j < m; j++) {
            L.push_back(Line(poly[j], poly[(j+1)%m]));
        }
    }
    vector<Point> poly = HPI(L);
    if (poly.empty()) {
        cout << "0.000\n";
        return;
    }
    ld area = 0;
    int m = poly.size();
    for (int i = 0; i < m; i++) {
        area += Cross(poly[i], poly[(i+1)%m]);
    }
    area = fabs(area) / 2.0;
    cout << fixed << setprecision(3) << area << '\n';
}

int main(){
    solve();
}
