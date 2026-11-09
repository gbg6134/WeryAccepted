//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <bits/stdc++.h>
#include <ext/rope>
using namespace std;
 
typedef long long ll;
typedef long double ld;
 
const ll INF=1e18;
const ll MAXN=206;//UPDATERA ARRAY STORLEKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ll n,lst[MAXN],ans;

// /https://www.geeksforgeeks.org/cpp/stl-ropes-in-c/

/*
push_back(): This function is used to input a character at the end of the rope. Time Complexity: O(log N).
pop_back(): Introduced from C++11(for strings), this function is used to delete the last character from the rope. Time Complexity: O(log N).
insert(int x, crope r1): Inserts the contents of r1 before the xth element. Time Complexity: For Best Case: O(log N) and For Worst Case: O(N).
erase(int x, int l): Erases l elements, starting with the xth element. Time Complexity: O(log N).
substr(int x, int l): Returns a new rope whose elements are the l characters starting at the position x. Time Complexity: O(log N).
replace(int x, int l, crope r1): Replaces the l elements beginning with the xth element with the elements in r1. Time Complexity: O(log N).
concatenate(+): concatenate two ropes using the '+' symbol. Time Complexity: O(1).
*/

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    __gnu_cxx::rope<int> r;
    r.push_back(0);
    r.push_back(5);

    r.insert(1,2);
    r.insert(3,3);
    r.insert(2,6);

    for(int i:r){
        cout<<i<<' ';
    }
    cout<<'\n';
}
