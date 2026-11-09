#include <bits/stdc++.h>

using namespace std;

const int MAXN=106;
int s[MAXN],n,height[MAXN],d[MAXN];

int find_set(int x){
    int t=s[x];
    if(x!=s[x]){
        s[x]=find_set(s[x]);
    }//modify the weight
    d[x]+=d[t];
    return s[x];
}

void merge_set(int x,int y){
    int a=find_set(x);
    int b=find_set(y);

    if(height[a]==height[b]){
        height[a]++;
        s[b]=a;
    }else{
        if(height[a]>height[b]){
            s[b]=a;
        }else{
            s[a]=b;
        }
    }
}

int main(){
    cin>>n;
    for(int i=1;i<=n;i++){
        s[i]=i;
        height[i]=1;
    }

    for(int i=1;i<=n-1;i++){
        merge_set(i,i+1);
    }
}