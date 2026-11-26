#!/usr/bin/env python3
"""Shared capture cache: makes or reuses BWPORT_VCAPTURE captures so no agent records what exists.

    python3 tools/caps.py get PLAY SPEC      print the directory of the captures (recording them first when missing)
    python3 tools/caps.py seed               get the standard sets (town-walk 7500-9200/8, route1-battle 300-3000/4)
    python3 tools/caps.py list               what the cache holds

PLAY is a tools/plays NAME (its header's `# game:` picks the game), SPEC the BWPORT_VCAPTURE frame spec (7500-9200/8).
The cache is $BW_CAPS (default ~/bw-cache/caps), one directory per (game, play, spec, KEY). KEY hashes the files that
change what a capture records: the play script and its runner, libntr's capture writer and the patches that hook it
(KEYFILES), plus the game's source tree with --src (a game-code change can move a frame: use it when yours does), and whether
BWPORT_VCAPTURE_TAGS is on (tagged captures are a set of their own).
Captures hold ROM data: the cache lives outside the repo, never commit it. Concurrent calls wait on one lock per entry.
"""
import fcntl, hashlib, os, re, shutil, subprocess, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CACHE = os.environ.get('BW_CAPS') or os.path.expanduser('~/bw-cache/caps')
KEYFILES = ['tools/play_script.py', 'port/subprojects/libntr/libraries/sim/src/sim_vcapture.cpp',
            'port/subprojects/libntr/libraries/sim/src/sim_g3_CommandProcessor.cpp',
            'port/subprojects/pokeplatinum-tags.patch', 'port/subprojects/pokeheartgold-tags.patch']
SRC_TREES = ['src', 'include', 'port/src', 'port/game']


def play_file(play):
    hits = [os.path.join(r, f) for r, _, fs in os.walk(os.path.join(REPO, 'tools/plays')) for f in fs if f == play + '.txt']
    if len(hits) != 1:
        sys.exit(f'caps: play {play!r}: {len(hits)} scripts in tools/plays')
    game = re.search(r'^# game: (\w+)', open(hits[0]).read(), re.M)
    return hits[0], (game.group(1) if game else os.path.basename(os.path.dirname(hits[0])))


def key(play_path, src):
    h = hashlib.sha1()
    paths = [os.path.relpath(play_path, REPO)] + KEYFILES
    for p in paths:
        h.update(p.encode())
        if os.path.exists(os.path.join(REPO, p)):
            h.update(open(os.path.join(REPO, p), 'rb').read())
    if os.environ.get('BWPORT_VCAPTURE_TAGS', '0') not in ('', '0'):
        h.update(b'tags')  # Smooth motion's tags in the stream (libntr's simulator/sim_g3tag.h): a set of its own
    if src:
        # tracked files' content, working-tree state included (git ls-files + hash-object)
        out = subprocess.run(['git', 'ls-files', '-z', '--'] + SRC_TREES, cwd=REPO, capture_output=True).stdout
        names = [n for n in out.split(b'\0') if n]
        r = subprocess.run(['git', 'hash-object', '--stdin-paths'], cwd=REPO, input=b'\n'.join(names), capture_output=True)
        h.update(r.stdout)
    return h.hexdigest()[:12]


def get(play, spec, src=False, quiet=False):
    pp, game = play_file(play)
    name = f"{game}-{play}-{spec.replace('/', 'x')}-{key(pp, src)}{'s' if src else ''}"
    d = os.path.join(CACHE, name)
    os.makedirs(CACHE, exist_ok=True)
    with open(d + '.lock', 'w') as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        if not os.path.exists(os.path.join(d, 'DONE')):
            tmp = d + '.tmp'
            shutil.rmtree(tmp, ignore_errors=True)
            os.makedirs(tmp)
            env = dict(os.environ, BWPORT_VCAPTURE=f'{tmp},{spec}')
            print(f'caps: recording {name} ...', file=sys.stderr)
            r = subprocess.run([sys.executable, os.path.join(REPO, 'tools/port_run.py'), '--game', game, '--play', play,
                                '-t', '600'], cwd=REPO, env=env, capture_output=True, text=True)
            n = len([f for f in os.listdir(tmp) if f.endswith('.vcs')])
            if n == 0:
                sys.exit(f'caps: no captures recorded: {r.stdout[-400:]}')
            open(os.path.join(tmp, 'DONE'), 'w').write(f'{n}\n')
            shutil.rmtree(d, ignore_errors=True)
            os.rename(tmp, d)
    return d


def main():
    a = sys.argv[1:]
    src = '--src' in a
    a = [x for x in a if x != '--src']
    if a[:1] == ['get'] and len(a) == 3:
        print(get(a[1], a[2], src))
    elif a[:1] == ['seed']:
        for p, s in (('town-walk', '7500-9200/8'), ('route1-battle', '300-3000/4')):
            print(get(p, s, src))
    elif a[:1] == ['list']:
        for n in sorted(os.listdir(CACHE)) if os.path.isdir(CACHE) else []:
            if os.path.exists(os.path.join(CACHE, n, 'DONE')):
                print(n, len([f for f in os.listdir(os.path.join(CACHE, n)) if f.endswith('.vcs')]))
    else:
        sys.exit(__doc__)


if __name__ == '__main__':
    main()
