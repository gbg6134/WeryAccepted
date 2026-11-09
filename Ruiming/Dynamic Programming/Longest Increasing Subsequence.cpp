//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,dp[MAXN],cnt;
 
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    for(int i=1;i<=n;i++) cin>>lst[i];
 
    for(int i=1;i<=n;i++){
        ll pos=lower_bound(dp+1,dp+cnt+1,lst[i])-dp;//becase I have 1 indexing
        if(pos==cnt+1){
            dp[++cnt]=lst[i];
        }else{
            dp[pos]=lst[i];
        }
    }    
    cout<<cnt<<'\n';//this also constructs a LIS
}
