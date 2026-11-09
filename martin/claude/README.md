# ICPC / Kattis Template Library (C++17)

A reference set of templates for ICPC-style contests (NCPC, the Swedish
qualifiers, and similar Kattis-judged events). Everything is self-contained
C++17 — copy-paste the struct/functions you need directly into your solution
file, or `#include` the file if you're testing locally.

All files were compiled with `g++ -std=c++17 -fsyntax-only` and several were
smoke-tested against known answers.

## Layout

```
00_template.cpp                     Base contest skeleton (fast IO, macros)

data_structures/
  segment_tree.cpp                  Point-update segment tree (iterative + recursive)
  segment_tree_lazy.cpp             Range-update, range-sum with lazy propagation
  fenwick_tree.cpp                  BIT: point update / prefix sum, range-update BIT, 2D BIT
  dsu.cpp                           Union-Find with path compression + union by size
  sparse_table.cpp                  O(1) range-min query (idempotent ops)
  trie.cpp                          Prefix trie + 01-trie for max-XOR queries

graphs/
  dijkstra.cpp                      Shortest paths, non-negative weights
  bellman_ford.cpp                  Shortest paths with negative edges + neg-cycle detection
  floyd_warshall.cpp                All-pairs shortest paths
  mst_kruskal.cpp                   Minimum spanning tree via DSU
  mst_prim.cpp                      Minimum spanning tree via priority queue
  topo_sort.cpp                     Kahn's algorithm, cycle detection
  scc_tarjan.cpp                    Strongly connected components
  bridges_articulation.cpp          Bridges + articulation points (undirected graphs)
  lca_binary_lifting.cpp            LCA + tree distance, O(log n) per query
  bipartite_matching.cpp            Kuhn's algorithm, max bipartite matching
  max_flow_dinic.cpp                Dinic's max flow
  mcmf.cpp                          Min-cost max-flow (SPFA based)

strings/
  kmp.cpp                           Pattern matching, failure function
  z_function.cpp                    Z-array, generic pattern matching
  manacher.cpp                      All palindromic substrings in O(n)
  suffix_array.cpp                  Suffix array + Kasai's LCP array
  aho_corasick.cpp                  Multi-pattern matching automaton
  string_hashing.cpp                Double polynomial hashing for O(1) substring compare

math/
  number_theory.cpp                 gcd/extGcd, modpow, modinverse, sieve, factorization,
                                     Euler's totient, CRT, Miller-Rabin primality test
  matrix_exponentiation.cpp         Fast linear recurrences (e.g. Fibonacci) mod p
  combinatorics.cpp                 Precomputed factorials, nCr/nPr mod p, Catalan numbers,
                                     Pascal's triangle

geometry/
  geometry_basics.cpp               Point/vector ops, orientation, segment intersection,
                                     polygon area, point-in-polygon
  convex_hull.cpp                   Andrew's monotone chain, O(n log n)

misc/
  binary_search_ternary.cpp         Generic predicate binary search, ternary search
                                     (int and double) for unimodal functions
```

## Notes for contest use

- **Complexity**: given in each file's header comment. Most standard limits
  (n ≤ 1e5–1e6, ~1–2s time limit typical on Kattis) will pass with these.
- **`ll` vs `int` overflow**: most templates use `long long`; check whether
  your problem's constraints need `__int128` (already used in modmul spots)
  or plain `int` for speed. Sums that could exceed ~2e9 must use `ll`.
- **1-indexed vs 0-indexed**: Fenwick tree is 1-indexed internally; segment
  trees and most graph code are 0-indexed. Read the comment at the top of
  each struct before wiring it into your solve function.
- **MOD**: default `1e9+7` in the math templates — change the constant if
  the problem specifies a different modulus (e.g. `998244353`).
- Bring a **printed copy** of whichever subset you're most likely to need —
  during the actual contest you often won't have internet access, only
  your team's reference material.

## Suggested prep order for qualifiers

1. Graph basics: BFS/DFS (not included — trivial to write from scratch),
   Dijkstra, DSU, MST.
2. Segment tree / Fenwick tree — very common in problems requiring range
   queries.
3. Basic string algorithms: KMP, Z-function, hashing.
4. Number theory basics: modpow, sieve, nCr mod p — these appear constantly.
5. Only after the above are solid: flows, SCC, LCA, suffix arrays,
   Aho-Corasick, geometry — these show up less often but can be the
   difference-maker in the last 1–2 problems of a set.
