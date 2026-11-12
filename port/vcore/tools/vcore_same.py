#!/usr/bin/env python3
"""vcore_same: byte-equivalence check of two vcore_render binaries (a speed change must not move one output byte).

    port/vcore/tools/vcore_same.py [--ref BIN] [--new BIN] [--scenes DIR] [-j N] [--regen] [--quick]
                                   [--scale N] [--opts "--aa 2 --edge fine ..."]

Scene set (made once into --scenes, default /root/mt/vcore_same_scenes, then reused so that later vcore_gen changes
do not move the set): for every piece p01-p19 the unit tests' emitted scenes and the 100 fuzz scenes of seed 1 (as
vcore_check makes them), plus the corpus frames (/root/ref/corpus/*.vcs). Each scene is rendered by both binaries
with --3d, --dump-vram, --geo and the screen PNG; every output byte must be equal. --ref defaults to
/root/mt/sp56/ref/vcore_render (build it from the commit you start from), --new to build/vcore/vcore_render.
--scale N renders every scene at N x (the N x picture, p21) with both binaries, --opts adds vcore_render
options (--aa K, --dot fine, --line thin, --edge fine) to both. Exit 1 on any difference."""
import argparse, os, subprocess, sys, tempfile, shutil, random, hashlib
from multiprocessing import Pool

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../.."))
sys.path.insert(0, os.path.dirname(__file__))
CORPUS = "/root/ref/corpus"


def make_scenes(d, jobs):
    import vcore_gen
    os.makedirs(d, exist_ok=True)
    tests = os.path.join(ROOT, "build/vcore/vcore_tests")
    for piece in range(1, 20):
        ed = os.path.join(d, "em%02d" % piece)
        os.makedirs(ed, exist_ok=True)
        subprocess.run([tests, "p%02d" % piece, "-q"], env=dict(os.environ, VCT_EMIT=ed), cwd=ROOT,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        for i in range(100):
            rng = random.Random("%d/%d/%d" % (piece, 1, i))
            f, desc, kind = vcore_gen.gen(piece, rng, False)
            f.write(os.path.join(d, "fz%02d_%04d.vcs" % (piece, i)))


def list_scenes(d):
    out = []
    for dp, dn, fn in os.walk(d):
        out += [os.path.join(dp, x) for x in fn if x.endswith(".vcs")]
    if os.path.isdir(CORPUS):
        out += [os.path.join(CORPUS, x) for x in os.listdir(CORPUS) if x.endswith(".vcs")]
    return sorted(out)


def render(binp, scene, tmp, tag, extra):
    base = os.path.join(tmp, tag)
    cmd = [binp, scene, base + ".png", "--3d", base + ".3d.png", "--dump-vram", base + ".vram", "--geo", base + ".geo"] + extra
    r = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    res = [("rc", r.returncode)]
    for ext in (".png", ".3d.png", ".vram", ".geo"):
        try:
            res.append((ext, hashlib.sha1(open(base + ext, "rb").read()).hexdigest()))
            os.unlink(base + ext)
        except OSError:
            res.append((ext, None))
    return res


def one(a):
    ref, new, scene, tmp, extra = a
    tag = hashlib.sha1(scene.encode()).hexdigest()[:12]
    ra = render(ref, scene, tmp, tag + "r", extra)
    rb = render(new, scene, tmp, tag + "n", extra)
    return scene, [x[0] for x, y in zip(ra, rb) if x != y]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--ref", default="/root/mt/sp56/ref/vcore_render")
    ap.add_argument("--new", default=os.path.join(ROOT, "build/vcore/vcore_render"))
    ap.add_argument("--scenes", default="/root/mt/vcore_same_scenes")
    ap.add_argument("-j", type=int, default=int(os.environ.get("BW_JOBS", "0") or 0) or os.cpu_count())
    ap.add_argument("--regen", action="store_true")
    ap.add_argument("--scale", type=int, default=1)
    ap.add_argument("--opts", default="")
    a = ap.parse_args()
    extra = (["--scale", str(a.scale)] if a.scale != 1 else []) + a.opts.split()
    if a.regen or not os.path.isdir(a.scenes):
        shutil.rmtree(a.scenes, ignore_errors=True)
        make_scenes(a.scenes, a.j)
    scenes = list_scenes(a.scenes)
    tmp = tempfile.mkdtemp(prefix="vcore_same_")
    with Pool(a.j) as p:
        rs = p.map(one, [(a.ref, a.new, s, tmp, extra) for s in scenes], chunksize=4)
    shutil.rmtree(tmp, ignore_errors=True)
    bad = [(s, d) for s, d in rs if d]
    print("vcore_same: %d scenes, %d differ%s" % (len(scenes), len(bad), " (%s)" % " ".join(extra) if extra else ""))
    for s, d in bad[:20]:
        print("  DIFF", s, " ".join(d))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
