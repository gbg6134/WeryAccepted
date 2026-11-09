//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MOD=1e9+7;
const ll MAXN=26;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll t,n,fac[MAXN],inv[MAXN],ans,k;

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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>k;
    ll temp=1;
    for(int i=1;i<=n;i++){
        temp*=i;
        temp%=MOD;
        inv[i]=fast(temp,MOD-2,MOD);
        fac[i]=temp;
    }

    fac[0]=1;
    inv[0]=1;

    for(int i=0;i<=n;i++){
        ll comb=((fac[n]*inv[i])%MOD*inv[n-i])%MOD;
    }
}
