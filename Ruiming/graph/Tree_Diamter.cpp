//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,place,maxi;

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

void dfs1(ll now,ll pa,ll dist){
    if(dist>maxi){
        maxi=dist;
        place=now;
    }

    for(int i=Link[now];i;i=Edge[i].next){
        if(Edge[i].end==pa) continue;

        dfs1(Edge[i].end,now,dist+1);
    }
}

void dfs2(ll now,ll pa,ll dist){
    if(dist>ans){
        ans=dist;
    }

    for(int i=Link[now];i;i=Edge[i].next){
        if(Edge[i].end==pa) continue;

        dfs2(Edge[i].end,now,dist+1);
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

    dfs1(1,-1,0);
    maxi=0;
    dfs2(place,-1,0);

    cout<<ans<<'\n';
}
