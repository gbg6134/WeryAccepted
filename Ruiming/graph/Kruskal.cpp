//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,m;
ll Link[MAXN],cnt;
ll s[MAXN],sz[MAXN];

ll find_set(ll x){
    if(x!=s[x]){
        s[x]=find_set(s[x]);
    }
    return s[x];
}

void merge_set(ll x,ll y){
    ll a=find_set(x);
    ll b=find_set(y);

    if(a==b) return;
    
    sz[a]+=sz[b];
    s[b]=a;
}

struct node{
    ll end,next,w,start;
}Edge[2*MAXN];

void insert(ll x,ll y,ll z){
    ll temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].start=x;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
    Edge[cnt].w=z;
}

bool cmp(node x,node y){
    return x.w<y.w;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        s[i]=i;
        sz[i]=1;
    }

    for(int i=1;i<=m;i++){
        ll a,b,c;
        cin>>a>>b>>c;

        insert(a,b,c);
    }

    sort(Edge+1,Edge+m+1,cmp);

    for(int i=1;i<=m;i++){
        if(find_set(Edge[i].start)!=find_set(Edge[i].end)){
            merge_set(Edge[i].start,Edge[i].end);
            ans+=Edge[i].w;
        }
    }    

    if(sz[find_set(1)]!=n){
        cout<<"IMPOSSIBLE"<<'\n';
    }else{
        cout<<ans<<'\n';
    }
}
