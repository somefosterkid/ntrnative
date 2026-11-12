#!/usr/bin/env python3
"""port/gpu's programs in MSL for the Metal core (src/gpu_mtl.m; docs/gpu-backends.md, M1), generated from the GLSL.

    gpu_msl.py gen GLSLANG SPIRV_CROSS      regenerate port/gpu/msl/*.metal (needs the two tools; the build does not)
    gpu_msl.py --list                       the generated files, one absolute path a line (meson's input list)
    gpu_msl.py embed OUT.c                  check that every msl/*.metal was made from the GLSL as it is now (a SHA-1 of
                                            each program's GLSL in its first line) and write OUT.c: gpu_msl_src[], the
                                            programs by name; a stale or missing file fails the build with the command

Each program is the text src/gpu_gl.c's build_program gives GL, with the SPIR-V header instead of the GL one: `#version
310 es` (glslang's SPIR-V needs 3.10; nothing past 3.00 is used), GPU_SPIRV (the uniform block's binding) and GPU_SPAN_BUF
(pass R reads the span records from pass S's buffer, shaders/g00_head.glsl). The fragment programs: the header, G_K (and G_FILTER
for pass R's "#Kf"), the head, every other *.glsl by name, the pass's main; the full-screen vertex shader alone (gl_VertexID
is gl_VertexIndex in Vulkan GLSL); pass S's kernel: shaders/g02_setup.vert with S_COMP after the head. glslang -V (Vulkan SPIR-V, the opaque uniforms'
bindings mapped by glslang) -> SPIRV-Cross --msl (MSL 2.1; the resources numbered in order, the core finds them by name
through the pipeline's reflection). The block's argument is renamed gU and the span buffer's gSpan, so the names are stable.
Made with glslang 16.6.0 (github.com/KhronosGroup/glslang releases, glslang-16.6.0-macos-universal-release.tar.gz) and
SPIRV-Cross vulkan-sdk-1.4.363.0 (github.com/KhronosGroup/SPIRV-Cross, built with cmake: build/spirv-cross).
"""
import hashlib
import pathlib
import re
import subprocess
import sys
import tempfile

HERE = pathlib.Path(__file__).resolve().parent
SHADERS = HERE.parent / "shaders"
MSL = HERE.parent / "msl"
UNI_H = HERE.parent / "src" / "gpu_uni.h"
HEADER = ("#version 310 es\n#define GPU_SPIRV 1\n#define GPU_SPAN_BUF 1\nprecision highp float;\nprecision highp int;\n"
          "precision highp usampler2D;\nprecision highp sampler2D;\nprecision highp isampler2D;\n")
# (program name as gpu_gl_program's fs, stage, file of the main, G_K or None)
PROGRAMS = [("g00_full.vert", "vert", "g00_full.vert", None)]
for fs in ("g00_raster.frag", "g06_edgemap.frag", "g06_post.frag"):
    PROGRAMS += [(fs, "frag", fs, None), (fs + "#2", "frag", fs, 2)]
PROGRAMS += [("g00_raster.frag#1f", "frag", "g00_raster.frag", 1), ("g00_raster.frag#2f", "frag", "g00_raster.frag", 2)]  # G_FILTER
PROGRAMS += [("g06_post.frag#2+P_FAST", "frag", "g06_post.frag", 2)]  # pass P's first draw (shaders/g06_post.frag)
PROGRAMS += [(fs, "frag", fs, None) for fs in ("g07_compose.frag", "g08_smaa_edge.frag", "g08_smaa_weight.frag",
                                                "g08_smaa_blend.frag")]
PROGRAMS += [("g07_compose.frag#1+C_PLAN", "frag", "g07_compose.frag", 1)]  # pass C's plan (shaders/g07_compose.frag)
PROGRAMS += [("g02_setup.comp", "comp", "g02_setup.vert", None)]
# the fast 3D renderer (src/g10_fast_mtl.inc; docs/gpu-hwraster.md): the triangles' fragment programs read the attachment, MSL 2.3
# with framebuffer fetch (FETCH); its vertex program pulls src/g10_fast.c's vertices from buffer hwV and the polygons' edge
# functions from hwE (hwU the grid's size)
PROGRAMS += [(n, n.split(".")[1], n, None) for n in ("g10_fast.vert", "g10_fast.frag", "g10_clear.frag",
                                                   "g10_fast4.vert", "g10_fast4.frag", "g10_clear4.frag")]
PROGRAMS += [("g10_fast.frag#1f", "frag", "g10_fast.frag", 1), ("g10_fast4.frag#1f", "frag", "g10_fast4.frag", 1)]  # G_FILTER
# the 4-sample variant (BWPORT_FAST4, src/g10_fast_mtl.inc): keep
PROGRAMS += [("g10_fast4.frag" + f + "+G10_KEEP", "frag", "g10_fast4.frag", 1 if f else None) for f in ("", "#1f")]
FETCH = ("g10_fast.frag", "g10_fast.frag#1f", "g10_fast4.frag", "g10_fast4.frag#1f") + tuple(
    n for n, _, m, _ in PROGRAMS if "+G10_" in n and m != "g10_pack.frag")
TOOLS = "glslang 16.6.0, SPIRV-Cross vulkan-sdk-1.4.363.0"
# Programs that must round as Apple's GL compiles them, where relaxed math does not: (edits of the GLSL, lines put in the MSL).
# SMAA's weight pass: its searches stop on exact compares of texture coordinates, and at 3x (1/w inexact) one rounding moves
# an end by two pixels. Probed against GL (docs/gpu-backends.md, "SMAA's weight pass"): no reassociation, every a * b + c fused
# across statements, and the +16 search ends as gl_FragCoord * aMetrics + (16 + 1.25) * aMetrics.
FIXES = {"g08_smaa_weight.frag": (
    [("                 vec4(offset0.xz, offset1.yw);\n", "                 vec4(offset0.xz, offset1.yw);\n"
      "  offset2.yw = gl_FragCoord.xy * aMetrics.xy + (16.0 * aMetrics.xy + 1.25 * aMetrics.xy); // gpu_msl.py FIXES\n")],
    ["#pragma METAL fp math_mode(safe)", "#pragma METAL fp contract(fast)"])}


def out_name(name):
    return MSL / (name.replace("#", "_k").replace("+", "_") + ".metal")


def glsl(name, stage, main, k):
    src = {p.name: p.read_text() for p in SHADERS.iterdir() if p.suffix in (".glsl", ".vert", ".frag")}
    if stage == "vert":
        return HEADER + src[main].replace("gl_VertexID", "gl_VertexIndex")
    if stage == "comp":
        return HEADER + "#define S_COMP 1\n" + src["g00_head.glsl"] + src[main]
    libs = [src[n] for n in sorted(src) if n.endswith(".glsl") and n != "g00_head.glsl"]
    base = name.split("+")[0]
    kdef = ("#define G_K %d\n" % k if k else "") + ("#define G_FILTER 1\n" if "#" in base and base.endswith("f") else "")
    for d in name.split("+")[1:]:  # "name.frag#K+DEF": DEF defined too, as src/gpu_gl.c's build_program does (here also +DEF+DEF)
        kdef += "#define %s 1\n" % d
    text = HEADER + kdef + src["g00_head.glsl"] + "".join(libs) + src[main]
    for old, new in FIXES.get(name, ([], []))[0]:
        if text.count(old) != 1:
            sys.exit("gpu_msl.py: %s: FIXES' text is not in the GLSL once:\n%s" % (name, old))
        text = text.replace(old, new)
    return text


def digest(name, text):
    return hashlib.sha1((TOOLS + "\n" + "".join(FIXES.get(name, ([], []))[1]) + text).encode()).hexdigest()


def check_block():
    """the GLSL block's fields against src/gpu_uni.h's GpuU, in order (names; the C side's _Static_assert has the size)"""
    head = (SHADERS / "g00_head.glsl").read_text()
    body = head[head.index("uniform GpuU {"):]
    body = body[:body.index("};")]
    g = re.findall(r"^\s+(?:int|uint|float|vec4)\s+(\w+)", body, re.M)
    c_body = UNI_H.read_text()
    c_body = c_body[c_body.index("typedef struct GpuU {"):c_body.index("} GpuU;")]
    c = []
    for decl in re.findall(r"^\s+(?:u?int32_t|float)\s+([^;]+);", c_body, re.M):
        c += [re.sub(r"\[.*", "", v).strip() for v in decl.split(",")]
    c = [n for n in c if n != "pad"]
    if g != c:
        sys.exit("gpu_msl.py: shaders/g00_head.glsl's GpuU and src/gpu_uni.h's differ:\n  %s\n  %s" % (g, c))


def gen(glslang, spvc):
    check_block()
    MSL.mkdir(exist_ok=True)
    for name, stage, main, k in PROGRAMS:
        text = glsl(name, stage, main, k)
        with tempfile.TemporaryDirectory() as d:
            f, spv = pathlib.Path(d) / ("p." + stage), pathlib.Path(d) / "p.spv"
            f.write_text(text)
            r = subprocess.run([glslang, "-V", "--auto-map-bindings", "--auto-map-locations", "-S", stage, "-o", str(spv),
                                str(f)], capture_output=True, text=True)
            if r.returncode != 0:
                sys.exit("%s: glslang:\n%s%s" % (name, r.stdout, r.stderr))
            ver = ["--msl-version", "20300", "--msl-framebuffer-fetch"] if name in FETCH else ["--msl-version", "20100"]
            msl = subprocess.run([spvc, "--msl"] + ver + [str(spv)], check=True, capture_output=True,
                                 text=True).stdout
        for typ, new in (("GpuU", "gU"), ("GpuSpanIn", "gSpan"), ("GpuSpanOut", "gSpan"), ("HwV", "hwV"), ("HwU", "hwU"), ("HwE", "hwE")):
            m = re.search(r"\b%s& (\w+) \[\[buffer\(" % typ, msl)
            if m:
                msl = re.sub(r"\b%s\b" % re.escape(m.group(1)), new, msl)
        lines = "".join(l + "\n" for l in FIXES.get(name, ([], []))[1])
        msl = msl.replace("using namespace metal;\n", "using namespace metal;\n" + lines, 1)
        hdr = ("// %s: generated by port/gpu/tools/gpu_msl.py from port/gpu/shaders (%s); do not edit.\n"
               "// glsl-sha1 %s\n" % (name, TOOLS, digest(name, text)))
        out_name(name).write_text(hdr + msl)
        print("%s: %d lines" % (out_name(name).name, msl.count("\n")))


def c_string(text):
    out = []
    for line in text.splitlines():
        out.append('    "%s\\n"' % line.replace("\\", "\\\\").replace('"', '\\"'))
    return "\n".join(out)


def embed(out):
    check_block()
    stale = []
    texts = []
    for name, stage, main, k in PROGRAMS:
        p = out_name(name)
        t = p.read_text() if p.exists() else ""
        m = re.search(r"^// glsl-sha1 (\w+)$", t, re.M)
        if m is None or m.group(1) != digest(name, glsl(name, stage, main, k)):
            stale.append(p.name)
        texts.append((name, t))
    if stale:
        sys.exit("port/gpu/msl is stale (%s): the GLSL changed since it was generated. Regenerate it with\n"
                 "  python3 port/gpu/tools/gpu_msl.py gen GLSLANG SPIRV_CROSS\n"
                 "(glslang 16.6.0 and SPIRV-Cross vulkan-sdk-1.4.363.0: tools/gpu_msl.py says where to get them) and commit "
                 "port/gpu/msl." % ", ".join(stale))
    with open(out, "w") as f:
        f.write("/* generated by port/gpu/tools/gpu_msl.py from port/gpu/msl: do not edit */\n")
        f.write('#include "gpu_mtl.h"\n\n')
        f.write("const GpuMslSrc gpu_msl_src[] = {\n")
        for name, t in texts:
            f.write('  {"%s",\n%s},\n' % (name, c_string(t)))
        f.write("  {0, 0}};\n")


def main():
    a = sys.argv[1:]
    if a == ["--list"]:
        for name, *_ in PROGRAMS:
            print(out_name(name))
    elif len(a) == 3 and a[0] == "gen":
        gen(a[1], a[2])
    elif len(a) >= 2 and a[0] == "embed":
        embed(a[1])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
