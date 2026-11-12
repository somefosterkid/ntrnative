# Pass R as a compute kernel on Metal (measured study, 2026-11-03)

## The answer

**No-go on a compute rewrite as a cost lever.** It is easy and exact, but only a few percent cheaper, because pass R's cost is
the arithmetic it does per covered sample, and a different dispatch shape does not change that arithmetic.

- **Exact, and cheap to build.** The fragment main ported to a kernel (thread per output pixel, 8 x 8 threadgroups, the same
  GLSL pieces through glslang and SPIRV-Cross, the same relaxed math mode) wrote **byte-identical sample layers** (all four words of
  every sample) on the first run: 222 of 222 runs (37 captures x 1x/2x/3x x AA off/4 samples). The tile function also matched
  on the captures at 6x and on 1460 fuzz-scene runs (see "Exactness").
- **Pass R alone** (M2, 3x, min ms): fragment 0.58 / 1.78 / 0.50 / 1.47 (town AA off / town 4 samples / battle AA off / battle
  4 samples); kernel 0.55 / 1.62 / 0.48 / 1.34; **tile function** (the same kernel run inside the render pass, writing the
  attachments through the imageblock) 0.55 / 1.67 / 0.47 / 1.37. That is 3-5% off with AA off and 6-10% off with 4 samples.
- **The plain kernel loses that gain in pass P.** A kernel can only write a texture that has `MTLTextureUsageShaderWrite`, and
  such a texture is stored without lossless compression. Pass E+P then read it 0.05-0.23 ms slower. `optimizeContentsForGPUAccess`
  does not bring the speed back. The whole frame (S, R, E, P in one command buffer) comes out level at 4 samples and 0.07-0.18 ms
  worse with AA off.
- **The tile function keeps the compression** (it writes the render pass's own attachments, stored as now). Whole frame: about
  **-0.1 ms with 4 samples (-4-5%)** and level with AA off (inside the noise).
- Every shape aimed at the work itself measured worse, not better: one list shared per tile in threadgroup memory, the samples'
  state in threadgroup memory, wider SIMD footprints, broadcasting the list entry across the SIMD group (table below). The
  counters and lane statistics say why. The bins already cull well. A SIMD group runs the cover code at 91-92% of its lanes and
  the shading code at 82-90%. The kernel is ALU-bound on integer work (ALU limiter 62-76%). With 4 samples the register peak
  (in g_cover2, docs/gpu-cost.md) caps the threadgroup at 640 threads in every form tried.
- **For the phone:** if the M2's proportions hold, a tile-function pass R would cut ~0.3 ms with 4 samples (from ~5.1) and
  ~0.1 ms with AA off (from ~2.9). It is not the substantial cut asked about.
- **Recommendation:** do not port. If ~0.3 ms with 4 samples on the phone is worth a day of work, the tile function is the form
  to take, Metal only. Time it on the phone first (below). The larger lever is fewer integer operations per covered sample,
  which helps the fragment version, GL and Metal alike ("What to measure next").

## What was built (scratch only; nothing committed but this file)

In the session scratchpad's `rcompute/`, there is a copy of port/gpu with:
- `shaders/g00_rcomp.comp`: g00_raster.frag's main as a kernel. `gl_GlobalInvocationID` replaces gl_FragCoord, and
  `imageStore` into a `uimage2DArray` (rgba32ui, one layer per sample) replaces the outputs. Every function it calls is the
  pieces' GLSL as it is. Variants are `#ifdef`s: RC_TILE (list sharing), RC_SHST (state in threadgroup memory), RC_STAT
  (counters), RC_UNI (subgroup broadcast), and RC_TW/RC_TH (threadgroup shape).
- `tools/gpu_msl.py`: a `rcomp` stage (glslang `-S comp --target-env vulkan1.1`, SPIRV-Cross `--msl-version 20300`) for the
  programs `g00_rcomp.kern#K+DEFS`. The name avoids ".comp", so gpu_mtl.m compiles them **relaxed**, as the fragment programs
  are. Safe math would change texture coordinates (M1).
- `mktile.py`: rewrites the kernel's MSL into a tile function. It drops the texture argument, adds
  `imageblock<RTOut, imageblock_layout_implicit>` (one `uint4 [[color(i)]]` per sample) and `thread_position_in_threadgroup`, and
  replaces the stores with one `ib.write`. That is 25 lines of text substitution.
- `src/gpu_mtl.m`: GPU_RCOMP=variant dispatches the kernel (the six tables bound by reflected name, the output with
  ShaderWrite usage). GPU_RTILE=path runs the tile function: an `MTLTileRenderPipelineDescriptor` (k x k RGBA32UI,
  `threadgroupSizeMatchesTileSize`), the render pass's `tileWidth`/`tileHeight` 16, `dispatchThreadsPerTile`, and bindings
  from `refl.tileBindings`. GPU_RDUMP writes the raw sample layers. GPU_WHOLE times a frame as one command buffer.
- Scripts: `check.sh` (raw layers, fragment against variant, cmp), `time.sh` (pass R alone: GPU_RREP=20 passes a timed
  frame, the alternated variants' min, as docs/gpu-cost.md's probe), `ptime.sh` (P), `wtime.sh` (whole frame), `stat.sh`,
  `traces.sh` (`xctrace` Metal GPU Counters).

## The data

M2, `gpu_render --api metal --scale 3` on the town and battle captures (aa2pass/), Resolution 3. The table is "pass R alone",
min of 3 alternated rounds of 40 frames x 20 passes. The load was 3.4-5 through all of it, with other agents' runs beside it;
read the differences, not the absolute figures.

| pass R alone, ms | town AA off | town 4 samples | battle AA off | battle 4 samples |
| --- | --- | --- | --- | --- |
| fragment (today) | 0.58 | 1.77-1.79 | 0.49-0.50 | 1.47 |
| kernel, 8 x 8 | 0.55 | 1.61-1.62 | 0.47-0.48 | 1.32-1.34 |
| tile function, 16 x 16 | 0.55-0.56 | 1.66-1.68 | 0.47 | 1.37 |

**Shapes** (each one alternated run against the fragment version and the 8 x 8 kernel of the same run; ms, same columns):

| variant | town off | town 4s | battle off | battle 4s | vs 8 x 8 kernel |
| --- | --- | --- | --- | --- | --- |
| threadgroup 16x4 / 16x2 / 32x2 / 4x8 / 8x4 | .58/.59/.71/.56/.56 | 1.63/1.74/2.00/1.61/1.62 | .48/.49/.54/.49/.49 | 1.30/1.36/1.50/1.34/1.32 | 8 x 8 best; wider is worse (more rows a SIMD) |
| RC_TILE: the tile's list chunk (64 entries) and a mask of its rows the run meets, in threadgroup memory; 8x8 / 8x4 / 16x8 | .66/.64/.71 | 1.83/1.76/2.06 | .56/.54/.58 | 1.60/1.54/1.73 | 10-25% worse: the barriers, and the visits it skips were cheap |
| RC_SHST: C, D, F, I, L of the samples in threadgroup memory, loaded where used; 8x8 / 8x4 / 16x8 | .79/.95/.71 | 1.91/1.69/1.64 | .62/.68/.56 | 1.48/1.36/1.32 | worse; still 640 threads max with 4 samples: the state is not the register peak |
| RC_UNI: `subgroupBroadcastFirst` of the bin and the list entry | .61 | 2.24 | .54 | 1.72 | much worse with 4 samples (load 4.5-5) |

**Why** (`stat.sh`: atomics per lane and per SIMD group via `subgroupElect`, town / battle, the same at both AA settings):
per pixel 7.3 / 3.9 list visits, 3.75 / 2.2 with the row covered (g_cover2 runs), 1.6 / 1.75 shaded. SIMD utilisation:
visits 100% (one bin a group), cover 91% / 92%, shade 82% / 90%. So reordering or compacting lanes can win at most a tenth of
the shading. The visits a tile cull removes cost a list fetch and three record fetches, not ALU. Counters (town, `traces.sh`):
fragment AA off ALU limiter 63%, occupancy 25%; kernel AA off ALU limiter 76%, occupancy 29%; kernel 4 samples ALU limiter 62%,
occupancy 19.5%. Texture, buffer, threadgroup and cache limiters are all below 20%. The pipelines' `maxTotalThreadsPerThreadgroup`
is 1024 with AA off and 640 with 4 samples, for the kernel, RC_TILE (576), RC_SHST and the tile function alike.

**Pass P after a shader-writable output** (`ptime.sh`, one pass R a frame, min of 4 rounds of 100, load 4-7; "fragment+SW" is
the fragment version with ShaderWrite added to the sample texture's usage):

| P, ms | town off | town 4s | battle off | battle 4s |
| --- | --- | --- | --- | --- |
| fragment | 0.25-0.29 | 0.61-0.79 | 0.05-0.06 | 0.11-0.15 |
| fragment+SW | 0.42-0.43 | 0.84-0.87 | 0.12 | 0.17 |
| kernel | 0.41-0.64 | 0.77-0.85 | 0.11-0.12 | 0.16-0.17 |
| kernel + `optimizeContentsForGPUAccess` blit | 0.55 | 0.83 | 0.12 | 0.17 |
| tile function | 0.33-0.34 | 0.61-0.66 | 0.06 | 0.13-0.14 |

The blit also adds 0.03-0.05 to R. One pass a frame with a commit per pass (`ptime.sh`'s R column) is too noisy to read pass R
from: the GPU's clock ramps differently per run. The whole-frame figure below is the one to trust.

**Whole frame** (`wtime.sh`, GPU_WHOLE: S, R, E, P in one command buffer as the game encodes it, timing mode off; min per
round over 100 frames, the 5 rounds' range, load 3.7-4.0):

| frame GPU ms | town off | town 4s | battle off | battle 4s |
| --- | --- | --- | --- | --- |
| fragment | 1.00-1.46 | 2.64-2.66 | 0.66-0.72 | 1.71-1.76 |
| kernel | 1.18-1.30 | 2.62-2.63 | 0.73-0.78 | 1.64-1.84 |
| tile function | 1.07-1.24 | 2.51-2.55 | 0.65-0.78 | 1.62-1.68 |

## Exactness

`check.sh`: the fragment version and the variant run on the same capture, and every sample layer is read back raw (all four
words: depth, IDs/flags/coverage with pass P's bits 30/31, colour, and the debug last polygon), then compared with `cmp`.
- Kernel 8 x 8: the 37 captures (aa2pass, caps, aas/vcs, aas/vcsb) x 1x, 2x, 3x x AA off and 4 samples: **222 of 222
  identical**. Every other variant timed above was identical on town and battle at 3x (and 1x for RC_TILE).
- Tile function 16 x 16: the same 222 of 222, the 37 captures at 6x with 4 samples 37 of 37, and the 730 p13-p19 fuzz scenes
  (p1/fuzz) at 2x, AA off and 4 samples: 1460 of 1460 identical.
- Not covered: the Filter=bilinear programs (`#1f`/`#2f`), which would be two more programs from the same main, and Apple's GL
  (no compute in GL 4.1).

The fragment version's exactness logic does not have to be carried over by hand. The kernel calls the same GLSL functions,
compiled by the same tools in the same math mode, and only the main's first three lines and its stores differ. The remaining
risk is a future Metal compiler contracting a float expression differently in a kernel than in a fragment function. The raw
layer check over the captures and the fuzz scenes catches that, so it would join gpu_same.sh in a real port.

## What a full port would take

- **Tile function, Metal only (the form worth having):** about a day. g00_raster.frag gets an `#ifdef R_TILE` head and tail
  (the prototype's diff is 15 lines; no shared function with array out-parameters, which crash Apple's GL compiler).
  gpu_msl.py gets the kernel stage and mktile's rewrite for four programs (k = 1, 2, and the filter's 1f, 2f). gpu_mtl.m gets
  about 60 lines (the tile pipeline, the encode, tile bindings). The raw-layer comparison moves into gpu_render or gpu_same.sh.
  Tile functions need Apple GPU family 4 (A11) or later, so every Apple-silicon Mac and every iPhone iOS 18 runs on. The
  fragment version stays for GL and as the reference. The imageblock holds 16 B a sample: 64 B a pixel with 4 samples, so tiles
  of 16 x 16. 32 x 32 is refused with 4 samples (1024 threads > 640) and works with AA off at no gain (0.56 against 0.55-0.58).
- **Plain kernel:** the same work without mktile, but it is a net loss in the frame (P's compression), unless passes E and P
  became kernels reading a buffer too. That is a larger rewrite of three passes for a gain nobody has measured.
- **GL / GLES:** GL 4.3 (Windows, Linux) and GLES 3.1 (Android) could run the same .comp main. GLES 3.0 and Apple's GL 4.1
  cannot. There is no tile-function equivalent, and image stores on mobile GPUs generally lose framebuffer compression too (AFBC,
  UBWC), so the plain kernel's loss in P is likely there as well. No reason to do it for GL.

## What to measure next

1. **The tile function on the phone.** A throwaway IPA with the prototype's GPU_RTILE path behind an ini key, then the perf
   test's passes rows at aa1 and aa4. Go if R drops by 0.3 ms or more with 4 samples.
2. **Integer work per covered sample (both APIs).** This is where the time goes: g_cover2's depth step, g_lin, the texture
   decode. Much of g_lin is constant along a row: the ends' floor and fraction parts, dz, and dz's division by den (dq, dr).
   Today every covered sample recomputes it. Pass S could compute it once per record and widen the record (at most a texel or
   two). Count the operations saved per covered sample first (3.75 / 2.2 covered visits a pixel, x 4 samples), then time one
   variant.
3. Only if the phone shows a different picture (occupancy well below the M2's 19-29%), retry RC_SHST there. On the M2 it gained
   nothing, because the register peak is elsewhere.
