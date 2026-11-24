//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;
 
const ll MOD=1e9+7;
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,ans,k;

ld eps=1e-7;//MIGHT GET WA PGA EPS TOO SMALL!!!!!!!!!!!!!!!!!!!!!!!!!!!!

ll ldcmp(ld x,ld y){
    if(fabs(x-y)<eps) return 0;
    else return x<y?-1:1;
    //is x<y return -1, x>y return 1;
}

struct matrix{
    ld row,col;
    ld m[MAXN][MAXN];
};

ld res[MAXN];

//A merged matrix
void gauss_elimination(matrix& A,ld *res){
    memset(res,0,sizeof(res));

    ll n=A.row;
    if(A.col!=n+1){
        for(int i=1;i<=n;i++){
            res[i]=INF;
        }
        return;
    }

    for(int i=1;i<=n;i++){
        ll maxi=i;

        //find pivot row
        for(int j=1;j<=n;j++){
            if(j<i&&ldcmp(A.m[j][j],0)>0){
                continue;
            }
            if(ldcmp(abs(A.m[j][i]),abs(A.m[maxi][i]))==1){
                maxi=j;
            }
        }
        swap(A.m[maxi],A.m[i]);

        if(ldcmp(A.m[i][i],0)!=0){
            ld tmp=A.m[i][i];
            for(int j=i;j<=n+1;j++){
                A.m[i][j]/=tmp;
            }

            for(int j=1;j<=n;j++){
                if(i!=j){
                    ld rate=A.m[j][i]/A.m[i][i];

                    for(int k=i;k<=n+1;k++){
                        A.m[j][k]-=A.m[i][k]*rate;
                    }
                }
            }
        }
    }

    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;

    matrix A;
    A.row=n;
    A.col=n+1;

    for(int i=1;i<=n;i++){
        for(int j=1;j<=n+1;j++){
            cin>>A.m[i][j];
        }
    }

    gauss_elimination(A,res);
    
    ll sign=1;
    for(int i=1;i<=n;i++){
        if(ldcmp(abs(A.m[i][i]),0)==0&&ldcmp(abs(A.m[i][n+1]),0)!=0){
            sign=-1;
            break;
        }

        if(ldcmp(abs(A.m[i][i]),0)==0){
            sign=0;
        }
    }

    if(sign==1){
        cout<<fixed<<setprecision(2);
        for(int i=1;i<=n;i++){
            cout<<"x"<<i<<"="<<A.m[i][n+1]<<'\n';
        }
    }else cout<<sign<<'\n';
}
