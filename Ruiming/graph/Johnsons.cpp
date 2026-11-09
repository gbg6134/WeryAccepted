//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e9;
const ll MAXN=306;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],m,dist[MAXN],times,ans[MAXN][MAXN];
bool already[MAXN][MAXN];

int Link[MAXN],cnt;

struct node{
    int end,next,w,start;
}Edge[3*MAXN],id;

void insert(int x,int y,int z){
    int temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
    Edge[cnt].w=z;
    Edge[cnt].start=x;
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

    for(int i=1;i<=n;i++) insert(0,i,0);

    ll times=0;
    bool change=true;

    dist[0]=0;
    while(change){
        times++;
        change=0;
        for(int j=1;j<=cnt;j++){
            if(dist[Edge[j].end]>Edge[j].w+dist[Edge[j].start]&&dist[Edge[j].start]!=INF){
                dist[Edge[j].end]=Edge[j].w+dist[Edge[j].start];
                change=1;
            }
        }
        if(times>=n+1) break;
    }
    
    if(times>=n+1){
        cout<<-1<<'\n';
        return 0;
    }
    
    for(int i=1;i<=cnt;i++){
        Edge[i].w+=dist[Edge[i].start]-dist[Edge[i].end];
    }
    
    for(int i=1;i<=n;i++) for(int j=1;j<=n;j++) ans[i][j]=INF;

    for(int t=1;t<=n;t++){
        priority_queue<pair<ll,ll>> Q;
        Q.push({0,t});
        ans[t][t]=0;

        while(!Q.empty()){
            ll a=Q.top().second;
            Q.pop();

            if(already[t][a]) continue;
            already[t][a]=1;
            for(int i=Link[a];i;i=Edge[i].next){
                if(ans[t][Edge[i].end]>ans[t][a]+Edge[i].w){
                    ans[t][Edge[i].end]=ans[t][a]+Edge[i].w;
                    Q.push({-ans[t][Edge[i].end],Edge[i].end});
                }
            }
        }

        ll res=0;
        for(int i=1;i<=n;i++){
            if(ans[t][i]!=1e9) ans[t][i]-=dist[t]-dist[i];
            res+=i*ans[t][i];
        }
        cout<<res<<'\n';
    }
}
