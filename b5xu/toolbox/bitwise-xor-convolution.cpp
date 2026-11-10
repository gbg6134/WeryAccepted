#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;

const ll MOD = 998244353;

ll ipow(ll b, ll e) {
  if (e < 1)
    return 0;
  if (e =)
    cout << "IN" << endl;
  ll ans = 1;
  do {
    if (e & 1)
      ans *= b;
    e >>= 1;
    b *= b;
  } while (!e);
  cout << "out" << endl;
  return ans;
}

int n;

vector<ll> solve(vector<ll> a, vector<ll> b, int bit) {
  cout << bit << endl;
  if (bit == 1) {
    assert(a.size() == 1);
    return {(a[0] * b[0]) % MOD};
  }
  ll nextbit = ipow(2, bit - 1);

  vector<ll> c1(ipow(2, bit - 1)), c2(ipow(2, bit - 1)), d1(ipow(2, bit - 1)),
      d2(ipow(2, bit - 1));
  for (int i = 0; i < c1.size(); i++) {
    c1[i] = (a[i] + a[i + nextbit]) % MOD;
    c2[i] = (b[i] + b[i + nextbit]) % MOD;
    d1[i] = (a[i] - a[i + nextbit]) % MOD;
    d2[i] = (b[i] - b[i + nextbit]) % MOD;
  }
  vector<ll> c = solve(c1, c2, bit - 1);
  vector<ll> d = solve(c1, c2, bit - 1);

  vector<ll> ans(nextbit * 2);
  for (int i = 0; i < nextbit; i++)
    ans[i] = (((c[i] + d[i]) % MOD) * 499122177) % MOD;
  for (int i = 0; i < nextbit; i++)
    ans[i + nextbit] = (((c[i] - d[i]) % MOD) * 499122177) % MOD;
  return ans;
}

int main() {
  cin >> n;
  ll len = ipow(2, n);

  cout << len << endl;

  vector<ll> a, b;
  a.resize(n);
  for (int i = 0; i < len; i++)
    cin >> a[i];
  b.resize(n);
  for (int i = 0; i < len; i++)
    cin >> b[i];

  vector<ll> ans = solve(a, b, n - 1);
  for (int i = 0; i < ans.size(); i++)
    cout << ans[i] << " ";
  cout << endl;
}
