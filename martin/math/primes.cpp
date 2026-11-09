#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

vector<ll> findprimes(ll n){ // find all primes <= n in O(n log log n)
    if(n < 2) return {};
    vector<bool> isprime(n+1, 1);
    isprime[0] = 0; isprime[1] = 0;
    for(ll p = 2; p*p <= n; p++){
        if(isprime[p]){
            for(ll i = p*p; i <= n; i+=p){
                isprime[i] = 0;
            }
        }
    }
    vector<ll> primes;
    for(ll p = 2; p <= n; p++){
        if(isprime[p]) primes.push_back(p);
    }
    return primes;
}

vector<ll> factorize(ll n){ // factorize n in O(sqrt(n))
    if(n < 2) return {};
    vector<ll> factors;
    while(n % 2 == 0){
        factors.push_back(2);
        n /= 2;
    }
    for(ll p = 3; p*p <= n; p+=2){
        while(n % p == 0){
            factors.push_back(p);
            n /= p;
        }
    }
    if(n > 2) factors.push_back(n);
    return factors;
}

int main(){
    return 0;
}
