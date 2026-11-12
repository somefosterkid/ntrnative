#!/usr/bin/env python3
"""Tests for tools/msgid_neutral.py on a synthetic tree:

    python3 tools/msgid_neutral_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import io
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import msgid_neutral as M  # noqa: E402

HEADER = """// sample
#ifndef X_H
#define X_H

#define Foo_Text_HelloThereFriend 0
#define Foo_Text_SecondLine 0x0b // trailing comment
#define Foo_Text_SameIndex 11
#ifdef BLACK2
#define Foo_Text_Color 24
#else
#define Foo_Text_Color 14
#endif
#define Foo_Text_Alias Foo_Text_SecondLine
#define Bar_Text_Already_Done 7

#endif // X_H
"""
USE = "int f(void) { return Foo_Text_HelloThereFriend + Foo_Text_SecondLine + Foo_Text_Color + Foo_Text_Alias; }\n"


def make(d):
    d = Path(d)
    (d / 'include/text').mkdir(parents=True)
    (d / 'src').mkdir()
    (d / 'docs').mkdir()
    (d / 'include/text/foo.h').write_text(HEADER)
    (d / 'src/a.c').write_text(USE)
    (d / 'docs/n.md').write_text('see Foo_Text_HelloThereFriend and Foo_Text_Unknown\n')
    return d


def snapshot(d):
    return {str(p): p.read_bytes() for p in sorted(Path(d).rglob('*')) if p.is_file()}


def test_rename_and_uses():
    with tempfile.TemporaryDirectory() as t:
        d = make(t)
        out = io.StringIO()
        M.run(d, ['include/text'], ['src', 'include', 'docs'], out=out)
        h = (d / 'include/text/foo.h').read_text()
        assert '#define Foo_Text_000 0\n' in h
        assert '#define Foo_Text_011 0x0b // trailing comment\n' in h
        assert h.count('#define Foo_Text_011') == 1      # SameIndex dropped as a duplicate
        assert 'HelloThere' not in h and 'SecondLine' not in h
        assert '#define Foo_Text_Var0 24\n' in h and '#define Foo_Text_Var0 14\n' in h
        assert '#define Foo_Text_Alias Foo_Text_011\n' in h      # the alias keeps its name, its value is renamed
        assert '#define Bar_Text_Already_Done 7' not in h
        assert '#define Bar_Text_007 7' in h
        assert (d / 'src/a.c').read_text() == (
            "int f(void) { return Foo_Text_000 + Foo_Text_011 + Foo_Text_Var0 + Foo_Text_Alias; }\n")
        assert (d / 'docs/n.md').read_text() == 'see Foo_Text_000 and Foo_Text_Unknown\n'


def test_idempotent():
    with tempfile.TemporaryDirectory() as t:
        d = make(t)
        M.run(d, ['include/text'], ['src', 'include', 'docs'], out=io.StringIO())
        a = snapshot(d)
        assert M.run(d, ['include/text'], ['src', 'include', 'docs'], out=io.StringIO()) == []
        assert snapshot(d) == a


def test_check_changes_nothing():
    with tempfile.TemporaryDirectory() as t:
        d = make(t)
        a = snapshot(d)
        assert M.main(['--root', str(d), '--check', '--dirs', 'src', 'include']) == 1
        assert snapshot(d) == a
        assert M.main(['--root', str(d), '--dirs', 'src', 'include']) == 0
        assert M.main(['--root', str(d), '--check', '--dirs', 'src', 'include']) == 0


def test_other_header_dir():
    with tempfile.TemporaryDirectory() as t:
        d = make(t)
        (d / 'gen').mkdir()
        (d / 'gen/bank.h').write_text('#define Bank0001_Text_ItsNoGood 44\n')
        (d / 'src/b.c').write_text('x = Bank0001_Text_ItsNoGood;\n')
        M.main(['--root', str(d), '--headers', 'gen', '--dirs', 'src', 'gen'])
        assert (d / 'src/b.c').read_text() == 'x = Bank0001_Text_044;\n'
        assert (d / 'gen/bank.h').read_text() == '#define Bank0001_Text_044 44\n'


def test_collision_raises():
    with tempfile.TemporaryDirectory() as t:
        d = Path(t)
        (d / 'h').mkdir()
        (d / 'h/a.h').write_text('#define P_Text_005 1\n#define P_Text_Word 5\n')
        try:
            M.run(d, ['h'], ['h'], out=io.StringIO())
        except ValueError:
            return
        raise AssertionError('no collision error')


def main():
    n = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            fn()
            print('ok', name)
            n += 1
    print('%d tests passed' % n)


if __name__ == '__main__':
    main()
