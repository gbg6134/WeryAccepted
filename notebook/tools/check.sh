#!/usr/bin/env bash
# Extract every lstlisting from notebook.tex and syntax-check it with g++.
# Markers, each on its own line immediately before \begin{lstlisting}:
#   %HEADER          this listing is the shared prelude
#   %NOCOMPILE       fragment / not C++ -- skip it
#   %LABEL <name>    also save this listing as a reusable dependency
#   %DEP <a> <b> ..  prepend those labelled listings before compiling this one
set -u
cd "$(dirname "$0")/.."
OUT=build/snips
rm -rf "$OUT"; mkdir -p "$OUT"

# No backslashes in this awk program on purpose (the tooling eats them);
# markers are matched as plain substrings.
awk -v out="$OUT" '
  BEGIN { B = "begin{lstlisting}"; E = "end{lstlisting}"
          man = out "/manifest.txt" }
  index($0, "%NOCOMPILE") == 1 { skip = 1; next }
  index($0, "%HEADER")    == 1 { hdr  = 1; next }
  index($0, "%LABEL")     == 1 { lbl  = $2; next }
  index($0, "%DEP")       == 1 { dep = $0; sub(/^%DEP[ ]*/, "", dep); next }
  index($0, B) > 0 {
      n++
      lf = (lbl != "") ? out "/lib_" lbl ".cpp" : ""
      if (hdr)       { f = out "/header.cpp"; hdr = 0 }
      else if (skip) { f = ""; skip = 0 }
      else           { f = sprintf("%s/snip_%03d.cpp", out, n)
                       printf "%s\t%s\n", f, dep > man }
      if (lf != "") printf "" > lf
      inl = 1; lbl = ""; dep = ""; next
  }
  index($0, E) > 0 {
      if (inl) { if (f != "") close(f); if (lf != "") close(lf) }
      inl = 0; f = ""; lf = ""; next
  }
  inl { if (f  != "") print > f
        if (lf != "") print > lf }
' notebook.tex

[ -f "$OUT/header.cpp" ] || { echo "FAIL: no %HEADER listing found"; exit 1; }
# the header listing ends in a main(); strip it so snippets supply their own
grep -v -e 'int main' -e 'sync_with_stdio' -e 'cin.tie' "$OUT/header.cpp" \
  | sed '/^}$/d' > "$OUT/prelude.cpp"

fail=0; pass=0; failed=""
while IFS=$'\t' read -r s deps; do
  [ -n "$s" ] || continue
  for std in gnu++17 gnu++20; do
    tu="$OUT/tu_$(basename "$s" .cpp)_$std.cpp"
    cat "$OUT/prelude.cpp" > "$tu"
    for d in $deps; do
      if [ -f "$OUT/lib_$d.cpp" ]; then cat "$OUT/lib_$d.cpp" >> "$tu"
      else echo "WARN: $(basename "$s") wants missing dep '$d'"; fi
    done
    cat "$s" >> "$tu"; echo 'int main(){}' >> "$tu"
    if err=$(g++ -std=$std -fsyntax-only "$tu" 2>&1); then
      pass=$((pass+1))
    else
      fail=$((fail+1)); failed="$failed $(basename "$s")[$std]"
      echo "=== FAIL [$std] $(basename "$s") ==="
      echo "$err" | grep -E 'error' | head -6
    fi
  done
done < "$OUT/manifest.txt"
echo "-----------------------------------------"
echo "syntax check: $pass ok, $fail failed (gnu++17 + gnu++20)"
[ -n "$failed" ] && echo "failed:$failed"
[ "$fail" -eq 0 ]
