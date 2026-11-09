#include <bits/stdc++.h>

using namespace std;

const int MAXN=106;

int Link[MAXN],cnt;

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