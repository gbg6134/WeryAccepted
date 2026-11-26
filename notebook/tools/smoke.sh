#!/usr/bin/env bash
# Build + run every test in tools/smoke/ against the templates extracted from
# notebook.tex. All tests compile and run in parallel.
#   *.cpp  first line is  // DEPS: name1 name2  (matching %LABEL names)
#   *.sh   self-contained, may transform a template before compiling
set -u
cd "$(dirname "$0")/.."
bash tools/check.sh >/dev/null 2>&1
S=build/snips
[ -f "$S/prelude.cpp" ] || { echo "extraction failed; run tools/check.sh"; exit 1; }
mkdir -p build/smoke
rm -f build/smoke/*.res

one() {   # one .cpp test -> build/smoke/<name>.res (first word OK or FAIL)
  t=$1; name=$(basename "$t" .cpp)
  deps=$(head -1 "$t" | sed -n 's|^// DEPS:[ ]*||p')
  tu=build/smoke/$name.cpp; res=build/smoke/$name.res
  cat "$S/prelude.cpp" > "$tu"
  for d in $deps; do
    if [ -f "$S/lib_$d.cpp" ]; then cat "$S/lib_$d.cpp" >> "$tu"
    else echo "FAIL [$name] MISSING dep $d" > "$res"; return; fi
  done
  tail -n +2 "$t" >> "$tu"
  if ! g++ -std=gnu++17 -O2 -o "build/smoke/$name.exe" "$tu" \
        2>"build/smoke/$name.log"; then
    { echo "FAIL [$name] COMPILE"; grep -m6 error "build/smoke/$name.log"; } > "$res"
    return
  fi
  if out=$("build/smoke/$name.exe" 2>&1); then echo "OK [$name] $out" > "$res"
  else echo "FAIL [$name] RUNTIME: $out" > "$res"; fi
}
# at most 6 g++ -O2 at once: 17 together run out of memory
for t in tools/smoke/*.cpp; do
  [ -e "$t" ] || continue
  while [ "$(jobs -rp | wc -l)" -ge 6 ]; do wait -n; done
  one "$t" &
done
for t in tools/smoke/*.sh; do
  [ -e "$t" ] || continue
  ( n=$(basename "$t" .sh)
    if out=$(bash "$t" 2>&1); then echo "OK [$n] $out"; else echo "FAIL [$n] $out"; fi \
      > "build/smoke/$n.res" ) &
done
wait
cat build/smoke/*.res
fail=$(grep -c '^FAIL' build/smoke/*.res | awk -F: '{s+=$2} END {print s+0}')
echo "-----------------------------------------"
echo "smoke: $fail failed"
[ "$fail" -eq 0 ]
