# Cutting the GPU renderer's 3D cost (design study, 2026-11-02)

Goal: a test iPhone (iPhone17,2, A18 Pro) at Resolution 3, antialiasing and Smooth motion at 120 Hz without heat
throttling. This is a decision document: findings with numbers, the recommended design, and the work cut into task-sized
pieces. Nothing here is implemented. Measured on the M2 (8 cores, load 1.8-2.3 throughout, another agent's runs beside
these) with `build/gpu/gpu_render` on the two captures of docs/perf.md "Antialiasing on the GPU path" (town f1400,
battle), min of 200 renders unless said; the game's profile is the main session's 10 s `sample` of a Mac run (gpu,
Resolution 3, 4 samples, Smooth, Monitor at a pretended 120 Hz, town); the phone's numbers come from the bench log of
2026-11-02 (build 86c22fb77). Throwaway probes are described inline; none is committed.

## In ten lines

1. The CPU half is one thing: **span records, one per polygon per row of the N k grid**. Everything in it (the edge walk,
   the attribute ends, the AA windows, the packing, the upload, Apple's twiddling of the upload) is linear in N k, and
   the per-polygon part is noise (0.02 ms of 3.6 ms). Spreading it over cores moves the heat, smaller records trim 30%,
   reuse across in-betweens helps only while standing still.
2. So **build the records on the GPU** (a setup pass S before pass R: one fragment per record texel, running p16's row
   rules from a per-polygon table of ~400 bytes). The CPU keeps p16's per-polygon setup and the band lists; uploads drop
   from 3.3-6.6 MB a frame to ~0.2 MB; pass R and its exactness logic are untouched. Verified by reading the GPU's
   records back and diffing them against the CPU builder's, field by field, plus the existing picture checks.
3. **Antialiasing: SMAA on the 3D layer** (edge detection from pass R's polygon IDs and edge flags, not colour, so the
   pixel art is left alone) instead of 4 samples by default on the phone: pass R at the AA-off cost (2.3x cheaper on the
   GPU) plus ~0.2-0.4 ms of post. 4 samples stays as an option. The DS's own edge antialiasing is already in the AA-off
   picture at N.
4. Uploads that remain go through a PBO (GLES 3.0 core): half the CPU of Apple's client-memory path.
5. Each in-between then costs the geometry (0.7 ms) plus ~0.2 ms, not 2.6-4.6 ms; a tick whose vertices did not move
   skips its in-betweens' geometry entirely.
6. Expected on the M2 at Resolution 3, SMAA, Smooth at 120: CPU 1.1 -> ~0.45 cores, GPU ~2.2 -> ~2.5 ms a frame (AA off
   + SMAA) against 4.4 today with 4 samples. The phone's GPU time per frame is the open number (its log gives bounds only):
   piece P0 measures it first.

## The data

**Records (GPU_STATS probe in g00_frame.c: nspan, bytes, prep ms; a record is 12 RGBA32F texels = 192 bytes, not the 128 of
docs/gpu.md "The data").**

| frame | grid | records | span bytes | list bytes | prep ms (CPU) | pixel-visits/px | covered/px |
| --- | --- | --- | --- | --- | --- | --- | --- |
| town | 1x | 5,697 | 1.09 MB | 41 KB | 0.62 | 6.7 | 3.81 |
| town | 3x, AA off | 17,081 | 3.28 MB | 42 KB | 1.72-1.81 | 6.8 | 3.75 |
| town | 3x, 4 samples (6x grid) | 34,153 | 6.56 MB | 42 KB | 3.58-3.66 | 6.9 | 3.73 |
| battle | 3x, AA off | 7,071 | 1.36 MB | 23 KB | 0.76-0.83 | 3.8 | 2.23 |
| battle | 3x, 4 samples | 14,132 | 2.71 MB | 23 KB | 1.60 | 3.8 | 2.21 |

prep ms against records is a straight line: ~0.105 us a record, intercept ~0.02 ms. The per-polygon work (vc_span_poly,
the polygon table, 324 polygons) is nothing; the per-row work is everything. 17,073 of town's 17,081 records carry the
DS antialiasing windows (GPU_ROW_AA): the game runs with DISP3DCNT bit 4 on, so p16's aa_edge runs for every row at
every setting, "Antialiasing off" included.

**The game's profile (main session's sample, 2449 samples = one busy thread, ~1.1 cores busy in all):** span prep 0.59
cores (gpu_span_pack 358, vc_span_row 344, edge_span 290, aa_edge 181, edge_attr 135, gpu_prepare 97), the GL driver's
twiddling of uploads 0.17 (agxsTwiddleAddressCommon: 236 samples under gpu_render_prepared's span upload in Stage2 and
InterpTexture, 23 under g07_compose's stage upload), 2D layers 0.13, geometry 0.08, sound 0.05. gpu_span_pack is 27% of
the prep: a 192-byte memset, ~40 double-to-float conversions and the mask build per record.

**gpu_render, Resolution 3, this worktree's build (the main session's table in the brief agrees within noise):**

| frame | CPU ms (spans, bands, uploads) | uploads ms | R min | P min | frame min |
| --- | --- | --- | --- | --- | --- |
| town AA off | 1.90 | 0.18 | 1.04 | 0.64 | 3.46 |
| town 4 samples | 3.45 | 0.34 | 2.86 | 1.16 | 7.41 |
| battle AA off | 0.83 | 0.09 | 0.88 | 0.33 | 2.05 |
| battle 4 samples | 1.59 | 0.14 | 2.25 | 0.40 | 4.25 |

**Phone (bench log 2026-11-02, Resolution 3, Smooth, Monitor at 120 Hz):** AA off: 120 presents/s, 60 in-betweens/s, 0.51
cores, CPU 8.6 ms a tick, "gpu <= 7.5 ms/frame" (a bound from fence latency; the same row also printed "<= 45% busy");
4 samples: 120 presents/s, **0 in-betweens/s** (the bench's 4-sample Smooth row shows none; the Mac shows half for ~5 s
after 4 samples comes on), 0.61 cores, CPU 10.1 ms a tick, gpu <= 9.9-12.8 ms/frame; speed 4 with 4 samples 0.79 cores,
AA off 0.57. Sustained fast-forward: 4.0 -> 2.0 GHz within ~70 s (docs/perf.md "The phone throttles"). Resolution 2
with Frame rate Unlimited drew 224 frames/s (4.5 ms a frame, CPU 3.25 ms a tick), the only row that bounds the phone's
GPU from the drawing side.

## 1. Span prep

**What scales with N and k, and why.** gpu_spans_build walks each polygon on the N k grid (`walk` -> vc_span_row per
grid row -> gpu_span_pack): rows = (polygon's DS rows) x N k, so every term is linear in N k and in the polygon count. Per
row, vc_span_row runs edge_span over all edges three times (flat-row test, non-flat test, the walk), edge_run twice, an
insertion sort of the runs, edge_attr twice (lerp_attr in double with two divisions each), and aa_edge twice (Y-major:
closed form; X-major: a scan of 2 (|dx| / dy + 3) + 1 pixels each calling aa_covx with two 64-bit divisions). Then
gpu_span_pack writes 192 bytes. Nothing is reused from the row above: the hardware's slope steps are recomputed from
the top vertex every row (that is what makes 1x exact: p16's code, not a DDA).

**Designs judged.**

| design | prep CPU (town 3x, 4 samples: 3.6 ms today) | uploads | 1x exactness | verdict |
| --- | --- | --- | --- | --- |
| A. Records built on the GPU (pass S), CPU keeps the per-polygon setup | ~0.1 ms + the band lists ~0.1 | ~0.15 MB (polygon tables) | by readback diff against the CPU builder; integer fields bit-exact, attribute ends within float-float rounding | **recommended** |
| B. Per-pixel edge evaluation inside pass R (no records) | as A | as A | as A | rejected: pass R is ALU-bound (docs/gpu.md "Antialiasing"); 20 M polygon-visits a frame at 3x would each pay the row setup that A pays once per (polygon, row): 410 k fragments |
| C. Records per 1x row, the N rows derived | 0.6 ms | 1.1 MB | exact at 1x | rejected: the N x walk is not N copies of the 1x walk (vertices snap to the N grid from the clip coordinates, the slope is truncated on that grid); deriving the N rows from the 1x ends gives the 1x picture scaled (blocky edges), not the N x look |
| D. Smaller records (8 texels RGBA32UI, 16-bit ints where the range allows, 128 bytes) | ~2.5 ms (the pack shrinks) | 4.4 MB | exact | worth doing after A as a shader-side change: fewer fetch bytes in pass R's g_row (3 texels -> 2), and the CPU fallback's pack |
| E. Reuse across in-betweens for unmoved polygons | 0 for an unmoved polygon (memcpy of its rows) | unchanged | exact | walking moves the camera, so every polygon moves: no gain in the common case; standing still: all unmoved, but then the whole in-between can be skipped (see 4). Under A the per-polygon cost is already nothing |
| F. Spread over cores (vcore's task threads, polygons in halves) | wall /2, energy unchanged | unchanged | exact | not a heat fix; keep as a latency option only if A's band lists ever matter |
| G. A DDA walk on the CPU (edge steps incremental, one pass over the edges per row) | ~1.5 ms estimated | unchanged | would need a proof that an incremental integer step equals p16's from-the-top formula (it does for the Y-major floor and the X-major round if the accumulator carries the 2^18 fraction exactly; the AA windows and edge_attr's perspective lerp stay per row) | a fallback if A stalls; it leaves the uploads |

**A in detail (pass S).** The CPU runs one_poly's decisions per polygon as today (the grid g, the widening m, Dot/Line
options, the 1-dot and line cases) and vc_span_poly on that grid, then uploads a per-polygon setup table instead of
records: a header (W, H, flags wire/full/aa/flatbot, ymin, yend, m, nv, ne, first record, the extra row flags), the
edges (ax, ay, dx, dy, ia, ib: 16-bit fields, 1.5 texels each, up to VC_POLY_MAXV), the vertices (w = clip w as s32, z =
sz 24-bit or w, colour 3 x 9-bit, s and t 12.4: one RGBA32UI texel each: the vertex values are exact integers, the
doubles only arise in the lerp). ~25 texels = 400 bytes a polygon, 130 KB for town; the record count per polygon
(rows x m) and the prefix sum of first records stay on the CPU (one add per polygon). Pass S draws one fragment per
record texel over the span texture's rows (GL_RGBA32UI target: colour-renderable in GLES 3.0 core; RGBA32F is not
without EXT_color_buffer_float, so the span texture becomes a usampler2D and g_span wraps uintBitsToFloat: a two-line
change in g00_head.glsl; the CPU fallback uploads the same bytes as uints), finds its polygon by a binary search of
uPoly's first-record field (9 fetches) or an R16UI owner texture, and runs vc_span_row for its (polygon, row) and writes
its texel. The band lists come from the edges: a polygon's x extent over a band's rows is at the band's first and last
rows or at a vertex inside the band (convex), so build_bands evaluates the left and right edges there; the lists grow
by nothing measurable (a probe halving the bins, 8 x 4 DS pixels, cut pixel-visits 6.8 -> 5.1/px and pass R by only
0.01-0.14 ms: pass R hardly pays for misses, so a slightly loose extent costs nothing).

Three things in p16 use 64-bit integers and GLSL ES 3.0 has none: the slope step S k (S = |dx| floor(2^18 / dy): S k
reaches 2^39 on the 6x grid), aa_covx's products, and the left/right choice by the edge's x at the row centre
(compared as rationals). All split into 32-bit halves exactly (for S k: a = |dx| k < 2^21, q = floor(2^18 / dy) <= 2^18;
split a = ah 2^11 + al, then floor(a q / 2^18) = floor((ah q + floor(al q / 2^11)) / 2^7), every product under 2^30),
and the X-major AA window's scan (first and last pixel with coverage > 0 of a function monotone in the pixel) has a
closed form from aa_covx's inequality. These are the risky lines: they are written first in C beside p16 (tests/t02_spans.c)
and compared with the s64 originals over the whole input range (dx, dy up to 2048, k up to 1536), before the shader.
The attribute ends (edge_attr: lerp_attr in double, colours floored) are float-float (two floats, Dekker products) in the
shader: ~2x the ALU of a float lerp on 410 k fragments, nothing; the hi part matches the CPU's (float)v and the lo part
is within an ulp of its lo_part, which is inside the tolerance the 1x compare already allows for the across-row lerp.

Cost of pass S: town 3x with 4 samples 34 k records x 12 texels = 410 k fragments, each a few hundred ALU ops and ~15
fetches: ~0.1-0.3 ms on the M2 by the fragment count (a quarter of pass P's fragments with far less work), ~0.3-0.5 ms
on the phone. The records are written once and read by pass R as today.

## 2. Uploads

**What is twiddled.** Apple's GL on Metal keeps every 2D texture in the GPU's twiddled layout and converts client memory
on the CPU inside glTexSubImage2D (agxsTwiddleAddressCommon under gldModifyTexSubImage, in the sample). Per frame at
Resolution 3: the span records 3.28 MB (AA off) / 6.56 MB (4 samples), the polygon table 10 KB, the band and list
textures 42 KB, the texture slots and palettes only their changed rows (gpu_gl_data_tex_diff; a frame that changes no
VRAM uploads none), engine A's stage 786 KB + src 393 KB (g07_compose.c: TexImage2D re-specified every frame, docs/perf.md
"gpu path"), and vcore's engine B picture (Upload1x). So ~4.5 MB (AA off) / ~7.8 MB (4 samples) a present at 3x, of which
the span records are 73% / 84%. With in-betweens the records go up once per present (120/s), the 2D once per tick.

**Probe (study3/upload_probe.c, GL 4.1 Metal on the M2, hot data, min of 27):** 6.4 MB of RGBA32F into a 1024-wide 2D
texture by client memory: 0.25 ms in the call (0.46 with a texelFetch draw and glFinish); through a PBO filled by
glBufferSubData 0.13 (0.68); a PBO through glMapBufferRange(INVALIDATE) and memcpy 0.12 (0.53); a texture buffer
(GL 3.1, not GLES 3.0) by BufferSubData 0.12 (0.53), by MapBufferRange 0.11 (0.30). At 3.2 MB: 0.12 / 0.063 / 0.061 /
0.052 / 0.049. So on Apple's GL the client-memory twiddle costs ~0.04 ms a MB with hot data (the game's cold, cross-core
data pays ~0.1-0.2 ms a MB: 1.4 ms of CPU a present at 120 Hz in the sample), a PBO halves the CPU (the driver then
twiddles on the GPU: the frame time is the same), and a mapped buffer the span builder writes into directly saves the
memcpy too. Only a texture buffer avoids the twiddle outright and it is not in GLES 3.0 (GLES 3.2 / EXT_texture_buffer;
Apple's GLES has neither).

**iOS pays it too, most likely.** The iOS build uses Apple's OpenGLES.framework (ES 3.0 on Metal, libntr-ios-gles.patch),
not ANGLE: the same AGX texture code as the Mac's GL-on-Metal (the Mac's sample shows AppleMetalOpenGLRenderer calling
the same agxs twiddle). Not measured on the device: the bridge already times the upload calls (g00_frame.c's up_ms,
gpu_last_passes) and the phone's sys: line does not print them. Piece P0 adds that. ANGLE on Metal (if iOS ever moves to
it) stages client uploads in a buffer and blits on the GPU by its documented design, so the PBO path and ANGLE behave
alike; the Simulator would measure the Mac's driver and says nothing about the device.

**Decision.** Under design A the records are never uploaded, which removes 73-84% of the bytes and the whole 0.17 cores
of twiddle for them. What remains (the 2D stage and src, 1.2 MB a tick; the texture diff rows; the polygon tables) goes
through a double-buffered PBO with glMapBufferRange, GLES 3.0 core, no fallback needed (a driver without a fast path
just copies). Baseline stays GLES 3.0 everywhere.

**After the review fixes (2026-11-02).** Per frame the GL thread no longer flattens VRAM or packs the 2D: the bridge's worker
builds the texture slots' flat copy (gpu_texdata_build, 512 KB + 24 KB, a page with one bank copied) and pass C's stage and 1x
lines (g07_pack, 1.2 MB) once per frame into the slot, and the GL half uploads each only when that build is not the one up: a
frame's in-between pictures and the frame itself share one upload of each (before: the flatten, two mallocs and the 512 KB
compare on every GL half, and the 2D packed and uploaded by each mid and again by the frame). What is left of the uploads, per
present: the polygon table (~10 KB) and the band and list textures (~42 KB), client memory (TexSubImage2D, twiddled by the
driver); once a tick: the 512 KB compare of the texture copy and its changed rows, the 2D stage and src (1.2 MB through
TexSubImage2D into textures of a fixed size); vcore's 1x picture on capture frames. P3's PBOs would take the 1.2 MB and the
changed rows. Mac, town, Resolution 3, passes=1 rows at 60 (load 2-3): the bridge's uploads part 0.10 -> 0.06 ms, the compose
part 1.09-1.17 -> 1.06-1.13 (GPU-bound there); with Smooth at Monitor and BWPORT_PANELHZ=120 (SMAA / 4 samples / off): cores
0.43 / 0.41 / 0.41 -> 0.41 / 0.41 / 0.40, CPU ms a tick 7.14 / 6.87 / 6.88 -> 6.83 / 6.91 / 6.76, in-betweens 59.6 / 59.9 /
60.2 -> 60.4 / 59.7 / 60.2 a second: the Mac's GL thread had time to spare; the phone's readout (~1.0 ms uploads, 1.0-1.4
compose a frame, paid again by each in-between) is where it should show.

## 3. Antialiasing

What "Antialiasing off" already is at N: the game keeps DISP3DCNT bit 4 on, so the DS's own edge antialiasing runs at N in
pass P (an opaque edge pixel's coverage blended with the farthest neighbour behind, 1 N-pixel wide, 32 levels). In the
3x crops of town (study3/ct_aa1.png against ct_aa2.png) the sprite (alpha-tested, nearest texels) is identical, the pixel
art is identical, and 4 samples smooths the roof's diagonal and the polygon-against-polygon edges of the house a little
further; battle shows the same on the platform edges. 4 samples also averages texel boundaries of the 2N grid's
nearest sampling (a slight softening of texture edges at 2N, invisible at nearest 3x in the crops).

| direction | CPU | GPU (M2, town 3x) | picture on DS content | complexity |
| --- | --- | --- | --- | --- |
| 4 samples kept, with design A | prep gone (A), records 2x AA off's count built on the GPU | R 2.86 + P 1.16 = 4.0 ms (2.3x AA off's 1.7); the floor is ~2.2x: every sample pays coverage, depth and the plot, the two-pass and one-state variants were measured slower (docs/gpu.md "Antialiasing") | the reference quality: edges, translucent edges, alpha cutouts, edge marking all per sample; the resolve is vcore's | done |
| analytic coverage in pass R: one state per pixel, per-polygon coverage from the edge equations | as A | AA off's cost + the coverage arithmetic per hit (~AA off + 10-20%) | coverage-weighted blending in one state is an A-buffer without the buffer: a later polygon behind a partly covered edge pixel depth-tests at the centre and overwrites or loses the blend (order and depth artefacts along every silhouette); keeping per-sample depth to fix it brings back most of the per-sample cost (the measurements: no exact depth -0.9 of 3.2 ms). What it would deliver at 1x is exactly the DS's own edge AA, which the AA-off path already has at N | high, and the quality is bounded |
| SMAA (Jimenez 2012, MIT reference) on the 3D layer at N, before pass C; edges from pass R's aux (opaque/translucent ID change, the edge flags, depth steps), colour only as a high-threshold second input | 0 | three full-screen passes at N: edge detect, blend weights (the area and search textures, 160x560 RGBA8 and 66x33 R8 from the reference, with credit), neighbourhood blend; published 1x cost ~1 ms at 1080p on a 2010 GPU; at 768x576 (0.21x the pixels) on the M2 ~0.15 ms, on the A18 Pro ~0.2-0.4 ms (an estimate until P2's first step measures it) | polygon silhouettes and interior polygon edges reconstructed from the ID/edge map, straight edges and long diagonals best, sub-pixel slivers least; translucent edges yes (their IDs are in the aux); alpha cutouts only through the colour input (left crisp by default: the pixel art stays nearest); edge-marking outlines (N wide) get their own sides smoothed; fog is before it; the 3D layer's alpha is blended premultiplied with its colour, so the layer over the 2D stays right; at 1x it is off (the 1x oracle checks run without it) | medium: a known port (the reference has GLSL paths), the adaptation is the edge detector reading our aux target |

**Recommendation: SMAA as the default "Antialiasing" at Resolution >= 2 on the phone, 4 samples kept as the option for
hosts with GPU to spare.** The reasons are the budget below (the 4-sample pass R alone is over the phone's 120 Hz frame)
and that the 2N grid's extra coverage is the marginal gain over the DS AA the AA-off picture already has. Order: design A
first (it helps every AA setting), SMAA beside it, then judge the pictures on the phone with the two side by side (the
bench's screenshot strips).

## 4. In-betweens

Today each in-between redoes the geometry on the blended vertices (gpu_geometry_again, 0.67 ms town), the records at N k
(1.75 ms AA off, 3.6 ms 4 samples), the band lists and the upload (0.15-0.35 ms of calls plus the twiddle): 2.6-4.6 ms
of CPU a mid at 3x, 60 a second. Under design A a mid is the geometry plus ~0.2 ms (the per-polygon table, the prefix
sum, the band extents, ~0.15 MB through a mapped PBO): ~0.9 ms, and the GPU builds its records in pass S (~0.1-0.3 ms of
GPU). Smooth motion's own pacing (SpInterpGuard, the cap loop, InterpTexture's wait) is another agent's and is not
touched. Two small additions: a tick whose paired vertices are all equal to the previous keyframe's (standing still, no
animation) marks its mids as "same picture" in Stage1Interp and skips their geometry and prep (the keyframe's prep is
presented again: today a mid with f in (0, 1) is always rebuilt); and with the records built on the GPU the per-polygon
reuse of design E is not needed.

## 5. Budget

**M2, Resolution 3, SMAA, Smooth at 120 (120 presents, 60 ticks):** CPU: 2D layers 0.13 cores, geometry 0.08 (60
keyframes + 60 mids), per-polygon setup and bands ~0.02, uploads through PBOs ~0.03, sound 0.05, the rest (compose
stage, pacing, input, present) ~0.1: **~0.4-0.45 cores** against 1.1 measured. GPU a frame (gpu_render mins, town): S
~0.2 + R 1.04 + P 0.64 + SMAA ~0.15 + C (compose, not in gpu_render; perf.md's accounting: a 768 x 1152 RGBA8 draw) ~0.3
= **~2.3 ms**, against ~4.6 with 4 samples today (R 2.86 + P 1.16 + C) and ~2.0 AA off: at 120 Hz 28% of the M2's GPU.

**A18 Pro, projected.** CPU: the phone ran 0.51 cores at AA off and 0.61 with 4 samples where the M2 ran ~1.1 for the
same setting, so its CPU side is lighter than the M2's by the clock and by what its driver skips; taking the same
fractions off (span prep + twiddle = 69% of the busy time) gives **~0.2-0.25 cores**, which is the regime where its log
read 3.4-4.0 GHz without a drop. GPU: unknown in absolute terms; the log's bounds (<= 7.5 ms AA off, <= 9.9-12.8 with 4
samples, and 4.5 ms a frame drawn at Resolution 2 Unlimited) say that 4 samples at 3x is at or over the 8.3 ms of a 120
Hz frame and AA off is under it. With R + P cut from 4.0 to 1.7 ms on the M2 (2.3x) the phone's 3x frame should land
around 3.5-4.5 ms if it scales like the M2's, and the 2x Unlimited row (4.5 ms with 2.25x fewer pixels) says the
fixed part (compose at N, the present at the panel's 1320 x 2868, the UI) may be as large as the 3D: P0 measures both
before any GPU piece is judged. The heat target is the sum: a frame at 120 Hz must stay under ~50% of the GPU and the
CPU under ~0.3 cores for the clock to hold (the fast-forward run throttled at 0.8 cores plus a busy GPU within 70 s).

## The pieces

Each piece is one agent in its own worktree. "Exactness" means: the DS ROM build is untouched (no src/ change here), and
at 1x `port/vcore/tools/vcore_check pNN --oracle gpu=build/gpu/gpu_render` on p13-p19 and `gpu_compare.py` on the
corpus keep their numbers (docs/gpu.md "Testing by CPU"), `tools/gpu_same.sh OLD NEW 1 2 3 4` lists what changed at N.
Timings by gpu_render on the two captures, min of 200, load noted.

**P0. Measure the phone (small, first; parallel with everything).** Files: the perftest's cfgscript rows (where cb51a1042
put them) and the sys: line's printer (port/bridge/gpu_bridge.c PerfReport1 / the libntr patch that prints sys:, not
sim_main.cpp's pacing code). Adds bench rows gpu3-unlimited and gpu3-aa4s-unlimited (Frame rate Unlimited: drawn/s is the
GPU's frame time when the CPU is faster; at 2x it gave 224/s) and prints per tick the bridge's up_ms (the upload calls'
CPU), the prep ms and gpu_last_passes' R/P. Verification: none needed (readout). Saving: none; it decides the GPU
budget and tells whether the device twiddles. Dependencies: none. An IPA through tools/ios_publish.sh.

**P1. Records on the GPU (pass S; large).** Files: port/gpu/src/g02_spans.c/.h (the per-polygon setup table beside
gpu_spans_build, which stays as the reference and the `BWPORT_GPU_SPANS=cpu` fallback), new port/gpu/shaders/g02_setup.frag
(+ a g02_setup.glsl shared with nothing), port/gpu/src/g00_frame.c (pass S, the span texture as an RGBA32UI render
target, band extents from the edges), port/gpu/shaders/g00_head.glsl (usampler2D + uintBitsToFloat in g_span),
port/gpu/src/gpu_gl.c (a data texture that is also a framebuffer attachment), port/gpu/tools/gpu_render.c
(`--spans gpu|cpu`, `--spans-check`: read the GPU's records back with glReadPixels and diff them against the CPU's, per
field: integer fields bit-exact, attributes hi exact and lo within 1 ulp, a count per field and the first ten
differences), port/gpu/tests/t02_spans.c (the 32-bit forms of edge_span, xmajor_q, aa_covx, the row-centre compare and
the closed-form X-major window against p16's s64 code over the whole range). Steps in order: the C forms and their test;
the setup table and the shader for the integer fields with the CPU's attribute ends spliced in (so --spans-check passes
field by field as the shader grows); the float-float attribute ends; the band extents; the fallback switch. Verification:
--spans-check on the corpus and the p13-p19 fuzz scenes at 1x, 2x, 3x and 6x (0 integer differences), then the exactness
set above, then gpu_same OLD NEW at 1x-4x (expected 0 differ; any difference must be inside the attribute tolerance and
listed). Saving: CPU 0.59 + 0.17 cores -> ~0.1 at 120 presents/s (M2), uploads -0.15 to -6.4 MB a present; GPU +0.1-0.3 ms.
Dependencies: none (the bridge's gpu_prepare / gpu_render_prepared signatures stay; nothing in gpu_bridge.c's pacing
code). Parallel with P0, P2, P3, P4.

**P2. SMAA on the 3D layer (medium).** Files: new port/gpu/shaders/g08_smaa_edge.frag, g08_smaa_weight.frag,
g08_smaa_blend.frag (ported from the MIT reference's GLSL path, credited in the headers; its SMAA_PRESET_MEDIUM), new
port/gpu/src/g08_smaa.c (the area and search textures embedded as .inc from the reference's .h, the three draws between
pass P and pass C into a second RGBA8 layer), port/gpu/src/g00_frame.c (the pass hook), port/gpu/include/gpu.h
(GpuOptions.aa gains a value: 0 off, 2 four samples, 3 SMAA), port/bridge/gpu_bridge.c (the option plumbing in Setup /
Stage2's option copy only), one new libntr patch libntr-ios-smaa.patch (the Antialiasing setting's third value in the
settings sheet and sim_config.ini; last in the wrap's diff_files, the whole chain checked with git apply --check),
tools/gpu_render.c (`--aa smaa`). The edge detector reads pass R's aux (unit 7's sample 0: ID word and flags) and a
colour threshold the option exposes (default: IDs and edge flags only). Verification: off at 1x so the oracle set is
unchanged; pictures: gpu_render --hi at 3x on both captures against AA off and 4 samples (crops in the commit message's
run folder), the bench's screenshot strips on the phone; timing: gpu_render --time 200 --aa smaa (the first step of the
piece, before tuning: if the three passes cost more than ~0.6 ms on the M2 at 3x, stop and report). Saving: GPU R + P
4.0 -> ~1.9 ms (town 3x) against 4 samples. Dependencies: none. Parallel with P1 (different files; g00_frame.c's hook is
a few lines: rebase).

**P3. Uploads through PBOs (small; done 2026-11-02 for pass C's stage only, "P3 status" below).** Files: port/gpu/src/gpu_gl.c (gpu_gl_data_tex / _diff and a new
gpu_gl_data_map that hands the caller a mapped range of a double-buffered PIXEL_UNPACK_BUFFER, then TexSubImage2D from
it), port/gpu/src/g07_compose.c (stage and src: TexSubImage2D from the PBO instead of a TexImage2D re-specification),
port/bridge/gpu_bridge.c Upload1x (engine B's picture). GLES 3.0 core: glMapBufferRange with MAP_WRITE | MAP_INVALIDATE_BUFFER,
no extension, no fallback needed. Verification: gpu_same 0 differ at 1x-4x; the twiddle samples under g07_compose and
Upload1x gone from a Mac sample; up_ms before and after. Saving: ~half the CPU of what P1 leaves of the twiddle (0.17
cores -> ~0.02 with P1, ~0.08 without). Dependencies: none; P1 drops the span upload, P3 covers the rest. Parallel.

**P4. Smaller records (small, after P1).** Files: port/gpu/src/g02_spans.h/.c (8 RGBA32UI texels: t0 = xs | xe << 16,
den_f | den_z << 16, x0 | flags << 16, L.w; t1 = R.w, L.z hi, R.z hi, the mask runs; ... the AA windows as 16-bit pairs),
g02_setup.frag, g00_head.glsl / g03_pixel.glsl (g_row reads 2 texels, the unpack). Verification: --spans-check still bit-exact
against the CPU packer (both change together), the exactness set, gpu_same. Saving: pass R's fetch bytes a visit 48 ->
32; expected small (the bin probe says R is not fetch-bound: 0.05-0.15 ms); measure first with a probe that drops t4's
fetch and report whether to go on. Dependencies: P1. Measured 2026-11-02, not built: "P4 status" below.

**P5. In-between skips (small, after the pacing agent lands).** Files: port/bridge/gpu_bridge.c Stage1Interp and
port/bridge/ip_common.h (IpGather already pairs the lists: add "all pairs equal" -> the slot's mids present the keyframe's
prep without geometry or prep), nothing in InterpTexture, sim_main.cpp or the cap loop. Verification: BWPORT_INTERP_VERIFY-style
count (mids skipped vs rebuilt, the picture identical by the present hash), the walking and standing runs of
docs/interp.md's checks. Saving: standing still: a mid's 0.9 ms (after P1) -> 0; walking: none. Dependencies: the Smooth
motion pacing work in flight (same file).
**P5 status (2026-11-02): measured, not built: it would skip nothing.** A local counter in IpGather (keyframes whose paired
vertices all equal their partners', clip, texcoords and colour) over 469 valid keyframes per run (gpu 3x, Smooth, Monitor,
BWPORT_PANELHZ=120 BWPORT_DRAWABLES=3, no skip, standing from the save): town 0, Route 1 0, the lab 0, the town with the start
menu open 0; walking in town 0. Every 3D frame of the field is a new keyframe because something in it moves: the water's and the
grass's texcoords scroll by under a texel (about 44 lists a keyframe in town, 31 on Route 1, 4 in the lab; Route 1 and the lab
are texcoords only in 391 and 469 of 469 keyframes) and in town a few lists move (clip changes on ~3.6 lists a keyframe). Those
mids are not the keyframe's picture (the scroll is blended), so they are drawn, 60 a second. Where the 3D is really still (the
room: 1 keyframe, 937 repeats) the existing path already skips them: a repeat keeps the keyframe's time, f >= 1, the mid settles
"none" without geometry, prep or draw (0 mids drawn). Left, if wanted: counting a texcoords-only change under a texel as still
would skip most mids on Route 1 and in the lab, but it changes the picture (the scroll steps at 30 Hz), so it is the maintainer's call.

Order: P0 today; P1, P2, P3 side by side; P4 after P1; P5 after the pacing agent. P1 is the critical path (two to three
rounds); P2 can ship and be judged on the phone before P1 lands.

## P1 status (2026-11-02)

### (2026-11-02): pass S as one transform-feedback draw

The review of found that the phone would take the slow path: pass S needed 8 draw buffers and a test iPhone has
4 (A18 Pro, "OpenGL ES 3.0 Metal - 104 (draw buffers 4, colour attachments 4, transform feedback components 64)"), and the
fallback reordered the records into 12 layers every frame (+0.65 ms CPU, +0.55 ms uploads at town 3x, 4 samples). Pass S
also walked each row twice (two draws). Now (on the ios branch, after SMAA):
- shaders/g02_setup.vert (the fragment shader's body): one GL_POINTS vertex per record (gl_VertexID), the rasterizer off,
  the twelve texels as `flat out uvec4 s0..s11`, written interleaved by transform feedback (48 components; GLES 3.0
  guarantees 64) into a buffer; one PIXEL_UNPACK_BUFFER TexSubImage2D (whole rows, then the partial one) copies it into the
  span texture, record-major as before P1 (RGBA32UI holding the floats' bits; g_span reads texel 12 r + t). The CPU's
  records upload as they are, as before P1. gpu_gl_program_tf builds it (g00_head.glsl before the vertex shader,
  TransformFeedbackVaryings before the link, an empty fragment shader).
- No static between threads: the prep builds only the setup unless BWPORT_GPU_SPANS=cpu (the environment, read per
  frame); a core whose pass S failed to compile builds the records itself on the GL thread from the frame (gpu_g02_records;
  `BWPORT_GPU_SPANS=nopass` / gpu_render `--spans nopass` forces it). Records beyond GL_MAX_TEXTURE_SIZE rows fail the frame
  (vcore draws it). Pass S draws with pass R's framebuffer bound: on iOS framebuffer 0 is not SDL's, and the Simulator's
  first frame failed with GL_INVALID_FRAMEBUFFER_OPERATION before that.
- gpu_last_passes' R counts pass S again (P0's phone readout); gpu_last_pass_s stays apart (gpu_render prints "S" and
  "R with S").
- Where S's time goes (town 3x, 4 samples, M2, load 4-7): 0.77-0.80 ms; the copy 0.2 of it, a vertex shader that returns at
  once (with the copy) 0.56-0.62, a sixteenth of the vertices (with the copy) 0.47-0.51. So ~0.3 ms is a fixed cost of a
  draw plus its glFinish in timing mode, ~0.25 the walk, ~0.2 the copy. P4's 8-texel records would cut the copy and the
  transform feedback by a third.
- gpu_render, Resolution 3, min of 200, load 5-7 (old = ios before this piece, 465ce911c; p1 = as reviewed; cpu =
  this build `--spans cpu`; new = pass S; R includes S for new and cpu, not for p1; old's CPU is a mean):

  | frame | CPU ms old / p1 / cpu / new | uploads | S p1 / new | R | P | frame |
  | --- | --- | --- | --- | --- | --- | --- |
  | town AA off | 1.85 / 0.10 / 1.73 / 0.10 | 0.17 / 0.04 / 0.16 / 0.04 | 0.91 / 0.56 | 1.06 / 1.11+S / 1.05 / 1.69 | 0.61 | 3.46 / 2.85 / 3.54 / 2.47 |
  | town 4 samples | 3.54 / 0.11 / 3.31 / 0.11 | 0.34 / 0.04 / 0.29 / 0.04 | 0.96 / 0.80 | 2.85 / 2.83+S / 2.97 / 3.67 | 1.16-1.22 | 7.54 / 5.11 / 7.59 / 5.09 |
  | town SMAA | 1.79 / - / 1.71 / 0.10 | 0.17 / - / 0.16 / 0.04 | - / 0.50 | 1.05 / - / 1.04 / 1.65 | 0.62 | 3.92 / - / 3.72 / 2.86 |
  | battle AA off | 0.85 / 0.05 / 0.80 / 0.06 | 0.09 / 0.04 / 0.09 / 0.04 | 0.49 / 0.36 | 0.88 / 0.91+S / 0.89 / 1.29 | 0.35 | 2.10 / 1.84 / 2.09 / 1.73 |
  | battle 4 samples | 1.63 / 0.05 / 1.54 / 0.06 | 0.14 / 0.04 / 0.13 / 0.04 | 0.62 / 0.46 | 2.26 / 2.31+S / 2.25 / 2.79 | 0.40 | 4.33 / 3.49 / 4.23 / 3.28 |
  | battle SMAA | 0.93 / - / 0.80 / 0.06 | 0.10 / - / 0.09 / 0.04 | - / 0.34 | 0.87 / - / 0.90 / 1.27 | 0.33 | 2.45 / - / 2.50 / 2.07 |

  (SMAA's own passes 0.35-0.40, unchanged. `--spans nopass`: the CPU's 1.7 / 3.3 ms move into the uploads, on the GL thread.)
- The game (tools/mac_run.sh --noskip, town save, Resolution 3, Smooth, Monitor at BWPORT_PANELHZ=120, bench at f1500; load
  2-9, one run at load ~37 discarded): 4 samples: pass S 0.36 cores, 6.08 ms a tick, 119.9 presents/s; `BWPORT_GPU_SPANS=cpu`
  0.87 cores, 14.54 ms; `nopass` 0.69 cores, 11.52 ms (112.6 presents/s: the GL thread is busier). SMAA: pass S 0.42 cores,
  7.12 ms; cpu 0.59 cores, 9.95 ms.
- Verified on the merged tree (M2, GL 4.1 Metal): `--spans-check` 0 integer fields differ on the 37 captures and the 730
  p13-p19 scenes at 1x, 2x, 3x and 6x, AA off and 4 samples (the attribute counts are's to the digit); the --hi and
  1x pictures byte-identical to ios before this piece on the 37 captures at 1x-4x, AA off, 4 samples and SMAA, for pass S,
  `--spans cpu` and `--spans nopass`; gpu_tests pass. iOS: the device build and IPA, the Simulator app; a Simulator run
  (Apple's software ES 3.0, vcore to f1700, then gpu at Resolution 2) drew the town with pass S and no GL error.
- The phone: what pass S costs there is the open number; it lands inside R in P0's readout (the phone's R today: 4.6 ms AA
  off, 5.9 with 4 samples, with 2.8 / 3.9 ms of CPU records and 1.0 ms of uploads that pass S removes).

2026-11-02, the stalls round: the copy is one PIXEL_UNPACK TexSubImage2D (the transform-feedback buffer has room for whole
rows and the last, partial row goes up whole; the texels past the records are the buffer's, pass R never reads them), not two:
on Apple's GL each buffer-sourced upload is a Metal blit encoder. gpu_render --time 200 at 3x, S min (load ~6): town 0.51 /
0.74-0.78 / 0.47 -> 0.48 / 0.69 / 0.46 (off / 4 samples / SMAA), battle 0.33-0.39 / 0.46 / 0.37 -> 0.30 / 0.39 / 0.31.

Next agent: (1) the phone's readout with this build (R with S against the 2.8-3.9 ms CPU and 1.0 ms uploads it replaces);
(2) P4 on top (8 texels: 32 transform-feedback components, a third less to write and copy); (3) if S's fixed ~0.3 ms shows on
the phone, measure it without timing mode's per-pass glFinish (BWPORT_FRAMELOG-style frame totals) before cutting more.
Scratch scripts in the session's scratchpad p1b/: final.sh (the whole verification), time.sh / time2.sh, game.sh, probe.sh +
variant.py (shader variants via BWPORT_GPU_SHADERS), envp.sh.

### (2026-11-03): pass S on a test iPhone

The phone (A18 Pro, "OpenGL ES 3.0 Metal - 104", build 5d403e5e0) failed self-check 1 (frame 30, Resolution 3, 4 samples,
43888 records, 412 polygons): records 0-4091 right, every field 0 from record 4092 on (setup 69, its record 6: mid-polygon, so
not the shader's logic; an empty record writes xs = 1.0, not 0). 4092 x 192 = 785,664 bytes = 768 KiB - 768: not a row of the
span texture (record 4092 starts at texel 976 of row 47), not a power of two. Either transform feedback stops at ~4092 points
(a cap per draw, per session, per binding, or on gl_VertexID) or the buffer-to-texture copy stops at a byte count. Nothing here
reproduces it (Mac, llvmpipe, Simulator are right), so the self-check now decides on the phone:
- **Diagnostics** (gpu_g02_step, a few lines in the log after a failed check): a GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN query
  around the draw(s), the transform-feedback buffer mapped after a glFinish (GL_BUFFER_SIZE, the first wrong record, the count,
  where the all-zero tail starts), the same of the span texture's readback, and once the limits (GL_MAX_TEXTURE_SIZE,
  MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS / SEPARATE_ATTRIBS, MAX_VERTEX_OUTPUT_COMPONENTS, MAX_ELEMENTS_VERTICES,
  MAX_ELEMENT_INDEX on ES). Expected readings: "wrote 4092 of 43888" = a transform-feedback cap; "wrote 43888", buffer wrong from
  4092 = the writes are dropped; buffer right, texture wrong = the copy.
- **The cascade**, same frame, until the records are right: `tf` (one draw, the default: the Mac's cost unchanged) -> `chunk`
  (draws of 1024 points in the one session, gl_VertexID from 0 in each with uFirst: free, takes a cap per draw or on the vertex
  id) -> `range` (2048-record chunks = 393 KB, each BindBufferRange'd at its own offset in its own session: a cap per binding or
  session; 22 sessions at 43888 records, and each Begin flushes on Apple's GL, hence after chunk) -> `frag` (no transform
  feedback: g02_setup.vert compiled as a fragment shader with S_FRAG, four RGBA32UI targets, three draws writing texels 4j..4j+3
  into band j, the record built three times, then one draw interleaving the bands into the span texture) -> `cpu`
  (gpu_pass_s_off). A buffer found right with the texture wrong first keeps the way and splits the copy into TexSubImage2D calls
  of 32 rows (512 KB). The way found holds for the process (a new core starts from it); BWPORT_GPU_PASSS picks the first.
- Verified on the M2 (GL 4.1 Metal): each way forced, `--spans-check` 0 integer fields on the 37 captures at 1x/2x/3x/6x, AA
  off, 4 samples, SMAA (hi/lo attribute counts equal to tf's), and the --hi and 1x pictures byte-identical to 5d403e5e0 at 1x-4x
  in all three AA modes for tf, chunk, range, frag and cpu. Broken shaders (BWPORT_GPU_SHADERS; scratch passs-ios/badA-C): zeros
  from gl_VertexID 4092 step tf -> chunk (right); zeros from record 4092 step tf -> chunk -> range -> frag (right); a wrong xs
  everywhere steps to cpu. A short copy (a local hack, 47 rows) stepped tf -> tf with the copy in pieces (right).
  The Simulator (Apple's software ES 3.0, iOS app built from this tree): tf right at 1x (5707 records); with badB at
  Resolution 2 (SIMCTL_CHILD_BWPORT_GPU_SHADERS) the self-check printed the diagnostics (wrote 11405 of 11405, buffer and
  texture all 0 from 4092) and stepped tf -> chunk -> range -> frag, frag right (0 integer fields): every way compiles and
  runs on Apple's ES. The device IPA builds; the phone's next log decides.
- What each way costs on the M2 (gpu_render --time 200, Res 3, S min in ms, load ~3.5): town AA off tf 0.46 / chunk 0.88 /
  range 0.97 / frag 0.95, 4 samples 0.68 / 1.61 / 1.83 / 1.55; battle AA off 0.29 / 0.49 / 0.53 / 0.70, 4 samples 0.38 / 0.73
  / 0.81 / 0.79. (Draws inside one session cost about as much as separate sessions here: Apple's GL seems to split them.) The
  game (BWPORT_GPU_PASSS forced, town, Res 3, Smooth, Monitor at a pretended 120 Hz, 3 drawables, one 4 s bench row, load
  2-6), in-betweens/s and cores: 4 samples tf 60.1 / 0.42, chunk 59.7 / 0.42, range 60.1 / 0.51, frag 60.1 / 0.42, cpu 54.1 /
  0.68; SMAA 58.1 / 0.42, 59.9 / 0.42, 58.1 / 0.48, 57.6 / 0.43, 59.0 / 0.53; AA off 58.2 / 0.40, 58.9 / 0.41, 58.7 / 0.47,
  55.9 / 0.42, 42.0 / 0.50 (presents 119.3-120.0 in all). range's sessions cost CPU (~1.5 ms a tick more), the others not.
- **The fallback's cost.** The phone with pass S off (5d403e5e0, gpu3-aa4s-mon-sm) showed 0.2 in-betweens/s against 60.2 at
  8597aa725 (late 1.5/s, then held 51/s: the hold rule turns a few late mids into none). The Mac reproduces a milder form
  (BWPORT_GPU_SPANS=cpu, BWPORT_PANELHZ=120, BWPORT_DRAWABLES=3, town, Res 3, 4 samples, Smooth, Monitor; 3 alternating A/B
  pairs, two 4 s bench rows each, BWPORT_INTERP_REPORT, a perf log; load 2.5-3.5): pass S on 59.6 in-betweens/s, 0.39 cores;
  pass S off 40.5 mean (23-59 a row), late 2.8/s, held 15.4/s, 0.71-0.76 cores, the GL thread's CPU 222-248 ms/s.
  - The guess (job C put the 2D pack and the VRAM flatten on the worker ahead of the mid) measured small: the perf log's
    "worker 2D" is 1.10 ms with them and 1.04 without (B lines and A layers are the rest), and leaving them to the GL thread
    when the worker builds the records (tried, A/B both before and after the next fix) changed nothing (56.6 -> 57.1). Not
    kept.
  - What was big: the CPU's records went up from client memory, 6-8 MB a picture at 3x with 4 samples, and Apple's GL converts
    client memory to the texture layout on the GL thread inside TexSubImage2D (P3). They now go through the unpack ring
    (gpu_gl_unpack_map: one memcpy into a mapped buffer, one buffer-sourced TexSubImage2D the GPU does). Same A/B: the GL
    thread's CPU 222-248 -> 150-172 ms/s, cores 0.71-0.76 -> 0.64-0.68, CPU ms a tick 12.0-12.7 -> 10.8-11.4, in-betweens
    40.5 -> 50.2 (late 2.8 -> 1.1/s); a later, quieter set gave 56.6-57.1 (late 0.8-1.0/s). Shared code: the CPU-records path
    of every platform (the Windows/Linux fallback too; pass S's path is untouched). Pictures byte-identical (below).
  - What the Mac cannot show: the worker builds the mid's records (~4.2 ms on the phone with 4 samples) before the mid is due,
    as at 8597aa725; the phone's bench row with this build is the check.

###

Done, all five steps (commits 24d139edf, b38315510 and the one after them, one fragment per record): pass S builds the records on the GPU by default, `BWPORT_GPU_SPANS=cpu`
(gpu_render `--spans cpu`) is the CPU builder as before.
- The 32-bit forms (g02_spans.c gpu_s32_*) needed no splits for the slope step: within an edge's rows k <= dy, so S (k + 1) <=
  |dx| 2^18 (2^30 on the 16x grid); the study's "S k reaches 2^39" took k beyond the edge. aa_edge's Y-major sums keep the row
  centre and top relative to ax (whole part + 2^-20 fraction); the X-major window scan is closed-form (first and last pixel with
  16 N > -15 |dx| or 16 N <= 15 |dx|). tests/t02_spans.c: `gpu_tests 0 2048` checks every edge with dy <= 2048, |dx| <= 2048 on
  all its rows against p16's s64 code (0 differ; ~4 core-minutes), plus random edges up to the 16x grid.
- The left/right choice was a double compare in p16 and is not a function of the rationals (equal row-centre x's reached from
  different vertices round apart, 10% of such pairs in a probe): p16 now compares exactly (s64 cross products), and pass S
  the same way in 32 bits. vcore's pictures did not change on the 37 Mac captures.
- Attributes: float-float, every error-free step through an opaque XOR with a zero uniform (no reassociation, no fusing).
  Against the CPU's doubles: hi equal except values the CPU gets as exactly 0 and the GPU as ~1e-19 (4 of ~4M at 3x on the
  fuzz scenes), lo within the double's own rounding (~2^-50 of the value), which the picture does not see.
- Band lists from the edges (gpu_setup_extent): town 3x 7.3 pixel-visits a pixel against 6.8 from the records.
- One fragment per record, not per texel: the span texture is a 2D array of 12 layers (texel t of record r in layer t at
  (r & 1023, r >> 10); g00_head.glsl g_span), pass S draws twice into 8 and 4 layers (MRT; a context with fewer than 8 draw
  buffers falls back to the CPU builder). A fragment per texel (12 per record, each walking the row) cost S 2.3 ms (AA off) /
  4.0 ms (4 samples) at town 3x under load; per record 0.9-1.0 ms.
- Pass S is NOT the 0.1-0.3 ms the study expected: ~0.95 ms min at town 3x for both AA off (17 k records) and 4 samples (34 k),
  battle 0.5-0.6 (M2, load 2-5). It hardly grows with the records, so it is latency (few, long fragments), not throughput.
  Probes (shader copies via BWPORT_GPU_SHADERS, town 3x): the binary search and the header alone 0.27-0.32 ms (that includes
  the setup upload, two draws and the glFinish of the timing), + the row walk 0.62, + the packing 0.80, + the attribute ends
  0.95. (A shader that returns at once measured 0.95 too, against 0.27 for one that fetches the setup first: unexplained, and
  a reason to distrust small differences here.) Ideas for the next round, cheapest first: fewer local arrays (the edges are
  40 ints and the runs 2 x 12 + 2 x 13, likely spilled; re-fetch an edge from the setup texture instead), the inner O(ne^2)
  "passing vertex" loop out of the row walk (a per-polygon flag), P4's 8-texel records (one draw, eight targets), and only then
  an instanced draw per polygon instead of the search (the search costs nothing measurable).
- gpu_render, Resolution 3, min of 200, load 3.7-3.9 (old = the build before P1, cpu = `--spans cpu`, gpu = pass S; "frame" is
  CPU + GPU with a glFinish per pass):

  | frame | CPU ms old / cpu / gpu | uploads | S | R | P | frame |
  | --- | --- | --- | --- | --- | --- | --- |
  | town AA off | 1.99 (mean) / 2.02 / 0.12 | 0.23 / 0.39 / 0.05 | - / - / 1.21 | 1.36 / 1.42 / 1.40 | 0.68 / 0.68 / 0.64 | 3.96 / 4.38 / 3.43 |
  | town 4 samples | 4.23 (mean) / 4.22 / 0.12 | 0.56 / 1.06 / 0.05 | - / - / 0.95 | 3.90 / 3.61 / 2.84 | 1.45 / 1.43 / 1.16 | 9.40 / 9.65 / 5.19 |
  | battle AA off | 0.93 (mean) / 0.91 / 0.06 | 0.13 / 0.17 / 0.04 | - / - / 0.61 | 0.90 / 1.19 / 1.20 | 0.34 / 0.35 / 0.34 | 2.14 / 2.56 / 2.27 |
  | battle 4 samples | 1.80 (mean) / 1.78 / 0.06 | 0.24 / 0.31 / 0.04 | - / - / 0.60 | 2.26 / 2.28 / 2.32 | 0.39 / 0.40 / 0.39 | 4.37 / 4.60 / 3.47 |

  (R swings by 0.3-1 ms between runs a minute apart; S's AA-off figure is above its 4-sample one in this run.)
- The game (tools/mac_run.sh --noskip, town save, Resolution 3, 4 samples, Smooth, Monitor at BWPORT_PANELHZ=120, bench row at
  f1440; load ~4 with this piece's checks running on the GPU beside it): `BWPORT_GPU_SPANS=cpu` 0.85 cores, CPU 14.75 ms a tick,
  110 presents/s, 27.9 in-betweens/s; pass S 0.32 cores, 5.56 ms a tick, 114 presents/s, 34.2 in-betweens/s. A 10 s `sample`
  (3299 samples = one busy thread): gpu_prepare 1788 -> 82, gpu_render_prepared 1464 -> 536, the driver's twiddle 478 -> 80.
- iOS: the device build and IPA (`tools/port_wraps.py build/port/ios && ninja -C build/port/ios bwport && tools/ios_package.sh ipa`)
  and the Simulator app build; a Simulator run (Apple's software ES 3.0) switched to gpu at Resolution 2 in the town drew the
  town right with pass S (8 draw buffers there; no fallback message in the log). The phone is not measured: its GPU cost of
  pass S is the open number (P0's readout shows it inside R).
- The CPU fallback (BWPORT_GPU_SPANS=cpu) now transposes the records into the 12 layers before its upload: in the game sample
  gpu_g02_upload took 1041 of 3299 samples (0.3 cores at Resolution 3, 4 samples, Smooth at 120), more than the old 2D upload.
  If the fallback matters on some device, gpu_span_pack should write the layers directly (GpuSpans as 12 arrays).

Earlier next steps and replaced the two draws; the rest stands): (1) cut pass S to ~0.3 ms at town 3x (the ideas above; keep `--spans-check` at 0 integer differences and the pictures
byte-equal, scratch scripts in the session's scratchpad p1/: check.sh, same.sh, time.sh, tS.sh + variant.py for shader probes,
mkfuzz.py for the 730 scenes, pylib/vcs.py the PNG stand-in); (2) the phone's pass S time once P0's readout is in (it is inside
R's bracket); (3) the fallback's layer-major records; (4) P4 on top (8 texels: one pass S draw).

Verified on the M2 (GL 4.1 Metal; llvmpipe is not on this Mac, so no ES run):
- `--spans-check`: 0 integer fields differ on the 37 captures (town, battle, aa2pass/caps, aas/vcs, aas/vcsb) and on p13-p19's
  730 emitted + fuzz scenes (tools/vcore_gen.py, seed 1, 100 a piece), at 1x, 2x, 3x and 6x, AA off and 4 samples.
- gpu_same (a Mac copy of tools/gpu_same.sh over the 37 captures; the script itself has /root paths): old build vs new, the
  --hi and 1x pictures byte-identical at 1x-4x, AA off and 4 samples; `--spans cpu` likewise at 1x and 3x.
- gpu_compare.py at 1x on the 730 scenes (vcs.py's PNG helpers from a stand-in, the oracle checkout is not here): the same
  report before and after (723 identical, exact 100.0000 %). `vcore_check pNN --fuzz 100 --oracle gpu=...` (GPU_API=gl, the
  stand-in on PYTHONPATH) p13-p19 with the old and the new gpu_render (per-texel pass S): the same counts on every piece.

## P4 status (2026-11-02): measured, not built

The probes say the 8-texel records save about 0.05-0.08 ms a frame at town 3x with AA off and about 0.15 with 4 samples on
the M2, before the unpacking they would add to g_row and g_shade. That is at or under the ~0.15 ms bar, so P4 stops here.
Probes on ios after SMAA (91704f439), gpu_render town.vcs, Resolution 3, `--time 300`, min values, variants alternated over
6 rounds, load 2.5-3.3:
- (a) R's fetches (shader copies via BWPORT_GPU_SHADERS). A plain "drop t4" changes the row flags and with them the work
  (flags 0: no mask runs, no AA windows; R with S 4.8 -> 3.2 ms at 4 samples). So the control a0 keeps t4's fetch and sets
  its values to what a1 derives without the fetch (t4 = L.w, R.w, flags 0, x0 = xs). Also c1: a1 plus no fetch of t5 (in
  g_cover_row and g_cover2) or t11 (g_shade), about the 8-texel layout's whole fetch saving.

  | R with S, min ms | a0 (t4 fetched) | a1 (no t4) | c1 (no t4, t5, t11) |
  | --- | --- | --- | --- |
  | AA off | 1.51-1.96 | 1.46-1.95 | 1.42-1.91 |
  | 4 samples | 2.49-2.53 (2.89 once) | 2.40-2.54 | 2.38-2.47 (2.97-2.99 twice) |

  So t4's fetch costs ~0.03-0.05 ms with 4 samples and is lost in the noise without AA (the spread between rounds is
  0.1-0.4 ms), and all three fetches ~0.07-0.1 with 4 samples. One control that kept the fetches live through an OR with
  a uniform zero (`floatBitsToUint(f) & (uSwap >> 31)`) came out 0.7 ms slower than a0 with 4 samples: Metal's compiler
  schedules differently, and that is larger than anything the layout saves. The unpacking ALU a packed layout needs could
  cost as much as the fetches it saves.
- (b) Pass S with 8 outputs: g02_setup.c captured only s0-s7 (TransformFeedbackVaryings) and copied 8 n texels; the shader
  copy kept s8-s11 as globals and XORed them into s0-s3 at the end, so the row walk was not optimised away. Pass S (its
  draw and the copy), min of 300: AA off 0.50-0.73 -> 0.46-0.67 ms (~0.03-0.05); 4 samples 0.77-0.82 -> 0.70-0.72 (~0.07).
- Projected: AA off ~0.05-0.08 ms, 4 samples ~0.15 ms (R ~0.07, S ~0.07), less whatever the unpacking costs. On the
  phone the share could be larger (a tile-based GPU, the bytes cost more there) but nothing measures it there yet: if the
  phone's readout shows S or R's fetches as a real share, the probe scripts are in the session's scratchpad p4/ (variant.py,
  probe.sh, mkc.sh for the a/c variants). An exact layout fits: xs, xe, den_f, den_z, x0 and the flags as 16-bit halves
  (3 words), the 12 floats of w, z, s, t and their 6 low parts as they are (18), the six 9-bit colours (2-3), the mask's
  first two runs (2) and the union of the other four runs / the two AA windows (lo, hi, cov0 + 64 sh, ax, k, dx, dy a side)
  as 16-bit pairs (7): 28-29 of 32 words.

## P3 status (2026-11-02)

**Done for pass C's 1.2 MB only.** g07_compose_packed uploads the 2D stage and the 1x lines (786 KB + 393 KB, once a frame)
through one of two GL_PIXEL_UNPACK_BUFFERs (gpu_gl_unpack_map/_unmap/_done in gpu_gl.c: MapBufferRange with MAP_WRITE |
MAP_INVALIDATE_BUFFER, memcpy on the GL thread, TexSubImage2D from offsets; a map or unmap failure uploads from client memory).
The worker does not write into the mapped range: only the GL thread may map, the copy is 4 samples in 10 s, and the mids reuse
the pack, so it was not worth the hand-over. Everything else stays client memory.

Measured on the M2 (town, Resolution 3; load 2-3 unless said; `BWPORT_P3`, a local switch that was not committed, picked the sites
in one binary):
- Samples of the GL thread (10 s, Smooth at Monitor, BWPORT_PANELHZ=120, SMAA; ~4900 samples a busy thread): the twiddle
  (agxsTwiddleAddressCommon / convertTiles) under g07_compose_packed 46 -> 0, the function 68 -> 11. Under gpu_gl_data_tex
  12-33 samples stay (the polygon, band and list tables).
- passes=1 rows (60, Smooth off, SMAA), means of 239 frames: compose 1.03-1.10 -> 0.77-1.00 ms, uploads 0.05-0.06 -> 0.05-0.06.
- Bench rows (Smooth at Monitor, 120 Hz pretended, SMAA and 4 samples, three pairs each): cores and CPU ms a tick equal within
  the noise (0.35-0.41 / 5.8-6.8 both ways; DVFS moves them more than this does). The Mac was GPU-bound there, as before.
- Pictures: gpu_render old vs new, --hi and 1x byte-identical on the 37 captures at 1x-4x, AA off / 4 samples / SMAA. The game:
  36 live switches of Renderer, Resolution and AA with Smooth on, no GL error.

**What did not pay, and why.** The same buffers for the per-present tables (polygons ~10 KB, bands and lists ~42 KB) and the
texture rows: the passes rows' uploads part went 0.05 -> 0.28-0.32 ms (tables) and -> 0.12 (texture rows), the polygon table
alone 0.28-0.32 with SMAA and compose slower after it, and the bench rows' CPU did not drop. In the sample each buffer-sourced
TexSubImage2D makes a Metal blit encoder (blitCommandEncoderCommon, whose context init mallocs a large block): a fixed cost a
call that a 10 KB upload never earns back. The same holds for pass S's records copied from the transform-feedback buffer
(g02_setup.c, two such calls a frame; P4's area). vcore's 1x picture (Upload1x, 384 KB on capture frames) was left as it was:
no run here makes enough of them to measure.

**Found on the way: the polygon table stalls the GL thread.** With client memory, gpu_gl_data_tex(t_poly) waits in
GLDContextRec::finishResource (iokit_user_client_trap) for the GPU to finish the earlier frame's reads of that texture: 113 and
301 samples in two 10 s samples when GPU-bound, none under load 100 (the GPU was not behind). The band and list tables do not
wait. A PBO removes the wait but costs more GPU time (above); two t_poly textures used in turn would remove it without the blit.
Not done (g00_frame.c, pass S reads t_poly: after P4).

**Fixed (2026-11-02, the stalls round): rings of three.** The wait is not the polygon table's own: any client-memory
TexSubImage2D into a texture an earlier frame still reads waits, and the first such upload of a frame takes the whole wait. With
t_poly alone in turn the 10 s sample's waits moved to the band table (821 samples), with the tables in turn to the texture slots'
diff upload (305). Now every texture the CPU writes per frame goes into one of GPU_RING (3) in turn (gpu_gl.h): t_poly, t_band,
t_list (g00_frame.c), pass S's t_setup and t_find (g02_setup.c), and the t_tex / t_pal pair (g04_texture.c: it moves only on a new
texture build, an in-between keeps its frame's; each pair keeps its own diff copy, so a changed row goes up once into each). The
frame's passes read the one it wrote; an in-between is a frame here (gpu_render_prepared). No new GL calls, ~20 data textures a
core instead of ~8 (+1 MB for the texture pairs and their copies). Mac town, Resolution 4, 4 samples, Smooth at a pretended
120 Hz, 3 drawables, 10 s `sample` (load 2-3; ~4900 samples a busy thread; stalls/waits.py counts the topmost finishResource
under each site): under gpu_gl_data_tex / _diff 600 -> 0 (and 932 / 1329 -> 0 with BWPORT_GPUSLOW=20, the GPU ~80% busy).
**But the GL thread does not get the time back on the Mac:** the wait moves to glBeginTransformFeedback (pass S), which on
Apple's GL always flushes the context (GLDContextRec::flushContext, a new command buffer) and blocks on the driver's semaphore of
command buffers in flight (semaphore_wait_trap): under gpu_g02_records 513 -> 1436 (plain) and 567 / 755 -> 1593 / 2314 (GPUSLOW),
so the GL thread's driver waits in all are about the same (~1100-2100 samples either way). The difference: finishResource waited
for one earlier frame's reads to finish (the queue drained to it), the semaphore waits for a free command-buffer slot (the queue
stays full). A ring of three transform-feedback buffers (tried) did not stop the flush (1500). Bench rows (no sample, town, Smooth at Monitor, pretended 120 Hz, 3 drawables, load 2-3;
base = ios at 278a24cb5, new = the three fixes on 10e53dc4d): Res 4 / 4 samples 119.9 / 60.1 -> 120.0 / 60.1 presents / in-betweens
a second, 0.40 / 6.77 -> 0.40 / 6.79 cores / CPU ms a tick; Res 3 SMAA 119.9 / 60.1 -> 120.0 / 58.9, 0.40 / 6.78 -> 0.40 /
6.64; GPU-bound (BWPORT_GPUSLOW=20, the GPU ~80% busy), three runs each: presents 118.6 / 118.4 / 117.2 -> 119.0 / 118.7 /
119.0, in-betweens 31.2 / 40.2 / 24.2 -> 55.9 / 55.7 / 41.4, cores 0.41-0.42 both, CPU ms 6.86-7.07 -> 6.69-7.05. So when the
GPU is behind, more in-betweens make it (a frame no longer waits for the one before it to drain).
Not rotated: t_span (written by the GPU; only the CPU-records fallback uploads it from client memory and would wait),
pass E/P's own targets, the 2D stage (pass C: through the unpack ring), vcore's Upload1x (capture frames only).

## Pass R's cost (2026-11-03): what it spends, three register cuts, the pictures unchanged

Measured on the M2 (load 2.5-5, other agents' runs beside these) with `build/gpu/gpu_render --time` and a throwaway probe that
draws pass R 20 times a timed frame (Metal: 20 render passes in one command buffer; GL: 20 draws with glFlush between), so the
GPU stays at its clock and the numbers repeat to ~0.02 ms; min of 2-3 alternated rounds, pass R alone (pass S taken out), 3x.
GPU counters headless: `xcrun xctrace record --instrument 'Metal GPU Counters' --launch -- gpu_render ...`, then
`xctrace export --xpath '.../table[@schema="gpu-counter-value"]'` (and `gpu-counter-info` for the names: ALU Limiter, Fragment
Occupancy, Buffer Read Limiter, ...), averaged over the samples with fragment work.

**What it does per pixel** (a count variant writing its tallies into the colour; town / battle at 3x, the same at 4 samples):
7.3 / 3.9 list visits, 3.75 / 2.2 of them on a row the polygon covers, 1.6 / 1.75 pass the early depth test and are shaded and
textured. The bins already cull well; half the covered visits fail depth (the DS's order is not front to back).

**Where the time goes** (Metal, town AA off 0.71 ms / 4 samples 2.11 ms before; each a variant with one part taken out, so the
parts overlap): the fixed part (clear, rear plane, the stores of the 1 or 4 RGBA32UI layers) 0.11 / 0.11-0.20; texture decode and
blend 0.31 / 0.56 (0.17 / 0.37 after the cuts); g_shade 0.08 / 0.18; g_plot4 0.05 / 0.22; the coverage steps (g_cover2) about
half of the 4-sample cost. The counters: ALU limiter 58% (AA off) / 43% (4 samples) with F32 utilisation ~4% (integer work),
texture sample and buffer read limiters 4-10%, last-level cache 11-12%, read and write bandwidth ~4% each, and **fragment occupancy
24% / 16%**: pass R is latency-bound by its register count, most of all with 4 samples. Loads, stores and tile memory are not it.

**The cuts** (each its own commit, pictures byte-identical):
1. Filter=bilinear's `g4_filtered` (four more g4_sample calls, a uniform branch) lived in the only pass R program and held its
   registers every frame. Now `g00_raster.frag#1f` / `#2f` define G_FILTER and the cores pick them per frame when the filter
   applies (N > 1). Occupancy 24 -> 30% (AA off).
2. g_cover2 fetched both sample rows' edge windows (ten vec4) before any edge step; now row by row. Occupancy 18 -> 24% (4 samples).
3. The shading row's GRow (three vec4) was kept from g_row through the coverage steps; now its record index, and g_row_rec
   fetches the texels again for g_shade.

| pass R alone, ms | Metal before | Metal after | GL before | GL after |
| --- | --- | --- | --- | --- |
| town AA off (and SMAA) | 0.71 | 0.58 | 0.87 | 0.79 |
| town 4 samples | 2.11 | 1.69 | 2.54 | 2.09 |
| battle AA off | 0.59 | 0.49 | 0.70 | 0.64 |
| battle 4 samples | 1.78 | 1.41 | 2.06 | 1.68 |

(One alternated run, load 3.4-4.2.) Per cut, Metal town: (1) 0.72 -> 0.60 AA off, 4 samples ~same; (2) 4 samples 2.19 -> 1.89;
(3) 4 samples 1.79 -> 1.70. GL follows (its own compiler, the same registers).

**Tried, no gain** (all exact variants, timed the same way): prefetching the next list entry; taking attr from the list entry
instead of the polygon table (the compiler shares the fetch); fetching the polygon's texture word at the top of the loop (worse:
it holds registers); dropping the debug last-polygon lane (4 registers: no step in occupancy); g_cover_row's per-sample loop for
4 samples in place of g_cover2 (much worse); removing the 4x4-compressed or toon code (no change: a path that is present costs
nothing by itself; g4_filtered cost because of what it held live).

**Pictures.** Before against after, GL and Metal, 37 captures x `--aa 1, 2, smaa` x scale 1 and 3, the 1x picture and the `--hi`
layer: 444 of 444 identical; GL against Metal 222 of 222; the same at 3x with BWPORT_GPU_FILTER=bilinear (the filter programs):
222 of 222. `--spans-check` 0 integer fields. gpu_tests pass. In the game (town save, gpu 3x, Smooth off, shots at 4 samples, SMAA
and AA off, Metal and GL, before and after): the pictures look right, but the town save does not repeat run to run (its NPCs
walk: two runs of one build differ as much as before against after, only where they stand), so the byte proof is the captures'.

**For the phone.** The passes rows' R should drop by about the same fractions: Metal ~3.50 -> ~2.9 ms AA off and SMAA, ~6.4 ->
~5.1 with 4 samples; GL ~7.4 -> ~6.7 and ~10.2 -> ~8.4 (if the phone is as register-bound as the M2: its GPU's register file is
the same design, so the occupancy gains should carry over; the in-game clock is lower than gpu_render's loop, so the absolute
figures are higher there).

**Left.** Texture decode and blend are now the largest single part (~30% of pass R AA off): integer ALU, the counters say
ALU-bound once occupancy rose (ALU limiter 73% AA off). With 4 samples occupancy is still ~20-24%: the next register peak is
the depth step of g_cover2 (both rows' t0/t1/t4 and the two dlo texels with the four g_lin) and the plot's vector state.
A pass R as a compute kernel (choosing the SIMD shape, sharing a bin's list in threadgroup memory) is the structural option.

## E+P with 4 samples (2026-11-03): pass P in two draws

The phone's passes rows (docs/gpu-backends.md M5 item 2) had Metal's E+P at 4 samples 3.42-3.50 ms against GL's 2.87, AA off
1.15 against 1.65: Metal grew 3.0x, GL 1.7x. On the Mac gpu_render shows the same ratios (Metal 0.25 -> 0.76, GL 0.60 -> 1.12
wall) and the in-game passes rows too (Metal 0.64-0.67 -> 2.05-2.15).

**What cost where** (gpu_render town 3x, Metal, min ms, throwaway variants under load 3-5, so read the deltas): of the 4-sample
cost, the second p_base that p_sample inlined for the antialiasing's neighbour (its edge-mark walk; with that p_base replaced by
the neighbour's fogged colour E+P fell 0.90 -> 0.60, GL only -0.09), the slow samples' own walks (~0.13), the out-of-pixel
neighbours (~0.12), the alike pixels' p_thick4 walk (~0.09). Pass E is small in gpu_render (0.13 / 0.16) and in-game (0.36 /
0.30-0.36: the passes rows' new E). One draw ran each group of fragments at the pace of its slowest pixel.

**What changed** (shaders/g06_post.frag's multisampling main, both APIs, the pictures the same):
- The slow samples' work in parts: for an antialiasing sample the neighbour behind it is found first; one inside the pixel
  takes that sample's base (computed once, or its fogged colour: p_edge_mark is false for a sample that is not slow), one
  outside is a job of the same loop as the slow samples' own bases. The program holds one p_base; no base is computed twice.
- Pass P draws twice: `g06_post.frag#2+P_FAST` (a second program from the file; "#K+DEF" defines DEF in gpu_gl.c's
  build_program and gpu_msl.py) draws the pixels with no slow sample and discards the rest, the stencil marking where it
  wrote; the full program then runs where the stencil is 0. The GPU packs the slow pixels into full groups, and the first
  draw's program holds none of the slow code. GL: a DEPTH24_STENCIL8 texture on fbo_p, the stencil cleared at the pass's start,
  invalidated at its end, GL's default stencil state after. Metal: a memoryless Stencil8 attachment, both draws in one encoder.
- Tried, no gain: the p_thick4 walk in the second draw too (0.59 -> 0.66: alike pixels near edges are many); skipping a walk's
  direction when pass E's stage 2 (t_em[0], DS resolution) has no marked sample in the DS pixels it can reach (Metal 0.57 ->
  0.76, GL a little better: most walks end at their first step, so the two extra fetches cost more than they save).

**Before / after, Mac** (gpu_render --time 200, Resolution 3, min ms of E+P, before and after interleaved, load 1.9-4.5):

| | town 4 samples | town AA off | town SMAA | battle 4 samples | battle AA off |
| --- | --- | --- | --- | --- | --- |
| Metal | 0.76 -> 0.57-0.60 | 0.32 -> 0.32 | 0.29-0.31 -> 0.34 (E apart) | 0.12 -> 0.12 | 0.05 -> 0.05 |
| GL (wall) | 1.12-1.19 -> 0.94 | 0.63-0.69 = | 0.68 = | 0.37-0.41 -> 0.39 | 0.32 = |

Metal's 4-sample growth over AA off: 3.0x -> 1.8-1.9x (GL's 1.7-1.9x before, its own now 1.5x); Metal is below GL's absolute
time at both settings. The Metal columns but SMAA are with E in P's command buffer as before (a throwaway build; SMAA's program is untouched); the build as committed times E in a
command buffer of its own (the passes rows' E apart), which adds one buffer's start in timing mode only: gpu_render reads AA off
0.32 -> 0.34, battle 4 samples 0.12 -> 0.14, the in-game AA-off row 0.64-0.67 -> 0.74. Frames outside timing mode are one
command buffer as before.

In-game, the town save, gpu 3x, passes rows on Metal (load 2.3-3.4): aa4-passes E+P 2.05-2.15 -> 1.52-1.61 (E 0.30-0.37, P
1.15-1.30), aa1-passes 0.64-0.67 -> 0.74 (the extra buffer). GL's in-game rows ran at load 5-7 and spread 1.8-2.6 before and
after: no reading. Pictures: the 35 captures at 2x and 3x (AA off, 4 samples, SMAA) and 6x (4 samples), screen and --hi, plus
the 730 fuzz scenes at 2x with 4 samples, both APIs: 3900 files byte-identical to the build before, Metal identical to GL. The
room save at f1500, gpu 3x, 4 samples: the game area identical before and after on Metal and on GL; Metal against GL 198
pixels, all in the menu bar's text (as before the change).

**For the phone** (the perf test's three passes rows): the passes line now ends with `(E+P: E x, P y)` on Metal; the 4-sample
row's E+P should fall from ~3.4-3.5 ms toward ~2.5 (the Mac's in-game drop is 25-30%), below GL's 2.87 (GL's own should drop
too, ~15% on the Mac); AA off and SMAA as before plus the one buffer's start (a few hundredths of a ms on the Mac). The metal:
line's `renderer N ms/s` is no longer 0.0 on the passes rows.

## Open risks

- P1's 1x exactness rests on three 32-bit rewrites of p16's s64 arithmetic and a closed-form AA window; the C-side tests
  decide before any shader is written. If float-float attribute ends still move the 1x exact % on the corpus, the
  fallback inside P1 is to keep attribute ends on the CPU per polygon-row at 1x only (0.6 ms) with the GPU for N > 1,
  and say so.
- The phone's absolute GPU time is unmeasured; if P0 shows the fixed part (compose, present at the panel's resolution, the
  UI) near 4 ms, the 120 Hz target needs work outside this study (present at a lower size, a cheaper compose), and SMAA's
  three passes must be weighed against that number.
- SMAA's look on DS content is a judgement the maintainer makes on the phone; the ID-based detector may leave some edges
  (same ID, no edge flag: coplanar strips of one mesh) unsmoothed where 4 samples smoothed them.
- Apple's GL compiler crashes on array elements passed as out/inout (docs/gpu.md): pass S's shader is loop-heavy and
  must be written with that in mind; llvmpipe runs it for the tests either way.
- gpu_bridge.c is shared with the pacing agent: P2's option plumbing and P5 touch it; P1 and P3 do not.

## Pass R's texture decode cache (2026-11-03): built, measured, parked

Branch `park/texcache` (origin; commits 63dbee065, 2dc5b7b61, on 5919887b2): each texture the frame's polygons use is
decoded once a frame on the worker into words after the VRAM in uTex (keyed by its parameters without repeat/flip/transform
plus its palette base; re-decoded when a 1 KB texture or 256-byte palette chunk it read changed), and pass R fetches one word.
Exact: 444 captures, 222 bilinear, 4380 fuzz pictures and 1532 primed-cache pictures byte-identical, GL = Metal.

Measured (M2, pass R alone at 3x, interleaved mins): nearest textures 4-6% off pass R (Metal town 0.58 -> 0.54 AA off,
1.69 -> 1.61 with 4 samples; GL alike), Filter=bilinear 12-22% off (town 1.24 -> 0.99). The decode itself is small: the
captures use 10-30 textures, 70k-180k texels a frame, none 4x4-compressed; replacing every texel with a constant only
reaches 0.58 -> 0.49. CPU: ~0.045 ms a frame on the worker (0.3-0.6 ms for a cold build).

Not merged: with the default nearest filter the net on the phone is ~2-3% of the GPU frame for ~250 lines of cache and
change tracking, below the bar for added complexity. Merge it (rebase, regenerate msl/, rerun the picture checks) if
Filter=bilinear becomes a default or a common choice.

## Pass R's fixed part on the A18 Pro (2026-11-03): what it is, the 4-sample layers in RG32UI

The question: the phone's ceilings run (the maintainers' notes) had pass R at 2.67 of its 6.67 ms a frame with every polygon list
emptied (`ablate=r`; gpu 3x, 4 samples, the passes rows: each pass its own command buffer, the GPU at its bottom clock), so
"40% of R is fixed". The phone's GPU counters are not readable (the maintainers' notes, "iPhone bench": the A18 Pro's traces hold
one counter), so the parts were found by throwaway variants on the phone, passes rows, town save:

| variant (pass R's program and its 4 layers) | R | E | P |
| --- | --- | --- | --- |
| as it was: g00_raster, RGBA32UI (64 bytes a pixel) | 6.66-6.73 | 0.67-0.70 | 1.70-1.72 |
| every list empty (`ablate=r`, main's run) | 2.67 | 0.17 | 0.33 |
| a trivial program (4 constant texels a pixel), RGBA32UI | 2.78 | (no edges) | |
| the trivial program, RG32UI (32 bytes a pixel) | 1.59 | | |
| g00_raster with the layers RG32UI (half of every texel lost: a ceiling, the picture wrong) | 6.51-6.54 | 0.42 | 1.63 |

**What the fixed part is.** The empty-list R (2.67) is the trivial program's (2.78): the layers' tile stores, ~1.2 ms per 32
bytes a pixel at this clock, not the rear plane or the loop's start. But in the real pass they are not additive: the stores of
one tile overlap the next tiles' shading, so halving the bytes saves R only 0.15-0.2 ms (the RG32UI ceiling), not 1.2. R is
bound by its shading; the "40% fixed" is what remains when the shading is taken away. Pass E (stage 0 reads each flagged
pixel's samples and their neighbours) gains most from narrower texels (-0.26).

**What changed** (shaders/g00_head.glsl, g00_raster.frag, g06_edgemap.frag, g06_post.frag, src/gpu_mtl.m, g00_frame.c, both
APIs, k = 2 only): with 4 samples each sample is one RG32UI texel: depth | opaque ID << 24 (sample 0: | the edge-sample bit 30 |
the alike bit 31), colour (pass R's 23-bit form as it keeps it) | F_FOG, F_EDGE, F_TRANS << 23 | F_WRITTEN << 26 | coverage
<< 27. That is all passes E and P read; the translucent ID, F_STENCIL and the last polygon (debug) are pass R's own state. g_samp
unpacks to the old form, so E and P read the same values. The 1-sample layout (AA off, SMAA: its detector reads the last
polygon) is unchanged. The readbacks of the first sample (gpu_read_3d's aux, gpu_read_3d_raster) expand it (gpu_samp_unpack;
those three fields 0 at k = 2; the debug tools that read them run at 1 sample). GLES 3.0: RG32UI is colour-renderable, the
readback's RGBA_INTEGER gives r, g, 0, 1.

**Tried on the way, not taken:** memoryless or tile-resident layers (E and P read neighbours across tiles, so the layers must
reach memory); a load/store or clear change (the pass already loads DontCare and writes every pixel: there is no clear to
fold); fewer, wider attachments for the same bytes (two RGBA32UI of two samples each: the same stores, a component select per
fetch in E and P; not built).

**Phone A/B** (tools/ios_device_ab.py, A B A B A B, passes rows, min of 3, every run thermal nominal):

| ms a frame | before | after | |
| --- | --- | --- | --- |
| R | 6.66 (6.66-6.68) | 6.61 (6.61-6.63) | -0.8% |
| E | 0.67-0.70 | 0.53-0.54 | -0.15 |
| P | 1.71-1.72 | 1.71-1.72 | = |
| E+P | 2.38 | 2.24 | -5.9% |

About 0.2 ms of the 4-sample frame's ~9.9 (2%), and the same again per in-between with Smooth; the layers take half the memory
(28 -> 14 MB at 3x). Below the RG32UI ceiling (R -0.17, E -0.26, P -0.07) by the packing and unpacking ALU.

**Mac** (gpu_render --time 200, scale 3, 4 samples, min ms, before/after alternated 3 rounds, load 3.4-4.4 with other agents'
builds): GL town R (with S) 3.25 -> 2.95, 3.12 -> 2.91, P 0.97 -> 0.89, 0.98 -> 0.86; GL battle R 2.43 -> 2.25, 3.13 -> 3.03;
Metal town R 2.04 -> 1.93, 2.03 -> 1.95, P = ; Metal battle R 1.59 -> 1.52, 2.28 -> 2.18. One GL town pair ran beside a build
(3.55 -> 3.90). GL is not slower.

**Pictures.** Before against after, GL and Metal, the 37 captures x `--aa 1, 2, smaa` x scale 1 and 3, the 1x picture and the
`--hi` layer: 444 of 444 identical; GL against Metal 222 of 222; at 3x with BWPORT_GPU_FILTER=bilinear (pass R's filter
programs) 222 of 222 and GL against Metal 111 of 111. `--spans-check` at 3x with 4 samples, both APIs, the 37 captures: 0
integer fields. gpu_tests pass. In the game (town save, gpu 3x, 4 samples, Metal and GL on the Mac; Metal on the phone): the
picture looks right.

**Left for pass R on the phone:** its shading (the per-polygon work, the maintainers' notes item 2). The register cuts that paid on
the M2 did not on the A18 Pro, which allocates registers as a shader runs (Apple family 9's dynamic caching) instead of by the
program's peak; so occupancy by register count, the M2's limiter, is likely not the phone's. Without counters, the next step is
ablating parts of the shading on the phone (the texture decode, g_cover2, g_plot4) the way this section did the stores.

## Pass R's shading on the A18 Pro (2026-11-03): where it goes, the inner run (built, phone A/B pending)

**How measured.** Throwaway variants of pass R (and of pass S) compiled into one bench app, picked by an environment
variable at the core's start (the ablation patch is kept at the session scratchpad's rshade/abl.patch, not in the repo), each
one phone run from the town save: the passes rows at 60 Hz, no Smooth, gpu 3x, 4 samples, then AA off, then SMAA (each 240
frames; the rows are different frames, so the three rows are not one scene), with pass S's and pass R's own GPU times logged
apart (the passes row's R is S + R). The variants drop the work from a point on and keep its results live (a variant whose
result is not stored is compiled away: the first WALK/ROW at 4 samples wrote into the debug lane, which the 4-sample layers do
not store, and measured only the fixed part). Every run thermal nominal; R alone, ms a frame:

| variant | 4 samples | AA off (row 2) | AA off (SMAA row) |
| --- | --- | --- | --- |
| as it is (5 runs) | 4.46-4.61 | 3.25-3.59 | 3.01-3.17 |
| WALK: the list entries only (the fixed part with them) | 0.70 | 0.66 | 0.50 |
| ROW: + g_row's record fetches (3 a row) | 1.32 | 1.08 | 0.99 |
| COV: + the coverage, depth and edge steps (no shading) | 3.58-3.84 | 2.18-2.43 | 1.82-2.06 |
| SHADE: + g_shade, the plot a plain select | 3.91 | 2.77 | 2.43 |
| TEX: + the texture, the plot a plain select | 4.03 | 3.26 | 2.88 |
| NOTEX: all but the texture | 3.92 | 3.13 | 2.80 |
| NOPLOT: all but g_plot4 (a plain select) | 4.13 | 3.32 | 2.81 |
| C_DEPTH: no edge step (hit = the depth test) | 3.49 | 2.82 | 2.82 |
| C_BOUND: no exact depth, no edge step (more pixels shaded) | 3.42 | 2.64 | 2.46 |
| C_NOLIN: the depth by the float formula instead of g_lin | 3.80 | 3.06 | 3.12 |
| E_NOAA: the edge step without the antialiasing windows | 3.82 | 3.02 | 2.73 |
| E_NOT5: the edge step without t5's fetch | 4.47 | 3.32 | 3.08 |
| LAZY: g_row fetches t1, t4 only after t0's run test | 4.72 | 3.57 | 3.14 |

**The breakdown with 4 samples** (R alone ~4.55 of the passes row's ~5.6; pass S ~1.0): the fixed part and the list walk
0.7; g_row's six fetches a visit 0.6; the float bound and the exact depth ~1.2, of it g_lin ~0.75; **the edge step ~1.05**, of
it the antialiasing windows (t6-t9 and g_aa_side) ~0.7, t5's fetch ~0.1; g_shade ~0.3, the texture ~0.6, g_plot4 ~0.4 (these
three overlap). AA off: walk 0.5-0.66, g_row ~0.4, coverage 1.0-1.3 (edges 0.2-0.7), shading 0.9-1.1. On the A18 a fetch the
shader waits for costs: g_row's three fetches a row are 0.6 ms, and fetching t1/t4 later (LAZY) is worse (+0.2), as on the M2.
Every capture has antialiased polygons (DISP3DCNT bit 4), so the windows are on almost every row.

**Pass S** (1.0 ms with 4 samples, 0.7-0.8 AA off; measured beside R in the same runs). The S variants (no search, no AA
windows, no attribute ends, no mask) made pass S's self-check fail at the start, so the app turned pass S off and they measured
nothing; not repeated. Its cost scales with the records (rows of the N k grid: 0.75 -> 1.0 ms from 3x to 6x rows).

**4 samples against AA off and SMAA** (the same run, the passes rows; different frames, so ~0.3 ms of scene drift between the
rows): 4 samples S+R 5.64, E+P 1.84 = 7.48 ms; AA off 3.81 + 1.32 = 5.13; SMAA 4.13 + 1.51 + 0.26 = 5.90. So 4 samples cost
~2.35 ms a frame over AA off (R +1.8: the coverage, depth and edge steps per sample; E+P +0.5), SMAA ~0.5-0.8. Of R's 4-sample
extra, the edge step and g_lin are the per-sample parts that can shrink exactly; the rest (fetches, shading once a pixel) is
already shared by the four samples.

**The cut (built, exact, not yet timed on the phone): the inner run.** Pass S (and the CPU builder) writes each record's inner
run into t10.zw: the pixels after t5's first mask run and before its second (the runs are sorted, so in none of them), and with
antialiasing out of both windows (t6, t8; a DS-grid window lo..hi is lo m..hi m + m - 1 on the N grid; a window inside the run
keeps its left part). Those pixels have no edge flag and full coverage, so pass R's edge step (g_cover2 per row, g_cover_row)
gives them that without reading t5-t9 or running g_edge_px. t10.zw was free for it because the s low parts moved to t11.zw
(t10 is already fetched for the depth step; g_shade reads t11 anyway). A first version put the run in t4.xy and moved L.t, R.t
into t11: one pixel at 1x (f001600) changed, so the t coordinate is read from t4 as before. Files: shaders/g02_setup.vert,
g03_pixel.glsl, src/g02_spans.c/.h (the layout), src/g02_setup.c (the spans-check fields), msl/.
Checks (Mac, after rebasing on 90f03b07e): before against after, GL and Metal, the 37 captures x `--aa 1, 2, smaa` x scale 1
and 3, picture and `--hi` layer: 444 of 444 identical; GL against Metal 222 of 222; bilinear at 3x 222 of 222 and GL = Metal
111 of 111; `--spans-check` (3x 4 samples, 3x and 1x AA off, both APIs) 0 integer fields in 222 runs; gpu_tests pass.
Mac GL (gpu_render --time 200, 3x, min ms, 3 alternated rounds, load 3.4-4.0): town R+S 1.57 -> 1.48-1.55 AA off, 2.90-2.98 ->
2.92-3.00 with 4 samples; battle 1.24 -> 1.09-1.36, 2.29-2.35 -> 2.19-2.23: GL is not slower.
Expected on the phone: most of the edge step's ~1.05 ms with 4 samples (the window part alone is 0.7) and 0.2-0.5 AA off.

**Pending: the phone A/B** (the phone was locked at the try, then taken back by the maintainer). Branch a private branch,
commit "WIP" on 90f03b07e. Build the two apps with the S/R log line (rshade/mkapp.sh NAME: the worktree as it is; base from
HEAD~1's port/gpu), then rshade/ab.sh base.app d.app 3 d: three A B rounds of the passes rows above (about 45 s a run). Merge
if R with 4 samples drops by 0.3 ms or more and AA off does not rise; else park it.

**Next after it:** g_lin's row constants (floor of both ends, their fractions, dz / den: ~20 of ~140 integer operations a visit
per row) from pass S into the record (t10.xy could hold the fractions, the quotient and remainder need two more words), worth
at most ~0.2-0.25 ms with 4 samples by the C_NOLIN ceiling of 0.75.

## Pass R's row-pair cuts (2026-11-04): g_lin and the windows once a row, measured on the Mac (phone pending)

**What changed (both exact, merged with the phone A/B pending).** With 4 samples g_cover2 visits a pixel's two sample rows,
two samples each, and did every per-sample step alone. (1) `g_depth2` (e63259d27): g_lin's row constants (the ends' floor and
fraction parts, dz's quotient and remainder by den_z: one float division, a g_fixq) once a row instead of once a sample, and the
second sample's quotient from the first one's remainder (m + dr = qq d + rem + dr with both remainders below d: one compare
instead of a g_fixq). The float sum keeps its expression, so the values are the same. (2) `g_edge2` / `g_aa_side2` (82022653f):
the edge step for a row's two samples at once; a window's record parts (cov0, sh, ax, k, dx, dy) and N's constant decoded once,
the window skipped when neither sample lies in it, the second sample's N as N + 2 dy (DS-grid rows: from its own x / m). AA off
(one sample a row) is unchanged.
Checks for each, before against after: GL and Metal, the 37 captures x `--aa 1, 2, smaa` x scale 1 and 3, picture and `--hi`
layer, 444 of 444 identical; GL = Metal 222 of 222; bilinear at 3x 222 of 222 (GL = Metal 111 of 111); `--spans-check` (3x 4
samples, 3x and 1x AA off, both APIs) 0 integer fields in 222 runs; gpu_tests pass.

**Mac M2** (gpu_render --time 200, 3x, the min of 5 alternated rounds of each build's min, load 1.7-3.1; R = R+S - S):

| case | Metal R+S before -> after | Metal R alone | GL R+S | GL R alone |
| --- | --- | --- | --- | --- |
| town (t1401), 4 samples | 1.74 -> 1.54 | 1.41 -> 1.20 (-15 %) | 2.93 -> 2.59 | 2.19 -> 1.83 |
| battle, 4 samples | 1.32 -> 1.14 | 1.16 -> 0.98 (-16 %) | 2.15 -> 1.91 | 1.73 -> 1.49 |
| town / battle, AA off | 0.74 / 0.55 -> 0.73 / 0.55 | = | 1.51 / 1.08 -> 1.47 / 1.12 | = (noise) |

Of it, g_depth2 alone: Metal town 1.76 -> 1.67, battle 1.32 -> 1.22; g_edge2 on top 1.65 -> 1.57, 1.23 -> 1.15. A branch-free
g_aa_side2 (compute, then select by the window test) was better on GL (-0.2 / -0.13) but less on Metal (-0.05 / -0.08): the
branching one is in. **Expected on the phone:** a part of g_lin's ~0.75 ms and of the windows' ~0.7 ms with 4 samples; the A/B
is the one in "Pass R's shading on the A18 Pro" (rshade/ab.sh), 4 samples and AA off, merge rule as there (AA off must not rise).

**Tried, slower on the Mac, not kept** (exact in every check):
- *GPU_ROW_SPLIT*: a row flag (bit 7) set by pass S and the CPU builder when the inner run is not empty, the left window (and a
  vertical left edge's column) lies before it and the right window after it; a pixel before the inner run then reads and works
  only the left window (t6, t7), one after it only the right one (t8, t9). 0 integer fields, 444 of 444, but Metal 4 samples
  town 1.55 -> 1.64, battle 1.13 -> 1.20 (GL -0.05 / -0.03). Keeping all four fetches and gating only the math: the same; one
  g_aa_side2 call on the chosen window: the same; the flag read but the side forced to 0: as before (1.57). So the side branch
  itself costs: in a 32-lane group, a small polygon's left-edge and right-edge lanes run both branches. The M2 hides the two
  windows' fetches (dropping t9's fetch alone: no change). A phone with costlier fetches might differ: the variant's three
  edits (g02_spans.c/.h, g02_setup.vert's flag, g03_pixel.glsl's side) are in the session scratchpad's rexact2/c3src.
- *Rows with no sample left skip their depth and edge steps* (if (any(c.xy)) per row): town 1.64 -> 1.69 on top of SPLIT.
  The M2 prefers the straight-line pair.

**Ceilings measured on the Mac (pictures wrong, time only), after the two cuts:** the whole depth step by the float formula
instead of g_lin: Metal town 1.65 -> 1.56, battle 1.22 -> 1.11 (what pass S could still take by writing g_lin's row constants
into the record: at most ~0.1 ms here; the record has one word free in every row layout, t9.w, so it needs a wider record or
packing); the edge step without the antialiasing windows: 1.65 -> 1.35, 1.22 -> 0.99 (the four window fetches and the math;
the one-sided split above did not get it).

**Pass S's own cost** (Mac, S alone; gpu_render times S apart, so no self-check is in the way; pictures wrong): Metal town 4
samples 0.33 ms: the two edge attributes (s_edge_attr's float-float math, both ends) 0.33 -> 0.24 (~27 %); the polygon's binary
search done twice: no change (the search is ~0); every integer division made a shift: no faster (0.37: the changed paths); the
two insertion sorts (the runs, the mask) removed: 0.34 -> 0.31 (~10 %). The rest (~60 %) is the edge walk itself (s_edge_span
twice an edge, the runs, the windows, the record's stores). An exact cut in s_edge_attr needs the same float-float digits (the
lo parts feed g_lin's fractions), so a shortcut for n = 0 or n = d is not safe without proving the digits equal.

## Pass S's and pass C's exact cuts (2026-11-04): private arrays and integer divisions out, measured on the Mac (phone pending)

**Where pass S went** (Mac M2, Metal, gpu_render --time, 3x town t1401 with 4 samples, S alone 0.33 ms; throwaway ablations,
pictures wrong): the float-float edge attributes 0.06 of it after the s_k cut below (the perspective part 0.02, the colours ~0.005);
s_k's XOR barrier 0.04 (~12 %); s_mid_cmp's two integer divisions 0.02; the runs' insertion sort 0.01; the AA windows ~0.01; the
record's stores ~0.03; threadgroup 32 or 128 instead of 64: no change. The big one was not an operation but private memory: the four
edge arrays indexed by le and re, and the mask's tl/th/mlo/mhi arrays (Apple GPUs keep a dynamically indexed private array in memory).

**What changed** (port/gpu/shaders/g02_setup.vert, the Metal kernel generated from it; all exact):
1. The edges fetched from the setup texture where they are used (cached), not copied into arrays: -15 % alone.
2. The edge-flag mask in one pass over the already sorted, merged runs, the row's last pixel put before the first run that starts
   after it (where the stable insertion sort put it); no arrays: -5 %.
3. s_mid_cmp by the cross products n1 d2, n2 d1 in two 12-bit limbs (d < 2^13, |n| < 2^30; the division path kept beyond): -4 %.
4. s_k the identity under S_COMP: the kernel is compiled with MTLMathModeSafe (no reassociation, no contraction) and the MSL keeps
   every step a statement of its own (each s_k argument is a variable). -12 %. On GL the XOR stays: removing it there changed the
   records' lo bits on Apple's GL (148 of 148 runs), not one on Metal (0 of 148).
Also a dot (ne = 0) now reads its edge values as 0, as the CPU builder does, instead of an unset array element (no capture has one).
**Tried, not kept:** merging the runs on the way into the mask too (no runlo/runhi arrays): slower on Metal (0.204 -> 0.227).

**Pass C** (g07_compose.frag; Metal pass C's command buffer GPU time, 3x town 0.150 ms; ablations): the composition logic (priorities,
candidates, blending, master brightness) ~0.04, the stage and 3D fetches ~0.03, the pass itself ~0.055 (one fetch and the store);
the integer divisions ~0.03 of it: q / N (two) and c_3d's scroll modulo and scale (three). Cut, exact: d = int((q + 1/2) / N) (the
quotient lies at least 1/2N from the next integer, the float error is ~1e-5 at these sizes), the modulo as one conditional subtraction
(the sum is below 3W), the 3D scale only when cScale3d != cScale. The second composition for AA edges costs ~0.004: left.

**Checks**, before (f2548a3f5) against after: the 37 captures x `--aa 1, 2, smaa` x scale 1 and 3, GL and Metal, picture and `--hi`:
444 of 444 identical, GL = Metal 222 of 222; pass C's screens (`--compose gpu`, 1x and 3x, aa 1, 2, smaa and `--3d-from vcore`) 592
of 592, GL = Metal 296 of 296; bilinear at 3x 222 of 222 (GL = Metal 111 of 111); pass S's raw records (read back, every field's
bits, md5) at 3x and 1x with 4 samples and AA off, both APIs: 296 of 296 identical, `--spans-check` 0 integer fields in all;
gpu_fastdiff's fast and exact pictures 74 of 74; gpu_tests pass.

**Mac M2** (gpu_render --time 300, 3x, min of 5 alternated rounds of each build's min; load 5-60 during the run, the same numbers
as the quieter runs before it at load 4-5):

| case | Metal S | Metal R+S | Metal pass C (GPU) | GL S | GL R+S | GL pass C (wall) |
| --- | --- | --- | --- | --- | --- | --- |
| town, 4 samples | 0.330 -> 0.195 (-41 %) | 1.545 -> 1.408 | 0.147 -> 0.117 (-20 %) | 0.693 -> 0.603 | 2.521 -> 2.448 | 0.46 -> 0.43 |
| town, AA off | 0.183 -> 0.104 (-43 %) | 0.723 -> 0.644 | 0.147 -> 0.118 | 0.434 -> 0.353 | 1.364 -> 1.312 | 0.47 -> 0.41 |
| battle, 4 samples | 0.157 -> 0.091 (-42 %) | 1.128 -> 1.048 | 0.154 -> 0.123 | 0.372 -> 0.294 | 1.825 -> 1.768 | 0.47 -> 0.44 |
| battle, AA off | 0.098 -> 0.051 | 0.545 -> 0.490 | 0.154 -> 0.123 | 0.295 -> 0.282 | 1.063 -> 1.034 | 0.47 -> 0.44 |

**Expected on the phone:** pass S ~1.0 ms a frame at 3x with 4 samples (more with Smooth: it runs per in-between too) by a similar
share if the A18 Pro's private memory and divisions cost alike, and compose 0.6-0.7 ms by ~20 %. The phone should verify: the
passes rows (exact renderer, 4 samples and AA off, with and without Smooth) before and after; and since s_k's identity in the kernel
rests on the Metal compiler's safe math, the phone's pictures with the exact renderer against the Mac's for a capture (the records'
lo bits feed g_lin's fractions; the bridge's self-check compares integer fields only and would not see a difference there).

## Pass P's slow draw with 4 samples (2026-11-04): where its time goes, measured on the Mac (study, nothing built)

**Question.** With 4 samples pass P's second draw (the slow pixels, 5-6 % of the picture: the stencil's 0 after the P_FAST draw)
costs about as much as the first; the in-game passes rows had P 1.35 ms of which `ablate=pslow` 0.73. Why do so few pixels
cost that, and which per-sample work is it?

**Method.** gpu_render on the town capture (scratchpad cpresent/town.vcs, the maintainer's save), `--scale 3 --aa 2 --api metal
--time 60..150`, the min of 5-6 interleaved rounds, under the shared timing lock, load 3-7 (five agents measuring). Throwaway
variants of the slow draw's program made by editing the generated MSL (port/gpu/msl/g06_post.frag_k2.metal; no glslang on
the Mac, so the GLSL stayed as committed and only the Metal binary changed): `stub` returns p_resolve of the four fogged
samples right after the slow test (the draw's launch, stencil test and group packing alone, no slow work); `noaa` skips the
antialiasing's neighbour search (aam = 0, so also the outside-the-pixel neighbour bases); `nowalk` cuts p_edge_mark's
Edge=ds thickening walk (p_fine_mark stays); `floor` = noaa + nowalk; `nothick` returns 0 from p_thick4 (the alike pixels'
walk, both draws); `fast1` makes the P_FAST draw treat every pixel as alike without p_thick4 (one sample load and the fog: the
first draw's bandwidth floor). The standard ablations: `pslow` (4) skips the second draw, `noP` (8) skips pass P.

**The M2's GPU clock stepped between two states during the hour** (the same binary: a whole frame 2.9 ms in one, 3.6-4.1 in the
other, every variant moving together, load 3-7); each state's readings are listed apart, the deltas agree between them.

| variant | low-clock state (min ms, whole frame) | high-clock state | what it says |
| --- | --- | --- | --- |
| base | 2.89-2.92 (E 0.15 in it) | 3.93-4.17 | |
| pslow (no second draw) | 2.61-2.74 | 3.55-3.72 | the slow draw: **0.18-0.30 / 0.43** |
| stub (second draw, no slow work) | 2.66 | 3.55-3.71 | its launch + stencil + packing: **~0.03 (nothing)** |
| noaa | 2.75 | 3.77-3.85 | the AA neighbour search + outside bases: **0.14 / 0.15-0.2 (about half)** |
| nowalk | (no low-state sample) | 3.68-3.77 | the thickening walks: **0.2-0.3 (over half; overlaps noaa)** |
| nothick | 3.42 (one sample) | 3.95-4.07 | the alike pixels' p_thick4: **0** |
| noP | 2.25-2.30 | 3.08 | all of P: 0.62-0.64 / 0.85-1.0; the first draw is **0.33-0.47**, more than the second |

**The steadiest run** (10:37, load 3.4, the high-clock state, 5 rounds x `--time 60`, every value within 0.1 of its min but
where noted; whole-frame min ms):

| base | pslow | floor | nowalk | noaa | noP | fast1 | fast1 + pslow |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 3.80-3.87 | 3.48-3.57 | 3.51-3.55 | 3.57-3.63 | 3.70-3.77 | 2.95-3.00 | 2.63 (3.2-3.6 the others) | 3.25-3.33 |

So here: the slow draw 0.32; `floor` is `pslow` + 0.03 (the walks and the AA search are the whole of it; loads, p_fine_mark,
fog and resolve nothing); the walks alone 0.23 (72 %), the AA search alone 0.10; the first draw 0.53, of which a full-screen
draw that loads one sample and fogs it is 0.30 (`fast1 + pslow` - noP: every full-screen pass at 3x pays about this) and the
non-alike pixels' four loads, slow test and resolve 0.23.

**Answers.**
- *The stencil draw's overhead is not it.* `stub` equals `pslow`: starting the second draw, testing 442k stencil values and
  packing the survivors costs nothing measurable. (The aa-2pass lesson, "scattered fragments cost their full-screen share",
  was about one draw at the slowest pixel's pace; the two-draw cut already fixed that.) So the 0.26-0.43 ms is the slow
  pixels' own work: ~24k pixels at 3x, 10-18 ns each, i.e. the draw is latency-bound on dependent texture fetches: a slow pixel
  does up to 4 samples x (4 directions x 2 steps of walk, each step's fetch deciding the next) plus the AA search (5 fetches a
  sample) plus a neighbour's base (another walk). Each walk is a chain the GPU cannot issue ahead, and most walks end at their
  first step, so the latency, not the fetch count, is the cost: the per-direction skip tried on 2026-11-03 (two extra fetches
  to save the walks) lost for the same reason.
- *Which work dominates:* the thickening walks (p_edge_mark for the slow samples and their AA neighbours) are the larger half,
  the AA neighbour search and the outside-pixel neighbour bases the smaller half; the resolve, the fog and the loads are the
  floor. The alike pixels' p_thick4 walk in the first draw is free (its 2-per-direction pairing already serves).
- *The first draw is the bigger piece now* (0.33-0.53 ms of P's 0.62-1.0): a full-screen pass at 3x reading up to four
  RGBA32UI samples a pixel (64 B; 28 MB a frame) plus the edge map; 0.30 of it is what any full-screen pass that reads one
  sample costs at this clock (`fast1`), the rest the non-alike pixels' three more loads and the resolve. That floor is pass
  P's real ceiling with 4 samples: only fewer or smaller full-screen passes move it.

**What a cheaper exact resolve could look like, ranked.**
1. *Pass E precomputes the thickening marks* (expected: most of the walks' 0.2-0.3 ms back-to-back, ~0.4-0.5 in game; effort
   medium; risk low: Exact's picture must stay byte-identical, picsab2 444/444). Pass E already walks from edge samples at
   DS resolution (its stage 2 marks the DS pixels a walk can reach). A stage at N k resolution that marks each sample
   p_edge_mark would hit (the walk run once from each fine edge sample outward, like p_thick4's pairing, writing a bit per
   sample) turns p_base's walk into one fetch for the slow samples and their neighbours. Pass E's cost grows (its full-screen
   stage at N k), so measure E+P together; the aa-2pass lesson says a sparse stencil-tested stage does not save its share,
   so it should be the dense stage that writes the bits (per DS pixel, 4 bits x 4 samples = one R16UI).
2. *The AA search from pass E too* (expected 0.1-0.15 / ~0.25 in game; effort medium; risk low): the farthest-behind
   4-neighbour of each opaque edge sample depends only on depths; pass E's dense stage can write its P_DIR index (3 bits a
   sample) in the same texel as 1, so the slow draw does no search and knows its outside jobs at once.
3. *A compute pass over a compacted slow-pixel list* (expected: nothing beyond 1-2; effort high; risk medium): `stub` shows
   packing is already free; a compute dispatch would only move the same dependent chains to threads, and Apple's GPU runs the
   stencil-packed fragments as full groups already. Not worth building.
4. *Splitting the slow work by kind* (edge-marked only vs AA only vs both): no gain expected; the loop over jobs already pays
   for the most jobs in a group, and the stub shows group overhead is nil.
5. *The first draw* (0.33-0.53 ms, 0.30 of it the full-screen floor): bound by the sample layers' bandwidth; a cut needs pass R to write less per sample
   (docs/gpu-cost.md "the 4-sample layers in RG32UI"), agent 4's area. With it the alike path's single load would be 8 B.

**Checks any of 1-2 needs:** picsab2 BEFORE AFTER 444/444 identical (Exact), gpu_fastdiff's 4-sample mean unchanged at 8.20 %
(Fast takes P's edges), the GL forms compiling and gpu_render --api gl identical pictures, in-game passes rows E+P before and
after at 4 samples and AA off, and the phone's passes rows.

## Pass E writes the edge marks (2026-11-04): item 1 above, built, measured on the Mac (phone pending)

**What changed** (shaders/g06_edgemap.frag, g06_post.frag; both APIs; 4 samples only, k = 1 untouched):
- Pass E has a stage 3 at N (after stage 1; R8UI, t_em[3] / em[3]): for each sample of the pixel the whole of p_edge_mark, the
  fine mark (stage 0's bit) or the Edge=ds / Edge=fine thickening (T = N k or k samples). It runs on every pixel; where the DS
  pixel's map (stage 1) is 0 it writes 0 after that one fetch, the same test pass P made.
- Its walk is not a chain. On each line the walk marks its sample when the first sample that stage 0 marked fine lies within
  T - 1 steps and every sample up to it has the walker's ID (the old walk broke at the first other ID, skipped unmarked
  samples). So stage 3 fetches the fine bits of the T / 2 pixels ahead (one R8 texel each, both lines of the direction), takes
  the first fine sample's step, and only then fetches the IDs up to it, none depending on another. The pair trick of the old
  p_thick4 (the trailing sample's walk is the leading one's, one step short, after checking the leading sample's ID and fine
  bit) holds for any pixel, alike or not, so each direction walks twice for four samples.
- Pass P reads one bit a sample (uMark) instead: p_thick4 and the walks are gone, the alike pixels' path takes its marks from
  the same texel (mk = 0: sample 0 fogged, as before when em was 0), every sample's base is `p_fog(mark ? edge colour : colour)`,
  and a sample is slow only for the antialiasing (an opaque edge-flagged one with AA on). The second draw is now the AA's
  neighbour search and the outside-the-pixel neighbours' bases (a load and a mark bit each), fewer pixels than before.
- Host: gpu_uni_edge sets uSnap and uEdgeFine; GL binds stage 0's uEm to a texture other than its target, as Metal did.
- Tried first: stage 3 as the old walk (a fetch deciding the next): E 0.15 -> 0.26 against 0.22 for the committed form, the
  stage's draw alone (a variant writing 0) 0.15-0.16, so its walk costs ~0.07 now (~0.11 as a chain).

**Pictures:** 444/444 gpu_render captures identical to the base (37 captures, 1x and 3x, GL and Metal, AA off / 4 samples /
SMAA, picture and --hi); Edge=fine (BWPORT_GPU_EDGE=fine) and the Fast renderer the same before and after on 6 captures x 1x/3x x
both APIs x AA off / 4 samples; gpu_fastdiff over the 37 at 3x: 4-sample mean 8.20 %, AA off 11.23 %, every line the same;
vcore 74/74 (untouched); MTL_DEBUG_LAYER=1 clean (town and battle, 1x and 3x, the three AA settings, both Edge settings, Exact
and Fast). The in-game shots differ before/before as much as before/after (the NPCs move on a real clock), so the gpu_render
captures are the check.

**Timing, gpu_render** (`--time 120`, 3x, min ms, before and after interleaved, 3 rounds; load 2.5-6 at the start (a build
ran), 2.5 at the end; E+P is gpu_render's "P", E inside):

| | Metal E+P (E) back-to-back | Metal E+P, --gap 16 | GL E+P (wall) back-to-back | GL, --gap 16 |
| --- | --- | --- | --- | --- |
| town 4 samples | 0.77 (0.17-0.19) -> 0.47-0.49 (0.27-0.29) | 0.76-0.78 -> 0.48-0.49 | 0.85-0.88 -> 0.73-0.77 | 1.07-1.25 -> 0.84-0.96 |
| town AA off | 0.61-0.62 = | 0.33-0.37 -> 0.35-0.36 (one round in the other clock state) | 0.59-0.63 -> 0.57-0.62 | 0.68-0.90 = (spread) |
| battle 4 samples | 0.14 -> 0.13 | 0.14-0.17 -> 0.13-0.15 | 0.37-0.38 -> 0.34-0.38 | 0.49-0.61 = |
| battle AA off | 0.05 = | 0.06-0.08 = | 0.35 -> 0.33-0.35 | 0.42-0.45 = |

Town at 4 samples: Metal E+P -38 % (whole frame 3.81-3.83 -> 3.45-3.52), GL -13 %. Battle has edge marking off (no pass E), so
only the smaller second draw shows. An earlier pair of rounds (load 1.6-2.5, the lower clock state) read Metal town E+P 0.58 ->
0.38 (E 0.15 -> 0.22).

**In game** (town save, gpu 3x, 60 Hz, Metal, cfgscript passes=1, two runs each, before / after / before / after, load 2.1-3.1):
aa4-passes E+P 1.73-1.77 -> 1.08-1.15 (E 0.41-0.44 -> 0.65-0.70, P 1.32-1.33 -> 0.43-0.46); aa1-passes 0.77-0.79 -> 0.79-0.80
(unchanged); the plain aa4 row's renderer GPU 378-395 -> 340-345 ms/s (-11 %). At the passes rows' bottom clock stage 3 costs
~0.25 ms, P saves ~0.88.

**For the phone:** the 4-sample passes row's E+P should fall by about a third (P by two thirds, E up by half); AA off and SMAA
as before. Item 2 above (the AA search from pass E) is what is left of the slow draw.

## Pass E finds the antialiasing neighbours (2026-11-04): item 2 above, built, measured on the Mac (phone pending)

**What changed** (shaders/g06_edgemap.frag, g06_post.frag; both APIs; 4 samples with edge marking on, the rest untouched):
- Pass E's stage 0 already fetched the four 4-neighbours of every edge-flagged sample (e_fine, the fine mark, stopping at the
  first hit). With antialiasing on, for an opaque edge sample of a pixel that is not alike, it now reads all four and keeps
  pass P's antialiasing neighbour: the greatest depth above the sample's own, the first of equals in P_DIR order, neighbours off
  the grid skipped (p_sample's rule, the 24-bit depths of g_samp). Its index (3 bits a sample, 4 none) goes above the 4 fine
  bits: t_em[2] / em[2] is R16UI (Metal: a second pass E pipeline, `e0`, for stage 0's format); stages 2 and 3 take the low 4
  bits. gpu_uni_edge sets uDisp3d (stage 0 reads the AA bit).
- Pass P's slow draw reads the four indices from that texel (uAaNb, one fetch) instead of the 5-fetch search a sample; the
  neighbours outside the pixel still get their base there (a load and a mark bit). With edge marking off there is no pass E
  (battle), and the slow draw searches as before (a uniform branch). The P_FAST program is unchanged (no uAaNb).
- Stage 3 (the edge marks) is not widened: P's slow draw reads both texels at its own pixel, independent fetches.

**Ceiling first** (gpu_render town 3x, Metal, the low clock state, `--ablate 4` = no slow draw): before, E+P 0.37 against 0.33
without the slow draw, so after item 1 the whole slow draw was **0.04 ms** back-to-back (the plan's 0.1-0.15 was measured
before item 1 took the walks out); after, 0.35-0.36 against 0.34-0.35: what is left of it is ~0.01.

**Pictures:** picsab2 (scratchpad pnear/picsab.sh, a copy with its own folders) 444/444 identical, vcore 74/74 with "vcore empty md5
lines: 0", vcab 74/74; Edge=fine (BWPORT_GPU_EDGE=fine) and Fast with Edge=fine through gpu_render, and gpu_fastdiff's exact and fast
pictures: the 37 captures x 1x/3x x both APIs x AA off / 4 samples, 1776 files (picture and --hi) the same before and after; gpu_fastdiff at 3x:
4-sample mean 8.20 %, AA off 11.23 %, every line the same; MTL_DEBUG_LAYER=1 clean (48 runs: town and battle, 1x and 3x, AA off /
4 samples / SMAA, both Edge settings, Exact and Fast); the GL forms compile as GLSL ES 3.00 (glslang); the iOS device target builds.

**Timing, gpu_render** (`--time 120`, 3x, 4 samples, min ms, before / after / before / after in each round, 4 rounds; load 1.9-3.2;
E+P is gpu_render's "P", E inside; GL is wall time, no E apart):

| | Metal E+P (E) back-to-back | Metal E+P (E), --gap 16 | GL E+P back-to-back | GL, --gap 16 |
| --- | --- | --- | --- | --- |
| town | 0.37-0.38 (0.22) -> 0.36 (0.23) | 0.47-0.53 (0.27-0.31) -> 0.45-0.50 (0.29-0.32) | 0.71-0.74 -> 0.72-0.73 | 0.85-1.05 = (spread) |
| battle (no pass E) | 0.10 = | 0.13-0.14 = | 0.35-0.36 = | 0.40-0.50 = |

The rounds stayed in the low clock state but one (r4 of town back-to-back, both builds higher). Metal town: P alone
0.15-0.16 -> 0.13, E +0.01.

**In game** (town save, gpu 3x, 60 Hz, cfgscript passes=1, before / after / before / after, load 1.5-3.5):
- Metal: aa4-passes E+P 1.07-1.12 -> 1.01-1.07 (E 0.65-0.69 -> 0.67-0.72, **P 0.42-0.43 -> 0.34-0.35**); the controls unchanged:
  aa1-passes E+P 0.79-0.80 -> 0.78-0.79, R 4.49-4.53 -> 4.49-4.54. So at the passes rows' clock the search cost ~0.08 in P and
  costs ~0.03 in stage 0: **-0.05 ms** net.
- GL: the rows spread with their own controls (R 6.18-6.55 before, 6.36-6.89 after; aa1-passes E+P 1.18-1.64): no reading.

**Verdict:** exact; a small gain on Metal (-0.05 ms in game, -0.01-0.02 in gpu_render), neutral on GL. Pass P's slow draw is
now about 0.01 ms in gpu_render: nothing is left in it to take. For the phone: the 4-sample passes row's P should drop by
~20 % (0.08 of ~0.4 on the Mac) and E rise a little.

## Pass R at 4 samples on the M2 (2026-11-04): what is left by part, and what could take it

**How measured.** Throwaway variants of g00_raster.frag / g03_pixel.glsl (pictures wrong, time only; each one's result kept live
in a stored output: sF, since the 4-sample layers do not store sL, a variant whose result went only there measured the fixed part),
each a copy of the tree with its MSL regenerated, built in the session scratchpad's passr4/ (ed.py has every edit; tm.sh, lt.sh
the runs). gpu_render --time 200 at 3x, Metal, 4 samples, town (aa2pass/caps/t1401) and battle, 5 rounds alternating the
builds, a warm-up run before each group (the first build of a group was 0.4-0.6 ms high without it: the GPU clock), the min of
the rounds; under the shared timing lock, load 2-8 (four other agents measuring). R = (R+S) - S; S is 0.20 town, 0.09 battle.
Base this round: R town 1.20-1.23, battle 0.97 (the row-pair cuts' 1.20 / 0.98).

| variant (what is dropped) | town R+S | battle R+S | town R part | battle R part |
| --- | --- | --- | --- | --- |
| as it is | 1.40-1.43 | 1.06 | | |
| nolist: every bin's list empty (the fixed part: clear, rear plane, the 4 RG32UI layers' stores) | 0.25 | 0.15 | fixed 0.05 | 0.06 |
| rowonly: the walk and g_row's three fetches a row, then continue | 0.39 | 0.23 | walk + rows 0.14 | 0.08 |
| coveronly: + g_cover2 (bound, exact depth, edge step), no shade / texture / plot | 1.07 | 0.64 | cover 0.68 | 0.41 |
| noedge: the edge step always the inner-run constant | 1.11 | 0.83 | edge step 0.29-0.32 | 0.23 |
| nodepth: the depth by the float formula, no g_depth2 | 1.29-1.31 | 0.95-0.98 | exact depth 0.11 | 0.10 |
| nobound: no float depth bound before the exact depth | 1.53 | 1.07 | the bound saves 0.11 | 0 |
| noshade: no g_shade (texture and plot stay) | 1.32 | 1.05 | shade 0.08 | 0.01 |
| notex: no g_texture (shade and plot stay) | 1.19 | 0.78 | texture 0.24 | 0.28 |
| texconst: g_texture at texcoord 0..1 (the fetches' locality) | 1.34 | 1.06 | locality 0.09 | 0 |
| noshadetex: neither | 1.06 | 0.71 | shade + texture 0.35 | 0.35 |
| (remainder: g_plot4 and the selects) | | | ~0 | ~0.07 |
| tile8: bins of 8 DS columns (exact; g00_frame.h TILE_DS_COLS) | 1.38 | 1.06 | -0.05 | 0 |
| tile8b4: bins of 8 x 4 DS pixels (exact; BAND_DS_ROWS too) | 1.39 | 1.03 | -0.04 | -0.03 |

So of R's ~1.2 ms (town) with 4 samples: the cover step is 55 % (edge step 25 %, exact depth 9 %, the float bound and the
coverage test the rest), the texture 20 % (29 % in battle: large sprites), the walk and row fetches 12 %, the shade 7 %, the
fixed part 4 %, the plot ~0. The texture's cost is its per-pixel chain (polygon parameters, address, texel then palette fetch,
the mode blend), not cache misses: a constant texcoord takes 0.09 in town and nothing in battle. Compared with the A18 Pro's
breakdown of 2026-11-03 (fixed + walk 0.7 of 4.5, g_lin 0.75, windows 0.7, shade/texture/plot 1.3): the shares agree except
that the M2's fixed part is tiny (tile memory) and its g_lin is smaller after the row-pair cuts.

With `--gap 16` (the game's cadence, the GPU clock lower: 3 rounds, same lock and load): R+S town base 2.03, notex 1.70, noedge
1.59, nolist 0.34 (S 0.28); battle 1.47, 1.13, 1.21, 0.21 (S 0.13). The same shares at the lower clock: the texture 19 % / 25 %
of R, the edge step 25 % / 19 %, the fixed part 3-5 %.

**Tried this round, exact.** Smaller bins (tile8, tile8b4): 3-4 % off R in town, 0-3 % in battle; the walk + rows part is only
12 % and most of it is the polygons that really touch the pixel. Not committed: the CPU list build visits 2-4x the bins a polygon
(g00_frame.c gpu_bins_build; measure the worker's time before taking it), and the gain is within what one load change moves.

**Ranked build plan (expected gain of R on the M2; the phone's shares are larger for g_lin and the windows):**

1. *Pass S writes g_depth2's row constants* (ai, bi, dq, dr as integers; fa, fb's lo parts are t10.xy already): ceiling 0.10-0.11
   ms = 8-9 % of R (nodepth); on the A18 Pro g_lin was 17 % of R before the row-pair cuts, so more there. The record has one
   word free in every row layout (t9.w / t7.w only with AA; t5-t9 are reused without it), so it needs a 13th texel (g00_head.glsl
   g_span's 12 r + t, g02_setup.vert's outputs, g02_spans.c for the CPU builder, gpu_tests' t02_spans) or packing dq, dr into
   t4's unused bits. The values are the same integers computed once a row in S instead of once a pixel (exact by construction;
   the floats fa, fb stay in R: a - floor(a) is the same expression). Effort medium (five files, the self-check `--spans-check`
   covers the new fields), risk low. Checks: picsab2 444/444 GL and Metal, --spans-check 0 fields, gpu_tests.
2. *The texture decode cache* (park/texcache, built and exact; measured 4-6 % of R with nearest, 12-22 % bilinear): the texture
   is now 20-29 % of R, the largest single part after the edge step; the cache takes the texel -> palette dependent fetch out but
   keeps the address and blend math, hence only a quarter of the part. Effort: a rebase and the picture checks; ~250 lines.
   Below the bar alone (as parked); worth it together with 1 if the phone's share of the texture is as large (its breakdown put
   shade/texture/plot at 29 %).
3. *The edge step* (25 % of R, the AA windows most of it): no exact structural lever found this round. Per-row data from pass S
   cannot replace it: a window's coverage is per sample x (p16's floor(n / d) per pixel over a run up to the row's width), so the
   record would be unbounded; the one-sided split and the both-rows-at-once forms were slower on the M2 (above). What is left is
   micro: g_aa_side2's compare chain, and a phone-only check whether the four window fetches (free on the M2: dropping t9's fetch
   changed nothing) cost there.
4. *Deferred texturing of overdrawn opaque polygons* (unmeasured ceiling): g_shade + g_texture run once a pixel for every polygon
   that hits a sample, including ones a later opaque polygon covers. Only a count of textured-then-overwritten pixels per frame
   (a debug counter in the plot) says what it could save of the 0.35 ms; a translucent polygon drawn later needs the covered
   colour at its plot, so the deferral must texture on demand there. Effort high, risk high (the plot's order); measure first.
5. *Smaller bins* (tile8b4): 3-4 % of R, trivial, exact; only after the worker's list-build cost is measured with it.

Not worth building: the fixed part (4 % on the M2; the phone's 23 % is its layer stores, already RG32UI), the shade (7 %), the
bound (already saves 9 %), the walk (the lists are already per 16 x 8 bin).

## Pass R's depth quotient from pass S (2026-11-04): built, exact, neutral on the M2 (phone pending)

Item 1 of the plan above. **Not merged**: the code is d8279d533 on branch a private branch, kept for the phone
A/B (that commit against its parent); dropped if the phone is neutral too. **What changed** (commit d8279d533): g_depth2 (4 samples) divided dz = floor(R.z) - floor(L.z) by den_z
(t0.w) for each of a pixel's two sample rows, every pixel. The quotient dq is a record constant; pass S's kernel (the GLSL shared by
GL's transform feedback, the fragment form and Metal's compute) and the CPU builder (g02_spans.c gpu_span_pack) now write it,
the exact floor (s_fdiv / fdiv32, 0 when t0.w is outside 1 .. 65535), into **t9.w**, a word neither row layout used (without
antialiasing t8 and t9 are empty; with it t9 is k, dx, dy and nothing). So no 13th texel and no packing: the record stays 12
texels, the fragment form's three parts and the transform feedback's twelve outputs are unchanged. g_depth2 reads t9.w (one more
fetch a row, from the record's line it already reads t10 from) instead of g_fixq(dz, d, float(dz) / fd); dr = dz - dq d stays
(one multiply). --spans-check compares the new field ("dq") as an integer: 0 differ. gpu_tests checks two values.

**Exact:** picsab2 444/444 (GL and Metal, 1x and 3x, AA off / 4 samples / SMAA), vcore 74/74 (vcab), gpu_fastdiff 74/74 (the 37
captures, AA off and 4 samples), --spans-check 0 integer fields (the lo parts as before), MTL_DEBUG_LAYER=1 clean (town, battle;
1x, 3x; three AA modes), the iOS device target builds.

**gpu_render** (scratchpad rdepth/tm.sh: --time 200 at 3x, base and after alternated, a warm-up before each group, 4-6 rounds,
the min of the rounds; timing lock held, load 1.6-3.5). The M2's GPU sat in one of two clock states for a whole run (town 4 s:
R+S 1.42 or 2.03 for both builds alike), so the rounds are compared within a state:

| R+S ms (S) | base | after |
| --- | --- | --- |
| town 4 samples, high clock | 1.41-1.43 (0.20) | 1.41-1.43 (0.20) |
| town 4 samples, low clock | 2.01-2.05 (0.28) | 2.00-2.03 (0.28) |
| battle 4 samples, high / low | 1.07 / 1.52-1.55 (0.09 / 0.13) | 1.06 / 1.50-1.53 (0.09 / 0.13) |
| town / battle AA off, high | 0.66 / 0.50 (0.11 / 0.05) | 0.66 / 0.50 (0.11 / 0.06) |
| --gap 16, town / battle 4 samples | 2.01 / 1.56 (0.28 / 0.13) | 2.01 / 1.55 (0.28 / 0.14) |
| --gap 16, town / battle AA off | 0.94 / 0.71 (0.16 / 0.08) | 0.95 / 0.73 (0.17 / 0.08) |
| GL, town / battle 4 samples (4 rounds) | 2.85-3.21 / 2.37-2.41 (S 0.75-0.77) | 2.96-3.29 / 2.36-2.44 (S 0.75-0.82) |

So in gpu_render R is the same within 0.01-0.02 ms on Metal (S unchanged: one integer division a record), and GL's town rows
read 0.05-0.1 higher, S included (GL's spread is that large between rounds of one build). Why so little of the 0.11 ms the exact
depth costs (nodepth, measured again in the same runs: town R+S 1.30 / 1.84-1.88, battle 0.95 / 1.35-1.37): the fragment
programs compile relaxed (src/gpu_mtl.m), so float(dz) / fd was a multiply by the reciprocal that fq = nf / t0.w needs anyway,
plus a floor and g_fixq's multiply and two compares; the t9 fetch costs about that. A cost probe that keeps the picture
(`dup`: every row constant of g_depth2 computed a second time from a + 0, the 0 a uniform so it is not folded) costs +0.07
town, +0.07-0.09 battle at the low clock: the rest of the row constants (two floors, the fractional parts, dz, dr) are worth at
most about that, and moving them needs a 13th texel (two fetches more a pixel) for what dq alone did not save.

Not kept: the same read for AA off (g_cover_row's g_lin through a g_lin_dq taking dq; t9 fetched a pixel): 0.65 / 0.49 against
0.66 / 0.50, 0.93 / 0.71 against 0.94 / 0.71 with --gap 16: neutral, so the AA-off path keeps its division.

**In game** (tools/mac_run.sh --save town --noskip, the pinned bench clock and seed, cfgscript `renderer=gpu,resolution=3,
antialias=2,speed=1,framerate=60,frameskip=0,smooth=0,passes=1` then two bench rows; the macOS bwport at d8279d533 and with
port/gpu at its parent, otherwise the same build; timing lock held, load 1.9-3.5), six runs, base first in four, after first in two, 12
rows each: R (with S) base 4.38-4.55, mean 4.50; after 4.30-4.46, mean 4.40 (-2.2 %). But E+P, which the change does not touch,
moved as much: base 1.74-1.87, mean 1.78; after 1.70-1.78, mean 1.74 (-2.5 %), and R / (E+P) is 2.52 against 2.53. So the in-game
difference is the runs' GPU clock (each pass waited for at 60 Hz: the clock follows the whole frame), not R's own work; in the two
after-first runs the base rows overlap (4.38-4.55 against 4.40-4.46).

Verdict: exact and cheap (twelve lines); neutral on the M2 in gpu_render and in game. The phone's A/B decides whether it
stays: on the A18 Pro g_lin was 17 % of R before the row-pair cuts, so its ALU share may be larger there.

## Pass C in two programs (2026-11-04): the 2D layers' decisions once a DS pixel, measured on the Mac (phone pending)

**Where pass C's time went** (g07_compose.frag before; Metal, gpu_render `--compose gpu --time 300`, 3x, 4 samples, the command
buffer's GPU time, min of 3 interleaved rounds at the fast clock state, load 2.3-3.5; throwaway variants made by editing the
GLSL and regenerating the MSL, loaded at run time by a scratch build; pictures wrong): town 0.130 ms; the pass with a constant
top screen (the bottom screen's 1x fetch and the store) 0.045; only the 3D layer copied 0.072; only the stage fetched 0.051; both
fetched, no logic 0.091; the composition without the 3D layer 0.072; no second composition for AA edges 0.125; no BGs 0.106; no
master brightness, blending, OBJ or windows 0.128-0.131 each; no 8-bit expansion 0.127; the 3D fetch issued before the stage's
returns 0.130. Battle: 0.136; constant 0.045, 3D only 0.073, stage only 0.052, both 0.089, no 3D 0.078, no BGs 0.107. So of 0.13:
~0.045 the pass itself, ~0.03-0.04 the 3D layer's fetch, ~0.04 the composition logic, done N^2 = 9 times a DS pixel at 3x for a
result that depends on the N x pixel only through the 3D layer.

**What changed** (shaders/g07_compose.frag, src/g07_compose.c, src/gpu_mtl.m; exact): the file with `C_PLAN` defined is a second
program ("g07_compose.frag#1+C_PLAN"), drawn at 1x into a 256 x 192 RG32UI plan: per DS pixel the picture without the 3D layer
(master brightness applied) and what the 3D layer does where its alpha is not 0: nothing (not a candidate, third or lower, or
under a 2D layer that does not blend with it: no 3D fetch then), the top layer (the layer beneath, and 3D-blend or EVY
brightness up / down), or the second (the top layer and its alpha weights, or the constant result). The N x pass reads the plan
and the 3D layer only; the AA edges' "picture without the 3D" is the plan's. The plan is made once a pack (G07Pack's aserial:
engine A's stage number, kept when the 2D skip reuses A's half) and register set (OBJ on, priorities, BLDCNT, EVA/EVB/EVY,
backdrop, MASTER_BRIGHT): a frame's in-betweens and a still scene's frames reuse it. Shared GLSL: the same two programs on GL.
gpu_render `--time` with `--compose gpu` now also prints pass C's GPU ms (Metal) as a frame's picture (pack, upload, plan) and as an
in-between (the same pack), alone and after the 3D passes in one command buffer ("frame", as the game draws).

**Mac M2** (Metal, 3x, 4 samples, GPU min of 6 interleaved rounds each, load 2.1-3.9):

| case | alone, picture | alone, in-between | after the 3D, picture | after the 3D, in-between |
| --- | --- | --- | --- | --- |
| town, back to back | 0.132 -> 0.140 (+6 %) | 0.130 -> 0.097 (-25 %) | 1.914 -> 1.916 (median) | 1.941 -> 1.890 (median, -0.05) |
| battle, back to back | 0.135 -> 0.137 | 0.135 -> 0.093 (-31 %) | 1.328 -> 1.302 | 1.339 -> 1.288 (-0.05) |
| town, --gap 16 (4 rounds) | 0.183 -> 0.201 | 0.204 -> 0.140 (-31 %) | 2.671 -> 2.618 (min) | 2.703 -> 2.654 (min) |

The plan pass costs ~0.04 alone, and a variant writing a constant plan as much (0.038): it is a dependent pass's fixed cost, not
its work (its logic is a ninth of what it replaces). After the 3D passes in one command buffer it overlaps them: a frame whose 2D
changed costs about what it did, an in-between or a frame with a kept plan ~0.05 less.
**Tried, not kept:** the plan drawn ahead of the 3D passes (an API for the bridge to call first): frame 1.926-1.941 against
1.895-1.915 drawn after them; the N x pass's 3D fetch before the plan's: 0.097 either way.

**In game** (town save standing, gpu 3x, Metal, cfgscript passes=1 at 60, B / A / B / A, load 1.4-2.6): compose (4 samples)
0.36, 0.37 / 0.36, 0.36 -> 0.28, 0.28 / 0.27, 0.28 ms; AA off 0.38 / 0.38 -> 0.29 / 0.29; the control R 4.49-4.52 -> 4.47-4.56,
E+P 1.07-1.10 both. Standing, the 2D skip keeps the pack, so every frame reuses the plan; walking, each frame makes it (the
"after the 3D, picture" column). The plain rows' renderer GPU ms/s: 60 Hz 332, 330 -> 330, 345; Monitor 120 + Smooth (59
in-betweens/s) 860, 831 -> 800, 836: inside their spread.

**Checks**, before (0fd405cb5) against after: pass C's screens (`--compose gpu`) for the 37 captures x scale 1 and 3 x GL and Metal
x AA off, 4 samples, SMAA and `--3d-from vcore`: 592 of 592 identical, GL = Metal 296 of 296; with random BLDCNT / BLDALPHA /
BLDY / MASTER_BRIGHT / BG priorities / OBJ on (a scratch patch of gpu_render), 6 captures x 25 register sets x (3x 4 samples Metal,
1x GL, 3x SMAA Metal): 450 of 450 identical (229 different pictures); in game BWPORT_GPU_CHECK at 1x (walk, X menu, Smooth on):
"with an equal 3D layer: 0 px", as before; picsab2 444/444; vcore 74/74; gpu_fastdiff 74/74; MTL_DEBUG_LAYER=1 (assert mode)
clean; both programs compile as GLSL ES 3.00 and GL 3.30 (glslangValidator); the iOS device target builds.

**Expected on the phone:** compose was 0.55-0.69 ms a picture (passes rows). An in-between and a still scene's frame by ~25-30 %
(-0.15 to -0.2 ms); a frame whose 2D changed about the same, if the A18 Pro overlaps the plan pass with pass P as the M2 does.
The phone should verify: the passes rows standing and walking, and the Smooth row's renderer GPU ms/s, before and after.

## What is left of E and P with 4 samples (2026-11-04): ceilings by part, one cut built (stage 0's fetches), measured on the Mac (phone pending)

**Method.** gpu_render on the town and battle captures (scratchpad aa2pass), `--scale 3 --aa 2 --api metal --time 120`, the min of
3-5 interleaved rounds, back-to-back and `--gap 16`, under the timing lock, load 1.7-3.4 (other agents building). Every round of
this hour stayed in the M2's lower clock state (frame 2.7-2.9 ms back-to-back, 3.9-4.9 with the gap). Throwaway variants: GLSL
edits regenerated to MSL, and a host hack that skips chosen E stages after the second frame (`EPDROP`: the skipped stage's
target keeps the first frames' contents, which in gpu_render are this frame's, so the rest does its real work). E+P is gpu_render's
P, E inside; "P" below is the difference.

**Base** (ios eaecec045), town: E+P 0.36 (E 0.23, P 0.13) back-to-back; 0.46-0.48 (E 0.29-0.31) with the gap. Battle (edge marking
off, no pass E): P 0.10.

| part (town, back-to-back) | how | ms | with --gap 16 |
| --- | --- | --- | --- |
| stage 0 (fine marks, AA neighbours) | EPDROP=0 | 0.08 | |
| stages 2 + 1 (the DS map) | EPDROP=12 | 0.02-0.03 | 0-0.06 (noisy) |
| stage 3 (edge marks) | EPDROP=3 | 0.06 | 0.12 |
| stage 3 with its work stubbed (the pass alone) | writes 0 | 0 (= EPDROP=3) | 0.06 |
| stage 3's walks (e_thick) | fine bits only | 0.04 | |
| of which the 12 fine-bit fetches and their packing | no line work | 0.03 | |
| of which the ID checks | | 0.01 | |
| E with all four stages trivial | | 0.13 (each trivial pass ~0.03 when alone) | 0.15 |
| P's draw floor | writes 0 | 0.03 | 0.04 |
| P, every pixel alike (one sample, fog) | | 0.05 | 0.07 |
| P, no slow pixels (all resolved in the first draw, no AA) | | 0.08 | |
| P's slow-pixel machinery (the discard, the second draw, the blends) | base - the line above | 0.05 | |
| P's resolve divisions | resolve returns sample 0 | 0.01 | |
| fog | off | 0 | |

**Designs tried:**
1. *Stage 3 folded into pass P's first draw* (stage 3's code in P_FAST, the stage skipped): pictures identical, P 0.13 -> 0.25,
   E 0.23 -> 0.17: **+0.06 b2b, +0.08-0.1 with the gap**. The full-screen program carrying the walks costs twice the stage.
2. *Pass E's stage 0 writes pass P's stencil* (E_ST: an `[[stencil]]` output through a gpu_msl.py rename, the stencil private;
   P's first draw tests it and has no slow test or discard, P_STF): pictures identical. With the second draw skipped P's first
   draw falls 0.12 -> 0.06 (a program that can discard cost twice as much even on the pixels it kept), but the second draw then
   costs 0.05 instead of 0.01-0.02 and stage 0 +0.02: **E+P 0.36 -> 0.35-0.37, neutral**, with the gap too. A shader-written stencil
   value costs as much as a discard (a variant writing 1 everywhere: the same 0.12). The slow pixels cost ~0.05 however they are split.
3. *Stage 0 fetches each sample once* (built): e_fine read a sample and its four neighbours for each of the pixel's four samples,
   20 fetches; the four samples' 4-neighbours are the four themselves and eight outside the pixel, so 12. **E 0.23 -> 0.20**.
- Small ones, no change: stage 1 as a plus instead of 3 x 3 (the walks are along axes; fewer gated pixels): 0; e_thick's loops
  with a constant bound and findLSB: 0. Stage 3's gate passes ~6-9 % of the pixels at 3x (25k of 442k changed when every gated
  pixel was marked), so its cost is those pixels' 13 fetches.

**What is left, and what it would take:** stage 3's fine-bit fetches (0.03) could fall by about half with a row/column-packed map
made in stage 2's place (stage 3 reads 6 texels, the gate inside them, stage 1 gone): ceiling ~0.02-0.03 b2b, a new pass layout;
not built. P's slow machinery (0.05) is the slow pixels' own work in whichever draw does it. P's alike floor (0.05) and the draw
floors are bandwidth and launch. Nothing else in E or P is ≥ 0.05 ms back-to-back at 3x.

**The cut built** (b724bfeaa, shaders/g06_edgemap.frag; both APIs; 4 samples with edge marking on): stage 0 fetches the pixel's four
samples and their eight outside 4-neighbours once each (e_fine4), 12 fetches where e_fine per sample made 20; the antialiasing
neighbour is now found for every edge sample (pass P reads it only where it did before, so the picture is the same). k = 1's
e_fine keeps its form, without the unused arguments.

**Pictures:** picsab2 444/444 identical, plain and with GPU_COMPOSE=gpu, "vcore empty md5 lines: 0"; vcab 74/74; gpu_fastdiff's
exact and fast pictures (37 captures x 1x/3x x both APIs x AA off / 4 samples) and gpu_render with Edge=fine, Exact and Fast, the
same before and after (296 + 592 lines); gpu_fastdiff at 3x: 4-sample mean 8.20 %, AA off 11.23 %, all 148 lines the same;
MTL_DEBUG_LAYER=1 clean (48 runs: town and battle, 1x and 3x, AA off / 4 samples / SMAA, both Edge settings, Exact and Fast); the GL
forms compile as GLSL ES 3.00; the iOS device target builds.

**Timing, gpu_render** (3x, 4 samples, `--time 120`, min ms, base / after / base / after in each of 4 rounds, load 1.6-4.0, the lower
clock state throughout):

| | Metal E+P (E) back-to-back | Metal E+P (E), --gap 16 | GL E+P back-to-back | GL, --gap 16 |
| --- | --- | --- | --- | --- |
| town | 0.36 (0.23) -> 0.33 (0.20) | 0.46-0.49 (0.30-0.31) -> 0.43-0.44 (0.26-0.27) | 0.70-0.73 -> 0.64-0.68 | 0.85-1.25 -> 0.90-1.20 (spread) |
| battle (no pass E) | 0.10 = | 0.13-0.14 = | 0.34-0.36 = | 0.51-0.58 = (spread) |

**In game** (town save, gpu 3x, 60 Hz, Metal, cfgscript passes=1, before / after / before / after, load 1.9-3.8): aa4-passes E+P
1.02-1.05 -> 0.95-0.99 (**E 0.68-0.71 -> 0.62-0.64**, P 0.34-0.35 = 0.34); the controls unchanged: R 4.46-4.51 both, aa1-passes E+P
0.79 -> 0.78-0.81. The plain aa4 row's renderer GPU 329-344 -> 321-331 ms/s (within its spread).

**For the phone:** the 4-sample passes row's E should fall by ~10 % (0.06 of 0.7 on the Mac); P, AA off and battle as before.
