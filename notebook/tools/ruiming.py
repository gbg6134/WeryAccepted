r"""Generate src/*-ruiming-*.tex from Ruiming's files, VERBATIM.

Rules (agreed with the team):
  * not a single character of Ruiming's code is changed, except:
  * empty / whitespace-only lines are dropped;
  * a top-level block (function or struct, with the // comment lines directly
    above it) that is IDENTICAL to one already printed earlier is replaced by a
    one-line stub that keeps its name:   <first line up to '{'> /* see FILE */ }
  * header lines (everything before the first function/struct) that are
    identical to header lines already printed in an earlier file are dropped,
    and the file starts with a comment saying where they are.
Long lines are wrapped visually by the printer (breaklines), not in the text.

Run: python tools/ruiming.py   (then tools/build.sh). It also verifies that every
file can be rebuilt exactly from the output (stubs expanded, header lines put back).
"""
import os, re, sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
RU = os.path.join(ROOT, 'Ruiming')
SRC = os.path.join(ROOT, 'notebook', 'src')

# (output file, section title or None, [(path, one-line description)])
PARTS = [
 ('01b-ruiming-setup.tex', None, [
   ('NewCodeforces.cpp', 'Ruiming\'s contest skeleton: header, \\code{init()} to clear the arrays, \\code{solve()} per test case.'),
   ('NewStresstest.sh', 'Stress test: compares \\code{sol.cpp} against \\code{brute.cpp} on random inputs.'),
   ('Stresstest.sh', 'Short version of the stress-test loop.'),
 ]),
 ('11-geometry.tex', 'Geometry (Ruiming)', [
   ('geomtery/Line_and_Vectors.cpp', 'Points, vectors, lines and segments: dot, cross, angles, rotation, point-line relations, projection, reflection, intersections. \\code{ldcmp} compares with \\code{eps}.'),
   ('geomtery/Polygon.cpp', 'Polygon area, point in polygon, angle sort (\\code{cmp}).'),
   ('geomtery/Convex_Hull.cpp', 'Convex hull (\\code{Convex\\_Hull}), with point in polygon and polygon area.'),
   ('geomtery/Lattic_points.cpp', 'Exact integer geometry on \\code{PointLL}: dot, cross, squared distance, area, lattice points.'),
   ('geomtery/Closest_pair.cpp', 'Closest pair of points, divide and conquer, $O(n \\log n)$.'),
   ('geomtery/Half_Plane_Intersect.cpp', 'Half-plane intersection: area of the intersection of convex polygons.'),
   ('geomtery/Circle.cpp', 'Circles: point / line / segment vs circle, line-circle intersection points, circumcentre.'),
   ('geomtery/min_cover_circle.cpp', 'Minimum enclosing circle, randomised, expected $O(n)$.'),
 ]),
 ('09a-ruiming-math.tex', 'Math (Ruiming)', [
   ('math/exgcd.cpp', 'Extended Euclid; prints the inverse of \\code{a} mod \\code{b}.'),
   ('math/CRT.cpp', 'Chinese remainder theorem by stepping through residues (small coprime moduli).'),
   ('math/Fast_gcd.cpp', 'Binary gcd with \\code{\\_\\_builtin\\_ctz}.'),
   ('math/Fastmod.cpp', '\\code{mul} (safe \\code{a*b\\%m}) and \\code{fast} ($x^p \\bmod m$).'),
   ('math/Eulerthm.cpp', 'Euler\'s theorem: $a^b \\bmod m$ for a huge exponent $b$.'),
   ('math/Phi.cpp', 'Euler\'s $\\varphi(n)$ for one $n$.'),
   ('math/Linearphi.cpp', '$\\varphi$ for all numbers up to $n$ (linear sieve).'),
   ('math/Euler_sieve.cpp', 'Linear (Euler) sieve of primes.'),
   ('math/Factorization.cpp', 'Factorises every number below \\code{MAXN} by trial division into \\code{map}s.'),
   ('math/Prime_factorization.cpp', 'Prime factorisation of one number, \\code{factor(n)}.'),
   ('math/miller_rabin.cpp', 'Miller-Rabin primality test.'),
   ('math/Pollard_Rho.cpp', 'Pollard rho factorisation for numbers up to $\\sim 10^{18}$.'),
   ('math/Combinumber.cpp', 'Pascal\'s triangle \\code{c[i][j]} mod \\code{M}.'),
   ('math/Combinumber_inv.cpp', 'Binomial coefficients with modular inverses.'),
   ('math/Linearinverse.cpp', 'Inverses of $1..n$ modulo a prime in $O(n)$.'),
   ('math/Matrix.cpp', 'Matrix multiplication and fast matrix power mod \\code{MOD}.'),
   ('math/gaussjordanelimination.cpp', 'Gauss-Jordan elimination over the reals.'),
   ('math/mod_elimination.cpp', 'Gaussian elimination modulo a prime.'),
   ('math/xor_gauss.cpp', 'Gaussian elimination over XOR (bitsets).'),
   ('math/xor_basis.cpp', 'XOR basis (linear basis); prints the maximum subset XOR.'),
   ('math/zero_one_partition.cpp', '0/1 fractional programming (binary search on the ratio).'),
 ]),
]

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
    # attach // comment lines directly above a block
    out = []
    for kind, ls in it:
        if kind == 'blk':
            com = []
            while out and out[-1][0] == 'one' and out[-1][1][0].startswith('//') \
                    and out[-1][2] is False:
                com = out.pop()[1] + com
            out.append(('blk', com + ls, False))
        else:
            out.append(('one', ls, False))
    return out

def stub(blk, where):
    body = [l for l in blk if not l.startswith('//')]
    first = body[0]
    k = first.find('{')
    head = first[:k + 1] if k >= 0 else first + ' {'
    semi = ';' if body[-1].rstrip().endswith('};') else ''
    return head + ' /* see ' + where + ' */ }' + semi

def tex_escape(s):
    return s.replace('_', '\\_')

def main():
    seen_blk, seen_hdr = {}, {}
    report = []
    for out, title, files in PARTS:
        tex = []
        if title: tex += ['\\section{%s}' % title, '']
        for path, desc in files:
            raw = open(os.path.join(RU, path), encoding='utf-8', newline='').read()
            orig = [l.rstrip('\r') for l in raw.split('\n')]
            lines = [l for l in orig if l.strip() != '']
            its = items_of(lines)
            is_sh = path.endswith('.sh')
            first_blk = next((k for k, (t, _, _) in enumerate(its) if t == 'blk'), len(its))
            body, removed_from, nstub, dropped = [], [], 0, []
            for k, (t, ls, _) in enumerate(its):
                if is_sh:
                    body += ls; continue
                if t == 'one' and k < first_blk:
                    l = ls[0]
                    if l in seen_hdr:
                        if seen_hdr[l] not in removed_from: removed_from.append(seen_hdr[l])
                        dropped.append(k)
                        continue
                    seen_hdr[l] = path
                    body.append(l)
                elif t == 'blk':
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
            # ---- verify: rebuild the file from body ----
            verify(path, lines, body, seen_blk, seen_hdr, is_sh, dropped)
            report.append((path, len(orig), len(body), nstub))
            name = path.split('/')[-1]
            tex.append('\\entry{%s}{Ruiming, verbatim}{%s}' % (tex_escape(name), desc))
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
            # keep whatever non-Ruiming geometry follows the marker
            old = open(dest, encoding='utf-8').read()
            mk = '% ---- other geometry (not Ruiming) ----'
            rest = old[old.index(mk):] if mk in old else mk + '\n'
            text = '\n'.join(tex) + '\n' + rest
        else:
            text = '\n'.join(tex) + '\n'
        open(dest, 'w', encoding='utf-8', newline='\n').write(text)
    for p, a, b, s in report:
        print('%-36s %4d lines -> %4d printed, %2d stubs' % (p, a, b, s))
    print('total printed lines:', sum(r[2] for r in report))

def verify(path, lines, body, seen_blk, seen_hdr, is_sh, dropped):
    if is_sh:
        assert body == lines, path; return
    # expand stubs
    inv = {}
    for key, where in seen_blk.items():
        ls = key.split('\n')
        inv.setdefault(stub(ls, where), []).append(ls)
    exp = []
    for l in body:
        if l.startswith('// header lines: see ') and l not in lines:
            continue
        if l in inv and l not in lines:
            cands = inv[l]
            assert len(cands) == 1, ('ambiguous stub', l)
            exp += cands[0]
        else:
            exp.append(l)
    # original minus the header lines that were dropped (item index k = line k,
    # because every header item is a single line)
    want = [l for k, l in enumerate(lines) if k not in set(dropped)]
    assert exp == want, ('MISMATCH', path)
    for k in dropped:
        h = lines[k]
        assert h in seen_hdr and seen_hdr[h] != path, ('dropped header not printed elsewhere', h)

if __name__ == '__main__':
    main()
