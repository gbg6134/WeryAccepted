//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll MOD=1e9+7;
const ll INF=1e18;
const ll MAXN=2006;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,k,m;

struct matrix{
    ll row,col;
    bitset<MAXN> m[MAXN]={};
};


//A bit matrix 0/1
void xor_gauss(matrix& A){
    ll m=A.row;
    ll n=A.col;

    for(int i=1;i<=n-1;i++){
        ll maxi=i;

        //find pivot row
        for(int j=1;j<=m;j++){
            if(j<i&&A.m[j][j]>0){
                continue;
            }
            if(A.m[j][i]==1){
                swap(A.m[j],A.m[i]);
                k=max(k,(ll)j);
                break;
            }
        }
        
        if(A.m[i][i]==1){
            for(int j=1;j<=m;j++){
                if(i!=j&&A.m[j][i]==1){
                    A.m[j]^=A.m[i];
                }
            }
        }else{
            k=m+1;
            return;
        }
    }

    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m;

    matrix A;
    A.row=m;
    A.col=n+1;

    for(int i=1;i<=m;i++){
        string s;
        cin>>s;
        for(int j=1;j<=n;j++){
            if(s[j-1]=='1') A.m[i][j]=1;
        }
        ll x;
        cin>>x;
        if(x) A.m[i][n+1]=1;
    }

    xor_gauss(A);

    if(k==m+1){
        cout<<"Cannot Determine"<<'\n';
    }else{
        cout<<k<<'\n';
        for(int i=1;i<=n;i++){
            if(A.m[i][n+1]==1){
                cout<<"?y7M#"<<'\n';
            }else cout<<"Earth"<<'\n';
        }
    }
}
