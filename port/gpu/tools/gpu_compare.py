#!/usr/bin/env python3
"""gpu_compare: the GPU path's 3D layer against vcore's (docs/gpu.md, "Testing by CPU").

  gpu_compare.py DIR|F.vcs ... [--scale N] [--tol T] [--diff DIR] [--hi DIR] [--show K] [--api es|gl] [-j N]
  gpu_compare.py --piece 13-19 [--fuzz N] [--seed S] [--emit] [...]      vcore_check's scenes, a table per piece

Each scene is rendered by build/vcore/vcore_render --3d (the reference) and build/gpu/gpu_render --3d. Two scores:
  exact   vcore_check's 3D rule: colour at 6 bits where alpha (5 bits) > 0, alpha at 5 bits; % of the pixels
  within  every channel within --tol levels (default 2; colour in 6-bit levels, alpha in 5-bit levels; colour is
          ignored where both alphas are 0)
At --scale N > 1 gpu_render writes the N x layer box-downsampled to 1x (each channel's mean over the N x N block,
alpha too): "within" is then a smoke test (edges differ by design), a drop between commits is the signal; --hi DIR
keeps the N x pictures (NAME.hi.png). --diff DIR writes NAME.diff.png (black = equal, yellow = within tol, red = off).
Exit status 0 always unless a render fails (then 1): the numbers are the output.
"""
import argparse
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile
from multiprocessing import get_context

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, "/root/ref/oracle")
sys.path.insert(0, os.path.join(ROOT, "port", "vcore", "tools"))
VRENDER = os.environ.get("VCORE_RENDER", os.path.join(ROOT, "build/vcore/vcore_render"))
GRENDER = os.environ.get("GPU_RENDER", os.path.join(ROOT, "build/gpu/gpu_render"))
W, H = 256, 192


def run(cmd, **kw):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kw)


def compare(a, b, tol):
    """a, b: RGBA bytes (W*H*4). returns (n exact-bad, n beyond-tol, [(x, y, mine, theirs)], diff rgb bytes)"""
    if a == b:
        return 0, 0, [], None
    bad = far = 0
    firsts = []
    out = bytearray(W * H * 3)
    for i in range(W * H):
        p, q = a[i * 4:i * 4 + 4], b[i * 4:i * 4 + 4]
        if p == q:
            continue
        a5, b5 = (p[3] * 31 + 127) // 255, (q[3] * 31 + 127) // 255
        pr, qr = (p[0] >> 2, p[1] >> 2, p[2] >> 2), (q[0] >> 2, q[1] >> 2, q[2] >> 2)
        if a5 == b5 and (not a5 or pr == qr):
            continue
        bad += 1
        d = abs(a5 - b5)
        if a5 or b5:
            d = max(d, abs(pr[0] - qr[0]), abs(pr[1] - qr[1]), abs(pr[2] - qr[2]))
        if d > tol:
            far += 1
            out[i * 3] = 255
        else:
            out[i * 3], out[i * 3 + 1] = 255, 255
        if len(firsts) < 6:
            firsts.append((i % W, i // W, pr + (a5,), qr + (b5,)))
    return bad, far, firsts, bytes(out)


def edge_mask(a, tol, r):
    """pixels within r pixels (square) of a discontinuity of the reference layer a (RGBA bytes): a 4-neighbour whose
    colour (6 bits, where alpha > 0) or alpha (5 bits) is more than tol levels away. Polygon outlines and texture
    edges: the places where N x and 1x differ by design (sub-pixel position, finer sampling)."""
    q = []
    for i in range(W * H):
        a5 = (a[i * 4 + 3] * 31 + 127) // 255
        q.append((a[i * 4] >> 2, a[i * 4 + 1] >> 2, a[i * 4 + 2] >> 2, a5))
    e = bytearray(W * H)

    def far(p, o):
        d = abs(p[3] - o[3])
        if p[3] or o[3]:
            d = max(d, abs(p[0] - o[0]), abs(p[1] - o[1]), abs(p[2] - o[2]))
        return d > tol
    for y in range(H):
        for x in range(W):
            i = y * W + x
            if x + 1 < W and far(q[i], q[i + 1]):
                e[i] = e[i + 1] = 1
            if y + 1 < H and far(q[i], q[i + W]):
                e[i] = e[i + W] = 1
    if r <= 0:
        return e
    m = bytearray(W * H)
    for y in range(H):
        for x in range(W):
            if e[y * W + x]:
                for yy in range(max(0, y - r), min(H, y + r + 1)):
                    m[yy * W + max(0, x - r):yy * W + min(W, x + r + 1)] = b"\x01" * (min(W, x + r + 1) - max(0, x - r))
    return m


def one(job):
    label, path, piece, seedt, a = job
    from vcs import read_png, write_png
    res = {"label": label, "piece": piece, "bad": 0, "far": 0, "px": W * H, "firsts": [], "desc": ""}
    work = tempfile.mkdtemp(prefix="gpu_compare_")
    try:
        if path is None:
            import vcore_gen
            rng = random.Random("%d/%d/%d" % seedt)
            f, desc, kind = vcore_gen.gen(piece, rng, False)
            path = os.path.join(work, label + ".vcs")
            f.write(path)
            res["desc"] = desc
        v3, g3 = os.path.join(work, "v.3d.png"), os.path.join(work, "g.3d.png")
        r = run([VRENDER, path, os.path.join(work, "v.png"), "--3d", v3])
        if r.returncode:
            res["error"] = "vcore_render: " + r.stderr.decode(errors="replace")[-200:]
            return res
        cmd = [GRENDER, path, os.path.join(work, "g.png"), "--3d", g3, "--api", a.api, "-q"]
        if a.scale != 1:
            cmd += ["--scale", str(a.scale)]
        if a.hi:
            cmd += ["--hi", os.path.join(a.hi, label + ".hi.png")]
        r = run(cmd)
        if r.returncode:
            res["error"] = "gpu_render: " + r.stderr.decode(errors="replace")[-200:]
            return res
        w, h, _, pa = read_png(v3)
        _, _, _, pb = read_png(g3)
        res["bad"], res["far"], res["firsts"], dif = compare(bytes(pa), bytes(pb), a.tol)
        if a.noedge >= 0:
            m = edge_mask(bytes(pa), a.tol, a.noedge)
            res["px_ne"] = W * H - sum(m)
            res["far_ne"] = 0
            if res["far"]:
                for i in range(W * H):
                    if not m[i] and dif[i * 3] and not dif[i * 3 + 1]:
                        res["far_ne"] += 1
        if a.diff and dif is not None:
            write_png(os.path.join(a.diff, label + ".diff.png"), W, H, 3, dif)
    except Exception as e:  # noqa: BLE001
        res["error"] = repr(e)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    return res


def pct(bad, px):
    return 100.0 * (px - bad) / px if px else 0.0


def parse_pieces(s):
    out = []
    for part in s.split(","):
        m = re.fullmatch(r"p?(\d+)(?:-p?(\d+))?", part.strip())
        if not m:
            raise SystemExit("bad --piece " + s)
        lo, hi = int(m.group(1)), int(m.group(2) or m.group(1))
        out += list(range(lo, hi + 1))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("scene", nargs="*", help="directories of .vcs, or .vcs files")
    ap.add_argument("--scale", type=int, default=1)
    ap.add_argument("--tol", type=int, default=2)
    ap.add_argument("--diff", help="write NAME.diff.png here")
    ap.add_argument("--hi", help="write the N x layer, NAME.hi.png, here")
    ap.add_argument("--show", type=int, default=5, help="worst scenes described (default 5)")
    ap.add_argument("--noedge", type=int, default=-1, metavar="R",
                    help="also score without the pixels within R pixels of a discontinuity of vcore's layer (polygon "
                         "outlines, texture edges: where N x differs by design); R = 1 is the usual")
    ap.add_argument("--api", default="es", choices=["es", "gl"])
    ap.add_argument("--piece", help="vcore_check's scenes of these pieces, e.g. 13-19 or p16,p17: fuzz scenes (--fuzz N)")
    ap.add_argument("--fuzz", type=int, default=100)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--emit", action="store_true", help="with --piece: also the unit tests' emitted scenes")
    ap.add_argument("-q", "--quiet", action="store_true", help="no per-scene lines")
    ap.add_argument("-j", "--jobs", type=int, default=int(os.environ.get("BW_JOBS", "0") or 0) or os.cpu_count())
    a = ap.parse_args()
    for d in (a.diff, a.hi):
        if d:
            os.makedirs(d, exist_ok=True)
    jobs = []
    tmp = None
    if a.piece:
        for piece in parse_pieces(a.piece):
            if a.emit:
                tmp = tmp or tempfile.mkdtemp(prefix="gpu_compare_emit_")
                ed = os.path.join(tmp, "p%02d" % piece)
                os.makedirs(ed)
                run([os.path.join(ROOT, "build/vcore/vcore_tests"), "p%02d" % piece, "-q"], env=dict(os.environ, VCT_EMIT=ed), cwd=ROOT)
                for fn in sorted(os.listdir(ed)):
                    if fn.endswith(".vcs"):
                        jobs.append(("p%02d_em_%s" % (piece, fn[:-4]), os.path.join(ed, fn), piece, None, a))
            for i in range(a.fuzz):
                jobs.append(("p%02d_fz_%04d" % (piece, i), None, piece, (piece, a.seed, i), a))
    for s in a.scene:
        if os.path.isdir(s):
            for fn in sorted(os.listdir(s)):
                if fn.endswith(".vcs"):
                    jobs.append((fn[:-4], os.path.join(s, fn), 0, None, a))
        else:
            jobs.append((os.path.basename(s)[:-4], os.path.abspath(s), 0, None, a))
    if not jobs:
        ap.error("no scenes")
    for exe in (VRENDER, GRENDER):
        if not os.path.exists(exe):
            raise SystemExit(exe + " is not built (ninja -C build/vcore; ninja -C build/gpu)")
    with get_context("fork").Pool(a.jobs) as pool:
        res = pool.map(one, jobs, chunksize=1)
    if tmp:
        shutil.rmtree(tmp, ignore_errors=True)
    errs = [r for r in res if "error" in r]
    ok = [r for r in res if "error" not in r]
    if not a.quiet:
        print("%-28s %9s %9s %7s" % ("scene", "exact %", "within %", "bad px"))
        for r in ok:
            print("%-28s %9.4f %9.4f %7d" % (r["label"], pct(r["bad"], r["px"]), pct(r["far"], r["px"]), r["bad"]))
    for r in errs:
        print("ERROR %s: %s" % (r["label"], r["error"]))
    if a.piece:
        print("\n%-8s %7s %9s %9s %9s %7s" % ("piece", "scenes", "identical", "exact %", "within %", "errors"))
        for piece in parse_pieces(a.piece):
            rs = [r for r in res if r["piece"] == piece]
            g = [r for r in rs if "error" not in r]
            px = sum(r["px"] for r in g)
            print("p%-7d %7d %9d %9.4f %9.4f %7d" % (piece, len(rs), sum(1 for r in g if not r["bad"]),
                  pct(sum(r["bad"] for r in g), px), pct(sum(r["far"] for r in g), px), len(rs) - len(g)))
    px = sum(r["px"] for r in ok)
    print("\nTOTAL %d scenes (scale %d, tol %d, api %s): identical %d, exact %.4f%%, within tol %.4f%%, %d errors" % (
        len(res), a.scale, a.tol, a.api, sum(1 for r in ok if not r["bad"]), pct(sum(r["bad"] for r in ok), px),
        pct(sum(r["far"] for r in ok), px), len(errs)))
    if a.noedge >= 0:
        pn = sum(r["px_ne"] for r in ok)
        print("edges excluded (%d px around discontinuities left out, %.1f %% of the pixels kept): within tol %.4f%%" % (
            a.noedge, 100.0 * pn / px if px else 0, pct(sum(r["far_ne"] for r in ok), pn)))
    worst = sorted((r for r in ok if r["bad"]), key=lambda r: -r["bad"])[:a.show]
    for r in worst:
        print("\n--- %s: %d px differ, %d beyond tol  %s" % (r["label"], r["bad"], r["far"], r["desc"].replace("\n", " | ")[:200]))
        for (x, y, p, q) in r["firsts"]:
            print("  x=%3d y=%3d  vcore=%s  gpu=%s   (6-bit r,g,b,a5)" % (x, y, p, q))
    return 1 if errs else 0


if __name__ == "__main__":
    sys.exit(main())
