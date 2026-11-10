#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
#define fi first
#define se second
#define out(val) cout << val << '\n'
#define out2(val1, val2) cout << val1 << " " << val2 << '\n'
#define dist(name, mi, ma)                                                     \
  uniform_int_distribution<mt19937::result_type> name(mi, ma)
#define dbg(expr)                                                              \
  if (true) {                                                                  \
    cerr << "[" << __FUNCTION__ << ", " << __LINE__ << "] " << #expr << ": "   \
         << expr << endl                                                       \
         << flush;                                                             \
  }

random_device dev;
mt19937 rnd(dev());

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  dist(dinm, 1, 1000);
  ll n = dinm(rnd);
  ll m = dinm(rnd);
  out2(n, m);

  return 0;
}
