#!/usr/bin/env python3
"""tools/release_games_tests.py: release_games.py on fake packages (no ROM, no build)."""
import os
import sys
import tempfile
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import release_games as rg

H = rg.game_hashes()


def t_the_table_gives_every_game():
    assert sorted(H) == sorted(['Black', 'White', 'Black 2', 'White 2', 'Platinum', 'HeartGold', 'SoulSilver']), H


def pkg(members):
    p = os.path.join(tempfile.mkdtemp(), 'x.zip')
    with zipfile.ZipFile(p, 'w') as z:
        for n, d in members.items():
            z.writestr(n, d)
    return p


def blob(skip=()):
    return b'\0' * 200000 + b'\0'.join(h.encode() for g, h in H.items() if g not in skip) + b'\0' * 10


def t_all_games_pass():
    assert rg.check(pkg({'lib/a/libmain.so': blob(), 'assets/x': b'y'}), H) == []


def t_a_missing_game_fails_naming_it():
    bad = rg.check(pkg({'lib/a/libmain.so': blob(('Platinum',))}), H)
    assert len(bad) == 1 and 'lacks Platinum' in bad[0], bad


def t_one_abi_without_a_game_fails():
    bad = rg.check(pkg({'lib/arm64/libmain.so': blob(), 'lib/x86_64/libmain.so': blob(('White',))}), H)
    assert len(bad) == 1 and 'x86_64' in bad[0] and 'White' in bad[0], bad


def t_no_binary_fails():
    assert 'no game binary' in rg.check(pkg({'readme': b'x' * 200000}), H)[0]


if __name__ == '__main__':
    n = 0
    for k, v in sorted(globals().items()):
        if k.startswith('t_'):
            v()
            print('ok', k)
            n += 1
    print(f'{n} tests passed')
