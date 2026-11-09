//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
using namespace std;
 

template <class T>
using Tree =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,a[MAXN],ans;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    Tree<ll> X;//can modify to eg pair<ll,ll> as well

    for(int i=1;i<=n;i++){
        cin>>a[i];
        X.insert(i);
    }

    for(int i=1;i<=n;i++){
        ll x;
        cin>>x;
        x--;
        cout<<a[*X.find_by_order(x)]<<' ';
        X.erase(*X.find_by_order(x));
    }

    //X.order_of_key(0) first one
    cout<<'\n';
}
