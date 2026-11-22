#!/usr/bin/env python3
r"""tools/release_paths_check.py PACKAGE... : no private build paths in a package that leaves this machine.

A shipped exe, .so, APK, IPA or app zip must not carry the builder's checkout or user dirs (/root/..., /home/x, /mnt/c,
/Users/x, C:\Users, \stress\), machine names (BDSERVER, DESKTOP-xxxx) or account names (BWPORT_PRIVATE_NAMES, comma
separated, adds to the built-in list). The builds map the checkout root to "." (-ffile-prefix-map, port/meson.build), so
__FILE__ strings and debug info are relative. Takes what rom_leak_check.py takes: an apk/ipa/zip (members, nested zips
too), a folder, plain files. Prints each hit (file, offset, the 60 bytes around) and exits 1 on any; 0 when clean.
Called by tools/release_check.sh. A path a package may hold goes in ALLOW below with the reason."""
import os
import re
import sys
import zipfile
import io

PATTERNS = [rb'/root/', rb'/home/[A-Za-z0-9_.-]', rb'/mnt/[a-z]/', rb'/Users/[A-Za-z0-9_.-]', rb'[A-Za-z]:\\Users\\',
            rb'[A-Za-z]:/Users/', rb'\\stress\\', rb'/stress/', rb'BDSERVER', rb'DESKTOP-[A-Z0-9]{5,}']
# (regex of the matched line) that is not ours: none yet
ALLOW = []


def regex():
    pats = list(PATTERNS)
    for n in os.environ.get('BWPORT_PRIVATE_NAMES', '').split(','):
        if n.strip():
            pats.append(re.escape(n.strip().encode()))
    # the match with context: up to 30 printable bytes either side
    return re.compile(rb'[\x20-\x7e]{0,30}(?:' + b'|'.join(pats) + rb')[\x20-\x7e]{0,30}')


def scan_bytes(name, data, rx, hits):
    for m in rx.finditer(data):
        s = m.group(0).decode('ascii', 'replace')
        if not any(re.search(a, s) for a in ALLOW):
            hits.append((name, m.start(), s))


def scan_zip(name, f, rx, hits):
    with zipfile.ZipFile(f) as z:
        for i in z.infolist():
            if i.is_dir():
                continue
            d = z.read(i)
            scan_member(name + '!' + i.filename, d, rx, hits)


def scan_member(name, data, rx, hits):
    if data[:4] == b'PK\x03\x04' and name.lower().endswith(('.zip', '.apk', '.ipa', '.jar', '.aar')):
        try:
            scan_zip(name, io.BytesIO(data), rx, hits)
            return
        except zipfile.BadZipFile:
            pass
    scan_bytes(name, data, rx, hits)


def scan_path(p, rx, hits):
    if os.path.isdir(p):
        for r, _, fs in os.walk(p):
            for f in sorted(fs):
                scan_path(os.path.join(r, f), rx, hits)
    elif zipfile.is_zipfile(p):
        scan_zip(p, p, rx, hits)
    else:
        with open(p, 'rb') as f:
            scan_bytes(p, f.read(), rx, hits)


def check(paths):
    rx = regex()
    hits = []
    for p in paths:
        scan_path(p, rx, hits)
    return hits


def main(argv):
    if not argv:
        print('usage: tools/release_paths_check.py PACKAGE...')
        return 2
    hits = check(argv)
    for n, o, s in hits[:40]:
        print(f'{n}:{o}: {s}')
    if len(hits) > 40:
        print(f'... {len(hits) - 40} more')
    print(f'release_paths_check: {len(hits)} private path/name hit(s) in {" ".join(argv)}')
    return 1 if hits else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
