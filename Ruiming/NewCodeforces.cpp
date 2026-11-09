//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll t,n,a[MAXN],ans;

void init(){
    for(int i=0;i<=n;i++){
        a[i]=0;
    }
}

void solve(){
    cin>>n;
    init();
    for(int i=1;i<=n;i++) cin>>a[i];
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>t;
    while(t--){
        solve();
    }
}
