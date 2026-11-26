r"""Generate the Ruiming parts of the notebook from Ruiming/*, keeping his code.

What is allowed to differ from his files (agreed with the team), and nothing else:
  1. empty / whitespace-only lines are dropped;
  2. layout only: a block holding ONE simple statement loses its braces and goes
     on one line:  if(x){ / a; / }  ->  if(x) a;   (never when that statement is
     itself an if, never when the head line has a comment after its '{');
  3. a top-level function/struct (with the // lines right above it) identical to
     one printed in an earlier file becomes  <first line up to '{'> /* see F */ }
  4. header lines (before the first function/struct) identical to header lines of
     an earlier file are dropped; the file starts with  // header lines: see F
  5. usage-only demo functions (main/solve/init that only read input and print,
     listed in DROP) are left out; what they showed is in the entry text instead.
Long lines are wrapped by the printer (breaklines), the text is not touched.

Checks run every time:
  * rule 2 is checked by compiling each original file and its re-laid-out version
    to assembly with g++ and requiring identical output (files that do not compile
    on their own are reported and checked by a token comparison instead);
  * rules 1, 3, 4, 5 are checked by rebuilding every file from the printed text.

Run: python tools/ruiming.py   then tools/build.sh / tools/pdf.sh
"""
import os, re, subprocess, sys, tempfile

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
RU = os.path.join(ROOT, 'Ruiming')
SRC = os.path.join(ROOT, 'notebook', 'src')

T = lambda *s: ''.join(s)

# (output file, section title or None, [(path, complexity, usage text)])
PARTS = [
 ('01b-ruiming-setup.tex', None, [
   ('NewStresstest.sh', '', T(
    'Put your solution in \\code{sol.cpp}, a slow but surely correct one in ',
    '\\code{brute.cpp} and a generator taking a seed in \\code{gen.py}; ',
    '\\code{bash stress.sh} stops at the first input where they disagree and prints it.')),
   ('Stresstest.sh', '', 'One round of the same: \\code{mine} vs \\code{ans} on one generated case.'),
 ]),
 ('11-geometry.tex', 'Geometry (Ruiming)', [
   ('geomtery/Line_and_Vectors.cpp', '', T(
    'The base every geometry file below shares (later files only point back here). ',
    '\\code{ld} everywhere, compared with \\code{ldcmp(x,y)} $\\in\\{-1,0,1\\}$ using ',
    '\\code{eps}; for big coordinates use a relative one: ',
    '\\code{eps = max(1e-6, 1e-6*max(|x|,|y|))}. \\code{Vector} = \\code{Point}.\\\\[0.3ex]',
    '\\begin{ctab}{0.44}{0.51}',
    '\\code{Line(p1,p2)}, \\code{Line(P,angle)}, \\code{Line(a,b,c)} & directed line, ',
    'or $ax+by+c=0$\\\\',
    '\\code{Point\\_line\\_relation(P,v)} & 1 left, 2 right, 0 on\\\\',
    '\\code{Line\\_relation(v1,v2)} & 0 parallel, 1 same line, 2 cross\\\\',
    '\\code{Cross\\_point(a,b,c,d)} & lines $ab$, $cd$; not parallel\\\\',
    '\\code{Cross\\_Segment(a,b,c,d)} & segments touch (ends count)\\\\',
    '\\code{Point\\_line\\_proj}, \\code{\\_symmetry} & foot of perpendicular, mirror\\\\',
    '\\code{Dis\\_point\\_line}, \\code{Dis\\_point\\_seg} & distances\\\\',
    '\\code{DirectedAngle(A,B)} & signed angle $A \\to B$, \\code{Rotate} ccw\\\\',
    '\\end{ctab}')),
   ('geomtery/Polygon.cpp', '$O(n)$', T(
    'Polygons are arrays \\code{p[1..n]} (\\textbf{1-indexed}), vertices in order. ',
    '\\code{Polygon\\_area(p,n)} is the absolute area. \\code{Point\\_in\\_polygon(pt,p,n)}: ',
    '3 on a vertex, 2 on an edge, 1 inside, 0 outside. \\code{cmp} sorts points by ',
    'angle around the origin (\\code{atan2l}), for ordering a convex polygon\'s vertices.')),
   ('geomtery/Convex_Hull.cpp', '$O(n \\log n)$', T(
    '\\code{v = Convex\\_Hull(p, n, ch)}: \\code{p[1..n]} in (gets sorted and deduplicated), ',
    'hull in \\code{ch[1..v]}. Points on the hull edges are \\textbf{kept} (it pops only on ',
    'a strict right turn). Note the last \\code{sort}: \\code{ch} comes back sorted by ',
    '$(x, y)$, \\textbf{not} in counter-clockwise order --- sort with \\code{cmp} around an ',
    'inside point, or drop that line, before walking the hull.')),
   ('geomtery/Lattic_points.cpp', 'exact', T(
    'Integer coordinates with \\code{ll} only: no \\code{eps}, no wrong answers. ',
    '\\code{Point\\_in\\_polygon(pt,p,n)} (1-indexed, 3 vertex / 2 edge / 1 inside / 0 ',
    'outside) and \\code{Polygon\\_area2} = \\textbf{twice} the signed area. Pick: ',
    'interior lattice points $I = (A_2 - B + 2)/2$, with $B = \\sum \\gcd(|dx|,|dy|)$ over ',
    'the edges. Distances and projections come back as \\code{ld} \\code{Point}s. ',
    'Coordinates up to $10^9$ keep the cross products inside \\code{ll}.')),
   ('geomtery/Closest_pair.cpp', '$O(n \\log^2 n)$', T(
    'Smallest distance between two of the points \\code{p[1..n]}. \\textbf{Sort first}: ',
    '\\code{sort(p+1, p+n+1, cmpx); closest\\_pair(1, n)} returns the distance. ',
    'Divide and conquer on $x$, then check the strip sorted by $y$.')),
   ('geomtery/Half_Plane_Intersect.cpp', '$O(n \\log n)$', T(
    '\\code{HPI(L)}: every \\code{Line(p1,p2)} keeps the half-plane on its ',
    '\\textbf{left}; returns the vertices of the intersection (empty if it is empty). ',
    'Intersection of convex polygons: add every edge \\code{Line(poly[j], poly[j+1])} with ',
    'the polygons counter-clockwise, then the area is $\\frac12|\\sum$ ',
    '\\code{Cross(q[i], q[i+1])}$|$. Add a big bounding box if the result may be unbounded.')),
   ('geomtery/Circle.cpp', '', T(
    '\\code{Circle(c, r)}. \\code{Point\\_circle\\_relation}, \\code{Line\\_Circle\\_relation}, ',
    '\\code{Seg\\_circle\\_relation}: 0 inside / crossing, 1 touching, 2 outside / missing. ',
    '\\code{Line\\_Cross\\_Circle(v, C, pa, pb)} returns how many intersection points (0--2) ',
    'and writes them to \\code{pa}, \\code{pb}. \\code{Circumcenter(A,B,C)}: centre of the ',
    'circle through three non-collinear points; radius \\code{Dist(center, A)}.')),
   ('geomtery/min_cover_circle.cpp', 'expected $O(n)$', T(
    '\\code{min\\_center\\_circle(p, n, c, r)}: smallest circle containing \\code{p[1..n]}; ',
    'centre into \\code{c}, radius into \\code{r}. Shuffles \\code{p} (randomised ',
    'incremental, Welzl). \\code{random\\_shuffle} is deprecated but g++ still has it; if ',
    'it ever fails to compile, use \\code{shuffle(p+1, p+n+1, mt19937(7))}.')),
 ]),
 ('09a-ruiming-math.tex', 'Math (Ruiming)', [
   ('math/exgcd.cpp', '$O(\\log)$', T(
    '\\code{exgcd(a, b)} fills the globals: $ax + by = d = \\gcd(a,b)$. Inverse of $a$ mod ',
    '$b$ (needs $d = 1$): \\code{(x \\% b + b) \\% b}.')),
   ('math/CRT.cpp', '$O(\\sum m_i)$', T(
    'Reads $n$ pairs \\code{m[i] a[i]} ($x \\equiv a_i \\bmod m_i$) and builds the answer by ',
    'trying every residue; \\code{ans[n]} mod the product. Moduli must be \\textbf{coprime} ',
    'and small (the loop is $O(m_i)$).')),
   ('math/Fast_gcd.cpp', '', T(
    'Binary gcd with \\code{\\_\\_builtin\\_ctz}. Both arguments must be $> 0$ ',
    '(\\code{ctz(0)} is undefined); otherwise \\code{\\_\\_gcd} is just as good.')),
   ('math/Fastmod.cpp', '$O(\\log p)$', T(
    '\\code{mul(a,b,m)} = $ab \\bmod m$ through \\code{\\_\\_int128} (safe for $m$ up to ',
    '$9\\cdot10^{18}$); \\code{fast(x,p,m)} = $x^p \\bmod m$.')),
   ('math/Eulerthm.cpp', '', T(
    '$a^b \\bmod m$ when $b$ has thousands of digits: $b$ is read as a string and reduced ',
    'mod $\\varphi(m)$ by \\code{read()}; \\code{flag} says $b \\ge \\varphi(m)$, and only then ',
    'is $\\varphi(m)$ added back (extended Euler theorem, works even if $\\gcd(a,m) \\ne 1$). ',
    '$\\varphi(m)$ is computed in \\code{main}.')),
   ('math/Phi.cpp', '$O(\\sqrt n)$', '\\code{phi(n)} = $\\varphi(n)$, numbers in $1..n$ coprime to $n$, for one $n$.'),
   ('math/Linearphi.cpp', '$O(n)$', T(
    '$\\varphi(i)$ for every $i \\le n$ into \\code{phi[]} with the linear sieve: for a prime ',
    '$p$ dividing $i$, $\\varphi(pi) = p\\,\\varphi(i)$, else $(p-1)\\varphi(i)$.')),
   ('math/Euler_sieve.cpp', '$O(n)$', T(
    'Linear sieve: every composite is crossed out once, by its smallest prime. ',
    '\\code{primes} lists the primes $\\le n$ in order, \\code{p[i]} marks composites; ',
    'the query loop prints the $a$-th prime.')),
   ('math/Factorization.cpp', '$O(N\\sqrt N)$', T(
    '\\code{fac[i]} = map prime $\\to$ exponent for every $i <$ \\code{MAXN}, by trial ',
    'division. For many numbers up to $10^7$ a smallest-prime-factor sieve is faster.')),
   ('math/Prime_factorization.cpp', '$O(\\sqrt n)$', T(
    '\\code{factor(n)} returns the prime factors with repetition, in increasing order ',
    '($12 \\to 2, 2, 3$). Up to $10^{12}$ fine; above that use Pollard rho.')),
   ('math/miller_rabin.cpp', '$O(12 \\log n)$', T(
    '\\code{miller\\_rabin(n)}: deterministic primality test for all 64-bit $n$ (bases ',
    '2..37). Here \\code{ll} is \\code{\\_\\_int128}, with \\code{read}/\\code{write}/',
    '\\code{print} for it --- do not paste this file next to code that expects ',
    '\\code{ll} = \\code{long long}.')),
   ('math/Pollard_Rho.cpp', '$\\approx O(n^{1/4})$', T(
    'Factorises $n$ up to $\\sim 10^{18}$: \\code{p.clear(); pollard\\_rho(n);} then ',
    '\\code{p} holds the prime factors (with repetition, unsorted). Check ',
    '\\code{miller\\_rabin(n)} first if you only need to know whether $n$ is prime.')),
   ('math/Combinumber.cpp', '$O(N K)$', T(
    'Pascal\'s triangle: \\code{c[i][j]} $= \\binom ij \\bmod$ \\code{M} for ',
    '$i <$ \\code{MAXN}, $j < 106$. No inverses needed, so any modulus works.')),
   ('math/Combinumber_inv.cpp', '$O(n \\log M)$', T(
    'Factorials \\code{fac[i]} and inverse factorials \\code{inv[i]} mod a prime; then ',
    '$\\binom ni$ = \\code{fac[n]*inv[i]\\%MOD*inv[n-i]\\%MOD} in $O(1)$.')),
   ('math/Linearinverse.cpp', '$O(n + \\log p)$', T(
    'Inverses of $1..n$ mod a prime $p$ with one \\code{fast}: inverse factorials ',
    'backwards from \\code{inv[n]}, then $i^{-1} = (i-1)! \\cdot (i!)^{-1}$.')),
   ('math/Matrix.cpp', '$O(k^3 \\log n)$', T(
    '\\code{struct matrix} is \\textbf{1-indexed} with \\code{row}, \\code{col}; ',
    '\\code{a*b} mod \\code{MOD}, \\code{pow\\_matrix(a, n)} = $a^n$. Linear recurrences: ',
    'put the recurrence in a $k\\times k$ matrix and raise it. \\code{MAXN} caps $k$ ',
    '(the struct is a fixed array).')),
   ('math/gaussjordanelimination.cpp', '$O(n^3)$', T(
    'Solves $Ax=b$ over the reals. \\code{A} is $n \\times (n{+}1)$, 1-indexed, last column = ',
    '$b$. After \\code{gauss\\_elimination(A, res)} the answer is \\code{x[i] = A.m[i][n+1]}; ',
    '\\code{main} tells the cases apart: \\code{-1} no solution, \\code{0} infinitely many.')),
   ('math/mod_elimination.cpp', '$O(n^3)$', T(
    'Same, modulo a prime \\code{MOD} (global). \\code{A} is $n \\times (n{+}1)$, 1-indexed; ',
    'solved rows end with \\code{A.m[i][i] = 1} and the value in \\code{A.m[i][n+1]}. ',
    '\\code{lin\\_inv} fills inverses up to \\code{MOD} into \\code{inv[MAXN]}, so ',
    '\\code{MOD} must be below \\code{MAXN}.')),
   ('math/xor_gauss.cpp', '$O(n^2 m / 64)$', T(
    'XOR equations: row $i$ is a \\code{bitset}, bit $j$ = variable $j$ is in equation $i$, ',
    'bit $n{+}1$ = right side. 1-indexed, \\code{A.row} equations, \\code{A.col} $= n+1$. ',
    'After \\code{xor\\_gauss(A)}: \\code{k} = how many of the first equations were needed, ',
    '\\code{k == m+1} means the system does not fix every variable; else ',
    '$x_i$ = \\code{A.m[i][n+1]}.')),
   ('math/xor_basis.cpp', '$O(64 n)$', T(
    '\\code{get\\_basis()} turns \\code{a[1..n]} into a reduced basis \\code{a[1..len]}; ',
    'every subset XOR is a XOR of basis elements. Largest subset XOR = XOR of all basis ',
    'elements; \\code{zero} = 0 can be made from a non-empty subset; $2^{len}$ distinct ',
    'values exist.')),
   ('math/zero_one_partition.cpp', '$O(n \\log n \\log)$', T(
    '01 fractional programming: drop \\code{k} of the $n$ pairs to maximise ',
    '$\\sum a_i / \\sum b_i$. Binary search the ratio $x$: \\code{check(x)} asks whether the ',
    '$n-k$ largest $a_i - x b_i$ sum to $\\ge 0$. Returns \\code{round(100 x)} (that problem ',
    'wanted percent); \\code{start} is the ratio itself.')),
 ]),
]

# rule 5: usage-only functions to leave out, by the start of their first line
DROP = {
  'geomtery/Line_and_Vectors.cpp': ['void init', 'void solve', 'int main'],
  'geomtery/Polygon.cpp': ['void init', 'void solve', 'int main'],
  'geomtery/Convex_Hull.cpp': ['void solve', 'int main'],
  'geomtery/Lattic_points.cpp': ['void solve', 'int main'],
  'geomtery/Closest_pair.cpp': ['void init', 'void solve', 'int main'],
  'geomtery/Half_Plane_Intersect.cpp': ['void solve', 'int main'],
  'geomtery/Circle.cpp': ['void init', 'void solve', 'int main'],
  'geomtery/min_cover_circle.cpp': ['void solve', 'int main'],
  'math/exgcd.cpp': ['int main'],
  'math/Fastmod.cpp': ['int main'],
  'math/Phi.cpp': ['int main'],
  'math/Prime_factorization.cpp': ['int main'],
  'math/miller_rabin.cpp': ['int main'],
  'math/Pollard_Rho.cpp': ['int main'],
  'math/Matrix.cpp': ['int main'],
  'math/mod_elimination.cpp': ['int main'],
  'math/xor_gauss.cpp': ['int main'],
  'math/xor_basis.cpp': ['int main'],
  'math/zero_one_partition.cpp': ['int main'],
}

def code_part(l):
    out, i, q = [], 0, None
    while i < len(l):
        c = l[i]
        if q:
            if c == '\\': i += 2; continue
            if c == q: q = None
        else:
            if c in '"\'': q = c
            elif l.startswith('//', i): break
            elif l.startswith('/*', i):
                j = l.find('*/', i + 2)
                if j < 0: break
                i = j + 2; continue
            else: out.append(c)
        i += 1
    return ''.join(out)

CTRL = re.compile(r'^(if|for|while|else)\b')

def collapse(lines):
    """rule 2. Repeats until nothing changes (nested single statements fold
    inside out)."""
    changed = True
    while changed:
        changed = False
        out, i = [], 0
        while i < len(lines):
            if i + 2 < len(lines):
                h, s, c = lines[i], lines[i + 1], lines[i + 2]
                hs, ss, cs = h.strip(), s.strip(), c.strip()
                head_ok = (h.rstrip().endswith('{') and code_part(h).rstrip().endswith('{')
                           and (CTRL.match(hs) or re.match(r'^}\s*else\b', hs))
                           and code_part(h).count('{') == 1 and '}' not in code_part(h)[:-1]
                           or (h.rstrip().endswith('{') and re.match(r'^}\s*else\b', hs)
                               and code_part(h).count('{') == 1))
                cp = code_part(s).strip()
                stmt_ok = (cp.endswith(';') and '{' not in cp and '}' not in cp
                           and not CTRL.match(ss) and not ss.startswith('//')
                           and not ss.startswith('#') and ss != ''
                           and not re.match(r'^(case|default)\b', ss))
                if head_ok and stmt_ok and (cs == '}' or re.match(r'^}\s*else\b', cs)):
                    ind = h[:len(h) - len(h.lstrip())]
                    out.append(h.rstrip()[:-1].rstrip() + ' ' + ss)
                    if cs == '}':
                        i += 3
                    else:   # '}else{' / '}else if(..){' -> the else continues
                        out.append(ind + cs[1:].lstrip())
                        i += 3
                    changed = True
                    continue
            out.append(lines[i]); i += 1
        lines = out
    return lines

def items_of(lines):
    """split into top-level items: ('one', [line]) or ('blk', [lines])."""
    it, i, n = [], 0, len(lines)
    while i < n:
        l = lines[i]
        top = l and not l[0].isspace() and not l.startswith('#')
        cp = code_part(l) if top else ''
        nxt = lines[i + 1].strip() if i + 1 < n else ''
        if top and ('{' in cp or (nxt.startswith('{') and not cp.rstrip().endswith(';')
                                  and not l.startswith('//'))):
            d, j, started = 0, i, False
            while j < n:
                c = code_part(lines[j])
                d += c.count('{') - c.count('}')
                started = started or '{' in c
                if started and d <= 0: break
                j += 1
            it.append(('blk', lines[i:j + 1])); i = j + 1
        else:
            it.append(('one', [l])); i += 1
    out = []
    for kind, ls in it:
        if kind == 'blk':
            com = []
            while out and out[-1][0] == 'one' and out[-1][1][0].startswith('//'):
                com = out.pop()[1] + com
            out.append(('blk', com + ls, False))
        else:
            out.append(('one', ls, False))
    return out

def head_of(blk):
    return [l for l in blk if not l.startswith('//')][0]

def stub(blk, where):
    body = [l for l in blk if not l.startswith('//')]
    first = body[0]
    k = first.find('{')
    head = first[:k + 1] if k >= 0 else first + ' {'
    semi = ';' if body[-1].rstrip().endswith('};') else ''
    return head + ' /* see ' + where + ' */ }' + semi

def tex_escape(s):
    return s.replace('_', '\\_')

def read_lines(path):
    raw = open(os.path.join(RU, path), encoding='utf-8', newline='').read()
    return [l.rstrip('\r') for l in raw.split('\n')]

# ---------------------------------------------------------------- rule-2 check
def asm(src, tmp, tag):
    f = os.path.join(tmp, tag + '.cpp'); s = os.path.join(tmp, tag + '.s')
    open(f, 'w', encoding='utf-8', newline='\n').write(src)
    r = subprocess.run(['g++', '-std=gnu++17', '-O0', '-S', '-g0', '-w', '-o', s, f],
                       capture_output=True, text=True)
    if r.returncode != 0: return None
    # drop the file-name line, it differs by construction
    return [l for l in open(s, encoding='utf-8', errors='replace').read().split('\n')
            if not l.lstrip().startswith(('.file', '.ident'))]

TOK = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\w+|\S', re.S)
def tokens_wo_braces_diff(a, b):
    ta = [t for t in TOK.findall('\n'.join(a))]
    tb = [t for t in TOK.findall('\n'.join(b))]
    # the only allowed difference is removed '{' '}' pairs
    i = j = 0
    while i < len(ta) and j < len(tb):
        if ta[i] == tb[j]: i += 1; j += 1
        elif ta[i] in '{}': i += 1
        else: return False
    return j == len(tb) and all(t in '{}' for t in ta[i:])

def check_collapse(files):
    tmp = tempfile.mkdtemp()
    bad = []
    for path in files:
        if path.endswith('.sh'): continue
        orig = read_lines(path)
        ne = [l for l in orig if l.strip() != '']
        col = collapse(ne)
        if col == ne: continue
        a = asm('\n'.join(orig) + '\n', tmp, 'a')
        b = asm('\n'.join(col) + '\n', tmp, 'b')
        if a is None:
            ok = tokens_wo_braces_diff(ne, col)
            print('  %-36s does not compile on its own; token check: %s'
                  % (path, 'ok' if ok else 'FAIL'))
            if not ok: bad.append(path)
        elif a != b:
            bad.append(path)
    assert not bad, ('collapse changed the compiled code of', bad)

# ---------------------------------------------------------------------- main
def main():
    seen_blk, seen_hdr = {}, {}
    report, allfiles = [], []
    for out, title, files in PARTS:
        tex = []
        if title: tex += ['\\section{%s}' % title, '']
        for path, cplx, desc in files:
            allfiles.append(path)
            lines = collapse([l for l in read_lines(path) if l.strip() != ''])
            is_sh = path.endswith('.sh')
            its = items_of(lines)
            first_blk = next((k for k, (t, _, _) in enumerate(its) if t == 'blk'), len(its))
            body, removed_from, nstub, dropped_hdr, dropped_blk = [], [], 0, [], []
            for k, (t, ls, _) in enumerate(its):
                if is_sh:
                    body += ls; continue
                if t == 'one' and k < first_blk:
                    l = ls[0]
                    if l in seen_hdr:
                        if seen_hdr[l] not in removed_from: removed_from.append(seen_hdr[l])
                        dropped_hdr.append(l)
                        continue
                    seen_hdr[l] = path
                    body.append(l)
                elif t == 'blk':
                    if any(head_of(ls).startswith(p) for p in DROP.get(path, [])):
                        dropped_blk.append(ls); continue
                    key = '\n'.join(ls)
                    if key in seen_blk:
                        body.append(stub(ls, seen_blk[key])); nstub += 1
                    else:
                        seen_blk[key] = path
                        body += ls
                else:
                    body += ls
            if removed_from:
                body.insert(0, '// header lines: see ' + ', '.join(removed_from))
            verify(path, lines, body, seen_blk, dropped_hdr, dropped_blk, is_sh)
            report.append((path, len(read_lines(path)), len(body), nstub, len(dropped_blk)))
            name = path.split('/')[-1]
            tex.append('\\entry{%s}{%s}{%s}' % (tex_escape(name),
                       ('Ruiming \\quad ' + cplx) if cplx else 'Ruiming', desc))
            tex.append('')
            tex.append('%NOCOMPILE')
            tex.append('%VERBATIM')
            lang = '[language=bash,breaklines=true]' if is_sh else '[breaklines=true]'
            tex.append('\\begin{lstlisting}' + lang)
            tex += body
            tex.append('\\end{lstlisting}')
            tex.append('')
        dest = os.path.join(SRC, out)
        if out == '11-geometry.tex':
            old = open(dest, encoding='utf-8').read()
            mk = '% ---- other geometry (not Ruiming) ----'
            rest = old[old.index(mk):] if mk in old else mk + '\n'
            text = '\n'.join(tex) + '\n' + rest
        else:
            text = '\n'.join(tex) + '\n'
        open(dest, 'w', encoding='utf-8', newline='\n').write(text)
    check_collapse(allfiles)
    for p, a, b, s, d in report:
        print('%-36s %4d lines -> %4d printed, %2d stubs, %d demo fns left out' % (p, a, b, s, d))
    print('total printed lines:', sum(r[2] for r in report))

def verify(path, lines, body, seen_blk, dropped_hdr, dropped_blk, is_sh):
    """body (stubs expanded, dropped parts put back) must equal lines."""
    if is_sh:
        assert body == lines, path; return
    inv = {}
    for key, where in seen_blk.items():
        ls = key.split('\n'); inv.setdefault(stub(ls, where), []).append(ls)
    exp = []
    for l in body:
        if l.startswith('// header lines: see ') and l not in lines: continue
        if l in inv and l not in lines:
            assert len(inv[l]) == 1, ('ambiguous stub', l)
            exp += inv[l][0]
        else:
            exp.append(l)
    want, hd = [], list(dropped_hdr)
    its = items_of(lines)
    first_blk = next((k for k, (t, _, _) in enumerate(its) if t == 'blk'), len(its))
    for k, (t, ls, _) in enumerate(its):
        if t == 'one' and k < first_blk and hd and ls[0] == hd[0]:
            hd.pop(0); continue
        if t == 'blk' and any(ls == d for d in dropped_blk): continue
        want += ls
    assert not hd, ('dropped header lines not found', path)
    assert exp == want, ('MISMATCH', path)

if __name__ == '__main__':
    main()
