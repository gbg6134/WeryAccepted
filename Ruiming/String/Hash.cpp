//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
 
const ll INF=1e15;
const ll MOD=1e9+7;
const ll mul=131;
const ll MAXN=106,MAXM=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans,m,k,dp[2][MAXN][MAXM],g[2][MAXN][MAXM];
ull pot[MAXN];
string s,t;

vector<ull> Bhash(string s){
    vector<ull> res;

    res.push_back(0);
    ull now=0;
    for(int i=0;i<s.size();i++){
        now=now*mul+(s[i]-'a');
        res.push_back(now);
    }
    return res;
}

ull get_hash(vector<ull> &x,ll a,ll b){
    if(a>b) swap(a,b); //a<=b

    ll res=x[b]-x[a-1]*pot[b-a+1];
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n>>m>>k;
    cin>>s>>t;
   
    pot[0]=1;
    for(int i=1;i<=n;i++){
        pot[i]=pot[i-1]*mul;
    }

    vector<ull> s_hash=Bhash(s);
    vector<ull> t_hash=Bhash(t);

    
    cout<<get_hash(s_hash,1,3)<<'\n';
    cout<<get_hash(s_hash,4,6)<<'\n';
    cout<<get_hash(t_hash,1,3)<<'\n';
      
}
