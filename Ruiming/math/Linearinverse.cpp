#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN=206;
ll n,p,ans[MAXN],corr[MAXN],now,fac[MAXN],inv[MAXN];

ll fastMod(ll a,ll n,ll mod){
    ll ans=1;
    a%=mod;
    while(n){
        if(n&1){
            ans=(ans*a)%mod;
        }
        a=(a*a)%mod;
        n>>=1;
    }
    return ans;
}

int main(){
    cin>>n>>p;

    fac[0]=1;
    for(int i=1;i<=n;i++){
        fac[i]=(fac[i-1]*i)%p;
    }

    inv[n]=fastMod(fac[n],p-2,p);//inv fac[i]
    for(int i=n-1;i>=1;i--){
        inv[i]=((i+1)*inv[i+1])%p;
    }

    for(int i=1;i<=n;i++){
        cout<<(fac[i-1]*inv[i])%p<<'\n';
    }
}
