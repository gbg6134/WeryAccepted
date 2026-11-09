//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],q;

ll tree[4*MAXN],tag[4*MAXN];

inline ll lc(ll p){return p<<1;}//left child
inline ll rc(ll p){return p<<1|1;}//right child

void push_up(ll p){
    tree[p]=tree[lc(p)]+tree[rc(p)];//update p
}

void build(ll root,ll l,ll r){
    tag[root]=0;//initialize tag
    if(l==r){
        tree[root]=lst[l];
        return;
    }

    ll mid=(l+r)>>1;
    build(lc(root),l,mid);//recurse left
    build(rc(root),mid+1,r);//recurse right
    push_up(root);
}

inline void f(ll p,ll l,ll r,ll k){
    tag[p]+=k;//update tag
    tree[p]+=k*(r-l+1);//update node
}

inline void push_down(ll p,ll l,ll r){
    ll mid=(l+r)>>1;
    f(lc(p),l,mid,tag[p]);//recurse left,not exceding?
    f(rc(p),mid+1,r,tag[p]);//recurse right
    tag[p]=0;
}

void update(ll nl,ll nr,ll l,ll r,ll p,ll k){
    if(nl<=l&&r<=nr){//entire segment with in then process
        tree[p]+=k*(r-l+1);
        tag[p]+=k;
        return;
    }
    push_down(p,l,r);//otherwise divide smaller
    ll mid=(l+r)>>1;

    if(nl<=mid) update(nl,nr,l,mid,lc(p),k);//recurse left
    if(nr>mid) update(nl,nr,mid+1,r,rc(p),k);//recurse right

    push_up(p);
}

ll query(ll q_l,ll q_r,ll l,ll r,ll p){
    ll res=0;
    if(q_l<=l&&r<=q_r) return tree[p];
    ll mid=(l+r)>>1;
    push_down(p,l,r);
    if(q_l<=mid) res+=query(q_l,q_r,l,mid,lc(p));
    if(q_r>mid) res+=query(q_l,q_r,mid+1,r,rc(p));

    return res;    
}


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>q;

    for(int i=1;i<=n;i++) cin>>lst[i];

    build(1,1,n);

    while(q--){
        ll a;
        cin>>a;
        if(a==1){
            ll l,r,k;
            cin>>l>>r>>k;
            update(l,r,1,n,1,k);
        }else{
            ll x,y;
            cin>>x>>y;
            ll ans=query(x,y,1,n,1);
            cout<<ans<<'\n';
        }
    }
}
