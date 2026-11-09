//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=406;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,phi[MAXN],ans=1;
bool p[MAXN];
vector<ll> primes;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    if(n==1) cout<<0<<'\n';
    else{
        phi[1]=1;
        for(int i=2;i<=n;i++){//modiying euler sieve
            if(!p[i]){
                primes.push_back(i);
                phi[i]=i-1;
            }

            for(int j=0;j<primes.size();j++){
                if(primes[j]*i>n) break;
                p[primes[j]*i]=1;
                if(i%primes[j]==0){
                    phi[i*primes[j]]=primes[j]*phi[i];
                    break;
                }else{
                    phi[i*primes[j]]=(primes[j]-1)*phi[i];
                }
            }
        }
    }
}
