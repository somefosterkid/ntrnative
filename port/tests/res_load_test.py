#!/usr/bin/env python3
"""The resource loaders on a 64-bit host: port/tests/res_load_test.c on every NitroSystem/SPL file of the ROM.

    python3 port/tests/res_load_test.py [--build build/port/linux] [--rom orig/baserom_b_us.nds] [--out DIR] [KIND ...]

Extracts each NARC member (LZ10/LZ11 undone) whose magic is a NitroSystem or SPL resource into OUT/<KIND>/<n>.bin
(duplicates once; OUT/index.tsv: kind, file, ROM path, member, compression), once per OUT. Builds the test with
libntrsystem's load sources, its g3d resource accessors and lib/spl's spl_manager.c from the PC build's own compile
commands (so the checkouts and patches the build uses: run tools/port_run.py or ninja first), under ASan, and runs it.
--nnsys: port/src/nnsys_load64.c's loaders and lib/nnsys's G3D code instead (the nnsys build's NitroSystem); with
RES_DUMP=FILE in the environment every result is written out field by field: run both and compare the dumps.
KINDS: NCGR NCLR NSCR NCER NANR NMAR NMCR NFTR NFTRDWC (the DWC settings' fonts, dwc/utility.bin) NSBMD NSBTX NSBCA NSBTP NSBTA NSBMA NSBVA SPA (default: all but NFTR).
Exit code 0 when every file of every kind checked out."""
import argparse, hashlib, json, os, shlex, struct, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
KINDS = {b'RGCN': 'NCGR', b'RLCN': 'NCLR', b'RCSN': 'NSCR', b'RECN': 'NCER', b'RNAN': 'NANR', b'RCMN': 'NMCR',
         b'RAMN': 'NMAR', b'RNEN': 'NENR', b'RTFN': 'NFTR', b'BMD0': 'NSBMD', b'BTX0': 'NSBTX', b'BCA0': 'NSBCA',
         b'BTP0': 'NSBTP', b'BTA0': 'NSBTA', b'BMA0': 'NSBMA', b'BVA0': 'NSBVA', b' APS': 'SPA'}
SOURCES = ['g2d/src/load/g2d_Load.c', 'g2d/src/load/g2d_NAN_load.c', 'g2d/src/load/g2d_NCG_load.c',
           'g2d/src/load/g2d_NCL_load.c', 'g2d/src/load/g2d_NFT_load.c', 'g2d/src/load/g2d_NMC_load.c',
           'g2d/src/load/g2d_NOB_load.c', 'g2d/src/load/g2d_NSC_load.c', 'g3d/src/binres/res_struct.c',
           'g3d/src/binres/res_struct_accessor.c', 'g3d/src/binres/res_struct_accessor_anm.c', 'g3d/src/kernel.c',
           'lib/spl/src/spl_manager.c']
ASAN = ['-fsanitize=address', '-fno-omit-frame-pointer']


def lz(data):
    """LZ10 / LZ11 (MI_UncompressLZ8's two formats), None when DATA isn't one"""
    if len(data) < 4 or data[0] not in (0x10, 0x11):
        return None
    size, pos = int.from_bytes(data[1:4], 'little'), 4
    if size == 0:
        size, pos = int.from_bytes(data[4:8], 'little'), 8
    if size == 0 or size > 16 << 20:
        return None
    out, ext = bytearray(), data[0] == 0x11
    try:
        while len(out) < size:
            flags = data[pos]; pos += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if not flags & (0x80 >> bit):
                    out.append(data[pos]); pos += 1
                    continue
                b0, hi = data[pos], data[pos] >> 4
                if not ext:
                    n, disp, pos = hi + 3, ((b0 & 15) << 8 | data[pos + 1]) + 1, pos + 2
                elif hi == 0:
                    n = ((b0 & 15) << 4 | data[pos + 1] >> 4) + 0x11
                    disp, pos = ((data[pos + 1] & 15) << 8 | data[pos + 2]) + 1, pos + 3
                elif hi == 1:
                    n = ((b0 & 15) << 12 | data[pos + 1] << 4 | data[pos + 2] >> 4) + 0x111
                    disp, pos = ((data[pos + 2] & 15) << 8 | data[pos + 3]) + 1, pos + 4
                else:
                    n, disp, pos = hi + 1, ((b0 & 15) << 8 | data[pos + 1]) + 1, pos + 2
                if disp > len(out):
                    return None
                for _ in range(n):
                    out.append(out[-disp])
    except IndexError:
        return None
    return bytes(out[:size])


def narc(data):
    if data[:4] != b'NARC':
        return None
    p = struct.unpack_from('<H', data, 12)[0]
    if data[p:p + 4] != b'BTAF':
        return None
    size, n = struct.unpack_from('<IH', data, p + 4)
    entries = [struct.unpack_from('<II', data, p + 12 + 8 * i) for i in range(n)]
    p += size
    p += struct.unpack_from('<I', data, p + 4)[0]       # BTNF
    if data[p:p + 4] != b'GMIF':
        return None
    return [data[p + 8 + s:p + 8 + e] for s, e in entries]


def rom_files(rom):
    fnt, _, fat, fat_size = struct.unpack_from('<IIII', rom, 0x40)
    names = {}

    def walk(dir_id, path):
        sub, fid = struct.unpack_from('<IH', rom, fnt + 8 * (dir_id & 0xfff))
        p = fnt + sub
        while rom[p]:
            t = rom[p]; name = rom[p + 1:p + 1 + (t & 0x7f)].decode('latin-1'); p += 1 + (t & 0x7f)
            if t & 0x80:
                walk(struct.unpack_from('<H', rom, p)[0], path + name + '/'); p += 2
            else:
                names[fid] = path + name; fid += 1
    walk(0xf000, '')
    for fid in range(fat_size // 8):
        s, e = struct.unpack_from('<II', rom, fat + 8 * fid)
        yield names.get(fid, f'#{fid}'), rom[s:e]


def sdk_archive(data):
    """the members of an SDK archive (FNT, FAT, the files; offsets from its start: dwc/utility.bin)"""
    fnt, _, fat, fat_size = struct.unpack_from('<IIII', data, 0)
    return [data[s:e] for s, e in
            (struct.unpack_from('<II', data, fat + 8 * i) for i in range(fat_size // 8))]


def extract(rom_path, out):
    rom = open(rom_path, 'rb').read()
    seen, count, index = set(), {}, []
    for path, data in rom_files(rom):
        dwc = path == 'dwc/utility.bin'   # the DWC settings' fonts, NitroSystem's (the game's own, a/0/2/3, are GFL's)
        members = sdk_archive(data) if dwc else narc(data)
        for idx, m in enumerate(members if members is not None else [data]):
            raw, packed = m, ''
            if m[:4] not in KINDS:
                raw = lz(m)
                if raw is None or raw[:4] not in KINDS:
                    continue
                packed = 'lz%x' % m[0]
            if dwc and raw[:4] != b'RTFN':
                continue
            h = hashlib.sha1(raw).digest()
            if h in seen:
                continue
            seen.add(h)
            kind = KINDS[raw[:4]] + ('DWC' if dwc else '')
            n = count[kind] = count.get(kind, -1) + 1
            os.makedirs(os.path.join(out, kind), exist_ok=True)
            open(os.path.join(out, kind, f'{n}.bin'), 'wb').write(raw)
            index.append(f'{kind}\t{kind}/{n}.bin\t{path}\t{idx if members is not None else "-"}\t{packed}')
    open(os.path.join(out, 'index.tsv'), 'w').write('\n'.join(index) + '\n')
    print('extracted:', ' '.join(f'{k}={v + 1}' for k, v in sorted(count.items())), flush=True)


def retarget(cmd, src, obj):
    """a compile command with SRC and OBJ in place of its own, no dependency files"""
    args, out, skip = shlex.split(cmd), [], False
    for a in args:
        if skip:
            skip = False
        elif a in ('-o', '-MF', '-MQ'):
            skip = True
            out += ['-o', obj] if a == '-o' else []
        elif a == '-c':
            out += ['-c', src]; skip = True
        elif a != '-MD':
            out.append(a)
    return out + ASAN


def build(build_dir, out):
    cc = json.load(open(os.path.join(build_dir, 'compile_commands.json')))
    objs, game = [], None
    for want in SOURCES:
        c = next((c for c in cc if c['file'].endswith(want)), None)
        if c is None:
            sys.exit(f'res_load_test: no compile command for {want} in {build_dir}')
        src = os.path.normpath(os.path.join(c['directory'], c['file']))
        obj = os.path.join(out, os.path.basename(want) + '.o')
        game = c if want.startswith('lib/spl') else game
        r = subprocess.run(retarget(c['command'], src, obj), cwd=c['directory'], capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stderr[-3000:])
        objs.append(obj)
    # the test: the game's flags (port_prefix.h ...) and libntrsystem's own headers, from the build's checkout
    nns = os.path.join(REPO, 'port/subprojects/libntrsystem')
    args = retarget(game['command'], os.path.join(HERE, 'res_load_test.c'), os.path.join(out, 'res_load_test.o'))
    args[1:1] = ['-I' + os.path.join(nns, 'include'), '-I' + os.path.join(nns, 'libraries/g2d/include')]
    r = subprocess.run(args, cwd=game['directory'], capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr[-4000:])
    exe = os.path.join(out, 'res_load_test')
    # what the loaders reference and never call here (GX loads, the SPL behaviours ...) stays unresolved
    r = subprocess.run(['clang', '-no-pie'] + ASAN + ['-o', exe] + objs + [os.path.join(out, 'res_load_test.o'),
                       '-Wl,--unresolved-symbols=ignore-all'], capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr[-3000:])
    return exe


# --nnsys: port/src/nnsys_load64.c's loaders and lib/nnsys's G3D accessors, on lib/nnsys's headers (the PC build's
# nnsys option: libntrsystem's include dirs out, port/include_nnsys and lib/nnsys/include in, BWPORT_NNSYS)
NNSYS_SOURCES = ['port/src/nnsys_load64.c', 'lib/nnsys/src/g3d/binres/res_struct_accessor.c',
                 'lib/nnsys/src/g3d/binres/res_struct_accessor_anm.c', 'lib/nnsys/src/g3d/kernel.c',
                 'lib/spl/src/spl_manager.c']


def nnsys_cmd(game, src, obj):
    args = [a for a in retarget(game['command'], src, obj) if 'libntrsystem' not in a]
    args[1:1] = ['-I' + os.path.join(REPO, 'port/include_nnsys'), '-I' + os.path.join(REPO, 'lib/nnsys/include'),
                 '-DBWPORT_NNSYS']
    if '/lib/nnsys/' in src:
        args[1:1] = ['-include', os.path.join(REPO, 'port/include_nnsys/nnsys_lib.h')]
    return args


def build_nnsys(build_dir, out):
    cc = json.load(open(os.path.join(build_dir, 'compile_commands.json')))
    game = next(c for c in cc if c['file'].endswith('lib/spl/src/spl_manager.c'))
    objs = []
    for want in NNSYS_SOURCES + ['port/tests/res_load_test.c']:
        obj = os.path.join(out, 'nnsys_' + os.path.basename(want) + '.o')
        r = subprocess.run(nnsys_cmd(game, os.path.join(REPO, want), obj), cwd=game['directory'], capture_output=True,
                           text=True)
        if r.returncode:
            sys.exit(f'{want}:\n' + r.stderr[-4000:])
        objs.append(obj)
    exe = os.path.join(out, 'res_load_test_nnsys')
    r = subprocess.run(['clang', '-no-pie'] + ASAN + ['-o', exe] + objs + ['-lpthread',
                       '-Wl,--unresolved-symbols=ignore-all'], capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr[-3000:])
    return exe


def build_nmc(build_dir, out):
    """port/tests/nmc_test.c on nnsys_load64.c and lib/nnsys's expanded heap (the free hook's wrapper)"""
    cc = json.load(open(os.path.join(build_dir, 'compile_commands.json')))
    game = next(c for c in cc if c['file'].endswith('lib/spl/src/spl_manager.c'))
    objs = []
    for want in ['port/src/nnsys_load64.c', 'port/extern/lib/nnsys/src/fnd/expheap.c', 'lib/nnsys/src/fnd/heapcommon.c',
                 'lib/nnsys/src/fnd/list.c', 'port/tests/nmc_test.c']:
        obj = os.path.join(out, 'nmc_' + os.path.basename(want) + '.o')
        r = subprocess.run(nnsys_cmd(game, os.path.join(REPO, want), obj), cwd=game['directory'], capture_output=True,
                           text=True)
        if r.returncode:
            sys.exit(f'{want}:\n' + r.stderr[-4000:])
        objs.append(obj)
    exe = os.path.join(out, 'nmc_test_nnsys')
    r = subprocess.run(['clang', '-no-pie'] + ASAN + ['-o', exe] + objs + ['-lpthread',
                       '-Wl,--unresolved-symbols=ignore-all'], capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr[-3000:])
    return exe


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', default=os.path.join(REPO, 'build/port/linux'))
    ap.add_argument('--rom', default=os.environ.get('BWPORT_ROM', os.path.join(REPO, 'orig/baserom_b_us.nds')))
    ap.add_argument('--out', default=os.path.join(os.environ.get('TMPDIR', '/tmp'), 'res_load_test'))
    ap.add_argument('--nnsys', action='store_true', help="nnsys_load64.c's loaders on lib/nnsys (default: libntrsystem's)")
    ap.add_argument('--nmc', action='store_true', help='run port/tests/nmc_test.c on --nnsys (the free hook) with '
                    'the first 16 NMCR files instead')
    ap.add_argument('kinds', nargs='*')
    a = ap.parse_args()
    files = os.path.join(a.out, 'files')
    if not os.path.exists(os.path.join(files, 'index.tsv')):
        if not os.path.exists(a.rom):
            common = subprocess.run(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=REPO,
                                    capture_output=True, text=True).stdout.strip()
            a.rom = os.path.join(os.path.dirname(common), 'orig/baserom_b_us.nds')
        os.makedirs(files, exist_ok=True)
        extract(a.rom, files)
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
    if a.nmc:
        nmcr = [os.path.join(files, 'NMCR', f'{i}.bin') for i in range(16)]
        sys.exit(subprocess.run([build_nmc(a.build, a.out)] + nmcr, env=env).returncode)
    exe = build_nnsys(a.build, a.out) if a.nnsys else build(a.build, a.out)
    sys.exit(subprocess.run([exe, files] + a.kinds, env=env).returncode)


main()
