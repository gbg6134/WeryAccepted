#!/usr/bin/env bash
# Build + run every test in tools/smoke/ against the templates extracted from
# notebook.tex.
#   *.cpp  first line is  // DEPS: name1 name2  (matching %LABEL names)
#   *.sh   self-contained, may transform a template before compiling
set -u
cd "$(dirname "$0")/.."
bash tools/check.sh >/dev/null 2>&1
S=build/snips
[ -f "$S/prelude.cpp" ] || { echo "extraction failed; run tools/check.sh"; exit 1; }
mkdir -p build/smoke
fail=0
for t in tools/smoke/*.cpp; do
  [ -e "$t" ] || continue
  name=$(basename "$t" .cpp)
  deps=$(head -1 "$t" | sed -n 's|^// DEPS:[ ]*||p')
  tu=build/smoke/$name.cpp
  cat "$S/prelude.cpp" > "$tu"
  ok=1
  for d in $deps; do
    if [ -f "$S/lib_$d.cpp" ]; then cat "$S/lib_$d.cpp" >> "$tu"
    else echo "[$name] MISSING dep $d"; ok=0; fi
  done
  [ "$ok" = 1 ] || { fail=$((fail+1)); continue; }
  tail -n +2 "$t" >> "$tu"
  if ! g++ -std=gnu++17 -O2 -o "build/smoke/$name.exe" "$tu" \
        2>"build/smoke/$name.log"; then
    echo "[$name] COMPILE FAIL"; head -6 "build/smoke/$name.log"
    fail=$((fail+1)); continue
  fi
  if out=$("build/smoke/$name.exe" 2>&1); then echo "[$name] $out"
  else echo "[$name] RUNTIME FAIL: $out"; fail=$((fail+1)); fi
done
for t in tools/smoke/*.sh; do
  [ -e "$t" ] || continue
  bash "$t" || fail=$((fail+1))
done
echo "-----------------------------------------"
echo "smoke: $fail failed"
[ "$fail" -eq 0 ]
