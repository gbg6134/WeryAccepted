//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
using namespace std;
 
typedef __int128 ll;

__int128 read128() {
    __int128 x = 0; int sign = 1; char c = getchar();
    while (c == ' ' || c == '\n') c = getchar();
    if (c == '-') { sign = -1; c = getchar(); }
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return sign * x;
}

void print128(__int128 x) {
    if (x < 0) { putchar('-'); x = -x; }
    if (x > 9) print128(x / 10);
    putchar('0' + x % 10);
}
 
const ll INF=1e18;
const ll MAXN=106;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,t,ans;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    n=read128();
    print128(n);
}
