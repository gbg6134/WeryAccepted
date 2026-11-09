//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],m,degree[MAXN],one,mone,other,idx;
multiset<ll> con[MAXN];

void euler(int u,vector<int> &temp){
    while(!con[u].empty()){
        ll a=*con[u].begin();

        con[u].erase(con[u].find(a));

        euler(a,temp);
        temp.push_back(a);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m;

    for(int i=1;i<=m;i++){
        int a,b;
        cin>>a>>b;
        con[a].insert(b);

        degree[a]++;
        degree[b]--;
    }

    for(int i=1;i<=n;i++){
        if(abs(degree[i])>1){
            cout<<"No"<<'\n';
            return 0;
        }

        if(degree[i]==1){
            one++;
            idx=i;
        }
        if(degree[i]==-1) mone++;
    }

    vector<int> t;

    if(one==0||mone==0){
        euler(1,t);
        t.push_back(1);
        reverse(t.begin(),t.end());
        for(int i=0;i<t.size();i++) cout<<t[i]<<' ';
        cout<<'\n';
    }else if(one==1||mone==1){
        euler(idx,t);
        t.push_back(idx);
        reverse(t.begin(),t.end());
        for(int i=0;i<t.size();i++) cout<<t[i]<<' ';
        cout<<'\n';
    }else{
        cout<<"No"<<'\n';
    }
}
