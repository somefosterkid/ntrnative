#!/usr/bin/env python3
"""The GL cores' shaders against strict GLSL ES compilers, without a GPU or a phone (docs/gpu-compat.md).

    python3 port/gpu/tools/gpu_glsl_check.py [--gpu-glsl build/gpu/gpu_glsl] [--out DIR] [--no-mali] [--only C,C] [-v]

build/gpu/gpu_glsl writes every program the cores can build, as src/gpu_gl.c's build_program would hand it to the
driver (ES 3.00, and desktop GL 3.30 / 4.50), and compiles them in a headless context. This script runs:

  names     every shader name in port/gpu/src/*.c is one gpu_glsl knows (a new program cannot slip past the check)
  lint      ES text: the header's `precision highp` for float, int and the samplers comes before any code, no mediump or
            lowp anywhere, every opaque type highp (by the header or its declaration). Mali and Adreno take mediump as
            fp16 / 16-bit ints in fragment shaders; the cores' integer work (24-bit depth, 32-bit packed state) and
            texture coordinates at 8x need highp, so nothing may fall back to a lower default.
  glslang   Khronos' reference front end, each ES and GL shader (and each pair linked): $GLSLANG, glslangValidator or
            glslang on PATH, or /opt/gpu-tools/glslang/bin/glslang (tools/gpu_shader_deps.sh)
  mesa      the headless context's own compiler (Mesa llvmpipe's GLES 3.2: gpu_glsl --compile)
  angle     the Android emulator's ANGLE (GLES 3.0 on its GL backend; ANGLE's front end is Chrome's and the one Android
            ships as a system GLES driver), from $ANDROID_EMU_LIB or /opt/android/sdk/emulator/lib64/gles_angle
  swiftshader  the emulator's SwiftShader GLES 3.0 (what -gpu swiftshader_indirect runs: its translator refuses ?: on a
            struct, as the emulator's log showed)
  mali      Arm's Mali Offline Compiler for MALI_TARGETS (default Mali-G52,G57,G78,G710,G715,G720): $MALIOC, malioc
            on PATH, or /opt/gpu-tools/arm-ps/*/mali_offline_compiler/malioc; prints the work registers, spilling and
            the longest path's cycles per program
  adreno    (only with $ADRENO_CMD) Qualcomm's offline compiler: a command with {file} and {stage} in it, run per ES shader

One line a program and compiler (ok / warn / FAIL / skip, with the compiler's message), then a table, then the verdict
`gpu_glsl_check: PASS ...` or `gpu_glsl_check: FAIL ...` (the gate reads the last line). A compiler that is not installed
is skipped with a note, except glslang and mesa (the gate's minimum).
"""
import argparse
import glob
import os
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile

HERE = pathlib.Path(__file__).resolve().parent
GPU = HERE.parent
EMU = os.environ.get("ANDROID_EMU_LIB", "/opt/android/sdk/emulator/lib64")
MALI_TARGETS = os.environ.get("MALI_TARGETS", "Mali-G52,Mali-G57,Mali-G78,Mali-G710,Mali-G715,Mali-G720").split(",")


def which(env, names, fallbacks):
    if os.environ.get(env):
        return os.environ[env]
    for n in names:
        p = shutil.which(n)
        if p:
            return p
    for f in fallbacks:
        for p in sorted(glob.glob(f)):
            if os.access(p, os.X_OK):
                return p
    return None


def load_programs(d):
    """programs.txt: api vs-file fs-file vs-name fs-name"""
    progs = []
    for line in (d / "programs.txt").read_text().splitlines():
        api, vf, ff, vs, fs = line.split(" ", 4)
        progs.append(dict(api=api, vf=vf, ff=ff, vs=vs, fs=fs, name="%s + %s" % (vs, fs)))
    return progs


def check_names(progs):
    known = {p["vs"] for p in progs} | {p["fs"] for p in progs}
    bad = []
    for f in sorted((GPU / "src").glob("*.c")):
        for m in re.finditer(r'"(g\d\d_\w+\.(?:frag|vert)(?:#\w+)?(?:\+\w+)?)"', f.read_text()):
            if m.group(1) not in known:
                bad.append("%s: %s" % (f.name, m.group(1)))
    return bad


OPAQUE = re.compile(r"\b(?:uniform\s+)?((?:highp|mediump|lowp)\s+)?([iu]?(?:sampler|image)\w+)\s+\w+\s*[;\[]")


def lint(text):
    """problems of an ES shader's text (empty: none)"""
    out = []
    head = text.split("\n")
    if not head[0].startswith("#version 3") or not head[0].endswith(" es"):
        return ["first line is not an ES #version: %s" % head[0]]
    code = re.sub(r"//[^\n]*|/\*.*?\*/", "", text, flags=re.S)
    m = re.search(r"^\s*(?!#|precision)\S", code.split("\n", 1)[1], re.M)
    before = code[:m.start() + len(code.split("\n", 1)[0]) + 1] if m else code
    declared = set(re.findall(r"precision\s+highp\s+(\w+)\s*;", before))
    for t in ("float", "int"):
        if t not in declared:
            out.append("no `precision highp %s;` before the code" % t)
    for w in re.findall(r"\b(mediump|lowp)\b", code):
        out.append("%s used" % w)
    for q, t in set(OPAQUE.findall(code)):
        if not q and t not in declared:
            out.append("%s without a precision (ES has no default for it, or a low one)" % t)
    return sorted(set(out))


def run(cmd, env=None, timeout=600):
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=timeout)
    return r.returncode, (r.stdout + r.stderr)


def glslang_check(gl, d, p):
    """(verdict, message) for one program with glslang: each shader, then the pair linked"""
    if "G10_FETCH" in p["fs"]:
        return "skip", "glslang has no EXT_shader_framebuffer_fetch (mali and the context compilers check it)"
    if "G10_IORDER" in p["fs"]:
        return "skip", "glslang has no INTEL_fragment_shader_ordering"
    msgs = []
    for f in (p["vf"], p["ff"]):
        rc, out = run([gl, str(d / f)])
        lines = [l for l in out.splitlines() if re.match(r"(ERROR|WARNING):", l) and "compilation errors" not in l]
        if rc != 0:
            return "FAIL", "%s: %s" % (f, "; ".join(lines[:4]) or out.strip()[:300])
        msgs += ["%s: %s" % (f, l) for l in lines]
    rc, out = run([gl, "-l", str(d / p["vf"]), str(d / p["ff"])])
    if rc != 0:
        lines = [l for l in out.splitlines() if "ERROR" in l]
        return "FAIL", "link: %s" % ("; ".join(lines[:4]) or out.strip()[:300])
    return ("warn" if msgs else "ok"), "; ".join(msgs[:4])


def context_check(exe, env, what):
    """gpu_glsl --compile in a context: {program name: (verdict, message)} and the context line"""
    try:
        rc, out = run([exe, "--compile", "es"], env=env, timeout=900)
    except subprocess.TimeoutExpired:
        return None, "%s: timed out" % what
    res, cur, ctx = {}, None, ""
    for line in out.splitlines():
        if line.startswith("context "):
            ctx = line[8:]
        m = re.match(r"(ok|warn|FAIL|skip) (.+?)( \((?:vertex|fragment|link)\))?(?:: (.*))?$", line)
        if m and " + " in m.group(2):
            cur = m.group(2)
            res[cur] = [m.group(1), (m.group(3) or "").strip() + (m.group(4) or "")]
        elif line.startswith("    ") and cur:
            res[cur][1] = (res[cur][1] + "; " if res[cur][1] else "") + line.strip()
    if not res:
        return None, "%s: no context (%s)" % (what, out.strip().splitlines()[-1] if out.strip() else rc)
    return res, ctx


def emu_env(sub, scratch):
    """the environment that makes gpu_glsl load the emulator's GLES `sub` (gles_angle / gles_swiftshader) through its
    libEGL.so.1 lookup"""
    lib = pathlib.Path(EMU) / sub
    if not (lib / "libEGL.so").exists():
        return None
    links = pathlib.Path(scratch) / sub
    links.mkdir(parents=True, exist_ok=True)
    for name, target in (("libEGL.so.1", "libEGL.so"), ("libGLESv2.so.2", "libGLESv2.so"), ("libGLESv2.so", "libGLESv2.so")):
        l = links / name
        if l.is_symlink() or l.exists():
            l.unlink()
        l.symlink_to(lib / target)
    if (lib / "libvulkan.so.1").exists():
        l = links / "libvulkan.so.1"
        if not l.exists():
            l.symlink_to(lib / "libvulkan.so.1")
    env = dict(os.environ)
    env["LD_LIBRARY_PATH"] = ":".join([str(links), str(lib), EMU] + ([env["LD_LIBRARY_PATH"]] if env.get("LD_LIBRARY_PATH") else []))
    if (lib / "vk_swiftshader_icd.json").exists():
        env["VK_ICD_FILENAMES"] = str(lib / "vk_swiftshader_icd.json")
    env.pop("GPU_EGL", None)
    return env


def mali_check(mc, d, p, target):
    """(verdict, message) of malioc for the program's two shaders on one Mali target; the message has the fragment
    shader's work registers, spilling and the longest path's cycles"""
    msgs, stats = [], ""
    for f, stage in ((p["vf"], "--vertex"), (p["ff"], "--fragment")):
        rc, out = run([mc, "-c", target, stage, str(d / f)])
        errs = [l.strip() for l in out.splitlines() if re.search(r"\b(ERROR|error)\b", l)]
        warns = [l.strip() for l in out.splitlines() if re.search(r"\bWARNING\b", l, re.I)]
        if rc != 0 or errs:
            return "FAIL", "%s: %s" % (f, "; ".join(errs[:4]) or out.strip()[-300:])
        msgs += ["%s: %s" % (f, w) for w in warns]
        if stage == "--fragment":
            reg = re.search(r"Work registers:\s*(\d+)(?:.*?at (\d+)% occupancy)?", out)
            spill = re.search(r"Spill region:\s*(\d+)", out)
            cyc = re.search(r"Longest path cycles:\s*(.+)", out)
            if cyc is None or "N/A" in cyc.group(1):  # a loop the compiler cannot bound: the total over the code once
                cyc = re.search(r"(Total) instruction cycles:\s*(.+)", out)
                cyc_s = "total %s" % " ".join(cyc.group(2).split()) if cyc else "?"
            else:
                cyc_s = "longest %s" % " ".join(cyc.group(1).split())
            stats = "regs %s%s, %s, cycles A LS V T %s" % (
                reg.group(1) if reg else "?", " at %s%% occupancy" % reg.group(2) if reg and reg.group(2) else "",
                "spills %s bytes" % spill.group(1) if spill else "no spills", cyc_s)
    return ("warn" if msgs else "ok"), stats + ("; " + "; ".join(msgs[:3]) if msgs else "")


def adreno_check(cmd, d, p):
    msgs = []
    for f, stage in ((p["vf"], "vert"), (p["ff"], "frag")):
        rc, out = run(cmd.format(file=str(d / f), stage=stage).split())
        if rc != 0:
            return "FAIL", "%s: %s" % (f, out.strip()[-300:])
        msgs += [l.strip() for l in out.splitlines() if re.search(r"warning", l, re.I)]
    return ("warn" if msgs else "ok"), "; ".join(msgs[:3])


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--gpu-glsl", default="build/gpu/gpu_glsl")
    ap.add_argument("--out", default=None, help="where the shaders' text goes (default: a temporary directory)")
    ap.add_argument("--no-mali", action="store_true", help="leave out Arm's Mali Offline Compiler")
    ap.add_argument("--only", default="", help="comma list of the compilers to run (names, lint, glslang, mesa, ...)")
    ap.add_argument("-v", action="store_true", help="every program's line, not only the ones that are not ok")
    a = ap.parse_args()
    only = set(a.only.split(",")) - {""}
    want = lambda c: not only or c in only
    exe = os.path.abspath(a.gpu_glsl)
    tmp = tempfile.TemporaryDirectory(prefix="gpu_glsl_")
    d = pathlib.Path(a.out or tmp.name)
    d.mkdir(parents=True, exist_ok=True)
    rc, out = run([exe, str(d)])
    if rc != 0:
        print(out.strip())
        print("gpu_glsl_check: FAIL (gpu_glsl could not write the programs)")
        return 1
    progs = load_programs(d)
    es = [p for p in progs if p["api"] == "es"]
    cols = []  # (compiler, {program name: (verdict, message)})
    notes = []
    fails = 0

    if want("names"):
        bad = check_names(progs)
        for b in bad:
            print("FAIL names: %s is not in tools/gpu_glsl.c's list" % b)
        fails += len(bad)
    if want("lint"):
        r = {}
        for p in es:
            probs = lint((d / p["vf"]).read_text()) + lint((d / p["ff"]).read_text())
            r[p["name"]] = ("FAIL", "; ".join(sorted(set(probs)))) if probs else ("ok", "")
        cols.append(("lint", r))
    gl = which("GLSLANG", ["glslangValidator", "glslang"], ["/opt/gpu-tools/glslang/bin/glslang"])
    if want("glslang"):
        if gl is None:
            print("FAIL glslang: not installed (apt install glslang-tools, or tools/gpu_shader_deps.sh)")
            fails += 1
        else:
            cols.append(("glslang", {p["name"]: glslang_check(gl, d, p) for p in es}))
            cols.append(("glslang-gl", {p["name"]: glslang_check(gl, d, p) for p in progs if p["api"] == "gl"}))
    scratch = tempfile.TemporaryDirectory(prefix="gpu_glsl_lib_")
    for name, sub in (("mesa", None), ("angle", "gles_angle"), ("swiftshader", "gles_swiftshader")):
        if not want(name):
            continue
        env = None
        if sub is not None:
            env = emu_env(sub, scratch.name)
            if env is None:
                notes.append("%s: skipped (no %s/%s: tools/android_deps.sh installs the emulator)" % (name, EMU, sub))
                continue
        res, ctx = context_check(exe, env, name)
        if res is None:
            if name == "mesa":
                print("FAIL mesa: %s" % ctx)
                fails += 1
            else:
                notes.append("%s: skipped (%s)" % (name, ctx))
            continue
        notes.append("%s: %s" % (name, ctx))
        cols.append((name, {k: tuple(v) for k, v in res.items()}))
    if not a.no_mali and want("mali"):
        mc = which("MALIOC", ["malioc"], ["/opt/gpu-tools/arm-ps/*/mali_offline_compiler/malioc",
                                          "/opt/gpu-tools/arm-ps/mali_offline_compiler/malioc"])
        if mc is None:
            notes.append("mali: skipped (no malioc: tools/gpu_shader_deps.sh --mali, docs/gpu-compat.md)")
        else:
            rc, ver = run([mc, "--version"])
            notes.append("mali: %s" % " ".join(ver.split()[:12]))
            for t in MALI_TARGETS:
                cols.append((t.replace("Mali-", "mali-"), {p["name"]: mali_check(mc, d, p, t) for p in es}))
    if os.environ.get("ADRENO_CMD") and want("adreno"):
        cols.append(("adreno", {p["name"]: adreno_check(os.environ["ADRENO_CMD"], d, p) for p in es}))

    # the lines: every not-ok result (all with -v), then the table
    names = [p["name"] for p in progs if p["api"] == "es"] + [p["name"] + " [gl]" for p in progs if p["api"] == "gl"]
    for c, r in cols:
        for n, (v, msg) in r.items():
            if v == "FAIL":
                fails += 1
            if v != "ok" or a.v:
                print("%-4s %-12s %s%s" % (v, c, n, (": " + msg) if msg else ""))
    short = {"ok": "ok", "warn": "warn", "FAIL": "FAIL", "skip": "-"}
    escols = [(c, r) for c, r in cols if c != "glslang-gl"]
    if es and escols:
        w = max(len(p["name"]) for p in es)
        print("\n%-*s  %s" % (w, "ES 3.00 program (- = skipped)", " ".join(c for c, _ in escols)))
        for p in es:
            cells = []
            for c, r in escols:
                v = r.get(p["name"])
                cells.append("%-*s" % (len(c), short[v[0]] if v else ""))
            print("%-*s  %s" % (w, p["name"], " ".join(cells)))
    for c, r in cols:
        if c == "glslang-gl":
            n = {k: sum(1 for v, _ in r.values() if v == k) for k in short}
            print("desktop GL 3.30 / 4.50 programs, glslang: %d ok, %d with warnings, %d failed, %d skipped" % (
                n["ok"], n["warn"], n["FAIL"], n["skip"]))
    for n in notes:
        print("note " + n)
    run_cols = ", ".join(c for c, _ in cols)
    print("gpu_glsl_check: %s (%d programs ES, %d GL; %s%s)" % ("FAIL" if fails else "PASS", len(es), len(progs) - len(es),
                                                               run_cols, "; %d failed" % fails if fails else ""))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
