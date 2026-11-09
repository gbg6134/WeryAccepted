//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef vector<ll> vi;
typedef vector<vi> matrix;
#define rep(i, a, b) for(int i = a; i < b; i++)
 
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

ll n;

struct node{
    ll in,out,id;
}lst[MAXN];

class Compare {
public:
    bool operator()(node a, node b)
    {
        return a.in<b.in;
    }
};

priority_queue<node,vector<node>,Compare> Q;

///////////////////////////////////////////////// Way 2
#include<iostream>
#include<queue>
using namespace std;
struct node{
    int i,j,num,f;
};//结构体存点
struct cmp1{
    bool operator()(node x,node y){
        return x.num>y.num;
    }
};//优先队列小的在前面
priority_queue<node,vector<node>,cmp1>q;//stl大法好
