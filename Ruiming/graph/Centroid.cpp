//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)
 
const ll MAXN=506;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int Link[MAXN],cnt,n,mini=99999999,subtree[MAXN];
vector<int> cand;
bool already[MAXN];

struct node{
    int end,next;
}Edge[2*MAXN];

void insert(int x,int y){
    int temp=Link[x];
    cnt++;
    Link[x]=cnt;
    Edge[cnt].next=temp;
    Edge[cnt].end=y;
}

void dfs(int now){
    int summ=1,maxi=0;
    for(int i=Link[now];i;i=Edge[i].next){
        if(!already[Edge[i].end]){
            already[Edge[i].end]=1;
            dfs(Edge[i].end);
            already[Edge[i].end]=0;
            summ+=subtree[Edge[i].end];
            maxi=max(subtree[Edge[i].end],maxi);
        }
    }
    subtree[now]=summ;
    int last=n-summ;
    maxi=max(maxi,last);
    if(maxi<=mini){
        if(maxi==mini){
            cand.push_back(now);
        }else{
            mini=maxi;
            cand.clear();
            cand.push_back(now);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin>>n;
    for(int i=1;i<=n-1;i++){
        int a,b;
        cin>>a>>b;
        insert(a,b);
        insert(b,a);
    }

    already[1]=1;
    dfs(1);

    sort(cand.begin(),cand.end());
    for(auto i:cand) cout<<i<<' ';
    cout<<'\n';
}
