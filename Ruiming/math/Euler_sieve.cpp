#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN=106;

vector<ll> primes;
bool p[MAXN];
ll n,q;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin>>n>>q;

    for(ll i=2;i<=n;i++){
        if(!p[i]){
            primes.push_back(i);
        }

        for(ll j=0;j<primes.size();j++){
            if(primes[j]*i>n) break;
            p[primes[j]*i]=1;
            if(i%primes[j]==0) break;
        }
    }

    for(int i=1;i<=q;i++){
        ll a;
        cin>>a;
        cout<<primes[a-1]<<'\n';
    }
}
