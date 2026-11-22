#!/usr/bin/env python3
"""Tests for tools/gpu_fd.py (the parsing, the sums, the compare) and tools/caps.py (the cache key).

    python3 tools/gpu_fd_tests.py       # prints "ok" per test, exits 1 on a failure
"""
import io, os, sys, contextlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gpu_fd  # noqa: E402
import caps  # noqa: E402

bad = 0


def check(name, cond):
    global bad
    print(('ok ' if cond else 'FAIL ') + name)
    bad += not cond


def res(d, g=0, e=0, s=0, k=0):
    return {'differ': d, 'gaps': g, 'extras': e, 'specks': s, 'dark': k}


a = {'res': {'x/1': res(5.0, 1, 2, 3, 1), 'x/2': res(7.0)}}
b = {'res': {'x/1': res(5.5, 0, 2, 4, 0), 'x/2': res(7.0)}}
t = gpu_fd.sums(a['res'])
check('sums', t['gaps'] == 1 and t['specks'] == 3 and t['dark'] == 1 and abs(t['differ_mean'] - 6.0) < 1e-9 and t['caps_with'] == 1)
out = io.StringIO()
with contextlib.redirect_stdout(out):
    gpu_fd.compare(a, b, 5)
o = out.getvalue()
check('compare lists the changed capture only', 'changed: 1 of 2' in o and 'x/1: gaps -1 extras +0 specks +1 dark -1' in o and 'x/2:' not in o)
check('caps key is stable and 12 hex', caps.key(os.path.join(caps.REPO, 'tools/plays/platinum/town-walk.txt'), False)
      == caps.key(os.path.join(caps.REPO, 'tools/plays/platinum/town-walk.txt'), False))
sys.exit(1 if bad else 0)
