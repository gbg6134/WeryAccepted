#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef __int128 i128;
ld eps = 1e-9;

ll gcd(ll a,ll b)
{
	ll az=__builtin_ctz(a),bz=__builtin_ctz(b);
	ll z=min(az,bz);
	ll dif;
	b>>=bz;
	while(a)
	{
		a>>=az;
		dif=b-a;
		az=__builtin_ctz(dif);
		if(a<b) b=a;
		if(dif<0) a=-dif;
		else a=dif;
	}
	return b<<z;
}
