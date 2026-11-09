#!/usr/bin/env bash
# Build notebook.pdf with tectonic (self-contained; fetches its own TeX bundle
# on first run). Overleaf is the other route -- just paste notebook.tex there.
set -eu
cd "$(dirname "$0")/.."
bash tools/build.sh
mkdir -p build/pdf
tectonic -X compile notebook.tex --outdir build/pdf --keep-logs --keep-intermediates \
  2>&1 | grep -vE '^note: downloading' || true

log=build/pdf/notebook.log
echo "-----------------------------------------"
grep -a -oE 'Output written[^)]*\)' "$log" || echo "no page count in log"
over=$(grep -a -c 'Overfull' "$log" || true)
echo "overfull hboxes: ${over:-0}   (must be 0: the column gutter is only 11.4pt)"
echo "toc entries:     $(grep -c contentsline build/pdf/notebook.toc)"
echo "pdf:             build/pdf/notebook.pdf"
