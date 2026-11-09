//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=506;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,dp[MAXN][MAXN],ans,a[MAXN];//circular

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>a[i];
    }

    for(int i=1;i<=n;i++) for(int j=1;j<=n;j++) dp[i][j]=INF;
    for(int i=1;i<=n;i++) dp[i][i]=0;//line 2 find max

    for(int len=2;len<=n;len++){//length
        for(int i=1;i<=n-len+1;i++){//start point
            ll j=i+len-1;//end point
            for(int k=i;k<j;k++){
                dp[i][j]=max(dp[i][j],dp[i][k]+dp[k+1][j]);
            }
        }
    }
}
