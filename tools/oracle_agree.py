#!/usr/bin/env python3
"""oracle_agree: how far vcore and the reference emulators (melonDS, NooDS, DeSmuME) agree with each other.

    python3 tools/oracle_agree.py [--piece pNN ...|--all-pieces] [--fuzz N] [--seed S] [--corpus DIR] [--synthetic]
        [--scene F.vcs ...] [--oracle NAME ...] [--no-vcore] [--suspect FILE] [--keep DIR] [-j N]

Scene sets (each is a group in the tables): a piece's fuzz scenes (port/vcore/tools/vcore_gen.py: --fuzz N per piece, 100
by default; pieces 1-19), the corpus (--corpus DIR, e.g. /root/ref/corpus), the synthetic scenes (--synthetic: mkvcs.py --all)
and single files (--scene). Participants: vcore (build/vcore/vcore_render) and every harness present among melonds
(/root/ref/oracle/oracle), noods (/root/ref/noods/oracle), desmume (/root/ref/desmume/oracle); --oracle picks
(NAME, NAME=PATH or PATH, repeatable, or $ORACLES=a,b), --no-vcore leaves vcore out. All run as black boxes with the oracle
interface (renders are cached: port/vcore/tools/oracles.py).

Per group and per pair of participants: scenes identical / compared and the pixel match % over the compared layers: the two
screens (6 bits per channel), the 3D layer (5-bit alpha, colour only where alpha > 0; fuzz pieces 13-19 and any scene with a
SWAP_BUFFERS) and, for capture scenes (piece 11, DISPCAPCNT bit 31), the VRAM banks (a scene is identical only when the
bytes are too; they don't count in the pixel %). Then the scenes where the EMULATORS differ from each other (vcore left
out): every such scene is a suspect.txt candidate: all emulators agree and vcore differs = vcore's bug; they disagree =
suspect (one of them has a quirk, or the rule vcore copied from melonDS is not the hardware's). Written to --suspect FILE
(default: build/oracle_agree/suspect_candidates.txt): group scene pairs-with-bad-pixel-counts and the first differing pixel.
"""
import argparse
import os
import random
import subprocess
import sys
import tempfile
from collections import OrderedDict, defaultdict
from itertools import combinations
from multiprocessing import get_context

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, ".."))
VT = os.path.join(ROOT, "port", "vcore", "tools")
sys.path.insert(0, VT)
sys.path.insert(0, "/root/ref/oracle")
import oracles  # noqa: E402
from vcs import read_png, read_vcs  # noqa: E402

RENDER = os.environ.get("VCORE_RENDER", os.path.join(ROOT, "build/vcore/vcore_render"))
PARTS = []  # [(name, path)] the participants; vcore first
CAPTURE_PIECE = 11


def load_png(path):
    w, h, ch, px = read_png(path)
    return np.frombuffer(px, dtype=np.uint8).reshape(h, w, ch)


def layer_scr(p):
    return (load_png(p)[..., :3] >> 2).astype(np.uint8)


def layer_3d(p):
    a = load_png(p)
    a5 = ((a[..., 3].astype(np.int32) * 31 + 127) // 255).astype(np.uint8)
    rgb = (a[..., :3] >> 2).astype(np.uint8)
    rgb[a5 == 0] = 0
    return np.dstack([rgb, a5])


def first_diff(x, y):
    d = np.nonzero((x != y).reshape(x.shape[0], x.shape[1], -1).any(axis=2))
    if not len(d[0]):
        return None
    i = 0
    return "x=%d y=%d %s vs %s" % (d[1][i], d[0][i], tuple(int(v) for v in x[d[0][i], d[1][i]]),
                                   tuple(int(v) for v in y[d[0][i], d[1][i]]))


def scene_layers(path, kind):
    """the layers to compare for a scene: from the generator's kind, else from what the scene holds"""
    if kind:
        return {"3d": ["3d"], "scr": ["scr"], "scr+vram": ["scr", "vram"]}[kind]
    v = read_vcs(path)
    gx = v.get("GX3D", b"")
    io9 = v.get("IO9 ", b"\0" * 0x70)
    ls = ["scr"]
    sw, i = False, 0
    while i + 4 <= len(gx):
        h = int.from_bytes(gx[i:i + 4], "little")
        sw = sw or (h & 0xFF) == 0x50
        i += 4 + 4 * ((h >> 8) & 0xFF)
    if sw:
        ls.append("3d")
    if int.from_bytes(io9[0x64:0x68], "little") >> 31:
        ls.append("vram")
    return ls


def render_one(name, path, scene, sbytes, layers, work, label):
    """-> dict layer -> array (or bytes for vram) for one participant"""
    want3d, wantvram = "3d" in layers, "vram" in layers
    if name == "vcore":
        base = os.path.join(work, label + ".vcore")
        cmd = [RENDER, scene, base + ".png"]
        if want3d:
            cmd += ["--3d", base + ".3d.png"]
        if wantvram:
            cmd += ["--dump-vram", base + ".vram"]
        r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if r.returncode:
            raise RuntimeError("vcore_render: " + r.stderr.decode(errors="replace")[-200:])
        png, p3, vr = base + ".png", base + ".3d.png", base + ".vram"
    else:
        png, p3, vr = oracles.render(path, scene, want3d, wantvram, oracles.cache_key(sbytes, path))
    out = {}
    if "scr" in layers:
        out["scr"] = layer_scr(png)
    if want3d:
        out["3d"] = layer_3d(p3)
    if wantvram:
        out["vram"] = open(vr, "rb").read()
    return out


def check(job):
    group, label, scene, seedt, work = job
    res = {"group": group, "label": label, "errors": {}}
    try:
        kind = None
        if scene is None:
            import vcore_gen
            piece, seed, idx = seedt
            f, desc, kind = vcore_gen.gen(piece, random.Random("%d/%d/%d" % seedt), False)
            scene = os.path.join(work, label + ".vcs")
            f.write(scene)
        res["path"] = scene
        layers = scene_layers(scene, kind)
        sbytes = open(scene, "rb").read()
        outs = OrderedDict()
        for name, path in PARTS:
            try:
                outs[name] = render_one(name, path, scene, sbytes, layers, work, label)
            except Exception as e:  # noqa
                res["errors"][name] = str(e)[-200:]
        pairs = {}
        for a, b in combinations(outs, 2):
            bad = {}
            tot = 0
            first = None
            for L in layers:
                x, y = outs[a][L], outs[b][L]
                if L == "vram":
                    bad["vram"] = int(sum(1 for i in range(min(len(x), len(y))) if x[i] != y[i])) if x != y else 0
                    continue
                if L == "scr":
                    n = int((x != y).any(axis=2).sum())
                else:
                    n = int((x != y).any(axis=2).sum())
                tot += x.shape[0] * x.shape[1]
                bad[L] = n
                if n and first is None:
                    first = L + " " + (first_diff(x, y) or "")
            pairs[(a, b)] = (tot, bad, first)
        res["pairs"] = pairs
        res["layers"] = layers
    except Exception as e:  # noqa
        import traceback
        res["fatal"] = "%s: %s %s" % (type(e).__name__, e, traceback.format_exc()[-300:])
    for ext in (".vcore.png", ".vcore.3d.png", ".vcore.vram"):
        try:
            os.unlink(os.path.join(work, label + ext))
        except OSError:
            pass
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--piece", action="append", default=[], help="pNN (repeatable): its fuzz scenes")
    ap.add_argument("--all-pieces", action="store_true")
    ap.add_argument("--fuzz", type=int, default=100)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--corpus", help="a directory of .vcs frames (group 'corpus')")
    ap.add_argument("--synthetic", action="store_true", help="mkvcs.py --all scenes (group 'synthetic')")
    ap.add_argument("--scene", action="append", default=[])
    ap.add_argument("--oracle", action="append", default=[])
    ap.add_argument("--no-vcore", action="store_true")
    ap.add_argument("--suspect", help="candidates file (default build/oracle_agree/suspect_candidates.txt)")
    ap.add_argument("--keep", help="(unused: pictures are in ~/.cache/vcore_check, keyed by scene hash)")
    ap.add_argument("-j", "--jobs", type=int, default=int(os.environ.get("BW_JOBS", "0") or 0) or os.cpu_count())
    a = ap.parse_args()

    if a.oracle:
        ors = oracles.parse_oracles(a.oracle)
    elif os.environ.get("ORACLES"):
        ors = oracles.parse_oracles([])
    else:
        ors = oracles.parse_oracles(oracles.present([n for n, _ in oracles.KNOWN]))
    PARTS.clear()
    if not a.no_vcore:
        PARTS.append(("vcore", RENDER))
    PARTS.extend(ors)
    if len(PARTS) < 2:
        raise SystemExit("need at least two participants")
    work = tempfile.mkdtemp(prefix="oracle_agree_")
    jobs = []
    pieces = list(range(1, 20)) if a.all_pieces else [int(p.lower().lstrip("p")) for p in a.piece]
    import vcore_check
    for p in pieces:
        for i in range(a.fuzz):
            jobs.append(("p%02d %s" % (p, vcore_check.PIECE_NAMES[p]), "p%02d_fz%04d" % (p, i), None, (p, a.seed, i), work))
    if a.corpus:
        for fn in sorted(os.listdir(a.corpus)):
            if fn.endswith(".vcs"):
                jobs.append(("corpus", fn[:-4], os.path.join(a.corpus, fn), None, work))
    if a.synthetic:
        sd = os.path.join(work, "synth")
        os.makedirs(sd)
        subprocess.run([sys.executable, os.path.join(VT, "mkvcs.py"), "--all", sd], check=True)
        for fn in sorted(os.listdir(sd)):
            jobs.append(("synthetic", fn[:-4], os.path.join(sd, fn), None, work))
    for s in a.scene:
        jobs.append(("scenes", os.path.basename(s)[:-4], os.path.abspath(s), None, work))
    jobs = [(g, l, sc, st, w) for (g, l, sc, st, w) in jobs]
    print("participants: %s; %d scenes" % (", ".join("%s" % n for n, _ in PARTS), len(jobs)))
    with get_context("fork").Pool(a.jobs) as pool:
        results = pool.map(check, jobs, chunksize=1)

    groups = OrderedDict()
    for r in results:
        groups.setdefault(r["group"], []).append(r)
    names = [n for n, _ in PARTS]
    emus = [n for n in names if n != "vcore"]
    suspects = []
    for g, rs in groups.items():
        print("\n== %s: %d scenes" % (g, len(rs)))
        errs = [r for r in rs if "fatal" in r or r["errors"]]
        for r in errs[:3]:
            print("  error %s: %s" % (r["label"], r.get("fatal") or r["errors"]))
        if errs:
            print("  (%d scenes with errors, left out of the pairs they break)" % len(errs))
        print("  %-18s %12s %12s" % ("pair", "identical", "pixel match"))
        for a_, b_ in combinations(names, 2):
            ps = [r["pairs"][(a_, b_)] for r in rs if "pairs" in r and (a_, b_) in r["pairs"]]
            if not ps:
                continue
            ident = sum(1 for t, bd, f in ps if not any(bd.values()))
            tot = sum(t for t, bd, f in ps)
            bad = sum(v for t, bd, f in ps for k, v in bd.items() if k != "vram")
            print("  %-18s %5d/%-6d %11.4f%%" % (a_ + " ~ " + b_, ident, len(ps), 100.0 * (tot - bad) / tot if tot else 100.0))
    for r in results:
        if "pairs" not in r:
            continue
        dis = [(p, v) for p, v in r["pairs"].items() if p[0] in emus and p[1] in emus and any(v[1].values())]
        if dis:
            vc = [("%s~%s" % p, sum(v[1].values())) for p, v in r["pairs"].items() if "vcore" in p]
            suspects.append("%s %s: emulators differ: %s | vcore: %s | first: %s" % (
                r["group"].split()[0], r["label"],
                ", ".join("%s~%s %s" % (p[0], p[1], dict(v[1])) for p, v in dis),
                ", ".join("%s %d" % x for x in vc) or "-", dis[0][1][2]))
    out = a.suspect or os.path.join(ROOT, "build", "oracle_agree", "suspect_candidates.txt")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "w").write("\n".join(suspects) + ("\n" if suspects else ""))
    print("\n%d scenes where the emulators (%s) differ from each other -> %s" % (len(suspects), ", ".join(emus), out))
    # the scenes where every emulator agrees and vcore differs
    if "vcore" in names and len(emus) > 1:
        ours = []
        for r in results:
            if "pairs" not in r:
                continue
            agree = all(not any(r["pairs"][(x, y)][1].values()) for x, y in combinations(emus, 2) if (x, y) in r["pairs"])
            vd = [p for p, v in r["pairs"].items() if "vcore" in p and any(v[1].values())]
            if agree and vd:
                ours.append(r["label"])
        print("scenes where all emulators agree and vcore differs (vcore's bugs): %d%s" % (
            len(ours), "  " + " ".join(ours[:20]) if ours else ""))
    import shutil
    shutil.rmtree(work, ignore_errors=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
