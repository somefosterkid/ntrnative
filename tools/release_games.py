#!/usr/bin/env python3
"""tools/release_games.py PACKAGE... : every game is in the package. The launcher's game table (port/src/launcher_core.c) keeps a
row's ROM SHA-1 only when that game's module is in the build (`#ifdef BWPORT_PLAT` ...; else NULL and the ROM is refused as
unknown), so each game's SHA-1 as text is in the game binary exactly when the game plays. A binary (any package member, any file of
a folder) that holds at least one of them must hold all of them: v0.2.1 / v0.2.2 shipped without Platinum (a build dir kept
-Dplatinum=false from before the default flip). A package with no such binary is refused too. Exit 0 all there, 1 a game missing,
2 can't check. The hashes are ROM identifiers (public), not ROM bytes."""
import os
import re
import sys
import zipfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def game_hashes(src=None):
    """{name: sha1} of the table's rows (the first SHA-1 of each row, with the shortName before it)"""
    text = open(src or os.path.join(REPO, 'port/src/launcher_core.c')).read()
    return {m.group(1): m.group(2) for m in re.finditer(r'\{"[A-Z0-9]{4}", "[^"]*", "([^"]*)", "[^"]*", \{"([0-9a-f]{40})", NULL\}', text)}


def members(path):
    """(name, bytes) of each file of a package: an apk/ipa/zip's members, a folder's files, a plain file"""
    if os.path.isdir(path):
        for d, _, fs in os.walk(path):
            for f in fs:
                p = os.path.join(d, f)
                yield os.path.relpath(p, path), open(p, 'rb').read()
    elif zipfile.is_zipfile(path):
        with zipfile.ZipFile(path) as z:
            for i in z.infolist():
                if not i.is_dir() and i.file_size > 100000:
                    yield i.filename, z.read(i)
    else:
        yield os.path.basename(path), open(path, 'rb').read()


def check(path, hashes):
    """list of problems with one package"""
    bad, seen = [], 0
    for name, data in members(path):
        have = {g for g, h in hashes.items() if h.encode() in data}
        if have:
            seen += 1
            if len(have) != len(hashes):
                bad.append(f'{path}: {name} lacks {", ".join(sorted(set(hashes) - have))}')
    if not seen:
        bad.append(f'{path}: no game binary in it (none holds a game of the launcher table)')
    return bad


def main(argv):
    if len(argv) < 2:
        print('usage: tools/release_games.py PACKAGE...')
        return 2
    hashes = game_hashes()
    if len(hashes) < 7:
        print(f'release_games: the launcher table gave {len(hashes)} games: {", ".join(hashes)}')
        return 2
    bad = [b for p in argv[1:] for b in check(p, hashes)]
    for b in bad:
        print('release_games: MISSING: ' + b)
    if not bad:
        print(f'release_games: all {len(hashes)} games in {" ".join(argv[1:])}')
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
