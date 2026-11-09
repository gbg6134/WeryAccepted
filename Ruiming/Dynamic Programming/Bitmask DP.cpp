//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=26;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,dp[MAXN],ans;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    //template for bitmask dp
    for(int s=1;s<=(1<<n);s++){
        for(int j=1;j<=n;j++){
            if(s&(1<<(j-1))){
                for(int k=1;k<=n;k++){
                    dp[s][j]+=dp[s^(1<<(j-1))][k];  
                }
            }
        }
    }

    //transition from subset to set
    for(int s=1;s<(1<<n);s++){
        for(int i=s;i;i=(i-1)&s){
            int t=s^i;
            dp[s]=min(dp[t]+dp[i],dp[s]);
        }
    }
}
