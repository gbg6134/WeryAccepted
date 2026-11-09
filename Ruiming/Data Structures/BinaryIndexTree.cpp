//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans,tree[MAXN],q;
#define lowbit(x) ((x)&-(x));

void update(ll x,ll v){
    while(x<=n){
        tree[x]+=v;
        x+=lowbit(x);
    }
}

ll sum(ll x){
    ll res=0;
    while(x>0){
        res+=tree[x];
        x-=lowbit(x);
    }
    return res;
}


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>q;
    for(int i=1;i<=n;i++){
        cin>>a[i];
    }
}
