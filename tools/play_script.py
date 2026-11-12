#!/usr/bin/env python3
"""Play scripts: tools/plays/<game>/<name>.txt, our own key sequences (no ROM bytes), run by `port_run.py --play NAME`.

    # game: black                 black | white | b2 | w2 (the game's folder in a launcher root: Pokemon Black, ...)
    # from: room                  a saves-library name (the saves README), or boot (a new game: nothing is loaded)
    # reaches: the stairs         what it gets to, one line
    # store: downstairs           the library name its end state can be stored as (--store NAME)
    # end: f600                   quit at this frame (default: 30 frames after the last key)
    keys start@f600 a@f1700       raw BWPORT_KEYS items (KEY[*HOLD]@fFRAME), frames counted from boot
    walk 400 48 up2 right5 a      walk.py's grammar: START frame, STEP frames, then moves: DIR[N][:fH], KEY, KEY*N[/GAP], wN

A script from a save starts with BWPORT_SKIPMENU=1 (the field is up at about f220), so its frames are written from there;
`from: boot` runs from the title. The header's `# name: value` lines are metadata; everything else is a body line.
"""
import re
import sys

GAMES = {'black': 'Pokemon Black', 'white': 'Pokemon White', 'b2': 'Pokemon Black 2', 'w2': 'Pokemon White 2'}


def walk(start, step, moves, hold='f6'):
    """walk.py's grammar -> (BWPORT_KEYS items, last frame)"""
    f, facing, out = start, None, []
    for m in moves:
        g = re.fullmatch(r'(up|down|left|right)(\d*)(:f\d+)?', m)
        if g:
            d, n = g.group(1), int(g.group(2) or 1)
            h = g.group(3)[1:] if g.group(3) else hold
            if d != facing:
                out.append(f'{d}*{hold}@f{f}')
                f += step
                facing = d
            for _ in range(n):
                out.append(f'{d}*{h}@f{f}')
                f += step
        elif re.fullmatch(r'[a-z]+\*\d+(/\d+)?', m):
            k, rest = m.split('*')
            n, gap = (rest.split('/') + ['90'])[:2]
            for _ in range(int(n)):
                out.append(f'{k}@f{f}')
                f += int(gap)
        elif re.fullmatch(r'w\d+', m):
            f += int(m[1:])
        elif re.fullmatch(r'[a-z]+', m):
            out.append(f'{m}@f{f}')
            f += step * 3
        else:
            raise ValueError(f'bad walk move {m!r}')
    return out, f


def parse(text, name='?'):
    """-> dict(game, from, reaches, store, end, keys: [items], last: the last frame-counted key's frame)"""
    meta = {'game': 'black', 'from': 'boot', 'reaches': '', 'store': '', 'end': ''}
    keys = []
    for n, line in enumerate(text.splitlines(), 1):
        line = line.strip()
        m = re.fullmatch(r'#\s*(\w+):\s*(.*)', line)
        if m and m.group(1) in meta:
            meta[m.group(1)] = m.group(2).strip()
        elif not line or line.startswith('#'):
            continue
        elif line.startswith('keys '):
            keys += line[5:].split()
        elif line.startswith('walk '):
            p = line[5:].split()
            if len(p) < 3 or not (p[0].isdigit() and p[1].isdigit()):
                raise ValueError(f'{name}:{n}: walk START STEP MOVES...')
            items, _ = walk(int(p[0]), int(p[1]), p[2:])
            keys += items
        else:
            raise ValueError(f'{name}:{n}: unknown line {line!r}')
    if meta['game'] not in GAMES:
        raise ValueError(f'{name}: game {meta["game"]!r}: one of {" ".join(GAMES)}')
    frames = [int(m.group(1)) for k in keys for m in [re.search(r'@f(\d+)$', k)] if m]
    if len(frames) != len(keys):
        raise ValueError(f'{name}: keys must be frame-counted (@fN): a seconds key makes the run real time')
    meta['keys'], meta['last'] = keys, max(frames, default=0)
    end = re.fullmatch(r'f(\d+)', meta['end']) if meta['end'] else None
    if meta['end'] and not end:
        raise ValueError(f'{name}: end: fN')
    meta['end_frame'] = int(end.group(1)) if end else meta['last'] + 30
    return meta


if __name__ == '__main__':
    print(' '.join(walk(int(sys.argv[1]), int(sys.argv[2]), sys.argv[3:])[0]))
