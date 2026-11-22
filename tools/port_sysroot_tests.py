#!/usr/bin/env python3
"""Tests for port_sysroot.py: python3 tools/port_sysroot_tests.py"""
import hashlib
import io
import os
import sys
import tarfile
import tempfile
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import port_sysroot as P


def tree(d):
    """a fake root: usr/include/a.h, usr/lib/x/libz.so -> /usr/lib/x/libz.so.1 (absolute), the .so.1"""
    os.makedirs(os.path.join(d, 'usr/include'))
    os.makedirs(os.path.join(d, 'usr/lib/x'))
    open(os.path.join(d, 'usr/include/a.h'), 'w').write('int a;\n')
    open(os.path.join(d, 'usr/lib/x/libz.so.1'), 'wb').write(b'\x7fELF')
    os.symlink('/usr/lib/x/libz.so.1', os.path.join(d, 'usr/lib/x/libz.so'))
    return ['/usr/include/a.h', '/usr/lib/x/libz.so', '/usr/lib/x/libz.so.1']


def t_tar_deterministic_and_relative():
    with tempfile.TemporaryDirectory() as d:
        files = tree(d)
        a = P.tar_bytes(files, d)
        os.utime(os.path.join(d, 'usr/include/a.h'), (12345, 12345))
        assert P.tar_bytes(files, d) == a, 'mtime must not change the bytes'
        with tarfile.open(fileobj=io.BytesIO(a)) as t:
            m = {x.name: x for x in t.getmembers()}
        assert m['usr/lib/x/libz.so'].linkname == 'libz.so.1', m['usr/lib/x/libz.so'].linkname
        assert m['lib'].linkname == 'usr/lib' and m['lib64'].linkname == 'usr/lib64'
        assert m['usr/include/a.h'].uid == 0 and m['usr/include/a.h'].mtime == 0


def wheel(path):
    """a fake meson wheel: mesonbuild/mesonmain.py whose main() prints 9.8.7"""
    with zipfile.ZipFile(path, 'w') as z:
        z.writestr('mesonbuild/__init__.py', '')
        z.writestr('mesonbuild/mesonmain.py', 'def main():\n    print("9.8.7")\n    return 0\n')
    return hashlib.sha256(open(path, 'rb').read()).hexdigest()


def t_install_checks_hash_and_writes_ini():
    with tempfile.TemporaryDirectory() as d:
        files = tree(os.path.join(d, 'root'))
        tar = os.path.join(d, 's.tar')
        open(tar, 'wb').write(P.tar_bytes(files, os.path.join(d, 'root')))
        h = hashlib.sha256(open(tar, 'rb').read()).hexdigest()
        whl = os.path.join(d, 'm.whl')
        pin = {'sha256': h, 'meson': '9.8.7', 'meson_sha256': wheel(whl), 'clang': '21.1.8'}
        for bad in ({'sha256': '0' * 64}, {'meson_sha256': '0' * 64}):
            try:
                P.install(tar, os.path.join(d, 'bad'), dict(pin, **bad), whl)
                assert False, 'a wrong hash must stop'
            except SystemExit as e:
                assert 'pins' in str(e)
        out = os.path.join(d, 'sr')
        P.install(tar, out, pin, whl)
        assert open(os.path.join(out, 'usr/lib/x/libz.so'), 'rb').read() == b'\x7fELF'
        ini = open(os.path.join(out, 'native-ccache.ini')).read()
        assert f"cpp = ['ccache', 'clang++', '--config={out}/clang.cfg', '-ffile-prefix-map={out}=/sysroot', " in ini, ini
        assert open(os.path.join(out, 'clang.cfg')).read() == f'--sysroot={out}\n'   # not on the command line: ccache's base_dir
        assert """'-U__clang_version__', '-D__clang_version__="21.1.8"', '-U__VERSION__', '-D__VERSION__="Clang 21.1.8"']""" in ini, ini
        assert "c = ['clang', '--config=" in open(os.path.join(out, 'native.ini')).read()
        bad = P.check(out, pin)    # the meson runs (bin/meson -> the wheel's main); clang is whatever this host has
        assert not [b for b in bad if 'meson' in b or 'not the pinned' in b], bad
        bad = P.check(out, dict(pin, meson='1.0'))
        assert any('bin/meson is 9.8.7, the pin 1.0' in b for b in bad), bad
        assert any('not installed' in b for b in P.check(os.path.join(d, 'none'), pin))


def t_keep():
    assert P.keep('/usr/include/stdio.h') and P.keep('/usr/lib/x86_64-linux-gnu/libc.so.6')
    assert not P.keep('/usr/share/doc/libc6/copyright') and not P.keep('/usr/bin/ldd')


def t_pin_file():
    p = P.pinned()
    assert len(p['sha256']) == 64 and any(x.startswith('libc6 ') for x in p['packages']), p
    assert len(p['meson_sha256']) == 64 and p['meson'][0].isdigit() and p['clang'][0].isdigit(), p


if __name__ == "__main__":
    bad = 0
    for n, f in sorted(globals().items()):
        if n.startswith("t_"):
            try:
                f()
                print("ok", n)
            except AssertionError as e:
                bad = 1
                print("FAIL", n, repr(e))
    print("port_sysroot_tests:", "FAIL" if bad else "PASS")
    sys.exit(bad)
