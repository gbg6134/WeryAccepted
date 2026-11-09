//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)
 
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const double alpha=0.7;

struct Node{
    ll ls,rs;
    ll val;
    ll tot;
    ll size;
    ll del;
}t[MAXN];

ll order[MAXN],cnt;
ll tree_stack[MAXN],top;
ll root;

void inorder(ll u){
    if(!u) return;
    inorder(t[u].ls);
    if(t[u].del) order[++cnt]=u;
    else tree_stack[++top]=u;
    inorder(t[u].rs);
}

void Initnode(ll u){
    t[u].ls=t[u].rs=0;
    t[u].size=t[u].tot=t[u].del=1;
}

void Update(ll u){
    t[u].size=t[t[u].ls].size+t[t[u].rs].size+1;
    t[u].tot=t[t[u].ls].tot+t[t[u].rs].size+1;
}

void build(ll l,ll r,ll &u){
    ll mid=(l+r)>>1;
    u=order[mid];
    if(l==r){
        Initnode(u);
        return;
    }

    if(l<mid) build(l,mid-1,t[u].ls);
    if(l==mid) t[u].ls=0;
    build(mid+1,r,t[u].rs);
    Update(u);
}

void rebuild(ll &u){
    cnt=0;
    inorder(u);
    if(cnt) build(1,cnt,u);
    Update(u);
}

bool notbalance(ll &u){
    if((double)t[u].size*alpha<=(double)max(t[t[u].ls].size,t[t[u].rs].size)) return true;
    return false;
}

void Insert(ll &u,ll x){
    if(!u){
        u=tree_stack[top--];
        t[u].val=x;
        Initnode(u);
        return;
    }
    t[u].size++;
    t[u].tot++;
    if(t[u].val>x) Insert(t[u].ls,x);
    else Insert(t[u].rs,x);
    if(notbalance(u)) rebuild(u);
}

ll Rank(ll u,ll x){
    if(u==0) return 0;
    if(x>t[u].val) return t[t[u].ls].size+t[u].del+Rank(t[u].rs,x);
    return Rank(t[u].ls,x);
}

ll kth(ll k){
    ll u=root;
    while(u){
        if(t[u].del&&t[t[u].ls].size+1==k) return t[u].val;
        else if(t[t[u].ls].size>=k) u=t[u].ls;
        else{
            k-=t[t[u].ls].size+t[u].del;
            u=t[u].rs;
        }
    }
    return t[u].val;
}

void Del_k(ll &u, ll k){
    t[u].size--;
    if(t[u].del&&t[t[u].ls].size+1==k){
        t[u].del=0;
        return;
    }
    if(t[t[u].ls].size+t[u].del>=k) Del_k(t[u].ls,k);
    else Del_k(t[u].rs,k-t[t[u].ls].size-t[u].del);
}

void Del(ll x){
    Del_k(root,Rank(root,x)+1);
    if(t[root].tot*alpha>=t[root].size){
        rebuild(root);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    for(int i=MAXN-1;i>=1;i--){
        tree_stack[++top]=i;
    }
    ll q;
    cin>>q;
    while(q--){
        ll opt,x;
        cin>>opt>>x;
        if(opt==1){
            Insert(root,x);
        }
        if(opt==2){
            Del(x);
        }
        if(opt==3){
            cout<<Rank(root,x)+1<<'\n';
        }
        if(opt==4){
            cout<<kth(x)<<'\n';
        }
        if(opt==5){
            cout<<kth(Rank(root,x))<<'\n';
        }
        if(opt==6){
            cout<<kth(Rank(root,x+1)+1)<<'\n';
        }
    }
}
