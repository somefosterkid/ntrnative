#!/usr/bin/env python3
"""Tests for tools/pokeplat_cc.py's file list (a scratch tree stands in for build/pokeplatinum; no clang, no network):

    python3 tools/pokeplat_cc_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pokeplat_cc as C  # noqa: E402

LSF = """Overlay main
{
\tObject src/a.c
\tObject src/sub/b.c
}
"""


def tree(tmp):
    for f in ['src/a.c', 'src/sub/b.c', 'src/stale.c', 'src/a.h', 'lib/spl/src/s.c']:
        p = os.path.join(tmp, f)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, 'w').write('int x;\n')
    os.makedirs(os.path.join(tmp, 'platinum.us'))
    open(os.path.join(tmp, 'platinum.us/main.lsf.in'), 'w').write(LSF)


def test_sources_skips_files_pret_does_not_build():
    with tempfile.TemporaryDirectory() as tmp:
        tree(tmp)
        C.PLAT = tmp
        got = [os.path.relpath(f, tmp) for f in C.sources()]
        # src/stale.c is not in main.lsf.in (like pret's src/unk_02095CD4.c): not compiled; lib/ is not in the lsf
        assert got == ['lib/spl/src/s.c', 'src/a.c', 'src/sub/b.c'], got


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print('FAIL', name, repr(e))
    sys.exit(1 if bad else 0)
