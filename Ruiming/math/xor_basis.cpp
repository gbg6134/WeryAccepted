//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans,len=1,bits=64;
bool zero;

void get_basis(){
    for(int i=bits;i>=0;i--){
        for(int j=len;j<=n;j++){
            if(a[j]&(1LL<<(i-1))){
                swap(a[len],a[j]);
                break;
            }
        }

        if(a[len]&(1LL<<(i-1))){
            for(int j=1;j<=n;j++){
                if(j==len) continue;

                if(a[j]&(1LL<<(i-1))){
                    a[j]^=a[len];
                }
            }
            len++;
        }
    }
    len--;
    zero=(len!=n);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    for(int i=1;i<=n;i++) cin>>a[i];
    
    get_basis();

    sort(a+1,a+len+1);

    for(int i=1;i<=len;i++) ans^=a[i];
    cout<<ans<<'\n';
}
