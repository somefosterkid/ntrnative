#!/usr/bin/env python3
"""hg_sweep_merge.py RAW OUT OLD: merge the passes of a HG/SS sweep (RAW/<pass>/ = hg_sweep / hg_app_sweep / hg_battle_sweep out dirs, SoulSilver passes end in _ss)
into OUT/sweep.tsv + crash_groups.tsv (top function + bt top 3) and OUT/compare.txt against an older sweep (OLD = its dir with sweep.tsv + crash_groups.tsv). tools/sweep8_job.sh runs the passes in a slot."""
import sys, os, re, collections

raw, out, old = sys.argv[1], sys.argv[2], sys.argv[3]
# name, dirs (merged), id col, name col, verdict col, frame col, top col, bt_id col, bt path, sweep-7 pass name
PASSES = [
    ('apps_hg', ['apps_hg'], 0, 1, 2, 3, 4, 5, lambda r: f'plays/{r[0]}/bt.txt', 'appsw'),
    ('apps_ss', ['apps_ss'], 0, 1, 2, 3, 4, 5, lambda r: f'plays/{r[0]}/bt.txt', None),
    ('maps_hg', ['maps_hg'], 0, 1, 2, 3, 4, 5, lambda r: f'maps/{r[0]}/bt.txt', 'maps'),
    ('party_hg', ['party_hg'], 0, 1, 2, 3, 4, 5, lambda r: f'maps/{r[0]}/bt.txt', 'party'),
    ('maps_ss', ['maps_ss', 'maps_ss_a', 'maps_ss_b'], 0, 1, 2, 3, 4, 5, lambda r: f'maps/{r[0]}/bt.txt', None),
    ('party_ss', ['party_ss'], 0, 1, 2, 3, 4, 5, lambda r: f'maps/{r[0]}/bt.txt', None),
    ('battleB_hg', ['battleB_hg'], 1, 2, 3, 4, 5, 6, lambda r: f'runs/{r[0]}-{r[1]}/bt.txt', 'battleB'),
    ('battleA_hg', ['battleA_hg'], 1, 2, 3, 4, 5, 6, lambda r: f'runs/{r[0]}-{r[1]}/bt.txt', 'battleA'),
    ('battleB_ss', ['battleB_ss'], 1, 2, 3, 4, 5, 6, lambda r: f'runs/{r[0]}-{r[1]}/bt.txt', None),
]
BAD = ('crash', 'heap', 'hang', 'timeout', 'cap')


def fn(s):
    return re.sub(r'\s*\(.*?\)\s*$', '', s.strip())


rows, groups, counts = [], collections.OrderedDict(), collections.OrderedDict()
for name, dirs, ci, cn, cv, cf, ct, cb, btp, _old in PASSES:
    c = None
    for d in dirs:
        p = os.path.join(raw, d, 'sweep.tsv')
        if not os.path.exists(p):
            continue
        c = c or collections.Counter()
        for l in open(p).read().splitlines()[1:]:
            r = l.split('\t')
            r += [''] * (12 - len(r))
            v = r[cv]
            c[v] += 1
            bt = ''
            if v in BAD:
                bp = os.path.join(raw, d, btp(r))
                if os.path.exists(bp):
                    ls = open(bp).read().split('\n')
                    bt = ls[1] if len(ls) > 1 else ''
            rows.append((name, r[ci], r[cn], v, r[cf], r[ct], r[cb], bt))
            if v in BAD:
                fr = [fn(x) for x in bt.split(' <- ')][:3] if bt else [r[ct]]
                key = (v, ' <- '.join(fr) if v == 'crash' else r[ct])
                g = groups.setdefault(key, {'n': 0, 'passes': collections.Counter(), 'ids': [], 'bt': bt, 'top': r[ct]})
                g['n'] += 1
                g['passes'][name] += 1
                g['ids'].append(f'{name}:{r[ci]}')
    counts[name] = c
os.makedirs(out, exist_ok=True)
with open(os.path.join(out, 'sweep.tsv'), 'w') as f:
    f.write('pass\tid\tname\tverdict\tframe\ttop\tbt_id\tbacktrace\n')
    for r in rows:
        f.write('\t'.join(r) + '\n')
gs = sorted(groups.items(), key=lambda kv: -kv[1]['n'])
with open(os.path.join(out, 'crash_groups.tsv'), 'w') as f:
    f.write('rank\tcount\tkind\ttop_function\tbt_top3\tpasses\tfirst_ids\tbacktrace\n')
    for i, ((k, t3), g) in enumerate(gs, 1):
        ps = ','.join(f'{p}:{n}' for p, n in g['passes'].items())
        f.write(f"{i}\t{g['n']}\t{k}\t{g['top']}\t{t3}\t{ps}\t{','.join(g['ids'][:12])}\t{g['bt']}\n")

# ---- compare with sweep 7
o_rows = [l.split('\t') + [''] * 8 for l in open(os.path.join(old, 'sweep.tsv')).read().splitlines()[1:]]
o_by = collections.defaultdict(dict)       # sweep-7 pass -> id -> (verdict, top, bt)
for r in o_rows:
    o_by[r[0]][r[1]] = (r[3], r[5], r[7])
o_groups = {}
for l in open(os.path.join(old, 'crash_groups.tsv')).read().splitlines()[1:]:
    r = l.split('\t') + [''] * 8
    o_groups[(r[2], r[4] if r[2] == 'crash' else r[3])] = (int(r[1]), r[6])
n_by = collections.defaultdict(dict)
for r in rows:
    n_by[r[0]][r[1]] = (r[3], r[5], r[7])

L = []
L.append('COMPARE sweep 8 vs sweep 7 (HG pass names: maps_hg=maps, party_hg=party, apps_hg=appsw, battleB_hg=battleB, battleA_hg=battleA)')
L.append('')
L.append('verdict counts per pass')
for name, dirs, *_r in PASSES:
    oname = _r[-1]
    c = counts.get(name)
    if c is None:
        continue
    oc = collections.Counter(v[0] for v in o_by[oname].values()) if oname else None
    L.append(f'  {name:11} sweep 8: {dict(c)} ({sum(c.values())})' + (f'   sweep 7: {dict(oc)} ({sum(oc.values())})' if oc else '   (no sweep-7 twin)'))
L.append('')
L.append('crash groups vs sweep 7 (HG passes only; key = verdict + bt top 3)')
new_keys_hg = {}
for (k, t3), g in groups.items():
    hg_n = sum(n for p, n in g['passes'].items() if not p.endswith('_ss'))
    if hg_n:
        new_keys_hg[(k, t3)] = (hg_n, [i for i in g['ids'] if not i.split(':')[0].endswith('_ss')])
ran_old = {_r[-1] for name, dirs, *_r in PASSES if _r[-1] and counts.get(name) is not None}
for key, (n, ids) in sorted(o_groups.items(), key=lambda kv: -kv[1][0]):
    gp = {i.split(':')[0] for i in ids.split(',') if ':' in i}
    if gp and not (gp & ran_old):
        L.append(f'  NOT RUN  {n:3}         {key[1][:130]}  (pass {",".join(sorted(gp))} not in this sweep)')
    elif key in new_keys_hg:
        L.append(f'  REMAINS  {o_groups[key][0]:3} -> {new_keys_hg[key][0]:3}  {key[1][:130]}  ids {",".join(new_keys_hg[key][1][:5])}')
    else:
        L.append(f'  GONE     {n:3} ->   0  {key[1][:130]}  (was {ids[:60]})')
for key, (n, ids) in sorted(new_keys_hg.items(), key=lambda kv: -kv[1][0]):
    if key not in o_groups:
        L.append(f'  NEW      {"":3}    {n:3}  {key[1][:130]}  ids {",".join(ids[:5])}')
L.append('')
L.append('per-id changes (same pass, HG)')
for name, dirs, *_r in PASSES:
    oname = _r[-1]
    if not oname or name not in n_by:
        continue
    fixed = [i for i, v in o_by[oname].items() if v[0] in BAD and i in n_by[name] and n_by[name][i][0] not in BAD]
    broke = [i for i, v in n_by[name].items() if v[0] in BAD and i in o_by[oname] and o_by[oname][i][0] not in BAD]
    still = [i for i, v in n_by[name].items() if v[0] in BAD and i in o_by[oname] and o_by[oname][i][0] in BAD]
    L.append(f'  {name}: bad->ok {len(fixed)} {fixed[:40]}; ok->bad {len(broke)} {broke[:40]}; bad->bad {len(still)} {still[:40]}')
    for lbl in ('stuck-walk', 'no-spawn', 'entered-wrong'):
        if name.startswith('maps'):
            a = {i for i, v in o_by[oname].items() if v[0] == lbl}
            b = {i for i, v in n_by[name].items() if v[0] == lbl}
            L.append(f'    {lbl}: sweep 7 {len(a)}, sweep 8 {len(b)}; only 7: {sorted(a - b, key=int)[:15]}; only 8: {sorted(b - a, key=int)[:15]}')
open(os.path.join(out, 'compare.txt'), 'w').write('\n'.join(L) + '\n')
print('\n'.join(L[:6]))
print()
for i, ((k, t3), g) in enumerate(gs, 1):
    print(f"{i:3} {g['n']:4} {k:7} {t3[:150]}  [{','.join(f'{p}:{n}' for p, n in g['passes'].items())}] {','.join(g['ids'][:4])}")
