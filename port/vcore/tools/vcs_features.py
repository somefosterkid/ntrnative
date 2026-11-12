#!/usr/bin/env python3
"""The video features a .vcs frame uses, read from its own state (registers, OAM, VRAMCNT, the 3D command stream):
the feature vocabulary of docs/vcore.md's piece specs.

    python3 port/vcore/tools/vcs_features.py FILE.vcs ...        one line a file: name, then its features
    python3 port/vcore/tools/vcs_features.py DIR [--index OUT]   every DIR/*.vcs; --index writes the lines as an
                                                                 index.txt for tools/vcore_compare.py
    python3 port/vcore/tools/vcs_features.py --list              the vocabulary with what each feature means
A feature is on when the frame's state makes the hardware use it on either engine (a layer enabled and present in
the BG mode, an OBJ on screen, a polygon drawn with the attribute). Pure Python, no game data.
"""
import struct
import sys
from pathlib import Path

VOCAB = {
    "bg.text": "a text BG (piece 1)", "bg.text.8bpp": "a 256-colour text BG", "bg.text.extpal": "8bpp text BG with "
    "extended palettes", "bg.affine": "an affine BG (2)", "bg.ext.map16": "an extended BG, affine 16-bit map (3)",
    "bg.ext.bmp256": "an extended BG, 256-colour bitmap (3)", "bg.ext.direct": "an extended BG, direct colour (3)",
    "bg.large": "the large bitmap BG, mode 6 (4)", "bg.mosaic": "a BG with mosaic (4)",
    "obj.normal": "a tile OBJ on screen (5)", "obj.8bpp": "a 256-colour tile OBJ", "obj.extpal": "OBJ extended "
    "palettes on", "obj.1d": "1D tile mapping", "obj.2d": "2D tile mapping", "obj.affine": "an affine OBJ (6)",
    "obj.double": "a double-size affine OBJ (6)", "obj.bitmap": "a bitmap OBJ (7)", "obj.window": "an OBJ-window "
    "OBJ (7)", "obj.mosaic": "an OBJ with mosaic (7)", "obj.semi": "a semi-transparent OBJ (5, 9)",
    "win.0": "window 0 (8)", "win.1": "window 1 (8)", "win.obj": "the OBJ window on (8)",
    "blend.alpha": "BLDCNT alpha blending (9)", "blend.bright": "BLDCNT brighten (9)", "blend.dark": "BLDCNT "
    "darken (9)", "bright.master": "MASTER_BRIGHT up or down (10)", "disp.off": "display mode 0 (10)",
    "disp.vram": "display mode 2, VRAM (10)", "disp.mmem": "display mode 3, main memory (10)", "disp.blank":
    "forced blank (10)", "disp.swap": "engine B on the top screen", "capture": "display capture this frame (11)",
    "vram.extpal": "a bank mapped as extended palettes (12)", "vram.overlap": "two banks mapped on one page (12)",
    "vram.tex": "a bank in a texture slot (12)",
    "3d": "the 3D layer shown with polygons (13-20)", "3d.mtxstack": "matrix push/pop/store/restore (13)",
    "3d.strip": "triangle or quad strips (14)", "3d.quad": "quads (14)", "3d.cull": "a culling setting other than "
    "both faces (14)", "3d.light": "lit polygons (15)", "3d.texgen": "texture coordinate generation (15)",
    "3d.tex": "textured polygons (17)", "3d.tex.a3i5": "format 1", "3d.tex.pal4": "format 2", "3d.tex.pal16":
    "format 3", "3d.tex.pal256": "format 4", "3d.tex.4x4": "format 5", "3d.tex.a5i3": "format 6",
    "3d.tex.direct": "format 7", "3d.translucent": "translucent polygons (18)", "3d.shadow": "shadow polygons (18)",
    "3d.decal": "decal mode (17)", "3d.toon": "toon / highlight mode (19)", "3d.alphatest": "the alpha test (18)",
    "3d.blend": "3D alpha blending on (18)", "3d.edge": "edge marking (19)", "3d.fog": "fog (19)", "3d.aa":
    "antialiasing (19)", "3d.clearimage": "the rear-plane bitmap (19)", "3d.wbuffer": "w-buffering (16)",
}

OBJ_SIZE = {(0, 0): (8, 8), (0, 1): (16, 16), (0, 2): (32, 32), (0, 3): (64, 64), (1, 0): (16, 8), (1, 1): (32, 8),
            (1, 2): (32, 16), (1, 3): (64, 32), (2, 0): (8, 16), (2, 1): (8, 32), (2, 2): (16, 32), (2, 3): (32, 64)}
BG_KIND = [[1, 1, 1, 1], [1, 1, 1, 2], [1, 1, 2, 2], [1, 1, 1, 3], [1, 1, 2, 3], [1, 1, 3, 3], [5, 0, 4, 0],
           [0, 0, 0, 0]]


def chunks(path):
    d = Path(path).read_bytes()
    if d[:4] != b"VCST":
        raise ValueError(f"{path}: not a .vcs file")
    out, at = {}, 8
    while at + 8 <= len(d):
        tag, n = d[at:at + 4].decode("latin-1"), struct.unpack_from("<I", d, at + 4)[0]
        out[tag.strip()] = d[at + 8:at + 8 + n]
        at = (at + 8 + n + 3) & ~3
    return out


def features(path):
    c = chunks(path)
    io9 = c.get("IO9", bytes(0x700)).ljust(0x700, b"\0")
    io9b = c.get("IO9B", bytes(0x70)).ljust(0x70, b"\0")
    oam = c.get("OAM", bytes(0x800)).ljust(0x800, b"\0")
    gx = c.get("GX3D", b"")
    f = set()
    r16 = lambda io, o: struct.unpack_from("<H", io, o)[0]
    r32 = lambda io, o: struct.unpack_from("<I", io, o)[0]
    powcnt = r16(io9, 0x304)
    if not powcnt & 0x8000:
        f.add("disp.swap")
    three_d_shown = False
    for e, io in ((0, io9), (1, io9b)):
        dc = r32(io, 0)
        dmode = (dc >> 16) & (3 if e == 0 else 1)
        if dmode == 0:
            f.add("disp.off")
            continue
        if dmode == 2:
            f.add("disp.vram")
        if dmode == 3:
            f.add("disp.mmem")
        if dc & 0x80:
            f.add("disp.blank")
            continue
        mb = r16(io, 0x6C)
        if (mb >> 14) in (1, 2) and mb & 0x1F:
            f.add("bright.master")
        if dmode != 1:
            continue
        mosaic = r16(io, 0x4C)
        for n in range(4):
            if not dc & (1 << (8 + n)):
                continue
            kind = BG_KIND[dc & 7][n]
            if n == 0 and dc & 8:
                kind = 5 if e == 0 else kind
            elif kind == 5:
                kind = 0
            cnt = r16(io, 8 + 2 * n)
            if kind == 1:
                f.add("bg.text")
                if cnt & 0x80:
                    f.add("bg.text.8bpp")
                    if dc & (1 << 30):
                        f.add("bg.text.extpal")
            elif kind == 2:
                f.add("bg.affine")
            elif kind == 3:
                f.add("bg.ext.map16" if not cnt & 0x80 else ("bg.ext.direct" if cnt & 4 else "bg.ext.bmp256"))
            elif kind == 4 and e == 0:
                f.add("bg.large")
            elif kind == 5:
                three_d_shown = True
            if kind in (1, 2, 3, 4) and cnt & 0x40 and mosaic & 0xFF:
                f.add("bg.mosaic")
        if dc & 0x2000:
            f.add("win.0")
        if dc & 0x4000:
            f.add("win.1")
        if dc & 0x8000:
            f.add("win.obj")
        bld = r16(io, 0x50)
        mode = (bld >> 6) & 3
        if mode == 1:
            f.add("blend.alpha")
        elif mode == 2 and r16(io, 0x54) & 0x1F:
            f.add("blend.bright")
        elif mode == 3 and r16(io, 0x54) & 0x1F:
            f.add("blend.dark")
        if dc & 0x1000:
            for i in range(128):
                a0, a1, a2 = struct.unpack_from("<HHH", oam, e * 0x400 + i * 8)
                aff = a0 & 0x100
                if not aff and a0 & 0x200:
                    continue
                w, h = OBJ_SIZE.get((a0 >> 14, a1 >> 14), (0, 0))
                if w == 0:
                    continue
                if aff and a0 & 0x200:
                    w, h = w * 2, h * 2
                y, x = a0 & 0xFF, a1 & 0x1FF
                if not (y < 192 or y + h > 256) or not (x < 256 or x + w > 512):
                    continue
                m = (a0 >> 10) & 3
                if m == 3:
                    f.add("obj.bitmap")
                else:
                    f.add("obj.normal")
                    f.add("obj.1d" if dc & 0x10 else "obj.2d")
                    if a0 & 0x2000:
                        f.add("obj.8bpp")
                        if dc & (1 << 31):
                            f.add("obj.extpal")
                if m == 1:
                    f.add("obj.semi")
                if m == 2:
                    f.add("obj.window")
                if aff:
                    f.add("obj.affine")
                    if a0 & 0x200:
                        f.add("obj.double")
                if a0 & 0x1000 and mosaic & 0xFF00:
                    f.add("obj.mosaic")
    cap = r32(io9, 0x64)
    if cap & 0x80000000:
        f.add("capture")
    # VRAM banks: extended palettes, texture slots, overlaps (GBATEK 22, coarse: the MST values)
    pages = {}
    for b, ofs in enumerate((0x240, 0x241, 0x242, 0x243, 0x244, 0x245, 0x246, 0x248, 0x249)):
        v = io9[ofs]
        if not v & 0x80:
            continue
        mst, o = v & 7, (v >> 3) & 3
        if (b == 4 and mst == 4) or (b in (5, 6) and mst in (4, 5)) or (b == 7 and mst == 2) or (b == 8 and mst == 3):
            f.add("vram.extpal")
        if b <= 3 and mst == 3:
            f.add("vram.tex")
        key = (mst if b <= 6 else 10 + b * 4 + mst, o if b in (0, 1, 2, 3, 5, 6) else 0)
        if mst != 0 and b not in (7, 8):
            pages.setdefault(key, []).append(b)
    if any(len(v) > 1 for v in pages.values()):
        f.add("vram.overlap")
    # the 3D stream
    dc3 = r16(io9, 0x60)
    attr, tex, npoly, i = 0, 0, 0, 0
    words = [struct.unpack_from("<I", gx, k)[0] for k in range(0, len(gx) - 3, 4)]
    while i < len(words):
        op, n = words[i] & 0xFF, (words[i] >> 8) & 0xFF
        p = words[i + 1:i + 1 + n]
        i += 1 + n
        if op in (0x11, 0x12, 0x13, 0x14) and not (words[i - 1 - n] >> 16) & 1:
            f.add("3d.mtxstack")
        elif op == 0x29 and p:
            attr = p[0]
        elif op == 0x2A and p:
            tex = p[0]
        elif op == 0x40 and p:
            npoly += 1
            if p[0] & 3 in (2, 3):
                f.add("3d.strip")
            if p[0] & 3 in (1, 3):
                f.add("3d.quad")
            if attr & 0xF:
                f.add("3d.light")
            if (attr >> 6) & 3 != 3:
                f.add("3d.cull")
            pm = (attr >> 4) & 3
            f.add({1: "3d.decal", 2: "3d.toon", 3: "3d.shadow"}.get(pm, "3d"))
            alpha = (attr >> 16) & 0x1F
            fmt = (tex >> 26) & 7
            if 0 < alpha < 31 or fmt in (1, 6):
                f.add("3d.translucent")
            if fmt and dc3 & 1:
                f.add("3d.tex")
                f.add("3d.tex." + ["", "a3i5", "pal4", "pal16", "pal256", "4x4", "a5i3", "direct"][fmt])
                if tex >> 30:
                    f.add("3d.texgen")
        elif op == 0x50 and p and p[0] & 2:
            f.add("3d.wbuffer")
    if not (three_d_shown and npoly):
        f = {x for x in f if not x.startswith("3d")}
    else:
        f.add("3d")
        for bit, name in ((2, "3d.alphatest"), (3, "3d.blend"), (4, "3d.aa"), (5, "3d.edge"), (7, "3d.fog"),
                          (14, "3d.clearimage")):
            if dc3 & (1 << bit):
                f.add(name)
    return sorted(f)


def main():
    a = sys.argv[1:]
    if not a:
        sys.exit(__doc__)
    if a == ["--list"]:
        for k, v in VOCAB.items():
            print(f"{k:18s} {v}")
        return
    index = None
    if "--index" in a:
        k = a.index("--index")
        index = a[k + 1]
        a = a[:k] + a[k + 2:]
    files = []
    for x in a:
        files += sorted(Path(x).glob("*.vcs")) if Path(x).is_dir() else [Path(x)]
    lines = [f"{p.stem} {' '.join(features(p))}" for p in files]
    if index:
        Path(index).write_text("# frame features (port/vcore/tools/vcs_features.py)\n" + "\n".join(lines) + "\n")
    print("\n".join(lines))


if __name__ == "__main__":
    main()
