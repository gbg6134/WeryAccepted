//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=16;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],a[MAXN],m[MAXN],ans[MAXN],k,temp;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;


    for(int i=1;i<=n;i++){
        cin>>m[i]>>a[i];
    }

    ans[1]=a[1];
    for(int i=2;i<=n;i++){
        for(ll j=0;j<=m[i]-1;j++){
            if(((j*m[i-1]+ans[i-1])%m[i])==a[i]){
                ans[i]=j*m[i-1]+ans[i-1];
                break;
            }            
        }
        m[i]*=m[i-1];
    }
    cout<<ans[n]<<'\n';
}
