#!/usr/bin/env python3
"""hwfit: the hardware fitter's probe side (docs/vcore.md, tools/hwfit/README.md).

    hwfit.py gen   --n 3000 --seed 1 [--dir D]   random synthetic .vcs scenes -> D/vcs/NNNNN.vcs
    hwfit.py render [--dir D]    run the melonDS oracle (black box) and vcore_render --geo on every probe once; cached
                                 in D/png/NNNNN.png (the 3D layer, RGBA) and D/geo/NNNNN.txt
    hwfit.py pack  [--dir D]     D/probes.bin for the candidate harness (hwfit_raster.c)
    hwfit.py all   ...           gen + render + pack

Probe kind raster: one triangle with random vertex colours, screen positions, depths and w values.
  --wmode flat|persp|mixed   flat: equal w at every vertex (no perspective); persp: w differs per vertex
  --maxsize N                triangle bounding box up to N pixels
Each probe is a complete .vcs frame (3D layer on BG0, no texture, no lighting, no blending, no edge marking, no
antialiasing, alpha 31) rendered through the oracle only as a black box.
"""
import argparse
import os
import random
import struct
import subprocess
import sys
from multiprocessing import Pool
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
sys.path.insert(0, str(ROOT / "port/vcore/tools"))
sys.path.insert(0, "/root/ref/oracle")
import mkvcs  # noqa: E402
from vcs import read_png  # noqa: E402

ORACLE = "/root/ref/oracle/oracle"
VCORE_RENDER = str(ROOT / "build/vcore/vcore_render")


def scene(p):
    """a mkvcs.Frame with probe p's triangle (p: dict with sx, sy, zz, col[3x3], k)"""
    f = mkvcs.Frame("hwfit")
    f.w32(0x000, f.r32(0x000) | 1 << 3 | 1 << 8)  # BG0 = 3D
    f.w16(0x060, 0)
    f.w32(0x350, 0)
    f.w16(0x354, 0x7FFF)
    one = 0x1000
    # row-vector projection: w' = z * k + 1.0 (matrix index 11 = row 2, column 3), clip z = z
    proj = [one, 0, 0, 0, 0, one, 0, 0, 0, 0, one, int(p["k"] * one), 0, 0, 0, one]
    f.cmd(0x10, 0)
    f.cmd(0x16, *proj)
    f.cmd(0x10, 2)
    f.cmd(0x15)
    f.cmd(0x60, 0xBFFF0000)
    f.cmd(0x29, 0x001F00C0)
    f.cmd(0x2A, 0)
    f.cmd(0x40, 0)
    for i in range(3):
        r, g, b = p["col"][i]
        f.cmd(0x20, r | g << 5 | b << 10)
        w = 1.0 + p["k"] * p["zz"][i]
        x = (p["sx"][i] / 128.0 - 1.0) * w
        y = (1.0 - p["sy"][i] / 96.0) * w
        xi, yi, zi = (int(round(v * one)) & 0xFFFF for v in (x, y, p["zz"][i]))
        f.cmd(0x23, xi | yi << 16, zi)
    f.cmd(0x41)
    f.cmd(0x50, 0)
    return f


def make_vgrad(rng, maxsize):
    """apex above a flat bottom whose two corners share a colour: the colour depends on the row only (the edge rule);
    both edges are Y-major (|dx| <= dy)"""
    dy = rng.choice([3, 5, 8, 12, 17, 24, 31, 40, 55, 77, 100, 140])
    ax = rng.randrange(110, 146)
    ay = rng.randrange(6, 186 - dy)
    r1, r2 = rng.randrange(1, dy + 1), rng.randrange(1, dy + 1)
    r1, r2 = min(r1, ax - 2), min(r2, 253 - ax)
    pts = [(ax, ay), (ax + r2, ay + dy), (ax - r1, ay + dy)]
    c0 = tuple(rng.randrange(32) for _ in range(3))
    c1 = tuple(rng.randrange(32) for _ in range(3))
    zz = [0.3] * 3
    return {"sx": [p[0] for p in pts], "sy": [p[1] for p in pts], "zz": zz, "k": 0.0, "col": [c0, c1, c1]}


def make_probe(rng, wmode, maxsize):
    if wmode == "vgrad":
        return make_vgrad(rng, maxsize)
    size = rng.choice([6, 10, 16, 24, 40, 64, 100, maxsize])
    size = max(4, min(size, maxsize))
    x0 = rng.randrange(4, 252 - size)
    y0 = rng.randrange(4, 188 - size)
    pts = [(x0 + rng.randrange(size + 1), y0 + rng.randrange(size + 1)) for _ in range(3)]
    k = 0.0
    zz = [rng.random() * 0.9 for _ in range(3)]  # depth varies; with k = 0, w stays 1
    if wmode == "persp" or (wmode == "mixed" and rng.random() < 0.5):
        k = rng.choice([0.5, 1.0, 2.0, 4.0, 7.0])
    mode = rng.random()
    cols = []
    for i in range(3):
        if mode < 0.6:
            cols.append(tuple(rng.randrange(32) for _ in range(3)))
        else:  # extremes and a free value: clean ramps
            cols.append(tuple(rng.choice([0, 31, rng.randrange(32)]) for _ in range(3)))
    return {"sx": [p[0] for p in pts], "sy": [p[1] for p in pts], "zz": zz, "k": k, "col": cols}


def cmd_gen(a):
    d = Path(a.dir)
    (d / "vcs").mkdir(parents=True, exist_ok=True)
    rng = random.Random(a.seed)
    for i in range(a.n):
        p = make_probe(rng, a.wmode, a.maxsize)
        scene(p).write(str(d / "vcs" / f"{i:05d}.vcs"))
        (d / "vcs" / f"{i:05d}.txt").write_text(repr(p))
    print(f"gen: {a.n} probes in {d}/vcs")


def _render_one(args):
    d, name = args
    v = d / "vcs" / name
    png = d / "png" / (name[:-4] + ".png")
    geo = d / "geo" / (name[:-4] + ".txt")
    if not png.exists():
        subprocess.run([ORACLE, str(v), "/dev/null", "--3d", str(png), "-q"], stderr=subprocess.DEVNULL,
                       stdout=subprocess.DEVNULL)
    if not geo.exists():
        subprocess.run([VCORE_RENDER, str(v), "/dev/null", "--geo", str(geo)], stderr=subprocess.DEVNULL,
                       stdout=subprocess.DEVNULL)
    return 1


def cmd_render(a):
    d = Path(a.dir)
    (d / "png").mkdir(exist_ok=True)
    (d / "geo").mkdir(exist_ok=True)
    names = sorted(x.name for x in (d / "vcs").glob("*.vcs"))
    jobs = int(os.environ.get("BW_JOBS") or 0) or os.cpu_count() or 4
    with Pool(jobs) as pool:
        n = sum(pool.imap_unordered(_render_one, [(d, n) for n in names], chunksize=8))
    print(f"render: {n} probes")


def parse_geo(path):
    vs = []
    for ln in Path(path).read_text().splitlines():
        t = ln.split()
        if t and t[0] == "v":
            # v SX SY z HEX24 w W rgb R G B st S T clip X Y Z W
            vs.append((int(t[1]), int(t[2]), int(t[4], 16), int(t[6]), int(t[8]), int(t[9]), int(t[10])))
    return vs


def cmd_pack(a):
    d = Path(a.dir)
    out = bytearray()
    n = 0
    for png in sorted((d / "png").glob("*.png")):
        geo = d / "geo" / (png.stem + ".txt")
        if not geo.exists():
            continue
        vs = parse_geo(geo)
        if len(vs) != 3:
            continue
        w, h, ch, px = read_png(str(png))
        assert (w, h, ch) == (256, 192, 4)
        ys, xs = [], []
        for y in range(h):
            for x in range(w):
                if px[(y * w + x) * 4 + 3]:
                    xs.append(x)
                    ys.append(y)
        if not xs:
            continue
        x0, x1, y0, y1 = min(xs), max(xs), min(ys), max(ys)
        bw, bh = x1 - x0 + 1, y1 - y0 + 1
        out += struct.pack("<7i", int(png.stem), 3, x0, y0, bw, bh, 0)
        for (sx, sy, z, wv, r, g, b) in vs:
            out += struct.pack("<7i", sx, sy, z, wv, r, g, b)
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                o = (y * w + x) * 4
                al = px[o + 3]
                if al == 0:
                    out += struct.pack("<I", 0)
                else:
                    a5 = (al * 31 + 127) // 255
                    out += struct.pack("<I", (px[o] >> 2) | (px[o + 1] >> 2) << 8 | (px[o + 2] >> 2) << 16 | a5 << 24)
        n += 1
    (d / "probes.bin").write_bytes(struct.pack("<i", n) + bytes(out))
    print(f"pack: {n} probes -> {d}/probes.bin ({len(out)} bytes)")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("cmd", choices=["gen", "render", "pack", "all"])
    ap.add_argument("--dir", default="/root/mt/f54a/hwfit/raster")
    ap.add_argument("--n", type=int, default=3000)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--wmode", default="mixed", choices=["flat", "persp", "mixed", "vgrad"])
    ap.add_argument("--maxsize", type=int, default=100)
    a = ap.parse_args()
    if a.cmd in ("gen", "all"):
        cmd_gen(a)
    if a.cmd in ("render", "all"):
        cmd_render(a)
    if a.cmd in ("pack", "all"):
        cmd_pack(a)


if __name__ == "__main__":
    main()
