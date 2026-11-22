#!/usr/bin/env python3
"""Find ROM bytes in the repo: every tracked file (or git blob) holding a run of >= MINRUN bytes that also
appears in a game ROM (raw .nds, dsd extract, and LZ77-decompressed data inside it).

    tools/rom_leak_check.py                  tracked files, full scan (needs the ROMs; builds a cache once)
    tools/rom_leak_check.py --static         no ROM needed: tracked binaries / big numeric tables not on the allowlist
    tools/rom_leak_check.py --history        every blob reachable from origin (all remote branches and tags)
    tools/rom_leak_check.py --files a b ...  only these paths (a folder: its tracked files); a path that is not there,
                                             or a folder with no tracked file, is an error (exit 2)
    tools/rom_leak_check.py --package P ...  a built package (exe, .dll, .so, .apk/.ipa/.zip members, a folder):
                                             every run counts but the public tables tools/release_allow.tsv names
                                             by the sha1 of the run's bytes (tools/release_check.sh)

A text file is scanned five ways: its raw bytes, as UTF-16LE, and its integer literals (hex or decimal, in order)
packed as u8, u16 and u32 little endian, so a C array copied from the ROM is found whatever its element width.
Runs that are one byte or a short repeating pattern are skipped. Exit 1 on any finding not on
tools/rom_leak_allow.txt (one path per line, '#' comments). Never prints ROM bytes, only paths and offsets.

Method: 32-byte windows of every ROM source are hashed at a stride of 16 (numpy); every offset of every
candidate stream is hashed and looked up, hits are extended byte by byte and kept if the run is >= MINRUN.
Any shared run of >= 48 bytes contains one stride-16 window, so none is missed.
"""
import argparse
import bisect
import hashlib
import mmap
import pickle
import os
import re
import struct
import subprocess
import sys
import multiprocessing
Pool = multiprocessing.get_context("fork").Pool

import numpy as np

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MINRUN = 48
WIN = 32
STRIDE = 16
K = [np.uint64(x) for x in (0x9E3779B97F4A7C15, 0xC2B2AE3D27D4EB4F, 0x165667B19E3779F9, 0x27D4EB2F165667C5)]

def _main_checkout():
    """The main checkout (a worktree's orig/ and extract/ are the main one's by hardlink or not at all): git's common dir's parent."""
    try:
        out = subprocess.run(["git", "-C", REPO, "rev-parse", "--path-format=absolute", "--git-common-dir"],
                             capture_output=True, text=True, check=True).stdout.strip()
        return os.path.dirname(out)
    except Exception:
        return REPO


def _default_roms():
    """(name, ROM, dsd extract dir or None) of each game this machine has: the checkout's orig/ (the worktree's, else the main
    one's), White's in $ROMLEAK_WHITE_ROM / $ROMLEAK_WHITE_EXT (default /root/white_rom, else orig/; the checkout's extract/w_us, else /root/white_ext), Black 2, White 2 and
    Platinum (port/include_plat, port/subprojects/pokeplatinum-*.patch), HeartGold and SoulSilver (third_party/pokeheartgold)
    from orig/ (no extract: the raw ROM, its modules BLZ-decompressed and its LZ cache are searched)."""
    roots = [REPO, _main_checkout()]

    def find(*rel):
        for r in roots:
            if os.path.exists(os.path.join(r, *rel)):
                return os.path.join(r, *rel)
        return None

    out = []
    if sys.platform == "darwin":   # the Mac: no dsd extract (dsd is a Linux program): modules and LZ cache from each ROM
        for name, f in (("black", "baserom_b_us.nds"), ("white", "baserom_w_us.nds"), ("black2", "baserom_b2_us.nds"),
                        ("white2", "baserom_w2_us.nds"), ("platinum", "baserom_plat_us.nds"), ("heartgold", "baserom_hg_us.nds"),
                        ("soulsilver", "baserom_ss_us.nds")):
            if find("orig", f):
                out.append((name, find("orig", f), None))
        return out
    b = find("orig", "baserom_b_us.nds")
    if b:
        out.append(("black", b, find("extract", "b_us")))
    w = os.environ.get("ROMLEAK_WHITE_ROM", "/root/white_rom/baserom_w_us.nds")
    if not os.path.exists(w):
        w = find("orig", "baserom_w_us.nds")
    if w:
        # the White extract: $ROMLEAK_WHITE_EXT, else the checkout's extract/w_us when it exists (no env var needed), else /root/white_ext
        wx = os.environ.get("ROMLEAK_WHITE_EXT") or find("extract", "w_us") or "/root/white_ext"
        out.append(("white", w, wx))
    for name, f in (("black2", "baserom_b2_us.nds"), ("white2", "baserom_w2_us.nds"),
                    ("platinum", "baserom_plat_us.nds"), ("heartgold", "baserom_hg_us.nds"),
                    ("soulsilver", "baserom_ss_us.nds")):
        if find("orig", f):
            out.append((name, find("orig", f), None))
    return out


DEFAULT_ROMS = _default_roms()
# tracked non-text files that are ours (not game assets); anything else binary is a finding
STATIC_BINARY_OK = {"tools/ios/AppIcon.png"}
STATIC_MAX_NUMS = 400  # a text file with this many integer literals in a row, only punctuation between = a data table

_idx = None  # (sorted hashes, src id per entry, offset per entry, names, bufs) set before fork


def hashes(buf, stride=1):
    """64-bit hash of every WIN-byte window of buf (at multiples of stride): (hashes, offsets). Periodic windows skipped."""
    n = len(buf)
    if n < WIN:
        return np.zeros(0, np.uint64), np.zeros(0, np.int64)
    arr = np.frombuffer(buf, np.uint8)
    out_h, out_o = [], []
    CH = 1 << 22
    nwin = n - WIN + 1
    for s in range(0, nwin, CH):
        cnt = min(nwin, s + CH) - s
        segs = [np.ndarray(shape=(cnt,), dtype="<u8", buffer=arr, offset=s + 8 * q, strides=(1,)) for q in range(4)]
        h = segs[0] * K[0] ^ segs[1] * K[1] ^ segs[2] * K[2] ^ segs[3] * K[3]
        h ^= h >> np.uint64(29)
        sel = ~((segs[0] == segs[1]) & (segs[1] == segs[2]) & (segs[2] == segs[3]))
        if stride > 1:
            sel &= ((np.arange(cnt) + s) % stride) == 0
        o = np.nonzero(sel)[0]
        out_h.append(h[o])
        out_o.append(o + s)
    return np.concatenate(out_h), np.concatenate(out_o)


# ---------------------------------------------------------------- ROM side
def lz_decompress(d):
    """LZ77 (type 0x10 / 0x11 as in NitroSDK/BIOS). Returns bytes or None if malformed."""
    if len(d) < 8 or d[0] not in (0x10, 0x11):
        return None
    size = d[1] | d[2] << 8 | d[3] << 16
    p = 4
    if size == 0:
        size = struct.unpack_from("<I", d, 4)[0]
        p = 8
    if size < 16 or size > 1 << 26:
        return None
    out = bytearray()
    n = len(d)
    t = d[0]
    try:
        while len(out) < size:
            flags = d[p]
            p += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if flags & (0x80 >> bit):
                    b0 = d[p]
                    if t == 0x10:
                        ln = (b0 >> 4) + 3
                        disp = ((b0 & 15) << 8 | d[p + 1]) + 1
                        p += 2
                    else:
                        k = b0 >> 4
                        if k == 0:
                            ln = (b0 << 4 | d[p + 1] >> 4) + 0x11
                            disp = ((d[p + 1] & 15) << 8 | d[p + 2]) + 1
                            p += 3
                        elif k == 1:
                            ln = ((b0 & 15) << 12 | d[p + 1] << 4 | d[p + 2] >> 4) + 0x111
                            disp = ((d[p + 2] & 15) << 8 | d[p + 3]) + 1
                            p += 4
                        else:
                            ln = k + 1
                            disp = ((b0 & 15) << 8 | d[p + 1]) + 1
                            p += 2
                    if disp > len(out):
                        return None
                    for _ in range(ln):
                        out.append(out[-disp])
                else:
                    out.append(d[p])
                    p += 1
    except IndexError:
        return None
    if p > n or n - p > 8:  # the stream must be used up (padding allowed)
        return None
    return bytes(out[:size])


def narc_members(d):
    if d[:4] != b"NARC" or len(d) < 0x30:
        return []
    try:
        p = struct.unpack_from("<H", d, 12)[0]
        if d[p:p + 4] != b"BTAF":
            return []
        bsz, cnt = struct.unpack_from("<II", d, p + 4)
        ents = [struct.unpack_from("<II", d, p + 12 + 8 * i) for i in range(cnt)]
        p += bsz
        if d[p:p + 4] != b"BTNF":
            return []
        p += struct.unpack_from("<I", d, p + 4)[0]
        if d[p:p + 4] != b"GMIF":
            return []
        base = p + 8
        return [d[base + s:base + e] for s, e in ents if base + e <= len(d)]
    except struct.error:
        return []


def derive(item):
    """Walk one file: write every LZ77 decompression (recursively through NARC members) to the cache."""
    path, cache, tag = item
    out = []

    def walk(d, depth):
        if depth > 6 or len(d) < 16:
            return
        if d[:4] == b"NARC":
            for m in narc_members(d):
                walk(m, depth + 1)
            return
        u = lz_decompress(d) if len(d) <= 1 << 24 else None
        if u is not None:
            with open(os.path.join(cache, f"{tag}.{len(out)}.bin"), "wb") as f:
                f.write(u)
            out.append(1)
            walk(u, depth + 1)

    if "@" in path:   # ROM@START:END: a file of the ROM's FAT (the Mac, no extract)
        rom, span = path.rsplit("@", 1)
        a, b = (int(x) for x in span.split(":"))
        with open(rom, "rb") as f:
            f.seek(a)
            walk(f.read(b - a), 0)
        return len(out)
    with open(path, "rb") as f:
        walk(f.read(), 0)
    return len(out)


def build_cache(roms, cache):
    os.makedirs(cache, exist_ok=True)
    stamp = os.path.join(cache, "stamp")
    key = ("v1fat " if sys.platform == "darwin" else "v1 ") + " ".join(str(os.path.getsize(r)) for _, r, _ in roms)
    if os.path.exists(stamp) and open(stamp).read() == key:
        return
    for f in os.listdir(cache):
        os.remove(os.path.join(cache, f))
    items = []
    for name, _, ext in roms:
        k = 0
        for root, _, fs in os.walk(os.path.join(ext, "files")) if ext else ():
            for fn in sorted(fs):
                items.append((os.path.join(root, fn), cache, f"{name}{k}"))
                k += 1
    for name, rom, ext in roms:
        if ext or sys.platform != "darwin":
            continue
        # the Mac (no dsd extract): the ROM's own files from its FAT, as the extract's files/ would give them
        with open(rom, "rb") as f:
            head = f.read(0x50)
            fat, fat_size = struct.unpack_from("<II", head, 0x48)
            f.seek(fat)
            table = f.read(fat_size)
        for k in range(fat_size // 8):
            a, b = struct.unpack_from("<II", table, 8 * k)
            if b > a:
                items.append((f"{rom}@{a}:{b}", cache, f"{name}{k}"))
    with Pool(int(os.environ.get("BW_JOBS", "8"))) as p:
        for _ in p.imap_unordered(derive, items, chunksize=8):
            pass
    if sys.platform == "darwin":
        # macOS lets a process hold 10240 files (kern.maxfilesperproc) and each mapped file keeps one: the ~48000
        # decompressed files go into packs of 64 MB, each file followed by a separator no binary holds
        names = sorted(f for f in os.listdir(cache) if f.endswith(".bin"))
        pack, k = None, 0
        for fn in names:
            if pack is None or pack.tell() >= 64 << 20:
                if pack:
                    pack.close()
                pack = open(os.path.join(cache, f"pack{k:03d}.pk"), "wb")
                k += 1
            with open(os.path.join(cache, fn), "rb") as f:
                pack.write(f.read() + b"\xa5\x5a" * 32)
            os.remove(os.path.join(cache, fn))
        if pack:
            pack.close()
        for f in os.listdir(cache):
            if f.endswith(".pk"):
                os.rename(os.path.join(cache, f), os.path.join(cache, f[:-3] + ".bin"))
    open(stamp, "w").write(key)


def _map(path):
    """The file as a read-only mapping (page cache, shared by the pool's workers and reclaimable) instead of a private copy."""
    with open(path, "rb") as f:
        return mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) if os.fstat(f.fileno()).st_size else b""


def rom_sources(roms, cache):
    """(name, bytes): raw ROMs, the dsd extract outside files/ (decompressed arm9, overlays...) or without one the
    ROM's own modules (tools/rom_modules.py), the LZ cache."""
    for name, rom, ext in roms:
        data = _map(rom)
        yield name + ":rom", data
        if not ext:  # no dsd extract (Black 2, White 2, Platinum): main and the overlays BLZ-decompressed from the ROM itself
            import rom_modules
            yield name + ":arm9", rom_modules.arm9(data)
            for ov, image in rom_modules.overlays(data):
                yield f"{name}:ov{ov:03d}", image
        for root, _, fs in (os.walk(ext) if ext else []):
            if os.path.relpath(root, ext).split(os.sep)[0] == "files":
                continue
            for fn in sorted(fs):
                p = os.path.join(root, fn)
                if os.path.getsize(p) >= WIN:
                    yield name + ":" + os.path.relpath(p, ext), _map(p)
    for fn in sorted(os.listdir(cache)):
        if fn.endswith(".bin"):
            yield "cache:" + fn, _map(os.path.join(cache, fn))


def build_index(roms, cache):
    """Sorted window hashes of every ROM source (cached next to the LZ cache: the hashing takes minutes)."""
    global _idx
    import resource
    lim = resource.getrlimit(resource.RLIMIT_NOFILE)  # every mapped file keeps a descriptor
    resource.setrlimit(resource.RLIMIT_NOFILE, (min(lim[1], 1 << 18) if lim[1] != resource.RLIM_INFINITY else 1 << 18, lim[1]))
    srcs = list(rom_sources(roms, cache))
    names, bufs = [n for n, _ in srcs], [b for _, b in srcs]
    key = " ".join(f"{n}:{len(b)}" for n, b in srcs)
    npz = os.path.join(cache, "index.npz")
    if os.path.exists(npz):
        z = np.load(npz)
        if str(z["key"]) == key:
            _idx = (z["h"], z["s"], z["o"], names, bufs)
            return
    hs, so, oo = [], [], []
    for i, b in enumerate(bufs):
        h, o = hashes(b, STRIDE)
        hs.append(h)
        so.append(np.full(len(h), i, np.int32))
        oo.append(o)
    h, s, o = np.concatenate(hs), np.concatenate(so), np.concatenate(oo)
    order = np.argsort(h, kind="stable")
    h, s, o = h[order], s[order], o[order]
    keep = np.ones(len(h), bool)
    keep[1:] = h[1:] != h[:-1]
    h, s, o = h[keep], s[keep], o[keep]
    np.savez(npz, h=h, s=s, o=o, key=key)
    _idx = (h, s, o, names, bufs)
    print(f"index: {len(names)} sources, {len(h)} windows", file=sys.stderr)


# ---------------------------------------------------------------- candidate streams
NUM_RE = re.compile(rb"(?<![\w.])(-?0[xX][0-9a-fA-F]+|-?\d+)(?![\w.])")
STR_RE = re.compile(rb'"(?:[^"\\\n]|\\.)*"')   # a C string literal: its digits are text, not numbers
STR_BREAK = -0x5B   # stands for a string's digits in the number streams (0xa5, 0xffa5, 0xffffffa5: rare in a ROM)


GAP_RE = re.compile(rb"[ \t,\r\n\\]*")


def numbers(data):
    """[(byte position, value)] of the integer literals of a text."""
    out = []
    strings = [m.span() for m in STR_RE.finditer(data)]
    k = 0
    for m in NUM_RE.finditer(data):
        while k < len(strings) and strings[k][1] <= m.start():
            k += 1
        if k < len(strings) and strings[k][0] <= m.start():
            # the digits of a string are text, but the numbers on either side are no run: one break stands for them
            if not out or out[-1][1] != STR_BREAK:
                out.append((m.start(), STR_BREAK))
            continue
        try:
            out.append((m.start(), int(m[0], 0)))
        except ValueError:
            out.append((m.start(), int(m[0], 10)))
    return out


def streams(data, deep=True):
    yield "raw", data
    if not deep or b"\0" in data[:4096]:
        return
    try:
        t = data.decode("utf-8")
    except UnicodeDecodeError:
        return
    if not t.isascii():
        t = t  # keep
    yield "utf16", t.encode("utf-16-le")
    vals = [v for _, v in numbers(data)]
    if len(vals) >= 12:
        for w, mask in ((1, 0xFF), (2, 0xFFFF), (4, 0xFFFFFFFF)):
            fit = [v & mask for v in vals if -(mask >> 1) - 1 <= v <= mask]
            if len(fit) * w >= MINRUN:
                yield f"u{w * 8}", np.array(fit, dtype="<u%d" % w).tobytes()


def run_len(a, ai, b, bi):
    """Common run through a[ai] == b[bi]: (start offset in a, length)."""
    l = 0
    while ai - l > 0 and bi - l > 0 and a[ai - l - 1] == b[bi - l - 1]:
        l += 1
    r = 0
    na, nb = len(a), len(b)
    while ai + r < na and bi + r < nb:
        step = min(4096, na - ai - r, nb - bi - r)
        x = np.frombuffer(a, np.uint8, step, ai + r)
        y = np.frombuffer(b, np.uint8, step, bi + r)
        ne = np.nonzero(x != y)[0]
        if len(ne):
            r += int(ne[0])
            break
        r += step
    return ai - l, l + r


def trivial(run, w):
    """A run that repeats with a short period (padding, fill), or is mostly one step apart (enum values, counters; in
    raw bytes a ramp of 8-, 16- or 32-bit elements), or holds fewer than 8 nonzero bytes (zeros around a pointer: the macOS
    image's filled tables next to a chained fixup matched a ROM file's padding, 2026-11-12)."""
    x = np.frombuffer(run, np.uint8)
    if np.count_nonzero(x) < 8:
        return True
    for per in range(1, 17):
        if (x[per:] == x[:-per]).mean() >= 0.95:
            return True
    if w:
        a = np.frombuffer(run[:len(run) // w * w], "<u%d" % w)
        df = np.diff(a.astype(np.int64))
        return np.unique(df, return_counts=True)[1].max() >= 0.7 * len(df)
    # raw bytes (--package): a ramp of 8-, 16- or 32-bit elements, 80 % of them one same nonzero step apart (code point and
    # identity tables, a compiler's jump table of equal-sized cases): start and step say it all, nobody's data
    for k in (1, 2, 4):
        for ph in range(k):
            m = (len(run) - ph) // k
            if m < 8:
                continue
            df = np.diff(np.frombuffer(run[ph:ph + m * k], "<u%d" % k).astype(np.int64))
            vals, cnt = np.unique(df, return_counts=True)
            i = int(cnt.argmax())
            if vals[i] != 0 and cnt[i] >= 0.8 * len(df):
                return True
    return False


def scan_stream(data, w=0, cap=200):
    """List of (offset in stream, run length, rom source name, rom offset) with run >= MINRUN."""
    h, s, o, names, bufs = _idx
    wh, wo = hashes(data, 1)
    if not len(wh):
        return []
    pos = np.searchsorted(h, wh)
    pos[pos >= len(h)] = len(h) - 1
    hit = np.nonzero(h[pos] == wh)[0]
    res = []
    covered = -1
    for j in hit:
        off = int(wo[j])
        if off < covered:
            continue
        k = pos[j]
        src = bufs[s[k]]
        st, ln = run_len(data, off, src, int(o[k]))
        covered = st + ln
        if ln >= MINRUN and not trivial(data[st:st + ln], w):
            res.append((st, ln, names[s[k]], int(o[k]) - (off - st)))
        if len(res) >= cap:
            break
    return res


INIT_RE = re.compile(r"(\w+)\s*(?:\[[^\]]*\]\s*)*(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{")


def initializers(text):
    """[(start, end, name)] of the brace initialisers of a C text (`name[..] = { ... }`, outermost only)."""
    out, i = [], 0
    while True:
        m = INIT_RE.search(text, i)
        if not m:
            return out
        depth, j = 1, m.end()
        while depth and j < len(text):
            depth += (text[j] == "{") - (text[j] == "}")
            j += 1
        out.append((m.start(), j, m[1]))
        i = j


GAME_CFG = {"black": "b_us", "white": "w_us", "black2": "b2_us", "white2": "w2_us", "platinum": "plat_us"}
SECTION_RE = re.compile(r"^\s+\.\w+\s+start:(0x[0-9a-fA-F]+)\s+end:(0x[0-9a-fA-F]+)\s+kind:(\w+)", re.M)
_cfg_cache, _hdr_cache = {}, {}


def _cfg_ranges(game):
    """{module: [(start, end, what)]} of what config/<ver>/ says about a game's address ranges: the named tables of
    assets.tsv (and Platinum's symbols.tsv), then a dsd module's delinks.txt sections (code / rodata / data / bss)."""
    if game in _cfg_cache:
        return _cfg_cache[game]
    out = {}
    d = os.path.join(REPO, "config", GAME_CFG.get(game, "-"))
    for fn, cols in (("assets.tsv", (0, 1, 2, 3)), ("symbols.tsv", (1, 2, 3, 4))):
        try:
            rows = open(os.path.join(d, fn)).read().splitlines()[1:]
        except OSError:
            continue
        for row in rows:
            c = row.split("\t")
            try:
                out.setdefault(c[cols[1]], []).append((int(c[cols[2]], 16), int(c[cols[3]], 16), "data " + c[cols[0]]))
            except (IndexError, ValueError):
                pass
    ovs = os.path.join(d, "arm9", "overlays")
    mods = [("main", os.path.join(d, "arm9", "delinks.txt"))]
    mods += [(o, os.path.join(ovs, o, "delinks.txt")) for o in (sorted(os.listdir(ovs)) if os.path.isdir(ovs) else [])]
    for mod, p in mods:
        try:
            text = open(p).read()
        except OSError:
            continue
        for a, b, kind in SECTION_RE.findall(text):
            out.setdefault(mod, []).append((int(a, 16), int(b, 16), kind))
    _cfg_cache[game] = out
    return out


def _rom_header(game):
    """(ROM bytes, {module: RAM base}, [(file start, end, module or None, RAM base, compressed)] sorted: the raw ROM's
    ARM9 and its overlays, then every other file of its FAT)."""
    if game in _hdr_cache:
        return _hdr_cache[game]
    names, bufs = _idx[3], _idx[4]
    rom = bufs[names.index(game + ":rom")]
    a9_off, _, a9_load, a9_size = struct.unpack_from("<4I", rom, 0x20)
    fat, fat_size, ovt, ovt_size = struct.unpack_from("<4I", rom, 0x48)
    bases = {"main": a9_load}
    params = rom.find(struct.pack("<II", 0xDEC00621, 0x2106C0DE), a9_off, a9_off + a9_size) - 0x1C
    packed = params >= a9_off and struct.unpack_from("<I", rom, params + 0x14)[0] != 0   # BLZ-compressed ARM9
    files = [(a9_off, a9_off + a9_size, "main", a9_load, packed)]
    ov_files = set()
    for e in range(ovt, ovt + ovt_size, 32):
        ov_id, ram = struct.unpack_from("<2I", rom, e)
        fid, word = struct.unpack_from("<2I", rom, e + 24)
        start, stop = struct.unpack_from("<2I", rom, fat + 8 * fid)
        bases[f"ov{ov_id:03d}"] = ram
        files.append((start, stop, f"ov{ov_id:03d}", ram, bool(word >> 24 & 1)))
        ov_files.add(fid)
    for fid in range(fat_size // 8):
        if fid not in ov_files:
            start, stop = struct.unpack_from("<2I", rom, fat + 8 * fid)
            files.append((start, stop, None, fid, False))
    files.sort(key=lambda f: f[:2])
    _hdr_cache[game] = (rom, bases, files)
    return _hdr_cache[game]


def place(name, off):
    """Where a ROM source's offset is: `<game> <module>+0x<offset in the module image> @0x<address> <what>`, `what` being
    code / rodata / data from the module's delinks.txt, `data <table>` from assets.tsv / symbols.tsv, else `unlisted` (a
    place the config knows nothing about: code, or a table nobody moved yet). A raw-ROM offset is mapped to the module or
    FAT file holding it (a compressed module image has no address: its bytes are not the module's)."""
    game, _, src = name.partition(":")
    generic = f"{name}+0x{off:x}"
    m = re.fullmatch(r"ov(\d+)", src) or re.search(r"arm9_overlays/ov(\d+)\.bin$", src)
    try:
        if src == "arm9" or src.endswith("arm9/arm9.bin"):
            mod, mo = "main", off
        elif m:
            mod, mo = f"ov{int(m[1]):03d}", off
        elif src == "rom":
            _, _, files = _rom_header(game)
            i = bisect.bisect_right(files, off, key=lambda f: f[0]) - 1
            while i >= 0 and not files[i][0] <= off < files[i][1]:
                i -= 1
            if i < 0:
                return f"{game} rom+0x{off:x} (outside the files)"
            start, _, mod, base, comp = files[i]
            if mod is None:
                return f"{game} file {base}+0x{off - start:x} data"
            if comp:
                return f"{game} {mod}+0x{off - start:x} (raw ROM: compressed image)"
            mo = off - start
        else:
            return generic
        base = _rom_header(game)[1].get(mod)
    except (KeyError, ValueError, struct.error, IndexError):
        return generic
    if base is None:
        return f"{game} {mod}+0x{mo:x}"
    addr = base + mo
    what = next((w for a, b, w in _cfg_ranges(game).get(mod, ()) if a <= addr < b), "unlisted")
    return f"{game} {mod}+0x{mo:x} @0x{addr:08x} {what}"


def where_data(data, deep=True):
    """What scan_data found, with the places: [(kind, [(first line, last line, bytes, [table names], where in the ROM)])], one entry per
    stream with runs. A run of a number stream maps back to the literals it came from (the stream is the file's
    numbers, concatenated: it can span several tables and code constants), a run of raw bytes to its offsets."""
    text = data.decode("latin-1")
    inits = initializers(text)
    nl = [m.start() for m in re.finditer("\n", text)]

    def line(pos):
        return bisect.bisect_left(nl, pos) + 1

    def tables(a, b):
        return [n for s, e, n in inits if s <= b and a < e]

    nums = numbers(data) if not data[:4096].count(b"\0") else []
    out = []
    for kind, d in streams(data):
        w = {"u8": 1, "u16": 2, "u32": 4}.get(kind, 0)
        runs = scan_stream(d, w)
        if not runs:
            continue
        if w:
            mask = (1 << 8 * w) - 1
            at = [p for p, v in nums if -(mask >> 1) - 1 <= v <= mask]   # the stream's elements, in the file
        res = []
        for st, ln, src, ro in sorted(runs):
            if w:
                a, b = at[st // w], at[(st + ln - 1) // w]
            elif kind == "utf16":
                a, b = st // 2, (st + ln - 1) // 2
            else:
                a, b = st, st + ln - 1
            res.append((line(a), line(b), ln, tables(a, b), place(src, ro)))
        out.append((kind, res))
    return out


def where_path(p):
    try:
        with open(os.path.join(REPO, p), "rb") as f:
            return p, where_data(f.read())
    except OSError:
        return p, []


def where_files(paths, roms, cache, jobs):
    """{path: where_data} of the files (their runs are found again, with the lines)."""
    build_index(roms, cache)
    with Pool(jobs) as pool:
        return dict(pool.imap_unordered(where_path, paths))


def where_lines(p, found):
    """One line per finding of a file: `path [kind] bytes: L<first>-<last> <tables> <bytes> B <- <where in the ROM>, ...`
    (see place(): the ROM, module and offset the run matches, and whether that is code, a listed table or unlisted)."""
    out = []
    for kind, runs in found:
        parts = [f"L{a}" + (f"-{b}" if b != a else "") + (" " + ",".join(dict.fromkeys(t)) if t else " (code)") + f" {n}B <- {pl}"
                 for a, b, n, t, pl in runs[:8]]
        more = f", +{len(runs) - 8} more" if len(runs) > 8 else ""
        out.append(f"WHERE {p} [{kind}] {sum(r[2] for r in runs)}B: " + ", ".join(parts) + more)
    return out


def scan_data(label, data, deep=True):
    out = []
    for kind, d in streams(data, deep):
        r = scan_stream(d, {"u8": 1, "u16": 2, "u32": 4}.get(kind, 0))
        if r:
            out.append((label, kind, len(r), sum(x[1] for x in r), max(x[1] for x in r), r[0][2], r[0][0]))
    return out


def scan_path(p):
    try:
        with open(os.path.join(REPO, p), "rb") as f:
            data = f.read()
    except OSError:
        return []
    return scan_data(p, data)


def scan_oids(chunk):
    """chunk: list of (oid, size). One cat-file --batch per call."""
    out = []
    pr = subprocess.Popen(["git", "-C", REPO, "cat-file", "--batch"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    for oid, size, path in chunk:
        pr.stdin.write(oid.encode() + b"\n")
        pr.stdin.flush()
        hd = pr.stdout.readline().split()
        n = int(hd[2])
        data = pr.stdout.read(n)
        pr.stdout.read(1)
        # the big docs and tables of the history (a few MB each, thousands of versions) only as raw bytes
        out += scan_data(oid, data, size <= 400000 or path.endswith((".c", ".h", ".inc", ".py", ".json", ".m", ".mm")))
    pr.stdin.close()
    pr.wait()
    return out


# ---------------------------------------------------------------- packages (tools/release_check.sh)
PKG_CHUNK = 4 << 20
PKG_OVERLAP = 64 << 10
_pkg = []  # (label, bytes) set before fork


def package_items(paths):
    """(label, bytes) of every file of the packages: a folder's files, a zip's (apk, ipa, aab) members, plain files."""
    import zipfile
    out = []
    for p in paths:
        files = [p]
        if os.path.isdir(p):
            files = sorted(os.path.join(r, f) for r, _, fs in os.walk(p) for f in fs)
        for f in files:
            if os.path.islink(f) or not os.path.isfile(f):
                continue
            with open(f, "rb") as fh:
                d = fh.read()
            if d[:4] == b"PK\x03\x04":
                with zipfile.ZipFile(f) as z:
                    for i in z.infolist():
                        if not i.is_dir():
                            out.append((f"{f}!{i.filename}", z.read(i)))
            else:
                out.append((f, d))
    return out


def scan_pkg_chunk(task):
    """Runs of one chunk of a package file: (label, offset, length, rom source, rom offset); a run counts where it starts."""
    i, start = task
    label, data = _pkg[i]
    end = min(len(data), start + PKG_CHUNK)
    view = data[start:min(len(data), end + PKG_OVERLAP)]
    res = []
    for kind, d in streams(view, deep=False):
        for st, ln, src, so in scan_stream(d, 0, cap=1 << 30):
            if st < end - start:
                res.append((label, start + st, ln, src, so))
    return res


PUBLIC_TABLES = os.path.join(REPO, "tools/release_allow.tsv")


def load_public():
    """From tools/release_allow.tsv: ({sha1 of a run's bytes: what it is}, {sha256 of a whole file: what it is}). Runs that
    are public tables or chance matches in code that is not the game's (standard tables, third-party data), and
    third-party files as their upstream ships them (`file` lines: every run in such a file is that library's), each
    named; never game data."""
    runs, files = {}, {}
    if os.path.exists(PUBLIC_TABLES):
        for line in open(PUBLIC_TABLES):
            if line.strip() and not line.startswith("#"):
                f = line.rstrip("\n").split("\t")
                if f[0] == "file":
                    files[f[1]] = f[2] + ": " + f[3]
                else:
                    runs[f[0]] = f[2]
    return runs, files


# the launcher's table (port/src/launcher_core.c sGames): a row's game code and the sha1 of its ROM, which a build holds only
# for the games it plays (White's, Black 2's, ... rows have no sha1 without their C)
GAME_ROM = {"IRBO": "black", "IRAO": "white", "IREO": "black2", "IRDO": "white2", "CPUE": "platinum", "IPKE": "heartgold",
            "IPGE": "soulsilver"}


def built_games(paths):
    """The ROM names (DEFAULT_ROMS') of the games the packages play: the launcher table's ROM sha1s found in their files."""
    text = open(os.path.join(REPO, "port/src/launcher_core.c"), encoding="utf-8").read()
    sha = {code: s for code, s in re.findall(r'\{"(\w{4})", "[^"]*", "[^"]*", "[^"]*", \{"([0-9a-f]{40})"', text)}
    missing = sorted(set(GAME_ROM) - set(sha))
    if missing:
        sys.exit(f"rom_leak_check: port/src/launcher_core.c's table has no ROM sha1 for {' '.join(missing)}")
    found = set()
    for _, d in package_items(paths):
        for code, s in sha.items():
            if code not in found and s.encode() + b"\0" in d:   # (a C string: not a README naming the dumps)
                found.add(code)
    return sorted(GAME_ROM[c] for c in found)


def package_check(paths, detail=None):
    """Exit status 1 if any file of the packages holds a run of ROM bytes that is not a listed public table (a run's
    sha1 in tools/release_allow.tsv: the standard tables and chance matches of third-party code, each named there)."""
    import hashlib
    global _pkg
    _pkg = [(l, d) for l, d in package_items(paths) if len(d) >= MINRUN]
    data = dict(_pkg)
    tasks = [(i, s) for i, (_, d) in enumerate(_pkg) for s in range(0, len(d), PKG_CHUNK)]
    runs = []
    with Pool(int(os.environ.get("BW_JOBS", "8"))) as pool:
        for r in pool.imap_unordered(scan_pkg_chunk, tasks):
            runs += r
    runs.sort()
    public, public_files = load_public()
    file_what = {}
    for label, d in _pkg:
        file_what[label] = public_files.get(hashlib.sha256(d).hexdigest(), "")
    total, known = {}, {}
    rows = []
    for label, off, ln, src, so in runs:
        h = hashlib.sha1(data[label][off:off + ln]).hexdigest()
        what = public.get(h, "") or file_what[label]
        rows.append((label, off, ln, src, so, h, what))
        t = known if what else total
        n, b, longest = t.get(label, (0, 0, 0))
        t[label] = (n + 1, b + ln, max(longest, ln))
    if detail:
        with open(detail, "w") as f:
            for r in rows:
                f.write("\t".join(str(x) for x in r) + "\n")
    for label, (n, b, longest) in sorted(total.items()):
        print(f"ROM BYTES {label}  runs={n} bytes={b} longest={longest}")
    for label, (n, b, longest) in sorted(known.items()):
        print(f"public tables {label}  runs={n} bytes={b} (tools/release_allow.tsv)")
    nbad = sum(t[0] for t in total.values())
    print(f"rom_leak_check: {len(_pkg)} package files, {sum(len(d) for _, d in _pkg)} bytes scanned, "
          f"{sum(t[1] for t in total.values())} ROM bytes in {nbad} runs"
          f" (and {sum(t[1] for t in known.values())} bytes in {sum(t[0] for t in known.values())} runs of listed"
          " public tables)")
    return 1 if nbad else 0


# ---------------------------------------------------------------- static checks (no ROM)
def static_check(paths):
    res = []
    for p in paths:
        try:
            with open(os.path.join(REPO, p), "rb") as f:
                d = f.read()
        except OSError:
            continue
        isbin = b"\0" in d[:8192]
        if not isbin:
            try:
                d.decode("utf-8")
            except UnicodeDecodeError:
                isbin = True
        if isbin:
            if p not in STATIC_BINARY_OK:
                res.append((p, "binary", 1, len(d), len(d), "-", 0))
            continue
        n, best, prev_end, start, nz = 0, (0, 0), -1, 0, False
        for m in NUM_RE.finditer(d):
            if prev_end >= 0 and GAP_RE.fullmatch(d, prev_end, m.start()):
                n += 1
            else:
                n, start, nz = 1, m.start(), False
            prev_end = m.end()
            nz = nz or bool(m[0].lstrip(b"-0xX"))   # (a run of zeros only holds nothing: a blanked table, filled at start)
            if n > best[0] and nz:
                best = (n, start)
        if best[0] >= STATIC_MAX_NUMS:
            res.append((p, "numtable", 1, best[0], best[0], "-", best[1]))
    return res


def _digest(data):
    return hashlib.blake2b(data, digest_size=16).digest()


def _hash_path(p):
    try:
        with open(os.path.join(REPO, p), "rb") as f:
            return p, _digest(f.read())
    except OSError:
        return p, None


def code_version():
    """Identity of what decides a file's findings: this checker and the module decompressor, their thresholds."""
    h = hashlib.blake2b(digest_size=16)
    for fn in ("rom_leak_check.py", "rom_modules.py"):
        with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), fn), "rb") as f:
            h.update(f.read())
    h.update(repr((MINRUN, WIN, STRIDE, STATIC_MAX_NUMS)).encode())
    return h.hexdigest()


def index_version(roms, cache):
    """The ROM index's identity without loading it: its file and the ROMs it was built from (size, mtime)."""
    parts = []
    for f in [os.path.join(cache, "index.npz")] + [r[1] for r in roms]:
        try:
            st = os.stat(f)
            parts.append((os.path.basename(f), st.st_size, st.st_mtime_ns))
        except OSError:
            parts.append((f, None, None))
    return repr(parts)


def scan_cached(paths, roms, cache, jobs, use_cache=True, full=True):
    """scan_path over paths; a file whose content, the checker and the ROM index are unchanged reuses its last result
    (cache/scan_results.pkl, written as a new file; safe to delete)."""
    fn = os.path.join(cache, "scan_results.pkl")
    build_cache(roms, cache)
    ver = (code_version(), index_version(roms, cache))
    old = {}
    if use_cache:
        try:
            with open(fn, "rb") as f:
                z = pickle.load(f)
            if z.get("ver") == ver:
                old = z["files"]
        except Exception:
            pass
    with Pool(jobs) as pool:
        hs = dict(pool.imap_unordered(_hash_path, paths, chunksize=64))
    todo = [p for p in paths if hs[p] is None or old.get(p, (None,))[0] != hs[p]]
    skip = set(todo)
    new = {p: old[p] for p in paths if p not in skip}
    if todo:
        build_index(roms, cache)
        ver = (ver[0], index_version(roms, cache))  # a first run (re)builds the index file
        with Pool(jobs) as pool:
            for p, r in zip(todo, pool.imap(scan_path, todo, chunksize=16)):
                new[p] = (hs[p], r)
    print(f"scan cache: {len(paths) - len(todo)} reused, {len(todo)} scanned", file=sys.stderr)
    if not full:
        new = {**old, **new}  # a partial run keeps the other files
    if todo or len(new) != len(old):
        tmp = f"{fn}.{os.getpid()}.tmp"
        try:
            with open(tmp, "wb") as f:
                pickle.dump({"ver": ver, "files": {p: v for p, v in new.items() if v[0] is not None}}, f)
            os.replace(tmp, fn)
        except OSError:
            pass
    res = []
    for p in paths:
        res += new[p][1] if p in new else []
    return res


def load_allow():
    """path -> bytes already accepted (tools/rom_leak_allow.txt: 'path limit', '#' comments). A file may not grow past it."""
    fn = os.path.join(REPO, "tools", "rom_leak_allow.txt")
    out = {}
    if os.path.exists(fn):
        for l in open(fn):
            f = l.split("#")[0].split()
            if f:
                out[f[0]] = int(f[1]) if len(f) > 1 else 1 << 62
    return out


def write_allow(res, static):
    lim = load_allow() if static else {}  # a static run only adds the paths a ROM scan doesn't know
    for r in res:
        if static and r[0] in lim:
            continue
        lim[r[0]] = max(lim.get(r[0], 0), r[3])
    fn = os.path.join(REPO, "tools", "rom_leak_allow.txt")
    why = {}  # the reason written after '#' on a line stays with its path
    for l in open(fn) if os.path.exists(fn) else []:
        if not l.startswith("#") and "#" in l:
            why[l.split()[0]] = l.split("#", 1)[1].strip()
    with open(fn, "w") as f:
        f.write("# Files the checker accepts, with the most ROM-matching bytes each may hold (tools/rom_leak_check.py).\n"
                "# A file over its limit, or not listed, fails the check. Only ever shrink this list: move the data out\n"
                "# (tools/asset.py --move) and delete the line. --write-allow regenerates it from a full scan.\n"
                "# Kinds: lib/{nitro,msl,twl,dwc,dwcutil,nitrowifi,...} = Nintendo SDK / third-party tables and strings\n"
                "# (sine table, MD5, JPEG, ADPCM): the same in every game, not the game's assets. port/ = our own copies\n"
                "# of the same algorithm tables. src/ and lib/gfl = small numeric parameter tables of the game's code\n"
                "# that asset.py --move can't take yet (struct arrays, names mixed in).\n")
        for p, n in sorted(lim.items()):
            f.write(f"{p} {n}" + (f"  # {why[p]}" if p in why else "") + "\n")


def tracked():
    return subprocess.check_output(["git", "-C", REPO, "ls-files", "-z"]).decode().split("\0")[:-1]


def file_args(args):
    """--files' paths: a file as it is, a folder as its tracked files; (paths, the arguments that name nothing)."""
    files, bad, tr = [], [], None
    for f in args:
        f = os.path.normpath(f.removeprefix("./")) if f not in (".", "./") else ""
        if os.path.isfile(os.path.join(REPO, f)):
            files.append(f)
            continue
        tr = tr if tr is not None else tracked()
        under = [t for t in tr if not f or t.startswith(f + "/")] if os.path.isdir(os.path.join(REPO, f)) else []
        files += under
        bad += [] if under else [f or "."]
    return list(dict.fromkeys(files)), bad


def history_blobs():
    """(oid, size, path) of every blob reachable from the remotes (all branches) and tags."""
    revs = subprocess.check_output(["git", "-C", REPO, "rev-list", "--objects", "--remotes", "--tags"]).decode(errors="replace").splitlines()
    names = {}
    for l in revs:
        oid, _, path = l.partition(" ")
        names.setdefault(oid, path)
    pr = subprocess.run(["git", "-C", REPO, "cat-file", "--batch-check=%(objectname) %(objecttype) %(objectsize)"],
                        input="\n".join(names).encode(), capture_output=True)
    out = []
    for l in pr.stdout.decode().splitlines():
        oid, t, sz = l.split()
        if t == "blob":
            out.append((oid, int(sz), names[oid]))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--static", action="store_true")
    ap.add_argument("--history", action="store_true")
    ap.add_argument("--files", nargs="*")
    ap.add_argument("--cache", default=os.environ.get("ROMLEAK_CACHE", os.path.join(_main_checkout(), "orig", "leak_cache")))
    ap.add_argument("--no-cache", action="store_true", help="rescan every file (default: reuse the last result of unchanged files)")
    ap.add_argument("--write-allow", action="store_true", help="accept the current findings (tools/rom_leak_allow.txt)")
    ap.add_argument("--where", action="store_true", help="for each finding, the source lines (and the table names) the "
                    "matched runs came from")
    ap.add_argument("--out", help="write the full finding list (tsv) here")
    ap.add_argument("--package", nargs="+", help="built packages: exe, libraries, apk/ipa/zip, folders (only the public "
                    "tables of tools/release_allow.tsv pass)")
    ap.add_argument("--detail", help="--package: every run (file, offset, length, ROM source, ROM offset, sha1 of its "
                    "bytes, what it is if listed) to this tsv")
    a = ap.parse_args()
    if a.package:
        roms = [r for r in DEFAULT_ROMS if os.path.exists(r[1]) and (r[2] is None or os.path.isdir(r[2]))]
        if not roms:
            print("rom_leak_check: --package needs a ROM and its extract (DEFAULT_ROMS)", file=sys.stderr)
            return 2
        # a game the package plays whose ROM is not here: its bytes can't be found, so no leak of them can be ruled out
        lack = [g for g in built_games(a.package) if g not in {r[0] for r in roms}]
        if lack:
            print(f"rom_leak_check: --package: the package plays {' '.join(lack)}, whose ROM (and extract) is not here "
                  "(DEFAULT_ROMS): it can't be checked for that game's bytes", file=sys.stderr)
            return 2
        build_cache(roms, a.cache)
        build_index(roms, a.cache)
        return package_check(a.package, a.detail)
    allow = load_allow()
    paths = tracked()
    if a.files is not None:
        paths, missing = file_args(a.files)
        if missing or not paths:
            print(f"rom_leak_check: --files: {' '.join(missing) or '(none given)'}: no such file, or a folder with no "
                  f"tracked file (paths are from the repo's top, e.g. third_party/pokebw2/src/...)", file=sys.stderr)
            return 2
    jobs = int(os.environ.get("BW_JOBS", "8"))
    roms = [r for r in DEFAULT_ROMS if os.path.exists(r[1]) and (r[2] is None or os.path.isdir(r[2]))]
    meta = {}
    if a.static or not roms:
        if not a.static:
            print("rom_leak_check: no ROM here, only the static check ran", file=sys.stderr)
        res = static_check(paths)
    else:
        res = []
        if not a.history:
            res = scan_cached(paths, roms, a.cache, jobs, not a.no_cache, not a.files)
        else:
            build_cache(roms, a.cache)
            build_index(roms, a.cache)
            with Pool(jobs) as pool:
                blobs = [b for b in history_blobs() if b[1] >= MINRUN]
                print(f"history: {len(blobs)} blobs", file=sys.stderr)
                meta = {b[0]: b for b in blobs}
                blobs.sort(key=lambda b: -b[1])
                n = jobs * 8
                chunks = [[(b[0], b[1], b[2]) for b in blobs[i::n]] for i in range(n)]
                for r in pool.imap_unordered(scan_oids, chunks):
                    for x in r:
                        res.append((f"{x[0]}\t{meta[x[0]][2]}\t{meta[x[0]][1]}",) + x[1:])
    res.sort(key=lambda x: -x[3])
    if a.write_allow:
        write_allow(res, a.static)
        allow = load_allow()
    # a static run counts numbers, not bytes: a listed path passes whatever its limit
    bad = [r for r in res if (r[0] not in allow if a.static or not roms else r[3] > allow.get(r[0], -1))]
    if a.out:
        with open(a.out, "w") as f:
            for r in res:
                f.write("\t".join(str(x) for x in r) + "\n")
    for r in bad[:60]:
        print(f"LEAK {r[0]}  [{r[1]}] runs={r[2]} bytes={r[3]} longest={r[4]} in {r[5]}")
    if len(bad) > 60:
        print(f"... and {len(bad) - 60} more (use --out)")
    if a.where and bad and roms and not a.static:
        for p, found in sorted(where_files(sorted({r[0] for r in bad}), roms, a.cache, jobs).items()):
            for l in where_lines(p, found):
                print(l)
    print(f"rom_leak_check: {len(bad)} finding(s)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
