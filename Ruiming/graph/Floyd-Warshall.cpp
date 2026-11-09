//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=506;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,dist[MAXN][MAXN],m,q;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m>>q;
    for(int i=1;i<=n;i++) for(int j=1;j<=n;j++) dist[i][j]=INF;
    for(int i=1;i<=n;i++) dist[i][i]=0;

    for(int i=1;i<=m;i++){
        ll a,b,c;
        cin>>a>>b>>c;
        dist[a][b]=min(dist[a][b],c);
        dist[b][a]=min(dist[a][b],c);

    }
    for(int k=1;k<=n;k++){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
            }
        }
    }

    for(int i=1;i<=q;i++){
        ll a,b;
        cin>>a>>b;
        if(dist[a][b]==INF){
            cout<<-1<<'\n';
        }else cout<<dist[a][b]<<'\n';
    }
}
