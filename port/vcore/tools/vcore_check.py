#!/usr/bin/env python3
"""vcore_check: check one piece of port/vcore against the melonDS oracle (docs/vcore.md, "Checking a piece").

    port/vcore/tools/vcore_check pNN [--fuzz N] [--seed S] [--keep DIR] [--no-emit] [--scene F.vcs ...] [-v]

pNN = 1..19. It renders (a) the piece's emitted unit-test scenes (VCT_EMIT=... vcore_tests pNN) and (b) N random scenes
for the piece's feature (tools/vcore_gen.py: random registers, VRAM, OAM, palettes, 3D commands in the feature, the rest
plain) with build/vcore/vcore_render and /root/ref/oracle/oracle, and compares pixel-exact: both screens for 2D pieces
(11 also the VRAM banks), the 3D layer (--3d: RGBA, alpha 5 bits, colour 6 bits) for pieces 13-19. The oracle runs as
a black box; its renders are cached by scene hash (~/.cache/vcore_check). Parallel over BW_JOBS (default: all cores).
The other pieces are the merged ones; a scene is built so the piece is the only thing that varies, but a mismatch can
still come from a neighbour (VRAM mapping p12, compositor p09): the report says what the scene held.
Failing scenes are kept in --keep DIR (default build/vcore_check/pNN) with both pictures, a diff picture and a .txt.
Exit status 1 when any scene fails.
"""
import argparse
import hashlib
import os
import random
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
from multiprocessing import get_context
Pool = get_context("fork").Pool

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, HERE)
sys.path.insert(0, "/root/ref/oracle")
import oracles  # noqa: E402
ORACLE = os.environ.get("VCORE_ORACLE", "/root/ref/oracle/oracle")
ORACLES = [("melonds", ORACLE)]  # [(name, harness path)]: --oracle / $ORACLES; the first is the primary (the exit status)
RENDER = os.environ.get("VCORE_RENDER", os.path.join(ROOT, "build/vcore/vcore_render"))
TESTS = os.path.join(ROOT, "build/vcore/vcore_tests")
CACHE = os.environ.get("VCORE_CHECK_CACHE", os.path.expanduser("~/.cache/vcore_check"))
PIECE_NAMES = {1: "text BGs", 2: "affine BGs", 3: "extended BGs", 4: "large bitmap + BG mosaic", 5: "OBJs",
               6: "affine OBJs", 7: "bitmap/window/mosaic OBJs", 8: "windows", 9: "compositor", 10: "display",
               11: "capture", 12: "VRAM mapping", 13: "matrix engine", 14: "polygons", 15: "lighting",
               16: "rasterizer", 17: "textures", 18: "translucency", 19: "post-processing"}


def read_png(path):
    from vcs import read_png as rp
    return rp(path)


def run(cmd, **kw):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kw)


def oracle_render(scene, kind, key, path=None):
    """one oracle's outputs for scene, cached (tools/oracles.py); returns paths (png, 3d png, vram bin)"""
    png, p3, vr = oracles.render(path or ORACLE, scene, kind == "3d", "vram" in kind, key)
    return png, p3, vr


def diff_rgb(a, b, ch_a, ch_b, w, h, first=6):
    """compare two RGB(A) buffers at 6 bits; returns (n differing pixels, [(x, y, mine, theirs)])"""
    n, firsts = 0, []
    for i in range(w * h):
        p, q = a[i * ch_a:i * ch_a + 3], b[i * ch_b:i * ch_b + 3]
        if p != q and (p[0] >> 2, p[1] >> 2, p[2] >> 2) != (q[0] >> 2, q[1] >> 2, q[2] >> 2):
            n += 1
            if len(firsts) < first:
                firsts.append((i % w, i // w, tuple(v >> 2 for v in p), tuple(v >> 2 for v in q)))
    return n, firsts


def diff_3d(a, b, w, h, first=6):
    n, firsts = 0, []
    for i in range(w * h):
        p, q = a[i * 4:i * 4 + 4], b[i * 4:i * 4 + 4]
        if p == q:
            continue
        a5, b5 = (p[3] * 31 + 127) // 255, (q[3] * 31 + 127) // 255
        pr, qr = tuple(v >> 2 for v in p[:3]), tuple(v >> 2 for v in q[:3])
        if a5 != b5 or (a5 and pr != qr):
            n += 1
            if len(firsts) < first:
                firsts.append((i % w, i // w, pr + (a5,), qr + (b5,)))
    return n, firsts


def write_diff(path, a, b, ch_a, ch_b, w, h):
    from vcs import write_png
    out = bytearray(w * h * 3)
    for i in range(w * h):
        p, q = a[i * ch_a:i * ch_a + 3], b[i * ch_b:i * ch_b + 3]
        same = (p[0] >> 2, p[1] >> 2, p[2] >> 2) == (q[0] >> 2, q[1] >> 2, q[2] >> 2)
        out[i * 3:i * 3 + 3] = bytes((0, 0, 0)) if same else bytes((255, 0, 0))
    write_png(path, w, h, 3, bytes(out))


def check_scene(args):
    """args: (piece, label, scene path or None, seed tuple, workdir, keepdir). returns a result dict"""
    piece, label, path, seedt, work, keep = args
    res = {"label": label, "piece": piece, "ok": False, "px": 0, "bad": 0, "firsts": [], "desc": "", "kind": "scr"}
    try:
        if path is None:
            import vcore_gen
            rng = random.Random("%d/%d/%d" % seedt)
            f, desc, kind = vcore_gen.gen(piece, rng, os.environ.get('VCORE_GREEN_LSB') == '1')
            path = os.path.join(work, label + ".vcs")
            f.write(path)
            res["desc"] = desc
        else:
            kind = "3d" if 13 <= piece <= 19 else ("scr+vram" if piece == 11 else "scr")
            res["desc"] = "emitted scene " + os.path.basename(path)
        res["kind"] = kind
        res["path"] = path
        sbytes = open(path, "rb").read()
        mine = os.path.join(work, label + ".my.png")
        cmd = [RENDER, path, mine]
        if kind == "3d":
            cmd += ["--3d", os.path.join(work, label + ".my.3d.png")]
        if "vram" in kind:
            cmd += ["--dump-vram", os.path.join(work, label + ".my.vram")]
        r = run(cmd)
        if r.returncode != 0:
            res["error"] = "vcore_render failed (exit %d): %s" % (r.returncode, r.stderr.decode(errors="replace")[-300:])
            return res
        res["layer"] = "3D layer" if kind == "3d" else "screens"
        res["per"] = {}  # oracle name -> {ok, px, bad, firsts, vram_bad, vram_first, error}
        for oi, (oname, opath) in enumerate(ORACLES):
            pr = {"ok": False, "px": 0, "bad": 0, "firsts": []}
            res["per"][oname] = pr
            try:
                key = oracles.cache_key(sbytes, opath, kind)
                opng, o3, ovr = oracle_render(path, kind, key, opath)
                bad, total, firsts = 0, 0, []
                if kind == "3d":
                    w, h, c1, a = read_png(os.path.join(work, label + ".my.3d.png"))
                    w2, h2, c2, b = read_png(o3)
                    bad, firsts = diff_3d(a, b, w, h)
                    total = w * h
                else:
                    w, h, c1, a = read_png(mine)
                    w2, h2, c2, b = read_png(opng)
                    total = w * h
                    if a != b:
                        bad, firsts = diff_rgb(a, b, c1, c2, w, h)
                    if "vram" in kind:
                        m = open(os.path.join(work, label + ".my.vram"), "rb").read()
                        o = open(ovr, "rb").read()
                        vb = sum(1 for i in range(0, min(len(m), len(o))) if m[i] != o[i]) if m != o else 0
                        pr["vram_bad"] = vb
                        if vb:
                            i0 = next(i for i in range(len(m)) if m[i] != o[i])
                            pr["vram_first"] = "first differing VRAM byte 0x%X (mine %02X %s %02X), %d bytes differ" % (
                                i0, m[i0], oname, o[i0], vb)
                pr["px"], pr["bad"], pr["firsts"] = total, bad, firsts
                pr["ok"] = bad == 0 and not pr.get("vram_bad")
                if not pr["ok"]:
                    os.makedirs(keep, exist_ok=True)
                    dst = os.path.join(keep, label)
                    if oi == 0:
                        shutil.copy(path, dst + ".vcs")
                        shutil.copy(mine if kind != "3d" else os.path.join(work, label + ".my.3d.png"), dst + ".mine.png")
                        res["saved"] = dst + ".vcs"
                    shutil.copy(opng if kind != "3d" else o3, dst + (".oracle.png" if oi == 0 else ".%s.png" % oname))
                    if kind != "3d":
                        write_diff(dst + (".diff.png" if oi == 0 else ".diff_%s.png" % oname), a, b, c1, c2, w, h)
            except Exception as e:  # noqa
                pr["error"] = "%s: %s" % (type(e).__name__, e)
        p0 = res["per"][ORACLES[0][0]]
        # the legacy fields are the primary oracle's
        res["px"], res["bad"], res["firsts"], res["ok"] = p0["px"], p0["bad"], p0["firsts"], p0["ok"]
        for k in ("vram_bad", "vram_first"):
            if k in p0:
                res[k] = p0[k]
        if "error" in p0:
            res["error"] = p0["error"]
    except Exception as e:  # noqa
        import traceback
        res["error"] = "%s: %s" % (type(e).__name__, e) + traceback.format_exc()[-400:]
    finally:
        for ext in (".my.png", ".my.3d.png", ".my.vram") + ((".vcs",) if res.get("ok") and not os.environ.get("VCORE_CHECK_ALL") else ()):
            try:
                os.unlink(os.path.join(work, label + ext))
            except OSError:
                pass
    return res


def main():
    ap = argparse.ArgumentParser(description="check one vcore piece against the oracle")
    ap.add_argument("piece", nargs="?", help="pNN or NN (1-19)")
    ap.add_argument("--fuzz", type=int, default=100, help="random scenes (default 100; 0 = emitted only)")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--keep", help="where failing scenes go (default build/vcore_check/pNN)")
    ap.add_argument("--no-emit", action="store_true", help="skip the unit tests' emitted scenes")
    ap.add_argument("--scene", action="append", default=[], help="also check this .vcs file (repeatable)")
    ap.add_argument("--show", type=int, default=5, help="failing scenes described in full (default 5)")
    ap.add_argument("-j", "--jobs", type=int, default=int(os.environ.get("BW_JOBS", "0") or 0) or os.cpu_count())
    ap.add_argument("--no-build", action="store_true")
    ap.add_argument("--green-lsb", action="store_true", help="leave bit 15 of palette entries random (melonDS's green-lsb quirk shows)")
    ap.add_argument("--list", action="store_true", help="list the pieces")
    ap.add_argument("--oracle", action="append", default=[],
                    help="a reference harness: melonds, noods, desmume, NAME=PATH or a PATH (repeatable, or ORACLES=a,b,c); "
                         "the first is primary (exit status, kept pictures), the others are reported beside it")
    a = ap.parse_args()
    global ORACLES
    ORACLES = oracles.parse_oracles(a.oracle, default=["melonds=" + ORACLE] if "VCORE_ORACLE" in os.environ else ["melonds"])
    if a.list or not a.piece:
        for k, v in PIECE_NAMES.items():
            print("p%02d %s" % (k, v))
        return 0
    if a.green_lsb:
        os.environ["VCORE_GREEN_LSB"] = "1"
    piece = int(a.piece.lower().lstrip("p"))
    if piece not in PIECE_NAMES:
        print("pieces: 1-19 (20: use tools/vcore_compare.py corpus)")
        return 2
    if not a.no_build:
        if not os.path.exists(os.path.join(ROOT, "build/vcore/build.ninja")):
            run(["meson", "setup", "build/vcore", "port/vcore"], cwd=ROOT)
        r = run(["ninja", "-C", os.path.join(ROOT, "build/vcore")])
        if r.returncode:
            print(r.stdout.decode(errors="replace")[-3000:])
            print("vcore build failed")
            return 2
    keep = a.keep or os.path.join(ROOT, "build/vcore_check/p%02d" % piece)
    if os.path.isdir(keep):
        for fn in os.listdir(keep):
            if fn.startswith("fz") or fn.startswith("em") or fn.startswith("sc"):
                os.unlink(os.path.join(keep, fn))
    work = tempfile.mkdtemp(prefix="vcore_check_")
    jobs = []
    if not a.no_emit:
        ed = os.path.join(work, "emit")
        os.makedirs(ed)
        env = dict(os.environ, VCT_EMIT=ed)
        r = run([TESTS, "p%02d" % piece, "-q"], env=env, cwd=ROOT)
        if r.returncode:
            print("note: unit tests of p%02d fail (exit %d); their scenes are still checked" % (piece, r.returncode))
        for fn in sorted(os.listdir(ed)):
            if fn.endswith(".vcs"):
                jobs.append((piece, "em_" + fn[:-4], os.path.join(ed, fn), None, work, keep))
    for fpath in a.scene:
        jobs.append((piece, "sc_" + os.path.basename(fpath)[:-4], os.path.abspath(fpath), None, work, keep))
    nemit = len(jobs)
    for i in range(a.fuzz):
        jobs.append((piece, "fz_%04d" % i, None, (piece, a.seed, i), work, keep))
    print("p%02d %s: %d emitted/given scenes + %d random scenes (seed %d), %d jobs" % (
        piece, PIECE_NAMES[piece], nemit, a.fuzz, a.seed, a.jobs))
    with Pool(a.jobs) as pool:
        results = pool.map(check_scene, jobs, chunksize=1)
    shutil.rmtree(work, ignore_errors=True)

    def summary(name, rs):
        if not rs:
            return
        ok = sum(1 for r in rs if r["ok"])
        px = sum(r["px"] for r in rs)
        bad = sum(r["bad"] for r in rs)
        err = sum(1 for r in rs if "error" in r)
        print("%-8s scenes passed %d/%d   pixel match %.4f%% (%d of %d pixels differ)%s" % (
            name, ok, len(rs), 100.0 * (px - bad) / px if px else 0.0, bad, px, "   %d errors" % err if err else ""))
    summary("emitted", results[:nemit])
    summary("fuzz", results[nemit:])
    if len(ORACLES) > 1:
        print("\nper oracle (the first, %s, is the primary):" % ORACLES[0][0])
        for on, _ in ORACLES:
            rs = [r["per"][on] for r in results if "per" in r]
            px, bad = sum(r["px"] for r in rs), sum(r["bad"] for r in rs)
            print("  %-8s scenes identical %d/%d   pixel match %.4f%%   %d errors" % (
                on, sum(1 for r in rs if r["ok"]), len(rs), 100.0 * (px - bad) / px if px else 0.0,
                sum(1 for r in rs if "error" in r)))
        alld = [r["label"] for r in results if "per" in r and not any(v["ok"] for v in r["per"].values())]
        print("  scenes where vcore differs from EVERY oracle: %d %s" % (len(alld), " ".join(alld[:30])))
    fails = [r for r in results if not r["ok"]]
    if not fails:
        print("p%02d: ALL PASS" % piece)
        return 0
    print("\n%d failing scenes (kept in %s)" % (len(fails), keep))
    for r in fails[:a.show]:
        print("\n--- %s  [%s]  %s" % (r["label"], r.get("layer", "?"), r.get("saved", "")))
        if "error" in r:
            print("  ERROR", r["error"])
            continue
        print("  %d of %d pixels differ%s" % (r["bad"], r["px"], "; " + r["vram_first"] if r.get("vram_bad") else ""))
        for (x, y, p, q) in r["firsts"]:
            scr = ""
            if r["kind"] != "3d":
                scr = "top" if y < 192 else "bottom"
                y = y % 192
            else:
                scr = "3D"
            print("  %-6s x=%3d y=%3d  mine=%s  oracle=%s   (6-bit r,g,b%s)" % (
                scr, x, y, p, q, ",a5" if r["kind"] == "3d" else ""))
        for line in r["desc"].splitlines():
            print("  | " + line)
    if len(fails) > a.show:
        print("\nalso failing: " + ", ".join("%s(%d px)" % (r["label"], r["bad"]) for r in fails[a.show:a.show + 40])
              + (" ..." if len(fails) > a.show + 40 else ""))
    return 1


if __name__ == "__main__":
    sys.exit(main())
