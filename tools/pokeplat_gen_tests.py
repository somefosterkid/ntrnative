#!/usr/bin/env python3
"""Tests for tools/pokeplat_gen.py's working copy (a scratch git repo stands in for pret's submodule; no network):

    python3 tools/pokeplat_gen_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pokeplat_gen as G  # noqa: E402

PATCH = """a patch header: what it fixes and why

diff --git a/src/a.c b/src/a.c
--- a/src/a.c
+++ b/src/a.c
@@ -1,2 +1,2 @@
 int x;
-enum T y;
+T y;
"""


def git(d, *a):
    return subprocess.check_output(['git', '-C', d, '-c', 'user.name=t', '-c', 'user.email=t@t', *a], text=True).strip()


def repo(tmp):
    sub = os.path.join(tmp, 'sub')
    os.makedirs(os.path.join(sub, 'src'))
    subprocess.check_call(['git', 'init', '-q', sub])
    open(os.path.join(sub, 'src/a.c'), 'w').write('int x;\nenum T y;\n')
    git(sub, 'add', '.')
    git(sub, 'commit', '-q', '-m', 'one')
    p = os.path.join(tmp, 'x.patch')
    open(p, 'w').write(PATCH)
    return sub, git(sub, 'rev-parse', 'HEAD'), p


def test_checkout_patches_a_clone_not_the_submodule():
    with tempfile.TemporaryDirectory() as tmp:
        sub, c, p = repo(tmp)
        work = os.path.join(tmp, 'work')
        G.checkout(work, sub, c, [p])
        assert open(os.path.join(work, 'src/a.c')).read() == 'int x;\nT y;\n'
        assert open(os.path.join(sub, 'src/a.c')).read() == 'int x;\nenum T y;\n'
        assert git(sub, 'status', '--porcelain') == ''
        G.checkout(work, sub, c, [p])   # again: reset to the commit first, so the patch applies again
        assert open(os.path.join(work, 'src/a.c')).read() == 'int x;\nT y;\n'
        G.checkout(work, sub, c, [])    # a dropped patch is undone
        assert open(os.path.join(work, 'src/a.c')).read() == 'int x;\nenum T y;\n'


def test_stamp_follows_commit_and_patches():
    with tempfile.TemporaryDirectory() as tmp:
        p = os.path.join(tmp, 'x.patch')
        open(p, 'w').write(PATCH)
        a = G.stamp('c1', [p])
        assert a == G.stamp('c1', [p]) and a != G.stamp('c2', [p]) and a != G.stamp('c1', [])
        open(p, 'w').write(PATCH + ' ')
        assert a != G.stamp('c1', [p])


def test_resolve_prefers_the_c_tree():
    with tempfile.TemporaryDirectory() as tmp:
        co, b = os.path.join(tmp, 'co'), os.path.join(tmp, 'b')
        for f in ('co/include/x.h', 'b/x.h', 'b/generated/y.h', 'b/res/text/bank/z.h'):
            os.makedirs(os.path.dirname(os.path.join(tmp, f)), exist_ok=True)
            open(os.path.join(tmp, f), 'w').write('')
        assert G.resolve('x.h', co, b, {}) is None
        assert G.resolve('y.h', co, b, {}) == 'generated/y.h'
        assert G.resolve('bank/z.h', co, b, {}) == 'res/text/bank/z.h'
        assert G.resolve('w.naix', co, b, {'w.naix': 'res/q/w.naix'}) == 'res/q/w.naix'
        assert G.resolve('d/w.naix', co, b, {'w.naix': 'res/q/w.naix'}) is None


def main():
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except AssertionError as e:
                bad += 1
                print('FAIL', name, e)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
