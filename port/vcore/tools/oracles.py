"""The reference emulators' harnesses as black boxes, for vcore_check.py and tools/oracle_agree.py.

Every harness has the melonDS oracle's command line (/root/ref/oracle/README):  H F.vcs OUT.png [--3d OUT3D.png]
[--dump-vram OUT.bin] -q.  Known ones: melonds = /root/ref/oracle/oracle, noods = /root/ref/noods/oracle, desmume =
/root/ref/desmume/oracle (when built). Their renders are cached by scene hash and harness (path + mtime), so a harness
rebuild forgets its old pictures.

    parse_oracles(["noods", "melonds=/x/oracle", "/y/z/oracle"])  -> [(name, path), ...]  (ORACLES=a,b,c when empty)
    render(path, scene, want3d, wantvram) -> (png, 3d png, vram bin) cached files
"""
import hashlib
import os
import subprocess

KNOWN = [("melonds", "/root/ref/oracle/oracle"), ("noods", "/root/ref/noods/oracle"),
         ("desmume", "/root/ref/desmume/oracle")]
CACHE = os.environ.get("VCORE_CHECK_CACHE", os.path.expanduser("~/.cache/vcore_check"))


def oracle_name(path):
    for n, p in KNOWN:
        if os.path.abspath(path) == p:
            return n
    d = os.path.basename(os.path.dirname(os.path.abspath(path)))
    return d if d and d != "oracle" else os.path.basename(path)


def parse_oracles(specs, default=None):
    """specs: names (melonds, noods, desmume), NAME=PATH or PATHs; empty -> $ORACLES (comma list), else default
    (a list of the same). A known name whose harness isn't built is an error."""
    if not specs:
        env = os.environ.get("ORACLES", "")
        specs = [s for s in env.split(",") if s] or (default or ["melonds"])
    out = []
    for s in specs:
        if "=" in s:
            n, p = s.split("=", 1)
        elif s in dict(KNOWN):
            n, p = s, dict(KNOWN)[s]
        else:
            n, p = oracle_name(s), s
        if not os.path.exists(p):
            raise SystemExit("oracle %s: %s doesn't exist (build it first)" % (n, p))
        out.append((n, os.path.abspath(p)))
    return out


def present(names):
    """the known harnesses among names that exist"""
    return [n for n in names if os.path.exists(dict(KNOWN)[n])]


def run(cmd, **kw):
    return subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kw)


def cache_key(scene_bytes, oracle_path, tag=""):
    return hashlib.sha1(scene_bytes + str(int(os.stat(oracle_path).st_mtime)).encode() + oracle_path.encode()
                        + tag.encode()).hexdigest()


def render(oracle_path, scene, want3d, wantvram, key=None):
    """a harness's outputs for scene, cached; returns (png, 3d png or None, vram bin or None)"""
    os.makedirs(CACHE, exist_ok=True)
    if key is None:
        key = cache_key(open(scene, "rb").read(), oracle_path)
    base = os.path.join(CACHE, key)
    png, p3, vr = base + ".png", base + ".3d.png", base + ".vram"
    need = [png] + ([p3] if want3d else []) + ([vr] if wantvram else [])
    if not all(os.path.exists(p) for p in need):
        tmp = base + ".%d" % os.getpid()
        cmd = [oracle_path, scene, tmp + ".png", "-q"]
        if want3d:
            cmd += ["--3d", tmp + ".3d.png"]
        if wantvram:
            cmd += ["--dump-vram", tmp + ".vram"]
        r = run(cmd)
        if r.returncode != 0:
            raise RuntimeError("%s failed on %s: %s" % (oracle_path, scene, r.stderr.decode(errors="replace")[-300:]))
        for ext in (".png", ".3d.png", ".vram"):
            if os.path.exists(tmp + ext):
                os.replace(tmp + ext, base + ext)
    return png, (p3 if want3d else None), (vr if wantvram else None)
