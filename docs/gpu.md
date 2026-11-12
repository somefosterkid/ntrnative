# port/gpu: the DS 3D at N x on the GPU (GLES3)

port/gpu draws the DS's 3D layer, and engine A's compositing with it, on the GPU at N times the DS resolution
(N = 1..8), so the game's models get smooth edges and fine texture detail on a big screen, while the 2D stays the DS's
own pixels and the picture at N = 1 stays the DS's. It sits beside port/vcore (docs/vcore.md: the clean-room software
DS video core that draws the port today, native 256x192 per screen) and reuses vcore's CPU code for everything that is
not per pixel: the geometry engine, the polygon sort, the edge walk, the 2D layers. The target API is OpenGL ES 3.0 (iOS
through ANGLE on Metal, Android, desktop GL 3.3+ with the same shaders, Mesa llvmpipe headless for the tests); a thin
native Metal backend can come later with the same data and the same shader logic.

The maintainer asked for it: upscaling through a GPU 3D path, iOS-friendly. g59 (gu59) wrote this design, the
skeleton (port/gpu: a headless GLES3 context, the frame driver, a first raster shader, `gpu_render`, the compare tool)
and the piece specs below for a wave of agents.

Clean room, as for vcore: GBATEK (<ref>/gbatek/) is the spec, melonDS's oracle and vcore are black boxes to compare
with. Do NOT read any emulator's GPU/OpenGL/Vulkan/compute renderer source (melonDS's GL and compute renderers,
DeSmuME's OpenGL renderer, NooDS's): this design must not be derived from them. vcore's own source is ours: read it.

## The design in ten lines

1. The CPU runs vcore's geometry engine unchanged (p13-p15, p18's sort): `s->geo` holds the clipped, lit, transformed
   polygons in drawing order, with clip coordinates, colours and texture coordinates per vertex.
2. The CPU walks each polygon's edges with vcore's own rules (p16's edge walk, factored out: piece G2) on a screen N
   times bigger and writes one **span record** per polygon and row: where the row starts and ends, the attributes at
   both ends, the edge runs. At N = 1 these are exactly vcore's rows, so coverage at 1x is exact by construction.
3. The GPU runs one full-screen fragment pass, **the polygon loop**: every pixel walks the list of polygons that touch
   its band of rows, in drawing order, reads its row's span record, and if it is inside, interpolates, samples the
   texture (decoded in the shader from the raw VRAM), and applies the DS's per-pixel rules (depth test, alpha test,
   translucency, polygon IDs, shadows, fog and edge flags) to a pixel state kept in registers. No fixed-function depth,
   stencil or blending is used, so no extension (framebuffer fetch, interlock) is needed and the order is exact.
4. A second pass does the DS's post effects (edge marking, fog, antialiasing) from the first pass's outputs.
5. A third pass composites engine A at N x: vcore draws the 2D layers (BGs, OBJs, windows) at 1x on the CPU as today,
   the shader reads them nearest-neighbour and the 3D layer at full resolution, and applies the DS's priorities,
   windows, blending (the 3D layer's alpha included) and master brightness. Engine B is vcore's 1x screen, scaled.
6. Display capture stays at 1x: a frame that captures also runs vcore's CPU 3D, and the capture is vcore's (the game
   reads it back).
7. `Renderer=vcore|gpu|libntr` and `Scale=N` in sim_config.ini and the Config window choose it; the bridge
   (port/bridge/gpu_bridge.c) gathers the frame as today, runs the CPU half on its worker and the GL half on libntr's
   render thread, and hands libntr a GL texture to show.
8. Tests run by CPU: Mesa llvmpipe through EGL surfaceless, no window, no GPU. At N = 1 the GPU's 3D layer is compared
   with vcore's (`gpu_compare.py`, and `vcore_check pNN --oracle gpu=build/gpu/gpu_render` on every piece's fuzz
   scenes); at N > 1 the N x layer is downsampled and compared with a tolerance.

## Layout and ownership

```
port/gpu/
  meson.build                   its own meson project (like port/vcore): libgpu, gpu_render, gpu_info; vcore's sources
                                are compiled in from ../vcore (absolute paths); EGL + GLESv2 from Mesa for the tools
  include/gpu.h                 the public interface                                                 SKELETON
  src/gpu_gl.h, gpu_gl.c        the GL function table (loaded by name: eglGetProcAddress / SDL_GL_GetProcAddress),
                                shader compile + link, data textures                                 SKELETON (+ G1)
  src/gpu_egl.c                 the headless context (EGL, Mesa surfaceless platform)               SKELETON (+ G1)
  src/g00_frame.c, g00_frame.h  the frame driver: uploads, the passes, readback; GpuCore            SKELETON (G0)
  src/g02_spans.c, .h           the span records (skeleton: a plain pixel-centre rule; G2: vcore p16's rules)  G2
  src/gNN_*.c                   piece NN's CPU side (its uploads, tables), when it needs one          PIECE NN
  shaders/g00_head.glsl         the shared declarations: samplers, records, the pixel state         SKELETON
  shaders/g00_raster.frag       the polygon loop (main of pass R)                                    SKELETON (G0)
  shaders/g03_pixel.glsl        span interpolation: colour, texcoords, w, depth                      G3
  shaders/g04_texture.glsl      texture decode + blending (modulate, decal, toon, highlight)         G4
  shaders/g05_plot.glsl         the per-pixel rules: depth/alpha test, translucency, IDs, shadows    G5
  shaders/g06_post.frag         pass P: edge marking, fog, antialiasing                              G6
  shaders/g07_compose.frag      pass C: engine A's compositing at N x                                G7
  shaders/g00_full.vert         the full-screen triangle                                             SKELETON
  tools/embed_glsl.py           shaders/*.glsl|frag|vert -> a C table (build step)                   SKELETON
  tools/gpu_render.c            gpu_render F.vcs OUT.png [--3d L.png] [--scale N] [--hi H.png] ...  SKELETON
  tools/gpu_info.c              the context's GL_RENDERER / version / limits                         SKELETON
  tools/gpu_compare.py          the GPU's 3D layer vs vcore's over a directory or vcore_check's scenes (1x exact,
                                N x downsampled)                                                       G1
  tools/mingw_check.sh          compiles libgpu's GL-facing files for win64 (clang --target mingw, no EGL)  G1
port/bridge/gpu_bridge.c        the port's side (G8)                                                 G8
```

- A piece edits only its own files (gNN_*, shaders/gNN_*); the SKELETON files change by the main session or on a
  request in the piece's report (same rule as vcore). The shader files are concatenated in a fixed order
  (g00_head.glsl, then every gNN_*.glsl by number, then the pass's main), so a piece's functions are visible to the
  mains and to later pieces; each piece's functions keep the signatures given below.
- vcore stays the reference and is not changed for the GPU except by the vcore piece owners' rules (a vcore change
  keeps vcore byte-identical: `vcore_same.py` 0 differ). G2's p16 refactor is the one planned vcore change.
- C11 like vcore (gcc, clang, mingw). GLSL ES 3.00: no compute, no SSBOs, no image load/store, no 64-bit integers, no
  doubles; integer textures, `texelFetch`, `uint`, dynamic loops, 4 colour attachments. The same source compiles as
  desktop GLSL 3.30 core with a different first line (`gpu_gl.c` prepends `#version 300 es` + precision lines, or
  `#version 330 core`).

## The frame

```
CPU (vcore, C)                                   GPU (GLSL ES 3.00)
vc_vram_map, geometry (p13-p15), sort (p18)
spans at N x (G2) ----- span records ------------> pass R: the polygon loop          -> color RGBA8UI, aux RGBA32UI
polygon table, band lists, VRAM tex/pal, regs --->   (G3 pixel, G4 texture, G5 plot)
                                                   pass P: post (G6)                  -> the 3D layer at N x (RGBA8UI)
engine A at 1x without the 3D (vcore a1) -------> pass C: compose engine A (G7)      -> the top screen at N x
engine B at 1x (vcore) --------------------------> scaled (nearest or a filter)      -> the bottom screen
capture frames: vcore's CPU 3D + capture at 1x (the bank the game reads)
```

1. **Geometry** (`gpu_geometry(s)`): `vc_vram_map`, the reset of the three geometry pieces and every GX3D entry through
   `vc_gx_command`, as `vc_3d_frame` does before its raster (the skeleton has its own copy of that loop; a
   `vc_3d_geometry()` split in p20_frame.c is a request to piece 20). Nothing per pixel.
2. **Spans** (`gpu_spans(g, s)`, G2): for each polygon in `s->geo.order`, its N x vertex positions (N = 1: `sx, sy`,
   exactly; N > 1: the viewport transform of the clip coordinates at N times the viewport, see "Positions at N x"),
   then the edge walk, one record per covered row. The polygon table gets its first and last row and its first record.
   Since P1 (docs/gpu-cost.md) the CPU stops after p16's per-polygon setup (vc_span_poly and the grid/Dot/Line decisions)
   and writes a few texels a polygon (g02_spans.h GpuSetup); **pass S** (shaders/g02_setup.vert: one draw of GL_POINTS,
   one vertex per record, the rasterizer off) runs vc_span_row for each (polygon, row) in 32-bit integers and float-float, and
   transform feedback writes the twelve texels (48 interleaved components; GLES 3.0 guarantees 64) into a buffer that one
   PIXEL_UNPACK_BUFFER TexSubImage2D copies into the span texture (RGBA32UI holding the floats' bits, record-major as before
   P1: g02_setup.h). `BWPORT_GPU_SPANS=cpu` (gpu_render `--spans cpu`) builds the records on the CPU (the prep, on the
   worker) and uploads them as they are: the reference (gpu_render `--spans-check` diffs the two field by field) and the
   fallback. A core whose pass S does not compile builds them on the GL thread from the frame (`--spans nopass` forces that
   case), so the prep never needs to know what the core can do.
   2026-11-03 (the setup memo, g02_spans.h GpuMemo): each prep keeps, by polygon RAM index, the last polygon's inputs to the
   setup (nv, POLYGON_ATTR, the viewport, each vertex's clip coordinates, screen x y z, colour and texcoords; and the build's
   own: the scale, samples, Dot and Line, DISP3DCNT bits 3-5, w-buffering, DISP_1DOT_DEPTH) and what it made: the rows, the
   records it counts, the setup texels and the band extents. A polygon whose inputs equal its entry's gets them again with its
   first record moved (standing in town ~99 % of polygons; walking, when the camera moves, almost none). Records builds
   (`BWPORT_GPU_SPANS=cpu`, the self-check) do not use it. `BWPORT_GPU_MEMOCHECK=1` builds every polygon anyway and compares
   (stderr: `MEMOCHECK: N polygons, H from the memo, D differ`); `BWPORT_GPU_MEMO=0` turns it off.
   2026-11-03: pass S has five ways (g02_setup.h GPU_PS_*), tried in turn by the self-check on the same frame while the
   records differ: `tf` (the one draw), `chunk` (draws of 1024 points in the one session), `range` (2048-record chunks, each
   bound at its own offset in its own session), `frag` (shaders/g02_setup.vert as a fragment shader, S_FRAG: four targets,
   three draws, then a draw that interleaves them into the span texture) and `cpu`; when the transform-feedback buffer is
   right and the texture is not, the copy goes up in pieces of 32 rows first. A failed check logs what transform feedback
   wrote (a GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN query), the first wrong record in the mapped buffer and in the texture,
   and the GL limits. `BWPORT_GPU_PASSS=tf|chunk|range|frag|cpu` picks the first (docs/gpu-cost.md P1,).
3. **Bands**: the rows are cut in bands of 8 DS rows (8N rows); each band lists the polygons (drawing order) whose rows
   meet it. A pixel's loop is its band's list. With pass S a polygon's columns in a band come from its edges' x range
   over the band's rows, 3 pixels wider (gpu_setup_extent: town 3x 7.3 pixel-visits a pixel against 6.8 from the records).
4. **Pass R** (shaders/g00_raster.frag): per pixel: the clear values (colour, depth, ID, fog; the rear-plane bitmap is
   G6's), then for each polygon of the band: the row range, the record, `x` inside the record's run, then
   `g_pixel` (G3: the attributes), `g_texture` (G4: the colour), `g_plot` (G5: the rules, updating the pixel state).
   Out: colour (r6, g6, b6, a5 as integers) and aux (depth 24, opaque ID, translucent ID, flags, coverage).
5. **Pass P** (G6): edge marking, fog, antialiasing: each pixel reads its own and its neighbours' colour/aux texels.
   Out: the 3D layer, RGB6 + alpha 5 like `s->f3d.out`. The skeleton's pass P copies.
6. **Pass C** (G7): engine A's line buffers from vcore (`s->p20.stage[y]`: bg[4], obj, winmask after
   `vc_render_line_a1`) uploaded as integer textures, composited per N x pixel with the 3D layer; master brightness
   and the display mode. Engine B and engine A in display modes 0/2/3 are vcore's 1x screens, scaled.
7. **Out**: a GL texture (the bridge draws it in libntr's window) or a readback (`gpu_read_3d`, `gpu_read_frame`: the
   tools).

## Why a polygon loop and not hardware triangles

The DS's rendering engine is a per-pixel state machine: a pixel's depth test can be "less" or "equal within 0x200",
translucent pixels blend by a 5-bit formula and only when the pixel underneath has alpha, a translucent polygon does not
draw over its own polygon ID, shadow polygons set and test a stencil that depends on the depth test failing and on the
opaque polygon ID, the fog flag is ANDed, edge flags and coverage are kept for the post pass. Hardware triangles plus
fixed-function depth/stencil/blend can approximate most of it but not all (the conditional blend, the equal test's two
bounds, shadows against the polygon ID); doing it exactly with hardware triangles needs programmable blending
(GL_EXT_shader_framebuffer_fetch: Apple GPUs, Mesa, Mali, Adreno; not NVIDIA or AMD on desktop GL, not ANGLE/D3D11) or
ordered interlock (ARB_fragment_shader_interlock: NVIDIA, Intel; not AMD GL), and still the DS's coverage rules
(p16's measured edge rules: X-major runs, the dropped right/bottom pixels, the flat-edge cases) are not the GPU's.

The polygon loop keeps the whole pixel state in registers and visits polygons in the DS's order, so every rule is plain
code, identical on every GPU, with ES 3.0 only. The cost is one span-record fetch per polygon in the pixel's band: the
game's frames have 55-859 polygons (corpus samples: battle 55, room 259, town 322, starter 368, fly-over 802, title
859), well under the DS's 2048, and a band holds a fraction of them. At 4x (1024x768) with ~40 polygons per band that is
~30 M record visits per frame, small for a phone GPU. If a device is too slow, the fallback is hardware triangles with
framebuffer fetch (the same `g_pixel`/`g_texture`/`g_plot` functions, called from a per-polygon fragment shader; a
later piece), and finer bins (16x16 tiles instead of rows: G11).

## The data (pass R's inputs)

All in 2D textures 1024 texels wide (`texelFetch(t, ivec2(i & 1023, i >> 10), 0)`), rebuilt each frame:

| texture | format | content |
|---|---|---|
| uPoly | RGBA32UI, 2 texels per polygon (drawing order k) | t0: POLYGON_ATTR, TEXIMAGE_PARAM, PLTT_BASE, flags (bit 0 translucent, 1 front, 2 w-buffer, 3 wire-frame, 4 drawn whole); t1: first row, last row, first span record, 0 |
| uSpan | RGBA32UI holding floats (g_span: texel 12 r + t for record r's texel t, uintBitsToFloat), 12 texels per record (g02_spans.h's GpuSpan: the layout is documented there and is the authority over this summary); written by pass S, or uploaded (BWPORT_GPU_SPANS=cpu) | t0: xs, xe (the drawn run, inclusive), den_f (the attribute fraction's denominator), den_z (depth's); t1: L.w, R.w, L.z, R.z; t2: L.c0..2 (9-bit colour: 16 c5 + 15), L.s; t3: R.c0..2, R.s; t4: L.t, R.t, row flags, 0; t5: the left edge's run lo, hi and the right edge's lo, hi (edge flags); t6, t7: the two edges' (ax, ay, dx, dy) for the coverage (G5/G6) |
| uBand | RG32UI, one per band | the band's first list entry, its count |
| uList | R32UI | the bands' polygon lists, drawing order |
| uTex | R32UI, 128K words | the texture slots as the texture unit sees them (VC_RGN_TEX, 512 KB, flattened by the CPU) (G4) |
| uTexPal | R32UI, 24K words | the texture-palette slots (VC_RGN_TEXPAL, 96 KB) (G4) |

Uniforms: the scale N, DISP3DCNT, CLEAR_COLOR/DEPTH, ALPHA_TEST_REF, the toon table (32), the edge colours (8), fog
colour/offset/table (32), DISP_1DOT_DEPTH, SWAP_BUFFERS' bits. The attachments of pass R: o0 colour RGBA8UI (r6, g6,
b6, a5), o1 aux RGBA32UI (x depth 24 bits, y opaque ID | translucent ID << 8 | flags << 16 | coverage << 24 (flags =
VC_F3_*), z w (for AA at N x), w the polygon k that wrote the pixel last (debug)).

2026-11-02: uPoly, uBand, uList, pass S's setup tables and the uTex / uTexPal pair are each GPU_RING (3) textures used in turn
(gpu_gl.h): a frame (or an in-between) writes the next one and its passes read it, so a client-memory upload never lands in a
texture an earlier frame's draw may still read (Apple's GL waited for the GPU there: docs/gpu-cost.md, P3 status).

Size: a record is 128 bytes; a 4x frame of 860 polygons with ~20 DS rows each is ~70k records, ~9 MB. G11 packs it
(edges in a per-polygon table, attributes as half floats where exact).

## Positions at N x

The DS snaps every vertex to a whole pixel (GBATEK 44: the viewport transform gives integer screen coordinates). At
N = 1 the records use `sx, sy` as vcore computed them: exact. At N > 1 the positions come from the clip coordinates
`clip[4]` (kept per vertex, clipped vertices included) with the viewport transform at N times the viewport:
`sxN = (x + w) * (vw * N) / (2w) + x1 * N`, `syN = (w - y) * (vh * N) / (2w) + (191 - y2) * N`, in 64-bit integers as
p14 does, so the vertices land on the N x grid and a polygon's edges are as smooth as N allows (no vertex wobble). The
viewport is per polygon (VIEWPORT may change mid-frame); `VC_Poly` doesn't keep it: G9 asks piece 14 to keep the
viewport each polygon was assembled with (in VC_P14, indexed by polygon). Until then the skeleton uses the last
VIEWPORT of the frame (the game sends one per frame).

The depth `sz` comes from the clip z the same way at any N (it doesn't depend on N); w is clip w.

## The DS rasterizer at N x, rule by rule

"Exact" = the GPU at N = 1 must give vcore's bytes; "N x" = what the rule becomes at N > 1. CPU = in the span builder,
R/P/C = in which pass.

| rule (GBATEK) | where | at 1x | at N x |
|---|---|---|---|
| edge walk: which pixels a row covers, X-major runs, Y-major single pixels, the dropped right and bottom edges of opaque polygons, the "drawn whole" polygons (translucent with blending, edge marking, AA, wire-frame), the vertical right edge, flat top/bottom rows, the measured slope precision (51, 44; p16) | CPU (G2) | exact: p16's code | the same rules on the N x grid (as if the DS's screen were N times bigger); the dropped edge is 1 N x pixel |
| attribute interpolation: along edges perspective-correct by w, across the row by the measured fractions `(x - xs) / den_f`, depth `(x - xs) / den_z`; 9-bit colour with 3 fraction bits; texcoords 12.4 | CPU edges, R (G3) | float vs vcore's double: a rare 1-level miss (tolerance); exact where w is equal | the same formulas on the finer grid: smoother gradients, texcoords per N x pixel |
| depth: z-buffer screen-linear 24 bits, or w (SWAP_BUFFERS bit 1); the test "less", or "equal within 0x200" (POLYGON_ATTR bit 14) | R (G3, G5) | 24-bit integers in the record ends, the lerp in fp32 (exact on whole steps; z-fights may flip, tolerance) | unchanged |
| 1-dot polygons and DISP_1DOT_DEPTH (35, 51): a polygon whose vertices are on one pixel draws that pixel, hidden by depth unless bit 13 | CPU (G2) | exact | a polygon that collapses to one DS pixel draws an N x N block (DS look; `Dot=block`), or one N x pixel (`Dot=fine`) |
| line segments (two equal vertices: a polygon of zero area drawn as its edge, 1 pixel wide) | CPU (G2) | exact | width N (`Line=wide`, default) or 1 |
| wire-frame (alpha 0: edges only, alpha 31) | CPU + R | exact | 1 N x pixel wide edges, or N (same option as lines) |
| texture sampling: nearest, repeat/flip/clamp, 7 formats, colour 0 transparent (47-50) | R (G4) | exact (integer decode from raw VRAM) | nearest at the finer texcoords (crisp texels); optional filtering later (G9: bilinear on the decoded texel grid, 5-bit alpha kept) |
| modulation, decal, toon / highlight from TOON_TABLE (50) | R (G4) | exact (integer formulas from p19/p17) | unchanged |
| translucency: alpha 1-30 or A3I5/A5I3, the blend `(src (a+1) + dst (31-a)) / 32` only when the pixel below has alpha, alpha = max, the depth update (bit 11), the same-ID rule, fog flag ANDed (43, 51) | R (G5) | exact (integers in registers) | unchanged per pixel |
| the sort: opaque by Y, translucent by Y or as sent (SWAP_BUFFERS bit 0) | CPU (p18) | exact | the order of the 1x sort (Y extents in DS pixels) |
| polygon IDs (opaque, translucent), the clear ID | R (G5) | exact | unchanged |
| shadows (46): ID 0 sets the stencil where the depth test fails, others draw where it is set and the ID differs, the stencil cleared after | R (G5) | exact (the stencil is a flag bit in the pixel state) | unchanged; the shadow's outline at N x |
| alpha test (ALPHA_TEST_REF), alpha 0 never drawn | R (G5) | exact | unchanged |
| clear colour/depth/ID/fog; the rear-plane bitmap (54) scrolled by CLRIMAGE_OFFSET | R (G6) | exact | the bitmap sampled nearest at (x/N, y/N) |
| edge marking (51): an edge pixel with a 4-neighbour of another ID and greater depth takes EDGE_COLOR[id >> 3] | P (G6) | exact | neighbours N x pixels away: a 1-N x-pixel outline (`Edge=fine`) or an outline N pixels wide (`Edge=ds`, default: the DS's 1 DS pixel) |
| fog (51): density by depth >> 9 from the 32-entry table with the shift, colour and alpha or alpha only | P (G6) | exact (integers) | unchanged per pixel |
| antialiasing (51): an opaque edge pixel's alpha = its coverage, blended with the farthest 4-neighbour behind | P (G6) | exact (p19's rule; coverage from the record's edge params) | the coverage of the N x pixel (smooth edges at the finer grid); neighbours 1 N x pixel away |
| the 3D layer's BG0HOFS scroll (55) | C (G7) | exact | the scroll times N |
| depth precision: 24-bit z, w 20.12 | R | uint compares, fp32 lerp | unchanged: N doesn't change depths |

Everything per pixel is integer GLSL where vcore is integer (the blend, fog, toon, modulation, tests), so 1x is exact
except where vcore itself uses doubles (the across-row lerp, perspective). Those are fp32 on the GPU: the 1x compare
uses a tolerance and reports exact % and within-tolerance % separately.

## The 2D engines and compositing

The 2D stays the DS's pixels: vcore draws engine A's BGs and OBJs and the window mask at 1x on the CPU
(`vc_render_line_a1`: `s->p20.stage[y]`), as it does now while its 3D thread runs. Pass C (G7) uploads bg[4], obj,
winmask (and bg_on, bg3d per line) as R32UI/R8UI textures and composites each N x pixel: the 2D layers at the DS pixel
(x / N, y / N) nearest, the 3D layer at the N x pixel (shifted by BG0HOFS times N). The compositor is p09's rules
(priorities, the OBJ's priority, windows, BLDCNT/BLDALPHA/BLDY with the 3D layer as a first target using its own alpha,
semi-transparent and bitmap OBJs) and p10's (display mode 1, master brightness, forced blank), ported to GLSL with the
same integer arithmetic: at N = 1 with vcore's own 3D layer as input it must give vcore's engine A picture byte for
byte. Display modes 0, 2, 3 and engine B: vcore's 1x screen (`s->eng_out`), drawn at N x by nearest-neighbour (a
filter, xBR/ScaleFX style written from its paper, is a later option for the 2D: it must not touch the 3D layer).

2026-11-02: the bridge's worker packs the stage and the 1x lines into the slot's G07Pack (g07_pack, after g07_stage_build) and
the GL thread's g07_compose_packed uploads a pack only when it is not the one up: a frame's in-between pictures and the frame
itself share one upload (at 120 Hz with Smooth motion the same stage went up twice a tick, packed on the GL thread each time).
The textures keep their size (sub-image uploads). With SMAA, pass C also does SMAA's neighbourhood blending where it reads the
3D layer ("Antialiasing: SMAA").

## Display capture

The game reads display captures back (the title, the battle intro, capture fades): DISPCAPCNT writes the composited
picture or the 3D layer into a VRAM bank the game then shows as a BG or reads as data. A capture must hold the DS's
1x pixels, so a frame with DISPCAPCNT's enable bit runs vcore's whole frame on the CPU (its 3D too) and the bridge
stores that capture into libntr's bank exactly as today (vcore_bridge.c's StoreCapture); the GPU still draws the N x
picture shown on that frame. A captured image shown later is 1x (scaled like any BG). Cost: vcore's 3D on capture
frames only (1-4 ms).

## The renderer switch and the bridge (G8)

- libntr (a new patch after libntr-vbridge.patch): `Renderer=vcore|gpu|libntr` (SIM_CONFIG_RENDERER_GPU), `Scale=1..8`
  in sim_config.ini, both in the Config window (the ImGui combo the vbridge patch added; the ` key toggles it);
  `BWPORT_RENDERER=gpu` and `BWPORT_GPU_SCALE=N` override. A second hook, `SIM_SetFrameRendererGL(fn)`: fn returns a GL
  texture name and its size (256N x 384N, top screen first), which libntr draws with its screen quads (sampling with
  GL_NEAREST; the window's own scaling then fits it), so the picture never leaves the GPU.
- port/bridge/gpu_bridge.c: the gather is vcore_bridge.c's (SIM_VCapGather into a VCoreState). The CPU half (geometry,
  spans, bands, engine A's 2D at 1x, engine B) runs on the pipeline worker as vcore's frame does today; the GL half
  (uploads, passes R, P, C) runs in the hook on libntr's render thread, where libntr's context is current, with the GL
  state it changes saved and restored (program, textures, FBO, viewport, VAO). The shaders compile as GLSL 3.30 core
  there (libntr's context is GL 4.2 core). The picture is one frame late, as vcore's pipeline is.
- Fallback: no GL 3.3/ES 3.0, a shader that fails to compile, or `gpu_create` failing -> vcore draws (log one line).
- Both bridges stay linked; the switch is per frame (the Config window can change it while the game runs).

## iOS and Metal

- GLES 3.0 through ANGLE's Metal backend runs the same shaders (no extension used). ANGLE is the first iOS step: the
  libntr window on iOS is SDL with an ANGLE context, and gpu_gl.c loads the functions by name either way.
- A native Metal backend later: the CPU half is unchanged C; the records and textures become MTLBuffers (no 1024-wide
  texture packing needed); the three passes become fragment (or compute) functions in MSL, transliterated from the GLSL
  (the same integer code). The pieces' GLSL is written so that this is mechanical: no GLSL built-ins beyond
  texelFetch, bit operations and arithmetic.
- Apple GPUs are tile-based with unified memory: the per-frame uploads (~1-9 MB) are cheap; the loop's records come
  through the texture cache.
- The API decision (ANGLE, native Metal, or Vulkan through MoltenVK), with the probes and the order of work: docs/gpu-backends.md
  (2026-11-03; recommends native Metal with the MSL generated from this GLSL at build time, pass S as compute first).

## Precision

- Exact (integers in GLSL, same formulas as vcore): coverage (records from p16's C), texture decode, modulation, toon,
  the blend, the plot's tests and IDs, fog, edge marking, the compositor.
- fp32 (vcore: double): the across-row perspective lerp of colour, texcoords and w; z's lerp. vcore floors these
  (`floor(c / 8)`, `floor(st)`), so a value within 1e-5 of an integer can floor the other way: rare single-level misses.
  Where it matters (texcoords on texel boundaries), G3 computes the fraction as vcore does (`(x - xs) / den`) and keeps
  the perspective form `k1 / (k0 + k1)`; a later step can do the fixed-point form in 32-bit pieces if the tolerance
  numbers ask for it.
- Depth compare: uint 24-bit; the lerped depth rounds `+0.5` as vcore's clamp_depth.

## Testing by CPU

Everything runs headless here: EGL on Mesa's surfaceless platform (EGL_PLATFORM_SURFACELESS_MESA, no X, no window) with
llvmpipe (`gpu_info`: "llvmpipe (LLVM 21.1.8, 256 bits)", OpenGL ES 3.2, GLSL ES 3.20; 4 draw buffers, max texture
16384). llvmpipe is deterministic, so every check is a byte compare.

1. **1x, piece scenes**: `port/vcore/tools/vcore_check pNN --oracle gpu=build/gpu/gpu_render` runs vcore (mine) against
   the GPU path (as the "oracle": gpu_render takes the oracle's command line) on piece NN's emitted and fuzz scenes and
   reports the 3D layer's match % (pieces 13-19) or the screens' (2D pieces, for G7). A GPU piece names the vcore
   pieces whose scenes cover it (G3: p16, G4: p17, G5: p18, G6: p19; p13-p15 check the geometry hand-off).
2. **1x, the corpus**: `python3 port/gpu/tools/gpu_compare.py <ref>/corpus` (or any directory of .vcs) renders
   each frame's 3D layer with vcore_render and gpu_render and prints, per frame and in total, the exact match % (the
   3D rule of vcore_check: colour at 6 bits where alpha > 0, alpha at 5 bits) and the within-tolerance % (every channel
   within `--tol` levels, default 2), with `--diff DIR` pictures.
3. **N x**: `gpu_compare.py --scale N`: the N x layer box-downsampled to 1x (each channel's mean over the N x N block,
   alpha too) against vcore's 1x layer, within-tolerance % (edges differ by design: the score is a smoke test, a drop
   between commits is the signal), plus `--hi DIR` to look at the N x pictures.
4. **The screens**: `gpu_render F.vcs OUT.png` writes vcore's screens with the GPU's 3D layer substituted (engine A
   recomposited by vcore at 1x): the 2D vs 3D integration at 1x before G7 exists; with G7, `--compose gpu` writes the
   GPU compositor's output instead, which at 1x must equal vcore's screen byte for byte when fed vcore's 3D layer
   (`--3d-from vcore`).
5. **Unit tests**: the CPU side (spans, bands, packing) has C unit tests in port/gpu/tests (vct.h's framework, linked
   from vcore); shader pieces are tested through scenes (a hand-made .vcs with mkvcs.py or a vcore unit test's
   `vct_emit` scene).

**Round-60 baseline** (main 78fee48ef + G1; the skeleton's stand-in shaders, plain spans: the numbers are the starting
line, they must only rise). `gpu_compare.py --piece 13-19 --fuzz 100 --emit` (seed 1; "exact" is the 3D rule, "within"
is every channel within 2 levels; 730 scenes, 218 identical):

| piece | scenes | identical | exact % | within % |
|---|---|---|---|---|
| p13 matrix | 101 | 47 | 99.44 | 99.91 |
| p14 polygons | 106 | 36 | 98.11 | 99.88 |
| p15 lighting | 108 | 28 | 96.10 | 96.83 |
| p16 rasterizer | 109 | 15 | 93.29 | 95.05 |
| p17 textures | 100 | 23 | 88.21 | 88.89 |
| p18 translucency | 101 | 45 | 99.21 | 99.25 |
| p19 post-processing | 105 | 24 | 53.46 | 56.10 |

`gpu_compare.py <ref>/corpus` (56 frames): 1x 2 identical, exact 31.12 %, within 32.52 %; `--scale 2`: within
32.37 % (exact 31.17 % at the downsample); `--api gl`: the same bytes as ES. `vcore_check pNN --oracle
gpu=build/gpu/gpu_render` (100 fuzz + the emitted scenes) gives the same per-piece match. `GPU_API=es` and
`GPU_API=gl` give byte-identical PNGs on all 56 corpus frames (1x) and at `--scale 3`.

**Round-61 after G2** (spans from vcore p16's own row setup; same command, seed 1, 730 scenes, 574 identical):

| piece | scenes | identical | exact % (was) | within % (was) |
|---|---|---|---|---|
| p13 matrix | 101 | 100 | 100.00 (99.44) | 100.00 (99.91) |
| p14 polygons | 106 | 105 | 100.00 (98.11) | 100.00 (99.88) |
| p15 lighting | 108 | 86 | 98.86 (96.10) | 98.86 (96.83) |
| p16 rasterizer | 109 | 94 | 98.85 (93.29) | 98.85 (95.05) |
| p17 textures | 100 | 31 | 91.13 (88.21) | 91.39 (88.89) |
| p18 translucency | 101 | 91 | 99.68 (99.21) | 99.68 (99.25) |
| p19 post-processing | 105 | 67 | 98.88 (53.46) | 99.12 (56.10) |

Corpus (56 frames): 1x exact 33.43 % (was 31.12), within 33.47 %, 6 identical; the corpus frames are mostly black on the GPU
(the same at the round-60 baseline: textures or the clear in real frames, not the spans: see below). `--scale 3` on
p16/p19 fuzz: within 97.7 / 98.7 %, no errors.

**Round-61 after G9's first jobs** (main 48581d8b3 + the palette-unit fix, per-polygon viewport):
the corpus's black frames were one bug: `gpu_g06_bind_raster` made its rear-plane texture with `gpu_gl_data_tex`, which binds
to the ACTIVE texture unit, and G4's palette texture was bound there (unit 5): every palette format sampled black on
a frame's first render (and on every frame with the rear-plane bitmap). Fixed in g06_post.c (ActiveTexture before the upload).
Corpus 1x: exact 33.43 % -> 99.85 %, within tol 33.47 -> 99.93 %, 16 identical (was 6); p13-p19's 730 scenes: exact 99.97 %,
709 identical (p17 textures 91.13 -> 100.00 %). N x on the corpus, box-downsampled vs vcore's 1x layer: `--scale 2` within
tol 92.1 %, `--scale 4` 90.7 %; with `--noedge 1` (the pixels within 1 DS pixel of a discontinuity of vcore's layer left
out: polygon outlines and texture edges, 59 % of the pixels kept) 99.94 % and 99.93 %: the interior matches, the rest is the
finer geometry and sampling by design (the DS snaps vertices to whole pixels: its interpolated texture coordinates differ
from the N x ones by the snap times the gradient; no sub-pixel shift of the sample point improved it). `GPU_DOWN=point`
(gpu_render) downsamples by the block's top-left pixel instead of the mean. Per-polygon viewport: vcore p14 keeps it
(`VC_Poly.viewport`, a new field; vcore_same 0 differ on 2126 scenes), gpu_vertex_pos takes the polygon. Cracks:
`build/gpu/gpu_crack F.vcs N` (GPU_FLAT coverage at N x inside vcore's 1x silhouette eroded by 2 DS pixels, backdrops
with a bounding box of a third of the screen left out so they cannot hide a crack): 0 at N = 2, 3, 4, 8 on town (f002500),
the flyover town (f008800, f009300), the room and the title; the battle (a few polygons, one big ground) has 2 at the
silhouette's rim at N = 4. Not done yet: bilinear texture filtering, AA at N x (ROW_AA is 1x only), pass P output accessor,
G7 compositor in the bridge.

**The span check** (`build/gpu/gpu_cover F.vcs`, G2): the GPU runs with `GPU_FLAT=1` (G3-G5 off: every covered pixel is
written opaque white, with its edge flag, coverage and polygon) and vcore's p16 draws the same polygons through a
recording plot. Covered set, edge flags, the polygon that covered each pixel last and (with antialiasing) the coverage
must be equal at 1x: 786 scenes (the corpus and p13-p19's emitted and fuzz scenes) 0 differ. `gpu_tests`: the packing's
C tests (port/gpu/tests/t02_spans.c).

## Build, run, test

```
meson setup build/gpu port/gpu && ninja -C build/gpu      # libgpu, gpu_render, gpu_info (needs EGL + GLESv2 dev files)
build/gpu/gpu_info                                         # the headless context: renderer, version, limits
build/gpu/gpu_render F.vcs OUT.png [--3d L.png] [--scale N] [--hi H.png] [--dump-vram V.bin] [-q] [--time]
python3 port/gpu/tools/gpu_compare.py DIR|F.vcs [--scale N] [--tol T] [--diff DIR] [--show K]
port/vcore/tools/vcore_check p16 --oracle gpu=build/gpu/gpu_render   # vcore vs the GPU on p16's scenes
BWPORT_GPU_SHADERS=port/gpu/shaders build/gpu/gpu_render ...         # read the shaders from disk (no rebuild)
build/gpu/gpu_render F.vcs OUT.png --api metal ...                   # macOS: the Metal core (docs/gpu-backends.md, M1)
python3 port/gpu/tools/gpu_msl.py gen GLSLANG SPIRV_CROSS            # after a GLSL change: regenerate port/gpu/msl (Apple builds check it)
```

- **The uniforms** (2026-11-03): every non-opaque uniform of every program is a field of one std140 block, `GpuU` in
  shaders/g00_head.glsl, and of the C struct `GpuU` in src/gpu_uni.h, the same fields in the same order (tools/gpu_msl.py checks
  the names, gpu_uni.h's `_Static_assert` the size). A pass fills a GpuU (memset, then its fields: g00_frame.c's gpu_uni_*,
  g06_post.c, g08_smaa.c, g07_compose.c) and hands it to `gpu_gl_uniforms` (a uniform buffer at binding 0, from a ring of 32,
  orphaned and filled each draw) or to the Metal core. A new uniform is a new field in both, then `gpu_msl.py gen`. Samplers stay
  loose uniforms. Since the block, a GLSL change also needs the MSL regenerated: an Apple build fails until it is (the message
  says how), the other builds do not look at it.

## The pieces

Each piece: its files, its functions (fixed signatures), the spec it follows, the check that says it is done. The
check runs here, by CPU, through llvmpipe. "vcore" below means the reference: the GPU at 1x must match vcore, which
matches melonDS where vcore's piece is done (docs/vcore.md); a GPU miss on a scene vcore misses is not the GPU's.

**G1. Contexts and loading** — src/gpu_gl.c, src/gpu_egl.c (SKELETON, G1 extends), tools/gpu_info.c.
The function table for ES 3.0 / GL 3.3 core (one list, loaded by name from a `GpuGetProc`), the GLSL header per API
(`#version 300 es` + precision, or `#version 330 core`), the headless EGL context (surfaceless, then EGL device, then a
pbuffer on the default display), the SDL hidden-window context for a host without EGL (win64 tests under wine are not
needed: the exe's context is libntr's), errors as text. Check: gpu_info runs here; `gpu_render` gives the same bytes
with `GPU_API=es` and `GPU_API=gl` (desktop core via EGL_OPENGL_API on Mesa); the mingw build of libgpu compiles
(tools/port_win64.sh's cross files) with the Khronos headers vendored in port/gpu/include/KHR,GLES3 (MIT licence).

**G2. Spans: p16's edge walk as records** — port/vcore/src/p16_raster.c (with piece 16's rules), src/g02_spans.c/.h.
Split `vc_raster_band` into the row setup (edges, the left/right choice, the runs, `xs, xe, den_f, den_z`, L/R
attributes, the edge mask's runs, the flat-bottom row) and the pixel loop, with the row setup's result a struct
`VC_Span` (p16.h) that vcore's own pixel loop then consumes: vcore stays byte-identical (`vcore_same.py`: 0 differ on
all 2126 scenes) and gets no slower (`vcore_render --bench`). The setup takes the vertex positions as input (not from
`s->geo.vtx[].sx` directly) and the screen size (`192 * N` rows, `256 * N` columns), so `gpu_spans` calls it with the N x
positions. g02_spans.c packs `VC_Span` into GpuSpan records (8 texels) and the polygon table's rows. The 1-dot rule and
the line/wire-frame width at N x (Dot=, Line= options). Check: with G3 and G5 off (`GPU_FLAT=1`: every covered pixel
written opaque white), the alpha plane of the GPU's 3D layer equals vcore's coverage exactly on p16's emitted and fuzz
scenes and on the corpus (`gpu_compare.py --coverage`); C unit tests of the packing.

**G3. The pixel's attributes and depth** — shaders/g03_pixel.glsl.
`bool g_pixel(uint k, int x, int row, out GPx px)` (GPx: c[3] 9-bit, st, w, depth 24-bit, edge flag, coverage
inputs): the record's fraction `(x - xs) / den_f` and `(x - xs) / den_z`, the perspective form of p16's `lerp_px`
(`k0 = (1 - f) / L.w`, `k1 = f / R.w`, `q = k1 / (k0 + k1)`, w = `1 / (k0 + k1)`; linear when L.w == R.w), colour
`floor((c + 1e-6) / 8)`, `floor(st + 1e-6)`, depth z or w with `+0.5` and the clamp, the edge flag (the runs, `x == xe`,
the flat-bottom row). Check: `vcore_check p16 --oracle gpu=...`: 3D layer match >= 99.5% (exact % and the misses'
size in the report); gpu_compare on the corpus with G4/G5 stand-ins.

**G4. Textures** — shaders/g04_texture.glsl, src/g04_texture.c (the flattened slots).
`uvec4 g_texture(uint k, GPx px, uvec4 vcol)`: TEXIMAGE_PARAM's address, size, repeat/flip/clamp, the 7 formats
decoded from uTex/uTexPal (4x4 compressed with its index data in slot 1), colour 0 transparent (bit 29), PLTT_BASE units,
then vcore's `vc_tex_blend` (modulation `((a+1)(b+1)-1)/64`, decal, shadow mode, DISP3DCNT bit 0) and `vc_toon_blend`
(toon / highlight; untextured mode 2 too). The CPU side flattens VC_RGN_TEX/TEXPAL (the OR of the banks per page) into
the upload buffers. Check: `vcore_check p17 --oracle gpu=...` >= 99.5%, and 100% on scenes with ortho, whole-texel
steps (the generator's exact mode); the corpus's textured frames (3d.tex.* features) within tolerance >= 99.5%.

**G5. The plot** — shaders/g05_plot.glsl.
`void g_plot(inout GPixel S, uint k, GPx px, uvec4 col)`: p18's `vc_p18_plot` and p16's `vc_3d_depth_test` in
integers: wire-frame alpha, alpha 0, the alpha test, shadows (mask and shadow), opaque writes (colour, depth, opaque ID,
translucent ID cleared, fog and edge flags, the stencil kept), translucent (the same-ID rule, the conditional blend,
alpha max, fog AND, the depth update), the coverage written for edge pixels (emit_pixel's "changed" rule). The early
depth test for plain polygons. Check: `vcore_check p18 --oracle gpu=...` >= 99.5%; corpus frames with 3d.translucent
within tolerance >= 99.5%.

**G6. Clear and post** — shaders/g06_post.frag (pass P), shaders/g06_clear.glsl (`void g_clear(inout GPixel S, ivec2
p)`: called by pass R first).
The clear colour/depth/ID/fog and the rear-plane bitmap (slots 2/3, the offset, nearest at N x); edge marking, fog
(p19's `fog_density` with its fitted rounding), antialiasing (coverage from the record's edge params at the N x
pixel; the farthest 4-neighbour behind), with the N x options (Edge=ds|fine). Check: `vcore_check p19 --oracle gpu=...`
>= 99.5% at 1x; the corpus's 3d.edge and 3d.aa frames (18 and 35 of the 56) within tolerance >= 99.5%; the N x pictures
of the battle and the fly-over looked at (outlines one DS pixel wide with Edge=ds).

**G7. Engine A's compositor at N x** — shaders/g07_compose.frag (pass C), src/g07_compose.c.
The upload of `s->p20.stage[]` (bg[4], obj, winmask, bg_on, bg3d per line), p09's compositing and p10's display
(mode 1: the layers; modes 0/2/3: vcore's line; master brightness; forced blank) per N x pixel, the 3D layer's scroll.
Engine B: vcore's screen as a texture, nearest. `gpu_read_frame` (both screens at N x). Since 2026-11-04 in two programs:
the 2D layers' decisions once a DS pixel into a 256 x 192 RG32UI plan (the file with C_PLAN, made once a pack), then the
N x pass from the plan and the 3D layer (docs/gpu-cost.md, "Pass C in two programs"). Check: at 1x with vcore's own
3D layer uploaded as the 3D input (`gpu_render --3d-from vcore --compose gpu`), the top screen equals vcore's byte for
byte on the corpus and on `vcore_check p08/p09/p10 --oracle gpu=...` (100%); with the GPU's 3D, within tolerance.

**G8. The bridge and the switch** — port/bridge/gpu_bridge.c, a libntr patch (libntr-gpu.patch), port/meson.build (the
gpu library beside vcore's).
`Renderer=gpu`, `Scale=N`, the Config window's combo and scale, BWPORT_RENDERER=gpu / BWPORT_GPU_SCALE;
`SIM_SetFrameRendererGL`; the CPU half on the pipeline worker, the GL half in the hook with the state saved and
restored; capture frames through vcore at 1x (StoreCapture); the fallback to vcore. Check: `port_run.py` with
BWPORT_RENDERER=gpu BWPORT_GPU_SCALE=1 reaches the same frame as vcore from the saves (town, battle, room) and its
screenshots match vcore's within tolerance (the 3D layer) / exactly (the 2D); Scale=2 and 4 run; BWPORT_FPSLOG at
1x/2x/4x on llvmpipe in the report; the win64 build links (the exe on the maintainer's GPU is the maintainer's check).

**G9. N x quality** — src/g09_*.c, shader options.
The per-polygon viewport (a request to piece 14: VC_P14 keeps each polygon's viewport), positions from clip
coordinates at N x, the Dot/Line/Edge options, AA at N x, optional texture filtering (bilinear on decoded texels, the
alpha rules kept: a texel with colour 0 transparent stays a hard edge). Check: gpu_compare --scale 2/4 on the corpus
within tolerance >= 97% (edges excluded by design); no polygon cracks: a T-junction-free mesh (the corpus's town) shows
no clear-colour pixels inside its silhouette at 4x (a count in the report); pictures of the battle and the title at 4x.

**G10. iOS** — later, on a test Mac: ANGLE (Metal) context in the iOS SDL build; then optionally native Metal
(src/gpu_metal.m) with the same records. Check there: the 1x compare on the corpus on the device equals llvmpipe's.

**G11. Speed** — g00_frame.c's binning (16x16 tiles), the record packing, uploads with glTexSubImage2D into persistent
textures, pass R's early-outs (a polygon's x extent per band), timings (`gpu_render --time`, GL timer queries where
available). Check: the same bytes as before (gpu_compare --same OLD NEW: 0 differ on the corpus at 1x and 4x);
times in the report (llvmpipe here; the maintainer's GPU later).

**Round-62 G9 (gq62): options, bilinear filter, AA of DS-grid rows.** `GpuOptions` (gpu.h: dot, line, edge, filter; `gpu_set_options` on
the core, `gpu_prep_set_options` on a prep; defaults from BWPORT_GPU_DOT=block|fine, _LINE=wide|thin, _EDGE=ds|fine, _FILTER=nearest|bilinear)
and sim_config.ini `Dot=`, `Line=`, `Edge=`, `Filter=` (libntr-gpuopt.patch, after libntr-gpu.patch: sim_config.h/.c, the Config window under
"GPU scale", `SIM_GLRendererOption(which)`; the same env vars override the ini; it also moves the default Scale=2 out of RendererFromEnv's
vcore branch, where libntr-gpu.patch's hunk had put it). The bridge reads them each frame. Filter=bilinear (g04_texture.glsl `g4_filtered`, N > 1
only): the four texels around the sample point with weights in 1/16 steps from the 12.4 coordinate, interpolated on the decoded 6-bit colours;
formats with a hard alpha (colour-0 transparent, 4x4 modes, direct bit 15) keep the nearest texel's alpha (a transparent one stays transparent,
an opaque one stays 31) and transparent neighbours take no part in the colour; A3I5/A5I3 interpolate alpha and weight colour by it. Corpus
within tol, edges excluded (--noedge 1): 2x 99.94 -> 99.82 %, 4x 99.93 -> 99.81 % with the filter on (it blurs by design; default off, 1x
untouched). AA at N x: regular polygons already got AA on the N x grid (the span walk runs vcore's AA windows on the N x grid: one blended
N x pixel per edge row); what was missing was the DS-grid rows (lines walked on the DS grid and widened by N): `GPU_ROW_DSGRID`, the shader reads
the windows at pixel x / N. Defaults unchanged at 1x (0 of 56 differ); at 4x 13 frames differ by a few pixels (lines).

## Dependencies and waves

G1 and G2 first (the skeleton's context and plain spans run already, so G3-G6 can start at once on the skeleton's
plain records and switch to G2's when it lands: the record format is fixed by g02_spans.h). G3 -> G4 -> G5 are one
chain in the shader but separate files with fixed signatures: each can be written against the skeleton's stand-ins.
G6 needs G5's aux outputs (fixed by g00_head.glsl). G7 is independent of G3-G6 (it can use vcore's 3D layer as input).
G8 needs G7 for the full picture but can start with 3D-only (vcore's compositor at 1x fed the GPU's downsampled layer).
G9 after G2/G6; G11 last.

## Lessons

- A GL upload binds to the ACTIVE texture unit: set ActiveTexture before any upload that happens between binding the frame's textures and drawing (G6's rear-plane upload replaced G4's palette on unit 5: every real frame black for a whole round, because the synthetic scenes mostly used direct-colour textures). Check pieces on the corpus, not only on their fuzz scenes.
- A wrong picture: replace one stage's output with raw values (vertex colour, texcoords, raw texture bytes, the palette read) through `BWPORT_GPU_SHADERS` (no rebuild) until one stage goes wrong.
- Coverage checks: `GPU_FLAT` still goes through pass P (edge marking recolours edge pixels): read `aux.last` from pass R or the polygon-index plane, not alpha (the clear colour has alpha too); a recording plot must write a changing value, or repeated writes hide.
- In the game, to tell the compositor from the 3D layer, read the 3D layer back too and count differing pixels where the layers are equal (`BWPORT_GPU_CHECK` does this since G11).
- A piece written against the skeleton misses neighbours merged later (G8 used a stand-in compositor until G11): rebase and name the neighbours you used.

## Open items

- A `vc_3d_geometry(s)` entry in p20_frame.c (the first half of vc_3d_frame) so that the GPU path calls vcore's own
  geometry step rather than a copy of its loop (request to piece 20).
- The per-polygon viewport in p14 (G9).
- p19's `vc_3d_post` keeps two static buffers (`marked`, `src`): two VCoreStates rendering at once on threads (a GPU
  bridge running vcore's CPU 3D for a capture frame beside the pipeline's frame) would race on them (note for piece 19 /
  G8).

**Round-61 G11 (jobs 1 and 2 done, speed spec not started)**: the bridge uses G7's pass C at every scale (no stand-in, no
readback: `gpu_3d_tex(g)` is pass P's RGBA8UI texture, handed to `g07_compose` with sc = N; engine A's 2D layers
(`g07_stage_build`) and the spans (`gpu_prepare`, a `GpuPrep` per frame in flight) are built on the pipeline worker; the
GL half uploads and draws; `gpu_render_3d` = prepare + `gpu_render_prepared`; glFinish only after `gpu_set_timing(g, 1)`,
which gpu_render sets). `BWPORT_GPU_CHECK=DIR` at 1x: a line per differing frame, and engine A's differing pixels split
into those where the 3D layer itself differs (G3/G4's 99.85 %: ~56 px per town frame, the same pixels as the corpus) and
those with an equal 3D layer (pass C's fault: 0 on town, room, caught, 30 s each). Town fps in the game on llvmpipe
(1x / 2x / 4x): 136 / 75 / 27 before, 147 / 93 / 33 after. `tools/gpu_same.sh OLD NEW [scales]` compares two
gpu_render builds byte for byte on the corpus (the G11 check; gpu_compare has no --same yet).

**Round-62 G11 (speed spec, gs62)**: bins of 16 DS columns x 8 DS rows (`uBand` has one entry per bin, bin = band * 16 + column; each
polygon's x extent per band comes from its span records' runs [xs, xe], which is exact: g_pixel draws only inside a row's
run); `g_pixel` computes the depth first and drops a pixel that fails the depth test (`g_early_on`, set by pass R per
polygon, not for shadow polygons) before colour, texcoords and the edge mask; t6/t7 are read only for rows flagged
GPU_ROW_MASK2 (more than 2 mask runs); pass E (g06_edgemap.frag, N > 1 with edge marking) makes a DS-resolution map of where a
fine-marked pixel is near, so pass P's Edge=ds thickening (12 fetches per pixel) runs only there; data textures are persistent
(TexSubImage2D; `gpu_gl_data_tex_diff` uploads only the rows of VRAM textures/palettes/rear plane that changed).
`gpu_render --time K` prints uploads / R / P per frame and the min; `GPU_CPUTIME=1 LP_NUM_THREADS=0` makes the times the
process's CPU time on one thread (stable under the shared load: wall times here vary 2x), `GPU_STATS=1` prints bin statistics.
Check: gpu_same (corpus, OLD = main 4871084f7) 0 of 56 differ at 1x, 2x, 3x, 4x; fuzz p13-p19: 726 of 730 identical as before;
in the game (town, BWPORT_GPU_CHECK) 0 differing pixels at 1x/2x/4x. Town (322 polygons), CPU time on one llvmpipe thread,
ms per frame at 1x/2x/4x: 26.7/110/445 -> 11/50/180 (R 9/33/150, P 1/12/29). Town fps in the game (lockstep --fast) 1x 150-370, 2x ~65, 4x ~39.
Not done: the span record is still 12 texels (192 B); R's cost is the per-covered-pixel g_pixel arithmetic (divisions in g_lin) on llvmpipe.

**Round-65 pass R (gp65)**: three changes, same bytes (gpu_same 0 of 56 differ at 1x/2x/3x/4x; fuzz p13-p19 726/730 identical; gpu_tests pass). (1) No
integer division in g_pixel: `g_fixq` takes a float quotient estimate and fixes it by the integer remainder (llvmpipe scalarizes idiv);
`g_lin`/`g_lini` share fq = n / d. (2) A float bound of the depth (+-8, +-520 for the equal test) rejects a pixel before the exact depth,
the fractions and the perspective terms; w is computed only for the w-buffer. (3) The bin list is RGBA32UI (k, attr, first record - first
row + bias, rows clipped to the band): a visit fetches the list entry and the record's t0 only. Town, one llvmpipe thread, ms per frame at 4x,
R 141 -> 89 (frame ~166 -> ~120); 1x R 10 -> 7.3; other scenes at 4x R: battle 64 -> 46, room 83 -> 60, fly-over 211 -> 181, starter 163 -> 114,
title 99 -> 83. Where the 89 ms go (ablations, town 4x): bin visits + the float bound 34, exact depth 13, texture 14, colour/texcoords 20, edge mask 8,
plot ~7; the bin size does not matter (rows 2-8, columns 8-16: +-5 %). Left: pass P is 22 ms at 4x of which pass E (the edge map) is ~11 and a bare
full-screen pass 8.7; the exact z/s/t terms (ai, dq, dr, fa, fb - fa) could be precomputed per record on the CPU (~5 ms); the record's hot fields
(xs, xe, z at x = 0, dz/dx) in one texel would save ~5 ms of fetches. Game, town, Renderer=gpu, --fast on the shared pool: 1x 171, 2x 123, 4x 64 fps
. Frame skip: BWPORT_FPSLOG now prints "skip drawn D skipped S longest L busy ..." each second.

**Apple GPU check (macOS, M2, Metal-backed GL 4.1, 2026-11)**: the first gpu run's "engine A differs by ~300 px per frame" is not an Apple
GPU/driver fault. `BWPORT_GPU_CHECK` at Resolution > 1 compares pass C's top-left pixel of each DS pixel with vcore's DS-resolution
frame, and engine A's 3D layer is drawn at N x, so its polygon edges and texture samples differ from vcore's wherever 3D shows (the intro
has full-screen 3D: Minccino, the light rays). The count grows with N, which a shader bug would not do. At 1x (the check's exact scale)
the intro, 6000 frames: engine A differs in 269 frames, worst 2 px, 279 px in all, every one with a differing 3D layer (G3/G4's known
0.15 %); engine A pixels differing with an equal 3D layer: 0; engine B: 0. So pass C (compositing, brightness, blending) is exact on
Apple's GPU, as on llvmpipe. The first 12 differing pixels of a frame can be logged with `BWPORT_GPU_CHECK_PX=1`.
At 2x / 4x / 8x (3600 frames of the intro): engine A mean 561 / 879 / 1268 px per frame, worst 4957 / 6824 / 7928, engine B 0. Expected,
not a defect; the layer counts are only filled in at 1x (they were meaningless at N > 1 before, now they read 0).
Also: a frame with display capture on (DISPCAPCNT bit 31, the title screen's Reshiram fade) is drawn by vcore at 1x in both modes, so in
gpu mode the Metrics line says 256 x 384 there while the picture is shown scaled; the intro and later 3D/2D frames say N x.
Screenshots (title, intro text scene, Minccino) at vcore and gpu, 2x/4x/8x, looked at: same picture in both renderers, no seams, missing
layers or blur; the 8x window is a downscale of 2048x3072.

## Antialiasing: multisampling (2026-11-02)

Antialiasing=off|4 samples (libntr-ios-msaa.patch; the old 2x and 4x read as 4 samples). The targets stay at N; the spans are built
on the N k grid (k = 2) exactly as supersampling built them, and sample (i, j) of output pixel (x, y) is grid pixel (2x + i, 2y + j)
(the 2x2 ordered grid of the old 2x, so edges match it: town at 3x, the house roof crop, old 2x vs new: 1 pixel of 60000 differs by 4).
- Pass R (g00_raster.frag, compiled per k as "g00_raster.frag#2": gpu_gl.c defines G_K) runs one fragment per output pixel and keeps
  the k x k samples' DS state in vector lanes (C, D, F, I, L: one uvec4 each). Per polygon of the bin: g_row fetches each sample
  row's record (t0, t1, t4, all rows before any test), g_cover_row does coverage, the early depth reject, the exact depth and the
  edge flags/DS coverage per sample (the record's further fetches once per row, only when a sample is left); if any sample is
  covered, g_shade + g_texture run ONCE for the pixel at its first sample on the first covered row (vcore p21's shading point),
  and g_plot4 (g05_plot.glsl) applies the DS's rules to all covered lanes at once (no per-sample branches). Out: one RGBA32UI per
  sample (depth, IDs|flags|coverage, colour, last) into the layers of a 2D array (4 draw buffers: GLES 3.0's minimum).
- Pass E and pass P read the samples through g_samp (the N k grid position -> layer, N position). Pass P (one draw, a fragment
  per output pixel) reads the pixel's four samples: the plain ones get only the fog; a sample that needs its neighbours (edge
  flag with edge marking or AA on, or F_WRITTEN where the edge map is set: the Edge=ds thickening) runs p_sample, in a loop over
  the pixel's own slow samples (a group pays for the most slow samples one fragment has, not for every sample index any needs).
  Then the resolve inline: all alike -> that sample, else the premultiplied average (vcore's resolve_row arithmetic). Pass A,
  g07_res.frag and g06_resolve.frag are gone; g07_compose takes the layer at N.
- Pass E (k = 2): pass R sets bit 30 of sample 0's ID word when a sample of the pixel has the edge flag. Stage 0 runs per output
  pixel into an R8UI map at N (t_em[2]) and looks at the samples of flagged pixels only; stage 2 ORs each DS pixel's N x N into
  t_em[0]; stage 1 as without. (A fragment per DS pixel reading all 4 N^2 samples cost 1.7 ms at 3x: few fragments, long loops.)
  Stage 0's value is a bit per sample (bit j k + i: sample (i, j) passes p_fine_mark), and pass P's Edge=ds walk reads that bit
  (uFine, unit 3) for an edge-flagged neighbour instead of fetching the neighbour's four neighbours.
- Alike pixels (aa-2pass, 2026-11-02): pass R sets bit 31 of sample 0's ID word when the four samples give pass P the same value
  without their neighbours: the same colour, flags and opaque ID, no edge flag that edge marking or AA acts on, and the same fog
  (fog off, the flag clear, or the same depth >> 9). Pass P then reads sample 0 alone and fogs it; where the Edge=ds thickening
  reaches the pixel (edge map set, F_WRITTEN) p_thick4 walks once for the four samples (along each direction the trailing sample
  walks the leading one's line a step behind). Town at 3x: 84% of the pixels are alike, battle 92%. The picture is unchanged.
- Tried and dropped (aa-2pass): pass R in two halves, a one-state loop on every pixel that discards (stencil mark) the pixels
  whose samples might part (a polygon drawn on some samples only, a sample in an edge window with a different edge step, a depth
  test not certain on the float depths), then the four-state loop under a stencil test on the marked ones only. Exact (the same
  3D layer on 8 captures at 1x-4x), marked town 7.8%, battle 5.0%, but SLOWER: town R 4.3-4.4 ms against 2.8. The marked pixels lie
  along every edge (8x4 groups with one: 36%, 32x32 tiles: 92%) and the M2 runs sparse fragments far below their share: the
  second half cost ~2.0 ms of the full screen's 2.1 ms of loop work; probes with the second half on a fixed 10% of pixels: 8x8
  blocks 28% of the full cost, scattered pixels 55%. The one-state half alone cost 1.4-2.4 ms (AA off's whole loop: 0.76 ms of
  work): two sample rows' records, the edge windows of both rows, and the uniform edge step's branch (0.66 ms). One draw that runs
  the one-state loop and continues in four states from the polygon where the samples part: 4.5 ms. Fixed costs measured with
  shaders that write zeros: AA off 0.28 ms (one RGBA32UI target), 4 samples 0.59 (four layers): pass R's samples are 64 bytes a
  pixel. A glDrawBuffers change between two draws into the same framebuffer splits Apple's render pass (+0.5 ms: the four layers
  stored and loaded again). What could still pay: fewer bytes per sample (RG32UI: depth 24, opaque ID 6, 4 flags, coverage 5,
  colour 23 = 62 bits; the translucent ID stays in pass R), and a pass that is dense, not stencilled (the CPU knows every run's
  ends and edge windows: a list of candidate pixels rendered into a compact target, copied back by points).
- g_cover2 (k = 2) does g_cover_row's steps for both sample rows at once, each step's fetches for both rows together; g_aa_side
  divides by a float quotient with a remainder correction (g_fdiv_near: exact for the 0..31 kept), not by integer division.
- The game's own edge antialiasing (ROW_AA coverage), edge marking (Edge=ds: N k samples wide = one DS pixel), fog, translucency,
  shadows, Dot/Line/Edge/Filter (on the N grid, uSnap = k) are per sample as under supersampling. Capture frames stay vcore at 1x.
- No cap: GPU_MAX_SCALE (8) bounds N only; the prep accepts grids up to 16. GLES 3.0 core only (2D array textures, integer
  targets, FramebufferTextureLayer); no extension.
- Apple's GL compiler (M2, 4.1 Metal) crashes (SEGV in glpLLVMCGLValue / glpLLVMCGCommaExpr, inside glLinkProgram) when an array
  element is passed as an out or inout argument: copy to a local.
- Cost (M2, town, Resolution 3, GPU timer queries, load 4-6): pass R AA off 2.26 ms, 4 samples 6.3 ms (old supersampling 2x: 7.3);
  pass P + resolve 1.6 ms (old P + A at 6x: 0.75); E 0.83 (0.52). So 4 samples are ~2.8x AA off in R, NOT the 1.5x aimed at;
  overall about the old 2x's cost. What was tried and measured is in ~/bw-cache/progress/aa-msaa.txt; in short: the
  shared texture lookup saves ~1.1 ms, every further sample still costs ~1.1 ms (coverage, depth and the plot), and changes to the
  per-sample code (vector plot, no exact depth, manual unrolling) do not move it while extra fetches do: the loop is bound by its
  dependent fetch chains per polygon (latency, few fragments in flight). Next to try: a per-row record smaller than 12 texels for
  the extra sample row (x range and depth only), and fewer polygons per pixel (smaller bins at k = 2).
  CORRECTION (2026-11-02, aa-speed): those timer-query numbers mix in the M2's clock states (they swing ~2x in one run) and other
  GPU users; an EMPTY polygon loop measured the same as the full one. Measured instead with gpu_render on captured frames (below):
  the loop is ALU-bound, and removing per-sample work does show (no AA-side coverage -1.3 ms of 3.2, no exact depth -0.9, a
  scalar plot -0.5, row 0 only 1.6).
- Cost after aa-speed (gpu_render, Resolution 3, min of 300 renders, GPU saturated, R + E + P): town AA off 1.77 ms (R 1.05,
  E+P 0.72), 4 samples 4.50 (R 2.83, E+P 1.67; before 5.59) = 2.5x; battle off 1.32, 4 samples 2.91 (before 3.42) = 2.2x. The 1.5x
  target is NOT met. Pixels whose samples end up different: town 2.7%, battle 0.9%, so a raster that keeps one state for a
  pixel until it splits could approach AA off, but only as two passes (a one-state pass that marks split pixels, a four-state
  pass on them through a depth or stencil test): a branch inside one shader keeps the four-state code's cost and registers
  (a "plain pixel" skip of the edge step made R slower, 2.8 -> 3.0). Also untried: the thickening walk over a fine-mark map
  (it costs ~0.5 ms of E+P), the second sample of a row's exact depth from the first's remainders, smaller bins.
- Cost after aa-2pass (gpu_render, Resolution 3, min of 300, load 2-3, before -> after): town 4 samples R 2.77 -> 2.78, P (E +
  P) 1.47 -> 1.14, whole 4.24 -> 3.92 (AA off 1.67: 2.3x); battle R 2.23 -> 2.26, P 0.61 -> 0.39, whole 2.84 -> 2.65 (off 1.23:
  2.2x). The 1.5x target is not met. Without the thickening walk P would be 0.85 (town).
- Timing method for shader work: capture a frame (BWPORT_VCAPTURE=DIR,FRAME on a run), build the standalone tool (`meson setup
  build/gpu port/gpu; ninja -C build/gpu gpu_render`) and run `SDL_MAC_BACKGROUND_APP=1 BWPORT_GPU_SHADERS=DIR build/gpu/gpu_render
  F.vcs out.png --scale 3 --aa 2 --api gl --time 300 -q` (glFinish per pass, back-to-back renders keep the GPU at its top clock;
  read the min). Repeating pass R inside the game's frame does not work: Apple's HSR drops the overdrawn draws, and with a render
  pass between them the copies cost the same whatever the shader does.
- vcore (p21) still supersamples (k = 2 at N k <= 8, so not at Resolution 5 and up; the status says "not drawn"). The same method
  there: p16's span walk at N k, but the per-pixel loop of p17/p18 keeping k x k sample states per output pixel and calling the
  texture/colour code once per pixel; the post (p19) per sample and p21's resolve unchanged.

## Antialiasing: SMAA (2026-11-02)

Antialiasing=SMAA (libntr-ios-smaa.patch: sim_config.ini `SMAA`, cfgscript `antialias=3`, BWPORT_AA=3|smaa, the UI value 2;
gpu_create's aa = GPU_AA_SMAA, gpu_render `--aa smaa`). One sample per pixel (the prep reads opt.aa 3 as k = 1), and at N > 1
two full-screen draws after pass P (port/gpu/src/g08_smaa.c, shaders/g08_smaa_edge.frag, g08_smaa_weight.frag: the edges, the
blending weights) and the neighbourhood blending inside pass C, where it reads the 3D layer (g07_compose.frag's c_3d through
shaders/g08_smaa.glsl's smaa_blend; g07_compose's `smaa` = gpu_smaa_weights). Off at N = 1 (the 1x picture and the oracle checks
are unchanged; checked on town). vcore draws SMAA as off; the status says "AA SMAA not drawn". A failed SMAA setup (g08_create)
is logged ("gpu: SMAA: ...; drawn without") and the GPU renderer draws without it, reporting AA used 1 (it used to lose the
renderer).
- Ported from the SMAA reference (github.com/iryoku/smaa, MIT, credited in each file): the weight pass (SMAA_PRESET_MEDIUM: 8 search
  steps, no diagonal or corner detection) and the neighbourhood blending; the area and search textures as .inc from its headers. The
  reference's vertex-shader offsets are computed in the fragment shaders (all passes share g00_full.vert). Texture row 0 is DS row 0,
  the top, like the reference's v = 0, so the textures go in as their bytes are. The blend reads the integer layer itself (the offset
  lies along one axis: a lerp of two texels), blends premultiplied by the layer's alpha and writes r6 g6 b6 a5; a pixel with no weight
  is copied as it is.
- The edge detector is ours (g08_smaa_edge.frag): an edge needs the reference's luma step >= 0.05 (BWPORT_SMAA_LUMA) AND two polygons
  meeting in pass R's aux: the last polygon differs, F_EDGE is on one side, and the opaque IDs differ or the depth is not one plane
  (|step - slope| > 512 + slope / 4 on either side). What was dropped, from the edge pictures (blended pixels painted red): depth alone
  marks every triangle of the flat ground meshes and the battle platforms' fans (the DS's vertex depth is 15 bits times 0x200, so two
  triangles of one plane differ in slope; dashed lines at a tolerance of 2, solid ones up to 256); without the F_EDGE test the sprites'
  alpha cutouts get outlined; a translucent-ID test smoothed the battle shadows' pixel art; luma 0.1 (the reference's MEDIUM) lost the
  battle platforms' rims. Not smoothed: polygon edges of one opaque ID and one plane, the sides of translucent polygons, and
  patterns inside an edge's own pixels (the sign's right post keeps the DS edge AA's dashes that 4 samples removes). Under a
  translucent polygon (the same polygon last on both sides and translucent on both: a fade, weather, water, the battle's shadows)
  the opaque IDs, edge flag and depth it leaves decide (2026-11-02; before, SMAA was off beneath it): of the 37 Mac captures at 3x
  11 change by 3-183 pixels, the battle platform's rim under the Pokemon's shadow and the town's under the player's now smoothed.
- Cost (gpu_render --time 300, Resolution 3, the min, M2 at load 4.6 with other agents on the GPU, so about 2x the quiet numbers above:
  AA off R + P 3.44 ms here against 1.67 quiet): SMAA town 0.50 ms, battle 0.46 (4 samples here: R + P 5.25 town, 3.50 battle).
- In the game (Mac, the town save, Resolution 3, Frame rate monitor, Smooth motion, BWPORT_PANELHZ=120, no frame skip, cfgscript
  bench, load 3.4-3.6): off 1.00x, 119.9 presents/s, 58.4 in-betweens/s, CPU 9.74 ms/tick, GPU lag 3.4 ms (max 14); 4 samples 1.00x,
  119.1, 56.9, 14.29, 4.9 (max 22), 3 drops; SMAA 1.00x, 119.8, 58.2, 9.91, 3.4 (max 16), no drops. The in-betweens go through the
  same passes. Resolution 3 -> 2 -> (4 samples) -> SMAA at 4 -> 1 -> 3 off in flight: no error; at 1x the status says "AA SMAA not
  drawn (the GPU renderer at Resolution 2 and up)". The bridge's pass timer counts SMAA inside pass P. iOS Simulator (Apple's
  software GLES 3.0): the shaders compile and the town draws at 2x with "AA SMAA".
- Pictures (3x, off | 4 samples | SMAA): the crops in the run folder named in the commit; the maintainer judges on the phone.
- The blending in pass C (2026-11-02, review): a probe with the passes' work stripped out cost nearly the whole 0.37-0.40 ms on
  the M2, so the cost was the three full-screen passes and their targets, not the arithmetic. The third pass and its RGBA8UI
  layer are gone; pass C blends the one pixel it reads (the weights texture is the layer's size, the blend's 4 texel reads only
  where the weights are non-zero). The readbacks (gpu_read_3d: gpu_render --hi, the tools) run the same function into a temporary
  layer (g08_read). Byte-identical to the three-pass SMAA on the 37 captures at 1x-4x (--hi, the 1x output and --compose gpu).
  The edge and weight passes set their sampler units and uniforms once (g08_create) and invalidate their targets first (below).
  The pass timer's SMAA part is now the two passes; the blending is in the compose's. gpu_render --time 200 at 3x (M2, min, load
  17-27 from other agents; `--compose gpu` now times pass C too): SMAA town 0.40 -> 0.31-0.33 ms, battle 0.36 -> 0.28-0.29;
  pass C with SMAA 0.50-0.51 -> 0.55-0.56 (the blending's reads), without it unchanged (0.51-0.53): about 0.05 ms less a frame
  on the M2, where nothing is reloaded into tile memory; the phone also saves the third target's store and load. Pass C binds a
  1 x 1 RGBA8 texture to cSmaa when there is no SMAA (texture 0 on a float sampler made Apple's GL log "unit 2
  GLD_TEXTURE_INDEX_2D is unloadable").
- glInvalidateFramebuffer (2026-11-02): every pass that overwrites its whole target (R, E, P, the two SMAA passes, C) calls
  gpu_gl_invalidate after binding it, so a tile-based GPU does not load the old contents into tile memory. GLES 3.0 core; loaded
  only on GLES, desktop GL 4.3+ or ARB_invalidate_subdata (macOS's GL 4.1 has none: a no-op there). Pass S writes through a
  buffer and a pixel-buffer copy, no framebuffer.

## Antialiased 3D edges over the 2D (2026-11-02)

Both antialiasing modes give a partly empty 3D pixel (an edge next to the clear plane at alpha 0) a premultiplied colour, the
object's own, and a partial alpha. Pass C used that alpha only where the DS blends the 3D with the layer beneath (BLDCNT's
second target: c_blend_3d); everywhere else it drew the pixel opaque, so the silhouette grew by the partly covered pixels and kept
its 1x staircase (p16_tri_gouraud at 3x: all 690 partial pixels with 4 samples, all 1018 with SMAA, drawn in the triangle's
full colour). Now the layer carries the empty part: bits 5-7 of the alpha, in eighths (g06_post.frag's p_resolve: 2 x the samples
of alpha 0; g08_smaa.glsl's smaa_blend: the blend of the texels' drawn / empty, rounded), which every other reader masks off
(alpha & 31: c_3d, gpu_read_3d). Where the 3D is the top layer and not blended (`over` in c_compose), pass C composes the pixel a
second time without the 3D and mixes the two by that part, as the DS's own 3D-over-2D blending would; interiors (empty part 0)
and AA off are as before. Crops (composed, 3x, off | 4 samples | SMAA): stalls/aa/b_p16_em_p16_tri_gouraud_crop.png (before),
n_..._crop.png (after) in the session scratchpad. The game's captures (town, battle, the title with Reshiram) have no such
pixels: their 3D covers the whole screen (the clear plane is opaque), so the game pictures there are unchanged. gpu_render
`--compose gpu` now composes from the core's own layer texture (gpu_3d_tex, as the bridge does), not a re-upload of the readback,
which drops these bits.

## Cost study (2026-11-02)

How to cut the 3D's CPU and GPU cost for the phone (records built on the GPU in a setup pass, SMAA instead of 4 samples,
PBO uploads, the in-betweens, the budget) and the work cut into pieces: docs/gpu-cost.md.
