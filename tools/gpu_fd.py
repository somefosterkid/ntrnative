#!/usr/bin/env python3
"""gpu_fastdiff over many captures in parallel, one summary table, and a before/after compare against a saved baseline.

    python3 tools/gpu_fd.py run CAPDIR... [--scale N] [--aa A] [--label L] [--save F.json] [--base F.json] [--exe PATH]
                                          [--filter SUBSTR] [--every K] [--nobuild] [--worst N]
    python3 tools/gpu_fd.py cmp BASE.json NEW.json [--worst N]

run: builds build/gpu/gpu_fastdiff (ninja, no-op when current; --nobuild skips), runs it over every *.vcs in the CAPDIRs
(--every K: each Kth only; --filter: names containing SUBSTR) with $BW_JOBS jobs, each under its own xvfb-run, and prints the
sums: gaps, extras, specks, dark specks (a speck where Fast is dark and Exact lighter by 24+ in the channel sum: the darkper
count), the mean and max "differ" % and the number of captures with any gap/extra/speck. --aa: 1 = off (default), 2 = 4 samples.
--save writes the per-capture numbers; --base compares this run with such a file (same as `cmp`): the sums before -> after,
and the captures whose gaps/extras/specks/dark/differ changed (the --worst N largest changes, default 10).
"""
import json, os, re, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FIELDS = ('gaps', 'extras', 'specks', 'dark')


def one(exe, cap, scale, aa, work):
    out = subprocess.run(['xvfb-run', '-a', exe, cap, os.path.join(work, os.path.basename(cap)[:-4]), '--scale', str(scale),
                          '--aa', str(aa), '--api', 'gl', '--nopng', '--polys'], capture_output=True, text=True).stdout
    r = {'differ': None, 'gaps': 0, 'extras': 0, 'specks': 0, 'dark': 0}
    for ln in out.splitlines():
        m = re.match(r'gpu_fastdiff: .* differ \d+ \(([\d.]+)%\).*gaps (\d+); extras (\d+); specks (\d+)', ln)
        if m:
            r.update(differ=float(m[1]), gaps=int(m[2]), extras=int(m[3]), specks=int(m[4]))
        m = re.match(r'speck \S+ fast poly -?\d+ ([0-9a-f]{6}) exact poly -?\d+ ([0-9a-f]{6})', ln)
        if m:
            s = [sum(int(x[i:i + 2], 16) for i in (0, 2, 4)) for x in m.groups()]
            if s[0] <= 24 and s[1] >= s[0] + 24:
                r['dark'] += 1
    return r


def sums(res):
    ok = [r for r in res.values() if r['differ'] is not None]
    t = {f: sum(r[f] for r in ok) for f in FIELDS}
    t.update(n=len(res), failed=len(res) - len(ok), differ_mean=sum(r['differ'] for r in ok) / max(1, len(ok)),
             differ_max=max((r['differ'] for r in ok), default=0),
             caps_with=sum(1 for r in ok if r['gaps'] or r['extras'] or r['specks']))
    return t


def table(t, label=''):
    print(f"{label} {t['n']} captures ({t['failed']} failed): gaps {t['gaps']}, extras {t['extras']}, specks {t['specks']}, "
          f"dark specks {t['dark']}; differ mean {t['differ_mean']:.3f}% max {t['differ_max']:.2f}%; "
          f"captures with a gap/extra/speck {t['caps_with']}")


def compare(base, new, worst):
    a, b = sums(base['res']), sums(new['res'])
    table(a, 'before:'), table(b, 'after: ')
    rows = []
    for k in sorted(set(base['res']) & set(new['res'])):
        x, y = base['res'][k], new['res'][k]
        d = {f: y[f] - x[f] for f in FIELDS}
        dd = (y['differ'] or 0) - (x['differ'] or 0)
        if any(d.values()) or abs(dd) > 1e-9:
            rows.append((sum(abs(v) for v in d.values()), abs(dd), k, d, dd))
    print(f'changed: {len(rows)} of {len(set(base["res"]) & set(new["res"]))} captures'
          f'; only before: {len(set(base["res"]) - set(new["res"]))}, only after: {len(set(new["res"]) - set(base["res"]))}')
    for _, _, k, d, dd in sorted(rows, reverse=True)[:worst]:
        print(f"  {k}: gaps {d['gaps']:+d} extras {d['extras']:+d} specks {d['specks']:+d} dark {d['dark']:+d} differ {dd:+.3f}%")


def run(a):
    opt = {'--scale': '3', '--aa': '1', '--label': '', '--save': '', '--base': '', '--exe': '', '--filter': '', '--every': '1',
           '--worst': '10'}
    caps, nobuild, i = [], False, 0
    while i < len(a):
        if a[i] == '--nobuild':
            nobuild = True
        elif a[i] in opt:
            i += 1
            opt[a[i - 1]] = a[i]
        else:
            caps.append(a[i])
        i += 1
    exe = opt['--exe'] or os.path.join(REPO, 'build/gpu/gpu_fastdiff')
    if not opt['--exe'] and not nobuild:
        if not os.path.exists(os.path.join(REPO, 'build/gpu/build.ninja')):
            subprocess.run(['meson', 'setup', 'build/gpu', 'port/gpu'], cwd=REPO, check=True, capture_output=True)
        r = subprocess.run(['ninja', '-C', 'build/gpu', 'gpu_fastdiff'], cwd=REPO, capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stdout[-1500:])
    files = sorted(os.path.join(d, f) for d in caps if os.path.isdir(d) for f in os.listdir(d) if f.endswith('.vcs') and opt['--filter'] in f)
    files = files[::int(opt['--every'])]
    if not files:
        sys.exit('gpu_fd: no captures')
    work = tempfile.mkdtemp(prefix='gpu_fd')
    jobs = int(os.environ.get('BW_JOBS', '8'))
    with ThreadPoolExecutor(jobs) as ex:
        out = list(ex.map(lambda c: one(exe, c, opt['--scale'], opt['--aa'], work), files))
    res = {os.path.basename(os.path.dirname(c)) + '/' + os.path.basename(c)[:-4]: r for c, r in zip(files, out)}
    new = {'scale': opt['--scale'], 'aa': opt['--aa'], 'res': res}
    if opt['--save']:
        os.makedirs(os.path.dirname(os.path.abspath(opt["--save"])), exist_ok=True)
        json.dump(new, open(opt['--save'], 'w'), indent=0)
    table(sums(res), opt['--label'] or f"scale {opt['--scale']} aa {opt['--aa']}:")
    if opt['--base']:
        compare(json.load(open(opt['--base'])), new, int(opt['--worst']))


def main():
    a = sys.argv[1:]
    if a[:1] == ['run'] and len(a) > 1:
        run(a[1:])
    elif a[:1] == ['cmp'] and len(a) >= 3:
        w = int(a[a.index('--worst') + 1]) if '--worst' in a else 10
        compare(json.load(open(a[1])), json.load(open(a[2])), w)
    else:
        sys.exit(__doc__)


if __name__ == '__main__':
    main()
