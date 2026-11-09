#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

vector<ll> kmp(string txt, string pat){ // returns all indices of occurences of pat within text in O(len(txt) + len(pat))
    ll n = txt.size(), m = pat.size(), len;
    vector<ll> lps(m, 0);
    for(ll i = 1; i < m; i++){
        len = lps[i-1];
        while(pat[i] != pat[len] && len > 0){
            len = lps[len-1];
        }
        if(pat[i] == pat[len]) len++;
        lps[i] = len;
    }
    vector<ll> ans;
    len = 0;
    for(ll i = 0; i < n; i++){
        while(txt[i] != pat[len] && len > 0){
            len = lps[len-1];
        }
        if(txt[i] == pat[len]) len++;
        if(len == m){
            ans.push_back(i - len + 1);
            len = lps[len-1];
        }
    }
    return ans;
}

int main(){
    return 0;
}
