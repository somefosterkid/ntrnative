#!/usr/bin/env python3
"""Tests for port/machine/ccache-nopch (the compiler launcher of linux-ccache.ini): objects go through ccache with
CCACHE_PCH_EXTSUM=1, a `-o X.pch` compile does not, and writes X.pch.sum, the same in two checkouts for the same header text and flags:

    python3 tools/ccache_nopch_tests.py      # prints "ok" per test, exits 1 on a failure
"""
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

LAUNCHER = Path(__file__).resolve().parent.parent / 'port' / 'machine' / 'ccache-nopch'

# fake clang: -E prints the input header (the last argument) and fails on "FAILE"; a compile writes the -o file
FAKE_CLANG = r"""#!/bin/sh
echo "clang $*" >> "$FAKE_LOG"
out=; e=; for a in "$@"; do [ "$prev" = -o ] && out=$a; [ "$a" = -E ] && e=1; prev=$a; case $a in *.h) last=$a;; esac; done
if [ -n "$e" ]; then [ -n "$FAKE_E_FAIL" ] && exit 1; cat "$last"; exit 0; fi
[ -n "$out" ] && echo built > "$out"
exit 0
"""
FAKE_CCACHE = '#!/bin/sh\necho "ccache EXTSUM=$CCACHE_PCH_EXTSUM $*" >> "$FAKE_LOG"\n'


class Env:
    """a fake checkout <tmp>/<name> with the launcher, a fake clang + ccache on PATH, a build dir <checkout>/build/port/b"""

    def __init__(self, tmp, name, header_text):
        self.root = Path(tmp, name)
        (self.root / 'port/machine').mkdir(parents=True)
        shutil.copy(LAUNCHER, self.root / 'port/machine/ccache-nopch')
        self.bin = Path(tmp, name + '-bin')
        self.bin.mkdir()
        for n, body in (('clang', FAKE_CLANG), ('ccache', FAKE_CCACHE)):
            (self.bin / n).write_text(body)
            (self.bin / n).chmod(0o755)
        self.build = self.root / 'build/port/b'
        self.build.mkdir(parents=True)
        self.header = self.root / 'port/include/pch.h'
        self.header.parent.mkdir(parents=True)
        self.header.write_text(header_text.replace('ROOT', str(self.root)))
        self.log = Path(tmp, name + '.log')
        self.log.write_text('')

    def run(self, *args, **extra):
        env = dict(os.environ, PATH=f'{self.bin}:{os.environ["PATH"]}', FAKE_LOG=str(self.log), **extra)
        return subprocess.run([str(self.root / 'port/machine/ccache-nopch'), 'clang', *args], cwd=self.build, env=env,
                              capture_output=True, text=True)

    def pch(self, *flags, **extra):
        return self.run(*flags, '-MD', '-MQ', 'x.pch', '-MF', 'x.pch.d', '-o', 'x.pch', '-c', str(self.header), **extra)

    def sum(self):
        p = self.build / 'x.pch.sum'
        return p.read_text() if p.exists() else None


def test_object_goes_through_ccache_with_extsum():
    with tempfile.TemporaryDirectory() as t:
        e = Env(t, 'a', 'int x;\n')
        assert e.run('-O2', '-include-pch', 'x.pch', '-o', 'a.o', '-c', 'a.c').returncode == 0
        assert 'ccache EXTSUM=1 clang -O2 -include-pch x.pch' in e.log.read_text()


def test_pch_compile_skips_ccache_and_writes_sum():
    with tempfile.TemporaryDirectory() as t:
        e = Env(t, 'a', 'int x;\n')
        assert e.pch('-O2').returncode == 0
        assert (e.build / 'x.pch').exists() and e.sum() and len(e.sum().strip()) == 64
        assert 'ccache' not in e.log.read_text()


def test_same_sum_in_another_checkout_and_dir():
    with tempfile.TemporaryDirectory() as t:
        a, b = Env(t, 'a', '// ROOT/gen\nint x;\n'), Env(t, 'bb', '// ROOT/gen\nint x;\n')
        a.pch('-O2', f'-I{a.root}/inc')
        b.pch('-O2', f'-I{b.root}/inc')
        assert a.sum() and a.sum() == b.sum()


def test_other_header_or_flags_other_sum():
    with tempfile.TemporaryDirectory() as t:
        a, b, c = Env(t, 'a', 'int x;\n'), Env(t, 'b', 'int y;\n'), Env(t, 'c', 'int x;\n')
        a.pch('-O2')
        b.pch('-O2')
        c.pch('-O0')
        assert len({a.sum(), b.sum(), c.sum()}) == 3


def test_failed_preprocess_leaves_no_sum():
    with tempfile.TemporaryDirectory() as t:
        e = Env(t, 'a', 'int x;\n')
        e.pch('-O2')
        assert e.sum()
        assert e.pch('-O2', FAKE_E_FAIL='1').returncode == 0
        assert e.sum() is None            # the old sum is gone too: ccache hashes the .pch itself


def test_sumonly_writes_the_same_sum_without_compiling():   # ccx2: pch_sums() of an existing build dir
    with tempfile.TemporaryDirectory() as t:
        a, b = Env(t, 'a', 'int x;\n'), Env(t, 'b', 'int x;\n')
        a.pch('-O2')
        (b.build / 'x.pch').write_text('old')
        r = b.pch('-O2', CCACHE_NOPCH_SUMONLY='1')
        assert r.returncode == 0 and b.sum() == a.sum() and b.sum()
        assert (b.build / 'x.pch').read_text() == 'old'              # not rebuilt (the fake clang writes "built")
        assert len(b.log.read_text().splitlines()) == 1 and ' -E ' in b.log.read_text()   # only the preprocess run


def test_failed_pch_compile_returns_its_status():
    with tempfile.TemporaryDirectory() as t:
        e = Env(t, 'a', 'int x;\n')
        (e.bin / 'clang').write_text('#!/bin/sh\nexit 3\n')
        assert e.pch('-O2').returncode == 3 and e.sum() is None


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_'):
            try:
                fn()
                print('ok', name)
            except AssertionError:
                print('FAIL', name)
                bad = 1
    sys.exit(bad)
