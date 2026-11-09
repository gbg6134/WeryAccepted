//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll MOD=1e9+7;
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,k;

struct matrix{
    ll row,col;
    ll m[MAXN][MAXN];
};

matrix operator * (const matrix &a,const matrix &b){
    //assuming they fits a.col==b.row

    matrix c;
    memset(c.m,0,sizeof(c.m));
    c.row=a.row;
    c.col=b.col;
    for(int i=1;i<=c.row;i++){
        for(int j=1;j<=c.col;j++){
            for(int k=1;k<=a.col;k++){
                c.m[i][j]+=(a.m[i][k]*b.m[k][j])%MOD;
                c.m[i][j]%=MOD;
            }
        }
    }
    return c;
}

matrix pow_matrix(matrix a,ll n){
    //assuming a is a square matrix
    matrix res;
    memset(res.m,0,sizeof(res.m));
    res.row=res.col=a.col;
    for(int i=1;i<MAXN;i++) res.m[i][i]=1;

    while(n){
        if(n&1) res=res*a;
        a=a*a;
        n/=2;
    }

    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>k;

    matrix A;
    A.row=A.col=n;

    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>A.m[i][j];
        }
    }

    matrix res=pow_matrix(A,k);
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cout<<res.m[i][j]<<' ';
        }
        cout<<'\n';
    }
}
