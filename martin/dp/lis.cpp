#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll lis(vector<ll> &arr){ // longest increasing subsequence in O(n log n)
    vector<ll> con;
    for(auto e : arr){
        if(con.empty()) con.push_back(e);
        else if(con[con.size()-1] < e) con.push_back(e);
        else{
            ll idx = lower_bound(con.begin(), con.end(), e) - con.begin();
            con[idx] = e;
        }
    }
    return con.size();
}

int main(){
    return 0;
}
