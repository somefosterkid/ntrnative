#!/usr/bin/env python3
"""Tests for win_sleep_bench.c: it compiles with mingw (skipped without a mingw compiler)."""
import os
import shutil
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))


def t_compiles():
    cc = shutil.which('x86_64-w64-mingw32-gcc')
    if not cc:
        return
    with tempfile.TemporaryDirectory() as tmp:
        r = subprocess.run([cc, '-O2', '-Wall', '-Werror', '-o', os.path.join(tmp, 'b.exe'), os.path.join(HERE, 'win_sleep_bench.c')],
                           capture_output=True, text=True)
        assert r.returncode == 0, r.stderr


if __name__ == '__main__':
    n = 0
    for k, v in sorted(globals().items()):
        if k.startswith('t_'):
            v()
            n += 1
    print(f'win_sleep_bench_tests: {n} ok')
