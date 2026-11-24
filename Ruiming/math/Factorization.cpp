//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;
 
const ll INF=1e18;
const ll MAXN=36;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans;
map<ll,ll> fac[MAXN];

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    for(int i=1;i<MAXN;i++){
        ll temp=i;
        for(int j=2;j<=sqrt(temp+0.5);j++){
            while(temp%j==0){
                fac[i][j]++;
                temp/=j;
            }
        }
        if(temp>1){
            fac[i][temp]++;
        }
    }

    for(int i=1;i<=n;i++){
        cin>>a[i];
    }


}
