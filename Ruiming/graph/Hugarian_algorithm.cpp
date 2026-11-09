//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;

//O(V*E)
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,m,e;
ll match_l[MAXN],match_r[MAXN];//l match is always current u
bool vis[MAXN];//r match is the otherside the v
set<ll> adj[MAXN];

bool dfs(ll u){//u is always on the right side
    for(auto& v:adj[u]){//v is always on the left side
        if(!vis[v]){
            vis[v]=1;
            if(!match_r[v]||dfs(match_r[v])){//match_ri s vs match in r
                match_l[u]=v;
                match_r[v]=u;
                return true;
            }
        }
    }
    return false;
}

ll hugarian(){
    ll res=0;
    for(int i=1;i<=n;i++){
        memset(vis,0,sizeof(vis));
        vis[i]=1;
        if(dfs(i)){//if we successfully found an alternative edge
            res++;//paired node i
        }
    }
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m>>e;

    for(int i=1;i<=e;i++){
        ll x,y;
        cin>>x>>y;

        adj[x].insert(y+n);
        adj[y+n].insert(x);
    }

    ans=hugarian();
    cout<<ans<<'\n';
}
