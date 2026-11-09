//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=100006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],b[MAXN],ans,k;
ld eps=1e-6;

bool check(ld x){
    vector<ld> all;
    ld res=0;

    for(int i=1;i<=n;i++){
        all.push_back(a[i]-x*b[i]);
    }
    sort(all.begin(),all.end(),greater<ld>());

    for(int i=0;i<n-k;i++){
        res+=all[i];
    }

    return res>=-eps;
}

ll zero_one_partition(){
    ld start=0,end=n;
    while(end-start>eps){
        ld x=start+(end-start)/2;

        //if f(x)>0
        if(check(x)){
            start=x+eps;
        }else end=x-eps;
    }

    ll res = floor(100 * start + 0.5);
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    while(true){
        cin>>n>>k;
        if(n==0&&k==0) break;

        for(int i=1;i<=n;i++) cin>>a[i];
        for(int i=1;i<=n;i++) cin>>b[i];

        cout<<zero_one_partition()<<'\n';
    }
}
