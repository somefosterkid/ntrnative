#!/usr/bin/env python3
"""HGSS on a 64-bit host: give every `struct FieldSystem_t` view the host layout of HG's real struct FieldSystem.

The m2c-typed files (ctx-packed: each carries its own context) declare the field system as `struct FieldSystem_t`, a copy of
pokeplatinum's FieldSystem with pads moved per file so the members that file touches sit at the right DS offsets
(252 views, 5 member orders). HG allocates and fills its own struct FieldSystem (include/field_system.h), whose pointers sit
elsewhere: the DS offsets agree, the host offsets do not (crash 36: sub_02054940 read the collision manager at host 0xC0,
HG keeps it at 0xB8). This pass keeps each view's names and types and puts every member at the host offset HG's struct
has for the member's DS offset: the members go into an anonymous union, each in its own anonymous struct after a pad, with
a _Static_assert per member (packed: a view pointer where HG keeps a 4-byte scalar lands 4-aligned). A member whose pointer-ness differs from HG's at that DS offset is listed (--report): the
view is wrong there, harmless while the file does not touch it.

    python3 tools/pokehg_fsview.py [--report]      # rewrites build/pokeheartgold in place (pokeheartgold-zfsview.patch is
                                                   # its diff against the tree with the other patches; remake it at a re-pin)
The real layout comes from DWARF: a probe including field_system.h, compiled with the HG module's command line
(build/port/linux-hgss/compile_commands.json), read with gdb's ptype/o.
"""
import argparse, glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TREE = os.path.join(ROOT, 'build/pokeheartgold')
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pokegen4  # noqa: E402
BUILD = os.path.join(ROOT, pokegen4.hgss_build())
VIEW = re.compile(r'^struct FieldSystem_t \{\n(.*?)^\};\n', re.M | re.S)
MARK = '// host layout: HG struct FieldSystem by DS offset (tools/pokehg_fsview.py)'
SCALAR = {'u8': 1, 's8': 1, 'bool8': 1, 'char': 1, 'u16': 2, 's16': 2, 'unsigned short': 2, 'short': 2}
DS4 = {'FieldWildBattleMetadata'}   # 4 DS bytes, no pointer


def real_layout(build, tmp):
    """HG's struct FieldSystem: [(name, ds_offset, ds_size, host_offset, host_size, is_pointer)]"""
    cmd = None
    for e in json.load(open(os.path.join(build, 'compile_commands.json'))):
        if e['file'].endswith('pokeheartgold/src/field_system.c'):
            cmd, cwd = e['command'], e['directory']
            break
    if cmd is None:
        sys.exit('pokehg_fsview: no field_system.c in compile_commands.json (build the HGSS exe once)')
    os.makedirs(tmp, exist_ok=True)
    src, obj = os.path.join(tmp, 'fsprobe.c'), os.path.join(tmp, 'fsprobe.o')
    open(src, 'w').write('#include "global.h"\n#include "field_system.h"\nstruct FieldSystem *hgfs_probe;\n')
    c = cmd.split(' -o ')[0].replace(' -c ', ' ')
    c = re.sub(r'\S*pokeheartgold/src/field_system\.c', '', c)
    subprocess.check_call(c + f' -g -c -Wno-everything -o {obj} {src}', shell=True, cwd=cwd)
    out = subprocess.check_output(['gdb', '-q', '-batch', '-ex', 'ptype/o struct FieldSystem', obj], text=True)
    lay, ds = [], 0
    total = re.search(r'total size \(bytes\):\s*(\d+)', out)
    real_layout.total = int(total[1])
    for l in out.split('\n'):
        m = re.match(r'/\*\s+(\d+)(?::\s*(\d+))?\s+\|\s+(\d+) \*/\s+(.*?)\s*(\w+)(\[\d+\])?( : \d+)?;$', l)
        if not m:
            continue
        host, bit, hsz, typ, name, arr, bf = int(m[1]), m[2], int(m[3]), m[4], m[5], m[6], m[7]
        if bf and bit and int(bit) > 0:
            continue   # the rest of a bitfield byte
        ptr = '*' in typ
        dsz = 4 if ptr else (1 if bf else hsz)
        if not ptr and not arr and dsz != hsz:
            sys.exit(f'pokehg_fsview: {name}: no DS size rule')
        if not ptr and not arr and not typ.split()[-1] in SCALAR and hsz > 8:
            dsz = None   # an embedded struct (FollowMon): its DS size is not the host's; stop the map here
        al = 1 if arr or bf else min(dsz or 4, 4)
        ds = (ds + al - 1) // al * al
        lay.append((name, ds, dsz, host, hsz, ptr))
        if dsz is None:
            break
        ds += dsz
    return lay


def ds_to_host(lay, x):
    """(host offset, real member, pointer) for DS offset x, or None"""
    for name, ds, dsz, host, hsz, ptr in lay:
        if dsz is None:
            return (host, name, ptr) if x == ds else None
        if ds <= x < ds + dsz:
            if x != ds and ptr:
                return None
            return host + (x - ds), name, ptr
    return None


def parse_view(body, path):
    """[(line, name, ds_offset, is_pointer)] of a view's member lines"""
    out, ds = [], 0
    lines = [l.strip() for l in body.split('\n') if l.strip()]
    for i, l in enumerate(lines):
        m = re.match(r'(.*?)\s*\b(\w+)(?:\[(\w+)\])?;$', l)
        if not m or l.startswith(('//', '/*')) or '(' in l:
            sys.exit(f'pokehg_fsview: {path}: member line not understood: {l}')
        typ, name, arr = m[1], m[2], m[3]
        ptr = '*' in typ
        base = re.sub(r'\b(const|volatile)\b', '', typ).strip()
        if ptr:
            sz = 4
        elif base.startswith('struct {'):
            inner = re.findall(r'(\w+(?: \w+)?) \w+;', base)
            sz = sum(SCALAR.get(t, 4) for t in inner)
        elif base.replace('struct ', '') in SCALAR:
            sz = SCALAR[base.replace('struct ', '')]
        elif base in DS4 or not base[:1].isupper() or base in ('BOOL',):
            sz = 4
        else:
            sz = None   # a struct of unknown DS size: only allowed last
            if i != len(lines) - 1:
                sys.exit(f'pokehg_fsview: {path}: {l}: DS size unknown and not the last member')
        if arr:
            sz = sz * int(arr, 0)
        al = 1 if arr else min(sz or 4, 4)
        ds = (ds + al - 1) // al * al
        out.append((l, name, ds, ptr))
        ds += sz or 0
    return out


def new_view(members, lay, path, report):
    full = real_layout.total
    body = ['struct FieldSystem_t { ' + MARK, '    union {', f'        u8 hgfs_size[{full}];', '        void *hgfs_align;']
    asserts = []
    for line, name, ds, ptr in members:
        r = ds_to_host(lay, ds)
        if r is None:
            sys.exit(f'pokehg_fsview: {path}: {name} at DS {ds:#x}: no HG member there')
        host, rname, rptr = r
        if rptr != ptr:
            report.append(f'{os.path.relpath(path, TREE)}: {name} (DS {ds:#x}) {"pointer" if ptr else "scalar"}, '
                          f'HG {rname} {"pointer" if rptr else "scalar"}')
        pad = f'u8 hgfs_p{ds:X}[{host}]; ' if host else ''
        body.append(f'        struct __attribute__((packed)) {{ {pad}{line} }}; // DS {ds:#x}: HG {rname}')
        asserts.append(f'_Static_assert(__builtin_offsetof(struct FieldSystem_t, {name}) == {host}, "FieldSystem_t.{name}");')
    body += ['    };', '};'] + asserts
    return '\n'.join(body) + '\n'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default=TREE)
    ap.add_argument('--build', default=BUILD)
    ap.add_argument('--tmp', default='/tmp/pokehg_fsview')
    ap.add_argument('--report', action='store_true', help='list the members whose pointer-ness differs from HG at that offset')
    o = ap.parse_args()
    lay = real_layout(o.build, o.tmp)
    files = [f for pat in ('src/**/*.c', 'include/**/*.h', 'port/**/*.c') for f in glob.glob(os.path.join(o.tree, pat), recursive=True)]
    n = v = 0
    report = []
    for f in sorted(files):
        s = open(f, errors='surrogateescape').read()
        if 'struct FieldSystem_t {' not in s or MARK in s:
            continue
        def sub(m):
            nonlocal v
            v += 1
            return new_view(parse_view(m.group(1), f), lay, f, report)
        t = VIEW.sub(sub, s)
        if t != s:
            open(f, 'w', errors='surrogateescape').write(t)
            n += 1
    print(f'pokehg_fsview: {v} views in {n} files; HG FieldSystem {real_layout.total} host bytes mapped to DS {lay[-1][1]:#x}; '
          f'{len(report)} pointer-ness conflicts')
    if o.report:
        print('\n'.join(report))


if __name__ == '__main__':
    main()
