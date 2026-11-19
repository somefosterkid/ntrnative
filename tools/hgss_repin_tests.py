#!/usr/bin/env python3
"""Tests for tools/hgss_repin.py's patch split, handover tags and landings (hg39), ninja deps, affected set and boot-run comparison, and tools/pokegen4.py's
snapshot / keep_mtimes (scratch trees; no git, no clang, no exe):

    python3 tools/hgss_repin_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hgss_repin as R  # noqa: E402
import pokegen4 as G4  # noqa: E402

PATCH = """From: x
Subject: widen

diff --git a/src/a.c b/src/a.c
--- a/src/a.c
+++ b/src/a.c
@@ -1 +1 @@
-u32 p;
+uintptr_t p;
diff --git a/include/b.h b/include/b.h
--- a/include/b.h
+++ b/include/b.h
@@ -1 +1 @@
-int f();
+void *f();
"""

DEPS = """port/hg_src_a.c.o: #deps 3, deps mtime 1 (VALID)
    ../../pokeheartgold/src/a.c
    ../../pokeheartgold/include/b.h
    ../../../port/include/port.h

port/hg_src_c.c.o: #deps 2, deps mtime 1 (VALID)
    ../../pokeheartgold/src/c.c
    ../../pokeheartgold/gen/files/msg.h

port/own.c.o: #deps 1, deps mtime 1 (VALID)
    ../../../port/own.c
"""


def test_split_patch():
    head, files = R.split_patch(PATCH)
    assert head == 'From: x\nSubject: widen\n\n', head
    assert list(files) == ['src/a.c', 'include/b.h']
    assert files['include/b.h'].startswith('diff --git a/include/b.h') and files['include/b.h'].endswith('void *f();\n')
    assert head + ''.join(files.values()) == PATCH


def test_split_patch_bare_pieces():
    # hand-appended pieces (zhostfix's crash fixes): a prose line, then '--- a/' with no 'diff --git' (hg36)
    extra = 'B6 (prose: why)\n--- a/src/c.c\n+++ b/src/c.c\n@@ -2 +2 @@\n-int x;\n+long x;\n'
    head, files = R.split_patch(PATCH + extra)
    assert list(files) == ['src/a.c', 'include/b.h', 'src/c.c'], list(files)
    assert files['include/b.h'].endswith('void *f();\n') and files['src/c.c'] == extra
    assert head + ''.join(files.values()) == PATCH + extra


def test_failed_files():
    out = ('error: patch failed: src/a.c:12\nerror: src/a.c: patch does not apply\n'
           'error: include/new.h: already exists in working directory\n')
    assert set(R.FAILED_FILE.findall(out)) == {'src/a.c', 'include/new.h'}


def test_parse_deps_affected():
    d = R.parse_deps(DEPS, '/r/build/port/linux-hgss', '/r/build/pokeheartgold')
    assert d == {'src/a.c': ['include/b.h'], 'src/c.c': ['gen/files/msg.h']}, d
    srcs = ['src/a.c', 'src/c.c', 'src/new.c']
    assert R.affected({'include/b.h'}, d, srcs) == ['src/a.c', 'src/new.c']   # new.c: no deps known
    assert R.affected({'gen/files/msg.h', 'src/a.c'}, d, srcs) == srcs
    assert R.affected(set(), d, srcs[:2]) == []


def test_compare():
    crash = 'crash SIGSEGV in f (x.c:25) <- g (y.c:12), frame 16396'
    tr = ['1 1 0 aa 0', '2 2 1 bb 0', '3 3 0 cc 0']
    worse, why = R.compare(crash, crash, tr, tr)
    assert not worse and why.startswith('same end') and 'traces the same' in why, why
    worse, why = R.compare(crash, 'crash SIGSEGV in h (z.c:1), frame 9000', tr, tr[:1] + ['2 2 1 XX 0'])
    assert worse and 'before 16396' in why and 'V-blank 2' in why, why
    worse, why = R.compare(crash, 'reached-frame 20000 in 300 s (66.7 fps)', tr, tr)
    assert not worse and 'NEW end' in why, why
    assert R.end_frame('reached-frame 120 in 3 s (40.0 fps)') == 120 and R.end_frame('no output') == 0


def test_compare_by_frames():
    """The boot runs end on a game frame (port_run --frames): the same end frame is the same result on a slow host or a
    fast one, the traces are compared to that frame, and the watchdog is no result"""
    ok = 'exit-frame 60000 in 190 s (330 fps)'
    tr = ['%d %d 0 aa 0' % (i, i) for i in range(1, 8)]
    assert R.end_frame(ok) == 60000 and R.end_frame('exit-frame 60000 in 600 s (100 fps)') == 60000
    worse, why = R.compare(ok, 'exit-frame 60000 in 640 s (94 fps)', tr, tr)   # a loaded host: slower, not worse
    assert not worse and why.startswith('same end') and 'traces the same to frame 60000' in why, why
    worse, why = R.compare(ok, 'crash SIGSEGV in h (z.c:1), frame 9000', tr, tr)
    assert worse and 'before 60000' in why, why
    # the baseline went on past the new run's last trace line: only the lines up to the shorter end are compared
    worse, why = R.compare('exit-frame 5 in 1 s (5 fps)', 'exit-frame 9 in 1 s (9 fps)', tr[:6], tr[:5] + ['6 6 0 XX 0'])
    assert not worse and 'traces the same to frame 5' in why, why
    assert R.slow('watchdog: frame 900 of 60000 in 903 s (1 fps)') and not R.slow(ok) and not R.slow('hang in f, frame 9')


def test_upto():
    tr = ['1 a', '2 b', '3 c', '4 d']
    assert R.upto(tr, 3) == tr[:3] and R.upto(tr, 0) == [] and R.upto(tr, 9) == tr and R.upto(['x y', '1 a'], 1) == ['x y', '1 a']


def test_snapshot_keep_mtimes():
    with tempfile.TemporaryDirectory() as t:
        for f, s in {'src/a.c': 'a', 'src/b.c': 'b', 'gen/m.h': 'm', 'files/x.bin': 'x', '.git/c.h': 'g'}.items():
            os.makedirs(os.path.dirname(os.path.join(t, f)), exist_ok=True)
            open(os.path.join(t, f), 'w').write(s)
            os.utime(os.path.join(t, f), ns=(10**18, 10**18))
        snap = G4.snapshot(t)
        assert sorted(snap) == ['gen/m.h', 'src/a.c', 'src/b.c'], sorted(snap)
        for f, s in {'src/a.c': 'a', 'src/b.c': 'B', 'gen/m.h': 'm'}.items():   # rewritten; b.c changed
            open(os.path.join(t, f), 'w').write(s)
        os.remove(os.path.join(t, 'gen/m.h'))
        assert G4.keep_mtimes(t, snap) == 1
        assert os.stat(os.path.join(t, 'src/a.c')).st_mtime_ns == 10**18
        assert os.stat(os.path.join(t, 'src/b.c')).st_mtime_ns != 10**18


TAGGED = """Prose head of a hand patch (no tag of its own).

Handover-ID: hg36-0001
diff --git a/src/a.c b/src/a.c
--- a/src/a.c
+++ b/src/a.c
@@ -1 +1 @@
-u32 p;
+uintptr_t p;
Handover-ID: hg37-0001-ov01_021F6614, hg37-item3-ov01_021F0614
diff --git a/src/b.c b/src/b.c
--- a/src/b.c
+++ b/src/b.c
@@ -1 +1 @@
-int f();
+void *f();
B6 (prose: a hand-appended piece)
Handover-ID: none (port-only)
--- a/src/c.c
+++ b/src/c.c
@@ -2 +2 @@
-int x;
+long x;
Handover-ID: hg38-0001
diff --git a/src/d.c b/src/d.c
--- a/src/d.c
+++ b/src/d.c
@@ -1 +1 @@
-int y;
+long y;
"""


def test_handover_tags():
    head, files = R.split_patch(TAGGED)
    assert head == 'Prose head of a hand patch (no tag of its own).\n\n', repr(head)
    assert list(files) == ['src/a.c', 'src/b.c', 'src/c.c', 'src/d.c'], list(files)
    assert head + ''.join(files.values()) == TAGGED
    assert R.block_ids(files['src/a.c']) == ['hg36-0001']
    assert R.block_ids(files['src/b.c']) == ['hg37-0001-ov01_021F6614', 'hg37-item3-ov01_021F0614']
    assert R.block_ids(files['src/c.c']) == [] and R.block_ids(head) is None
    assert files['src/a.c'].endswith('+uintptr_t p;\n')       # the next block's tag went with the next block
    # a patch that is one handover says it in its head
    one = 'widepointers3: why\nHandover-ID: phs1-0002\n\n' + PATCH.split('\n\n', 1)[1]
    assert R.drop_landed(one, {'phs1-0002': 'full'}) == (None, {'src/a.c': 'full', 'include/b.h': 'full'})
    assert R.drop_landed(one, {'phs1-0001': 'full'}) == (one, {})


def test_handover_landed():
    log = ('hk488: heap.c HEAP_HOST_SIZE\n\nHandover: phs1-0002\nHandover: phs1-0001\n\n'
           'hk497: ov07 typed\n\nHandover: hg38-0001 partial\nHandover: hg38-0002\n\nhandover: hg36-0003 partial\n'
           'later\n\nHandover: hg36-0003\nnot a trailer: Handover: hg99-0001\n')
    tr = R.parse_trailers(log)
    assert tr == {'phs1-0002': 'full', 'phs1-0001': 'full', 'hg38-0001': 'partial', 'hg38-0002': 'full',
                  'hg36-0003': 'full'}, tr
    rows = R.parse_tsv('# id\tstatus\tcoverage\tlanding\nhg36-0005\tlanded\tfull\t66eb6c32f\tx\n'
                       'hg37-0001-ov01_021F6614-set-MapModelLoadTask.cancel-by-its-fie.patch\tpending\tfull\t-\tx\r\n')
    assert rows == [('hg36-0005', 'landed', 'full', '66eb6c32f'),
                    ('hg37-0001-ov01_021F6614-set-MapModelLoadTask.cancel-by-its-fie', 'pending', 'full', '-')], rows
    landed = {'hg36-0001': 'full', 'hg37-0001-ov01_021F6614-set-MapModelLoadTask.cancel-by-its-fie': 'full',
              'hg38-0001': 'partial'}
    assert R.landing(['hg36-0001'], landed) == 'full' and R.landing(['hg36-0002'], landed) is None
    assert R.landing(['hg37-0001-ov01_021F6614'], landed) == 'full'          # the backfill's long name
    assert R.landing(['hg37-0001-ov01'], {'hg37-0001-ov01_021F6614': 'full'}) is None    # no prefix inside a word
    assert R.landing(['hg37-0001-ov01_021F6614', 'hg37-item3-ov01_021F0614'], landed) == 'partial'   # one of two
    assert R.landing(['hg38-0001'], landed) == 'partial' and R.landing([], landed) is None
    assert R.landing(None, landed) is None
    text, got = R.drop_landed(TAGGED, landed)
    assert got == {'src/a.c': 'full', 'src/b.c': 'partial', 'src/d.c': 'partial'}, got
    head, files = R.split_patch(text)
    assert list(files) == ['src/b.c', 'src/c.c', 'src/d.c'] and head.startswith('Prose head'), list(files)
    text, got = R.drop_landed(TAGGED, {'hg36-0001': 'full', 'hg38-0001': 'full', 'hg37-0001-ov01_021F6614': 'full',
                                      'hg37-item3-ov01_021F0614': 'full'})
    assert list(R.split_patch(text)[1]) == ['src/c.c'], text       # the port-only block stays


# a toy zhostfix: 2 files x 2 IDs; src/b.c's two pieces are two handovers of one path (rpz1: ov12_0225B960's file held
# hg37's block and hgr4-0001's hunk)
ZTREE = {'src/a.c': ''.join('a%d\n' % i for i in range(1, 21)), 'src/b.c': ''.join('b%d\n' % i for i in range(1, 21))}
ZHOST = """zhostfix: host fixes

Handover-ID: hg37-0001
diff --git a/src/a.c b/src/a.c
--- a/src/a.c
+++ b/src/a.c
@@ -2,3 +2,3 @@
 a2
-a3
+A3
 a4
@@ -15,3 +15,3 @@
 a15
-a16
+A16
 a17
Handover-ID: hg37-0002
diff --git a/src/b.c b/src/b.c
--- a/src/b.c
+++ b/src/b.c
@@ -2,3 +2,3 @@
 b2
-b3
+B3
 b4
hgr4: the send-out's hunk, appended
Handover-ID: hgr4-0001
--- a/src/b.c
+++ b/src/b.c
@@ -15,3 +15,3 @@
 b15
-b16
+B16
 b17
"""


def test_zdrop_text():
    head, blocks = R.split_blocks(ZHOST)
    assert [p for p, _ in blocks] == ['src/a.c', 'src/b.c', 'src/b.c'] and head + ''.join(b for _, b in blocks) == ZHOST
    assert R.block_ids(blocks[2][1]) == ['hgr4-0001'] and R.block_ids(blocks[1][1]) == ['hg37-0002']
    text, gone, unused = R.zdrop(ZHOST, ['src/b.c:2', 'src/x.c:1'])
    assert gone == ['src/b.c @@-2 (hg37-0002)'] and unused == ['src/x.c:1'], (gone, unused)
    assert 'B16' in text and 'B3' not in text and 'A3' in text and 'Handover-ID: hgr4-0001' in text
    text, gone, _ = R.zdrop(ZHOST, ['hg37-0002'])          # an ID: its block only, not the path's other piece
    assert gone == ['src/b.c block (hg37-0002)'] and 'B16' in text and 'B3' not in text, gone
    text, gone, _ = R.zdrop(ZHOST, ['src/a.c:15'])          # one hunk of a two-hunk block
    assert 'A3' in text and 'A16' not in text and gone == ['src/a.c @@-15 (hg37-0001)'], gone
    text, gone, _ = R.zdrop(ZHOST, ['src/b.c'])             # a path: all of its blocks (by hand only)
    assert len(gone) == 2 and 'src/b.c' not in text
    # a landed ID drops its piece, not the other piece of the same path
    text, got = R.drop_landed(ZHOST, {'hg37-0002': 'full'})
    assert 'B16' in text and 'B3' not in text and got == {'src/b.c': 'full'}, got


PARA = """Head of a hand patch: what it is.

A1 (why a.c): the paragraph of the first block.
Handover-ID: none yet (decomp bug: a)
diff --git a/src/a.c b/src/a.c
--- a/src/a.c
+++ b/src/a.c
@@ -1 +1 @@
-u32 p;
+uintptr_t p;
B1 (why b.c): two lines
of prose.
Handover-ID: hg36-0001
diff --git a/src/b.c b/src/b.c
--- a/src/b.c
+++ b/src/b.c
@@ -1 +1 @@
-int f();
+void *f();
"""


def test_paragraph_goes_with_block():
    # r2fx (rev2): a block's paragraph is its own, not the tail of the block above: dropping the block drops it
    head, ((_, a), (_, b)) = R.split_blocks(PARA)
    assert head == 'Head of a hand patch: what it is.\n\n', repr(head)
    assert a.startswith('A1 (why a.c)') and b.startswith('B1 (why b.c)') and a.endswith('+uintptr_t p;\n'), (a, b)
    assert head + a + b == PARA and R.block_ids(a) == [] and R.block_ids(b) == ['hg36-0001']
    text, gone, _ = R.zdrop(PARA, ['src/b.c'])
    assert gone == ['src/b.c block (hg36-0001)'] and text == head + a, text
    text, got = R.drop_landed(PARA, {'hg36-0001': 'full'})
    assert text == head + a and got == {'src/b.c': 'full'}, text
    text, gone, _ = R.zdrop(PARA, ['src/a.c:1'])            # the block's only hunk: its paragraph goes, the head stays
    assert text == head + b, text
    # a head written straight into its first block keeps its prose (only the tag moves, as before)
    head, ((_, a), _) = R.split_blocks(PARA.replace('what it is.\n\n', 'what it is.\n'))
    assert head.endswith('first block.\n') and a.startswith('Handover-ID: none yet'), head


def test_zdrop_failing_hunk():
    # upstream changed b3: only that hunk FAILS; the watcher's retry (--zdrop of the ZDROP specs) keeps the other three
    with tempfile.TemporaryDirectory() as t:
        w, out = os.path.join(t, 'w'), os.path.join(t, 'out')
        os.makedirs(os.path.join(w, 'src')), os.makedirs(out)
        for f, s in ZTREE.items():
            Path(w, f).write_text(s.replace('b3\n', 'b3 upstream\n'))
        old_w, old_out, R.W, R.OUT = R.W, R.OUT, w, out
        try:
            r = R.Repin.__new__(R.Repin)
            _, files = R.split_patch(ZHOST)
            hs = r.hunks(files['src/b.c'])
            assert [(s, ids, v) for s, ids, _, v in hs] == [('src/b.c:2', ['hg37-0002'], 'FAILS'),
                                                            ('src/b.c:15', ['hgr4-0001'], 'applies')], hs
            assert r.file_report('src/b.c', files['src/b.c']) == ['src/b.c: @@2 FAILS, @@15 applies']
            assert all(v == 'applies' for *_, v in r.hunks(files['src/a.c']))
            specs = [s for f in files for s, _, _, v in r.hunks(files[f]) if v != 'applies']
            text, gone, _ = R.zdrop(ZHOST, specs)
            assert gone == ['src/b.c @@-2 (hg37-0002)'], gone
            Path(out, 'z.patch').write_text(text)
            rc = r.sh(['git', 'apply', '--check', os.path.join(out, 'z.patch')], check=False, cwd=w).returncode
            assert rc == 0 and 'B16' in text and 'A3' in text and 'A16' in text
        finally:
            R.W, R.OUT = old_w, old_out


def test_protos_stale_widening():
    """hgsw5: a carried wideprotos hunk (header uintptr_t) whose definition upstream made an int is put back; a
    .c-local declaration and a real pointer conflict are left alone."""
    import pokehg_protos as PP
    with tempfile.TemporaryDirectory() as d:
        for rel, text in {
            'include/a.h': 'uintptr_t f_a(int x);\nvoid *g_a(int x);\nint h_a(int x);\n',
            'src/a.c': 'int f_a(int x) {\n    return x;\n}\nint g_a(int x) {\n    return x;\n}\n'
                       'int h_a(int x) {\n    return x;\n}\n',
            'src/b.c': 'extern uintptr_t f_a(int x);\n',
        }.items():
            p = Path(d, rel)
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text)
        occ, nd = PP.scan(d)
        st = PP.stale(d, occ, nd)
        assert [(f, new.strip()) for f, _, new in st] == [('include/a.h', 'int')], st
        assert PP.unwiden(d, occ, nd) == 1
        assert Path(d, 'include/a.h').read_text().splitlines()[0] == 'int f_a(int x);'
        assert 'uintptr_t' in Path(d, 'src/b.c').read_text()


def test_missing_patches():
    with tempfile.TemporaryDirectory() as t:
        p = Path(t, 'port/subprojects/pokeheartgold-zhostfix.patch')
        p.parent.mkdir(parents=True)
        p.write_text('x\n')
        git = lambda *a: R.subprocess.run(['git', '-C', t, '-c', 'user.name=t', '-c', 'user.email=t@t'] + list(a),
                                          check=True, capture_output=True)
        git('init', '-q')
        git('add', '.')
        git('commit', '-qm', 'x')
        assert R.missing_patches(t) == []
        p.unlink()          # what a run killed while the late patches were aside leaves
        assert R.missing_patches(t) == ['port/subprojects/pokeheartgold-zhostfix.patch']
    try:
        R.killed(15, None)
        assert False
    except R.Killed as e:
        assert str(e) == 'killed by signal 15'


def test_hgss_build():
    """rv1: the tools' build dir: $HGSS_BUILD, else linux-hgss when it has compile commands, else the all-games build."""
    old = os.environ.pop('HGSS_BUILD', None)
    try:
        assert G4.hgss_build() in ('build/port/linux-hgss', 'build/port/linux')
        os.environ['HGSS_BUILD'] = 'build/x'
        assert G4.hgss_build() == 'build/x'
    finally:
        os.environ.pop('HGSS_BUILD', None)
        if old is not None:
            os.environ['HGSS_BUILD'] = old


def main():
    bad = 0
    for n, f in list(globals().items()):
        if n.startswith('test_'):
            try:
                f()
                print('ok', n)
            except AssertionError as e:
                bad += 1
                print('FAIL', n, e)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
