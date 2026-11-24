//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans;

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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

}
