# A second, cheaper 3D path: the DS polygons through the GPU's own rasterizer (design study, 2026-11-03)

The maintainer asked for a 3D renderer beside today's exact one: the picture "only slightly off, looks more or less the same, running
MUCH more optimized", offered as an option. This is a measured study with a throwaway prototype on Metal; nothing is in the app.
Clean room: GBATEK, the Metal and OpenGL specifications, and our own port/vcore and port/gpu; no emulator source was read.

## The answer

- **Cost.** On the M2 (Metal, town capture, Resolution 3), the triangle pass that replaces pass S + pass R costs **0.31 ms against
  1.13 ms with AA off (-73 %)** and **0.88 ms against 1.95 ms with 4 samples (-55 %)**; battle 0.28 against 0.89 and 1.07 against
  2.24. Pass E/P, compose and the present are untouched and stay as they are. On the phone: see "The phone" below (filled in from
  the A/B of this round).
- **Picture.** Over the 37 captures at 3x, **11 % of the 3D layer's pixels differ with AA off (8-16 %), 15 % with 4 samples**, and
  almost all of it is small: 68 % of the differing pixels move a channel by 1-3 steps of 64 (gradient bands and texel-boundary shifts
  of one N-pixel, a third of a DS pixel at 3x), 10 % by 8 or more (a texel of a sharp texture lands one N-pixel over). Nothing
  changes which polygon wins a pixel (0.3 % of the differing pixels). The diff pictures are listed under "Pictures"; at the maintainer's
  viewing size the two are hard to tell apart; the known visible differences are the edge-marking outline (one N-pixel thinner
  and smoother on slanted edges), the DS antialiasing's soft edge (a plain analytic coverage instead of p16's), and the bottom row
  of every polygon (one N-pixel short).
- **What it is.** The geometry engine's clipped polygons (already on the CPU) go to the GPU as triangle fans with vcore's screen
  positions and attributes; a fragment per covered N-pixel runs the SAME per-pixel rules as pass R (G4 texture decode, G5 plot:
  the depth test with the DS's equal tolerance, alpha test, translucency with the polygon-ID rule, shadow stencil, fog flag) on the
  pixel state read from the colour attachment (programmable blending, Apple GPUs); the polygons' edges go once more as lines to
  set the edge flag and a coverage. The output is pass R's sample layer, so pass E, pass P, compose and SMAA run unchanged.
- **Recommendation.** Worth building as the option "3D renderer: Exact / Fast" on Apple (Metal) first, where the fragment can read
  the attachment: 3-4 days for the Metal form with the four-sample mode and the settings, after the phone A/B of this round
  confirms the proportion. On GL ES 3.0 (Android) the same shader works through EXT_shader_framebuffer_fetch (most phones);
  desktop GL 4.x has no equivalent in core (ARB_fragment_shader_interlock + image load/store on NVIDIA/Intel; AMD lacks it), so
  Windows/Linux would get a plainer form (hardware depth, fixed-function blending) that is further from the DS: described under
  "Platform parity". The exact renderer stays the default; "Fast" is for the phone at 120 Hz with Smooth motion.

## 1. What maps to hardware rasterization, and how close

The prototype's choices are what "close" means below; "exact" means the same bytes as pass R for that feature given the same inputs.

| DS feature (GBATEK) | Hardware form | Closeness | Cost |
| --- | --- | --- | --- |
| Polygon setup: the clipped polygon (3-10 vertices, integer screen positions from p14's viewport, at N x through gpu_vertex_pos) | A triangle fan per polygon in drawing order, one vertex buffer, one draw for the frame; positions as clip (x w, y w, z w, w) so the GPU interpolates by w | Exact positions. A quad's attributes: the DS interpolates along its edges then across the row; for a planar quad the hardware's two triangles give the same projective map, so the same values up to the DS's own rounding (below) | ~0.1 ms CPU for 600 triangles (12 floats a vertex); no span records, no pass S |
| Coverage: the DS fills from the left edge's pixel to the right edge's pixel inclusive on each row, every row from the top to the bottom vertex, except vertical right edges (p16; DISP3DCNT bit 4 is on in the game, so every polygon is "whole") | Vertices shifted by +0.5 so the GPU's centre sample at (i + 0.5) tests the DS's integer point (i, j); the top-left rule then includes the left/top edges, excludes a vertical right edge (the DS rule) | Close: the same pixels but the DS's own per-row edge x (its fixed-point slope rounding) at slanted edges, 1 N-pixel either way, and the polygon's bottom row (the DS draws it, the rule excludes it). The shift was measured (town, 3x): 0.5/0.5 gives 11.3 % differing pixels, 0/0 18.9 %, 0.75/0.75 17.8 % | free |
| Attribute interpolation: colour and texture coordinates perspective-correct by w; per row f = (x - xs) / den with den = n or n + 1 (slanted right edge), colour and texcoord floored, the edge's colour an integer, an X-major right edge sampled one pixel past (p16, measured against the hardware) | The GPU's perspective-correct interpolation of the plane; floor at the fragment | Close, not exact: the den quirk stretches a DS row's texture by n / (n + 1) (a texel of shift at a span's far end), the edge rounding bands a gradient per row. This is 62 % of the differing pixels (71 % with 4 samples), almost all 1-3 steps of 64 | free |
| Depth: 24-bit z screen-linear from p14's sz (z-buffering); w perspective-correct (SWAP_BUFFERS bit 1, the battles: swap 1 in the battle captures) | z: clip z = sz / 2^24 - 1 x w, so the GPU's gl_FragCoord.z is the same screen-linear plane; w: the interpolated w varying, perspective-correct = the DS's 1 / lerp(1 / w). The test itself runs in the fragment on the attachment's depth word (24 bits), so the equal mode's +-0x200 and the shadow mask's "failing samples" are as in pass R | Exact in form; the float32 interpolation of a 24-bit value rounds the last unit, which a `<` test can flip where two polygons are within 1 of each other (none seen: "poly" 0.3 %) | free; the hardware depth buffer is unused (an optional early reject for opaque polygons, "Open items") |
| Alpha test, alpha 0, wire-frame alpha 31 | In the fragment (G5 as it is) | Exact | - |
| Translucency: the draw order (opaque first then translucent, sorted by p18), the same-ID rule, the fog-flag AND, the depth update flag (bit 11) | The same order (one draw, the fan list follows s->geo.order); the rules on the attachment's state | Exact per pixel; the picture differs only where coverage or interpolation differ (22 % of the differing pixels are on translucent pixels) | - |
| Shadow polygons (mode 3: the mask by ID 0, the shadow by ID) | In the fragment on the state's stencil flag (F_STENCIL) | Exact (no capture uses them: 0 in all 37) | - |
| Edge marking and fog | Pass E and pass P unchanged: they read the sample layer (depth, IDs, flags, coverage, colour) | Exact given the flags; the edge flag is the lines' (next row) | as today |
| Edge flag and coverage (p16: the row's last pixel, the edge runs, every pixel of a flat bottom's last row; the coverage from aa_edge's formulas) | A second draw of every polygon edge as a hardware line (diamond-exit rule, 1 pixel wide); its fragment marks the pixel an edge pixel when the polygon's own fill owns it (last == k, not translucent) and sets the coverage to the pixel's covered part along the edge's minor axis (analytic) | Close: the same pixels on Y-major edges, 1 N-pixel differences on slanted X-major ones, the bottom row missing (above); the coverage is a different formula (the DS's is per edge pixel from its slope tables). 16 % of the differing pixels are on or next to edges | ~0.05 ms (town: 1,300 edges) |
| Toon / highlight, texturing (formats, palettes, repeat/flip, the decal and modulation modes) | g04_texture.glsl as it is, from the same flattened VRAM textures | Exact (the sampling coordinates are the interpolated ones) | as pass R's shade |
| The rear plane and clear image (DISP3DCNT bit 14) | A full-screen draw first (g06_clear.glsl as it is) | Exact | ~0.02 ms |
| 1-dot polygons (DISP_1DOT_DEPTH) and line polygons (zero area) | Not drawn by a triangle rasterizer: a point / line primitive each (not in the prototype; the captures have 0 dots, 0-2 lines) | Close (a point's N x N block as Dot=block) | nothing |
| Wire-frame (alpha 0) | Lines of the edges with the fill skipped (not in the prototype; 0 in the captures) | Close | nothing |
| The DS antialiasing (coverage blended in pass P) and the 4-sample mode | AA off: pass P's DS AA on the lines' coverage. 4 samples: the pass rendered on the 2N grid (supersampling, the grid pass R uses) and a small pack pass into pass R's four RG32UI layers with its "alike" bit; hardware MSAA is not usable for this: the plot rules are per sample state, so a per-sample fragment costs what supersampling costs, and an integer attachment has no hardware resolve | Close (the coverage), the 4-sample layers exact in form | 0.88 ms (M2, town) for 4x the fragments + 0.08 pack |
| In-betweens (Smooth motion) | The same: a vertex buffer from the blended geometry, no span records | Exact to the frame's rule | the geometry (0.7 ms CPU) + ~0.1 ms |

Not reproducible: the DS's per-row rounding of attributes and coverage (the "close" rows), unless the fragment recomputes them from
the span's ends, which is pass R again.

## 2. The prototype (scratch only, Metal, the maintainer's M2; nothing committed but this file)

In the session scratchpad's `hwraster/` (a copy of port/gpu and port/vcore):
- `port/gpu/src/g10_hw.c`: the vertex builder (fans in drawing order, 12 floats a vertex; the edge lines, 12 floats a vertex with
  the edge's ends and the interior side; GPU_HW_OFF=x,y the shift; counts of dots, lines, wire, translucent, equal, shadow polygons
  with GPU_HW_STATS=1).
- `port/gpu/shaders/g10_hw.vert/.frag` (the triangles; the fragment reads the attachment through a Vulkan-GLSL subpass input that
  SPIRV-Cross `--msl-framebuffer-fetch` turns into `uint4 iS [[color(0)]]`, MSL 2.3), `g10_clear.frag` (the rear plane),
  `g10_edge.vert/.frag` (the lines), `g10_pack.frag` (4 samples). `tools/gpu_msl.py` lists them.
- `port/gpu/src/gpu_mtl.m`: GPU_HW=1 (or `gpu_mtl_hw(1)`) swaps pass S + R for one render pass into pass R's layer: the clear
  draw, the triangles, the lines; at k = 2 into a 2N RGBA32UI target and the pack pass into the four layers. `pipe_render_vs`
  makes a pipeline with its own vertex function (the buffers found by reflection, `hwV`, `hwU`).
- `port/gpu/tools/hwdiff.c`: `hwdiff FILE.vcs OUT [--scale N] [--aa 1|2] [--time K] [--crop x,y,w,h]`: both renderers in one
  process, the differing pixels by class and size, OUT_exact/_hw/_diff.png (and 3 x crops), `--time`: the min GPU ms of each side's
  R(+S) and P(+E).
- `time.sh`, `offsets.sh`, `sweep.sh` (every capture at 3x, AA off and 4 samples: `out/sweep/stats.txt` and the pictures),
  `ios_build.sh` (the bench app with the prototype from the worktree, test-only).

### The data (M2, Metal, Resolution 3, min of 30, load 3-4 with other agents' builds)

| capture | exact R+S | hw triangles+lines | exact P+E | hw P+E |
| --- | --- | --- | --- | --- |
| town, AA off | 1.13 | 0.31 | 0.35 | 0.21 |
| town, 4 samples | 1.95 | 0.88 | 0.59 | 0.31 |
| battle, AA off | 0.89 | 0.28 | 0.07 | 0.05 |
| battle, 4 samples | 2.24 | 1.07 | 0.14 | 0.16 |

P+E is cheaper on the hw side because fewer pixels carry the edge flag (the lines mark one pixel an edge where the DS marks runs),
so more pixels take pass P's fast path: not a saving to count on.

### The picture (37 captures: aa2pass town and battle, caps, aas/vcs, aas/vcsb; 3x)

| | differing pixels | of those: edge / translucent / other polygon / same polygon | colour step 1 / 2-3 / 4-7 / 8+ (of 64) |
| --- | --- | --- | --- |
| AA off | mean 11.3 % (8.0-15.8) | 16 % / 22 % / 0.3 % / 62 % | 35 % / 33 % / 23 % / 10 % |
| 4 samples | mean 14.9 % (11.2-24.7) | 4 % / 25 % / 0.0 % / 71 % | 59 % / 22 % / 10 % / 8 % |

Alpha differs on 0.6 % of the differing pixels. Where: the battle backgrounds' vertical gradient (the DS's per-row integer edge
colour makes 1-step bands at other rows), textured ground (the den quirk: a texel of shift at a span's far end), and the outlines.
With 4 samples the extra is the coverage formula on the 2N grid and the resolve of it.

### Pictures (the scratchpad's hwraster/out/)

- `town3e_exact.png`, `town3e_hw.png`, `town3e_diff.png` (town at 3x, AA off; diff: the exact picture darkened, red = colour
  differs, brighter = larger, yellow = alpha only) and `town3e_crop_{exact,hw,diff}.png` (the house, 3 x magnified).
- `sweep/<capture>_aa{1,2}_{exact,hw,diff}.png` for every capture, `sweep/stats.txt` the numbers.

## 3. The plan

Phases (Metal first; the Mac's gpu_render and hwdiff are the test bed, the phone's A/B the measure):
1. **The core (2 days).** port/gpu: `g10_hw.c` as a real file (the fan and line builders in the worker beside the geometry, no
   span records and no pass S when the option is on), the five programs, the Metal pass; 1-dot and line polygons as points and
   lines, wire-frames as lines without the fill; `gpu_render --hw` and hwdiff committed as tools. Exactness checks: the plot rules
   are shared code (no new rules), so the check is hwdiff's numbers per capture (the class and step table above) kept in
   docs/gpu-cost.md's style, plus the in-run picture checks of the gpu path.
2. **The option (1 day).** Settings: `Renderer3D=exact|fast` in sim_config.ini (the ini reader, port/bridge: the prep skips the
   spans, the core takes the hw pass); the desktop Video window: a "3D renderer" combo (Exact: the DS bit for bit / Fast: the GPU's
   own rasterizer, slightly off at edges); the iOS sheet: the same row under Video (the maintainers' notes "Settings sheet"), applied
   live as Resolution is. The Metrics Breakdown shows the pass as "3D (hw)" in pass R's slot.
3. **Four samples (1 day).** The 2N supersampled form (above) under the existing Antialiasing=4 samples; SMAA works as it is
   (one sample).
4. **GL (2-3 days, after the Metal numbers are in).** GLES 3.0 + EXT_shader_framebuffer_fetch (Android; the same GLSL with
   `inout` on the output); desktop GL: the plainer form (below) or ARB_fragment_shader_interlock where present; else the option
   is greyed out and the exact renderer stays.

Risks: (a) the phone's proportion may differ from the M2's (the maintainers' notes "iPhone bench": the M2's wins did not transfer
before), hence the A/B first; (b) programmable blending serializes fragments per pixel: on a heavy translucent scene (many layers
over the same pixels) the GPU may lose the parallelism, which the counters cannot show on the A18 (ablate by drawing the opaque
polygons only); (c) the edge-marking outline differs visibly on slanted edges (one pixel thinner, no runs): acceptable for "Fast",
not for the default; (d) a line-primitive polygon (particles) drawn as a point may be missed at N = 1 if the point size is not 1.

What the CPU saves (the port's bridge, per drawn frame): the span records' build and upload are gone already (pass S); what
remains of the prep is per polygon (p16's setup for pass S's table, the bins' lists, ~0.1 ms at town) and is replaced by the fan
builder (~0.05 ms). The in-betweens save the same; the geometry (0.7 ms) stays.

Platform parity: Metal (macOS, iOS) the form above. Android (GLES 3.0): EXT_shader_framebuffer_fetch is on Adreno, Mali and
PowerVR; the shader is the same GLSL (an `inout uvec4` output). Windows/Linux (GL 4.x): no framebuffer fetch; the plain form is
the hardware depth test (LESS; equal-mode polygons and shadow masks with ALWAYS), hardware blending for translucent polygons
(the DS's (A+1)/32 weights are not GL's A/255: a 1-step difference per channel), the polygon-ID rules dropped (the same-ID
translucent rule: a visible double blend on overlapping translucent quads of the same ID, e.g. the battle bases' rings), the edge
flag from the lines into a second attachment, pass P on it: further from the DS. ARB_fragment_shader_interlock (NVIDIA, Intel) gives
the Metal form through image load/store; AMD's GL lacks it. Vulkan would unify it (docs/gpu-backends.md C) but is not on the table.

## The phone (pending: the maintainer needed the phone back before the run)

The bench app with the prototype is built and signed (test-only, never merged) in the study worktree:
`<worktree>/build/port/ios/bwport.app` (made by the scratchpad's
`hwraster/ios_build.sh` from that worktree with the prototype files copied into port/gpu; the files were reverted after the build,
so a rebuild needs them copied again from the scratchpad's `hwraster/port/gpu`: shaders/g10_*, msl/g10_*, src/g10_hw.c,
src/gpu_mtl.m, tools/gpu_msl.py). The A/B to run (one round, the full perftest from the town save: the passes rows at gpu 3x AA
off and 4 samples, the Monitor + Smooth row, instructions and cycles a drawn frame), from that worktree:

    BW_TEAM=ZB4S5KC5XK tools/ios_device_ab.py --rounds 1 \
      --app-a build/port/ios/bwport.app --app-b build/port/ios/bwport.app \
      --env-a GPU_HW=0 --env-b GPU_HW=1

Expected if the M2's proportion holds: pass R + S 7.7 ms -> ~3.5 ms with 4 samples and 4.5 -> ~1.3 ms with AA off at gpu 3x
(the brief's phone numbers: R 6.6 + S 1.1, AA off R 3.45 + S), E/P and the rest unchanged; GPU busy at Monitor + Smooth
from ~96 % to ~60 % with 4 samples. The picture checks of the prototype on the phone are the same hwdiff numbers (the shader
is the same MSL); only the time is new.

## The build (2026-11-03): "3D renderer: Exact / Fast", Metal

Built from the prototype into the app as a setting; the exact renderer stays the default and is unchanged (gpu_render: 444 of 444
pictures byte-identical, GL and Metal, 1x and 3x, AA off / 4 samples / SMAA).

- **The setting.** `Renderer3D=exact|fast` in sim_config.ini (default exact), `BWPORT_RENDERER3D=fast|exact`, cfgscript
  `renderer3d=exact|fast`, the Video window's "3D renderer" row under Renderer, the iOS sheet's "3D renderer" row under Display
  (SIM_UISetting `renderer3d`); libntr-ios-fast3d.patch. Applied live: the next gathered frame. The first frame that wants it draws
  exact while the render thread makes the pipelines (one hitch of 0.1-0.6 s on the M2, the MSL compile); after that switching is
  free. Fast draws with the GPU renderer on Metal only: on GL (Windows, Linux, Android, Apple with Graphics API OpenGL) the row says
  "Metal only for now" and the exact renderer draws (2026-11-04: GL and GLES too where the context allows it, "GL" below).
- **The code.** port/gpu/src/g10_fast.c (the CPU half: the polygon table and the triangles, in the worker where the spans were;
  no span records, no band lists, no pass S), g10_fast_mtl.inc (the Metal pass in pass R's slot, included by gpu_mtl.m),
  shaders/g10_fast.vert/.frag, g10_clear.frag, g10_pack.frag (msl/ generated as the others), tools/gpu_fastdiff.c (the study's
  hwdiff: both renderers on one capture, `--time K` interleaved, `--probe x,y,w,h` the raster words).
- **What changed from the prototype.** (1) The edge flag and coverage come from each filled polygon's edge functions in the
  fragment (an edge less than a pixel away along its minor axis: a 4-neighbour outside), three per fan triangle as flat varyings;
  the line pass is gone. Two DS rules found on the way: the bottom vertex's row is not drawn (the GPU's top-left rule agrees, so
  the "bottom row" item is closed) and the last drawn row is all edge pixels, so a right or bottom edge's last pixel is up to one
  pixel away; a flat top's row is not edge pixels (without that rule edge marking drew thin dark lines across the battle sky's
  strips). (2) With 4 samples each fragment shades at its output pixel's first sample (derivatives back to the 2x2 quad's first
  fragment), as pass R shades once a pixel: 14.9 % -> 8.2 % of pixels differ, and pass P's "alike" fast path holds (P+E as the
  exact renderer's instead of +40 %). (3) 1-dot polygons (blocks, Dot=block/fine), line polygons and wire-frame edges (bands one
  DS pixel or one N pixel wide, Line=wide/thin, every pixel an edge pixel) are drawn; Filter=bilinear has its own program.
- **The picture** (gpu_fastdiff over the 37 captures at 3x; the session scratchpad's fast3d/out/keep: stats.txt, summary.txt,
  town, battle and the battle-transition capture as _exact/_fast/_diff, the house crop, room crops from the game):

| | differing pixels | of those: edge / translucent / other polygon / same polygon | colour step 1 / 2-3 / 4-7 / 8+ (of 64) |
| --- | --- | --- | --- |
| AA off | mean 11.2 % (8.0-15.7) | 16 % / 22 % / 0 % / 62 % | 35 % / 33 % / 23 % / 9 % |
| 4 samples | mean 8.2 % (6.2-14.2) | 7 % / 30 % / 0 % / 63 % | 46 % / 26 % / 19 % / 9 % |

  Town at 3x: 10.4 % (AA off), 6.9 % (4 samples); battle 10.4 %, 7.8 %. The worst is the battle transition's radial wedges
  (texel shifts along long thin polygons, 15.7 %). In the game the room, town, menus and a wild battle look the same as Exact at
  the maintainer's viewing size; the outlines match (the prototype's were dotted).
- **The cost** (M2, Metal, gpu_fastdiff --time 100 interleaved, min of two runs; load 10-12 from other agents, so the GPU ran
  slower than in the study: compare within a row):

| capture, 3x | exact R+S | fast | exact P+E | fast P+E | per frame (R+S+E+P) |
| --- | --- | --- | --- | --- | --- |
| town, AA off | 2.22 | 0.83 (-63 %) | 0.63 | 0.64 | 2.85 -> 1.47 (-48 %) |
| town, 4 samples | 2.50 | 1.55 (-38 %) | 0.76 | 0.78 | 3.26 -> 2.33 (-29 %) |
| battle, AA off | 1.27 | 0.52 (-59 %) | 0.08 | 0.10 | 1.35 -> 0.62 (-54 %) |
| battle, 4 samples | 1.91 | 1.33 (-30 %) | 0.14 | 0.16 | 2.05 -> 1.49 (-27 %) |

  The CPU side (the prep: triangles instead of the memoised span setup): 0.07 -> 0.05 ms town; in the game (town, gpu 3x,
  passes=0 bench rows interleaved) 12.6 -> 12.5 M instructions a drawn frame with AA off, 12.7 -> 12.7 with 4 samples: the spans
  were already cheap (pass S on the GPU, the memo), so Fast is a GPU saving. The 4-sample gain is smaller than the prototype's
  (-55 %): the exact renderer's pass R got cheaper since, and Fast runs four fragments a pixel.
- **Checked.** MTL_DEBUG_LAYER=1 (and GPU validation in gpu_fastdiff): no message; live switches between Exact and Fast against
  Resolution 1-4, AA off / 4 samples / SMAA, Smooth, Renderer vcore/gpu, Filter=bilinear (3 rounds under the debug layer, 6
  without: RSS 255-277 MB, flat after the first round); a play-through from the route1 save with Fast at 4 samples (the menu, the
  grass, a wild battle); the iOS device target builds. Not checked: shadow polygons, wire-frames and line polygons in the game
  (none in the captures), the Simulator, the phone.
- **The phone A/B** (pending): `tools/ios_device_ab.py --rounds 1 --ini-b 'Renderer3D=fast'` (or `--env-b BWPORT_RENDERER3D=fast`)
  with one app: the passes rows at gpu 3x AA off and 4 samples (pass R's slot), the Monitor + Smooth row (GPU busy), and a look.

## Round two (2026-11-04): 4 samples at one fragment a pixel, the polygon kinds the captures lack (Mac)

- **Where Fast's frame goes** (gpu_render --time 50-200 --api metal --compose gpu, town at 3x, min ms, Exact and Fast interleaved;
  load 2.3-4: other agents' GPU work moves the mins by up to 3x between runs, so only rows of one run compare). AA off: Fast's R
  slot 0.28 (Exact R+S 0.73), P 0.27 with E 0.14 in it, compose 0.45, SMAA (when on) 0.10. 4 samples: R 1.13 (Exact 1.56), P 0.59
  (E 0.15), compose 0.45. Fast at 4 samples was four times its AA-off slot: four fragments a pixel on the 2N grid plus the pack
  (0.13). Throwaway variants of that shader (same run): without g_texture 1.55 -> 0.83, without the pack 1.42, so the texture
  decode, done four times a pixel with the same coordinates, was half of it. Smooth's in-betweens and the present were not
  re-measured (unchanged by this round; docs/gpu-cost.md).
- **Hardware MSAA** does not fit the plot rules: they are per-sample state (depth test, IDs, stencil, blending), so the fragment
  reads every sample's state; with Metal's programmable blending a fragment that reads a multisampled attachment runs per sample,
  which is what the 2N grid already costs, and an integer attachment has no hardware resolve. Coverage from the rasterizer with
  shading once a pixel needs the fragment to see its sample mask and all four samples' state, which programmable blending gives
  only per sample. So the coverage is now tested in the fragment instead: **g10_fast4.vert/.frag** (with g10_clear4.frag), one
  fragment per output pixel for its four samples. The vertex program pulls the fan triangle's three vertices, grows the triangle
  by three quarters of a 2N pixel (every pixel with a sample inside gets a fragment) and passes flat: the triangle's three edge
  functions in integers (twice the 2N grid: exact), its planes (z, 1 / w, colour / w, s / w, t / w over the screen) and the
  polygon edges as before. The fragment takes the samples inside by the top-left rule (the rasterizer's, so the same samples as
  the 2N grid's fragments got: the 60 synthetic scenes below give byte-identical stats), shades once at the first sample (as pass
  R; before, the 2 x 2 quad's derivatives stepped back to it), runs g_plot4 on the four samples' state (pass R's own 4-lane plot)
  and writes the four as four attachments (RGBA32UI layers of one N-size target, 64 bytes a pixel; the pack reads the layers). A
  fragment with no sample inside, or none passing the early depth test, discards. AA off is unchanged (g10_fast.frag, its dead
  4-sample branch removed).
- **Cost** (gpu_render --time 100-200, min ms, the build before and after interleaved in one run, 3 reps each, load 3-4):

| capture, 3x, 4 samples | R slot before | after |
| --- | --- | --- |
| town / t1401 | 1.13-1.14 / 1.13-1.14 | 1.12-1.13 / 1.10-1.11 (-1 to -3 %) |
| battle | 0.95-0.97 | 0.84-0.88 (-10 %) |
| bf001500 / bf002500 / bf003500 (the battle transition, battle) | 1.51-1.52 / 0.96 / 0.94 | 1.38 / 0.84-0.85 / 0.83-0.84 (-9 to -12 %) |

  Less than one shading a pixel promised: the new fragment is heavy (throwaway variants, town: hit test and the loads alone 0.25,
  without g_texture 0.72, one sample instead of four 0.71, all of it 1.12): the four samples' plot and the texture decode in one
  fragment hold many registers, so fewer fragments run at once. Pass P after it is the same (0.57-0.60).
- **The picture**: over the 37 captures, 4 samples 1,342,413 -> 1,341,977 differing pixels against Exact (32 captures better, one
  worse by 1 pixel; mean 8.20 % as before), AA off byte-identical. Exact: picsab2 444 of 444 identical. Filter=bilinear (its own
  program, g10_fast4.frag#1f), Dot=fine with Line=thin (town): stats as before. MTL_DEBUG_LAYER=1: no message. In the game (the room save,
  gpu 3x, a cfgscript under the debug layer): live switches Fast / Exact against 4 samples, AA off, SMAA, Resolution 2 and
  Renderer vcore and back, no message, the room as Exact draws it (the scratchpad's fast3d2/keep). The iOS device target builds.
- **E and P after Fast**: not changed. Pass E's stage 0 marks an edge-flagged sample with a 4-neighbour of another ID and a greater
  depth: it needs the neighbours' final state, which a polygon's fragment does not have (later polygons change them). With 4
  samples it could move into the pack (it reads the four samples; the neighbours are two more fetches each), saving one of E's
  three draws, about 0.05 ms of E's 0.15 at 3x: not done, small next to compose (0.45) and R.
- **Coverage: the polygon kinds the captures lack.** gpu_fastdiff now prints each frame's shadow masks, shadows, wire-frames,
  lines and dots (the 37 captures: none but lines, 0-7 in town). The game scenes with them were not found in the saves' areas, so
  synthetic scenes (the scratchpad's fast3d2/mkcov.py, from GBATEK's rules with vcore_gen.py's helpers: an opaque ground of
  triangles, shadow volumes as a mask behind the ground and a shadow in front, wire-frames, line polygons (sub-pixel thin
  triangles), 1-dot ones with POLYGON_ATTR bit 13 and DISP_1DOT_DEPTH 0x7FFF; edge marking, antialiasing and fog random), 12 of
  each kind, 3x, differing pixels against Exact (AA off / 4 samples):

| scenes | ground only | + shadows | + wire-frames | + lines | + dots |
| --- | --- | --- | --- | --- | --- |
| differing | 0.60 % / 0.67 % | 0.66 % / 0.75 % | 1.22 % / 1.43 % | 0.83 % / 0.94 % | 0.60 % / 0.67 % |

  Shadows draw where Exact draws them (the volumes' inside is the same; +0.06 % on their edges). Dots match. Lines: Exact draws a
  Line=wide line as DS-pixel blocks (a staircase), Fast as a band one DS pixel wide but not on the DS grid (smoother). Wire-frames:
  Exact draws an X-major wire-frame edge as one DS pixel a row (gaps between rows, as p16's edge rule), Fast as a continuous band.
  Both are close at viewing size; matching them would mean the DS's per-row edge sampling in the fragment for bands (flat edge
  data, a DS-row test): not done, the game's captures have no wire-frames. vcore_gen.py's random p16 scenes differ much more
  (up to 35 %): their quads are not planar (random z, w per vertex), which the DS interpolates along edges then rows and a fan
  cannot; the game's quads are planar.
- **Item 4 (closer to Exact cheaply)**: the slanted-edge and soft-edge differences come from the DS's own edge x per row (p16:
  a Y-major left edge starts at floor(x) for dx >= 0 and ceil(x) - 1 for dx < 0, one pixel left of the rasterizer's ceil(x)
  where x is not whole; a right edge ends one short). Moving the coverage one pixel left (vertices at -0.5 instead of +0.5 in x,
  the attributes moved back by dFdx) helps flat-coloured triangles (shadow scene 0.97 -> 0.69 %) but not the game (town 10.41 ->
  10.79 %, battle 10.39 -> 10.52 %; the edge class grew): the DS's X-major rules (sampled at the row's bottom, truncated
  slopes) differ again. Not kept; the house's outlines at 3x crop the same as Exact's.
- **For the phone**: the passes rows at gpu 3x 4 samples, Fast (pass R's slot): the Mac says -1 to -12 %; the fragment is heavier
  (registers), which may weigh differently on the A18; that A/B decides whether g10_fast4 stays (the 2N-grid program is the build
  before this round). Also the look of a wild battle with Fast at 4 samples.

## GL (2026-11-04): Fast on Windows, Linux, Android and Apple's GL

The same pass for GL and GLES (port/gpu/src/g10_fast_gl.c, in pass R's slot of g00_frame.c's GL frame): the rear plane, the
triangles of g10_fast.c; pass E, P, compose and SMAA unchanged. The fragment programs are Metal's own, only the way they read the
pixel's state differs. AA off: g10_fast.frag, with shaders/g10_fastgl.vert (GLES 3.0 has no storage buffers: the vertex as three
attributes, the edge functions from a data texture, and GL's depth range: z_ndc = 2 z / 2^24 - 1 so that gl_FragCoord.z is the
DS's z / 2^24 as on Metal). 4 samples: round two's g10_fast4.vert/.frag and g10_clear4.frag (one fragment per output pixel, the
four samples' coverage in integers on the 2N grid) into a 4-layer target and g10_pack.frag into pass R's layers, as on Metal;
g10_fast4.vert reads the triangle's vertices from a data texture by gl_VertexID on GL (#ifndef GPU_SPIRV) and does not flip y.

- **The paths, chosen at the first frame that wants Fast from the context's extensions** (`gpu_info` prints the choice):
  1. **fetch**: GL_EXT_shader_framebuffer_fetch, GLES or desktop: `g10_fast.frag#1+G10_FETCH`, the state as the output's old
     value (`inout`), as Metal's programmable blending. Android (Adreno, Mali, PowerVR list it), Apple's GLES (iOS with Graphics
     API OpenGL, the Simulator), Mesa on desktop.
  2. **ilock**: desktop GL 4.5 with GL_ARB_fragment_shader_interlock (NVIDIA, Intel, Mesa): `+G10_ILOCK`, GLSL 4.50, the state in
     an image (imageLoad/imageStore on pass R's layer or the 4-sample grid), the shader's main inside begin/endInvocationInterlockARB
     (pixel_interlock_ordered: primitive order per pixel), a framebuffer without attachments, glMemoryBarrier around it.
  3. **iorder**: the same with GL_INTEL_fragment_shader_ordering (AMD's Windows driver may list it: to check).
  4. **none**: the exact renderer draws and the Video window's 3D renderer row (Android: the settings footer, iOS: the sheet's
     footer) says why. macOS's GL is 4.1 with neither, so **Fast stays Metal-only on the Mac's GL** and the row says "Not on this
     GPU (OpenGL 4.1 Metal ... has neither framebuffer fetch nor 4.5 with images and fragment interlock)". GLES 3.0 devices without
     the extension stay on Exact. The lossy two-pass fallback (hardware depth and blending, further from the DS) was not built:
     where neither exists the exact renderer draws.
- **The edge rule.** The DS draws a polygon's top row and left column, not its bottom row or a vertical right edge; the
  vertices sit on pixel centres (g10_fast.c), so every horizontal edge passes through pixel centres and the hardware's tie rule
  decides. Metal's top-left rule in its y-down framebuffer is the DS's; GL leaves the rule to the implementation and its window
  is y-up (DS row 0 at GL row 0 here). A probe draw at the first use (a square and a triangle into a 16 x 8 target) checks it;
  Apple's GL 4.1 and Apple's software GLES fill exactly the DS's pixels. A GL that breaks the tie the other way gets no Fast
  ("the rasterizer fills an edge through a pixel centre otherwise than the DS") rather than a y-flipped draw nobody could test.
  With 4 samples g10_fast4.frag applies the top-left rule itself in integers, so the probe is for AA off only.
- **Shared files.** shaders/g10_fast.frag, g10_fast4.frag and g10_fast4.vert: preprocessor only (G10_VARY: GLSL ES 3.00 and 3.30
  have no locations on varyings; G10_LOAD / G10_LOAD4; G10_DISCARD, a return in the image modes; the image main at the end; the
  vertex shader's data textures): the MSL is the same text but its digest line (msl/g10_fast*.metal regenerated); Metal's Fast
  pictures are the ios build's (gpu_fastdiff, 37 captures at 3x, AA off and 4 samples: 74 of 74 identical).
  gpu_gl.c: "+DEF" adds the extension line after #version (4.50 for the image modes); 24 cached programs (16 before). g00_frame.c:
  the GL frame's fast branch (no pass S, records, bands or lists), gpu_fast_how; gpu_spans_check checks exact records even with
  Fast on (before, its prep had none to compare). libntr-ios-fastgl.patch: SIM_Renderer3DFast / SIM_Renderer3DFastState and the
  row's text; the bridge reports what the core found.
- **Tested here.** The Simulator's GLES (Apple Software Renderer, OpenGL ES 3.0 APPLE-24.0.2) has the extension: the tools run in
  it through a headless EAGL context (port/gpu/src/gpu_eagl.m, built into port/gpu's own project for the Simulator only; `meson
  setup B port/gpu --native-file port/machine/ios-sim.ini` plus a native file with cmake, pkg-config and sdl2-config set to false,
  then `xcrun simctl spawn DEVICE B/gpu_fastdiff CAPTURE OUT --api es`). Results (3x; Metal = the Mac's Metal core on the same
  capture; the last two columns compare the GLES and Metal pictures of one renderer):


| capture, 3x | AA | Metal: fast vs exact | GLES: fast vs exact | fast: GLES vs Metal | exact: GLES vs Metal (control) |
| --- | --- | --- | --- | --- | --- |
| town | off | 10.41 % | 10.43 % | 567 px (0.13 %), max step 219 | 27 px (0.006 %), max 45 |
| battle | off | 10.39 % | 10.42 % | 268 px (0.06 %), max 239 | 0 px |
| aas/vcsb/f002500 | off | 11.88 % | 11.93 % | 333 px (0.08 %), max 211 | 0 px |
| town | 4 samples | 6.92 % | 6.91 % | 4 px (0.001 %), max 12 | 73 px (0.017 %), max 37 |
| battle | 4 samples | 7.84 % | 7.84 % | 4 px (0.001 %), max 17 | 9 px (0.002 %), max 8 |
| aas/vcsb/f002500 | 4 samples | 8.34 % | 8.34 % | 3 px (0.001 %), max 9 | 3 px (0.001 %), max 4 |

  (On ios after round two, 2026-11-04.) So on GLES Fast is the Metal Fast picture but for 0.06-0.13 % of the pixels with AA off
  (single pixels on polygon edges: two rasterizers' float interpolation, as the exact control shows a few too) and 3-4 pixels
  with 4 samples, where g10_fast4 tests coverage in integers and takes its attributes from planes, so nothing is left to the
  rasterizer. (Before round two the GL 4 samples drew g10_fast.frag on the 2N grid and stepped back to the 2 x 2 quad's first
  fragment with dFdx/dFdy, which GL leaves to the implementation: 0.2-0.7 % of the pixels differed from Metal; that path is
  gone.) In the app (Simulator, Graphics API OpenGL, Renderer gpu, Resolution 2, the town save,
  cfgscript `renderer3d=fast`): the log says `gpu: the fast 3D renderer on GL: framebuffer fetch`, the town looks as with Exact,
  and a drawn frame took about 10 s against 20 s for Exact (a CPU rasterizer: says nothing about a phone's GPU). The Mac app with
  BWPORT_PRESENT=gl: `gpu: the fast 3D renderer on GL: OpenGL 4.1 Metal - 91.7 has neither framebuffer fetch nor 4.5 with images
  and fragment interlock; the exact renderer draws`, and it draws exact; with Metal Fast draws as before. Exact on the Mac (GL and
  Metal, gpu_render 1x and 3x, AA off / 4 samples / SMAA, 37 captures): 444 of 444 pictures byte-identical to the build before.
  No GL timing was possible on a GPU here (macOS's GL has no Fast path; the Simulator's GLES runs on the CPU).

  The interlock and ordering paths compile in glslang (GLSL 4.50 core; the INTEL builtin is not known to glslang) and were not
  run: no GL 4.5 here. Fetch on desktop GL and on a real GLES driver were not run either. the maintainers' notes "Checks owed"
  lists what the Windows, Linux and Android sessions must run.

## Open items

- The hardware depth buffer as an early reject for opaque polygons: a Depth32Float attachment, LEQUAL, the fragment's own test kept;
  equal-mode and shadow-mask polygons drawn with the test off. Measure on the phone; the prototype runs every covered fragment.
- GL: built ("GL" above); what is left is running it on real drivers (the maintainers' notes "Checks owed") and, if a GL breaks the
  edge tie the other way, a y-flipped draw; a GL without fetch or interlock (AMD's desktop GL without the INTEL extension, a GLES
  3.0 phone without the extension) stays on Exact.
- The first switch's pipeline compile (0.1-0.6 s): compile in the background (newRenderPipelineStateWithDescriptor:completionHandler:).
- The power side: the GPU ms saved is the lever; the maintainers' notes (the power agent) will say what a GPU ms costs in mW on the
  A18 when it exists.

## Round three (2026-11-04): the 4 samples straight into pass R's layers (Mac)

Where Fast's 4-sample cost went, measured first (gpu_render --time, M2, Metal, town and battle at 3x, 4 samples; min ms of 100
back-to-back renders, three reps interleaved, load 1.6-2.0), with throwaway variants of g10_fast4.frag and the pass:

- **Which number is the work.** The in-game passes rows read Fast's R slot at 3.41 ms and gpu_render --time at 1.12: the same
  work at two GPU clocks. gpu_render's new `--gap 16` (16 ms of sleep between the timed renders: the game's cadence, the GPU
  idling between frames as in a passes row, where each pass is a command buffer of its own) gives R 2.65-2.85 mean (min 1.5), P
  1.27-1.40, E 0.27, compose 1.7 against 1.11 / 0.59 / 0.15 / 0.42 back to back: the passes rows are the bottom DVFS clock (2.3-3 x),
  the back-to-back figure the work. Both are quoted below; the in-game rows for the phone's kind of reading, the top-clock mins for
  the work.
- **The parts** (town / battle, the R slot's min ms; the drop against the build before, 1.11 / 0.85):

| variant | R | drop | at the game's cadence (`--gap 16`, town mean of 60) |
| --- | --- | --- | --- |
| no pack pass, the grid's store DontCare (the 64 bytes a pixel never leave the tile) | 0.92 / 0.68 | -0.19 / -0.17 | 2.40 (-0.33) |
| no g_texture (the polygon colour) | 0.82 / 0.61 | -0.29 / -0.24 | 2.19 (-0.55) |
| no g_plot4 (the hit lanes overwritten) | 0.95 / 0.68 | -0.16 / -0.17 | 2.37 (-0.37) |
| no edge flag / coverage (12 dot products) | 0.99 / 0.77 | -0.12 / -0.08 | 2.58 (-0.15) |
| one depth plane instead of four | 1.08 / 0.82 | -0.03 / -0.03 | |
| hit test, depth, the loads and the stores only (no shading, no plot) | 0.40 / 0.31 | -0.71 / -0.54 | 1.06 (-1.67) |
| `--ablate 1`: no triangle at all (the clear, the stores, the pack) | 0.16 / 0.16 | -0.95 / -0.69 | 0.44 (-2.3) |

  So of the 1.11: the pass's floor 0.16 (clear draw, the 64-byte store, the pack), the fragments' hit test and loads 0.24, the
  shading and plot 0.71 (texture ~0.29, plot ~0.16, edges ~0.12, the rest the register weight of all of it in one fragment: the
  parts do not add up to the whole). Registers themselves were not read (Xcode's compiler stats need a GPU capture): the variants
  are the measure. Pass P under Fast is as under Exact: gpu_fastdiff's new line counts the "alike" pixels (sample 0's bit 31) of
  both renderers: town Exact 94.40 % / Fast 94.18 %, battle 97.21 / 96.66 (edge-flagged 5.39 / 5.60, 2.79 / 3.33), so P's fast draw
  holds and its slow draw is the same 5-6 % of pixels either way; P with the layers all alike (a stale-layer variant) is 0.21 at the
  top clock / 0.44 at the game's cadence against 0.59 / 1.34: P's 4-sample cost is its slow draw, pass P's own item (docs/gpu-cost.md), not Fast's.
- **Built: no pack pass, no grid.** On Metal the triangles' pass draws into pass R's own k = 2 layers (RG32UI, g00_raster.frag's
  r_out form) as its four attachments, and the state r_out drops (the translucent ID, F_STENCIL: a byte a sample) into a fifth
  attachment, R32UI, memoryless on an Apple GPU, load and store DontCare: 36 bytes a pixel in the tile instead of 64, nothing
  stored but the 32 bytes E and P read anyway, and no pack pass. Each fragment unpacks the four samples (D, C, F, I as g_plot4
  holds them, the coverage in F's upper bits), plots, repacks the four, and sets sample 0's bits 30 (an edge-flagged sample) and
  31 (alike, the same rule as g00_raster.frag's and g10_pack.frag's) from the four samples' new state, so the layers are final
  after every fragment; a fragment that changes no lane discards. g10_clear4.frag writes the same form. The translucent ID keeps
  6 bits (an opaque write sets 255 and clears F_TRANS, and the same-ID rule tests F_TRANS first, so 63 is safe). GL keeps the grid
  and g10_pack.frag (GLES 3.0 promises four colour attachments; the shader's G10_FETCH and image forms are untouched, the Metal form
  is the `#else`, G10_DIRECT). Also the edge-distance loop evaluates the three edges once and steps (3 dot products instead of 12).
- **Cost** (the build before and after interleaved, min of 100 x 3 reps; `--gap 16` the mean of 60 x 3):

| capture, 3x, 4 samples | R before | R after | at the game's cadence before -> after |
| --- | --- | --- | --- |
| town | 1.10-1.11 | 0.92-0.93 (-16 %) | 2.72-2.85 -> 2.45-2.48 (-11 %) |
| battle | 0.85 | 0.68 (-20 %) | 2.04-2.17 -> 1.85-1.94 (-10 %) |

  P at the top clock is unchanged (0.64-0.68 either way); at the game's cadence it read 0.1 higher after (1.42-1.47 against
  1.32-1.36): the shorter R hands P a lower clock in that mode (each pass its own command buffer); R+P together 4.1-4.2 -> 3.9.
  In the game (the town save, Fast, gpu 3x, 4 samples, Metal, BWPORT_PANELHZ=120, `--noskip`, the pinned bench; before and after
  interleaved, three runs each, load 1.7-2.6): the 60 Hz passes row R 3.35-3.43 -> 2.92-2.95 ms (-14 %), E 0.32-0.33 -> 0.44-0.50, P
  1.32-1.36 -> 1.35-1.38, C 0.37; R+E+P 5.05 -> 4.75 (-6 %); the row's `metal:` renderer 320-328 -> 303-311 ms/s (-5 %). E and
  P read higher for the same reason as at `--gap 16` (the clock each pass starts at, with its own command buffer: at the top clock
  E is 0.17-0.18 and P 0.59 either way), so the work saved is R's. `ablate=r` now works under Fast: R 0.15, E 0.19, P 0.23 (the
  floor of Fast's slot with the layers all alike). AA off unchanged (R 0.84, E 0.39, P 0.38). The own row (Smooth, Monitor 120)
  swings with the in-betweens made (26-57 a second: renderer 532-810 ms/s before, 542-700 after) and says nothing finer.
- **The picture.** The direct form is lossless by construction and gave the same pictures (town, battle: the stats and the 1x
  picture byte-identical to the build before). The edge step moves 1-2 pixels in 8 of the 37 captures (float reassociation: a
  coverage value one step off), 4 better and 4 worse (bf002000, f002000, f002100 +1 pixel, f001600 -2): 4 samples 1,341,977 ->
  1,341,975 differing pixels, mean 8.20 % as before; AA off byte-identical (37 of 37). Exact: picsab2 444 of 444 pictures byte-identical (and vcore 74 of 74). MTL_DEBUG_LAYER=1:
  no message in gpu_fastdiff nor in the game (the room save, gpu 3x: live Fast / Exact switches against 4 samples, AA off, SMAA, Resolution 2 and Renderer vcore and back; the room as Exact draws it, the scratchpad's fast4s/keep). The GL forms of g10_fast4.frag and g10_clear4.frag (fetch, ES 3.00; interlock, GL 4.50) compile in glslang; no GL here runs Fast (macOS's GL 4.1), so the GL pictures were not re-run: its shader text is the same. The iOS device target builds.
- **Tools.** gpu_render `--gap MS` and `--ablate MASK` (the Metal core's GPU_ABL_* bits, as the cfgscript's ablate= key); gpu_fastdiff's edge/alike line; `ablate=r` works under Fast (no triangle drawn: the clear and the stores stay), so the phone can read Fast's floor.
- **What is left of the 4 samples** (town, top clock): R 0.93 against 0.28 with AA off; the floor without triangles is 0.16, the fragment's shading and plot ~0.7. The next cuts would be in the fragment itself (the texture decode once a pixel is already the minimum; the 4-lane plot and the register weight remain), or in pass P's slow draw (0.9 ms at the game's cadence for 5-6 % of the pixels: Exact's problem too).

## Round four (2026-11-04): 4-sample forms for the A18 to choose from (BWPORT_FAST4; Mac only so far)

a test phone (47bafc3df, the same session, passes rows at 4 samples): Fast's R slot 5.94 ms against Exact's 5.64, though AA
off it is 1.43 against 3.15; on the A18 Fast's 4 samples cost ~4.5 ms over its AA-off slot, Exact's ~2.5. The Mac had the direct
form 33 % under Exact. So the 4-sample pass now comes in five forms, chosen at the first Fast frame by the environment
(`BWPORT_FAST4=direct|lite|pre|keep|grid`, default direct; the log says `gpu: BWPORT_FAST4=...` when set; Metal only, GL keeps its
grid of layers and pack), for an A/B on the phone where each form tests one guess about the A18:

| form | what | the guess it tests |
| --- | --- | --- |
| direct | round three (g10_fast4.frag, 5 attachments, 36 bytes a pixel in the tile) | (the base) |
| lite | `g10_fast4.frag+G10_LITE`: g_plot on one sample at a time (unpacked, plotted, packed, written before the next is read), the edge distance and coverage per sample, instead of g_plot4's four lanes and their copies | the fragment's live registers |
| pre | `+G10_LITE+G10_PRE`: also the colour, texture and edge distances before the pixel's state is read | the wait for the pixel's earlier fragments (programmable blending keeps them in order) holding the texture decode |
| keep | `g10_fast4.frag+G10_KEEP`: the direct form with no discard (the old values written back) | discard in a fragment that reads its attachments |
| grid | round one's 2N grid: `g10_fast.frag+G10_GRID` (four fragments an output pixel, the AA-off fragment with its colour and texture coordinates stepped back to the quad's first by dFdx/dFdy) into an RGBA32UI target at 2N, then `g10_pack.frag+G10_GRID` into pass R's layers | lighter fragments and 16 bytes a pixel of tile state (the AA-off form, which the A18 runs 2.2 x faster than Exact) against four of them and a 2N store and load |

- **The pictures** (gpu_fastdiff, the 37 captures at 3x, 4 samples, against Exact): direct 1,341,974 differing pixels, mean 8.20 %, worst
  aas/vcsb/f001600 14.16 %. lite, pre and keep: Fast's picture and --hi layer byte-identical to direct in all 37 (md5). grid:
  1,342,413, mean 8.20 %, worst f001600 14.17 % (32 captures worse by at most 120 pixels, one better: round one's grid exactly, its
  number in round two above). Exact: picsab2 444 of 444 pictures byte-identical, vcore 74 of 74. MTL_DEBUG_LAYER=1: no message
  (gpu_fastdiff, every form; the game, below).
- **Mac timing** (M2, Metal, gpu_render --time 100, R slot min ms, the forms interleaved in one run, 5 reps; load 3-5 with other
  agents' work, town's mins moved between reps by up to 1.5 x, battle's held):

| capture, 3x, 4 samples | direct | keep | lite | pre | grid | exact |
| --- | --- | --- | --- | --- | --- | --- |
| town | 0.93 | 0.99 (+6 %) | 1.06 (+14 %) | 1.41 (+52 %, 3 reps) | 1.11 (+19 %) | 1.43 |
| battle | 0.68 | 0.74 (+9 %) | 0.80 (+18 %) | 0.84 (+24 %, 3 reps) | 0.97 (+43 %) | 1.07 |

  At the game's cadence (`--gap 16`, mean of 60, 3 reps, load 2-3): town direct 2.42-3.01, keep 2.53-3.22, lite 2.84-3.40, grid
  2.78-3.51, pre 3.45-3.94, exact 3.83-4.58; battle direct 1.87-2.20, keep 1.92-2.35, lite 2.03-2.60, grid 2.44-2.97, pre
  2.25-2.72, exact 3.26-3.34. In the game (the town save, gpu 3x, 4 samples, Metal, BWPORT_PANELHZ=120, `--noskip`, the pinned bench;
  two runs a form interleaved, load 2.2-3.2), the 60 Hz passes row's R: direct 2.94-2.99, keep 3.14-3.15, lite 3.36-3.39, grid
  3.41-3.53, pre 4.39-4.45, exact 4.37-4.46 (E 0.52-0.64, P 0.33-0.36 in every form); the row's `metal:` renderer: direct 249-255
  ms/s, keep 261-262, grid 271-282, lite 275-278, pre 332-333, exact 328-340. `ablate=r` (Fast's floor: the clear, the stores, the
  pack): grid 0.46 against 0.15-0.16 (its 2N store and the pack's load: 128 bytes a pixel more traffic).
- **Read on the Mac**: every form is slower than direct here, pre as slow as Exact (the texture decode then runs for every fragment the
  depth test would have dropped: overdraw), so none replaces direct on the Mac. They are kept for the phone: the M2 (Apple 8) and the
  A18 Pro (Apple 9: registers allocated as the shader goes, other ALU pairing) weigh the fragment differently, and on the A18 the AA-off
  fragment Fast's grid is made of beats Exact by 2.2 x while the direct 4-sample one does not. A form wins on the phone only if its R
  slot in the passes rows drops below direct's 5.94 (and Exact's 5.64); if none does, Fast's 4 samples on the A18 stay a loss and
  the next look is at the tile (36 bytes a pixel, over 32) or a hardware-MSAA approximation.
- **Not built: "hw"** (hardware MSAA, shading once a pixel, the rasterizer's coverage and an MSAA depth test per sample, a resolve). The
  plot rules it would give to fixed-function hardware (the translucent same-ID rule, shadow volumes, equal-depth mode, w-buffering
  (fragment depth is per pixel under MSAA), the fog flag's AND, the DS's 5-bit blend) would all become approximations, and the
  resolve into pass R's layers stores and reads 4 samples x (colour + depth + IDs/flags) = 48 bytes a pixel more than the direct
  form's 32: a larger build than this round, with no Mac evidence that the shading is the A18's problem. Worth it only if the phone
  says the per-sample state work itself (lite, keep) is where the A18 loses.
- **The game**: the room save under the debug layer, gpu 3x, each form (and grid, lite, keep with Filter=bilinear): live switches Fast
  / Exact, 4 samples / AA off / SMAA, Resolution 2 / 3, Renderer vcore / gpu and back: no message, no crash, the room as Exact draws
  it (the scratchpad's fast4a18/keep). Twelve cycles of live switches (Fast / Exact, 4 samples / AA off, Resolution 2 / 3) a run:
  the process grows 1.5-2 MB a cycle with grid (183 -> 203 MB), lite (187 -> 211) and the untouched direct alike (196 -> 213), so
  the forms add nothing of their own; what grows under every form is not yet known (open). The iOS device target builds. Every form's MSL is in the app (msl/*_G10_*.metal),
  compiled at the first use like the rest; the iPhone SDK's offline Metal compiler is not installed here, so the phone's compiler
  sees the forms first on the phone.
- **For the phone** (the main session; one app, both sides Fast, the env on side B only; `--pin`: the same game each run): the cfgscript
  `f1300:renderer3d=fast;p1:renderer=gpu,resolution=3,antialias=4,speed=1,framerate=monitor,frameskip=0,smooth=1,ff=0,passes=0,ablate=none;p60:bench=own;p1:framerate=60,smooth=0,passes=1;p60:bench=pas4;p1:ablate=r;p60:bench=pasr;p1:ablate=none,renderer3d=exact;p60:bench=pasx;p5:quit=1`
  (the maintainer's row, the 60 Hz passes row at 4 samples, Fast's floor, and Exact's passes row as each run's own reference) through
  `tools/ios_device_ab.py --rounds 3 --pin --cfg CFG --env-b BWPORT_FAST4=FORM` for grid, keep and lite in that order (pre only if
  lite wins: it is Exact's cost on the Mac). The figure is pas4's R against direct's 5.94 and pasx's R; then own's renderer ms/s.

- **Result and clean-up (2026-11-04)**: on a test iPhone (A18 Pro, 3 rounds each, pinned) pass R's slot at 4 samples: direct
  5.11 ms, keep 4.14 (-19 %), grid 5.54 and lite 5.58 (both +8 to +9 %); pre was never run (slower than lite on the Mac). Metal's
  lite, pre and grid forms are removed (FAST4_LITE / PRE / GRID in src/g10_fast_mtl.inc, their G10_LITE / G10_PRE / G10_GRID code in
  g10_fast4.frag, g10_fast.frag and g10_pack.frag, and their msl/*.metal files, g10_pack.frag's too: Metal has no pack pass now).
  `BWPORT_FAST4` takes `direct|keep` only (an unknown value prints the warning and keeps the default). **keep is the iOS default**
  (commit 91ea2a2e7); direct stays the Mac's (the M2 runs it 6 % faster). GL's 4-sample path (g10_fast4.frag on layers + g10_pack.frag)
  is untouched: the programs' preprocessed GLSL is the same before and after, blank lines aside.

## The gaps between polygons (2026-11-04): Fast with AA off grows the top and left edges

The maintainer saw "mesh lines" in the battle intro (the encounter effect's zoom, ov076, Renderer gpu, Fast, Resolution 2, AA off): a
dotted line of the clear colour along edges where two of the mesh's polygons meet. Only Fast with AA off draws it: Exact and vcore
(the same picture, byte for byte, in the game's shots) and Fast with 4 samples cover those pixels; Smooth motion on or off, the
same. Cause: two neighbours whose shared edge is not quite shared (each polygon's vertex at the screen corner comes from its own
clipping, 255 vs 256 at 1x; a vertex mid-way along another's edge), so the edges diverge by up to a pixel. The DS (p16) takes an
X-major edge's whole run in a row and a left edge's pixel from its floored x, so it leaves no gap; the GPU takes a pixel only when
its centre is inside both, and left single pixels between them.

The fix (src/g10_fast.c grow_poly, move_tri): with AA off each filled polygon's top and left edges (those whose pixel centres on
the line are already drawn, so no pixel on the line itself is added: growing right and bottom edges adds a row or column the DS
leaves out) move out along their minor axis, 7/8 of a pixel if X-major, 1/2 if Y-major; the vertices go where the moved edges
meet (at most 2 pixels out) and take their attributes from their fan triangle's own planes (screen-linear z, 1 / w and the others
over w), so the pixels inside keep their values (a grow of 0 gives the old pictures byte for byte). Vertices slid along an edge that
does not move broke the exact shared edges (float endpoints off the pixel grid: more gaps, not fewer), so a shared edge is always
moved on one side. 4 samples is untouched (no gaps: its samples are on the 2N grid). In g10_fast.frag a pixel outside the polygon's
edges (the grown part) loses to a polygon at its depth drawn before or after it (its depth + 256: as an edge pixel it took the
neighbour's pixel at an equal depth) and keeps coverage 31: the CPU half alone drew black dotted lines across the mesh, its
coverage-0 pixels under the DS's antialiasing (gpu_fastdiff did not see them in its mean: 11.27 % with them).

- **Gaps** (pixels Exact covers and Fast leaves empty: gpu_fastdiff's new `gaps` count; Metal, AA off), over 92
  intro captures (two runs of the route1 save) and the 37: before 148 / 490 / 456 at 1x / 2x / 3x in the intro (14 of 92 captures
  at 2x) and 32 in the 37 at 3x; after 0 everywhere. Sweep (2x, intro / 37 at 3x mean differing): every edge by 1/2: 0 gaps,
  11.42 %; top/left by 1/2: 40 gaps, 11.24 %; top/left 0.99: 0, 11.61 %; X-major 7/8 and Y-major 1/2: 0, 11.27 % (all without
  the fragment's part).
- **gpu_fastdiff**: the 37 at 3x, AA off 11.23 % -> 11.22 % mean differing (8.04-15.74), 4 samples 8.20 % (every line the same);
  the 92 intro captures at 2x, AA off 24.42 % -> 24.42 %, 4 samples 20.19 %. Pictures: introlines-shots/ in ~/bw-cache
  (zoom_f1427_before/after/exact: the dotted line and its absence). Exact: pics_ab 492 / 492 and vcore 82 / 82 identical, plain
  and with GPU_COMPOSE=gpu (the 37 and 4 intro captures; run before the fragment's part, which Exact does not use).
  MTL_DEBUG_LAYER=1 clean (gpu_fastdiff and the game); the GL forms of g10_fast.frag compile in glslang (ES 3.00 fetch, GL 4.50
  interlock); no GL here runs Fast.
- **Not checked**: Smooth motion's in-betweens while walking (the T-junction dots docs/interp.md suspected), the phone.
