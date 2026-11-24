//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans;

ll x,y,px,py,d;

void exgcd(ll a,ll b){
    if(b==0){
        x=1;
        y=0;
        d=a;    
    }else{
        exgcd(b,a-a/b*b);
        px=x;
        py=y;
        x=py;
        y=px-py*(a/b);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll a,b;
    cin>>a>>b;
    exgcd(a,b);
    cout<<(x%b+b)%b<<'\n';
}
