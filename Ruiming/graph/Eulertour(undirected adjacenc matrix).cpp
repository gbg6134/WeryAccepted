//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN];
vector<ll> odd;
ll G[66][66],degree[66],rem[66][66];
string ans;

void euler(int u,vector<int> &temp){
    for(int i=1;i<66;i++){
        if(G[u][i]){
            G[u][i]--;
            G[i][u]--;
            euler(i,temp);
            temp.push_back(i);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    for(int i=1;i<=n;i++){
        int a,b;
        cin>>a>>b;
        G[a][b]++;
        G[b][a]++;
        degree[a]++;
        degree[b]++;
    }

    for(int i=1;i<66;i++) for(int j=1;j<66;j++) rem[i][j]=G[i][j];

    for(int i=1;i<66;i++){
        if(degree[i]%2){
            odd.push_back(i);
        }
    }

    vector<int> t;
    if(odd.size()==0){
        for(int i=1;i<66;i++){
            t.clear();
            euler(i,t);
            t.push_back(i);
            for(int i=1;i<66;i++) for(int j=1;j<66;j++) G[i][j]=rem[i][j];
        }
    }else if(odd.size()==2){
        t.clear();
        euler(odd[0],t);
        t.push_back(odd[0]);
        t.clear();
        for(int i=1;i<66;i++) for(int j=1;j<66;j++) G[i][j]=rem[i][j];
        euler(odd[1],t);
        t.push_back(odd[1]);
    }else cout<<"No Solution"<<'\n';

}
