#!/usr/bin/env python3
"""tools/release_paths_check_tests.py: release_paths_check.py on fake packages."""
import os
import sys
import tempfile
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import release_paths_check as rp


def tmp(name, data):
    p = os.path.join(tempfile.mkdtemp(), name)
    with open(p, 'wb') as f:
        f.write(data)
    return p


def zipof(members):
    p = os.path.join(tempfile.mkdtemp(), 'x.apk')
    with zipfile.ZipFile(p, 'w', zipfile.ZIP_DEFLATED) as z:
        for n, d in members.items():
            z.writestr(n, d)
    return p


def t_clean_file():
    assert rp.check([tmp('a.so', b'\0src/foo.c\0./build/port/x.c\0/usr/lib/x\0')]) == []


def t_private_paths():
    for s in [b'/root/ntr/slot6/src/a.c', b'/home/bob/x', b'/mnt/c/stress', b'C:\\Users\\Admin\\x', b'/Users/me/x',
              b'BDSERVER', b'DESKTOP-AB12CD3', b'C:\\stress\\pm1']:
        h = rp.check([tmp('a.exe', b'\0\1' + s + b'\0')])
        assert len(h) == 1, s


def t_zip_and_nested_zip():
    inner = zipof({'lib/x.so': b'\0/root/a.c\0'})
    assert len(rp.check([zipof({'a.txt': b'ok', 'lib/y.so': b'/home/zz/q'})])) == 1
    nested = zipof({'inner.zip': open(inner, 'rb').read()})
    h = rp.check([nested])
    assert len(h) == 1 and 'inner.zip!lib/x.so' in h[0][0], h


def t_folder():
    d = tempfile.mkdtemp()
    open(os.path.join(d, 'f'), 'wb').write(b'/root/x')
    assert len(rp.check([d])) == 1


def t_private_names_env():
    os.environ['BWPORT_PRIVATE_NAMES'] = 'sekrit, other'
    try:
        assert len(rp.check([tmp('a', b'..sekrit..')])) == 1
    finally:
        del os.environ['BWPORT_PRIVATE_NAMES']


if __name__ == '__main__':
    n = 0
    for k, v in sorted(globals().items()):
        if k.startswith('t_'):
            v()
            print('ok', k)
            n += 1
    print(f'{n} tests passed')
