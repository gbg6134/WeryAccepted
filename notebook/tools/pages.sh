#!/usr/bin/env bash
# Rough page estimate for the KACTL layout: A4 landscape, 3 columns,
# text height 196mm = 557pt, code at 8/8.8pt -> ~63 lines/column
# -> ~189 column-lines per page. Prose at 8pt wraps near 62 chars in 93mm.
set -u
cd "$(dirname "$0")/.."
awk '
  BEGIN { B = "begin{lstlisting}"; E = "end{lstlisting}" }
  index($0, B) > 0 { inl = 1; next }
  index($0, E) > 0 { inl = 0; next }
  inl { code++; next }
  index($0, "%") == 1 { next }
  index($0, "section{") == 2 { sec++; next }
  index($0, "entry{") == 2 || index($0, "entryn{") == 2 { ent++ }
  { n = length($0); if (n > 0) prose += (n < 62 ? 1 : int(n/62) + 1) }
  END {
    lines = code + prose + 2*ent + 3*sec
    printf "code lines    %6d\n", code
    printf "prose lines   %6d\n", prose
    printf "entries       %6d\n", ent
    printf "sections      %6d\n", sec
    printf "column-lines  %6d\n", lines
    printf "est. pages    %6.1f   (189 col-lines/page, +1 index page)\n", lines/189 + 1
  }
' notebook.tex
