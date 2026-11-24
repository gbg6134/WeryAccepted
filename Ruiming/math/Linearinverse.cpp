#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;

const int MAXN=206;
ll n,p,ans[MAXN],corr[MAXN],now,fac[MAXN],inv[MAXN];

ll mul(ll a, ll b, ll m) { return (__int128)a * b % m; }

ll fast(ll x,ll p,ll m){
    x=(x%m+m)%m;
    ll ans=1%m;
    while(p>0){
        if(p&1){
            ans=mul(ans,x,m);
        }
        x=mul(x,x,m);
        p>>=1;
    }
    return ans;
}

int main(){
    cin>>n>>p;

    fac[0]=1;
    for(int i=1;i<=n;i++){
        fac[i]=(fac[i-1]*i)%p;
    }

    inv[n]=fast(fac[n],p-2,p);//inv fac[i]
    for(int i=n-1;i>=1;i--){
        inv[i]=((i+1)*inv[i+1])%p;
    }

    for(int i=1;i<=n;i++){
        cout<<(fac[i-1]*inv[i])%p<<'\n';
    }
}
