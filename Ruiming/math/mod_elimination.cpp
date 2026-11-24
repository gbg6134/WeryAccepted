//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;

const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,k,MOD;

struct matrix{
    ll row,col;
    ll m[MAXN][MAXN];
};

ll res[MAXN],inv[MAXN];

void lin_inv(){
    inv[1]=1;
    for(int i=2;i<MOD;i++){
        inv[i]=(MOD-inv[MOD%i]*(MOD/i)%MOD);
    }
}

//A merged matrix
void gauss_elimination(matrix& A,ll *res){
    memset(res,0,sizeof(res));

    ll n=A.row;
    if(A.col!=n+1){
        for(int i=1;i<=n;i++){
            res[i]=INF;
        }
        return;
    }

    for(int i=1;i<=n;i++){
        if(A.m[i][i]==0){
            for(int j=1;j<=n;j++){
                //even before not pivot row
                if(j<i&&A.m[j][j]!=0) continue;

                if(A.m[j][i]!=0){
                    swap(A.m[i],A.m[j]);
                    break;
                }
            }
        }

        if(A.m[i][i]!=0){
            for(int j=1;j<=n;j++){
                if(i!=j&&A.m[j][i]!=0){
                    ll d=__gcd(A.m[i][i],A.m[j][i]);
                    ll a=A.m[i][i]/d,b=A.m[j][i]/d;
                    for(int k=1;k<=n+1;k++){
                        A.m[i][k]*=b;
                        A.m[i][k]%=MOD;

                        A.m[j][k]*=a;
                        A.m[j][k]%=MOD;
                    }

                    for(int k=i;k<=n+1;k++){
                        A.m[j][k]-=A.m[i][k];
                        A.m[j][k]=(A.m[j][k]+MOD)%MOD;
                    }
                }
            }
        }
    }

    lin_inv();

    for(int i=1;i<=n;i++){
        if(A.m[i][i]!=0){
            bool flag=0;
            for(int j=i+1;j<=n;j++){
                if(A.m[i][j]!=0){
                    flag=1;
                    break;
                }
            }

            if(!flag){
                A.m[i][n+1]=(A.m[i][n+1]*inv[A.m[i][i]])%MOD;
                A.m[i][i]=1;
            }
        }
    }

    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>MOD;

    matrix A;
    A.row=n;
    A.col=n+1;

    for(int i=1;i<=n;i++){
        for(int j=1;j<=n+1;j++){
            cin>>A.m[i][j];
        }
    }

    gauss_elimination(A,res);
    
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n+1;j++) cout<<A.m[i][j]<<' ';
        cout<<'\n';
    }
}
