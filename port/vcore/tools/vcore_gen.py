"""Scene generators for vcore_check: per piece, a random .vcs for the piece's feature, everything else plain.

gen(piece, rng) -> (Frame, desc, kind)   kind "scr" compares both screens, "3d" the 3D layer, "scr+vram" also the banks.
All randomness comes from rng (random.Random), so a (piece, seed, index) always gives the same scene.
Register offsets: GBATEK's register maps; the engine B registers are the same offsets in io9b.
"""
import math
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mkvcs import Frame  # noqa: E402

DISPCNT, BGCNT, BGOFS, BG2PA, BG3PA = 0x00, 0x08, 0x10, 0x20, 0x30
WIN0H, WIN1H, WIN0V, WIN1V, WININ, WINOUT, MOSAIC, BLDCNT, BLDALPHA, BLDY = (
    0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x50, 0x52, 0x54)
DISP3DCNT, DISPCAPCNT, MASTER_BRIGHT = 0x60, 0x64, 0x6C
A, B, C, D, E, F, G, H, I = range(9)
BANKSIZE = [0x20000] * 4 + [0x10000, 0x4000, 0x4000, 0x8000, 0x4000]
VRAMCNT = [0x240, 0x241, 0x242, 0x243, 0x244, 0x245, 0x246, 0x248, 0x249]


def fill_vram(f, rng, banks=None):
    for b in (range(9) if banks is None else banks):
        f.bank(b, 0, rng.randbytes(BANKSIZE[b]))


def vramcnt(f, bank, mst, ofs=0):
    f.io9[VRAMCNT[bank]] = 0x80 | mst | ofs << 3


def map_bg_style(f, extpal=False, obj_a=False):
    """A,B -> engine A BG (or OBJ, with G as a little BG), C -> B BG, D -> B OBJ; E/F/H/I extended palettes"""
    if obj_a:
        vramcnt(f, A, 2, 0), vramcnt(f, B, 2, 1), vramcnt(f, G, 1, 0)
    else:
        vramcnt(f, A, 1, 0), vramcnt(f, B, 1, 1)
    vramcnt(f, C, 4), vramcnt(f, D, 4)
    if extpal:
        vramcnt(f, E, 4), vramcnt(f, F, 5), vramcnt(f, H, 2), vramcnt(f, I, 3)


def ew16(f, e, ofs, v):
    f.w16(ofs, v & 0xFFFF, b=e == 1)


def ew32(f, e, ofs, v):
    f.w32(ofs, v & 0xFFFFFFFF, b=e == 1)


def er16(f, e, ofs):
    return struct.unpack_from("<H", f.io9b if e else f.io9, ofs)[0]


def eng_regs_desc(f, e):
    r = lambda o: er16(f, e, o)
    d = "eng%s DISPCNT=%08X" % ("AB"[e], f.r32(0, e == 1))
    d += " BGCNT=" + ",".join("%04X" % r(0x08 + 2 * n) for n in range(4))
    d += " HOFS/VOFS=" + ",".join("%d/%d" % (r(0x10 + 4 * n) & 511, r(0x12 + 4 * n) & 511) for n in range(4))
    d += " WIN0H/1H/0V/1V=%04X,%04X,%04X,%04X IN/OUT=%04X/%04X MOSAIC=%04X BLDCNT/ALPHA/Y=%04X/%04X/%04X" % (
        r(0x40), r(0x42), r(0x44), r(0x46), r(0x48), r(0x4A), r(0x4C), r(0x50), r(0x52), r(0x54))
    return d


def reg_desc(f):
    d = eng_regs_desc(f, 0) + "\n" + eng_regs_desc(f, 1)
    d += "\nVRAMCNT A-I=" + ",".join("%02X" % f.io9[o] for o in VRAMCNT) + " POWCNT1=%04X MASTER_BRIGHT=%04X/%04X " \
        "DISPCAPCNT=%08X DISP3DCNT=%04X" % (er16(f, 0, 0x304), er16(f, 0, 0x6C), er16(f, 1, 0x6C), f.r32(0x64), er16(f, 0, 0x60))
    return d


# ---------------------------------------------------------------- 2D layers
def text_bgs(f, rng, e, mask, eight=None, extpal=False, prios=None):
    bits = 0
    for n in range(4):
        if not mask >> n & 1:
            continue
        is8 = rng.random() < 0.4 if eight is None else eight
        cnt = (rng.randrange(4) if prios is None else prios[n]) | rng.randrange(4) << 2 | (is8 << 7) \
            | rng.randrange(32) << 8 | rng.randrange(4) << 14
        if extpal:
            cnt |= rng.randrange(2) << 13
        ew16(f, e, BGCNT + 2 * n, cnt)
        ew16(f, e, BGOFS + 4 * n, rng.randrange(512)), ew16(f, e, BGOFS + 4 * n + 2, rng.randrange(512))
        bits |= 1 << (8 + n)
    return bits


def affine_regs(f, rng, e, n):
    sc = rng.choice([0x100, 0x80, 0x200, 0x100, 0x180, 0x40])
    ang = rng.random() * 6.2832
    c, s = int(math.cos(ang) * sc), int(math.sin(ang) * sc)
    if rng.random() < 0.4:
        c, s = sc, 0
    pa, pb, pc, pd = c, -s, s, c
    if rng.random() < 0.15:
        pa, pd = -pa, -pd
    if rng.random() < 0.1:
        pb, pc = rng.randrange(-0x200, 0x200), rng.randrange(-0x200, 0x200)
    o = BG2PA if n == 2 else BG3PA
    for i, v in enumerate((pa, pb, pc, pd)):
        ew16(f, e, o + 2 * i, v)
    ew32(f, e, o + 8, rng.randrange(-0x40000, 0x40000) & 0x0FFFFFFF)
    ew32(f, e, o + 12, rng.randrange(-0x40000, 0x40000) & 0x0FFFFFFF)
    return "pqrs=%d,%d,%d,%d" % (pa, pb, pc, pd)


def bases(f, rng, e):
    return (rng.randrange(8) << 24 | rng.randrange(8) << 27) if e == 0 else 0


def gen_p01(rng, f):
    e = rng.randrange(2)
    ext = rng.random() < 0.3
    map_bg_style(f, extpal=ext)
    fill_vram(f, rng)
    m = rng.randrange(1, 16)
    dc = 0x00010000 | text_bgs(f, rng, e, m, eight=(True if ext and rng.random() < 0.7 else None), extpal=ext)
    dc |= (1 << 30 if ext else 0) | bases(f, rng, e)
    ew32(f, e, DISPCNT, dc)
    return "text BGs mask=%X extpal=%d" % (m, ext), "scr"


def gen_p02(rng, f):
    e = rng.randrange(2)
    mode = rng.choice([1, 2, 4])
    map_bg_style(f)
    fill_vram(f, rng)
    dc = 0x00010000 | mode | bases(f, rng, e)
    desc = ""
    lay = {1: [3], 2: [2, 3], 4: [2]}[mode]
    for n in range(4):
        if n in lay:
            ew16(f, e, BGCNT + 2 * n, rng.randrange(4) | rng.randrange(16) << 2 | rng.randrange(2) << 13
                 | rng.randrange(32) << 8 | rng.randrange(4) << 14)
            desc += " BG%d %s" % (n, affine_regs(f, rng, e, n))
            dc |= 1 << (8 + n)
        elif rng.random() < 0.5:
            dc |= text_bgs(f, rng, e, 1 << n)
    ew32(f, e, DISPCNT, dc)
    return "affine mode %d%s" % (mode, desc), "scr"


def gen_p03(rng, f):
    e = rng.randrange(2)
    mode = rng.choice([3, 4, 5])
    ext = rng.random() < 0.3
    map_bg_style(f, extpal=ext)
    fill_vram(f, rng)
    dc = 0x00010000 | mode | (1 << 30 if ext else 0) | bases(f, rng, e)
    desc = ""
    for n in {3: [3], 4: [3], 5: [2, 3]}[mode]:
        kind = rng.choice(["map16", "bmp256", "direct"])
        cnt = rng.randrange(4) | rng.randrange(32) << 8 | rng.randrange(2) << 13
        if kind == "map16":
            cnt |= rng.randrange(16) << 2 | rng.randrange(4) << 14
        else:
            cnt |= 1 << 7 | (4 if kind == "direct" else 0) | rng.randrange(4) << 14
        ew16(f, e, BGCNT + 2 * n, cnt)
        desc += " BG%d %s cnt=%04X %s" % (n, kind, cnt, affine_regs(f, rng, e, n))
        dc |= 1 << (8 + n)
    ew32(f, e, DISPCNT, dc)
    return "extended mode %d%s" % (mode, desc), "scr"


def gen_p04(rng, f):
    kind = rng.choice(["large", "mosaic-text", "mosaic-affine"])
    if kind == "large":
        for b in range(4):
            vramcnt(f, b, 1, b)
        fill_vram(f, rng, range(4))
        ew16(f, 0, BGCNT + 4, rng.randrange(4) | rng.randrange(2) << 13 | rng.randrange(2) << 14 | rng.randrange(2) << 6)
        ew32(f, 0, DISPCNT, 0x00010006 | 1 << 10)
        affine_regs(f, rng, 0, 2)
        ew16(f, 0, MOSAIC, rng.randrange(0x10000) if rng.random() < 0.5 else 0)
        return "large bitmap", "scr"
    e = rng.randrange(2)
    map_bg_style(f)
    fill_vram(f, rng)
    dc = 0x00010000
    if kind == "mosaic-text":
        dc |= text_bgs(f, rng, e, rng.randrange(1, 16))
    else:
        dc |= 2 | 1 << 10 | 1 << 11
        for n in (2, 3):
            ew16(f, e, BGCNT + 2 * n, rng.randrange(4) | rng.randrange(16) << 2 | rng.randrange(32) << 8
                 | rng.randrange(4) << 14)
            affine_regs(f, rng, e, n)
    for n in range(4):
        ew16(f, e, BGCNT + 2 * n, er16(f, e, BGCNT + 2 * n) | (rng.random() < 0.7) << 6)
    ew16(f, e, MOSAIC, rng.randrange(0x10000))
    ew32(f, e, DISPCNT, dc)
    return kind, "scr"


def rand_oam(f, rng, e, n, modes, hide=0.15):
    ofs = 0x400 * e
    desc = []
    for i in range(128):
        o = ofs + i * 8
        if i >= n:
            struct.pack_into("<H", f.oam, o, 1 << 9)
            continue
        mode = rng.choice(modes)
        a0 = (rng.randrange(256) if rng.random() < 0.8 else rng.randrange(150, 256)) | mode << 8 | rng.randrange(3) << 14
        if mode == 0 and rng.random() < hide:
            a0 |= 1 << 9
        if mode != 3:
            a0 |= rng.randrange(2) << 13
        a1 = (rng.randrange(512) if rng.random() < 0.8 else rng.randrange(400, 512)) | rng.randrange(4) << 14
        a1 |= rng.randrange(32) << 9 if mode in (1, 3) else rng.randrange(4) << 12
        if mode in (1, 3) and mode != 3 and False:
            pass
        a2 = rng.randrange(1024) | rng.randrange(4) << 10 | rng.randrange(16) << 12
        struct.pack_into("<HHH", f.oam, o, a0, a1, a2)
        desc.append("#%d a0=%04X a1=%04X a2=%04X" % (i, a0, a1, a2))
    return desc


def rand_aff_params(f, rng, e):
    for g in range(32):
        ang = rng.random() * 6.2832
        sc = rng.choice([0x100, 0x80, 0x200, 0x100, 0x180])
        c, s = int(math.cos(ang) * sc), int(math.sin(ang) * sc)
        for k, v in enumerate((c, -s, s, c)):
            struct.pack_into("<h", f.oam, 0x400 * e + (g * 4 + k) * 8 + 6, v)


def obj_setup(f, rng, e):
    ext = rng.random() < 0.3
    map_bg_style(f, extpal=ext, obj_a=(e == 0))
    fill_vram(f, rng)
    dc = 0x00010000 | 1 << 12
    if rng.random() < 0.5:
        dc |= 1 << 4 | rng.randrange(4) << 20
    if ext and rng.random() < 0.7:
        dc |= 1 << 31
    return dc


def gen_p05(rng, f):
    e = rng.randrange(2)
    dc = obj_setup(f, rng, e)
    desc = rand_oam(f, rng, e, rng.choice([4, 12, 40, 128]), [0] * 8 + [1])
    if rng.random() < 0.3:
        ew16(f, e, BLDCNT, rng.randrange(0x4000)), ew16(f, e, BLDALPHA, rng.randrange(0x2000))
    ew32(f, e, DISPCNT, dc)
    return "normal OBJs " + "; ".join(desc[:6]), "scr"


def gen_p06(rng, f):
    e = rng.randrange(2)
    dc = obj_setup(f, rng, e)
    desc = rand_oam(f, rng, e, rng.choice([3, 8, 24]), [0, 1, 3, 3])
    rand_aff_params(f, rng, e)
    ew32(f, e, DISPCNT, dc)
    return "affine OBJs " + "; ".join(desc[:6]), "scr"


def gen_p07(rng, f):
    e = rng.randrange(2)
    dc = obj_setup(f, rng, e)
    kind = rng.choice(["bitmap", "window", "mosaic"])
    # rand_oam's "mode" is OAM attr 0 bits 8-9 (normal / affine / hidden / double); the OBJ mode (bits 10-11) is set below
    if kind == "bitmap":
        dc |= rng.randrange(2) << 6 | rng.randrange(2) << 5 | rng.randrange(2) << 22
        modes, objmode = [0, 0, 1, 3], [3, 3, 3, 0, 1]
    elif kind == "window":
        modes, objmode = [0, 0, 1, 3], [2, 2, 2, 0, 0, 3]
        dc |= 1 << 15 | text_bgs(f, rng, e, 1, prios=[0, 0, 0, 0])
        if e == 0:  # the text BG needs real BG VRAM (obj_setup gave engine A only a 16 KB bank G): A, B -> BG, G -> OBJ
            vramcnt(f, A, 1, 0), vramcnt(f, B, 1, 1), vramcnt(f, G, 2, 0)
        if rng.random() < 0.5:
            dc |= rng.randrange(1, 4) << 13  # windows 0/1 on as well
            for k in range(4):
                ew16(f, e, 0x40 + 2 * k, rng.randrange(0x10000))
        ew16(f, e, WININ, rng.randrange(0x10000))
        ew16(f, e, WINOUT, rng.randrange(0x10000))
    else:
        modes, objmode = [0, 0, 1, 3], [0, 0, 1, 3]
        ew16(f, e, MOSAIC, rng.randrange(0x10000))
        dc |= rng.randrange(2) << 6 | rng.randrange(2) << 5 | rng.randrange(2) << 22
    desc = rand_oam(f, rng, e, rng.choice([4, 10, 30]), modes)
    for i in range(128):
        o = 0x400 * e + i * 8
        a0 = struct.unpack_from("<H", f.oam, o)[0]
        if a0 & 0x300 == 0x200:
            continue  # hidden
        a0 = a0 & ~0xC00 | rng.choice(objmode) << 10
        if kind == "mosaic" and rng.random() < 0.7:
            a0 |= 1 << 12
        struct.pack_into("<H", f.oam, o, a0)
    rand_aff_params(f, rng, e)
    ew32(f, e, DISPCNT, dc)
    return kind + " OBJs " + "; ".join(desc[:5]), "scr"


def layers_scene(f, rng, e):
    dc = 0x00010000 | text_bgs(f, rng, e, rng.randrange(1, 16))
    rand_oam(f, rng, e, 20, [0, 0, 1], hide=0.1)
    return dc | 1 << 12


def gen_p08(rng, f):
    e = rng.randrange(2)
    map_bg_style(f)
    if e == 0:
        vramcnt(f, E, 2)
    fill_vram(f, rng)
    dc = layers_scene(f, rng, e) | rng.randrange(8) << 13
    for o in (WIN0H, WIN1H, WIN0V, WIN1V):
        ew16(f, e, o, rng.randrange(0x10000))
    ew16(f, e, WININ, rng.randrange(0x10000)), ew16(f, e, WINOUT, rng.randrange(0x10000))
    if rng.random() < 0.4:
        ew16(f, e, BLDCNT, rng.randrange(0x4000)), ew16(f, e, BLDALPHA, rng.randrange(0x2000))
    ew32(f, e, DISPCNT, dc)
    return "windows", "scr"


def gen_p09(rng, f):
    e = rng.randrange(2)
    map_bg_style(f)
    if e == 0:
        vramcnt(f, E, 2)
    fill_vram(f, rng)
    dc = layers_scene(f, rng, e)
    if rng.random() < 0.3:
        dc |= rng.randrange(8) << 13
        for o in (WIN0H, WIN1H, WIN0V, WIN1V, WININ, WINOUT):
            ew16(f, e, o, rng.randrange(0x10000))
    ew16(f, e, BLDCNT, rng.randrange(0x10000)), ew16(f, e, BLDALPHA, rng.randrange(0x10000))
    ew16(f, e, BLDY, rng.randrange(0x20))
    ew32(f, e, DISPCNT, dc)
    return "compositor", "scr"


def gen_p10(rng, f):
    map_bg_style(f)
    fill_vram(f, rng)
    f.w16(0x304, rng.choice([0x820F, 0x820F, 0x020F, 0x800F, 0x0003, 0x8005, 0x820D, 0x020B, 0x8209]))
    for e in (0, 1):
        dc = 0x00000000 | text_bgs(f, rng, e, rng.randrange(0, 4))
        dc |= rng.randrange(4 if e == 0 else 2) << 16 | (rng.randrange(4) << 18 if e == 0 else 0)
        dc |= (rng.random() < 0.15) << 7
        ew32(f, e, DISPCNT, dc)
        mb = rng.choice([0, 0x4000, 0x8000, 0xC000]) | rng.randrange(0, 32 if rng.random() < 0.2 else 17)
        ew16(f, e, MASTER_BRIGHT, 0 if rng.random() < 0.3 else mb)
    for b in range(4):  # banks as LCDC (MST 0) feed display mode 2, or disabled
        if rng.random() < 0.5:
            f.io9[VRAMCNT[b]] = 0x80 if rng.random() < 0.8 else 0
    return "display modes / master brightness", "scr"


def gen_p11(rng, f):
    map_bg_style(f)
    fill_vram(f, rng)
    dc = 0x00010000 | text_bgs(f, rng, 0, rng.randrange(1, 16)) | rng.randrange(4) << 18
    dc = (dc & ~(3 << 16)) | rng.choice([1, 1, 2]) << 16
    dst = rng.randrange(4)
    vramcnt(f, dst, 0)
    if rng.random() < 0.6:
        vramcnt(f, (dc >> 18) & 3, 0)
    cap = rng.randrange(0x20) | rng.randrange(0x20) << 8 | dst << 16 | rng.randrange(4) << 18 | rng.randrange(4) << 20 \
        | rng.randrange(4) << 26 | rng.choice([0, 1, 2, 2]) << 29 | 1 << 31
    ew32(f, 0, DISPCAPCNT, cap)
    ew32(f, 0, DISPCNT, dc)
    return "capture", "scr+vram"


def gen_p12(rng, f):
    valid = {A: [0, 1, 2, 3], B: [0, 1, 2, 3], C: [0, 1, 2, 3, 4], D: [0, 1, 2, 3, 4], E: [0, 1, 2, 3, 4],
             F: [0, 1, 2, 3, 4, 5], G: [0, 1, 2, 3, 4, 5], H: [0, 1, 2], I: [0, 1, 2, 3]}
    for b in range(9):
        if rng.random() < 0.15:
            continue
        mst = rng.choice(valid[b])
        ofs = rng.randrange(4) if b in (A, B, C, D, F, G) else 0
        if b in (A, B) and mst == 2:
            ofs &= 1
        if b in (A, B, C, D) and mst == 3:
            ofs &= 3
        vramcnt(f, b, mst, ofs)
    fill_vram(f, rng)
    for e in (0, 1):
        dc = 0x00010000 | text_bgs(f, rng, e, rng.randrange(1, 16), extpal=True) | 1 << 12
        dc |= rng.randrange(2) << 30 | rng.randrange(2) << 31
        rand_oam(f, rng, e, 20, [0])
        if e == 0:
            dc |= bases(f, rng, e)
            if rng.random() < 0.2:
                dc = (dc & ~(3 << 16)) | 2 << 16 | rng.randrange(4) << 18
        if rng.random() < 0.5:
            dc |= 1 << 4 | rng.randrange(4) << 20
        ew32(f, e, DISPCNT, dc)
    return "vram mapping", "scr"


# ---------------------------------------------------------------- 3D
def v16(x):
    return int(x * 4096) & 0xFFFF


def pack_normal(x, y, z):
    c = lambda v: int(max(-1, min(0.998, v)) * 512) & 0x3FF
    return c(x) | c(y) << 10 | c(z) << 20


def rotmat43(rng):
    a, b = rng.uniform(0, 6.28), rng.uniform(0, 6.28)
    ca, sa, cb, sb = math.cos(a), math.sin(a), math.cos(b), math.sin(b)
    s = rng.uniform(0.5, 1.2)
    m = [ca, sa, 0, -sa, ca, 0, 0, 0, 1]
    m2 = [cb, 0, sb, 0, 1, 0, -sb, 0, cb]
    r = [sum(m[i * 3 + k] * m2[k * 3 + j] for k in range(3)) * s * 4096 for i in range(3) for j in range(3)]
    return r + [rng.uniform(-0.5, 0.5) * 4096 for _ in range(3)]


def gen_3d(piece, rng, f):
    P = lambda n: piece == n
    desc = []
    ew32(f, 0, DISPCNT, 0x00010000 | 1 << 3 | 1 << 8)
    vramcnt(f, A, 3, 0), vramcnt(f, B, 3, 1), vramcnt(f, C, 3, 2), vramcnt(f, D, 3, 3)
    vramcnt(f, E, 3, 0), vramcnt(f, F, 3, 0), vramcnt(f, G, 3, 1)
    fill_vram(f, rng, [A, B, C, D, E, F, G])
    textured = P(17) or (rng.random() < (0.15 if P(18) else 0.25) and not P(13) and not P(14))
    d3 = 1 if textured else 0
    if P(17):
        d3 |= rng.randrange(2) << 1 | 1 << 3  # alpha blending on: with bit 3 off the oracle draws translucent pixels opaque (p18's rule)
    if P(18):
        d3 |= rng.randrange(2) << 2 | 1 << 3 | rng.randrange(2) << 1
    if P(19):
        d3 |= rng.randrange(2) << 1 | rng.randrange(2) << 4 | rng.randrange(2) << 5 | rng.randrange(2) << 6 \
            | rng.randrange(2) << 7 | rng.randrange(11) << 8 | (rng.random() < 0.25) << 14 | 1 << 3
    if P(14) or P(16):
        d3 |= rng.randrange(2) << 3
    f.w16(DISP3DCNT, d3)
    cc = rng.randrange(0x8000) | rng.randrange(32) << 16 | rng.randrange(64) << 24 | (rng.random() < 0.3) << 15
    if not P(19) and (rng.random() < 0.5 or P(14)):
        cc = rng.randrange(0x8000) | 31 << 16
    f.w32(0x350, cc), f.w16(0x354, rng.choice([0x7FFF, 0x7FFF, rng.randrange(0x8000)]))
    f.w16(0x356, rng.randrange(0x10000) if d3 & 1 << 14 else 0)
    if P(19):
        for i in range(8):
            f.w16(0x330 + 2 * i, rng.randrange(0x8000))
        f.w32(0x358, rng.randrange(0x8000) | rng.randrange(32) << 16)
        f.w16(0x35C, rng.randrange(0x8000))
        for i in range(32):
            f.io9[0x360 + i] = rng.randrange(128)
            struct.pack_into("<H", f.io9, 0x380 + 2 * i, rng.randrange(0x8000))
    if P(18):
        f.w16(0x340, rng.randrange(32))
    one = 0x1000
    rect = P(19) and rng.random() < 0.55
    persp = rng.random() < 0.5 and not P(16) and not P(17) and not rect  # p17: ortho, so the rasterizer's interpolation is exact
    f.cmd(0x10, 0)
    if persp:
        n_, f_ = 1.0, rng.choice([8.0, 20.0, 100.0])
        pj = [one, 0, 0, 0, 0, one, 0, 0, 0, 0, int(-(f_ + n_) / (f_ - n_) * 4096), -4096, 0, 0,
              int(-2 * f_ * n_ / (f_ - n_) * 4096), 0]
        f.cmd(0x16, *[p & 0xFFFFFFFF for p in pj])
    else:
        f.cmd(0x16, one, 0, 0, 0, 0, one, 0, 0, 0, 0, one, 0, 0, 0, 0, one)
    f.cmd(0x10, 2), f.cmd(0x15)
    if persp:
        f.cmd(0x1C, 0, 0, (-int(rng.uniform(1.5, 6) * 4096)) & 0xFFFFFFFF)
    if P(13):
        sm = lambda: int(rng.uniform(-0.7, 0.7) * 4096) & 0xFFFFFFFF
        # p13: modes 0 (projection), 1, 2 and the 31-entry / 1-entry stacks. POP / RESTORE only of what was pushed /
        # stored: an empty slot is the zero matrix (the oracle's rule, tested by t13), which makes polygons that
        # collapse to one point, and those belong to piece 14 / 16, not here.
        md, depth, stored, pstored, pdepth = 2, 0, set(), False, 0
        for _ in range(rng.randrange(2, 16)):
            op = rng.choice(["push", "pop", "store", "restore", "trans", "scale", "mult43", "mult33", "mult44",
                             "load43", "mode", "ident", "trans", "mult43"])
            m = rotmat43(rng)
            mi = [int(x) & 0xFFFFFFFF for x in m]
            if md == 0 and op in ("mult43", "mult33", "mult44", "load43", "scale") and rng.random() < 0.7:
                op = "trans"  # keep the projection sane
            if op == "push":
                if md == 0:
                    f.cmd(0x11)
                    pdepth, pstored = 1, True
                elif depth < 31:
                    f.cmd(0x11)
                    depth += 1
            elif op == "pop":
                if md == 0 and pdepth:
                    f.cmd(0x12, rng.randrange(64))
                    pdepth = 0
                elif md in (1, 2) and depth:
                    k = rng.randrange(1, depth + 1) if rng.random() < 0.5 else 1
                    f.cmd(0x12, k)
                    depth -= k
            elif op == "store":
                i = rng.randrange(31)
                f.cmd(0x13, i | rng.randrange(8) << 5)
                if md == 0:
                    pstored = True
                else:
                    stored.add(i)
            elif op == "restore":
                if md == 0 and pstored:
                    f.cmd(0x14, rng.randrange(64))
                elif md != 0 and stored:
                    f.cmd(0x14, rng.choice(sorted(stored)) | rng.randrange(8) << 5)
            elif op == "trans": f.cmd(0x1C, sm(), sm(), sm())
            elif op == "scale": f.cmd(0x1B, *[int(rng.uniform(0.3, 1.5) * 4096) for _ in range(3)])
            elif op == "mult43": f.cmd(0x19, *mi)
            elif op == "mult33": f.cmd(0x1A, *mi[:9])
            elif op == "mult44": f.cmd(0x18, mi[0], mi[1], mi[2], 0, mi[3], mi[4], mi[5], 0, mi[6], mi[7], mi[8], 0,
                                       mi[9], mi[10], mi[11], 4096)
            elif op == "load43": f.cmd(0x17, *mi)
            elif op == "mode":
                md = rng.choice([0, 1, 2, 2])
                f.cmd(0x10, md)
            elif op == "ident": f.cmd(0x15)
        f.cmd(0x10, 2)
    f.cmd(0x60, 0xBFFF0000 if rng.random() < 0.8 else (rng.randrange(40) | rng.randrange(40) << 8
                                                       | (200 + rng.randrange(55)) << 16 | (150 + rng.randrange(41)) << 24))
    if P(15):
        f.cmd(0x30, rng.randrange(0x8000) | rng.randrange(2) << 15 | rng.randrange(0x8000) << 16)
        f.cmd(0x31, rng.randrange(0x8000) | rng.randrange(2) << 15 | rng.randrange(0x8000) << 16)
        for ln in range(4):
            f.cmd(0x32, pack_normal(rng.uniform(-1, 1), rng.uniform(-1, 1), rng.uniform(-1, 1)) | ln << 30)
            f.cmd(0x33, rng.randrange(0x8000) | ln << 30)
        if rng.random() < 0.5:
            f.cmd(0x34, *[rng.randrange(1 << 32) for _ in range(32)])
    npoly = rng.randrange(1, 7 if P(14) else 5)
    # piece 19: a third of the scenes have no polygons (rear plane / clear image / fog on it alone), a third use
    # pixel-aligned rectangles (the rasterizer's slopes are piece 16's, so post-processing rules show up cleanly)
    if P(19) and rng.random() < 0.2:
        npoly = 0
    for _ in range(npoly):
        # p17 modes 0, 1 only: 2 is p19's (toon / highlight), 3 p18's (a shadow polygon draws only over a mask)
        mode = rng.choice([0, 1]) if P(17) else (rng.randrange(3) if P(19) else (rng.choice([0, 0, 0, 3]) if P(18) else 0))
        alpha = rng.randrange(0, 32) if (P(18) or P(19) or (P(17) and rng.random() < 0.3)) and rng.random() < 0.6 else 31
        if P(19) and rng.random() < 0.8:
            alpha = 31  # translucency is piece 18's; keep a few translucent polygons for fog / edge / AA interplay
        if P(16) and rng.random() < 0.15:
            alpha = 0
        lights = rng.randrange(16) if P(15) else 0
        cull = rng.choice([3, 3, 3, 0, 1, 2]) if P(14) else 3
        pid = rng.randrange(64) if P(18) or P(19) else rng.randrange(4)
        attr = lights | mode << 4 | cull << 6 | alpha << 16 | pid << 24 | rng.randrange(2) << 11 \
            | (rng.random() < 0.5) << 12 | (rng.random() < 0.5) << 13 | (rng.random() < 0.3) << 14 | rng.randrange(2) << 15
        if P(14):
            attr &= ~(1 << 11 | 1 << 15)  # translucent depth / fog belong to pieces 18 and 19
        f.cmd(0x29, attr)
        if textured:
            fmt = rng.choice([1, 6]) if P(18) else rng.randrange(1, 8)  # p18: only the translucent formats
            # TEXIMAGE_PARAM (GBATEK 47): 0-15 offset/8, 16-17 repeat S/T, 18-19 flip S/T, 20-22 size S, 23-25 size T,
            # 26-28 format, 29 colour 0 transparent, 30-31 texcoord transform
            tp = (rng.randrange(0x10000) if rng.random() < 0.3 else rng.randrange(0x400)) | rng.randrange(16) << 16                 | rng.randrange(8) << 20 | rng.randrange(8) << 23 | fmt << 26 | rng.randrange(2) << 29                 | (rng.randrange(4) << 30 if P(15) else 0)
            pltt = rng.randrange(0x200) if rng.random() < 0.7 else rng.randrange(0x2000)
            f.cmd(0x2A, tp), f.cmd(0x2B, pltt)
            desc.append("attr=%08X tex fmt=%d tp=%08X pltt=%03X" % (attr, fmt, tp, pltt))
        f.cmd(0x20, rng.randrange(0x8000))
        if P(17):
            # an axis-aligned rectangle on whole pixels with a constant z and colour, texture coordinates changing by
            # a whole 1/16 texel step per pixel with the vertex order below: the rasterizer's interpolation is then
            # exact, and what differs from the oracle is the texturing (p17) alone
            x0, y0 = rng.randrange(0, 240), 3 * rng.randrange(0, 60)
            wd, ht = min(rng.randrange(8, 200), 256 - x0), min(3 * rng.randrange(3, 40), (192 - y0) // 3 * 3)
            kx, ky = rng.choice([0, 1, 2, 4, 8, 16, 16, 16, 32, -16, -8]), rng.choice([0, 4, 8, 16, 16, 16, 32, -16])
            s0, t0 = rng.randrange(-300 * 16, 300 * 16), rng.randrange(-300 * 16, 300 * 16)
            z = v16(rng.uniform(-0.5, 0.5))
            desc.append("rect x%d y%d %dx%d s0=%d t0=%d k=%d,%d" % (x0, y0, wd, ht, s0, t0, kx, ky))
            f.cmd(0x40, 1)
            for (px, py) in ((x0, y0), (x0 + wd, y0), (x0 + wd, y0 + ht), (x0, y0 + ht)):
                f.cmd(0x22, ((s0 + kx * (px - x0)) & 0xFFFF) | ((t0 + ky * (py - y0)) & 0xFFFF) << 16)
                f.cmd(0x23, v16((px - 128) / 128) | v16((96 - py) / 96) << 16, z)
            f.cmd(0x41)
            continue
        if P(15):
            f.cmd(0x21, pack_normal(rng.uniform(-1, 1), rng.uniform(-1, 1), rng.uniform(-1, 1)))
        kind = rng.choice([0, 1, 2, 3]) if P(14) else rng.choice([0, 0, 1])
        if rect:
            kind = 1
        f.cmd(0x40, kind)
        nv = {0: 3, 1: 4, 2: rng.randrange(3, 7), 3: 4 + 2 * rng.randrange(0, 3)}[kind]
        cx, cy = rng.uniform(-0.7, 0.7), rng.uniform(-0.7, 0.7)
        rad = rng.uniform(0.2, 0.9) if not P(14) or rng.random() < 0.7 else rng.uniform(1.0, 3.0)
        zc = rng.uniform(-0.5, 0.5) if not persp else rng.uniform(-0.8, 0.8)
        if rect:
            rx0, ry0 = rng.randrange(0, 200), rng.randrange(0, 150)
            rx1, ry1 = rx0 + rng.randrange(3, 56), ry0 + rng.randrange(3, 42)
            rz = v16(zc)
        for k in range(nv):
            if rect:
                px_, py_ = [(rx0, ry0), (rx1, ry0), (rx1, ry1), (rx0, ry1)][k]
                f.cmd(0x23, ((px_ - 128) * 32) & 0xFFFF | (((96 - py_) * 42) & 0xFFFF) << 16, rz & 0xFFFF)
                continue
            if kind in (0, 1):
                a = 2 * math.pi * k / nv + rng.uniform(-0.2, 0.2) if kind == 0 else k * math.pi / 2 + math.pi / 4
                x, y = cx + rad * math.cos(a), cy + rad * math.sin(a)
            else:
                x, y = cx + (k // 2 - 1) * rad * 0.6, cy + (k % 2) * rad
            z = zc + rng.uniform(-0.3, 0.3)
            if rng.random() < 0.3 and not P(19) and not P(18):  # (18, 19: flat colours, the rasterizer's gouraud is piece 16's)
                f.cmd(0x20, rng.randrange(0x8000))
            if textured and rng.random() < 0.9:
                f.cmd(0x22, rng.randrange(128 * 16) | rng.randrange(128 * 16) << 16)
            if P(15) and rng.random() < 0.7:
                f.cmd(0x21, pack_normal(rng.uniform(-1, 1), rng.uniform(-1, 1), rng.uniform(-1, 1)))
            f.cmd(0x23, v16(x) | v16(y) << 16, v16(z))
        f.cmd(0x41)
    swap = rng.randrange(4) if piece in (14, 16) else (rng.randrange(2) if piece == 18 else 0)  # p18: no W-buffer (p16's)
    f.cmd(0x50, swap)
    return "3D piece %d: %d polys persp=%d swap=%d %s" % (piece, npoly, persp, swap, " ".join(desc)), "3d"


GENS = {1: gen_p01, 2: gen_p02, 3: gen_p03, 4: gen_p04, 5: gen_p05, 6: gen_p06, 7: gen_p07, 8: gen_p08, 9: gen_p09,
        10: gen_p10, 11: gen_p11, 12: gen_p12}


def mask_bit15(f):
    """palette entries and the extended / texture palette banks (E-I) without bit 15: melonDS turns a palette
    entry's bit 15 into the green channel's LSB (the known green-lsb quirk), which would drown every other mismatch"""
    for i in range(0, 0x800, 2):
        f.pltt[i + 1] &= 0x7F
    ofs = 0
    for b in range(9):
        if b >= E:
            for i in range(1, BANKSIZE[b], 2):
                f.vram[ofs + i] &= 0x7F
        ofs += BANKSIZE[b]


def drop_melonds_quirks(f):
    """the maintainers' notes: the cases where melonDS alone differs from NooDS and DeSmuME (verdict b) and the game
    never uses the feature (vcs_features on the 56 frames; the corpus has POWCNT1 = 820Fh/020Fh, no window, no
    bitmap OBJ, no display mode 0), so the fuzz does not score vcore on them. Post-filter: no random numbers are
    drawn, so the scenes of every other seed stay the same. VCORE_GEN_QUIRKS=1 keeps them."""
    if os.environ.get("VCORE_GEN_QUIRKS"):
        return
    # q10: an engine powered off in POWCNT1 (bits 1, 9): melonDS shows black / white, NooDS and DeSmuME draw it
    f.w16(0x304, struct.unpack_from("<H", f.io9, 0x304)[0] | 0x202)
    for e in (0, 1):
        io = f.io9b if e else f.io9
        dc = struct.unpack_from("<I", io, 0)[0]
        # display mode 0 (white): melonDS ignores MASTER_BRIGHT, NooDS and DeSmuME apply it
        if ((dc >> 16) & (1 if e else 3)) == 0:
            struct.pack_into("<H", io, MASTER_BRIGHT, 0)
        # 1D bitmap OBJ mapping with DISPCNT bit 5 set (GBATEK: must be 0): melonDS draws no bitmap OBJ, the others do
        if dc & 1 << 6 and dc & 1 << 5:
            struct.pack_into("<I", io, 0, dc & ~(1 << 5))
        # window Y start in lines 1-6: melonDS's line-0 event order (q08); NooDS and DeSmuME start the window at its line
        for o in (WIN0V, WIN1V):
            v = struct.unpack_from("<H", io, o)[0]
            if 1 <= v >> 8 <= 6:
                struct.pack_into("<H", io, o, v & 0xFF)


def gen(piece, rng, green_lsb=False):
    f = Frame("fuzz")
    f.pltt[:] = rng.randbytes(0x800)
    if piece in GENS:
        d, kind = GENS[piece](rng, f)
    elif 13 <= piece <= 19:
        d, kind = gen_3d(piece, rng, f)
    else:
        raise SystemExit("no generator for piece %d (piece 20: use tools/vcore_compare.py corpus)" % piece)
    if not green_lsb:
        mask_bit15(f)
    drop_melonds_quirks(f)
    return f, d + "\n" + reg_desc(f), kind
