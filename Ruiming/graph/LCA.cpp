//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=506;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,st[MAXN][26],q,level[MAXN];

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
    level[now]=level[pa]+1;
    st[now][0]=pa;
    for(int i=Link[now];i;i=Edge[i].next){
        if(Edge[i].end==pa) continue;
 
        dfs(Edge[i].end,now);
    }
}

ll lca(ll a,ll b){ 
    if(level[a]>level[b]) swap(a,b);

    ll step=20;
    while(level[b]>level[a]){
        while((1<<step)>level[b]-level[a]){
            step--;
        }

        b=st[b][step];
    }
    //same height

    step=20;
    while(a!=b){
        while(st[a][step]==st[b][step]&&step>0) step--;

        a=st[a][step];
        b=st[b][step];
    }
    return a;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll s;
    cin>>n>>q>>s;
    
    for(int i=2;i<=n;i++){
        ll a,b;
        cin>>a>>b;
        insert(a,b);
        insert(b,a);
    }

    dfs(s,-1);

    for(int i=1;i<=20;i++){
        for(int j=1;j<=n;j++){
            st[j][i]=st[st[j][i-1]][i-1];//spare table seems to be correct
        }
    }

    for(int i=1;i<=q;i++){
        ll a,b;
        cin>>a>>b;
        cout<<lca(a,b)<<'\n';
    }
}
