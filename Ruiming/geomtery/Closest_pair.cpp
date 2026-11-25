//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=10006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

bool cmpx(Point A,Point B){
    return ldcmp(A.x-B.x,0)<0||(ldcmp(A.x-B.x,0)==0&&ldcmp(A.y-B.y,0)<0);
}

bool cmpy(Point A,Point B){
    return ldcmp(A.y-B.y,0)<0;
}

void init(){
    for(int i=0;i<=n;i++){
        a[i]=0;
    }
}

Point p[MAXN],tmp[MAXN];

ld closest_pair(ll left,ll right){
    ld dis=INF;
    if(left==right) return dis;
    if(left+1==right) return Dist(p[left],p[right]);

    ll mid=(right+left)/2;
    ld cand1=closest_pair(left,mid);
    ld cand2=closest_pair(mid+1,right);
    dis=min(cand1,cand2);

    ll k=0;
    for(int i=left;i<=right;i++){
        if(fabs(p[mid].x-p[i].x)<=dis){
            tmp[++k]=p[i];
        }
    }

    sort(tmp+1,tmp+k+1,cmpy);

    for(int i=1;i<=k;i++){
        for(int j=i+1;j<=k;j++){
            if(ldcmp(tmp[j].y-tmp[i].y,dis)>=0) break;
            dis=min(dis,Dist(tmp[i],tmp[j]));
        }
    }

    return dis;
}

void solve(){
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>p[i].x>>p[i].y;
    }
    sort(p+1,p+n+1,cmpx);
    cout<<fixed<<setprecision(4);
    cout<<closest_pair(1,n)<<'\n';
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
}
