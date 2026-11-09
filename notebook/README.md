# Team Reference Document — NCPC / NWERC

`notebook.tex` is the single file to paste into Overleaf. Nothing under `Ruben/`,
`Ruiming/` or `martin/` was modified.

## Build

```
tools/build.sh    # src/*.tex  ->  notebook.tex   (run after any edit)
tools/pdf.sh      # notebook.tex -> build/pdf/notebook.pdf  (tectonic)
tools/lint.sh     # unescaped % _ # &, unbalanced $   -> 0 issues
tools/check.sh    # every listing compiled, gnu++17 + gnu++20 -> 116 ok, 0 failed
tools/smoke.sh    # templates vs brute force            -> 16 tests, 0 failed
tools/pages.sh    # rough page estimate without compiling (reads high: 18.5 vs 15 content pages)
```

Edit `src/*.tex`, not `notebook.tex` — the latter is generated. Two ways to get a
PDF: `tools/pdf.sh` locally (tectonic, self-contained, downloads its TeX bundle on
first run), or paste `notebook.tex` into Overleaf and hit Recompile. Tectonic is
XeTeX-based and Overleaf defaults to pdfLaTeX; both compile this file cleanly.

**Current output: 16 pages (front page + 15 of content), 0 overfull hboxes,
82 index entries.**

Team name, university and members are three macros at the top of
`src/00-preamble.tex` (`\team`, `\uni`/`\unishort`, `\members`); the front page and
the running header both read from them, so change them in one place.

## Rules compliance (NWERC 2025, <https://2025.nwerc.eu/rules>)

| Rule | How |
| --- | --- |
| max **25 pages** | **16** incl. the front page, measured from the compiled PDF — 9 pages of slack |
| single-sided, A4 | A4 landscape (still "A4 size"); print one-sided |
| university name **upper left** | `\lhead{\uni}` = "KTH Royal Institute of Technology" |
| page number **upper right** | `\rhead{\thepage}` |
| front page | carries team name, university and members; unnumbered and with no running header |
| readable at 0.5 m | 8pt code — *exactly* KACTL's size (see below) |
| institution on the folder cover | do this when you print |

NCPC allows unlimited printed material, so this one document covers both contests.

**Print page 1 and read it at half a metre before trusting it.** The size knob is
one line in the preamble:

```latex
\newcommand{\codesize}{\fontsize{8}{8.8}\selectfont}
```

Raise both numbers if it looks tight. **Do not lower them** — 8pt is what KACTL
prints for NWERC every year, and going below that is the one thing here that could
plausibly be challenged.

## Layout

Matched to KACTL deliberately, from `content/kactl.tex` + `content/tex/kactlpkg.sty`:

```latex
% KACTL's, for reference:
\documentclass[9pt,a4paper]{extreport}
\usepackage[a4paper,landscape,hmargin={0.5cm,0.5cm},vmargin={1.0cm,0.4cm}]{geometry}
\begin{multicols*}{3}
```

Ours is the same except `extarticle`, and `vmargin={0.4cm,0.4cm},includehead,headsep=4pt`
instead of KACTL's `vmargin={1.0cm,0.4cm}` — see *Verification* for why that
difference matters.

KACTL is **landscape**, which is the whole trick: landscape A4 with 0.5 cm side
margins gives 3 columns of ~93 mm — the same column width as Ruben's original
portrait 2-column layout, so every existing entry (including PathDP's 6-column
config table) still fits a column unchanged, at 1.5x the density.

Ruben's `\secbar` / `\entry` / `\code` macros and colours are untouched; `\entry`
now also emits a table-of-contents line, which is what builds the index.
If `tocloft` ever misbehaves, comment out `\tableofcontents` — nothing else
depends on it.

## Verification

| Tool | What it actually proves |
| --- | --- |
| `lint.sh` | no unescaped specials, inline math balanced, `tabular`/`ctab`/`begingroup` balanced |
| `check.sh` | all 116 listings compile under **both** gnu++17 and gnu++20 |
| `smoke.sh` | 16 randomised tests, each against an independent oracle |

`%HEADER`, `%NOCOMPILE`, `%LABEL <name>` and `%DEP <a> <b>` markers in `src/*.tex`
drive the extraction. `%NOCOMPILE` is Ruben's existing convention for fragments.

What the smoke tests check:

- **hungarian** — 400 rectangular cost matrices with negative costs, vs brute-force
  permutations
- **dinic** — 400 graphs: Dinic flow == MCMF flow == capacity of the cut `cut()` reports
- **matching** — Kuhn == unit-capacity Dinic, `matchL`/`matchR` consistent
- **euler** — 3000 multigraphs: existence matches a degree+connectivity oracle, and
  every returned path uses each edge exactly once
- **lazyseg** / **lazyseg_ap** — range add + range sum vs a naive array, *and* an
  arithmetic-progression config that actually exercises `sh(f,k)`
- **scc** — components == mutual reachability (Floyd closure), order is topological
- **paths** — Dijkstra == Floyd-Warshall, parent chains reach the source
- **wdsu** — contradictions detected, differences consistent
- **ntt** — 300 convolutions vs naive O(n²), plus n=5000
- **crt** — 4000 cases vs exhaustive search, non-coprime moduli included
- **pollard** — `isPrime` vs a sieve to 2e5; 304 factorisations multiply back, incl. 4e18
- **binom** — `C(n,k)` vs Pascal, out-of-range guards
- **gauss** — full-rank solutions satisfy `A*x == b`
- **strings** — kmp, period, zfunc, Manacher `isPal`, hashing, suffix array + lcp, Aho
- **geometry** — `segInter` vs an exact integer oracle (200k cases), `hull` vs an
  O(n³) oracle, `inConvex` vs `inPoly`, `area2`, `closest` vs O(n²), circles

The document itself compiles clean under tectonic: **16 pages** (front page + 15), **0 overfull
hboxes**, 82 index entries. Two layout bugs were found and fixed by actually looking
at the compiled output rather than trusting the source:

- Sections were numbered in the index, and since `\cftsecnumwidth` is `0em` the number
  overprinted the title ("T4rees"). Fixed with `\setcounter{secnumdepth}{0}`, which
  also matches the body, where the section bars show no number.
- 22 code lines exceeded the column width. The column fits **62 characters** at 8pt;
  the worst offender was 68. Anything over ~65 pushes past the 11.4pt gutter and
  collides with the next column. `tools/rewrap.awk` records those rewrites; they are
  pure reformatting apart from `closest` and `hungarian`, whose loop bodies were
  restructured equivalently and are both covered by the smoke tests.

**Keep code lines at or under 62 characters** when adding anything — `tools/pdf.sh`
reports the overfull count, and it must stay 0.

Two more, found only by looking at the printed header:

- **The header was sliced in half by the page edge.** With plain
  `vmargin={1.0cm,0.4cm}` LaTeX put the header box top at
  `1in + \topmargin = 72.27 - 80.82 = -8.55pt`, i.e. *above* the paper. KACTL hides
  this by setting `\headheight` to 25pt, which just pushes the text back down inside
  an oversized box. The fix here is `includehead` in `geometry`, so the header is
  reserved *inside* the top margin and the arithmetic comes out consistent
  (`\topmargin` now `-60.89pt`, header top at `+11.38pt` = the requested 0.4cm, and
  the bottom margin lands on 0.4cm too). If you ever change `vmargin`, re-check
  `\topmargin` in `build/pdf/notebook.log` — `1in + \topmargin` must be positive.
- **`tocloft` forces `\thispagestyle{plain}` on the table-of-contents page**, which
  silently removed the header from the index page (and only that page). Neither
  `\pagestyle` nor `\thispagestyle` *before* `\tableofcontents` helps; the undo has
  to come after it. Bisected against a minimal file — `titlesec` is innocent.

The one thing not verified is how it *looks*: there is no PDF rasteriser on this
machine (`pdftotext` is present but `pdftoppm` is not, and `convert` here is the
Windows filesystem tool, not ImageMagick). Open `build/pdf/notebook.pdf` and check
the print by eye, as the rules demand anyway.

## Bugs found in the original files (fixed here, originals untouched)

| File | Problem |
| --- | --- |
| `Ruiming/geometry/Convex_Hull.cpp` | two files concatenated, duplicate `#include`/`Point` — **does not compile** |
| `Ruiming/geometry/Lattic_Point.cpp` | uses `Point` and `Cross_point` before they are declared — **does not compile** |
| `Ruiming/Dynamic Programming/Bitmask DP.cpp` | `dp[MAXN]` is 1-D but indexed `dp[s][j]` — **does not compile** |
| `Ruiming/graph/Floyd-Warshall.cpp` | `dist[b][a]=min(dist[a][b],c)` reads the just-written cell; wrong when an earlier edge already set `b,a` |
| `Ruiming/String/Hash.cpp` | `get_hash` returns `ll` from `ull` arithmetic (sign bug); single overflow-mod hash |
| `Ruiming/Data Structures/Weighted_union_find.cpp` | `merge_set` never sets the weight, so the potentials are never usable |
| `Ruiming/graph/Hugarian_algorithm.cpp` | is **Kuhn's unweighted matching**, not the Hungarian algorithm |
| `martin/claude/strings/aho_corasick.cpp` | `search` walks the whole fail chain per position: O(n·depth), not the advertised O(n + matches) |
| `martin/claude/strings/suffix_array.cpp` | O(n log²n) via comparator sort |
| `martin/claude/math/number_theory.cpp` | `crt` arithmetic is fragile |

Two more were found *by the tooling* while building this, worth knowing:

- `bits/stdc++.h` on GCC 16 no longer pulls in `<cassert>`, so the header includes it
  explicitly. Judges on older GCC do not need this; it costs nothing.
- A `#define sz(v)` in the header **breaks `Treap`**, which has its own `sz()` member.
  That macro is deliberately *not* in the header. Do not add it.

## What changed vs. your own files

Ruben's entries are verbatim. For the others, the shared prelude that used to sit at
the top of every one of Ruiming's files (`#include`, typedefs, `MAXN=206`, `eps`) now
appears **once**, in *Contest setup*, and loose `main()` bodies became callable
functions. Names are kept wherever there was a real interface.

| Entry | From | Call site change |
| --- | --- | --- |
| `Tree` (order statistics) | `Ruiming/.../OrderStatisticTree.cpp` | none — his `Tree<T>` alias kept |
| `lis` | `martin/dp/lis.cpp` | none — `lis(a)` still returns the length |
| `kmp`, `failure` | `martin/string/kmp.cpp` | none — `kmp(txt, pat)` still returns start indices |
| `precompute`, `C` | `Ruben/PrecomputeBinCoeff.cpp` | none |
| `power`, `ext_gcd`, `inv` | `Ruben/InverseModulu.cpp` | none |
| `SpTab` | Ruiming + martin sparse tables | **was** global `st[j][i]`/`back[j][i]` 1-indexed arrays + `log2()`. Now `SpTab s(a); s.query(l, r)`, half-open, configurable `op` |
| `WDSU` | `Ruiming/.../Weighted_union_find.cpp` | **was** `find_set`/`merge_set` on globals `s[]`, `d[]`, `height[]`. Now `join(a,b,w)` returns false on a contradiction, `diff(a,b)` reads the difference |
| `diameter` | `Ruiming/graph/Tree_Diamter.cpp` | **was** two recursive DFS writing globals `place`/`ans`. Now returns `{length, a, b}`, iterative |
| `centroids` | `Ruiming/graph/Centroid.cpp` | **was** recursive DFS filling a global `cand`. Now returns the vector, iterative |
| `sumOfDists` | `Ruiming/.../Tree DP.cpp` | **was** `dfs`/`dfs1` on globals. Now returns `ans`, iterative |
| `kruskal` | Ruben's `.tex` snippet | wrapped in a function returning `{cost, edges}` instead of inline statements |
| `floyd` | `Ruiming/graph/Floyd-Warshall.cpp` | takes a `matrix&`, 0-indexed, `INF` guards added; **the min bug is fixed** |
| `dijkstra`, `bellman` | Ruiming + martin | **was** `Link`/`Edge`/`insert` linked-list adjacency, 1-indexed globals. Now `vector<vector<pii>>`, 0-indexed; `dijkstra` can fill a parent array |
| `kShortest` | `Ruiming/graph/kth_longest_path.cpp` | **was** a per-node `priority_queue` array. Now one function returning the k lengths |
| `eulerPath` | Ruiming's two `Eulertour` files | **was** recursive, adjacency-matrix or `multiset`, 1-indexed, with the degree checks inline in `main`. Now one iterative function for both directions, returning `{}` when no path exists |
| `Matching` | `martin/claude/.../bipartite_matching.cpp` | renamed from `BipartiteMatching`; same members |
| `Dinic` | `martin/claude/.../max_flow_dinic.cpp` | `addEdge` now returns an edge id; added `cut()` and `flowOn(id)` |
| `MCMF` | `martin/claude/.../mcmf.cpp` | added the `maxf` cap argument |
| `Hash` | `Ruiming/String/Hash.cpp` | **was** `Bhash(s)` + `get_hash(v, a, b)`, 1-indexed inclusive, single overflow hash. Now `Hash h(s); h.get(l, r)`, 0-indexed half-open, two moduli |
| `Manacher` | Ruiming + martin | **was** inline in `main` over a `#`-padded buffer. Now a struct with `d1`, `d2` and `isPal(l, r)` |
| `SuffixArray` | `martin/claude/.../suffix_array.cpp` | O(n log n) instead of O(n log²n); one struct holding `sa`, `rnk`, `lcp` |
| `Aho` | `martin/claude/.../aho_corasick.cpp` | output links added (so it hits its advertised complexity) and duplicate patterns now work; `match(txt, f)` callback instead of a returned matrix |
| `gauss` | `Ruiming/math/gaussjordanelimination.cpp` | **was** globals `A`/`b`/`ans` with the answer printed from `main`. Now returns the rank and fills `x`; partial pivoting added |
| `crt` | `Ruiming/math/CRT.cpp` | **was** an `O(m)` search loop over residues. Now `extGcd`-based, handles non-coprime moduli, returns `{-1,-1}` if inconsistent |
| `isPrime`, `factor` | `Ruiming/math/miller_rabin.cpp`, `Pollard_rho.cpp` | **was** `getchar` fast-IO and `typedef __int128 ll`. Now plain `ll` with `i128` only inside `mulm` |
| geometry | the four `Ruiming/geometry/*.cpp` | **replaced**, as you asked: one `P<T>` template. `Pl` = `P<ll>` is exact, `Pd` = `P<ld>` for metric work. **No global mutable `eps`**, and arrays are `vector`, **0-indexed** — the old code was 1-indexed `Point*`. Names carried over in spirit: `Dot`/`Cross` → `.dot()`/`.cross()`, `Point_on_seg` → `onSeg`, `Cross_Segment` → `segInter`, `Cross_point` → `lineInter`, `Dis_point_seg` → `segDist`, `Polygon_area` → `area2` (**twice** the area, integer), `Point_in_polygon` → `inPoly`, `Convex_Hull` → `hull` |

### New — nobody had these

`LineContainer` (CHT, adapted from KACTL, CC0), `hungarian` (real weighted
assignment), `ntt`/`conv` (FFT), `closest` (closest pair), `circleLine`,
`circleCircle`, `inConvex`, `sos` (subset sums), `compress`, and the whole
*Reference* section.

### Cut, and why

| Cut | Reason |
| --- | --- |
| `Scapegoattree.cpp` | superseded by `Tree` (pbds) + `Treap` |
| `Big_Integer.cpp` | 205 lines, and Python 3 / Java `BigInteger` are both available |
| `Johnsons.cpp` | `floyd` covers n ≤ 500; Johnson only wins on sparse APSP with negatives |
| `mst_prim.cpp` | `kruskal` + `DSU` covers it |
| `Modifyheap`, `DFS_standard`, `Edges_Link` | idioms, not algorithms — a comparator and a loop |
| `Minimum LIS.cpp` | folded into the `lis` notes (Dilworth) |
| `Built_In_Treap.cpp` (rope) | non-standard, and `Treap` does the same job |
| plain `Trie` | noted in the `XorTrie` entry — same shape, 26 children |
| `Eulerthm.cpp` | folded into the *Modular basics* prose |
| `Fast_gcd`, `Fastmod`, `Combinumber*`, `Factorization`, `Prime_factorization`, `Linearinverse` | duplicates of `__gcd`, `power`, `C`, `spf`, `factor`, `inv_fact` |
| contest skeletons (`Codeforces.cpp`, `luogu.cpp`, `USACO.cpp`, `NewCodeforces.cpp`) | one header is enough |

## Trim order

If Overleaf reports more than 25 pages, delete from the top. Each `\entry` plus its
following `lstlisting` is independently removable.

1. `SuffixArray` — `Hash` + binary search usually replaces it (~45 lines)
2. `Aho` — rare relative to its size (~45 lines)
3. `Treap` — needed only for insert/erase in the middle of a sequence (~60 lines)
4. `kShortest`
5. `sumOfDists` (rerooting) — the pattern is described in the prose anyway
6. `inConvex` — `inPoly` is O(n) but usually fast enough
7. `LineContainer` — only for a specific DP shape
8. `gauss` — drop last; it is short and shows up in odd places

## Still missing, deliberately

Simplex / LP, Berlekamp-Massey, suffix automaton, half-plane intersection, persistent
segment tree, Gomory-Hu, general (non-bipartite) matching, Mo's algorithm. All
plausible-but-unlikely at NCPC/NWERC, and there is ~6 pages of slack if you want to
add one — that is what the slack is for (16 of 25 pages used).
