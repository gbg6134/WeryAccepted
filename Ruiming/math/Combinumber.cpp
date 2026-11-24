//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)
 
const ll M=1000000007;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll c[MAXN][106],t,n,m,k,lst[MAXN],ans;
 
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    c[0][0]=1;
 
    for(int i=1;i<MAXN-1;i++){
        c[i][0]=1;
        if(i<106) c[i][i]=1;
        for(int j=1;j<min(i,105);j++){
            c[i][j]=c[i-1][j]+c[i-1][j-1];
            c[i][j]%=M;
        }
    }
}
