//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=LONG_LONG_MAX;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,G[MAXN][MAXN],mark[MAXN],odd,even;
bool already[MAXN],p,pos;

void dfs(ll now,ll c){
    already[now]=1;
    mark[now]=c;
    if(c==0) even++;
    else odd++;

    for(int i=1;i<=n;i++){
        if(G[now][i]){
            if(!already[i]){
                dfs(i,c^1);
            }else{
                if(mark[i]!=c^1){
                    pos=0;
                }
            }
        }
    }
}
