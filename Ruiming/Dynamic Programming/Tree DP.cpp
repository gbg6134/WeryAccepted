//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans[MAXN],sz[MAXN],dp[MAXN];

int Link[MAXN],cnt;

struct node{
    int end,next;
}Edge[2*MAXN];

void insert(int x,int y){
    int temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
}

void dfs(ll now,ll pa){
    for(int i=Link[now];i;i=Edge[i].next){
        if(Edge[i].end==pa) continue;

        dfs(Edge[i].end,now);

        sz[now]+=sz[Edge[i].end];
        dp[now]+=dp[Edge[i].end]+sz[Edge[i].end];
    }
}

void dfs1(ll now,ll pa){
    if(pa!=-1){
        ans[now]=n-sz[now]+ans[pa]-sz[now];
    }else ans[1]=dp[1];
    for(int i=Link[now];i;i=Edge[i].next){
        if(Edge[i].end==pa) continue;

        dfs1(Edge[i].end,now);
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    for(int i=1;i<=n-1;i++){
        ll a,b;
        cin>>a>>b;
        insert(a,b);
        insert(b,a);
    }
    
    for(int i=1;i<=n;i++){
        sz[i]=1;
    }

    dfs(1,-1);
    dfs1(1,-1);
}
