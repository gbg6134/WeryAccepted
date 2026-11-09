//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=100006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,dp[MAXN],a[MAXN],ans[MAXN];
vector<ll> last;
map<ll,ll> mapp;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    last.push_back(-INF);//dummy

    for(int i=1;i<=n;i++){
        cin>>a[i];
        ans[i]=INF;
    }

    for(int i=n;i>=1;i--){
        //directly add to the end
        if(-a[i]>*(--last.end())){
            last.push_back(-a[i]);
            dp[i]=last.size()-1;
        }else{
            //substitute
            auto temp=lower_bound(last.begin(),last.end(),-a[i]);
            dp[i]=temp-last.begin();
            last[temp-last.begin()]=-a[i];
        }
    }

    ll m=last.size()-1;

    for(int i=1;i<=n;i++){
        ll idx=m+1-dp[i];
        
        //only need to check larger than prev, if larger than this the even larger
        if(a[i]>ans[idx-1]){
            ans[idx]=a[i];
        }
    }

    for(int i=1;i<=m;i++){
        cout<<ans[i]<<' ';
    }
    cout<<'\n';
}
