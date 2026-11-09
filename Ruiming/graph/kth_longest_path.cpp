//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,m,k;
 
priority_queue<ll> best[MAXN];
 
ll Link[MAXN],cnt;
 
struct node{
    ll end,next,w;
}Edge[2*MAXN];
 
void insert(ll x,ll y,ll z){
    ll temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
    Edge[cnt].w=z;
}
 
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m>>k;
 
    for(int i=1;i<=m;i++){
        ll a,b,c;
        cin>>a>>b>>c;
        insert(a,b,c);
    }
 
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> Q;
    Q.push({0,1});
    best[1].push(0);
 
    while(!Q.empty()){
        pair<ll,ll> a=Q.top();
        Q.pop();
        if(a.first>best[a.second].top()) continue;
 
        for(int i=Link[a.second];i;i=Edge[i].next){
            ll temp=a.first+Edge[i].w;           
 
            if(best[Edge[i].end].size()<k){
                best[Edge[i].end].push(temp);
                Q.push({temp,Edge[i].end});
            }else{
                if(temp<best[Edge[i].end].top()){
                    best[Edge[i].end].pop();
                    best[Edge[i].end].push(temp);
                    Q.push({temp,Edge[i].end});
                }
            }
        }
    }
 
    vector<ll> ans;
    while(!best[n].empty()){
        ll a=best[n].top();
        ans.push_back(a);
        best[n].pop();
    }
 
    reverse(ans.begin(),ans.end());
    for(int i=0;i<k;i++) cout<<ans[i]<<' ';
    cout<<'\n';
}
