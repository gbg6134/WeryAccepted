// ==================== Binary Search & Ternary Search templates ====================
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Find smallest x in [lo, hi] such that pred(x) is true (pred is monotonic false->true)
ll binarySearch(ll lo, ll hi, function<bool(ll)> pred) {
    hi++; // search space [lo, hi), hi is "found nothing" sentinel
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (pred(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo; // == original hi+1 if never true
}

// Ternary search for the minimum of a unimodal (convex) function on [lo, hi], doubles
double ternarySearchMin(double lo, double hi, function<double(double)> f, int iters = 200) {
    for (int i = 0; i < iters; i++) {
        double m1 = lo + (hi - lo) / 3;
        double m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2; else lo = m1;
    }
    return (lo + hi) / 2;
}

// Integer ternary search for minimum of a unimodal function on [lo, hi]
ll ternarySearchMinInt(ll lo, ll hi, function<ll(ll)> f) {
    while (hi - lo > 2) {
        ll m1 = lo + (hi - lo) / 3;
        ll m2 = hi - (hi - lo) / 3;
        if (f(m1) <= f(m2)) hi = m2; else lo = m1;
    }
    ll best = f(lo), bestX = lo;
    for (ll x = lo + 1; x <= hi; x++) if (f(x) < best) { best = f(x); bestX = x; }
    return bestX;
}
