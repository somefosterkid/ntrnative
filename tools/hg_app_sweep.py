#!/usr/bin/env python3
"""hg_app_sweep.py: the HG/SS app sweep: deep UI paths from a field save with a party, not maps (docs/hgss.md "App sweep").
Crash finding only (a throwaway save and test cheats; never stored, never in a route chain).

    python3 tools/hg_app_sweep.py --plays all -j 6 --out /tmp/appsweep --heapcheck
    python3 tools/hg_app_sweep.py --plays dex,bag,pc-box --out DIR          # ids, or a group prefix: x-, pc-, mart, field-
    python3 tools/hg_app_sweep.py --list                                    # the plays, their save, warp and needs
    python3 tools/hg_app_sweep.py --summary --out DIR                       # only (re)write the summary files

A play is one run of tools/port_run.py on the HG/SS exe (hg_sweep's runner conventions: --no-build, the reload keys of
hg_sweep.RELOAD, BWPORT_KEYS frame keys, --hang 30, the coverage file of the -Dhgss_cov=true build) from a save of the library
(hg-r30c: Route 30 with a party), with BWPORT_HG_CHEAT (port/game/route_gen4.c Pr4_AppCheat: items, badges, a full party
with the field moves, 30 boxed Pokemon, the Pokegear cards) and BWPORT_HG_WARP (a map and tile, or -1: stay) so a play can stand
next to a PC, a clerk, a cuttable tree. Keys are a small language, see Seq: `x+60 stylus:46,78+400 a+100 b*8+40`: NAME[*HOLDFRAMES][+WAITFRAMES]
per key, frames of the game's V-blank count (never wall time); `wait+N` waits.
A play REACHED its screen when every `need` function (exact name, or PREFIX*) is in the run's executed set (the coverage
build) or, on a build without coverage, never (reached stays `?`: look at final.png / shots). Verdicts: ok (reached, END
V-blank passed), unreached (ran to END, a needed function never ran: the keys missed the screen), crash, hang (no frame for
30 s), heap (BWPORT_HEAPCHECK), timeout (the frame cap END + 6000: no wall watchdog, a slow run is never killed or a finding:
tools/sweep_wall.py), no-spawn.
Out dir: sweep.tsv, crash_groups.tsv (by top function), summary.txt, plays/<id>/ (run.txt, warp.txt, cov.txt.gz, final.png,
bt.txt + log.txt.gz for a crash / hang / heap).
"""
import argparse
import collections
import concurrent.futures
import gzip
import hashlib
import os
import queue
import re
import shutil
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hg_sweep as H   # the runner conventions, crash_info (gdb backtrace -> top / text / id), Symbols (coverage), spawn
import sweep_wall      # no wall watchdog

REPO = H.REPO
START = 1600   # the first key's V-blank: the reload is over (hg_sweep.WALK_FROM)
CHEAT_ALL = 'items,badges,party,box,gear'
# the X menu's icons by stylus tap (hg_sweep apps_keys' rects: 4 down the left, 3 on the right)
ICON = {'dex': (46, 38), 'mon': (46, 78), 'bag': (46, 118), 'gear': (46, 158), 'card': (126, 38), 'save': (126, 78),
        'opt': (126, 118)}


def Seq(text, start=START):
    """'x+60 stylus:46,78+400 a b*8+40 wait+200' -> (BWPORT_KEYS items, the V-blank after the last key). Each token is
    NAME[*HOLD][+WAIT]: HOLD frames the key is held (default 4), WAIT frames to the next key (default 45). `wait` only waits;
    `icon:NAME` is the X menu's icon tap; `n*K` repeats are written out by `rep(...)` below."""
    f, out = start, []
    for tok in text.split():
        m = re.match(r'^(.*?)(?:\+(\d+))?$', tok)
        name, wait = m.group(1), int(m.group(2)) if m.group(2) else 45
        if name != 'wait':
            if name.startswith('icon:'):
                name = 'stylus:%d,%d' % ICON[name[5:]]
            hold = ''
            if '*' in name and not name.startswith('stylus:'):
                name, h = name.split('*')
                hold = '*f' + h
            out.append(f'{name}{hold}@f{f}')
        f += wait
    return ' '.join(out), f


def rep(tok, n):
    return ' '.join([tok] * n)


class Play:
    def __init__(self, pid, name, keys, need=(), warp=None, face=None, save='hg-r30c', cheat=CHEAT_ALL, tail=300, game='heartgold', env=None, script=None):
        self.id, self.name, self.save, self.cheat, self.game, self.env = pid, name, save, cheat, game, env or {}
        self.face = face   # 0 north, 1 south (default), 2 west, 3 east: the facing after the warp (BWPORT_HG_WARP_DIR)
        self.keys, end = Seq(keys)
        self.warp = warp   # (map, x, y) or None: no warp
        self.end = end + tail   # the exit V-blank
        self.need = list(need)
        self.script = script   # a script id the game starts itself (BWPORT_HG_SCRIPT, route_gen4.c Pr4_ScriptHook): 2010 = the Pokemon Center PC

    def reached(self, funcs):
        miss = []
        for n in self.need:
            ok = any(f.startswith(n[:-1]) for f in funcs) if n.endswith('*') else n in funcs
            if not ok:
                miss.append(n)
        return miss


def plays():
    P = []
    a = P.append
    MENU = 'x+80'
    # ---- the X menu's places (docs/hgss.md: the bottom screen's icons; keys' cursor does not follow, a stylus tap does)
    # the party after the cheat: slot 0 the save's starter, 1 Mew (Cut Surf Strength Flash), 2 Pikachu (Rock Smash Headbutt Fly
    # Waterfall), 3 Eevee (Defog Whirlpool Rock Climb Teleport), 4 Abra (Dig Sweet Scent), 5 Squirtle; the party menu's cursor:
    # right = slot 1, down = slot 2 ...; the context menu lists the field moves on the left, Summary / Switch / Item on the right
    a(Play('x-dex', 'Pokedex: list, entry, pages, cry', MENU + ' icon:dex+700 a+300 a+400 right+200 right+200 a+200 a+200 down+100 a+300 l+100 r+100 b+150 b+150 b+150 b+200', ['ov18_*']))
    a(Play('x-card', 'Trainer card: flip, badges', MENU + ' icon:card+700 a+300 a+300 a+300 b+200 b+200', ['TrainerCard_LaunchApp']))
    a(Play('x-mon-summary', 'Pokemon: summary pages 1-6, next mon', MENU + ' icon:mon+700 a+250 a+300 ' + rep('right+150', 6) + ' a+150 b+150 down+150 b+250 b+250 b+300', ['PokemonSummary_Main']))
    a(Play('x-mon-switch', 'Pokemon: switch order', MENU + ' icon:mon+700 a+250 down+60 a+300 right+100 a+400 b+250 b+300', []))
    a(Play('x-mon-item', 'Pokemon: item give / take', MENU + ' icon:mon+700 a+250 down+60 down+60 a+300 a+400 a+300 a+300 a+300 b+250 b+300', []))
    for i, (slot, mv) in enumerate([('right', 'cut'), ('right', 'surf'), ('right', 'strength'), ('right', 'flash'), ('down', 'rocksmash'), ('down', 'headbutt'),
                                     ('down', 'fly'), ('down', 'waterfall'), ('right+80 down', 'defog'), ('right+80 down', 'whirlpool'),
                                     ('right+80 down', 'rockclimb'), ('right+80 down', 'teleport'), ('down+60 down', 'dig'), ('down+60 down', 'sweetscent')]):
        k = i % 4 if i < 12 else i - 12
        a(Play('x-mon-' + mv, 'Pokemon: field move ' + mv + ' from the party menu', MENU + ' icon:mon+700 ' + slot + '+80 a+250 left+80 '
               + rep('down+60', k) + ' a+500 a+400 a+400 a+400 b+300 b+300 b+300', []))
    a(Play('x-bag-pockets', 'Bag: every pocket, the first item', MENU + ' icon:bag+700 ' + rep('a+250 b+200 right+200', 8) + ' b+250', ['BagApp_*']))
    a(Play('x-bag-toss', 'Bag: toss an item', MENU + ' icon:bag+700 a+250 down+60 down+60 a+250 up+60 a+250 a+250 a+250 b+250 b+250', ['BagApp_*']))
    a(Play('x-bag-key', 'Bag: key items, register', MENU + ' icon:bag+700 ' + rep('right+200', 7) + ' a+250 down+60 a+250 b+250 b+250', ['BagApp_*']))
    a(Play('x-gear', 'Pokegear: the cards', MENU + ' icon:gear+700 a+300 right+200 a+300 b+200 b+200 left+200 a+300 b+200 b+300', ['ov101_*']))
    a(Play('x-save', 'Save', MENU + ' icon:save+500 a+300 a+500 a+500 b+200', ['TouchSaveApp_*']))
    a(Play('x-opt', 'Options: every option', MENU + ' icon:opt+700 ' + rep('down+60 right+60 right+60', 7) + ' a+200 b+250 b+250', ['OptionsApp_*']))
    # ---- the PC (Cherrygrove Pokemon Center: the PC's tile 4,8, stand below it facing north), the clerk, the nurse
    # the game's own script opens the PC (BWPORT_HG_SCRIPT: script 2010, the Pokemon Center PC's script, started on the field after the
    # warp; route_gen4.c Pr4_ScriptHook): the boot message, the first menu item, the menu message, then the storage menu (a 2 x 3 grid:
    # deposit, withdraw, move Pokemon, move items, see ya): the first A at the menu picks the highlighted one
    PC = dict(warp=(69, 4, 9), face=0, script=2010)
    pcopen = 'wait+300 a+250 a+250 a+250 '
    dep, wd, mv, items = 'a+300 ', 'right+100 a+300 ', 'down+100 a+300 ', 'down+100 right+100 a+300 '
    a(Play('pc-open', 'PC box: open in deposit mode, leave', pcopen + dep + 'wait+700 b+300 a+300 wait+500', ['ov14_*'], **PC))
    a(Play('pc-boxes', 'PC box: scroll the boxes L / R', pcopen + wd + 'wait+700 r+200 r+200 l+200 l+200 l+200 b+300 a+300', ['ov14_*'], **PC))
    a(Play('pc-withdraw', 'PC box: withdraw a Pokemon to the party', pcopen + wd + 'wait+700 a+300 a+300 a+400 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-deposit', 'PC box: deposit a party Pokemon', pcopen + dep + 'wait+700 a+300 a+300 a+400 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-move', 'PC box: move a Pokemon between boxes', pcopen + mv + 'wait+700 a+300 right+100 a+300 a+400 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-release', 'PC box: release, cancel', pcopen + wd + 'wait+700 a+300 down+60 down+60 down+60 a+300 b+300 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-mark', 'PC box: mark a Pokemon', pcopen + wd + 'wait+700 a+300 down+60 down+60 a+300 down+60 a+300 b+300 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-summary', 'PC box: summary of a boxed Pokemon', pcopen + wd + 'wait+700 a+300 down+60 a+400 right+150 b+300 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-items', 'PC box: item give / take in the box (move items)', pcopen + items + 'wait+700 a+300 a+300 a+300 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-wallpaper', 'PC box: the box menu, wallpaper', pcopen + wd + 'wait+700 up+100 a+300 down+60 a+300 a+300 a+300 b+300 b+400', ['ov14_*'], **PC))
    a(Play('pc-name', 'PC box: the box menu, box name', pcopen + wd + 'wait+700 up+100 a+300 down+60 down+60 a+500 a+300 b+300 b+400', ['ov14_*'], **PC))
    # the egg (the Violet Mart's aide gave it; hg-egg stands at 6,9 in the Mart): the cheat `egg` makes it hatch with the next step
    # (cycles 0, the day care's step counter at the end of a cycle); the walk, the script's text, the hatch app (overlay 95), the
    # nickname question (B = no) and back to the field
    a(Play('egg-hatch', 'Egg: one step from hatching, the hatch app, the nickname screen', 'wait+200 left*20+60 right*20+60 ' + rep('a+200', 14) + ' b+300 ' + rep('a+200', 4) + ' wait+300',
           ['ov95_*'], save='hg-egg', cheat='egg'))
    a(Play('pc-exit', 'PC box: exit, the menu again, see ya', pcopen + wd + 'wait+700 b+300 a+300 wait+400 down+100 right+100 down+100 a+300 wait+300', ['ov14_*'], **PC))
    a(Play('mart-buy', 'Mart: buy', 'wait+300 a+250 a+250 a+300 a+300 up+60 a+250 a+300 a+300 b+300 b+300', [], warp=(68, 4, 6), face=2))
    a(Play('mart-sell', 'Mart: sell', 'wait+300 a+250 a+250 down+60 a+300 a+300 up+60 a+250 a+300 a+300 b+300 b+300', [], warp=(68, 4, 6), face=2))
    a(Play('center-heal', 'Pokemon Center: heal', 'wait+300 ' + rep('a+150', 10) + ' wait+400', [], warp=(69, 8, 13), face=0))
    # ---- field moves and key items (the object's script asks the question: A at the tree, the rock, the boulder)
    a(Play('field-cut', 'Cut: the Route 30 tree', 'wait+200 ' + rep('a+250', 8), [], warp=(34, 559, 296), face=0))
    a(Play('field-rocksmash', 'Rock Smash: the Route 32 rock', 'wait+200 ' + rep('a+250', 8), [], warp=(36, 471, 440), face=0))
    a(Play('field-strength', 'Strength: the Dark Cave boulder', 'wait+200 ' + rep('a+250', 6) + ' up*16+40 up*16+40 up*16+40', [], warp=(176, 28, 21), face=0))
    a(Play('field-flash', 'Flash: the Dark Cave from the party menu', MENU + ' icon:mon+700 right+80 a+250 left+80 down+60 down+60 down+60 a+500 a+400 a+400', [], warp=(176, 28, 21), face=0))
    a(Play('field-bicycle', 'Bicycle: use the key item, ride, walk', MENU + ' icon:bag+700 ' + rep('right+200', 7) + ' a+250 a+300 a+300 wait+200 up*16+30 up*16+30 right*16+30 right*16+30', [], ))
    a(Play('field-registered', 'Registered key item: Y', MENU + ' icon:bag+700 ' + rep('right+200', 7) + ' a+250 down+60 a+250 b+250 b+250 b+250 y+250 a+300 a+300', []))
    return P


PLAYS = None


def pick(spec):
    allp = plays()
    if spec == 'all':
        return allp
    out = []
    for part in spec.split(','):
        out += [p for p in allp if p.id == part or p.id.startswith(part)]
    return list(dict.fromkeys(out))


HEAD = ['id', 'name', 'verdict', 'frame', 'top', 'bt_id', 'reached', 'missing', 'wall_s']


def run_play(a, p, slot, syms):
    d = os.path.join(a.out, 'plays', p.id)
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    run_dir = os.path.join(a.out, 'run', f'w{slot}')
    out_dir = os.path.join(run_dir, 'out')
    env = dict(os.environ, BWPORT_BUILD=a.build, BWPORT_HG_COV=os.path.join(run_dir, 'cov.bin'), PYTHONDONTWRITEBYTECODE='1',
               BWPORT_HG_WARP_LOG=os.path.join(d, 'warp.txt'), BWPORT_KEYS=a.reload + ' ' + p.keys)
    w = p.warp or (-1, 0, 0)
    env['BWPORT_HG_WARP'] = f'{w[0]}:{w[1]}:{w[2]}:{p.end}'
    if p.cheat:
        env['BWPORT_HG_CHEAT'] = p.cheat
    if p.face is not None:
        env['BWPORT_HG_WARP_DIR'] = str(p.face)
    if p.script is not None:
        env['BWPORT_HG_SCRIPT'] = f'{p.script}:{START}'
    env.update(p.env)
    cmd = ['python3', os.path.join(REPO, 'tools/port_run.py'), '--game', p.game, '--no-build', '--save', p.save,
           '--dir', run_dir, '--out', out_dir, '-t', sweep_wall.NO_WALL_CAP, '--frames', str(p.end + sweep_wall.END_SLACK),
           '--shots', str(a.shots), '--hang', '30', '--hang-stop',
           '--skip', str(a.skip)]
    t0 = time.time()
    r = subprocess.run(['nice', '-n', '10'] + cmd, env=env, capture_output=True, text=True, cwd=REPO)
    wall = time.time() - t0
    open(os.path.join(d, 'run.txt'), 'w').write('\n'.join((r.stdout + r.stderr).splitlines()[-20:]) + '\n')
    line = (r.stdout.strip().splitlines() or ['?'])[-1]
    try:
        line = sweep_wall.verdict_line(r, out_dir)
    except OSError:
        pass
    try:
        log = open(os.path.join(out_dir, 'log.txt'), errors='replace').read()
    except OSError:
        log = r.stdout + r.stderr
    warp = open(os.path.join(d, 'warp.txt')).read() if os.path.exists(os.path.join(d, 'warp.txt')) else ''
    m = re.search(r'frame (\d+)', line)
    frame = m.group(1) if m else ''
    top = text = bid = ''
    if line.startswith('heapcheck:'):
        verdict = 'heap'
        mo = re.search(r'heap (?:overrun|misuse)( after \S+| \(ow\wer [^)]*\))', line)
        top = 'heap overrun' + (mo.group(1) if mo else '')
        text = line
        bid = hashlib.sha1(top.encode()).hexdigest()[:8]
    elif line.startswith('crash'):
        verdict = 'crash'
        top, text, bid = H.crash_info(log)
    elif line.startswith('hang'):
        verdict = 'hang'
        top, text, bid = sweep_wall.frozen_top(log)
    elif line.startswith('harness'):   # port_run itself died (sweep_wall.verdict_line): not a game finding
        verdict, top, text = 'harness', line[:100], line
    elif line.startswith('stub'):
        verdict = 'stub'
        top = line.split()[1]
        text = line
    elif re.search(r'^end \d+', warp, re.M):
        verdict = 'ok'
        frame = re.search(r'^end (\d+)', warp, re.M).group(1)
    elif not warp:
        verdict = 'no-field'
    else:
        verdict = 'timeout'
    funcs = syms.funcs(os.path.join(run_dir, 'cov.bin')) if syms else None
    if funcs is not None:
        with gzip.open(os.path.join(d, 'cov.txt.gz'), 'wt') as f:
            f.writelines(n + '\n' for n in sorted(funcs))
    miss = p.reached(funcs) if funcs is not None and p.need else []
    reached = '?' if funcs is None or not p.need else ('no' if miss else 'yes')
    if verdict == 'ok' and reached == 'no':
        verdict = 'unreached'
    if verdict in ('crash', 'hang', 'stub', 'heap'):
        with gzip.open(os.path.join(d, 'log.txt.gz'), 'wt') as f:
            f.write(log)
        open(os.path.join(d, 'bt.txt'), 'w').write(line + '\n' + text + '\n')
    for png in sorted(os.listdir(os.path.join(out_dir, 'shots'))) if os.path.isdir(os.path.join(out_dir, 'shots')) else []:
        os.makedirs(os.path.join(d, 'shots'), exist_ok=True)
        shutil.copy(os.path.join(out_dir, 'shots', png), os.path.join(d, 'shots', png))
    if os.path.exists(os.path.join(out_dir, 'final.png')):
        shutil.copy(os.path.join(out_dir, 'final.png'), os.path.join(d, 'final.png'))
    if os.path.exists(os.path.join(run_dir, 'cov.bin')):
        os.remove(os.path.join(run_dir, 'cov.bin'))
    shutil.rmtree(out_dir, ignore_errors=True)
    return [p.id, p.name, verdict, frame, top, bid, reached, ','.join(miss), f'{wall:.0f}'], text


def rescore(a):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)]
    byid = {p.id: p for p in plays()}
    for r in rows[1:]:
        cov = os.path.join(a.out, 'plays', r[0], 'cov.txt.gz')
        if r[0] not in byid or not os.path.exists(cov):
            continue
        funcs = set(gzip.open(cov, 'rt').read().split())
        miss = byid[r[0]].reached(funcs) if byid[r[0]].need else []
        r[6] = '?' if not byid[r[0]].need else ('no' if miss else 'yes')
        r[7] = ','.join(miss)
        if r[2] in ('ok', 'unreached'):
            r[2] = 'unreached' if miss else 'ok'
    open(path, 'w').write('\n'.join('\t'.join(r) for r in rows) + '\n')


def summarize(a):
    path = os.path.join(a.out, 'sweep.tsv')
    rows = [l.rstrip('\n').split('\t') for l in open(path)][1:] if os.path.exists(path) else []
    verdicts = collections.Counter(r[2] for r in rows)
    groups = collections.OrderedDict()
    for r in rows:
        if r[2] in ('crash', 'hang', 'stub', 'heap'):
            g = groups.setdefault((r[2], r[4]), {'plays': [], 'bt': ''})
            g['plays'].append(r[0])
            if not g['bt']:
                try:
                    g['bt'] = open(os.path.join(a.out, 'plays', r[0], 'bt.txt')).read().split('\n')[1]
                except (OSError, IndexError):
                    pass
    order = sorted(groups.items(), key=lambda kv: -len(kv[1]['plays']))
    with open(os.path.join(a.out, 'crash_groups.tsv'), 'w') as f:
        f.write('top_function\tfirst_play\tcount\tbacktrace\tsave\tkind\tall_plays\n')
        for (kind, top), g in order:
            f.write(f'{top}\t{g["plays"][0]}\t{len(g["plays"])}\t{g["bt"]}\t{a.save or ""}\t{kind}\t{",".join(g["plays"])}\n')
    reached = collections.Counter(r[6] for r in rows)
    lines = [f'hg_app_sweep: {len(rows)} plays, ' + ', '.join(f'{k} {v}' for k, v in verdicts.most_common())
             + '; reached ' + ', '.join(f'{k} {v}' for k, v in reached.most_common()), '',
             'play  verdict  reached  frame  top   (missing needs)']
    for r in rows:
        lines.append(f'{r[0]:18} {r[2]:10} {r[6]:3} {r[3]:>6}  {r[4]}' + (f'   missing: {r[7]}' if r[7] else ''))
    lines += ['', 'count  kind   top function   (first play)']
    for (kind, top), g in order:
        lines.append(f'{len(g["plays"]):5}  {kind:5}  {top}   ({g["plays"][0]}; {",".join(g["plays"][:12])})')
    open(os.path.join(a.out, 'summary.txt'), 'w').write('\n'.join(lines) + '\n')
    print('\n'.join(lines[:60]))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--plays', default='all', help='all | ids or id prefixes: dex,x-,pc-,field-')
    ap.add_argument('--out')
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--save', default=None, help="override every play's save (default: each play's, hg-r30c)")
    ap.add_argument('--game', choices=('heartgold', 'soulsilver'), default='heartgold',
                    help="soulsilver: every play runs SoulSilver from the save's ss- twin (hg-r30c -> ss-r30c)")
    ap.add_argument('--build', default='build/port/linux-hgss-cov')
    ap.add_argument('--skip', type=int, default=4, help="port_run's --skip (frame skip; 1 for screen checks)")
    ap.add_argument('--shots', type=float, default=0, help='a screenshot every N wall seconds (plays/ID/shots/SSs_fFRAME.png)')
    sweep_wall.ignore_t(ap)
    ap.add_argument('--reload', default=H.RELOAD)
    ap.add_argument('--heapcheck', nargs='?', const='1', default=None, metavar='LEVEL', help='BWPORT_HEAPCHECK=LEVEL (default 1)')
    ap.add_argument('--keys', help="one ad-hoc play (--plays ignored): the key language, e.g. 'x+80 icon:dex+500 a+200'")
    ap.add_argument('--warp', help='the ad-hoc play warps to MAP:X:Y (with --keys)')
    ap.add_argument('--face', type=int, help='the ad-hoc play faces 0 north 1 south 2 west 3 east after the warp')
    ap.add_argument('--script', type=int, help='the ad-hoc play starts this script id of the game after the warp (2010 = the PC)')
    ap.add_argument('--cheat', default=CHEAT_ALL, help='BWPORT_HG_CHEAT words of the ad-hoc play (default %(default)s)')
    ap.add_argument('--redo', action='store_true')
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--summary', action='store_true')
    ap.add_argument('--rescore', action='store_true', help="recompute reached / unreached of the rows in DIR/sweep.tsv from the saved plays/ID/cov.txt.gz with the current `need` lists (no run)")
    a = ap.parse_args()
    if a.list:
        for p in plays():
            print(f'{p.id:18} save {p.save} warp {p.warp} end {p.end} need {p.need}  {p.name}')
        return
    if not a.out:
        sys.exit('hg_app_sweep: --out DIR')
    a.out = os.path.abspath(a.out)
    if a.heapcheck is not None:
        os.environ['BWPORT_HEAPCHECK'] = a.heapcheck
    os.makedirs(os.path.join(a.out, 'plays'), exist_ok=True)
    if a.rescore:
        rescore(a)
    if a.summary or a.rescore:
        summarize(a)
        return
    path = os.path.join(a.out, 'sweep.tsv')
    done = {}
    if os.path.exists(path):
        for l in list(open(path))[1:]:
            done[l.split('\t')[0]] = l
    if a.keys:
        w = tuple(int(x) for x in a.warp.split(':')) if a.warp else None
        todo = [Play('adhoc', 'ad hoc', a.keys, warp=w, face=a.face, cheat=a.cheat, script=a.script)]
        a.redo = True
    else:
        todo = [p for p in pick(a.plays) if a.redo or p.id not in done]
    if a.game == 'soulsilver':
        for p in todo:
            p.game, p.save = 'soulsilver', re.sub(r'^hg-', 'ss-', p.save)
    if a.save:
        for p in todo:
            p.save = a.save
    done = {k: v for k, v in done.items() if k not in {p.id for p in todo}}
    with open(path, 'w') as f:
        f.write('\t'.join(HEAD) + '\n')
        f.writelines(done[k] for k in sorted(done))
    exe = os.path.join(REPO, a.build, 'bwport')
    syms = H.Symbols(exe, a.out)
    if syms.base is None:
        print(f'hg_app_sweep: {a.build} has no coverage (meson -Dhgss_cov=true): reached stays ?')
        syms = None
    slots = queue.Queue()
    for i in range(a.j):
        slots.put(i)
    lock = threading.Lock()
    t0 = time.time()
    n = [0]

    def job(p):
        slot = slots.get()
        try:
            row, text = run_play(a, p, slot, syms)
        finally:
            slots.put(slot)
        with lock:
            with open(path, 'a') as f:
                f.write('\t'.join(map(str, row)) + '\n')
            n[0] += 1
            print(f'[{n[0]}/{len(todo)} {time.time() - t0:.0f}s] {p.id}: {row[2]} {row[3]} {row[4]} reached {row[6]}'
                  + (f' missing {row[7]}' if row[7] else ''), flush=True)

    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        list(ex.map(job, todo))
    summarize(a)


if __name__ == '__main__':
    main()
