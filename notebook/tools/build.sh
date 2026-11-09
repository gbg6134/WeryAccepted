#!/usr/bin/env bash
# Concatenate src/*.tex into the single-file notebook.tex you paste into Overleaf.
set -eu
cd "$(dirname "$0")/.."
{ for f in src/*.tex; do cat "$f"; echo; done; } > notebook.tex
echo "notebook.tex: $(wc -l < notebook.tex) lines from $(ls src/*.tex | wc -l) parts"
