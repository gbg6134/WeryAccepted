//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,m,dist[MAXN];
bool already[MAXN];

int Link[MAXN],cnt;

struct node{
    int end,next,w;
}Edge[2*MAXN];

void insert(int x,int y,int z){
    int temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
    Edge[cnt].w=z;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m;

    for(int i=1;i<=m;i++){
        ll a,b,c;
        cin>>a>>b>>c;
        insert(a,b,c);
    }

    for(int i=1;i<=n;i++) dist[i]=INF;

    priority_queue<pair<ll,ll>> Q;
    Q.push({0,1});
    dist[1]=0;

    while(!Q.empty()){
        ll a=Q.top().second;
        Q.pop();

        if(already[a]) continue;
        already[a]=1;
        for(int i=Link[a];i;i=Edge[i].next){
            if(dist[Edge[i].end]>dist[a]+Edge[i].w){
                dist[Edge[i].end]=dist[a]+Edge[i].w;
                Q.push({-dist[Edge[i].end],Edge[i].end});
            }
        }
    }

    for(int i=1;i<=n;i++) cout<<dist[i]<<' ';
    cout<<'\n';
}
