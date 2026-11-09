//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,t,ans;

ll f(ll x){

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    ll start=0,last=1e9;
    while(last - start > 2){
        ll mid1 = start + (last - start) / 3;
        ll mid2 = last - (last - start) / 3;
        // Guaranteed: start < mid1 < mid2 < last when last-start > 2

        if(f(mid1) < f(mid2)){
            last = mid2;      // minimum in [start, mid2]
        }else{
            start = mid1;     // minimum in [mid1, last]
        }
    }

    // At most 3 elements remain — check all
    ll ans = f(start);
    for(ll i = start+1; i <= last; i++) ans=min(ans,f(i));

    cout<<ans<<'n';
}
