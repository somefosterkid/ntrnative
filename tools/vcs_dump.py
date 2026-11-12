#!/usr/bin/env python3
"""vcs_dump.py: read a frame-state capture (.vcs, made by the PC port with BWPORT_VCAPTURE; the format is
/root/ref/VCS_FORMAT.md: libntr-vcapture.patch's sim_vcapture.cpp writes it).

  vcs_dump.py FILE.vcs                  chunks and sizes, the INFO text, the video registers in short, the VRAM banks
                                        (VRAMCNT, where each is mapped, bytes in use), the 3D command count, the tags
  vcs_dump.py FILE.vcs --png OUT.png    the LSHT chunk (libntr's picture, 256x384, top screen first) as a PNG
               --l3d OUT.png            the L3D chunk (libntr's 3D layer, 256x192) as a PNG (RGBA)
               --bg A0 OUT.png          a text BG (engine A or B, BG 0-3) drawn from the VRAM/PLTT/IO chunks alone:
                                        a check that the banks are where the registers say (compare with --png)
               --cmds [N]               the 3D commands, one per line (the first N)
               --tags                   only the feature tags, one line (as /root/ref/corpus/index.json has them)
               --json                   a summary as JSON
Several files: one block each (--tags: one line each, "name: tags").

Pure Python (zlib; no PIL or numpy). As a module: Vcs(path) reads a file; tags(vcs) works out its tags.
"""
import argparse
import json
import os
import struct
import sys
import zlib

MAGIC = b'VCST'
FLAG_PROLOGUE, FLAG_LIST, FLAG_REG = 1, 2, 4

# the banks in the VRAM chunk (LCDC order: the chunk is 0x06800000-0x068A3FFF)
BANKS = [('A', 0x00000, 0x20000), ('B', 0x20000, 0x20000), ('C', 0x40000, 0x20000), ('D', 0x60000, 0x20000),
         ('E', 0x80000, 0x10000), ('F', 0x90000, 0x4000), ('G', 0x94000, 0x4000), ('H', 0x98000, 0x8000),
         ('I', 0xA0000, 0x4000)]
VRAMCNT_OFS = [0x240, 0x241, 0x242, 0x243, 0x244, 0x245, 0x246, 0x248, 0x249]

OPS = {
    0x00: ('NOP', 0), 0x10: ('MTX_MODE', 1), 0x11: ('MTX_PUSH', 0), 0x12: ('MTX_POP', 1), 0x13: ('MTX_STORE', 1),
    0x14: ('MTX_RESTORE', 1), 0x15: ('MTX_IDENTITY', 0), 0x16: ('MTX_LOAD_4x4', 16), 0x17: ('MTX_LOAD_4x3', 12),
    0x18: ('MTX_MULT_4x4', 16), 0x19: ('MTX_MULT_4x3', 12), 0x1a: ('MTX_MULT_3x3', 9), 0x1b: ('MTX_SCALE', 3),
    0x1c: ('MTX_TRANS', 3), 0x20: ('COLOR', 1), 0x21: ('NORMAL', 1), 0x22: ('TEXCOORD', 1), 0x23: ('VTX_16', 2),
    0x24: ('VTX_10', 1), 0x25: ('VTX_XY', 1), 0x26: ('VTX_XZ', 1), 0x27: ('VTX_YZ', 1), 0x28: ('VTX_DIFF', 1),
    0x29: ('POLYGON_ATTR', 1), 0x2a: ('TEXIMAGE_PARAM', 1), 0x2b: ('TEXPLTT_BASE', 1), 0x30: ('DIF_AMB', 1),
    0x31: ('SPE_EMI', 1), 0x32: ('LIGHT_VECTOR', 1), 0x33: ('LIGHT_COLOR', 1), 0x34: ('SHININESS', 32),
    0x40: ('BEGIN_VTXS', 1), 0x41: ('END_VTXS', 0), 0x50: ('SWAP_BUFFERS', 1), 0x60: ('VIEWPORT', 1),
    0x70: ('BOX_TEST', 3), 0x71: ('POS_TEST', 2), 0x72: ('VEC_TEST', 1),
}
VERTEX_OPS = (0x23, 0x24, 0x25, 0x26, 0x27, 0x28)
TEXFMT = ['none', 'A3I5', '4col', '16col', '256col', '4x4', 'A5I3', 'direct']

# the 2D registers' names (engine A offsets; engine B has the same below 0x70)
REGS2D = [(0x000, 4, 'DISPCNT'), (0x008, 2, 'BG0CNT'), (0x00A, 2, 'BG1CNT'), (0x00C, 2, 'BG2CNT'),
          (0x00E, 2, 'BG3CNT'), (0x010, 4, 'BG0OFS'), (0x014, 4, 'BG1OFS'), (0x018, 4, 'BG2OFS'),
          (0x01C, 4, 'BG3OFS'), (0x040, 2, 'WIN0H'), (0x042, 2, 'WIN1H'), (0x044, 2, 'WIN0V'), (0x046, 2, 'WIN1V'),
          (0x048, 2, 'WININ'), (0x04A, 2, 'WINOUT'), (0x04C, 2, 'MOSAIC'), (0x050, 2, 'BLDCNT'),
          (0x052, 2, 'BLDALPHA'), (0x054, 2, 'BLDY'), (0x06C, 2, 'MASTER_BRIGHT')]
REGS_A_ONLY = [(0x004, 2, 'DISPSTAT'), (0x060, 2, 'DISP3DCNT'), (0x064, 4, 'DISPCAPCNT'), (0x304, 2, 'POWCNT'),
               (0x320, 2, 'RDLINES_COUNT'), (0x340, 2, 'ALPHA_TEST_REF'), (0x350, 4, 'CLEAR_COLOR'),
               (0x354, 2, 'CLEAR_DEPTH'), (0x356, 2, 'CLRIMAGE_OFFSET'), (0x358, 4, 'FOG_COLOR'),
               (0x35C, 2, 'FOG_OFFSET'), (0x610, 2, 'DISP_1DOT_DEPTH')]


class Vcs:
    """A .vcs file: .version, .chunks [(tag, file offset, size)], .data {tag: bytes}, .info {key: text}."""

    def __init__(self, path):
        self.path = path
        self.name = os.path.splitext(os.path.basename(path))[0]
        raw = open(path, 'rb').read()
        if raw[:4] != MAGIC:
            raise ValueError('%s: not a VCST file' % path)
        self.version, = struct.unpack_from('<I', raw, 4)
        self.chunks, self.data = [], {}
        pos = 8
        while pos + 8 <= len(raw):
            tag = raw[pos:pos + 4].decode('latin1')
            size, = struct.unpack_from('<I', raw, pos + 4)
            body = raw[pos + 8:pos + 8 + size]
            if len(body) != size:
                raise ValueError('%s: chunk %r truncated (%d of %d bytes)' % (path, tag, len(body), size))
            self.chunks.append((tag, pos, size))
            self.data[tag] = body
            pos += 8 + ((size + 3) & ~3)
        if pos != len(raw):
            raise ValueError('%s: %d bytes after the last chunk' % (path, len(raw) - pos))
        self.info = {}
        for line in self.data.get('INFO', b'').decode('utf-8', 'replace').splitlines():
            k, _, v = line.partition('=')
            self.info[k] = v
        for tag, size in (('IO9 ', 0x700), ('IO9B', 0x70), ('VRAM', 0xA4000), ('PLTT', 0x800), ('OAM ', 0x800)):
            if len(self.data.get(tag, b'')) != size:
                raise ValueError('%s: chunk %r is %d bytes, not %d' % (path, tag, len(self.data.get(tag, b'')), size))

    # the registers: engine 'A' reads IO9 (0x04000000+ofs), 'B' IO9B (0x04001000+ofs)
    def reg(self, ofs, size=2, engine='A'):
        io = self.data['IO9 '] if engine == 'A' else self.data['IO9B']
        return int.from_bytes(io[ofs:ofs + size], 'little')

    def commands(self):
        """The GX3D chunk as (op, flags, params) tuples."""
        g = self.data.get('GX3D', b'')
        w = struct.unpack('<%dI' % (len(g) // 4), g)
        i, out = 0, []
        while i < len(w):
            h = w[i]
            op, n, flags = h & 0xFF, (h >> 8) & 0xFF, (h >> 16) & 0xFF
            out.append((op, flags, w[i + 1:i + 1 + n]))
            i += 1 + n
        return out

    def bank(self, i):
        _, ofs, size = BANKS[i]
        return self.data['VRAM'][ofs:ofs + size]

    def vramcnt(self, i):
        return self.reg(VRAMCNT_OFS[i], 1)


def bank_use(name, cnt):
    """Where VRAMCNT puts a bank (GBATEK "DS Memory Control - VRAM")."""
    if not cnt & 0x80:
        return 'off'
    i = 'ABCDEFGHI'.index(name)
    mst = cnt & (3 if i < 2 else 7)
    ofs = (cnt >> 3) & 3
    fg = (ofs & 1) * 0x4000 + (ofs >> 1) * 0x10000
    table = {
        'A': {0: 'LCDC', 1: 'BG-A +0x%x' % (ofs * 0x20000), 2: 'OBJ-A +0x%x' % ((ofs & 1) * 0x20000),
              3: 'tex slot %d' % ofs},
        'C': {0: 'LCDC', 1: 'BG-A +0x%x' % (ofs * 0x20000), 2: 'ARM7 %d' % (ofs & 1), 3: 'tex slot %d' % ofs,
              4: 'BG-B'},
        'D': {0: 'LCDC', 1: 'BG-A +0x%x' % (ofs * 0x20000), 2: 'ARM7 %d' % (ofs & 1), 3: 'tex slot %d' % ofs,
              4: 'OBJ-B'},
        'E': {0: 'LCDC', 1: 'BG-A +0', 2: 'OBJ-A +0', 3: 'texpal 0-3', 4: 'BG-A extpal'},
        'F': {0: 'LCDC', 1: 'BG-A +0x%x' % fg, 2: 'OBJ-A +0x%x' % fg, 3: 'texpal %d' % ((ofs & 1) + (ofs >> 1) * 4),
              4: 'BG-A extpal %d-%d' % ((ofs & 1) * 2, (ofs & 1) * 2 + 1), 5: 'OBJ-A extpal'},
        'H': {0: 'LCDC', 1: 'BG-B +0', 2: 'BG-B extpal'},
        'I': {0: 'LCDC', 1: 'BG-B +0x8000', 2: 'OBJ-B +0', 3: 'OBJ-B extpal'},
    }
    t = table.get({'B': 'A', 'G': 'F'}.get(name, name))
    return t.get(mst, 'MST %d?' % mst)


def bg_space(v, engine, kind='BG'):
    """The engine's BG (or OBJ) VRAM as the 2D engine sees it, from the banks VRAMCNT maps there (ORed where they
    overlap, as on the DS)."""
    size = {('A', 'BG'): 0x80000, ('B', 'BG'): 0x20000, ('A', 'OBJ'): 0x40000, ('B', 'OBJ'): 0x20000}[(engine, kind)]
    space = bytearray(size)
    want = '%s-%s' % (kind, engine)
    for i, (name, _, bsize) in enumerate(BANKS):
        use = bank_use(name, v.vramcnt(i))
        if use != want and not use.startswith(want + ' +'):
            continue
        ofs = int(use.split('+')[1], 0) if '+' in use else 0
        b = v.bank(i)[:max(0, size - ofs)]
        if not any(space[ofs:ofs + len(b)]):
            space[ofs:ofs + len(b)] = b
        else:
            for k in range(len(b)):
                space[ofs + k] |= b[k]
    return space


def bg_ext_palette(v, engine, slot):
    """A BG extended palette slot (256 x 16 palettes of 256 colours, 8 KB), or None when no bank holds it."""
    for i, (name, _, _) in enumerate(BANKS):
        use = bank_use(name, v.vramcnt(i))
        b = v.bank(i)
        if engine == 'A' and use == 'BG-A extpal' and name == 'E':
            return b[slot * 0x2000:(slot + 1) * 0x2000]
        if engine == 'A' and use.startswith('BG-A extpal ') and name in 'FG':
            first = int(use.split()[-1].split('-')[0])
            if slot in (first, first + 1):
                return b[(slot - first) * 0x2000:(slot - first + 1) * 0x2000]
        if engine == 'B' and use == 'BG-B extpal':
            return b[slot * 0x2000:(slot + 1) * 0x2000]
    return None


def rgb15(c):
    r, g, b = c & 31, (c >> 5) & 31, (c >> 10) & 31
    return (r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2)


def render_text_bg(v, engine, n):
    """A text BG (BGxCNT, DISPCNT's char/screen blocks, the scroll) drawn alone, 256x192 RGB; colour 0 shows the
    backdrop. Extended palettes when DISPCNT bit 30 says so."""
    dispcnt = v.reg(0, 4, engine)
    cnt = v.reg(0x08 + 2 * n, 2, engine)
    hofs = v.reg(0x10 + 4 * n, 2, engine) & 0x1FF
    vofs = v.reg(0x12 + 4 * n, 2, engine) & 0x1FF
    char = ((cnt >> 2) & 15) * 0x4000
    scr = ((cnt >> 8) & 31) * 0x800
    if engine == 'A':
        char += ((dispcnt >> 24) & 7) * 0x10000
        scr += ((dispcnt >> 27) & 7) * 0x10000
    c256 = (cnt >> 7) & 1
    w = 512 if (cnt >> 14) & 1 else 256
    h = 512 if (cnt >> 15) & 1 else 256
    vram = bg_space(v, engine)
    pal = v.data['PLTT'][(0 if engine == 'A' else 0x400):][:0x200]
    pal16 = struct.unpack('<256H', pal)
    ext = None
    if c256 and (dispcnt >> 30) & 1:
        slot = n
        if n < 2 and (cnt >> 13) & 1:
            slot = n + 2
        ext = bg_ext_palette(v, engine, slot)
    back = rgb15(pal16[0])
    out = bytearray(256 * 192 * 3)
    for y in range(192):
        py = (y + vofs) % h
        for x in range(256):
            px = (x + hofs) % w
            tx, ty = px >> 3, py >> 3
            block = (tx >> 5) + (ty >> 5) * (w >> 8)
            e_at = scr + block * 0x800 + ((ty & 31) * 32 + (tx & 31)) * 2
            e = vram[e_at] | (vram[e_at + 1] << 8) if e_at + 1 < len(vram) else 0
            tile, hf, vf, pn = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1, e >> 12
            fx, fy = px & 7, py & 7
            if hf:
                fx = 7 - fx
            if vf:
                fy = 7 - fy
            if c256:
                at = char + tile * 64 + fy * 8 + fx
                ci = vram[at] if at < len(vram) else 0
                if ci == 0:
                    col = back
                elif ext is not None:
                    col = rgb15(ext[(pn * 256 + ci) * 2] | (ext[(pn * 256 + ci) * 2 + 1] << 8))
                else:
                    col = rgb15(pal16[ci])
            else:
                at = char + tile * 32 + fy * 4 + (fx >> 1)
                bt = vram[at] if at < len(vram) else 0
                ci = (bt >> 4) if fx & 1 else (bt & 15)
                col = back if ci == 0 else rgb15(pal16[pn * 16 + ci])
            o = (y * 256 + x) * 3
            out[o:o + 3] = bytes(col)
    return out


def write_png(path, w, h, px, channels):
    """px: rows top first, channels (3 RGB, 4 RGBA) bytes per pixel."""
    stride = w * channels
    raw = b''.join(b'\x00' + bytes(px[y * stride:(y + 1) * stride]) for y in range(h))

    def chunk(tag, body):
        return struct.pack('>I', len(body)) + tag + body + struct.pack('>I', zlib.crc32(tag + body) & 0xFFFFFFFF)

    color_type = 2 if channels == 3 else 6
    png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, color_type, 0, 0, 0))
    png += chunk(b'IDAT', zlib.compress(raw, 6)) + chunk(b'IEND', b'')
    with open(path, 'wb') as f:
        f.write(png)


def rgba_to_rgb(rgba):
    out = bytearray(len(rgba) // 4 * 3)
    out[0::3], out[1::3], out[2::3] = rgba[0::4], rgba[1::4], rgba[2::4]
    return out


# ---- the summary of the 3D commands ----
def g3_summary(v):
    """Counts and the polygons' attributes over the frame's own commands (not the prologue)."""
    s = {'commands': 0, 'prologue': 0, 'list': 0, 'reg': 0, 'vertices': 0, 'begins': 0, 'normals': 0,
         'lit_normals': 0, 'swap': None, 'ops': {}, 'polys': {'alpha': set(), 'mode': set(), 'texfmt': set(),
                                                             'fog': 0, 'light': 0, 'textured': 0}}
    poly_attr = tex = 0
    latched = 0
    in_prim = False
    prim_vtx = 0
    for op, flags, p in v.commands():
        s['commands'] += 1
        if flags & FLAG_PROLOGUE:
            s['prologue'] += 1
        if flags & FLAG_LIST:
            s['list'] += 1
        if flags & FLAG_REG:
            s['reg'] += 1
        name = OPS.get(op, ('op_%02x' % op, 0))[0]
        if not flags & FLAG_PROLOGUE:
            s['ops'][name] = s['ops'].get(name, 0) + 1
        if op == 0x29 and p:
            poly_attr = p[0]
        elif op == 0x2a and p:
            tex = p[0]
        elif op == 0x40 and p:
            latched = poly_attr
            in_prim = True
            prim_vtx = 0
            if not flags & FLAG_PROLOGUE:
                s['begins'] += 1
        elif op == 0x41:
            in_prim = False
        elif op == 0x21 and not flags & FLAG_PROLOGUE:
            s['normals'] += 1
            if in_prim and latched & 15:
                s['lit_normals'] += 1
        elif op == 0x50 and p:
            s['swap'] = p[0]
        if op in VERTEX_OPS and not flags & FLAG_PROLOGUE:
            s['vertices'] += 1
            prim_vtx += 1
            if in_prim and prim_vtx >= 3:
                pa = s['polys']
                pa['alpha'].add((latched >> 16) & 31)
                pa['mode'].add((latched >> 4) & 3)
                fmt = (tex >> 26) & 7
                pa['texfmt'].add(fmt)
                if fmt:
                    pa['textured'] += 1
                if (latched >> 15) & 1:
                    pa['fog'] += 1
                if latched & 15:
                    pa['light'] += 1
    return s


# ---- the feature tags ----
OBJ_SIZES = [[(8, 8), (16, 16), (32, 32), (64, 64)], [(16, 8), (32, 8), (32, 16), (64, 32)],
             [(8, 16), (8, 32), (16, 32), (32, 64)], [(8, 8), (8, 8), (8, 8), (8, 8)]]


def visible_objs(v, engine):
    """The OAM entries on screen: (attr0, attr1, attr2) of each."""
    oam = v.data['OAM '][(0 if engine == 'A' else 0x400):][:0x400]
    out = []
    for i in range(128):
        a0, a1, a2 = struct.unpack_from('<HHH', oam, i * 8)
        affine = (a0 >> 8) & 1
        if not affine and (a0 >> 9) & 1:
            continue  # disabled
        shape = a0 >> 14
        if shape == 3:
            continue
        w, h = OBJ_SIZES[shape][a1 >> 14]
        if affine and (a0 >> 9) & 1:
            w, h = w * 2, h * 2
        x = a1 & 0x1FF
        if x >= 256:
            x -= 512
        y = a0 & 0xFF
        if y + h > 256:
            y -= 256
        if x < 256 and x + w > 0 and y < 192 and y + h > 0:
            out.append((a0, a1, a2))
    return out


def bg_kinds(dispcnt, cnts, engine):
    """The kind of each BG 0-3 in this BG mode (GBATEK "DS Video BG Modes"), None where the mode has none."""
    mode = dispcnt & 7
    kinds = {0: ['text'] * 4, 1: ['text', 'text', 'text', 'affine'], 2: ['text', 'text', 'affine', 'affine'],
             3: ['text', 'text', 'text', 'ext'], 4: ['text', 'text', 'affine', 'ext'],
             5: ['text', 'text', 'ext', 'ext'], 6: ['text', None, 'large', None]}.get(mode, [None] * 4)
    kinds = list(kinds)
    if engine == 'A' and (dispcnt >> 3) & 1:
        kinds[0] = '3d'
    if engine == 'B' and mode == 6:
        kinds = [None] * 4
    for n in range(4):
        if kinds[n] == 'ext':
            c = cnts[n]
            kinds[n] = 'affine' if not (c >> 7) & 1 else 'bitmap'
    return kinds


def tags(v):
    """The features a frame uses, from its registers, OAM and 3D commands (the corpus index's tags)."""
    t = set()
    g3 = None
    capcnt = v.reg(0x64, 4)
    cap_on = bool(capcnt & 0x80000000)
    cap_a = cap_on and (capcnt >> 29) & 3 != 1  # the capture takes source A (alone or blended with B)
    if cap_on:
        t.add('capture')
    if cap_a and (capcnt >> 24) & 1:  # source A = the 3D layer alone
        g3 = g3_summary(v)
        if g3['vertices'] >= 3:
            t.add('3d')
    for engine in ('A', 'B'):
        dispcnt = v.reg(0, 4, engine)
        dmode = (dispcnt >> 16) & 3
        if engine == 'A' and dmode == 2:
            t.add('vram_display')
        # engine A's layers are drawn when shown, or when the capture takes their picture (source A = the 2D/3D mix)
        drawn = dmode == 1 or (engine == 'A' and cap_a and not (capcnt >> 24) & 1)
        if not drawn:
            continue
        cnts = [v.reg(0x08 + 2 * n, 2, engine) for n in range(4)]
        kinds = bg_kinds(dispcnt, cnts, engine)
        enabled = [bool((dispcnt >> (8 + n)) & 1) and kinds[n] is not None for n in range(4)]
        layers = sum(1 << n for n in range(4) if enabled[n])
        objs = visible_objs(v, engine) if (dispcnt >> 12) & 1 else []
        if objs:
            layers |= 16
        for n in range(4):
            if not enabled[n]:
                continue
            k = kinds[n]
            if k == '3d':
                g3 = g3 or g3_summary(v)
                if g3['vertices'] >= 3:
                    t.add('3d')
            else:
                t.add('bg_' + k)
            if k != '3d' and (cnts[n] >> 6) & 1 and v.reg(0x4C, 2, engine) & 0xFF:
                t.add('mosaic')
        if objs:
            t.add('obj')
            for a0, a1, a2 in objs:
                m = (a0 >> 10) & 3
                if (a0 >> 8) & 1:
                    t.add('obj_affine')
                if m == 3:
                    t.add('obj_bitmap')
                if m == 2 and (dispcnt >> 15) & 1:
                    t.add('obj_window')
                if m == 1:
                    t.add('blend')
                if (a0 >> 12) & 1 and v.reg(0x4C, 2, engine) & 0xFF00:
                    t.add('mosaic')
        if (dispcnt >> 13) & 3:
            t.add('window')
        bld = v.reg(0x50, 2, engine)
        eff = (bld >> 6) & 3
        first = bld & 0x3F
        if eff == 1 and first & (layers | 32) and (bld >> 8) & 0x3F:
            t.add('blend')
        if eff in (2, 3) and first & (layers | 32) and v.reg(0x54, 2, engine) & 31:
            t.add('brighten')
        mb = v.reg(0x6C, 2, engine)
        if (mb >> 14) & 3 in (1, 2) and mb & 31:
            t.add('brightness')
    if '3d' in t:
        d3 = v.reg(0x60)
        pa = g3['polys']
        if 5 in pa['texfmt']:
            t.add('3d_tex4x4')
        if (d3 >> 7) & 1 and pa['fog']:
            t.add('3d_fog')
        if (d3 >> 5) & 1:
            t.add('3d_edge')
        if 2 in pa['mode']:
            t.add('3d_toon')
        if 3 in pa['mode']:
            t.add('3d_shadow')
        if (d3 >> 3) & 1 and ((pa['alpha'] - {0, 31}) or pa['texfmt'] & {1, 6}):
            t.add('3d_translucent')
        if g3['lit_normals']:
            t.add('3d_light')
        if pa['textured']:
            t.add('3d_tex')
        if 0 in pa['alpha']:
            t.add('3d_wire')
        if (d3 >> 4) & 1:
            t.add('3d_antialias')
        if (d3 >> 2) & 1:
            t.add('3d_alphatest')
    return sorted(t)


# ---- the report ----
def describe(v, out):
    w = out.write
    w('%s: VCST v%d, %d chunks\n' % (v.path, v.version, len(v.chunks)))
    for tag, ofs, size in v.chunks:
        w('  %-4s at 0x%07x  %8d bytes\n' % (tag, ofs, size))
    w('INFO:\n')
    for line in v.data.get('INFO', b'').decode('utf-8', 'replace').splitlines():
        w('  %s\n' % line)
    for engine in ('A', 'B'):
        regs = ['%s=%0*x' % (name, size * 2, v.reg(ofs, size, engine)) for ofs, size, name in REGS2D]
        if engine == 'A':
            regs += ['%s=%0*x' % (name, size * 2, v.reg(ofs, size)) for ofs, size, name in REGS_A_ONLY]
        w('engine %s:\n' % engine)
        for i in range(0, len(regs), 6):
            w('  ' + ' '.join(regs[i:i + 6]) + '\n')
        dispcnt = v.reg(0, 4, engine)
        kinds = bg_kinds(dispcnt, [v.reg(0x08 + 2 * n, 2, engine) for n in range(4)], engine)
        on = ['BG%d:%s' % (n, kinds[n]) for n in range(4) if (dispcnt >> (8 + n)) & 1 and kinds[n]]
        objs = visible_objs(v, engine)
        w('  mode %d, display %d, %s, OBJ %s (%d on screen)\n' % (
            dispcnt & 7, (dispcnt >> 16) & 3, ' '.join(on) or 'no BG',
            'on' if (dispcnt >> 12) & 1 else 'off', len(objs)))
    w('VRAM banks:\n')
    for i, (name, _, size) in enumerate(BANKS):
        b = v.bank(i)
        used = size - b.count(0)
        cnt = v.vramcnt(i)
        w('  %s VRAMCNT=%02x %-18s %6d/%6d bytes nonzero, crc32 %08x\n' % (
            name, cnt, bank_use(name, cnt), used, size, zlib.crc32(b) & 0xFFFFFFFF))
    pl = v.data['PLTT']
    w('PLTT: %s nonzero bytes (BG-A, OBJ-A, BG-B, OBJ-B)\n' % ', '.join(
        str(0x200 - pl[k * 0x200:(k + 1) * 0x200].count(0)) for k in range(4)))
    w('OAM: %d / %d entries on screen (A / B)\n' % (len(visible_objs(v, 'A')), len(visible_objs(v, 'B'))))
    g3 = g3_summary(v)
    w('GX3D: %d commands (%d prologue, %d from lists, %d register catch-ups); the frame: %d vertices, %d BEGINs, '
      '%d normals, SWAP_BUFFERS %s\n' % (g3['commands'], g3['prologue'], g3['list'], g3['reg'], g3['vertices'],
                                          g3['begins'], g3['normals'],
                                          'none' if g3['swap'] is None else '%x' % g3['swap']))
    ops = sorted(g3['ops'].items(), key=lambda kv: -kv[1])
    for i in range(0, len(ops), 6):
        w('  ' + ' '.join('%s:%d' % kv for kv in ops[i:i + 6]) + '\n')
    pa = g3['polys']
    w('  polygons: alpha %s, modes %s, texture formats %s, %d fogged, %d lit\n' % (
        sorted(pa['alpha']), sorted(pa['mode']), [TEXFMT[f] for f in sorted(pa['texfmt'])], pa['fog'], pa['light']))
    w('tags: %s\n' % ' '.join(tags(v)))


def print_cmds(v, n, out):
    for k, (op, flags, p) in enumerate(v.commands()):
        if n is not None and k >= n:
            break
        name = OPS.get(op, ('op_%02x' % op, 0))[0]
        fl = ''.join(c for c, b in (('P', FLAG_PROLOGUE), ('L', FLAG_LIST), ('R', FLAG_REG)) if flags & b) or '-'
        out.write('%6d %-3s %-14s %s\n' % (k, fl, name, ' '.join('%08x' % x for x in p)))


def summary_json(v):
    g3 = g3_summary(v)
    g3['polys'] = {k: (sorted(x) if isinstance(x, set) else x) for k, x in g3['polys'].items()}
    return {'file': v.path, 'version': v.version, 'chunks': [[t, s] for t, _, s in v.chunks], 'info': v.info,
            'gx3d': g3, 'tags': tags(v),
            'banks': {name: {'vramcnt': v.vramcnt(i), 'use': bank_use(name, v.vramcnt(i)),
                             'nonzero': size - v.bank(i).count(0)} for i, (name, _, size) in enumerate(BANKS)}}


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('files', nargs='+')
    ap.add_argument('--png', help='the LSHT chunk as PNG (one file)')
    ap.add_argument('--l3d', help='the L3D chunk as PNG (one file)')
    ap.add_argument('--bg', nargs=2, metavar=('EN', 'OUT'), help='a text BG drawn from VRAM: A0-A3, B0-B3')
    ap.add_argument('--cmds', nargs='?', type=int, const=-1, help='list the 3D commands (the first N)')
    ap.add_argument('--tags', action='store_true', help='only the tags')
    ap.add_argument('--json', action='store_true')
    a = ap.parse_args(argv)
    if (a.png or a.l3d or a.bg) and len(a.files) != 1:
        ap.error('--png/--l3d/--bg take one file')
    status = 0
    for path in a.files:
        try:
            v = Vcs(path)
        except (ValueError, OSError) as e:
            print('ERROR %s' % e)
            status = 1
            continue
        if a.tags:
            print('%s: %s' % (v.name, ' '.join(tags(v))))
            continue
        if a.json:
            print(json.dumps(summary_json(v)))
        elif a.cmds is None or a.png or a.l3d or a.bg:
            describe(v, sys.stdout)
        if a.cmds is not None:
            print_cmds(v, None if a.cmds < 0 else a.cmds, sys.stdout)
        if a.png:
            sh = v.data.get('LSHT', b'')
            if len(sh) != 256 * 384 * 4:
                print('no LSHT picture')
                status = 1
            else:
                write_png(a.png, 256, 384, rgba_to_rgb(sh), 3)
                print('wrote %s' % a.png)
        if a.l3d:
            l3 = v.data.get('L3D ', b'')
            if len(l3) != 256 * 192 * 4:
                print('no L3D picture')
                status = 1
            else:
                write_png(a.l3d, 256, 192, l3, 4)
                print('wrote %s' % a.l3d)
        if a.bg:
            en = a.bg[0].upper()
            if len(en) != 2 or en[0] not in 'AB' or en[1] not in '0123':
                ap.error('--bg: A0-A3 or B0-B3')
            write_png(a.bg[1], 256, 192, render_text_bg(v, en[0], int(en[1])), 3)
            print('wrote %s' % a.bg[1])
    return status


if __name__ == '__main__':
    sys.exit(main())
