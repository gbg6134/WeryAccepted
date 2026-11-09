#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

ll fast(ll x,ll p,ll m){
    x%=m;
    ll ans=1;
    while(p>0){
        if(p&1){
            ans*=x;
            ans%=m;
        }
        x*=x;
        x%=m;
        p>>=1;
    }
    return ans;
}
bool flag;

ll gcd(ll x,ll y){
    while(x!=y){
        if(x>y) swap(x,y);
        if(y%x==0) return x;
        y-=x*(y/x);
    }
    return x;
}



ll a,m,phi=1;
string b;

ll read(){
    string x;
    cin>>x;
    ll ans=0;

    for(int i=0;i<x.size();i++){
        ans=(ans*10+(x[i]-'0'));
        if(ans>=phi) flag=true;
        ans%=phi;
    }
    return ans;
}

int main(){
    cin>>a>>m;
    //find the euler function to m

    ll d=gcd(a,m);

    ll temp=m;
    for(int i=2;i<=temp;i++){
        if(temp%i==0){//here uses temp still makes sure i is a prime
            ll times=0;
            while(temp%i==0){
                times++;
                temp/=i;//makes sure i is a prime
            }
            phi*=pow(i,times-1)*(i-1);
        }
    }
    //if(phi==1) phi=m-1;
    
    //b needs to be stored as a string and takes mod phi
    //then we can calculate it with fast mod
    ll b=read();
    ll ans;
    if(flag) ans=fast(a,b+phi,m);
    else ans=fast(a,b,m);

    //deal with d d|m, will lead up to a , should be some period

    cout<<ans<<'\n';
}
