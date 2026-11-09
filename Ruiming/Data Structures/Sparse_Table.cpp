//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=506;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,st[MAXN][26],back[MAXN][26],q,st1[MAXN][26],back1[MAXN][26];
 
 
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>q;
    for(int i=0;i<=20;i++){
        for(int j=0;j<MAXN;j++){
            st[j][i]=INF;
            back[j][i]=INF;
        }
    }
    
    for(int i=1;i<=n;i++){
        cin>>st[i][0];
        st1[i][0]=st[i][0];
        back[i][0]=st[i][0];
        back1[i][0]=st[i][0];
    }

    for(int i=1;i<=20;i++){
        for(int j=1;j<=n;j++){
            if(j+(1<<(i-1))<MAXN){
                st[j][i]=min(st[j][i-1],st[j+(1<<(i-1))][i-1]);
                st1[j][i]=max(st1[j][i-1],st1[j+(1<<(i-1))][i-1]);
            }

            if(j>=(1<<(i-1))){//can use st[j-(1<<(i-1))]
                back[j][i]=min(back[j][i-1],back[j-(1<<(i-1))][i-1]);
                back1[j][i]=max(back1[j][i-1],back1[j-(1<<(i-1))][i-1]);
            }
        }
    }
 
    for(int i=1;i<=q;i++){
        ll a,b,res=INF;
        cin>>a>>b;
        
        ll k=log2(b-a+1);
        ll maxi=max(st1[a][k],back1[b][k]);
        //ll maxi=max(st1[a][k],st1[b-(1<<k)+1][k]);
        ll mini=min(st[a][k],back[b][k]);
        cout<<maxi-mini<<'\n';
    }
}
