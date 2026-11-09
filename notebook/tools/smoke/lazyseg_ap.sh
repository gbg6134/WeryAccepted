#!/usr/bin/env bash
# The sh(f,k) hook only does anything when the update varies along the range.
# Build an arithmetic-progression variant of LazySeg by rewriting its 5 config
# lines, then check it against a naive array. This exercises sh() and the
# "c = max(0, m - max(ql, l))" offset in upd().
set -u
cd "$(dirname "$0")/../.."
S=build/snips
out=build/smoke/apseg.cpp
mkdir -p build/smoke
cat "$S/prelude.cpp" > "$out"
sed -e 's|.*using F = ll;.*|    using F = pair<ll,ll>;   // {start, step}|' \
    -e 's|.*static T ap(T x, F f, int k).*|    static T ap(T x, F f, int k) { return x + f.first*k + f.second*((ll)k*(k-1)/2); }|' \
    -e 's|.*static F cm(F a, F f).*|    static F cm(F a, F f) { return {a.first+f.first, a.second+f.second}; }|' \
    -e 's|.*static F sh(F f, int k).*|    static F sh(F f, int k) { return {f.first + f.second*k, f.second}; }|' \
    -e 's|.*constexpr F NOP.*|    static constexpr F NOP = {0, 0};|' \
    -e 's|struct LazySeg {|struct APSeg {|' \
    -e 's|LazySeg(|APSeg(|g' \
    "$S/lib_LazySeg.cpp" >> "$out"
cat >> "$out" <<'CPP'
int main(){
    mt19937 rng(2024);
    for (int iter = 0; iter < 300; iter++) {
        int n = 1 + rng() % 30;
        vi a(n, 0);
        APSeg s(a);
        for (int q = 0; q < 150; q++) {
            int l = rng() % n, r = l + 1 + rng() % (n - l);
            if (rng() % 2) {
                ll st = (ll)(rng() % 11) - 5, sp = (ll)(rng() % 7) - 3;
                s.update(l, r, {st, sp});
                for (int i = l; i < r; i++) a[i] += st + sp * (i - l);
            } else {
                ll want = 0;
                for (int i = l; i < r; i++) want += a[i];
                if (s.query(l, r) != want) {
                    printf("MISMATCH iter=%d\n", iter); return 1;
                }
            }
        }
    }
    puts("OK 300 cases x 150 ops: arithmetic-progression range add vs naive (exercises sh)");
}
CPP
if ! g++ -std=gnu++17 -O2 -o build/smoke/apseg.exe "$out" 2>build/smoke/apseg.log; then
  echo "[lazyseg_ap] COMPILE FAIL"; head -8 build/smoke/apseg.log; exit 1
fi
echo "[lazyseg_ap] $(./build/smoke/apseg.exe)"
