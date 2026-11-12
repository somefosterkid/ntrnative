# The GPU renderer's API on Apple devices: ANGLE, native Metal, or Vulkan through MoltenVK (design study, 2026-11-03)

A decision document for the maintainer: which graphics API port/gpu should use on iOS and macOS, and whether iOS gets its own
Metal path or a shared Vulkan one translated by MoltenVK. Nothing here is implemented. Every number is marked **measured**
(this Mac or a test phone logs), **published** (a public document, named), or **estimate** (mine, with the reasoning).
Probes were throwaway and are not committed; what they did is described inline.

## Summary

| option | work (agent pieces) | iPhone CPU, per present | iPhone GPU, per frame | risks | verdict |
| --- | --- | --- | --- | --- | --- |
| **A. ANGLE** (GLES 3.0 on Metal) instead of Apple's OpenGLES.framework | 2 pieces: build ANGLE for iOS + Simulator, the SDL/EGL context; nothing in port/gpu | -0.5..-0.9 ms (uploads staged on the GPU instead of twiddled on the CPU); the GL thread's driver waits stay | about the same (same GLSL through another translator, +-20% unknown); the present quad and the per-pass drains stay | a ~10 MB dependency built with gn; its own bugs; still GLES 3.0: no compute, 4 draw buffers; pass S stays transform feedback | **not worth it on its own**; keep as the fallback if Apple removes OpenGLES.framework |
| **B. Native Metal** back end for iOS and macOS | 6 pieces, ~2,000 lines: Metal core + MSL generated from the GLSL at build time + compute pass S + the bridge's GL-neutral half + libntr's present/ImGui on Metal + pacing | -1.0..-1.8 ms (no twiddle, no blit encoders, no finishResource / transform-feedback flushes; ~0.1 ms of encoding) | -15..-25% at 4 samples, -10..-20% AA off/SMAA: pass S 2-3x cheaper as compute, the 4-sample state's store 0.59 -> 0.43 ms (M2, measured), the present quad folded into pass C; pass R's ALU-bound loop unchanged | the first second back end to maintain (the shaders stay one source); Metal runtime compile at first start (~0.5 s a program, cached by iOS); correctness of the generated MSL checked against the GLES pictures | **recommended**, in the order below |
| **C. Vulkan everywhere**, Apple through MoltenVK | 7-8 pieces, ~3,000 lines: a Vulkan core (2-3x Metal's boilerplate), the same generated shaders, compute pass S, MoltenVK as a static library, SDL Vulkan surface, ImGui's Vulkan backend; lavapipe for CPU tests | as B plus MoltenVK's own layer (estimate 0.05-0.3 ms a frame for our ~10 commands) | as B for the shaders (MoltenVK runs the same SPIRV-Cross MSL); no transform feedback (compute pass S is forced, fine); no tile shaders/imageblocks; present only through MoltenVK's swapchain | three back ends in practice (GLES stays for llvmpipe and old Android); +8-12 MB in the IPA; MoltenVK's bugs are ours; the Mac is a full test host for it, which is its one advantage over GLES-on-iOS | **not for Apple**; revisit Vulkan for Android only when a phone exists and GLES 3.1 compute is not enough |

The honest frame for all three: the phone's frame is dominated by pass R's polygon loop (4.8-6.3 ms of a measured 11-12 ms
with everything drained), which is ALU-bound shader work that no API changes. An API move buys the uploads, the waits, the
fixed costs and the present path, about a fifth of the frame; the road to 120 Hz with antialiasing is still the shader work
in docs/gpu-cost.md. What the API move does buy that matters for heat: the CPU side (the twiddle, the blit encoders and the
waits are busy-CPU, 0.5-0.6 cores on the phone today), compute for pass S (the phone's transform feedback is broken, see
below), and control of the present.

## What exists today (the layers an option touches)

- **port/gpu** (GLES 3.0 / GL 3.3): `gpu_gl.h` is a table of 92 GL entry points loaded by name (`X(PFNGL...)`), `gpu_gl.c`
  compiles the programs (API header + `g00_head.glsl` + every `gNN_*.glsl` by name + the pass's main), the data textures
  (1024-wide RGBA32UI/F, the rings of three), the pixel-unpack ring. The passes: S (`g02_setup.vert`, a transform-feedback
  draw of one point per record, copied into the span texture by one buffer-sourced TexSubImage2D), R (`g00_raster.frag`,
  compiled per k: 1 or 4 RGBA32UI targets), E (`g06_edgemap.frag`), P (`g06_post.frag`), SMAA (`g08_*`), C (`g07_compose.frag`).
  1,900 lines of GLSL, integer code: `texelFetch`, bit operations, `uintBitsToFloat`, dynamic loops, `flat out` varyings;
  no extension. The CPU half (`g02_spans.c` records, `g00_frame.c` prep, `g04_texture.c` texture build, `g07_compose.c`
  pack) is plain C and does not know the API except through `gpu_gl` calls.
- **port/bridge/gpu_bridge.c** (1,050 lines): the worker thread runs the CPU half (Gather, Stage1, the in-betweens'
  geometry); Stage2 runs the GL half on libntr's render thread: `gpu_render_prepared`, `g07_compose_packed`, and hands
  libntr a GL texture id (`sPic.tex`); `SaveGL/RestoreGL` keep libntr's state; `MakeTex/Upload1x` upload vcore's picture;
  `InterpTexture` serves the cap loop's in-between slots; `CheckFrame` reads pass C back.
- **libntr's present** (sim_main.cpp, libntr-gpu.patch): `SIM_DrawFrameTexture` draws the renderer's texture with a GL
  program as two screen quads into SDL's default framebuffer, then ImGui (`imgui_impl_opengl3`, ES 3.0 on iOS) draws the
  touch controls (ImGui's background draw list) and the overlay; `SDL_GL_SwapWindow`. iOS: SDL's UIKit view with a
  CAEAGLLayer, Apple's OpenGLES.framework ("OpenGL ES 3.0 Metal - 104": itself a layer on Metal since the A12/iOS 12
  era), no swap interval, the display link model of libntr-ios-lag/panel.patch. Android: the same `#if SIM_GLES` path
  on the device's GLES driver. Desktop: GL 4.1 (macOS) / 4.2.
- **Tests**: `--spans-check` (pass S's records against the CPU builder's, integer fields exact), gpu_render pictures
  byte-identical across changes (`tools/gpu_same.sh`), `vcore_check pNN --oracle gpu=...`, `gpu_compare.py`, the bridge's
  self-check (3 frames a run), all on Mesa llvmpipe headless (Linux CI) or Apple's GL 4.1 here.

## Facts from the phone (measured, bwport-20261103-074533.log and the 11-02 logs)

iPhone17,2 (A18 Pro, 6-core GPU, 120 Hz ProMotion): "OpenGL ES 3.0 Metal - 104", 4 draw buffers, 64 transform-feedback
components. Resolution 3, Smooth, 60 Hz load, means over ~230 frames with a glFinish after every pass (so each pass also
pays a queue drain, and the GPU clock is whatever DVFS chose at ~30% busy; these are upper bounds, not a 120 Hz frame):

| ms a frame | AA off | 4 samples | SMAA |
| --- | --- | --- | --- |
| CPU half: geometry / span records / 2D | 0.47 / 2.73 / 1.33 | 0.38 / 4.17 / 0.87 | 0.46 / 2.71 / 1.32 |
| GL: uploads / R / E+P / SMAA / compose | 1.18 / 4.79 / 1.55 / - / 1.43 | 1.31 / 6.25 / 2.71 / - / 1.15 | 1.02 / 4.77 / 1.58 / 0.67 / 1.49 |
| GL sum | 8.95 | 11.4 | 9.5 |

The records are still the CPU's on the phone: **pass S fails there** (records 0-4091 right, zeros from 785,664 bytes =
4092 x 192: the transform feedback stops writing at a 4092-vertex boundary; the Mac, llvmpipe and the Simulator are right),
so the self-check turns it off and the phone pays 2.7-4.2 ms of CPU records plus ~1.0 ms of uploads a present. Another
agent is on it. Whatever that finds, a compute pass S (B or C) does not go through Apple's transform-feedback path at all.
Sustained load throttles the phone (4.0 -> 2.0 GHz in ~70 s of fast forward, docs/perf.md); the bench rows read 0.48-0.61
cores at 60 Hz and thermal "fair" after a few minutes of the gpu rows.

## Probes run for this study (M2, load noted; throwaway, in the session scratchpad backends/probe/)

1. **GLSL -> SPIR-V -> MSL -> Metal's compiler** (glslang 16.6.0 prebuilt, SPIRV-Cross vulkan-sdk-1.4.363.0 built from
   source, both in the scratch folder; Metal compiled at run time by `MTLDevice newLibraryWithSource`, since Xcode's Metal
   toolchain is not installed here). The six programs as `gpu_gl.c` concatenates them (R at k = 1 and 2, P at k = 2, C,
   SMAA weights, S's vertex shader) with three mechanical changes to the source text: `#version 310 es` (glslang's SPIR-V
   needs 3.10; our code uses nothing past 3.00), the loose uniforms gathered into one `layout(std140) uniform U { ... }`
   block (Vulkan GLSL has no loose uniforms; a 25-line script did it), `gl_VertexID` -> `gl_VertexIndex`. **All six
   translate and compile** (MSL 270-1,860 lines each, 7,050 in all; Metal's compile 0.33-0.68 s a program cold, 1 ms
   cached). No 64-bit integers appear; the four RGBA32UI outputs become `[[color(0..3)]]`; the `texelFetch`es become
   `texture2d<uint>.read`; pass S's `flat out` varyings become a `device` buffer written by the vertex function
   (`--msl-capture-output`, Metal allows vertex functions to write buffers). With OpenGL semantics instead (`-G`, loose
   uniforms kept) the MSL did not compile (each uniform its own buffer slot, 38 > 31, and address-space clashes): the one
   source change a Metal or Vulkan path needs is the uniform block. Not verified: that the generated MSL draws the same
   picture; that is the first check of piece M1 below.
2. **Native Metal timings** (metal_probe.m, GPU time from the command buffer's timestamps, min of 200; load 94 for the
   first run, 4.8 for the second):
   - a pass-S-sized compute kernel: one thread per record (34,153, town 3x with 4 samples), a 9-step binary search of 324
     polygons, 40 or 150 iterations of integer row work with divisions, 12 uint4 written per record (6.56 MB): **0.24 /
     0.39 ms**; the same writing only: 0.05-0.10 ms; 50 dispatches back to back 0.24 ms each. The GL path's pass S on this
     Mac is 0.69-0.78 ms with 4 samples (0.46-0.51 AA off), of which ~0.3 is a draw's fixed cost with its glFinish and
     ~0.2 the buffer-to-texture copy (docs/gpu-cost.md, P1). So compute S is 2-3x cheaper here, and the copy and
     the `glBeginTransformFeedback` flush (the GL thread's main wait after the rings) are gone.
   - a full-screen fragment pass at 768 x 1152 writing four RGBA32UI attachments (pass R's 4-sample state, 64 bytes a
     pixel), stored: **0.43 ms**; the same attachments memoryless + dontCare: 0.13 ms; one RGBA32UI stored: 0.13 ms. The
     GL path's measured fixed costs were 0.59 (four layers) and 0.28 (one). The store of 14 MB is ~0.3 ms of the 0.43:
     the RG32UI packing (62 bits a sample, docs/gpu.md "Antialiasing") would halve it.
   - a trivial command buffer, encode + commit: 0.10 ms of CPU at load 94 (estimate 0.03-0.05 unloaded).
3. Vulkan / MoltenVK: no SDK on this Mac; not run. MoltenVK's MSL would be SPIRV-Cross's, i.e. probe 1's output, so the
   shader side is covered; its CPU layer is reasoned about below from its published design.

## A. ANGLE in place of OpenGLES.framework

**What changes.** Nothing in port/gpu, the bridge or the shaders: ANGLE's EGL + GLES 3.0 are loaded by name as today.
One libntr patch: the iOS (and macOS, if wanted) SDL window asks for an EGL context from ANGLE (SDL2 supports ANGLE/EGL on
iOS and macOS when built with it, or the app creates the EGL surface on the CAMetalLayer itself); ImGui's ES 3.0 backend
unchanged. The build: ANGLE from source with gn/ninja (a Chromium-style checkout: depot_tools, ~1 GB of sources, ~30-60 min
on this Mac for arm64 iOS + Simulator + macOS), as static libraries or an XCFramework into ~/bw-cache; `tools/ios_deps.sh`
grows one step. Shared with Android/Windows/Linux: everything (they would not use ANGLE).

**Work.** Two pieces: (1) build ANGLE for iphoneos, iphonesimulator and macOS, cache it, link it (1 agent, the risk is the
gn build on a sandboxed Mac); (2) the context patch, the Simulator and device runs, the bench rows (1 agent). Verified by
the existing checks: the pictures byte-identical to Apple's GLES (the Simulator, gpu_render on the Mac with ANGLE's
macOS build, the self-check on the phone), the bench and passes rows.

**Performance (estimate from the Mac's measurements and ANGLE's published design).** ANGLE's Metal backend stages client
uploads in buffers and blits them on the GPU (ANGLE's docs: the Metal backend's buffer and texture upload paths), so the
CPU twiddle (0.17 cores on the Mac's sample; part of the phone's 1.0-1.3 ms of upload calls) becomes GPU blit time: CPU
-0.5..-0.9 ms a present, GPU +0.1-0.2. Its command buffers flush on fences, queries and swaps much as Apple's GL does; the
waits for a texture still in use become its own fence waits; no gain in kind. The shaders go through ANGLE's GLSL -> MSL
translator (direct, since its 2021 rewrite) instead of Apple's GLSL front end: the same AIR back end afterwards, so +-20%
is the honest spread and the sign is unknown. The present stays a GL quad into ANGLE's drawable plus ImGui. Power and
heat: the CPU's share drops a little; the GPU's does not. Capability: GLES 3.0 only on Metal (ANGLE's feature matrix has
no ES 3.1 / compute on the Metal backend as of its public README; hedge: check the current matrix), so pass S stays a
transform-feedback draw (ANGLE implements it with a vertex function writing buffers, so the phone's 4092-record bug is
unlikely to carry over, but that is a guess until run). 4 draw buffers on ES 3.0 (ANGLE exposes 8 if asked for ES 3.0's
MAX_DRAW_BUFFERS on Metal; our code asks for 4). Debugging: Xcode's Metal frame capture works under ANGLE (it is a Metal
app) but shows ANGLE's command stream, not ours. Longevity: ANGLE is what Safari and Chrome use for WebGL on Apple
platforms (published: WebKit moved WebGL to ANGLE's Metal backend in Safari 15, 2021; Chrome on macOS in 2021-22), so it is
maintained; OpenGLES.framework has been deprecated since iOS 12 (Apple, WWDC 2018) and still ships in the iOS 27.1 SDK.

**Distribution.** +8-12 MB of arm64 code in the IPA (estimate from Chrome's libGLESv2 + libEGL; built static, no embedded
framework to sign). SideStore signs whatever is in the bundle; static is simplest.

**Verdict.** A dependency the size of the game for a ~1 ms CPU gain and no GPU gain; it fixes nothing structural. Keep it
written down as the fallback if an iOS release drops OpenGLES.framework before B exists.

## B. A native Metal back end

**What changes.**
- *port/gpu*: a second core beside `gpu_gl.c`: `gpu_mtl.m` (device, queue, pipelines, the frame's command buffer, the
  rings as a triple-buffered `MTLBuffer` set, `MTLSharedEvent` or completion handlers instead of `glFinish`), ~600-800
  lines of Objective-C. The data textures become buffers (no 1024-wide packing: the records, polygon, band and list tables
  are `device` arrays; the texture slots and palettes stay 2D textures or become buffers). The passes: S a compute kernel
  (one thread per record, writing the span buffer directly: no copy), R, E, P, SMAA fragment passes into `MTLTexture`s
  (R's four RGBA32UI slices as four colour attachments of one render pass, `storeAction` store, load dontCare), C drawing
  straight into the CAMetalLayer's drawable (the present quad disappears: pass C already produces the final picture at N).
  Public `gpu.h` grows an API field (`GPU_API_METAL`) and a "present texture / drawable" hand-over; the CPU half is
  untouched.
- *shaders*: **generated, not hand-written.** `tools/embed_glsl.py` gains a build step: for each program, the concatenation
  as today + the uniform block, glslang -> SPIR-V -> SPIRV-Cross -> MSL, embedded as C strings (the Metal toolchain is not
  needed: the device compiles MSL at first start, ~0.5 s a program, then iOS's shader cache). The GLSL stays the one source
  for every platform; the pieces' rule "no GLSL built-ins beyond texelFetch, bit operations and arithmetic" is what makes
  probe 1 pass. Hand-written MSL only for what Metal gives and GLSL cannot: pass S as a compute kernel (the body is the
  same code; the entry is `kernel`), and pass C's drawable target. The uniform block is a shared-code change: `g00_head.glsl`
  and the passes' own `uniform` lines move into one block (GLES 3.0 has uniform blocks; `gpu_gl.c` binds it; one UBO upload
  a pass instead of ~15 `glUniform` calls, a small gain on every platform, pictures unchanged).
- *the bridge*: Stage2's GL half behind a small vtable (`render`, `compose`, `picture`, `upload1x`, `save/restore`): the
  GL one is today's code moved, the Metal one new. The worker, Gather, Stage1, the in-betweens and the checks are
  untouched. `InterpTexture` hands libntr a `MTLTexture` (or the GPU half presents itself, below).
- *libntr's present, ImGui, the SDL window*: one patch `libntr-ios-metal.patch` under a new `SIM_METAL`: SDL creates a
  Metal view (`SDL_Metal_CreateView` / `SDL_Metal_GetLayer`, SDL 2.0.12+), the frame quad is a Metal render pass (or pass C
  draws into the drawable and the present is `presentDrawable`), ImGui uses `imgui_impl_metal.mm` (shipped with ImGui;
  the touch controls are draw-list data and need nothing), `SDL_GL_SwapWindow` becomes the command buffer's commit +
  present. The display-link pacing (libntr-ios-lag/panel/ffpace) keeps its model; the GPU-done fence becomes the command
  buffer's completion. macOS gets the same path (Metal is native here), which is what makes it testable: `tools/mac_run.sh`
  and gpu_render with `--api metal` run the phone's back end on this Mac, unlike GLES-on-iOS which only the Simulator's
  software ES approximates.
- *Shared with Android, Windows, Linux*: the CPU half, the GLSL, the bridge's worker, the checks; their GL path is the
  moved code plus the uniform block.

**Work, in order (each an agent in its worktree).**

| piece | what | size | verified by |
| --- | --- | --- | --- |
| M0 de-risk | pass S as a Metal compute kernel, standalone: the setup table from a capture, the records read back and diffed against the CPU builder's (`--spans-check`'s diff), timed on the M2 | 1 agent, ~400 lines | 0 integer fields differ on the 37 captures and the 730 scenes; time against GL's S |
| M1 | the generated-MSL build step + the uniform block in the GLSL + `gpu_mtl.m` running passes R, E, P, C, SMAA offscreen; gpu_render `--api metal` | 1-2 agents, ~900 lines + build | gpu_render pictures byte-identical to `--api gl` on the 37 captures at 1x-4x, AA off / 4 samples / SMAA; `vcore_check --oracle gpu=` at 1x; gpu_tests |
| M2 | M0's kernel inside M1 (the span buffer, no copy), the rings as buffers, timing hooks (`MTLCounterSampleBuffer` at stage boundaries, or command-buffer timestamps) | 1 agent, ~300 lines | `--spans-check` through the Metal core; pictures unchanged; the P1 timing table redone with `--api metal` |
| M3 | the bridge vtable, the Metal half in the game on macOS (hidden window, the present still GL: the Metal picture reaches GL through an IOSurface-backed texture, `CVMetalTextureCache` / `CVOpenGLTextureCache`, a known pattern) | 1 agent, ~400 lines | mac_run.sh town/battle shots identical to the GL run; the renderer switch test; the self-check |
| M4 | libntr on Metal: the SDL Metal view, pass C into the drawable, ImGui's Metal backend, the pacing fence; iOS and macOS | 1-2 agents, ~500 lines in a patch + ios glue | mac_run.sh pictures; the bench rows at a pretended 120 Hz with 3 drawables; the Simulator (Metal runs in the Simulator); the device IPA and the maintainer's bench |
| M5 | the phone round: the passes/bench rows on the device, RG32UI packing of pass R's state if the store shows, memoryless E targets where a pass's output is consumed in the same render pass | 1 agent | the maintainer's logs; pictures unchanged |

Risks: (1) the generated MSL draws differently (precision: `highp` ints are exact either way; floats are IEEE on both, but
Metal's fast-math is ON by default and must be off for our float-float lerps: `MTLCompileOptions.fastMathEnabled = NO` /
`mathMode`; probe 1's compile used the default); (2) Metal's runtime compile at first start (3-4 s for all programs cold;
acceptable, or ship a `.metallib` once the toolchain is installed); (3) a second core to keep in step with every piece: the
generated shaders remove most of that, the C core remains; (4) the bridge vtable touches shared code, kept small.

**Performance on the A18 Pro.**
- *CPU, per present* (estimate from the Mac's samples and probe 2): the twiddle and the client-memory waits (together the
  phone's ~1.0-1.3 ms of upload calls and the GL thread's finishResource / semaphore waits) become `memcpy` into shared
  buffers (unified memory; the span buffer is written by the GPU) and ~6 encoders in one command buffer: ~0.1-0.2 ms.
  Per in-between the same: today's 2.6-4.6 ms of CPU a mid (docs/gpu-cost.md "In-betweens", CPU records) becomes the
  geometry (~0.5 ms on the phone) plus ~0.1. Expected: the gpu rows' 0.48-0.61 cores -> ~0.25-0.35 (estimate), which is
  the regime where the log's clock held.
- *GPU, per frame* (estimate from probe 2 scaled; the A18 Pro's 6-core GPU is 1.6-2x the M2's time on ALU-bound work by
  published TFLOPS, and the phone's measured R is 2.2x the M2's min): pass S ~0.4-0.7 ms as compute (against ~1.0-1.5 for
  the GL path if it worked there); pass R's 4-sample fixed store -0.15..-0.3 ms (0.59 -> 0.43 measured on the M2; RG32UI
  halves the rest); E, P, SMAA unchanged in kind (the same ALU; E's three stages can share one render pass with
  memoryless intermediates only where no neighbour is read, i.e. stage 0 -> stage 2's OR: ~-0.1); pass C into the drawable
  saves the present quad: a 1320 x 2868 RGBA8 read + write, ~0.4-0.6 ms on the phone (estimate from its compose row,
  which is the same size of work); the per-pass drains of timing mode are not real-frame costs either way. Sum at 4
  samples: 11.4 (measured, drained) -> ~8.5-9.5 (estimate, drained) a frame; AA off/SMAA 9.0-9.5 -> ~7-8. Tile memory,
  imageblocks and tile shaders do not fit pass R (its state is in registers already and its neighbours are read by P
  across tiles); what they could do later is fold pass P's "alike pixel" case into pass R's tile (programmable blending
  reads R's output without a store) if that 84-92% of pixels is ever worth a tile-shader piece; not counted.
- *Power and heat* (reasoning): the CPU's busy time is where the phone's thermal "fair" comes from at 60 Hz (0.5-0.6 cores
  of mostly memory-bound driver work); halving it is the biggest heat lever this study found. The GPU's energy follows
  its busy time; -15..-25% at 4 samples.
- *Present*: CAMetalLayer with a 3-drawable pool, `presentDrawable:afterMinimumDuration:` or the display link's
  `presentDrawable:atTime:`, the same model libntr-ios-lag.patch keeps for EAGL; the fence is the command buffer's
  `addCompletedHandler`. Measurable to the microsecond with `MTLCommandBuffer.GPUStartTime/GPUEndTime` (probe 2 used
  them) where GLES has no timer queries.

**Capability.** Compute (pass S, later anything else), 8 colour attachments, memoryless targets, tile shading and
imageblocks (Apple GPU family 4+, published: WWDC 2017 "Metal 2 on A11", WWDC 2020 "Harness Apple GPUs with Metal"),
programmable blending / framebuffer fetch, raster order groups, GPU counters and timestamps at stage boundaries
(`MTLCounterSampleBuffer`, WWDC 2020 "Optimize Metal apps and games with GPU counters"), Xcode's Metal frame capture and
shader profiler on the device (per-line GPU time), Instruments' Metal System Trace (what the heat question needs: GPU
busy, clock state, present timing on one timeline). Longevity: Apple's only supported GPU API.

## C. One Vulkan back end, Apple through MoltenVK

**What changes.** A Vulkan core (`gpu_vk.c`): instance/device, a swapchain or offscreen images, render passes, pipelines,
descriptor sets, buffers with explicit memory, the rings as fences; 2-3x Metal's boilerplate (estimate ~1,500-2,000
lines; the Vulkan pattern for "a handful of full-screen passes and one compute" is well trodden). Shaders: the same GLSL
with the uniform block, glslang -> SPIR-V at build time (probe 1's first half), embedded; pass S a compute shader (Vulkan
has compute everywhere; MoltenVK has no `VK_EXT_transform_feedback`, its supported-extension list and user guide say which
Metal limits apply: no geometry shaders, tessellation through compute, Metal's memory model). MoltenVK as a static
library (the XCFramework in its releases: iOS device, Simulator, macOS); it converts SPIR-V -> MSL with SPIRV-Cross at
pipeline creation (the same MSL as probe 1) and lets Metal compile it (0.3-0.7 s a program here, with a pipeline cache for
the MSL). SDL: `SDL_Vulkan_CreateSurface` on iOS through MoltenVK (`VK_EXT_metal_surface`). ImGui: `imgui_impl_vulkan`.
The bridge: the same vtable as B. Shared: the CPU half, the GLSL.

**Work.** B's M0-M5 shapes, plus the Vulkan boilerplate piece, plus the test hosts: MoltenVK on this Mac (natively: the
phone's exact translation runs here), lavapipe (Mesa's CPU Vulkan driver, conformant; a Linux CI build beside llvmpipe)
for the byte-exact checks. 7-8 agent pieces, ~3,000 lines.

**Performance on the A18 Pro, against B.** The GPU work is identical where the MSL is identical, and probe 1 shows it
would be (MoltenVK's SPIRV-Cross options differ in details: argument buffers for descriptor sets, `use_framebuffer_fetch_subpasses`
on Apple GPUs). What MoltenVK adds (published design, MoltenVK's runtime user guide; estimate for our size): the
descriptor/pipeline/command translation per command (~10 a frame: tens of microseconds), one `MTLCommandBuffer` per
`vkQueueSubmit`, Vulkan barriers mapped to Metal's encoder boundaries (an extra encoder split where Metal would not need
one), swapchain presents through its own drawable handling, timestamp queries at command-buffer or stage granularity
depending on the Metal device; memoryless through `VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT`; no tile shaders, no
imageblocks, no raster order groups, framebuffer fetch only as subpass input attachments. Public comparisons of native
Metal against MoltenVK by projects that ship both (Dolphin's July 2022 progress report on its Metal backend: faster and
more reliable than its Vulkan backend through MoltenVK on Apple silicon; PPSSPP's and RPCS3's macOS notes on MoltenVK's
overheads and driver-level bugs) agree on the direction: native Metal wins, by a margin that depends on how
draw-call-heavy the app is. Ours is not draw-call-heavy, so the margin here is small in absolute terms: estimate 0.05-0.3 ms
of CPU a frame and a few percent of GPU, plus the things MoltenVK cannot express. The real costs of C are elsewhere:
a 8-12 MB static library whose bugs become ours (MoltenVK's issue tracker is the place to look), three back ends in
practice (GLES does not go away: llvmpipe tests, old Android, Windows/Linux GL stays for the people who have it), and a
Vulkan core 2-3x the size of the Metal one, for a platform set (Android new devices, Windows, Linux) that does not need
Vulkan today: Android's GLES 3.1 has compute for pass S, and on new Android devices GLES is itself ANGLE over Vulkan
(published: Google's Android 15/16 developer notes on ANGLE as the system GLES driver), so a GLES renderer already rides
the Vulkan driver there.

**Capability on Apple through MoltenVK.** Compute yes; 8 attachments yes; memoryless yes; tile shaders/imageblocks no;
framebuffer fetch through subpasses (hedged); Xcode's Metal frame capture works (MoltenVK's guide documents debugging with
Xcode; you see Metal's view of the Vulkan stream); GPU counters as Metal exposes them to a command buffer; conformance:
MoltenVK reports Vulkan 1.2 feature level with the portability subset (its README; the conformance claim is theirs to
state, hedge). Longevity: MoltenVK is Khronos-hosted and in the LunarG SDK; it tracks Metal.

**Distribution.** MoltenVK static in the IPA: +8-12 MB (estimate from its release XCFramework after link and strip; it
carries SPIRV-Cross). App Store apps ship it; SideStore has no size rule; nothing to sign separately when static.

## The direct comparison the maintainer asked for

| | native Metal (B) | Vulkan translated by MoltenVK (C) |
| --- | --- | --- |
| shaders | the same GLSL, MSL generated at build time (probe 1); hand-written only for compute S and the drawable | the same GLSL, SPIR-V at build time, MSL made by MoltenVK at run time from the same SPIRV-Cross |
| GPU time | the baseline | the same shaders; a few percent for encoder splits at barriers (estimate) |
| CPU per frame | ~0.1-0.2 ms of encoding (probe 2) | + MoltenVK's translation, estimate 0.05-0.3 ms for ~10 commands |
| pass S | compute, writes the span buffer | compute, the same |
| 4-sample state | 4 attachments stored, 0.43 ms fixed (measured M2); RG32UI next | the same through Vulkan attachments |
| present | CAMetalLayer, our drawable pool, `presentDrawable:atTime:` / afterMinimumDuration, the display link | MoltenVK's swapchain (FIFO / immediate), its drawable handling; present timing extensions as MoltenVK exposes them |
| tile memory, tile shaders, imageblocks, raster order groups, framebuffer fetch | all | memoryless and subpass fetch only |
| profiling on the device | Xcode capture + shader profiler + counters, Instruments, timestamps per stage | Xcode capture of the Metal stream; counters as mapped; timestamps as mapped |
| test host here | native | native (MoltenVK on the M2 is the phone's translation) |
| code to write | ~2,000 lines | ~3,000 + the library |
| binary | +0 | +8-12 MB |
| back ends maintained | GLES/GL + Metal | GLES/GL + Vulkan (+ MoltenVK as a dependency) |
| what Android/Windows/Linux get | nothing from B; the uniform block and the vtable are small shared gains | a Vulkan path, which they do not need today (GLES 3.1 compute on Android; GL on desktop) |

**Own Metal path or a combined one?** Own. The combined path's only real merit, one modern back end for everyone, is
not available: GLES must stay for llvmpipe and old Android, and Windows/Linux are served by GL, so C is a third back end
with a library attached, not a replacement. Its merit for Apple alone is nil against B (same shaders, more layers, fewer
features, more bytes). The maintenance worry about an Apple-only path is answered by generating the MSL from the GLSL
(probe 1), which keeps one shader source and leaves ~800 lines of Objective-C as the Apple-only code.

## Platform parity

- **Android**: stays GLES 3.0 on the device's driver (new devices: ANGLE over Vulkan, by Google); pass S as a GLES 3.1
  compute shader when the GLES driver has it (most devices since 2014) and the transform-feedback draw where not, both
  through `gpu_gl.c` (`GPU_API_ES31`); the uniform block and the vtable come for free. Old-device quality: GLES drivers
  are the best-tested path on 2016-2020 Mali/Adreno (published and widely reported by emulator projects: their Vulkan
  drivers are the ones with the long bug lists). Vulkan on Android only if a phone shows GLES 3.1 compute is not enough.
- **Windows, Linux**: GL 3.3/4.x as today; the uniform block; `vcore_check`/`gpu_compare` on llvmpipe unchanged. lavapipe
  exists if a Vulkan core ever comes; not needed for B.
- **Back ends under each plan**: A: one (GLES/GL) + ANGLE as a library on iOS; B: two (GLES/GL, Metal); C: two cores
  (GLES/GL, Vulkan) + MoltenVK, three paths in practice.

## Distribution (unsigned IPA through SideStore)

Today's IPA is 5.4 MB. B adds no library (Metal is the system's; the MSL strings are ~200 KB). A adds ~8-12 MB static.
C adds ~8-12 MB static. SideStore signs the bundle with the maintainer's free Apple ID (7-day refresh, 3 apps); size and static
libraries do not matter to it; an embedded dynamic framework would need to be signed with the bundle (SideStore does
this, but static is simpler). No App Store review is involved; if one ever is, Metal is the native API and both ANGLE and
MoltenVK are used by App Store apps.

## Recommendation and order

1. **De-risk now (M0, one agent, this Mac)**: pass S as a Metal compute kernel against the real setup tables, records
   diffed against the CPU builder's, timed. Probe 2 says ~0.25-0.4 ms on the M2 against 0.7-0.8 through GL, and it does not
   touch the phone's broken transform feedback. If the integer fields come out exact, B is on; if they do not, the problem is
   in the GLSL -> MSL route and is found before anything else is built. **Done 2026-11-03: exact, 2.2-3x cheaper (the M0 result below).**
2. **B, M1-M2**: the generated-MSL step and the Metal core in gpu_render, verified byte-identical against the GL pictures on
   this Mac. This is the piece that proves the shaders survive the route with the right picture, not just the compiler.
   **Done 2026-11-03 (the M1/M2 status below; SMAA at 3x since, "SMAA's weight pass").**
3. **B, M3-M4**: the bridge vtable and libntr on Metal, macOS first (hidden runs, the bench at a pretended 120 Hz), then
   the Simulator, then an IPA for the maintainer's bench.
4. **M5**: the phone round; RG32UI packing if the store shows; the present timing on the display link.
5. **Not now**: ANGLE (only if OpenGLES.framework goes away before M4), Vulkan (only for Android, only if needed).

What this does not promise: the 120 Hz 4-sample frame. The study's own numbers put B at ~8.5-9.5 ms drained for 4 samples
and ~7-8 for SMAA on the phone, so SMAA at Resolution 3 is the setting that lands inside a 120 Hz frame with the GPU under
~50%, and 4 samples needs the pass-R work in docs/gpu-cost.md on top of any API. The heat gain B brings is mostly the CPU
side (the driver's twiddle and waits), and that one is large and certain in kind.

## M0 result (2026-11-03): pass S as a Metal compute kernel is exact, and 2.2-3x cheaper on the GPU

**Exact.** `build/gpu/gpu_mtl_spans` (port/gpu/tools/gpu_mtl_spans.m, macOS only) builds a capture's setup and the CPU's records
(gpu_spans_build as gpu_prepare's check mode), runs the kernel once and diffs with gpu_g02_compare. **0 integer fields differ**
on the 37 captures and the 730 p13-p19 scenes at 1x, 2x, 3x and 6x, AA off and 4 samples (SMAA at 3x too: the AA-off records).
The frames go up to 73,654 records (captures, 6x with 4 samples) and 8,269 (scenes), far past the phone's 4,092 transform-
feedback boundary. The attributes, against GL's pass S on the same frames and this build (`gpu_render --spans-check`, Apple GL
4.1; the scenes' GL counts are the P1 verification, which the captures' counts match to the digit):

| frames | Metal hi / lo beyond an ulp | GL hi / lo |
| --- | --- | --- |
| captures 1x / 2x / 3x / 6x, AA off | 0 / 20,891 · 0 / 46,131 · 0 / 71,826 · 0 / 154,703 | 88 / 21,503 · 57 / 47,790 · 39 / 75,970 · 44 / 165,232 |
| captures 1x / 2x / 3x / 6x, 4 samples | 0 / 46,131 · 0 / 98,892 · 0 / 154,703 · 2 / 332,703 | 57 / 47,790 · 61 / 103,928 · 44 / 165,232 · 50 / 357,887 |
| scenes 1x / 2x / 3x / 6x, AA off | 4 / 5,540 · 6 / 11,323 · 4 / 14,330 · 6 / 30,962 | 4 / 5,578 · 6 / 11,433 · 4 / 14,495 · 6 / 31,176 |
| scenes 1x / 2x / 3x / 6x, 4 samples | 6 / 11,323 · 8 / 23,805 · 6 / 30,962 · 8 / 64,667 | 6 / 11,433 · 8 / 23,977 · 6 / 30,962 · 8 / 64,926 |

Metal with fast math off is closer to the CPU's doubles than Apple's GL. With `--fast-math` (Metal's default) the counts become
GL's to the digit (3x: 39 / 75,970 and 44 / 165,232), so Apple's GL compiles with fast math; the integer fields stay exact either
way (the s_k XOR keeps the float-float steps apart), but the safe mode is the one to ship.

**Timing** (M2, load 1.9-2.0, min of 200, Resolution 3; GL = `gpu_render --time 200 --api gl`'s S, which includes the copy into
the span texture and timing mode's glFinish; Metal = the command buffer's GPUEndTime - GPUStartTime, and in brackets the CPU's
encode-to-waitUntilCompleted, a round trip a real frame does not pay per pass):

| ms | GL S min (mean) | Metal kernel min / median (round trip) |
| --- | --- | --- |
| town AA off (17,081 records) | 0.47 (0.58-0.68) | 0.18 / 0.19 (0.36) |
| town 4 samples (34,153) | 0.76 (0.84) | 0.33 / 0.35 (0.52) |
| battle AA off (7,071) | 0.32 (0.44) | 0.10-0.11 / 0.12 (0.27-0.30) |
| battle 4 samples (14,132) | 0.38-0.41 (0.53) | 0.17 / 0.19 (0.34) |

Threads a threadgroup (32-256) change nothing beyond noise; 64 stays. Probe 2's 0.24 / 0.39 ms estimate held (lower: 0.18 /
0.33). The copy and the transform-feedback flush are gone by construction: the kernel writes the buffer pass R would read.

**Route: (a), generated.** port/gpu/tools/gpu_mtl_spans_gen.py takes g00_head.glsl's two lines S needs (the ROW_ constants,
g_at), g02_setup.vert unchanged with S_FRAG defined (s0-s11 plain globals) and its two mains cut, and tools/gpu_mtl_spans.comp's
main (one invocation per record, twelve uvec4 into an SSBO); the loose uniforms (uNFind, uZero, uN) become one std140 block;
glslang 16.6.0 `-V` -> SPIR-V -> SPIRV-Cross (vulkan-sdk-1.4.363.0) `--msl --msl-version 20100 --msl-decoration-binding` ->
1,673 lines of MSL, committed (tools/gpu_mtl_spans.metal; the build needs neither tool), compiled at run time by
newLibraryWithSource with `mathMode = MTLMathModeSafe`. It worked at the first run, so (b) was not needed. The same .comp body
is GLES 3.1 compute, which Android could run as it is.

**What M1 should know.**
- The translation is not where the risk is: integer code, texelFetch (-> `texture2d<uint>.read`), floatBitsToUint
  (-> `as_type`) and dynamic loops came through exact. SPIRV-Cross passes the uniform block (`constant U&`) into every
  function that reads it, and emits an unused `sampler` argument per sampler2D: bind any sampler state at those indices.
- Fast math must be off for the attribute lo/hi to be at least as close as GL's; set `mathMode` (macOS 15 / iOS 18 SDK) with
  `fastMathEnabled = NO` as the fallback.
- The cut of g02_setup.vert's mains is textual (the last `#ifndef S_FRAG`); when M1 adds the uniform block to the GLSL for
  every pass, give pass S a `#ifdef S_COMP` main in g02_setup.vert instead and drop the cut.
- The kernel still reads the setup and find tables as 1024-wide textures, so the GLSL stays the one source; M2 can make them
  `device` buffers if the fetches show (they did not need to here).
- Build and run: `meson setup build/gpu port/gpu && ninja -C build/gpu gpu_mtl_spans`;
  `build/gpu/gpu_mtl_spans --scale 3 --aa 2 [--time 200] [-v] FILE.vcs...` (a summary line per run, a file's report when it
  differs). Regenerate the MSL after a g02_setup.vert change: `python3 port/gpu/tools/gpu_mtl_spans_gen.py GLSLANG SPIRV_CROSS
  port/gpu/tools/gpu_mtl_spans.metal`.

## M1/M2 status (2026-11-03): the Metal core draws GL's pictures; pass S is its compute kernel

**What exists.**
- *The uniforms* are one std140 block (shaders/g00_head.glsl `GpuU`, src/gpu_uni.h): the one shared change. GL binds it as a
  uniform buffer; the pictures and `--spans-check` counts are the same as before (below).
- *The MSL* is generated and committed: `port/gpu/msl/*.metal`, twelve programs (the full-screen vertex shader; R, E, P at k = 1
  and 2; C; SMAA's edge, weight and blend passes; pass S's kernel), from the same concatenation `gpu_gl.c` gives GL with the
  SPIR-V header (`#version 310 es`, GPU_SPIRV, GPU_SPAN_BUF). `python3 port/gpu/tools/gpu_msl.py gen GLSLANG SPIRV_CROSS` makes
  them (glslang 16.6.0: github.com/KhronosGroup/glslang releases, the macOS universal tarball; SPIRV-Cross vulkan-sdk-1.4.363.0:
  github.com/KhronosGroup/SPIRV-Cross at that tag, `cmake -B build && cmake --build build`, the `spirv-cross` binary; neither is
  installed system-wide). Each file's second line is the SHA-1 of its GLSL; the Apple build's embed step (`gpu_msl.py embed`)
  recomputes it and fails with the command to run when a GLSL input changed. The build itself needs neither tool. A new program
  is a line in gpu_msl.py's PROGRAMS (meson lists msl/ at configure time: `meson setup --reconfigure build/gpu port/gpu` after
  adding one; gpu_msl.py and every shader are the embed step's inputs, so a GLSL edit reruns the check without a reconfigure).
- *The core*, src/gpu_mtl.m (Objective-C, ARC, macOS/iOS), behind `gpu_create(NULL, GPU_API_METAL, ...)`: one device and queue;
  the programs compiled at first use from the embedded MSL, pipelines found by name with their bindings read from the pipeline
  reflection; the frame's passes in one command buffer; pass R's k x k samples as slices of a 2D array texture, colour
  attachments of one render pass (dontCare / store); the tables in shared buffers in rings of three, seen as linear textures (a
  memcpy, no driver conversion), a frame waiting only for the command buffer that last read its ring entry; readbacks by blit.
  Pass S is a compute kernel (g02_setup.vert's S_COMP main, one thread a record) writing a buffer that pass R reads as it is
  (GPU_SPAN_BUF: no copy, no texture); the CPU's records (pass S off, `--spans cpu`) go into the same buffer. The M0 tool
  (gpu_mtl_spans) is folded into the core and removed.
- *gpu_render --api metal*: the same outputs (--hi, the 1x picture, --compose gpu), `--spans-check` through the core, `--time K`
  with each pass a command buffer of its own, its GPU time (GPUEndTime - GPUStartTime).

**Fast math.** Apple's GL compiles with fast math (M0). The Metal core compiles pass S's kernel with `MTLMathModeSafe` (its
records closer to the CPU's doubles than GL's, as M0 found) and the fragment programs with `MTLMathModeRelaxed`: with Safe,
pass R's texture coordinates round differently in a few pixels (town 1x: 7 pixels; neither GL nor Metal-safe is vcore's at
those 4-7 pixels, the float interpolation is approximate either way), with Relaxed or Fast the pictures are GL's to the byte.

**Verified** (M2, `SDL_MAC_BACKGROUND_APP=1`, the 37 captures of the P1 rounds):
- *Pictures*: gpu_render `--api metal` against `--api gl` (the GL pictures of the build before the uniform block), `--hi`, the
  1x picture and `--compose gpu` of every capture at 1x, 2x, 3x and 4x with AA off, 4 samples and SMAA: **1,322 of 1,332
  byte-identical**. The ten that differ are SMAA at 3x, `--hi` and `--compose gpu` of 5 captures (town, t1401, f001400,
  f001401, f003100), 3-4 pixels each (town: 3 of 442,368, one 6-bit step of a channel). Found: the edges pass is the same to
  the byte (RG8 read back from both); the weights differ in 6 texels (e.g. 120 / 119, 74 / 69), so it is SMAA's weight pass
  (its bilinear area and search lookups). Fixed since: see "SMAA's weight pass" below; **1,332 of 1,332** now.
- *GL after the uniform block*: the 1,332 GL pictures byte-identical to before it; `--spans-check` (GL) counts the same at 1x
  and 3x, AA off and 4 samples; gpu_tests pass.
- *Records*: `--spans-check` through the Metal core, the 37 captures: **0 integer fields differ** at 1x, 2x, 3x and 6x, AA off
  and 4 samples (103,419 to 1,241,194 records a run); the attributes exactly M0's (hi 0 but 2 at 6x with 4 samples, lo beyond an
  ulp 20,891 / 46,131 / 71,826 / 154,703 AA off, 46,131 / 98,892 / 154,703 / 332,703 with 4 samples, against GL's 21,503 ...
  165,232 at 3x).
- Not run here: `vcore_check --oracle gpu=` and gpu_compare.py (their scenes are on the PC). Their GL results cannot change:
  gpu_render's command line and GL pictures are the same as before. Through Metal they would need `--api metal` on the
  oracle's fixed command line (an env default like GPU_COMPOSE's), not added.

**Timing** (`gpu_render --time 200 --scale 3 --compose gpu`, min over 200; GL: glFinish after each pass, CPU wall time; Metal:
each pass its own command buffer, GPU time):
load 29-55 (other agents' builds: high, so the minimums are the numbers to read; the means swing 1.5-2x). GL's per-pass figures
are wall time with glFinish (each includes a queue round trip), Metal's the GPU's own; the frame (gpu_last_times' total) and
pass C (`compose`, wall time with glFinish / commit-and-wait after each) are wall time for both.

| ms, min of 200 | S | R with S | E + P | SMAA | C | frame |
| --- | --- | --- | --- | --- | --- | --- |
| town AA off, GL / Metal | 0.53 / 0.19 | 1.59 / 0.91 | 0.58 / 0.24 | - | 0.51 / 0.43 | 2.37 / 1.89 |
| town 4 samples | 1.04 / 0.48 | 4.94 / 3.52 | 1.50 / 1.04 | - | 0.85 / 0.71 | 6.76 / 5.72 |
| town SMAA | 0.63 / 0.19 | 1.82 / 0.93 | 0.63 / 0.25 | 0.32 / 0.10 | 0.60 / 0.74 | 3.13 / 2.28 |
| battle AA off | 0.52 / 0.10 | 1.92 / 0.70 | 0.36 / 0.05 | - | 0.56 / 0.75 | 2.54 / 1.37 |
| battle 4 samples | 0.54 / 0.21 | 2.83 / 2.29 | 0.38 / 0.12 | - | 0.50 / 0.73 | 3.31 / 3.33 |
| battle SMAA | 0.62 / 0.32 | 2.95 / 2.15 | 0.39 / 0.10 | 0.37 / 0.18 | 0.94 / 0.74 | 3.89 / 3.29 |

Pass S as compute is 2-5x cheaper than GL's transform feedback plus copy (M0's 2.2-3x, here under load and against GL's
wall time); R without S is about GL's at 4 samples (the ALU-bound loop, as the study expected) and cheaper AA off; E + P and
SMAA are 2-4x cheaper (GL's figures carry a round trip each). The frame's wall time in timing mode still pays a commit and wait
per pass on both; a real frame (one command buffer) pays one. Redo at low load before quoting a ratio.

**The API for M3/M4.** `gpu_create(NULL, GPU_API_METAL, N, aa, err, n)`, then as with GL: `gpu_render_prepared(g, s, p)` encodes
S, R, E, P and SMAA into the frame's command buffer; `g07_create_metal(N, ...)` and `g07_compose_packed(c, s, k, gpu_3d_tex(g), N,
gpu_smaa_weights(g))` encode pass C into the same one. The picture is `gpu_mtl_texture(g07_frame_tex(c))`: an id<MTLTexture>
(RGBA8Unorm, 256 N x 384 N, the top screen first, row 0 at the top; private storage). Texture names (gpu_3d_tex,
gpu_smaa_weights, g07_frame_tex, g07_upload_3d) stand for MTLTextures as GL names do for GL textures. `gpu_mtl_command_buffer()`
gives the frame's command buffer (a present encodes into it: M4's pass C into the drawable can replace g07's target later);
`gpu_mtl_commit(wait)` commits it; a readback commits and waits by itself, and a ring entry still in an uncommitted buffer is
committed before it is waited for. One thread at a time (the render thread), as the GL core. For M3's GL present the picture
must reach GL: c->out would be made from an IOSurface (or blitted into one), the one change pass C needs there.

**SMAA's weight pass (2026-11-03).** The cause: the searches (`a_search_x_left` ...) step 2 pixels while `texcoord > end`,
and in the plain case the last step lands on `end` exactly, so whether they take a 9th step hangs on the last bit of `end`
and of the steps. At 1x, 2x and 4x `aMetrics` is a power of two and every product is exact; at 3x (1/768, 1/576) it is not,
and Metal's relaxed math, which reassociates, computed `offset2.x = -16m + (-0.25m + fc * m)` as
`fma(fc, m, fl(-16.25m))`: some ends moved by an ulp, a search by two pixels, the area lookup's distance by 2. Not the
samplers, the textures or SPIRV-Cross's `textureLod(Offset)`: the edges, `e`, `e1`, `e2` and the search lengths were equal.
Found by writing the inputs' bits into the weights' free channels (four base-256 digits of `fract(v * 2^k)`, two a run in
`.ba` of the north block so GL's compile stays the one that writes the pictures: writing them elsewhere, or with
`floatBitsToUint`, changes how Apple's GL compiles the pass and its weights become Metal's), then fitting formulas to the
values in exact rationals. Apple's GL, for this pass: no reassociation, every `a * b + c` fused across statements (`offset0 =
fma(fc, m, o * m)`), and the `+16` ends (`offset2.yw`) alone as `fma(fc, m, fl(16m + 1.25m))`. The fix is in the generator
(tools/gpu_msl.py `FIXES`): the MSL of that pass gets `#pragma METAL fp math_mode(safe)` and `contract(fast)`, and its GLSL,
for SPIR-V only, the `+16` ends written as GL rounds them. Metal Safe alone left 6 texels, Safe with contract(fast) 3; with the
ends, the weights are GL's to the byte at 2x-6x on the 37 captures and the pictures 1,332 of 1,332. The pragmas are MSL 3.2
(macOS 15, iOS 18); what an older compiler makes of them is not tried (iOS 16.3 is the IPA's minimum).

**Left.**
- M3: the bridge's Metal half (port/bridge untouched here); the port's meson (port/meson.build) does not build gpu_mtl.m yet
  (GPU_HAVE_METAL, gpu_msl.c's embed step, the Metal framework: the lines of port/gpu/meson.build).
- M5: RG32UI packing of pass R's state, memoryless targets, pass C into the drawable.

## M4 status (2026-11-03): libntr presents with Metal on macOS and iOS (vcore's picture); off by default until M3

**What is there.** `Present=metal` in sim_config.ini (or BWPORT_PRESENT=metal|gl), read at window creation
(libntr-ios-metal.patch, last in the wrap; port/src/metal_present.mm + .h; Dear ImGui's own port/src/imgui_impl_metal.mm, MIT,
fetched at eabff0fd08, the IMGUI_VERSION_NUM 19297 libntr vendors, only its imgui.h include changed). The window is an SDL
Metal view (SDL_WINDOW_METAL, SDL_Metal_CreateView/GetLayer); the CAMetalLayer has 3 drawables, BGRA8Unorm (no sRGB step, as
GL wrote the bytes), framebufferOnly. One command buffer a present: nextDrawable (taken at the present, as late as possible),
a clear pass with the picture (an RGBA8 texture from a ring of 4, each reused once the GPU finished the last present that read
it; nearest; the screen quads from sim_GetScreenQuadArray, the viewport in drawable pixels from the top) and ImGui's draw data
(the touch overlay and controls are ImGui draw lists: nothing else to port), presentDrawable, commit. macOS sets
displaySyncEnabled from the swap interval GL would have had; iOS presents at the next refresh, as EAGL did (plain
presentDrawable: the slots are already locked to the CADisplayLink, so neither afterMinimumDuration nor atTime is needed).
- **Fences**: LagSubmit/LagPoll/LagDone and the BWPORT_DRAWABLES aid keep their code; their fence functions are shims whose
  "sync" is a present's serial, signalled by its command buffer's completion handler. LagPoll takes the command buffer's real
  GPU end (GPUEndTime on the pacing clock) as the done time, not the polls around it; the GPU-behind rule and the deadlines
  are unchanged. A present with no drawable still commits, so a fence on its serial ends.
- **GPU figures**: GPUEndTime - GPUStartTime of every present, summed a tick: the Metrics breakdown's "present" row (no GL
  timer queries: GqBegin is off), the sys line (`gpu N ms/frame ~P% busy (Metal command buffers)`, every frame, no burst), and a
  `metal:` line after each bench row (GPU ms a present, mean and max; nextDrawable waits over 0.1 ms a second and their ms).
- **Screenshots**: a cfgscript `shot` / BWPORT_SHOTS frame is encoded a second time into an offscreen BGRA8 texture in the same
  command buffer, blitted to a shared buffer and written after waitUntilCompleted; a present LagSubmit drops still gets its
  shot (offscreen only).
- **Renderer=gpu**: decided at window creation: Renderer=gpu keeps the GL present (log `Present=metal: Renderer=gpu draws with
  OpenGL until its Metal picture exists (M3): the GL present`); Renderer=auto under Metal is vcore (no GL to ask); a live switch
  to gpu under Metal draws vcore and says so once. The Metrics window and the overlay name `Metal: <GPU>`.
- **Not ported (GL only, test aids)**: BWPORT_GPUSLOW (off with a line), the switch test's GL census, BWPORT_VCAPTURE.

**Verified (M2, hidden runs, load 2-5 unless noted).**
- Pictures: the room save, vcore 1x, f1700: the game area of the Metal shot equals the GL shot's byte for byte; ImGui's
  blending differs by 1 in a handful of pixels (24 in the menu bar's border row, some text edges in the Metrics panel). Town and
  a wild battle (caught save, walking in the grass) look right (NPCs move on a real clock, so only the room compares exactly).
- MTL_DEBUG_LAYER=1 through live switches of Resolution 1/3/8/2/4/1, Frame rate 60/120/Monitor/Unlimited, Smooth on/off and a
  refused gpu: no validation message; ImGui's settings windows (BWPORT_UIOPEN=video,audio) drawn.
- Pacing at BWPORT_PANELHZ=120 (vcore, town, Smooth): every row 1.00x, behind/s 0.0-0.5 (the drawable waits are not counted as
  the game's lateness: they come inside the present, after the slot's sleep), in-betweens/s as GL's. Rows (GL / Metal):

| row | presents/s | mid/s | swap ms/s | gpu lag ms | held/s | Metal GPU a present | drawable waits |
| --- | --- | --- | --- | --- | --- | --- | --- |
| vcore 2x, 120, Smooth | 119.5 / 119.5 | 58.9 / 58.9 | 87 / 173 | 1.8 / 2.1 | 0.5 / 0.5 | 0.35 ms (max 6.0) | 99/s, 160 ms/s |
| vcore 1x, 120, Smooth | 119.5 / 119.5 | 59.4 / 59.9 | 89 / 213 | 1.8 / 2.4 | 0.2 / 0.0 | 0.35 ms | 104/s, 202 ms/s |
| vcore 2x, 120 | 119.5 / 119.2 | 0 / 0 | 121 / 220 | 1.9 / 2.5 | 0 / 0 | 0.36 ms | 100/s, 207 ms/s |
| vcore 2x, 60 | 59.6 / 59.6 | 0 / 0 | 76 / 5 | 1.8 / 1.4 | 0 / 0 | 1.0 ms (a low clock) | 0 |
| vcore 3x, 120, Smooth (both with BWPORT_DRAWABLES=3) | 118.1 / 119.5 | 0.0 / 8.5 | 330 / 346 | 3.9 / 3.5 | 56.7 / 38.2 | 0.36 ms | 108/s, 333 ms/s |
| vcore 4x, 120, Smooth (both with BWPORT_DRAWABLES=3) | 113.7 / 118.7 | 0.2 / 0.0 | 345 / 719 | 4.9 / 6.7 | 51 / 57.5 | 0.41 ms | 113/s, 703 ms/s |

  The hidden window's layer hands drawables back at the Mac panel's 60 Hz however fast the presents come, so above 60
  presents a second nextDrawable waits (1.6-6 ms a wait): a real drawable pool, harsher than the phone's (its panel runs at
  the 120 the presents do). BWPORT_DRAWABLES=3 on top of it waited 0-90 ms/s more: not needed under Metal (the GL rows
  need it to wait at all).
- Without a pretended link (load 30-47): Monitor on the 60 Hz panel 59.8 presents/s, swap 8 ms/s (GL's swap waits for the
  V-blank: 886-914 ms/s), GPU lag 0.7-0.8 ms (GL 16); Frame rate 120 on the 60 Hz panel 117.2 presents/s, 51 mids/s (GL 119.7,
  59.5: the drawable waits again).
- Simulator (iPhone 17 Pro, iOS 27.0, Present=metal via BW_SIM_INI, vcore): `present: Metal on Apple iOS simulator GPU,
  drawable 1206x2622, window 402x874`; the town in portrait and landscape (SIMCTL_CHILD_BWPORT_TOUCH_ORIENT) with the overlay
  and the buttons; the settings sheet open and closed over the Metal view (BWPORT_SETTINGS_TEST shots: UIKit's snapshot shows
  the Metal picture); a background trip (Settings app and back: paused, back, no Metal errors); ~65 ticks/s at Unlimited
  (the GL path's software ES managed ~35). The device IPA builds (`tools/ios_package.sh ipa`, SDK stamp 26.0); not published.

**For M3 (the GPU renderer's picture).** metal_present.h is the interface: `bwport_mtl_device()` / `bwport_mtl_queue()` give the
present's device and queue (make the picture's textures on that device and commit their command buffer on that queue before
the present, so the present runs after it without a fence); `bwport_mtl_quad(tex, verts, n, viewport, linear)` with tex an
`id<MTLTexture>` draws it instead of the uploaded picture (tex NULL = the last bwport_mtl_upload). In libntr that is a Metal
twin of SIM_SetFrameRendererGL (returns the texture and its size) called where SIM_DrawFrameRenderer calls the GL one, plus
`SIM_InterpTexture`'s Metal twin for the in-betweens; then the Renderer=gpu and auto rules in SIM_RendererWanted and at window
creation drop their Metal exceptions, and Present=metal becomes the Apple default (sim_config.c's default).

**Left.** (1) The macOS Monitor rate under Metal has no V-blank lock: the present does not block, so the phase loop runs on our
clock (59.8 Hz against the panel's 60); a CVDisplayLink / CAMetalDisplayLink feeding SpLink (as the CADisplayLink does on
iOS) is the fix, needed once Metal is the Mac default (done 11-03: "The Mac's display link"). (2) The panel model (LagShown) still estimates when a present reaches
the glass; under Metal the drawable's addPresentedHandler gives the real presentedTime (iOS 10.3+, macOS 10.15.4+): a better
`shown ms` and drops rule for M5. (3) The phone's own bench with Present=metal (the maintainer's IPA). (4) No Present row in the
settings UI (an ini key until M3 makes it the default).

## M3 status (2026-11-03): the GPU renderer draws with Metal under Present=metal, straight into M4's present

**Decision: the GPU API is the present's.** With Present=metal the GPU renderer draws with the Metal core; with Present=gl, with
the GL core. No IOSurface path (Present=gl with the Metal core): there is no GL context under the Metal present and no Metal
picture is wanted under the GL one, and the hand-over through IOSurfaces (rectangle textures on macOS, CVPixelBuffers and BGRA on
iOS, a fence each way, a ring) would be throw-away work once Metal presents. One setting therefore: sim_config.ini `Present=gl|metal`
(BWPORT_PRESENT, and BWPORT_GPU_API=metal|gl as its alias), the desktop Video window's **Graphics API** row (OpenGL / Metal, Apple
builds) and the iOS sheet's Display > **Graphics API**; both apply at the next start (the window is made with the API). The default
stays OpenGL for now (sim_config.c's `present = 0`): making Metal the Apple default is one line there.

**What exists.**
- *The bridge* (port/bridge/gpu_bridge.c): stage 2's API-specific steps are a table, `Half`: save/restore of the present's state,
  the error drain, the timing drain, pass C's creation, the picture's name, the 1x CPU picture, the per-pass GPU readout and its
  release, the out-of-memory check, the first log line. kGL is the code as it was, moved; kMetal (Apple builds, `BR_METAL`) is
  port/bridge/gpu_bridge_mtl.m. Gather, the worker, Stage1, the in-betweens' CPU half, the readouts and the self-check are shared.
- *The hand-over (the interface M4 asked for)*: `gpu_mtl_use(bwport_mtl_device(), bwport_mtl_queue())` before the first core, so
  the core's frame command buffer is committed on the present's queue ahead of the present's (`MtlPicture`: commit, no wait; Metal's
  hazard tracking orders pass C's writes of the output texture against the present's reads, across command buffers of one queue).
  libntr gets a name: `BR_MTL_BASE + ` the core's texture name, or `BR_MTL_UPLOADED` for a CPU picture the bridge handed to
  `bwport_mtl_upload` (vcore's fallback frames, capture frames); libntr-ios-gpuapi.patch's `SIM_SetFrameTextureMetal(fn)` turns
  the name back into the id<MTLTexture> (`br_mtl_tex_of`) for `bwport_mtl_quad`. The frame renderer and in-between hooks are the
  same functions as under GL (`SIM_SetFrameRendererGL`, `SIM_SetInterpTexture`); only the meaning of the returned name changes.
- *libntr-ios-gpuapi.patch* (last in the wrap): the texture hook; Renderer=gpu and auto no longer fall back to vcore or to the GL
  present under Metal (auto = gpu when the program draws with Metal); the GL-only test reads of the renderer's texture
  (SpHashTex, SwMeanTex, LookTex), the switch's texture reset and the in-between's glFlush skipped under Metal; the passes row's
  label and the status line's device name under Metal; the settings above.
- *The build* (port/meson.build): on macOS and iOS, port/gpu/src/gpu_mtl.m, the generated-MSL embed (gpu_msl.py embed, the
  staleness check as port/gpu's), -DGPU_HAVE_METAL for the gpu library and the bridge's Metal half; the frameworks were already
  linked (M4). tools/port_wraps.py reconfigures when port/gpu/msl changes. Windows, Linux, Android: the same sources and flags.
- *The Simulator*: its Metal makes no linear texture on a shared buffer, so there (TARGET_OS_SIMULATOR) the core's tables are
  private textures blitted from their ring buffers in the frame's command buffer.
- *GPU figures*: every command buffer the bridge commits has a completed handler with GPUEndTime - GPUStartTime; the sum goes to
  the sys line (`gpu N ms/frame (Metal command buffers)` is now the renderer's frames and in-betweens plus the presents). With the
  Metrics Breakdown's GPU timers on, the frame is split into one command buffer a pass (committed, never waited for) and each
  pass's GPU time fills its row. passes=1 rows: the core's timing mode (each pass a command buffer, waited for, its GPU time) and
  pass C's command buffer's GPU time. The bench's `metal:` line is still the presents' alone.

**Verified (M2, hidden runs).**
- *Pictures*, room save, gpu 3x, f1500, Metal against GL (both presents): the game area of the shots byte-identical at AA off, 4
  samples and SMAA (the 21 pixels that differ are the menu bar's border row, ImGui's blending, as M4 found). Town and the switch
  test's end shot look right (NPCs move on a real clock).
- *Self-check*: `pass S: metal ... 0 integer fields differ` at frames 30 and 300 (Mac and Simulator).
- *Live switches* under Metal with MTL_DEBUG_LAYER=1, Smooth on, 120: 48 steps of Renderer gpu/vcore, Resolution 1/2/3/4/8, AA
  off/4 samples/SMAA: every step's status line right (`gpu 3x: 768x1152, AA SMAA on Metal: Apple M2`), no validation message, no
  error. The same script under GL: the same lines. Not run: the RSS / malloc census of the 100-switch recipe.
- *Simulator* (iPhone 17 Pro, iOS 27.0, BW_SIM_RENDERER=gpu, Present=metal, Resolution 2): the town drawn, no errors; the first
  frame 6.2 s (the MSL compile). The device IPA builds (`tools/ios_package.sh ipa`); not published.
- *Bench rows*, town, gpu 3x, Smooth, Monitor, BWPORT_PANELHZ=120 BWPORT_DRAWABLES=3, load 2-4, GL / Metal:

| AA | speed | pres/s | mid/s | cores | CPU ms/tick | swap ms/s | gpu lag ms | behind/s |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| off | 1.00 / 1.00 | 119.8 / 120.0 | 58.9 / 54.2 | 0.30 / 0.24 | 5.11 / 4.05 | 177 / 677 | 5.2 / 6.5 | 1.2 / 6.0 |
| 4 samples | 1.00 / 1.00 | 119.8 / 119.6 | 58.4 / 59.2 | 0.30 / 0.25 | 5.03 / 4.19 | 193 / 472 | 6.2 / 5.4 | 2.0 / 0.7 |
| SMAA | 1.00 / 1.00 | 120.0 / 119.3 | 58.9 / 58.9 | 0.29 / 0.26 | 4.90 / 4.40 | 206 / 509 | 5.0 / 5.5 | 1.5 / 0.2 |

  Metal takes 15-20% less CPU a tick. A rerun at load 18-30 (other agents) kept GL at 59 mids/s while Metal fell to 24-48 with
  behind/s 11-16: Metal's swap ms/s is the hidden window's nextDrawable waits (M4: its layer hands drawables back at the panel's
  60 Hz, 475-840 ms/s here), which block the main thread where GL's swaps do not; on a loaded machine that leaves less room for the
  in-betweens. A visible window or the phone (its panel runs at the presents' 120) is the real test; M4's CVDisplayLink item applies.
- *passes=1 rows* (town 3x Unlimited, load ~4; GPU ms a frame, GL with glFinish / Metal command-buffer GPU time): AA off R 3.36 /
  2.90, E+P 1.00 / 0.65, C 0.91 / 0.45; 4 samples R 6.18 / 5.19, E+P 2.08 / 1.59, C 0.75 / 0.32; SMAA R 3.78 / 2.90, E+P 0.99 / 0.64,
  SMAA 0.45 / 0.23, C 0.90 / 0.47. (In the game the GPU runs at a lower clock than in gpu_render's --time loop: higher figures.)

**Left.** The phone's bench with Graphics API = Metal (the maintainer's IPA); the bench's `metal:` line could add the renderer's
command buffers; the RSS census over the switches; making Metal the Apple default once the phone confirms it.

## M5 status (2026-11-03): Metal tuned from the phone's first logs; Metal is the Apple default

a test iPhone17,2 (A18 Pro, iOS 26.3, 120 Hz, build 41febbabd, reduced CPU clock ~1.1-2.0 GHz) ran the perf test with
Graphics API = Metal (bwport-20261103-130102.log) and GL 7 minutes earlier (-125426.log). All in libntr-ios-metaltune.patch (last
in the wrap) and port/src unless noted.

**1. The drawable wait (fixed).** At Monitor + Smooth, gpu 3x, AA off and SMAA, every present waited in nextDrawable ~5.7 ms
(120 waits/s, 686-688 ms/s of the game thread): 48.7 / 44.7 in-betweens/s, behind 9 / 13 a second; with 4 samples 22 waits/s and
58.9/s. The waits are a phase, not a shortage of drawables: the layer frees one a few ms after the slot, and the 4-sample rows'
slower GPU shifts that phase. Now a thread of the present's own (port/src/metal_present.mm, `bwport-drawable`) calls nextDrawable
as soon as the present took the one before and holds it; the present takes the waiting drawable (2 ms at most if it has not come,
then that present goes without one: the screen keeps the picture before, counted as `no drawable N/s`), encodes the quad and
ImGui, presents and commits as before. The renderer's passes were already their own command buffer committed before the
present's (M3), independent of the drawable. presentDrawable:atTime / afterMinimumDuration were not needed (the slots are the
display link's). A drawable of an old size (resize, rotation) is let go and the next one is taken.
- Mac (town, gpu 3x, Monitor + Smooth, BWPORT_PANELHZ=120, load 2.3-2.7), before / after: swap ms/s 408 / 9 (AA off), 379 / 9
  (SMAA), 523 / 9 (4 samples); in-betweens/s 60.4 / 60.3, 60.2 / 60.1, 58.7 / 60.2; behind 0 / 0 (4 samples 1.5 / 0); gpu lag
  4.6 / 2.2 ms. The thread now waits the 395-800 ms/s the game thread did; no present lacked a drawable. Frame rate 120 on the real
  60 Hz panel (no BWPORT_PANELHZ): 119.4 presents/s, 59.3 in-betweens/s (M4: 117.2 and 51).
- What the Mac can and cannot show: its hidden window's layer frees drawables late as the phone's does (M4's "60 Hz" reading was
  this phase, not a 60 Hz pool: the thread gets 120 a second), so the game-thread cost and its removal show here. The Mac's
  in-betweens were already ~60 before the fix (a fast CPU absorbed 400 ms/s of waiting); the phone's lower clock is what turned
  the waits into "behind". BWPORT_DRAWABLES (the GL aid) is not used under Metal. The phone's bench is the real check.
- The present's command buffer now starts at the slot, beside the renderer's work, so its GPUStart..End span (the `metal:` line's
  GPU ms a present) reads longer (0.9 -> 1.8 ms AA off): overlap, not cost.

**2. E+P with 4 samples (found here; cut since: docs/gpu-cost.md "E+P with 4 samples", two draws in pass P).** Phone passes rows (ms a frame): Metal E+P 3.57 against GL 3.11 (wall time), while
AA off 1.15 / 1.58 and every other pass is faster on Metal. gpu_render --api metal --time on the Mac (town, 3x, load 4-12), E and
P timed apart (a throwaway split): E 0.16 / 0.18 ms (AA off / 4 samples), P 0.15 / 0.68: the 4-sample cost is pass P's, and of its
0.68 ms 0.46 are the slow-sample loop (with the loop skipped, a throwaway: 0.22). Metal's E+P scales 3.1x from AA off to 4
samples on both the Mac (0.24 -> 0.75 ms) and the phone (1.15 -> 3.57); GL's ~2x (Mac 0.60 -> 1.17 wall, phone 1.58 -> 3.11), so
it is the shader as Metal compiles it, not the phone's GPU alone. Tried, no gain: no lossless compression on the sample layers
(allowGPUOptimizedContents = NO: P 0.71 -> 0.71, E 0.18 -> 0.25), p_sample as a real call (noinline: P 0.68 -> 0.80). Not tried:
splitting P's slow samples into a pass of their own (a GLSL change, so GL's program too), MSL compile options. The loads and
stores are already DontCare / Store on every pass, and the sample layers are read with read() at integer coordinates.

**3. The restart prompt.** iOS: choosing the other Graphics API shows "Takes effect when the app starts again" with Close app now
(the setting is written again, then exit(0)) and Later; desktop: the Video window's row opens a "Graphics API" prompt with
Restart now (RestartProgram, which now also restarts on macOS: execv of the executable, so Game > Reset is on the Mac too) and
Later. Simulator: present=0 -> the alert -> Close app now -> sim_config.ini Present=gl -> the relaunch (tools/ios_sim_run.sh
BW_SIM_RELAUNCH=S) ran on GL ES. Test hooks: the sheet test's alert=close|later, BWPORT_UIOPEN=apiask.

**4. Metal is the Apple default.** sim_config.c: present = 1 under __APPLE__ for a missing key; an ini with Present=gl stays GL.
A fresh Simulator install (no Present key) logs `present: Metal`; Mac run folders without an ini run Metal (BWPORT_PRESENT=gl for
GL). Windows, Linux, Android: GL as before.

**5. The `metal:` line** adds `renderer N ms/s (K command buffers)` (every command buffer the bridge commits, from the completed
handlers the sys line already had), `GPU spans ~P% of the time` (presents + renderer; overlapping command buffers count twice),
`no drawable N/s` and `fetch thread waits N/s M ms/s`.

**The perf test** (maintainer's request; 27 rows since libntr-ios-perfrows.patch: 9 of them again with the Fast 3D renderer, `-fast`), each setting every key it depends on, with Max fast forward at gpu 3x 4 samples and
SMAA (docs/perf.md "The phone test"; about 3 minutes).

**Pictures.** The room save, gpu 3x 4 samples, f1500, GL against Metal after the change: the game area byte-identical (all 123
differing pixels in the menu bar's text rows, ImGui's blending). MTL_DEBUG_LAYER=1 through live switches of Resolution, AA, Frame
rate, Smooth and Renderer: no validation message.

**For the phone.** In the Monitor + Smooth rows the `metal:` line should read drawable waits ~0 and the fetch thread's waits
where the game thread's were; in-betweens/s near 59-60 at AA off and SMAA with behind ~0; no drawable near 0 (if it is high, the
2 ms take wait is too short for the phone's phase: the next knob). E+P at 4 samples is unchanged.

**Left.** E+P at 4 samples (item 2); ~~M4's CVDisplayLink for the Mac's Monitor rate under Metal~~ (done: "The Mac's display
link" below); the panel model's shown time from addPresentedHandler (the glass line measures it now; the model still estimates).

## The Mac's display link (2026-11-03, M5; libntr-ios-maclink.patch, port/src/metal_present.mm)

**The fix.** Under Present=metal on macOS the present never waits for the refresh (the drawable comes from M5's fetch thread), so
Frame rate Monitor ran on the program's clock: slots of 1/60 s from SDL's whole-Hz mode, no phase loop (that one reads a swap that
blocks), and the present calls wandered across the refresh boundary. Now a CVDisplayLink on the window's display gives the last
refresh and the period (`bwport_mtl_vsync`), and SpLink returns it as iOS returns its CADisplayLink: SpMonitorNs takes the link's
rate (whole Hz, a change followed after 0.5 s), a timeline restart keeps the slots on the refreshes, SpAfterSwap's phase loop aims
each present 1 ms after a refresh, and the panel model (shown ms, drops) runs. Only for Monitor without fast forward: 60, 120,
Unlimited and fast forward (Max) stay on the program's clock as before (SpLink sees no link there), and GL is unchanged (it never
asks). Game speed 2x-4x (the FF button's capped timeline) stays on the link.

**Which API.** No deployment minimum is set for the Mac build (port/machine/macos.ini: the SDK's own, macOS 27). CVDisplayLink
(deprecated since macOS 15, the warning silenced around it) because it is the display's, not the window's: its callback runs on its
own thread whether the window is on screen or not, so the hidden test window has a link, and it needs no run loop (SDL pumps the
main one once a frame). CAMetalDisplayLink (macOS 14) and NSView's displayLink follow the view and stop for a window not on screen.
The callback does not come at a fixed point of the refresh (inNow is the call's own time: inNow.hostTime - now ~0, inOutputTime -
inNow 24.0-24.9 ms on the M2's 60 Hz panel): the refresh grid is inOutputTime's, so the last refresh is that grid's last point at
or before now (on the pacing clock: mach_absolute_time converted, SDL's counter is the same). A link with no tick for 100 ms (a
display asleep) counts as none, and the pacing falls back to its own clock.

**A display change.** Each present checks the display the window is on (the layer's view's NSWindow.screen, the main display for
a window on none: the hidden one) and moves the link there (CVDisplayLinkSetCurrentCGDisplay; log line `display link on display
N (H Hz nominal)`); the period comes from each tick's timestamp, so a refresh change on the same display is followed too (and
SpMonitorNs restarts the timeline after it held 0.5 s). Not tested: the M2 here has one display and the test window is hidden;
the code path is the one the start takes (the first present creates the link on the window's display).

**The glass line** (after each bench row's `metal:` line): `shown`, `never shown` (each drawable's presentedTime from
addPresentedHandler; 0 = the compositor never showed it), `refreshes without a new picture` (between two shown presents) and
`present to glass` ms; then `link:` the present calls on the link's refreshes, which needs no glass: refreshes with no call and
with two, a second, and how far into its refresh a call came; `paced by the link` or `by the program's clock`. A HIDDEN window
is never composited: every present reads `never shown`, so on the hidden test runs only the `link:` half means anything.
BWPORT_DISPLAYLINK=0 keeps the link for the counts but the pacing does not see it (the before, on the same build).

**Bench** (town, gpu 3x, AA off, hidden, --noskip; load 3-5; before = BWPORT_DISPLAYLINK=0, after = default; 4 s rows):

| row | presents/s before / after | in-betweens/s | behind/s | link: refreshes with none / two a second, before | after | call into the refresh ms, before | after |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Monitor | 60.0, 59.8 / 59.7, 59.8 | 0 / 0 | 0 / 0 | 6.5 / 6.5, 10.0 / 10.2 | 0 / 0, 0 / 0 | 10.9 (0.0-16.6), 9.4 (0.0-16.7) | 2.2 (1.0-7.7), 2.3 (1.0-4.4) |
| Monitor + Smooth | 59.8, 59.9 / 59.8, 59.9 | 0.0, 0.2 / 0.0, 0.2 | 0 / 0 | 9.5 / 9.7, 12.0 / 12.0 | 0 / 0, 0 / 0 | 10.2, 6.4 (0.0-16.7) | 2.1 (1.0-4.0), 3.1 (1.0-5.9) |
| 60 | 59.6 / 59.6 | 0 / 0 | 0 / 0 | 0.5 / 0.5 | 0 / 0 | 6.7 (0.0-15.7) | 8.4 (1.8-15.0) |
| 120 | 119.5 / 119.5 | 0 / 0 | 0 / 0 | 0.25 / 59.9 | 0 / 59.6 | (two a refresh: the 60 Hz panel) | |
| 120 + Smooth | 119.4 / 119.4 | 59.8 / 56.1 | 0 / 0 | 0 / 59.3 | 0 / 59.1 | | |
| Unlimited | 235 / 235 (3.93x) | | | 0 / 60.0 | 0 / 60.2 | | |
| Monitor + fast forward (Max) | 59.7 / 59.7 (31x) | | | 0.75 / 0.5 | 0 / 0 | 10.1 | 7.8 |

Before, the calls sat across the refresh boundary (a 16.667 ms slot against the panel's own period, nothing pulling them off it),
so 6-12 refreshes a second got no new present and as many got two: on the glass, a repeat and a drop each time. After, every
refresh gets one call 1-8 ms into it. Ten consecutive Monitor + Smooth rows (40 s) and Game speed 4x and back: 0 refreshes with
none or two in every row, calls 1.9-2.7 ms in, no slow drift. 120 + Smooth's 56.1 is the in-betweens' usual A/B swing (docs/
port-worker.md "In-betweens A/B"). MTL_DEBUG_LAYER=1 through Monitor + Smooth, 60, a live renderer switch to vcore and a fast
forward toggle: no validation message.

**What a visible run would show** (not done: visible runs only when the maintainer asks): the glass half filling, Monitor `shown 60/s,
never shown 0, refreshes without a new picture ~0`, and present to glass about one refresh plus the GPU time (the model says 14
ms). Before the fix the same row should show the 6-12 a second. One open question it answers: after a late present (gpu lag max
24 ms in one row) the panel model's chain stays a refresh behind (shown ms 14 -> 30 for the rest of the run, no drops), because
with one present every refresh nothing lets it catch up. If the compositor queues the presents, the glass is a refresh late too
(present to glass ~30 ms: one present then wants dropping, as the model's 2-waiting rule would if it said 2); if it shows the newest
at each refresh, `never shown` counts the one it skipped and the model should learn from presentedTime. The phone's glass line
(the iOS build prints it too, with its CADisplayLink's period) answers the same there.


## Pass C and the present in one pass? (2026-11-03: measured, not built; nothing pays)

The question: fold pass C (compose, 256N x 384N RGBA8) and the present's pass (that picture to the drawable with nearest
sampling, the clear around it, ImGui / the touch overlay) into fewer full-screen passes. Under Metal the present's quad is
always nearest (libntr's SIM_DrawFrameTexture passes linear = 0), so the on-screen pixel is pass C's pixel at the nearest N x
position: a fused pass is exact only by evaluating compose once per drawable pixel inside the game's rectangle.

**Measured** (M2, load 2.7-4.3 unless noted; GPU time = GPUEndTime - GPUStartTime of a command buffer run alone; min over
300-2000 because the GPU's clock steps (the mins fall on a few levels, e.g. pass C 3x at 0.16 / 0.21 / 0.44): comparisons
only at the same level). Throwaway probes, not committed: a present that drains the queue first, waits for itself and cycles
five variants present by present; gpu_render printing the command buffer's GPU time; pass C's colour store as DontCare.
- *The present* (room save, gpu 3x, Unlimited, drawable 1984x1582, the game 1024x1536 at (0, 46), three windows of 300 each):
  full 0.176 / 0.186 / 0.185 ms; without the quad 0.159 / 0.162 / 0.164; without ImGui 0.156 / 0.169 / 0.163; clear and store
  alone 0.142 / 0.148 / 0.148; the quad reading a 1x1 texture instead of pass C's 768x1152 output 0.170 / 0.174 / 0.171. So of
  0.18 ms, 0.15 is the drawable's own clear + store (and the command buffer), the quad ~0.02, ImGui ~0.02, and reading the 3x
  output from memory ~0.01 of the quad's 0.02.
- *Pass C's store* (gpu_render town.vcs, 400 each, interleaved): Store / DontCare 0.44 / 0.44, 0.21 / 0.21 (3x), 0.39 / 0.39
  (5x): below the clock levels' resolution (< 0.01 ms). The 3x output is 3.5 MB written and read once a frame; pass C is bound by
  its shader, the store overlaps it.
- *Compose per pixel* (the fused pass's cost: pass C at the scale whose output is the drawable's game rectangle; gpu_render
  --compose gpu, 2000 each, top level): 3x 0.21, 4x 0.26, 5x 0.39, 6x 0.55-0.68 ms; about 0.11 ms fixed + 0.11 ms a megapixel.

**The phone** (bwport-20261103-144959.log, passes rows: each pass its own command buffer, waited for): compose 0.55 / 0.69 /
0.74 ms (4 samples / AA off / SMAA), the present 0.54 / 0.71 ms (its own command buffer: drawable 1320x2868, the game 1320x1980,
5.16 drawable pixels a DS pixel, 2.95x the 3x output's pixels). The Mac's split applied to the phone's present: drawable clear +
store ~0.55, quad ~0.08 of which reading the output ~0.04, ImGui ~0.08. The round trip through memory is not a real cost on
either GPU.

**The options.**
1. *Pass C straight into the drawable* (compose per screen pixel, nearest, the touch overlay / ImGui drawn after in the same
   render pass): saves the quad and the output's store and load (Mac ~0.03, phone ~0.1 ms) but evaluates compose 1.78x (Mac,
   4x-equivalent) / 2.95x (phone) as often: Mac +0.05 for compose, net ~+0.02 ms worse; phone, on the Mac's fixed + per-pixel
   shape scaled to its 0.69 ms, compose 0.69 -> ~1.3 ms, net ~+0.5 ms a frame worse, twice that with Smooth (both run per
   in-between). Capture frames and screenshots would also lose the N x picture they read (gpu_mtl_c_read, the shot's re-encode
   from the quad's texture) and need pass C anyway.
2. *The present sampling pass C's inputs directly*: the same per-pixel compose as 1 (no cheaper way to evaluate it exactly).
3. *One render pass, tile memory*: tile memory (programmable blending, imageblocks) gives a fragment its own pixel only; the
   present stretches each pass C pixel over 1.72 (phone) / 1.33 (Mac) drawable pixels, across tile edges, so the output must
   be in memory. A tile shader composing each tile's N x pixels into threadgroup memory first could save at most the
   ~0.01-0.04 ms of the store and load above, as hand-written MSL beside the generated GLSL (GL could not follow), with the same
   loss for capture frames and shots. One command buffer for both passes changes no GPU work and ties the present to the
   renderer's commit (the present's command buffer starts at the slot on its own: M5).

**Verdict.** None pays: the present's cost is the drawable's own clear + store (~80% of it), which every option keeps, and the
part a fusion could remove (the quad and the round trip, ~15%) is smaller than the extra compose work. Nothing was changed.
Where a cut could come from instead: the drawable's size (the phone draws 3.79 MP for a 2.6 MP game; a smaller drawable is a
picture change) or pass C's shader itself (0.11 ms a megapixel on the M2, the same work at any output size).

## The present's CPU on the phone (2026-11-03; libntr-ios-presentcpu.patch, port/src/metal_present.mm)

**Where it goes.** Time Profiler on a test iPhone (tools/ios_device_profile.py --template time, now system-wide with the
app's pid kept, symbolicated with atos: `--inclusive N`, `--under FN`), town save, gpu 3x, 4 samples, Monitor + Smooth (120
presents/s), 10 s, build 8dc7ad963. CPU ms a second by thread: game 246, worker 226, unnamed 130 (GCD, Metal, CA), AudioQueue 21,
bwport-drawable 7.3.
- unnamed 130: the command buffers' submission 50 (`-[_MTLCommandQueue _submitAvailableCommandBuffers]`, almost all of it the
  IOGPU kernel trap; 240 command buffers a second: the renderer's and the present's, ~0.2 ms each), GCD's worker threads starting
  and parking 39 (`start_wqthread` with nothing under it), the completion notifications 11, the system's FramePacing framework 7.5
  (it follows every command buffer of a queue that presents; ~4.5 more on the game thread), the drawable's present and CA ~5,
  the presented callbacks ~3.5 (+ ~3 of CA's IPC for them). Our own completion handlers (the present's, the bridge's, ImGui's): 0.5.
- game thread, the present (bwport_mtl_present) 19: the drawable's render encoder 6.5 (AGX framebuffer setup), ImGui's Metal
  encoding 4.6 (3.0 of it copying the vertices), the command buffer's creation 1.3 and commit 1.7, the quad 1.1, the rest small.
- game thread, the waits (SpSleepUntil) 35: run loop entries 25 (`bwport_ios_runloop_for`: while a frame's GPU end was awaited, nearly
  always under Metal, the wait went in 1 ms chunks, each a CFRunLoopRunInMode with UIKit's update sequence), the spin 7.
- the drawable thread 7.3: nextDrawable itself (CA's image queue, malloc), not our handoff.

**What changed.** (1) Under Metal the wait no longer goes in 1 ms chunks while a GPU end is awaited (libntr-ios-presentcpu.patch,
SpSleepUntil): the chunks were for GL, whose done time is the poll that saw the fence; Metal's is the command buffer's GPUEndTime
whenever it is polled. On iOS a wait is now one run loop entry until the last ms, then the last ms and the spin as before (GL:
unchanged; Windows, Linux, Android: GL only, so unchanged). (2) The glass line's bookkeeping (a presented handler a present,
GlassCall) runs only while a bench row is measured (SIM_BenchOpen, the same patch); BWPORT_GLASS=0 turns it off in the rows too
(the A/B's play state). A window's glass counts now start afresh (nothing is recorded between windows).
Tried, no gain: the last ms of a wait as a nanosleep instead of a run loop entry under Metal (B2 against B, 3 rounds: -0.5 to
+0.2 % cycles, inside the spread).

**Phone A/B** (tools/ios_device_ab.py, A = 8dc7ad963, B = this with BWPORT_GLASS=0, 3 rounds A B A B A B, all nominal; M cycles a
drawn frame from `threads:`, best of 3, spread in brackets):

| row | A | B | B/A | unnamed ms a drawn frame A / B | in-betweens/s A / B | behind/s, no drawable/s A / B |
| --- | --- | --- | --- | --- | --- | --- |
| Monitor + Smooth (maintainer's) | 16.91 (0.4 %) | 16.68 (1.4 %) | -1.4 % | 2.06 / 1.99 | 60.1 / 59.7 | 0.2, 0.2 / 0.2, 0.2 |
| 60 | 9.74 (0.6 %) | 9.02 (3.0 %) | -7.4 % | 1.75 / 1.39 | - | 0, 0 / 0, 0 |
| Game speed 4x | 12.03 (1.5 %) | 11.95 (0.8 %) | -0.7 % | 1.06 / 1.04 | - | 0, 0 / 0, 0 |
| fast forward (Max) | 70.97 | 70.84 | -0.2 % | 1.46 / 1.46 | - | 0, 0 / 0, 0 |
| Monitor + Smooth again | 15.66 (2.2 %) | 15.22 (5.9 %) | -2.8 % | 2.10 / 1.94 | 60.4 / 60.1 | 0, 0 / 0, 0 |

Instructions follow (-1.3, -5.1, -0.5, -1.4, -8.7 %). The 60 row gains most: its waits are 16 ms, so they had ~14 chunks each. One
B run's second Monitor + Smooth row had a GPU stall (GPU 7.8 ms a present, renderer 1176 ms/s, 41 in-betweens/s, 6.5 no drawable/s,
coming out of fast forward); the other 5 B rows and the 6 of the B2 A/B (same code) did not: not counted as the change's. Mac
(M2, load 1.9-4.4, the same rows, BWPORT_PANELHZ=120, old waits by a throwaway env switch against the new): instructions -0.5 %
(Monitor + Smooth) and -0.7 % (60): the Mac's waits were nanosleeps, not run loop entries; in-betweens 60, behind 0.

**The glass line's link: half on the iPhone (fixed).** It counted each call's refresh as an index from 0 (time / period): the
CADisplayLink's period moves by a few ns, which moved the index by thousands, so the phone's rows read 46-1494 "refreshes with
none" a second (the old build's A runs above). Now the refreshes between two calls come from the distance of their refreshes'
starts. Phone, this build with the glass on (town, gpu 3x, 4 samples): Monitor + Smooth none 1.25 / two 0.25 a second and 0 / 0,
calls 2.3-2.6 ms into the refresh, shown 118-120/s, never shown 0, refreshes without a new picture 1.0 and 0/s, present to glass
14.1-14.4 ms; x4 0 / 0; Monitor 0.75 / 0.25; 60 (the program's clock) 7 / 7, calls across the refresh as before; in-betweens/s
58.4-60.4, behind 0-0.5, no drawable 0-0.2. Mac (hidden, load 2.9-3.9, MTL_DEBUG_LAYER=1, no message): Monitor, Monitor + Smooth
0 / 0 with calls 3.3-3.5 ms in, 60 2 / 2 (the program's clock), in-betweens 0.2/s at a real 60 Hz panel as before.

**Why not one command buffer a present.** The renderer commits its frame (and each in-between) as soon as it is encoded, and the
present's command buffer comes at the slot, up to a refresh later; with the GPU busy ~96 % of the time at the maintainer's settings
(the `metal:` line's spans), holding the renderer's work until the slot would start it late and push the picture a refresh back,
and putting the present ahead of the next frame's passes in one command buffer would hold the drawable until those passes end. The
lever that would remove the present's command buffer is presentDrawable:atTime: in the renderer's own command buffer (the quad
drawn there, the drawable taken at render time, shown at the slot): the pacing's present call, its phase loop and the glass line
would all move to that time, and the drawables would be held a slot longer. Not done: a pacing change. Worth at most the present's
command buffer: ~0.2 ms of submission + the GCD wakes it causes a present, ~25-35 ms/s at the maintainer's settings (~4-5 % of CPU).

## The present path's CPU and GPU cost a picture (2026-11-04; measured on the Mac, nothing built)

The questions: is the drawable set up as cheaply as it can be, can ImGui's copy go, how much of the driver / GCD cost is per
command buffer, and which ideas keep the picture and the pacing exactly as they are. Mac (M2, hidden window, Metal, gpu 3x, 4
samples, Smooth, Monitor with BWPORT_PANELHZ=120, BWPORT_DRAWABLES=3, town save, `--noskip`, the pinned bench), build 4448c061e
plus two throwaway env switches in port/src/metal_present.mm (not committed): BWPORT_PRESENT_NOCB=1 (with `ablate=present`: no
present command buffer at all, the serial marked done at once) and BWPORT_PRESENT_NOCLEAR=1 (the drawable's load action DontCare
instead of the clear). Load 3.8-6.6 (four other agents measuring; every run under the timing lock).

**The drawable (port/src/metal_present.mm, by inspection).** `framebufferOnly = YES`, `MTLPixelFormatBGRA8Unorm` (no sRGB
conversion; CAMetalLayer offers no 16-bit format, and the game's bytes are 8-bit: this is the cheapest format it can take),
`maximumDrawableCount = 3`, `displaySyncEnabled` follows the frame-rate setting. The quad's 64 vertices go by setVertexBytes (no
buffer). So nothing is left on the layer itself; the present's GPU cost is the drawable's own clear + store (docs above: ~80 %
of 0.18 ms on the Mac, ~0.55 of 0.56 units on the phone).

**The clear (load action) is free: DontCare measured the same.** Apple's GPUs clear in tile memory; the memory traffic is the
store. Monitor + Smooth, `passes=1` (each renderer pass its own waited command buffer; the present's GPU time still includes its
wait behind an in-between, so this is an upper bound comparison at the same queueing): Clear 1.69 / 1.79 / 1.65 / 1.79 ms a present
(mean 1.73) against DontCare 1.82 / 1.91 / 1.71 / 1.69 / 1.80 / 1.75 (mean 1.78; two runs each, A B A B, 3 rows of 240 frames):
+3 %, inside the rows' spread (the queueing). At 60 Hz, Smooth off, `passes=1` (the GPU idle between command buffers: the present's
time is its own, the 0.57 ms of the passes rows): Clear 0.324 / 0.301 / 0.298 / 0.335 ms a present (mean 0.315) against DontCare 0.319 / 0.306 / 0.307 / 0.286 (mean 0.305),
A B A B, two rows of 240 presents each: -3 %, inside the spread (the renderer's 386-390 ms/s agree within 1 %; the present's absolute
0.30 here against the passes rows' 0.57 is a GPU clock level, this run's GPU idling more at 60 Hz with Smooth off). The clear's
3.1 MP x 4 bytes are never written: nothing to gain from a load action change, on the Mac or (the same tile architecture) the phone.

**ImGui.** On the Mac the game's draw data is empty during play (no touch overlay; a menu or the FF button only), so the Mac's
present is clear + quad + store and the Mac cannot measure ImGui's cost: the phone's 4.6 ms/s (3.0 copying vertices) is the touch
overlay, drawn every present (libntr-ios-metal.patch: `bwport_mtl_ui(1)` whenever `TotalVtxCount > 0`). The copy is
ImGui_ImplMetal_RenderDrawData's memcpy of every draw list's VtxBuffer / IdxBuffer into a reusable shared MTLBuffer each present
(port/src/imgui_impl_metal.mm). ImGui rebuilds its draw lists on every NewFrame, so "unchanged" cannot be read from ImGui; it must
come from the overlay's own state (pad opacity, finger positions, the FF button's state): when nothing changed since the last
present, keep the last present's vertex / index buffers and re-encode the same draw commands. A memcmp against the last copy costs
about what the memcpy does and gains nothing. Worth at most ~3 ms/s on the phone (0.5 % of CPU); item 7 (the UI built only for
presented ticks) is the larger ImGui lever and is already on the list.

**Command buffers.** At the maintainer's settings there are 2 a present on both machines: the renderer's (a frame or an in-between) and
the present's, 240 a second (the metal line: 480 command buffers a 4 s row at 120 presents). Empty present command buffer
(`ablate=present`) against none at all (BWPORT_PRESENT_NOCB=1), two runs with the rows interleaved ref / abl-present / ref / abl-present
inside each:

| row | process M instr a drawn frame | unnamed ms a drawn frame | all ms |
| --- | --- | --- | --- |
| ref (the real present), run A / B | 19.62, 18.62 / 19.37, 19.46 | 0.71, 0.76 / 0.73, 0.64 | 4.03, 4.58 / 4.16, 3.63 |
| abl-present, empty command buffer (A) | 20.53 (first row after the switch), 18.00 | 0.28, 0.53 | 4.08, 4.34 |
| abl-present, no command buffer (B) | 20.42 (first row), 17.83 | 0.28, 0.32 | 3.87, 3.00 |

The present's command buffer itself (empty) costs the Mac 0.1-0.2 M instructions a drawn frame (0.5-1 %); its unnamed-thread share is
inside the within-run spread (0.28 against 0.53 for the same rows in one run). What the real present costs over an empty one is the
drawable: unnamed 0.64-0.76 against 0.28-0.53 ms a drawn frame (presentDrawable, CA's transaction, the completion with GPU work,
the drawable thread's nextDrawable), and 1.2-1.6 M instructions. So on the Mac the submission is cheap and the drawable is the cost;
the phone's 0.2 ms a submission is the IOGPU kernel trap (docs above), which the Mac's driver does not have: item 3's gain cannot
be measured here, only on the phone (its `abl-present` row against a no-command-buffer variant, the same two switches).

**What keeps the picture and the pacing exactly.** Ranked by expected gain at the maintainer's settings:
1. *Phone: one command buffer a present* (item 3; the present's quad and overlay encoded into the renderer's command buffer with
   presentDrawable:atTime:). Expected 120 x ~0.2 ms = ~24 ms/s of unnamed CPU on the phone (3-4 %), nothing on the Mac (measured
   above). Effort medium (the pacing's present call moves to the renderer's commit, the glass line follows, the drawables are held
   a slot longer). Risk: pacing (behind/s, no drawable/s, the glass line's "call into the refresh" must stay as they are). First
   check: the phone's abl-present row against BWPORT_PRESENT_NOCB=1 (two 4 s rows) to confirm the 0.2 ms is per command buffer and
   not per drawable; if the phone shows what the Mac shows, drop the item.
2. *Phone: ImGui's overlay re-encoded from the last buffers when its state is unchanged*: ~3 ms/s (0.5 %), effort low, risk none
   (the draw data is bit-identical by construction). Only worth doing beside item 7.
3. *The drawable's load action*: DontCare saves nothing measurable (the clear is a tile operation); the store is the cost and must
   stay. Nothing to build.
4. *A 16-bit or smaller drawable*: the only lever on the store's ~0.55 units a present on the phone. Not available (no 16-bit
   CAMetalLayer format) / a picture change (the maintainer's call: a drawable of the game's rectangle only, 1320x1980 = 69 % of the pixels,
   would save ~0.17 of the 0.55 units a present, 2 % of GPU work, and change the letterbox's handling).

Not tried (time): the present's render encoder setup on the phone (6.5 ms/s: AGX framebuffer setup a present) follows the drawable's
texture and cannot be cached across drawables; a smaller `maximumDrawableCount` changes pacing, not CPU.
