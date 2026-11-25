//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=100006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

bool Cross_Segment(Point a,Point b,Point c,Point d){
    Point P=Cross_point(a,b,c,d);
    if(P==POF) return 0;

    //special case if all share a line, in line case whatever works, not in segment case
    if(Point_on_seg(a,Line(c,d))||Point_on_seg(b,Line(c,d))||Point_on_seg(c,Line(a,b))||Point_on_seg(d,Line(a,b))) return 1;

    if(Point_on_seg(P,Line(a,b))&&Point_on_seg(P,Line(c,d))) return 1;
    else return 0;
}

struct Circle{
    Point c;
    ld r;
    Circle(){}
    Circle(Point c,ld r): c(c),r(r){}
    Circle(ld x,ld y,ld _r){c=Point(x,y),r=_r;}
};

ll Point_circle_relation(Point P,Circle C){
    ld dist=Dist(P,C.c);
    if(ldcmp(dist-C.r,0)<0) return 0; //inside circle
    if(ldcmp(dist-C.r,0)==0) return 1; //on circle
    return 2; //outside of circle
}

ll Line_Circle_relation(Line v,Circle C){
    ld dist=Dis_point_line(C.c,v);
    if(ldcmp(dist-C.r,0)<0) return 0;
    if(ldcmp(dist-C.r,0)==0) return 1;
    return 2;
}

typedef Line Segment;

ll Seg_circle_relation(Segment v,Circle C){
    ld dist=Dis_point_seg(C.c,v);
    if(ldcmp(dist-C.r,0)<0) return 0;
    if(ldcmp(dist-C.r,0)==0) return 1;
    return 2;
}

ll Line_Cross_Circle(Line v,Circle C,Point &pa,Point &pb){
    if(Line_Circle_relation(v,C)==2) return 0;
    Point q=Point_line_proj(C.c,v);
    ld d=Dis_point_line(C.c,v);
    ld k=sqrt(C.r*C.r-d*d);
    if(ldcmp(k,0)==0){
        pa=q;
        pb=q;
        return 1;
    }
    Point n=(v.p2-v.p1)/Len(v.p2-v.p1);
    pa=q+n*k;
    pb=q-n*k;
    return 2;
}

Point Circumcenter(Point A,Point B,Point C){
    Point center;
    ld a1=B.x-A.x,b1=B.y-A.y,c1=(a1*a1+b1*b1)/2;
    ld a2=C.x-A.x,b2=C.y-A.y,c2=(a2*a2+b2*b2)/2;
    ld d=a1*b2-a2*b1;
    center.x=A.x+(c1*b2-c2*b1)/d;
    center.y=A.y+(a1*c2-a2*c1)/d;
    return center;
}

void min_center_circle(Point *p,ll n,Point &c,ld &r){
    random_shuffle(p+1,p+n+1);
    c=p[1];
    r=0;
    for(int i=2;i<=n;i++){
        if(ldcmp(Dist(p[i],c)-r,0)>0){
            c=p[i];
            r=0;
            for(int j=1;j<i;j++){
                if(ldcmp(Dist(p[j],c)-r,0)>0){
                    c.x=(p[i].x+p[j].x)/2;
                    c.y=(p[i].y+p[j].y)/2;
                    r=Dist(p[i],c);
                    for(int k=1;k<j;k++){
                        if(ldcmp(Dist(p[k],c)-r,0)>0){
                            c=Circumcenter(p[i],p[j],p[k]);
                            r=Dist(p[i],c);
                        }
                    }
                }
            }
        }
    }
}

Point p[MAXN];

void solve(){
    cin>>n;

    Point c;
    ld r;
    for(int i=1;i<=n;i++){
        cin>>p[i].x>>p[i].y;
    }

    min_center_circle(p,n,c,r);
    cout<<fixed<<setprecision(10);
    cout<<r<<' '<<c.x<<' '<<c.y<<'\n';
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();
    
}
