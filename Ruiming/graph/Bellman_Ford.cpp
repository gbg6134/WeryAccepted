//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,m,dist[MAXN],times;
bool already[MAXN];

int Link[MAXN],cnt;

struct node{
    int end,next,w,start;
}Edge[2*MAXN],id;

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
    ll t;
    cin>>t;
    while(t--){
        cin>>n>>m;

        cnt=0;
        for(int i=1;i<=m;i++) Edge[i]=id;
        for(int i=1;i<=n;i++){
            Link[i]=0;
            already[i]=0;
            dist[i]=INF;
        }

        for(int i=1;i<=m;i++){
            ll a,b,c;
            cin>>a>>b>>c;
            if(c>=0){
                insert(a,b,c);
                insert(b,a,c);
            }else insert(a,b,c);
        }

        ll times=0;
        bool change=true;

        dist[1]=0;
        while(change){
            times++;
            change=0;
            for(int j=1;j<=cnt;j++){
                if(dist[Edge[j].end]>Edge[j].w+dist[Edge[j].start]&&dist[Edge[j].start]!=INF){
                    dist[Edge[j].end]=Edge[j].w+dist[Edge[j].start];
                    change=1;
                }
            }
            if(times>=n) break;
        }
        
        if(times>=n) cout<<"YES"<<'\n';
        else cout<<"NO"<<'\n';
    }
}
