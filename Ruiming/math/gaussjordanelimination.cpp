//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n;
double A[MAXN][MAXN],b[MAXN],ans[MAXN];
stack<pair<ll,ll>> swaped;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>A[i][j];
        }
        cin>>b[i];
    }

    ll col=1;
    for(int i=1;i<=n&&col<=n;i++,col++){//row
        while(A[i][col]==0&&col<=n){
            bool found=false;
            for(int j=i+1;j<=n;j++){
                if(A[j][col]!=0){
                    swap(A[j],A[i]);
                    swap(b[j],b[i]);//swap b as well
                    found=true;
                    break;
                }
            }
            if(!found) col++;
        }

        for(int j=i+1;j<=n;j++){
            if(A[i][col]==0) break;//excepet bug
            double fac=A[j][col]/A[i][col];
            for(int k=col;k<=n;k++){
                A[j][k]-=fac*A[i][k];
            }
            b[j]-=fac*b[i];
        }
    }

    bool no=false,p=false;
    for(int i=1;i<=n;i++){
        bool zero=true;

        for(int j=1;j<=n;j++){
            if(A[i][j]!=0){
                zero=false;
                break;
            }
        }
        p|=zero;

        if(zero&&b[i]!=0){
            no=true;
            break;
        }
    }

    if(p){
        if(no){
            cout<<-1<<'\n';
        }else cout<<0<<'\n';
    }else{
        for(int i=n;i>=1;i--){
            for(int j=i+1;j<=n;j++){
                b[i]-=ans[j]*A[i][j];
            }
            ans[i]=b[i]/=A[i][i];
        }
        for(int i=1;i<=n;i++){
            printf("x%d=%.2f\n",i,ans[i]);
        }
    }
}
