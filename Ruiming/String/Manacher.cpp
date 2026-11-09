//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,p[2*MAXN+6],ans,R,C;
char a[2*MAXN+6];
string s;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>s;

    a[0]='&';
    for(int i=0;i<s.size();i++){
        a[2*i+1]='#';
        a[2*i+2]=s[i];
    }
    a[2*s.size()+1]='#';
    a[2*s.size()+2]='^';

    for(int i=0;i<2*s.size()+2;i++){
        p[i]=1;
        if(p[2*C-i]>=C+p[C]-i){
            p[i]=C+p[C]-i;
        }else{
            p[i]=p[2*C-i];
        }

        while(a[i+p[i]]==a[i-p[i]]){
            p[i]++;
        }

        if(p[i]+i>R){
            R=p[i]+i;
            C=i;
        }

        ans=max(ans,p[i]-1);
    }
    cout<<ans<<'\n';
}
