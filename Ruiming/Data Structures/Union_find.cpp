#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN=106;
ll s[MAXN],n,sz[MAXN];

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

int main(){
    cin>>n;
    for(int i=1;i<=n;i++){
        s[i]=i;
        sz[i]=1;
    }

    for(int i=1;i<=n-1;i++){
        merge_set(i,i+1);
    }
}
