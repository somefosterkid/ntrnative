#!/usr/bin/env python3
"""Synthetic .vcs frames for port/vcore and the melonDS oracle (docs/vcore.md): known state, no game data.

    python3 port/vcore/tools/mkvcs.py SCENE OUT.vcs        SCENE: blank textbg obj tri mixed
    python3 port/vcore/tools/mkvcs.py --all DIR            every scene as DIR/synth_SCENE.vcs

blank:  both engines on, nothing enabled: the backdrop colours (A dark blue, B dark green)
textbg: engine A BG0, a 4bpp text BG of one tile (a red square with a white diagonal) over the backdrop
obj:    engine A, one 16x16 4bpp OBJ (green) at (60, 40), 1D tile mapping
tri:    engine A BG0 = 3D: one flat white triangle, identity matrices, the full viewport
mixed:  tri + the text BG on BG1 behind it + the OBJ, and engine B a text BG0
The format: /root/ref/VCS_FORMAT.md (VCST v1, chunks INFO IO9 IO9B VRAM PLTT OAM GX3D).
"""
import struct
import sys
from pathlib import Path

BANK_OFS = [0x00000, 0x20000, 0x40000, 0x60000, 0x80000, 0x90000, 0x94000, 0x98000, 0xA0000]


class Frame:
    def __init__(self, name):
        self.name = name
        self.io9 = bytearray(0x700)
        self.io9b = bytearray(0x70)
        self.vram = bytearray(0xA4000)
        self.pltt = bytearray(0x800)
        self.oam = bytearray(0x800)
        self.gx = []
        # both engines and the 3D on, engine A on the top screen (POWCNT1)
        self.w16(0x304, 0x820F)
        self.w32(0x000, 0x00010000)  # engine A: display mode 1
        self.w32(0x000, 0x00010000, b=True)
        self.pal(0, 0, 0, 0x2800)  # A backdrop: dark blue
        self.pal(1, 0, 0, 0x0140)  # B backdrop: dark green

    def w8(self, ofs, v):
        self.io9[ofs] = v

    def w16(self, ofs, v, b=False):
        struct.pack_into("<H", self.io9b if b else self.io9, ofs, v)

    def w32(self, ofs, v, b=False):
        struct.pack_into("<I", self.io9b if b else self.io9, ofs, v)

    def r32(self, ofs, b=False):
        return struct.unpack_from("<I", self.io9b if b else self.io9, ofs)[0]

    def pal(self, e, obj, idx, c):
        struct.pack_into("<H", self.pltt, e * 0x400 + obj * 0x200 + idx * 2, c)

    def bank(self, b, ofs, data):
        a = BANK_OFS[b] + ofs
        self.vram[a:a + len(data)] = data

    def cmd(self, op, *params):
        self.gx.append(op | len(params) << 8)
        self.gx.extend(p & 0xFFFFFFFF for p in params)

    def write(self, path):
        info = f"source=mkvcs\nscene={self.name}\ng3_commands={self.count()}\n".encode()
        out = bytearray(b"VCST" + struct.pack("<I", 1))
        gx = b"".join(struct.pack("<I", w) for w in self.gx)
        for tag, data in ((b"INFO", info), (b"IO9 ", self.io9), (b"IO9B", self.io9b), (b"VRAM", self.vram),
                          (b"PLTT", self.pltt), (b"OAM ", self.oam), (b"GX3D", gx)):
            out += tag + struct.pack("<I", len(data)) + data + b"\0" * (-len(data) % 4)
        Path(path).write_bytes(out)

    def count(self):
        n, i = 0, 0
        while i < len(self.gx):
            i += 1 + ((self.gx[i] >> 8) & 0xFF)
            n += 1
        return n


def tile4(rows):
    """an 8x8 4bpp tile from 8 strings of 8 hex digits (left pixel first)"""
    out = bytearray()
    for r in rows:
        for i in range(0, 8, 2):
            out.append(int(r[i], 16) | int(r[i + 1], 16) << 4)
    return out


DIAG = tile4(["21111111", "12111111", "11211111", "11121111", "11112111", "11111211", "11111121",
              "11111112"])


def text_bg(f, e, n, prio=0):
    """BG n of engine e: a 4bpp 256x256 text BG, chars at 16 KB, map at 0, every entry tile 1"""
    if e == 0:
        f.w8(0x240, 0x81)  # bank A -> engine A BG 0x06000000
        bank = 0
    else:
        f.w8(0x242, 0x84)  # bank C -> engine B BG 0x06200000
        bank = 2
    f.w16(0x008 + 2 * n, (1 << 2) | prio, b=e == 1)  # char base 16 KB, screen base 0, 4bpp, 256x256
    f.bank(bank, 0x4000 + 32, DIAG)
    f.bank(bank, 0, struct.pack("<H", 1) * 1024)
    f.pal(e, 0, 1, 0x001F)  # red
    f.pal(e, 0, 2, 0x7FFF)  # white
    f.w32(0x000, f.r32(0x000, b=e == 1) | 1 << (8 + n), b=e == 1)


def obj(f):
    f.w8(0x241, 0x82)  # bank B -> engine A OBJ 0x06400000
    f.bank(1, 0, tile4(["11111111"] * 8) * 4)
    f.pal(0, 1, 1, 0x03E0)  # green
    struct.pack_into("<HHH", f.oam, 0, 40, 60 | 1 << 14, 0)  # 16x16, tile 0
    for i in range(1, 128):  # the others hidden
        struct.pack_into("<H", f.oam, i * 8, 1 << 9)
    f.w32(0x000, f.r32(0x000) | 1 << 12 | 1 << 4)  # OBJ on, 1D tile mapping (32-byte boundary)


def tri(f):
    f.w32(0x000, f.r32(0x000) | 1 << 3 | 1 << 8)  # BG0 = 3D, on
    f.w16(0x060, 0)  # DISP3DCNT: no texture, no blending
    f.w32(0x350, 0)  # clear colour transparent
    f.w16(0x354, 0x7FFF)  # clear depth
    one = 0x1000
    ident = [one, 0, 0, 0, 0, one, 0, 0, 0, 0, one, 0, 0, 0, 0, one]
    f.cmd(0x10, 0)          # MTX_MODE projection
    f.cmd(0x16, *ident)     # MTX_LOAD_4x4
    f.cmd(0x10, 2)          # position + vector
    f.cmd(0x15)             # MTX_IDENTITY
    f.cmd(0x60, 0xBFFF0000)  # VIEWPORT 0,0 - 255,191
    f.cmd(0x29, 0x001F00C0)  # POLYGON_ATTR: both faces, alpha 31
    f.cmd(0x2A, 0)          # no texture
    f.cmd(0x20, 0x7FFF)     # COLOR white
    f.cmd(0x40, 0)          # BEGIN_VTXS triangles

    def v16(x, y, z):
        f.cmd(0x23, (x & 0xFFFF) | (y & 0xFFFF) << 16, z & 0xFFFF)
    v16(-0x800, -0x800, 0)
    v16(0x800, -0x800, 0)
    v16(0, 0x800, 0)
    f.cmd(0x41)             # END_VTXS
    f.cmd(0x50, 0)          # SWAP_BUFFERS


def scene(name):
    f = Frame(name)
    if name == "textbg":
        text_bg(f, 0, 0)
    elif name == "obj":
        obj(f)
    elif name == "tri":
        tri(f)
    elif name == "mixed":
        tri(f)
        text_bg(f, 0, 1, prio=1)
        obj(f)
        text_bg(f, 1, 0)
    elif name != "blank":
        sys.exit(f"mkvcs: unknown scene {name}")
    return f


SCENES = ["blank", "textbg", "obj", "tri", "mixed"]


def main():
    a = sys.argv[1:]
    if len(a) == 2 and a[0] == "--all":
        Path(a[1]).mkdir(parents=True, exist_ok=True)
        for s in SCENES:
            scene(s).write(f"{a[1]}/synth_{s}.vcs")
            print(f"{a[1]}/synth_{s}.vcs")
    elif len(a) == 2:
        scene(a[0]).write(a[1])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
