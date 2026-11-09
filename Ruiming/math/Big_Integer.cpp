#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)

string Bigadd(string x,string y){
    //if(x[0]!='-'&&y[0]=='-') return Bigsub(x,y.substr(1,y.size()-1));
    //if(x[0]=='-'&&y[0]!='-') return Bigsub(y,x.substr(1,x.size()-1));
    if(x[0]=='-'&&y[0]=='-') return "-"+Bigadd(y.substr(1,y.size()-1),x.substr(1,x.size()-1));
    
    int s=max(x.size(),y.size()),a[s+6],b[s+6];
    int res[s+6];
    memset(res,0,sizeof(res));
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));

    for(int i=0;i<x.size();i++) a[i]=x[x.size()-1-i]-'0';
    for(int i=0;i<y.size();i++) b[i]=y[y.size()-1-i]-'0';

    for(int i=0;i<s;i++){
        res[i]+=a[i]+b[i];
        res[i+1]=res[i]/10;
        res[i]%=10;
    }
    string ans="";
    bool first=false;
    for(int i=s+1;i>=0;i--){
        if(res[i]==0&&!first) continue;
        else{
            ans+=res[i]+'0';
            first=true;
        }
    }
    if(ans=="") return "0";
    return ans;
}

string Bigsub(string x,string y){
    if(x[0]!='-'&&y[0]=='-') return Bigadd(x,y.substr(1,y.size()-1));
    if(x[0]=='-'&&y[0]!='-') return '-' + Bigadd(x.substr(1,x.size()-1),y);
    if(x[0]=='-'&&y[0]=='-') return Bigsub(y.substr(1,y.size()-1),x.substr(1,x.size()-1));
    

    bool neg=false;
    if(y.size()>x.size()){
        swap(x,y);
        neg=true;
    }

    if(y.size()==x.size()&&y>x){
        swap(x,y);//ensures y>x
        neg=true;
    }

    int s=max(x.size(),y.size()),a[s+6],b[s+6];
    int res[s+6];
    memset(res,0,sizeof(res));
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));

    for(int i=0;i<x.size();i++) a[i]=x[x.size()-1-i]-'0';
    for(int i=0;i<y.size();i++) b[i]=y[y.size()-1-i]-'0';

    for(int i=0;i<s;i++){
        res[i]+=a[i]-b[i];
        while(res[i]<0){
            res[i]+=10;
            res[i+1]--;
        }
    }
    string ans="";
    if(neg) ans+="-";
    bool first=false;
    for(int i=s+1;i>=0;i--){
        if(res[i]==0&&!first) continue;
        else{
            ans+=res[i]+'0';
            first=true;
        }
    }
    if(ans==""||ans=="-") return "0";
    return ans;
}

string Bigmul(string x,string y){// multiplication not done like this
    if(x=="0"||y=="0") return "0";

    if(x[0]!='-'&&y[0]=='-') return "-"+Bigmul(x,y.substr(1,y.size()-1));
    if(x[0]=='-'&&y[0]!='-') return "-"+Bigmul(y,x.substr(1,x.size()-1));
    if(x[0]=='-'&&y[0]=='-'){
        x=x.substr(1,x.size()-1);
        y=y.substr(1,y.size()-1);
    }
    
    int s=max(x.size(),y.size()),a[2*s+6],b[2*s+6];
    int res[2*s+6];
    memset(res,0,sizeof(res));
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));

    for(int i=0;i<x.size();i++) a[i]=x[x.size()-1-i]-'0';
    for(int i=0;i<y.size();i++) b[i]=y[y.size()-1-i]-'0';


    for(int i=0;i<x.size();i++){
        for(int j=0;j<y.size();j++){
            res[i+j]+=b[j]*a[i];
            res[i+j+1]+=res[i+j]/10;//will not increase 2.
            res[i+j]%=10;
        }
    }

    string ans="";
    bool first=false;
    for(int i=2*s+5;i>=0;i--){
        if(res[i]==0&&!first) continue;
        else{
            ans+=res[i]+'0';
            first=true;
        }
    }
    if(ans=="") return "0";
    return ans;
}

string Bighalf(string x){
    int s=x.size(),a[s+6];
    int res[s+6];
    memset(res,0,sizeof(res));
    memset(a,0,sizeof(a));

    for(int i=0;i<x.size();i++) a[i]=x[i]-'0';

    for(int i=0;i<x.size();i++){
        res[i]+=a[i]/2;
        if(a[i]%2){
            a[i+1]+=10;
        }
    }

    string ans="";
    bool first=false;
    for(int i=0;i<s;i++){
        if(res[i]==0&&!first) continue;
        else{
            ans+=res[i]+'0';
            first=true;
        }
    }
    if(ans=="") return "0";
    return ans;
}

bool Scompare(string x,string y){
    if(x.size()<y.size()) return true;
    if(x.size()>y.size()) return false;
    return x<y;
}

bool Ecompare(string x,string y){
    if(x.size()<y.size()) return true;
    if(x.size()>y.size()) return false;
    return x<=y;
}

string BigMod(string x,string m){//can get Bigdiv in the same way
    string start="0",end="1";
    for(int i=1;i<=x.size()+6;i++) end+="0";

    while(Scompare(start,end)){
        string temp=Bigadd(end,"1");
        string mid=Bigadd(start,Bighalf(Bigsub(temp,start)));
        if(Ecompare(Bigmul(mid,m),x)){
            start=mid;
        }else{
            end=Bigsub(mid,"1");
        }
    }
    string res=Bigsub(x,Bigmul(start,m));
    return res;
}

ll BigModSmall(string a,ll mod){
    ll x=0;
    for(int i=0;i<a.size();i++){
        x=((10*x)+(a[i]-'0'))%mod;
    }
    return x;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string a,b;
    cin>>a>>b;
    string c=BigMod(a,b);
    cout<<c<<'\n';
}
