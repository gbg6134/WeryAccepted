//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=1006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll t,n,a[MAXN],ans;

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
    bool operator < (Point B){
        if (x != B.x) return x < B.x;
        return y < B.y;
    }
};

ld Dist(Point A,Point B){
    return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));
}

typedef Point Vector;

ld Dot(Vector A,Vector B){return A.x*B.x+A.y*B.y;}
ld Len(Vector A){return sqrt(Dot(A,A));}
ld Angle(Vector A,Vector B){return acos(Dot(A,B)/Len(A)/Len(B));}
ld Cross(Vector A,Vector B){return A.x*B.y-A.y*B.x;}
ld Area(Point A,Point B,Point C){return Cross(B-A,C-A)/2;}
Vector Rotate(Vector A,ld rad){
    return Vector(A.x*cos(rad)-A.y*sin(rad),A.x*sin(rad)+A.y*cos(rad));
}
bool Parallel(Vector A,Vector B){return ldcmp(Cross(A,B),0)==0;}

struct Line{
    Point p1,p2;
    Line(){}
    Line(Point p1,Point p2): p1(p1),p2(p2){}
    Line(Point P,ld angle){
        p1=P;
        if(ldcmp(angle-pi/2,0)==0){
            p2=p1+Point(0,1);
        }else{
            p2=p1+Point(1,tan(angle));
        }
    }

    Line(ld a,ld b,ld c){
        if(ldcmp(a,0)==0){
            p1=Point(0,-c/b);
            p2=Point(1,-c/b);
        }else if(ldcmp(b,0)==0){
            p1=Point(-c/a,0);
            p2=Point(-c/a,1);
        }else{
            p1=Point(0,-c/b);
            p2=Point(1,(-c-a)/b);
        }
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

//Must have ab,cd not the same
Point Cross_point(Point a,Point b,Point c,Point d){
    ld s1=Cross(b-a,c-a);
    ld s2=Cross(b-a,d-a);
    return Point(c.x*s2-d.x*s1,c.y*s2-d.y*s1)/(s2-s1);
}

ll Point_in_polygon(Point pt, Point *p,ll n){//list of points p representing polygon
    for(int i=1;i<=n;i++){
        //Point on vetices
        if(ldcmp(Dist(p[i],pt),0)==0) return 3;
    }
    for(int i=1;i<=n;i++){
        Line v=Line(p[i],p[i%n+1]);
        
        //Point on Edge
        if(Point_on_seg(pt,v)) return 2;
    }
    ll num=0;
    for(int i=1;i<=n;i++){
        ll j=i%n+1;
        ll c=ldcmp(Cross(pt-p[j],p[i]-p[j]),0);
        ll u=ldcmp(p[i].y-pt.y,0);
        ll v=ldcmp(p[j].y-pt.y,0);
        if(c>0&&u<0&&v>=0) num++;
        if(c<0&&u>=0&&v<0) num--;
    }
    //return 1 is inside, 0 is outside
    return num!=0;
}

ld Polygon_area(Point *p,ll n){
    ld area=0;
    for(int i=1;i<=n;i++){
        area+=Cross(p[i],p[i%n+1]);
    }
    return abs(area/2.0);
}

void init(){
    for(int i=0;i<=n;i++){
        a[i]=0;
    }
}

Point poly[MAXN];

void solve(){
    cin>>n;
    eps=1e-6;
    for(int i=1;i<=n;i++){
        cin>>poly[i].x>>poly[i].y;
        //eps = max(eps,1e-6* (ld)max(abs(poly[i].x),abs(poly[i].y)));
    }

    cout<<abs(Polygon_area(poly,n))<<'\n';
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
}
