#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)
 
const ll MAXN=206;
ll n,ans;

int main(){
       vector<bool> already(n+1, 0);
       vector<ll> con[MAXN];
       vector<ll> inedge(n+1, 0);

       //dfs to gete the connections

       queue<ll> Q;
       for(int i=0;i<n;i++){
           if(inedge[i]==0) Q.push(i);
       }
       while(!Q.empty()){
           ll a=Q.front();
           Q.pop();
           already[a]=1;

           for(int i=0;i<con[a].size();i++){
               inedge[con[a][i]]--;
               if(inedge[con[a][i]]==0){
                   Q.push(con[a][i]);
               }
           }
       }
}
