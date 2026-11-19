#!/usr/bin/env python3
"""Tests for tools/hg_vendor.py (what the vendored tree keeps, the comment scrub) and tools/pokehg_gen.py's vendored
working copy (scratch trees; no ROM, no network):

    python3 tools/hg_vendor_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hg_vendor as V  # noqa: E402
import pokehg_gen as G  # noqa: E402


def write(tmp, files):
    for f, t in files.items():
        p = os.path.join(tmp, f)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, 'w').write(t)


def test_keep_takes_the_c_and_the_linked_data_asm_only():
    linked = {'asm/data.s', 'asm/code.s'}
    assert V.keep('src/a.c', b'', linked) and V.keep('include/a.h', b'', linked) and V.keep('main.lsf.in', b'', linked)
    assert V.keep('lib/spl/src/spl_anim.c', b'', linked) and V.keep('lib/include/x.h', b'', linked)
    assert V.keep('files/fielddata/script/scr_seq/event_T01.h', b'', linked)
    assert V.keep('port/nonmatching/ov01_0220.c', b'', linked) and V.keep('asm/include/x.inc', b'', linked)
    assert V.keep('asm/data.s', b'.byte 1\n', linked)
    assert not V.keep('asm/code.s', b'\tthumb_func_start f\n', linked)   # its C is in port/nonmatching
    assert not V.keep('asm/stale.s', b'.byte 1\n', linked)               # not linked
    for p in ('files/msgdata/msg/msg_0001.json', 'files/poketool/a.png', 'tools/x.c', 'lib/NitroSDK/src/a.c',
              'graphics/a.bin', 'src/a.c.s2', 'port/SIZES.tsv'):
        assert not V.keep(p, b'', linked), p


def test_uncomment_empties_comments_and_keeps_code_and_strings():
    got = V.uncomment(['    Text_A, // a message\n', 'f("a // b"); /* x */ g();\n', '/* one\n', ' * two\n',
                       ' end */ h();\n', '// alone\n'])
    assert got == ['    Text_A,\n', 'f("a // b"); /**/ g();\n', '/*\n', '\n', '*/ h();\n', '\n'], got


def test_gen_comments_are_dropped():
    s = b'#ifndef X\n// Some text\n#define msg_0001_00000 0\n    // more\n#define msg_0001_00001 1\n'
    assert V.GEN_COMMENT.sub(b'', s) == b'#ifndef X\n#define msg_0001_00000 0\n#define msg_0001_00001 1\n'


def test_gen_does_not_name_the_worktree_in_the_include_guard():
    s = b'/* made for /r/bw-port/.claude/worktrees/agent-x/build/a.narc */\n#ifndef _r_bw_port__claude_worktrees_agent_x_build_a_narc\n'
    assert V.strip_root(s, {'/r/bw-port/.claude/worktrees/agent-x'}) == b'/* made for build/a.narc */\n#ifndef build_a_narc\n'


def test_vendored_checkout_copies_patches_and_follows_the_tree():
    with tempfile.TemporaryDirectory() as tmp:
        root, sub, work = os.path.join(tmp, 'repo'), 'third_party/pokeheartgold', os.path.join(tmp, 'work')
        write(os.path.join(root, sub), {'PIN': 'abc\n', 'src/a.c': 'int a;\n', 'src/b.c': 'int b;\n',
                                        'gen/m.h': '#define M 0\n'})
        patch = os.path.join(tmp, 'p.patch')
        write(tmp, {'p.patch': '--- a/src/a.c\n+++ b/src/a.c\n@@ -1 +1 @@\n-int a;\n+long a;\n'})
        old = G.ROOT
        G.ROOT = root
        try:
            assert G.vendored()
            h1 = G.vendored_checkout(work, [patch])
            assert open(os.path.join(work, 'src/a.c')).read() == 'long a;\n'
            assert os.path.exists(os.path.join(work, 'gen/m.h')) and h1 == G.G4.head(work)
            os.remove(os.path.join(root, sub, 'src/b.c'))
            write(os.path.join(root, sub), {'src/c.c': 'int c;\n'})
            G.vendored_checkout(work, [patch])
            assert not os.path.exists(os.path.join(work, 'src/b.c')) and os.path.exists(os.path.join(work, 'src/c.c'))
            assert subprocess.check_output(['git', '-C', work, 'diff', '--name-only'], text=True) == 'src/a.c\n'
        finally:
            G.ROOT = old


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
