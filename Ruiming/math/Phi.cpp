//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans;

ll phi(ll n) {//求欧拉函数
    ll ans = 1;
    for(ll i = 2; i * i <= n; ++ i) {
        if(n % i == 0) {
            n /= i;
            ans *= i - 1;
            while(n % i == 0) {
                n /= i;
                ans *= i;
            }
        }
    }
    if(n > 1)//优化，n>1表示分解剩下一个质数
        ans *= n - 1;
    return ans;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

}
