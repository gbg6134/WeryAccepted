#!/usr/bin/env bash
# Every code line must fit the 93mm column at 8pt: 62 characters. Longer lines
# run into the next column (the gutter is only 11.4pt). Prints offenders.
# Listings marked %VERBATIM (Ruiming's code, printed unchanged) are exempt:
# they use breaklines=true, so the printer wraps them instead.
set -u
cd "$(dirname "$0")/.."
awk '
  index($0, "%VERBATIM") == 1 { verb = 1; next }
  index($0, "begin{lstlisting}") { inl = 1; next }
  index($0, "end{lstlisting}")   { inl = 0; verb = 0; next }
  inl && !verb && length($0) > 62 { printf "L%d (%d): %s\n", NR, length($0), $0; bad++ }
  END { printf "width: %d code lines over 62 chars\n", bad; exit (bad > 0) }
' notebook.tex
