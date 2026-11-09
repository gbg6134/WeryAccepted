#include<bits/stdc++.h>
using namespace std;
#define LL long long 
#define i128 __int128
mt19937_64 rd(time(0));

LL n;

i128 ksm(i128 x, i128 y, LL mod)
{
    i128 res = 1;
    x %= mod;
    while(y)
    {
        if(y & 1) res = res * x % mod;
        y >>= 1;
        x = x * x % mod;
    }
    return res;
}

bool miller_rabin(LL n)
{
    if(n <= 1 || n > 2 && ~n & 1) return false;
    LL m = n - 1, k = 0;
    while(~m & 1) m >>= 1, k++;
    vector<int> s = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for(int a : s)
    {
        if(a >= n) break;
        i128 x = ksm(a, m, n), y;
        for(int i = 1; i <= k; i++, x = y)
        {
            y = x * x % n;
            if(y == 1 && x != 1 && x != n - 1) return false;
        }
        if(x != 1) return false;
    }
    return true;
}

vector<LL> p;
void pollard_rho(LL n)
{
	if(n <= 1) return;
    if(miller_rabin(n))
    {
        p.push_back(n);
        return;
    }
    LL d = rd() % (n - 1) + 1;
    auto f = [&](LL x)
    {
        return ((i128)x * x + d) % n; 
    };
    LL x = rd() % (n - 1) + 1, y = f(x), t = 1;
    while(t == 1)
    {
        for(int i = 1; i <= 32; i++)
        {
            while(x == y)
                d = rd() % (n - 1) + 1, x = rd() % (n - 1) + 1, y = f(x);
            if((i128)t * abs(x - y) % n == 0) break;
            t = (i128)t * abs(x - y) % n;
            x = f(x), y = f(f(y));
        }
        t = __gcd(n, t);
    }
    pollard_rho(t), pollard_rho(n / t);
}

int main()
{
    int T;
    cin >> T;
    while(T--)
    {
        cin >> n;
        if(miller_rabin(n))
        {
            puts("Prime");
            continue;
        }

        p.clear();
        pollard_rho(n);
        LL ans = 0;
        for(LL i : p) ans = max(ans, i);
        cout << ans << endl;
    }
    return 0;
}
