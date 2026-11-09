#!/usr/bin/env bash
# Cheap LaTeX sanity checks on notebook.tex. There is no pdflatex on this
# machine, so this catches the usual paste-breakers: unescaped % _ # & in
# prose, and unbalanced inline math. Listings are verbatim and are skipped,
# as is the preamble (where #1 and trailing % are legitimate).
set -u
cd "$(dirname "$0")/.."

awk '
  BEGIN {
    BS = sprintf("%c", 92)          # a single backslash
    B  = "begin{lstlisting}"; E = "end{lstlisting}"
    bad = 0; started = 0; math = 0; ctab = 0
  }
  index($0, B) > 0 { inl = 1; next }
  index($0, E) > 0 { inl = 0; next }
  inl { next }
  index($0, "begin{document}") > 0 { started = 1; next }
  !started { next }
  index($0, "%") == 1 { next }                   # whole-line comment
  index($0, "begin{ctab}") > 0 || index($0, "begin{tabular}") > 0 { ctab = 1 }
  index($0, "end{ctab}")   > 0 || index($0, "end{tabular}")   > 0 { ctab = 0 }
  {
    line = $0
    sub(/%$/, "", line)                          # trailing continuation %
    n = length(line)
    for (i = 1; i <= n; i++) {
      c = substr(line, i, 1)
      if (c == BS) { i++; continue }             # escaped char / control seq
      if (c == "$") { math = 1 - math; continue }
      if (math) continue
      if (c == "%" || c == "_" || c == "#") {
        printf "L%d bare %s -> %s\n", NR, c, $0; bad++; break
      }
      if (c == "&" && !ctab) {
        printf "L%d bare & outside ctab -> %s\n", NR, $0; bad++; break
      }
    }
  }
  END {
    if (math) { print "ERROR: unbalanced inline math ($) at end of file"; bad++ }
    printf "lint: %d issues\n", bad
    exit (bad > 0)
  }
' notebook.tex
