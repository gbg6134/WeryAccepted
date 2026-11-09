#!/usr/bin/env bash
# tools/mark.sh <src file> <first code line of the listing> <marker text>
# Inserts the marker on its own line just before that listing's \begin{lstlisting}.
set -eu
cd "$(dirname "$0")/.."
f="$1"; pat="$2"; marker="$3"
ln=$(grep -nxF "$pat" "$f" | head -1 | cut -d: -f1)
[ -n "$ln" ] || { echo "mark.sh: pattern not found in $f: $pat"; exit 1; }
# walk up to the begin-listing line
b=$((ln-1))
while [ "$b" -gt 0 ] && ! sed -n "${b}p" "$f" | grep -q 'begin{lstlisting}'; do
  b=$((b-1))
done
[ "$b" -gt 0 ] || { echo "mark.sh: no lstlisting above $pat"; exit 1; }
sed -i "${b}i ${marker}" "$f"
echo "marked $f:$b  ${marker}"
