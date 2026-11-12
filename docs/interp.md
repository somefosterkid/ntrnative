# In-between 3D frames above 60 fps

Question: can the port show more than 60 frames a second while the game logic stays at 60 Hz, how, and at what cost and input
delay. Research and measurement only: nothing here changes the player. Clean room: the only thing known of the other project
(DS Kart Recompiled) is its README; the design below is ours.

What was built (all off by default, no change to any picture; vcore tests pass):

- `port/vcore/src/p23_interp.[ch]` (new, hooks in p14's `new_vertex` / BEGIN / END and `vc_poly_reset`, one `VC_P23 p23` in
  `VCoreState`): records the vertices a frame's command stream submits (clip coordinates BEFORE clipping, lit colour, texture
  coordinates), pairs the BEGIN_VTXS..END_VTXS lists of two frames (common prefix/suffix, then an LCS on a hash of attr without
  alpha, texparam, pltt, viewport, primitive type, vertex count; then a second pass inside the gaps by shape only), measures the
  screen motion of every pair (each vertex's position clamped to half a screen beyond the edges, the vertices behind the camera
  left out) and drops a pair that moves more than a limit (48 px mean) or crosses w = 0 with no vertex in front in both frames
  (2026-11-04: before, any w = 0 crossing or a mean blown up by a vertex near the camera plane dropped the map's ground pieces
  that reach under the camera, drawn unblended beside blended neighbours: lines of the clear colour between them in the
  in-betweens while walking), and lets the geometry
  engine blend a frame's vertices toward the other frame's (clip x y z w, colour, texcoords) before clipping, culling and the
  viewport transform. The gpu path shares p14, so it gets the same hook.
- `BWPORT_INTERP_STATS=FILE` (port/bridge/vcore_bridge.c, vcore renderer): one line per drawn frame, the 3D structure against the
  previous frame and what changed in 2D (scroll registers, OAM moves, palettes). `port/vcore/tools/interp_stats.py FILE... [--range A B]` turns the logs into the tables (classes below).
- `port/vcore/tools/vcore_interp.c` (`build/vcore/vcore_interp A.vcs B.vcs PREFIX [--f 0.5] [--scale N]`): the offline prototype,
  A's scene with its vertices blended toward B's by f (2D held at A): PREFIX_t.png, _mid.png, _t1.png. Shots:
  `C:\stress\shots\ip75_town_{t,mid,t1}.png` (town walking, frames 2780 -> 2782: 597 lists all paired, motion mean 2.1 px) and
  `ip75_battle_{t,mid,t1}.png` (Bianca's battle, camera move, frames 16424 -> 16425: 17 lists paired, mean 0.7 px, max 233 px).

## 1. Structure match (measured, vcore at 1x, the game running from the saves; frame pair = frame i-1, frame i)

Classes: STILL = every list pairs and the mean motion is < 0.05 px (the 3D did not change); MATCH = all lists pair 1:1 and move
sanely; MOSTLY = >= 90 % of the vertices pair (the rest: lists removed/added/moved > 48 px); PARTIAL = 50-90 %; CUT = < 50 %;
NO3D = the 3D engine produced no complete frame (title before the game starts, 614 frames of every run).

| scene (frames) | pairs | STILL | MATCH | MOSTLY | PARTIAL | CUT | vertices blendable | keyframe gap |
|---|---|---|---|---|---|---|---|---|
| town, walking (f2700-4000) | 1300 | 84.6 % | 12.1 | 3.1 | 0.2 | 0 | 99.8 % | 2 frames: 98 % |
| route 1, walking (f2700-5000) | 2300 | 62.7 | 10.0 | 12.2 | 13.5 | 1.7 | 95.2 | 1: 26 %, 2: 59 %, 4-10: 12 % |
| route 1, idle + menus + battle (f5000-14000) | 9000 | 80.8 | 6.1 | 11.3 | 1.7 | 0.1 | 99.9 | 1: 32 %, 2: 30 %, 4+: 25 % |
| lab (f2700-4300) | 1600 | 82.9 | 0.8 | 14.6 | 1.4 | 0.2 | 99.6 | 2: 52 %, 4-16: 45 % |
| boot: title, intro f0-2200 | 2200 | 51.5 | 1.9 | 12.5 | 5.4 | 0.2 | 97.2 | 1: 93 % |
| boot: intro cutscenes f2200-9000 | 6800 | 86.6 | 11.9 | 0.7 | 0.7 | 0.1 | 99.6 | 1: 28, 2: 39, 4: 23 |
| boot: f9000-16000 (naming, room, starter) | 7000 | 90.0 | 4.5 | 2.8 | 2.4 | 0.3 | 97.3 | 2: 91 % |
| boot: f16000-17500 (the battles, camera moves) | 1500 | 2.3 | 39.2 | 44.1 | 11.1 | 3.4 | | 1: 97 % |

(STILL includes idle seconds after a script ended; the "keyframe gap" is the number of game frames between frames whose 3D
changed.) Findings:

- THE FIELD'S 3D RUNS AT 30 FPS: while walking in town and on route 1, every second frame's 3D is identical to the one before
  (town: 2.1 px of motion on even frames, 0.0 on odd ones, the same list of 597 lists on both). The game logic is 60 Hz but the
  overworld produces a new 3D picture every 2 frames. Battles and the intro run 3D at 60 (gap 1). So in the field an in-between
  frame is also what raises 30 to 60: interpolation pairs the last two DISTINCT frames (keyframes), not frame t and t+1.
- The structure matches almost everywhere: a moving scene keeps the same lists (same primitive types, vertex counts, attr,
  texparam, palette) frame after frame, so vertices pair 1:1. Where it does not (MOSTLY/PARTIAL), the cause is a few lists
  appearing or disappearing at the edge of the view (route 1: 228 + 226 lists over 2300 pairs; the lab: lists added/removed with
  a palette change, 229 pairs, a map animation), an object that teleports (> 48 px mean: 499 of the 629 non-trivial route-1 pairs
  include one; a sprite changing its place, the camera cutting) and a texture/palette/attr change of a list (< 2 %). A camera cut
  inside one map keeps the structure and is caught only by the motion limit. Real CUTs (< 50 % pairable) are 0.2-3.4 %: map
  loads, the battle's first frames, the intro's scene changes.
- Fallbacks that follow: per list (an unpaired or too-far list is drawn as in frame t) is enough for appear/disappear and a
  teleporting object; when more than ~half the vertices fail (CUT) or the 3D flips on/off, show frame t for the whole interval.
- Pairs crossing w = 0 (a vertex behind the camera in one frame only): 142 of 1300 walking pairs in town contain one; the
  list fell back to frame t, which opened lines between ground pieces; since 2026-11-04 such a list is blended (clip space is
  linear, the clipper runs after the blend) unless none of its vertices is in front of the camera in both frames.

## 2. Where to blend

Chosen and prototyped: AFTER the vertex transform and BEFORE clipping: the blended value is the clip-space vertex
(x, y, z, w), which is a straight-line motion of the world point under frame t's projection. Clipping, culling, the viewport and
the sort then run on the blended polygon, so a polygon that crosses the screen edge differently in the two frames needs no
pairing at all (pairing is by submission order inside a list, not by polygon index after clipping). Colour (lighting result) and
texcoords are blended too (a texcoord only when the two differ by < 128 texels: scrolling water wraps). The matrix level
(blending MTX_* parameters) was rejected: a vertex's matrix is a product of several commands (the 4x3 of each bone and the
camera), blending the factors gives a second-order error, lights and normals would need their own handling, and it has to be done
per command (the stream has up to ~600 lists with push/pop). Post-clip pairing (by polygon index) breaks whenever clipping
changes a polygon's vertex count.
Artifacts of a linear blend: a rotation shrinks at the midpoint (chord instead of arc: negligible for the per-frame turns of walking
NPCs and camera pans of 1-3 degrees, visible for a fast spin such as the battle intro or a 180-degree turn: limit by the 48 px
rule or a rotation test on the model matrix), an object entering/leaving pops (as at 60 Hz), a vertex crossing w = 0 (fallback),
shadows/translucent sort order taken from frame t (a polygon that changes depth order pops once per keyframe). The 3D's inputs
that are not vertices (fog table, edge colour, clear colour, toon table, texture contents, palettes) are frame t's.

## 3. 2D

From the same logs (pairs with the 3D on): town walking (f2700-4000): no 2D change at all on either screen (no BG scroll, no
OBJ move, no palette). The overworld's 2D is static; the player, NPCs and map are 3D. Battle (boot f16000-17500): engine A BG scroll
changes in 0.1 % of the pairs, OBJs move in 10.5 % (the HP bars / HUD slide in, up to 248 px), other attr change 3 %, B (the touch
screen) scrolls in 1.5 %, moves OBJs in 1.9 %. Hold the 2D at frame t: the 2D is 60 Hz content that is steady or sliding in
short transitions; blending the BG scroll registers and OBJ positions linearly between the two frames is safe for slides but
not worth the complexity. The touch screen is engine B and is never interpolated.

## 4. Timing and input delay

How the loop works today: libntr's `SIM_Render` (sim_main.cpp) runs ON THE GAME'S THREAD from the V-blank wait (PORT_WaitIrq):
it gathers/draws (the renderer's frame call), runs the V-blank interrupt, ImGui, `SDL_GL_SwapWindow`, then the 60 fps cap sleeps
(`SDL_Delay`, then a spin for the last 2 ms) until 16.715 ms since the last frame's end, then returns to the game. The game
logic is 1-4 ms of that 16.7 ms; the rest of the time the game thread is in the cap's wait. vcore draws on a worker a frame late
(`vcore_bridge.c`: call k gathers frame k, returns picture k-1; BWPORT_VCORE_PIPE=0 draws in the call).

Smallest change that presents at the display's rate: keep the game at 60 Hz and turn the cap's wait into a present loop. After
the swap of the real frame, for the other M-1 sub-frames: wait until the sub-frame's time (SDL_Delay + spin as now), show the
in-between picture (already drawn by the worker: one texture upload + one quad + swap, the same ~1 ms present as today), and
swap. The game thread presents between its logic bursts, so the sub-frames before the logic are on time and only a long logic
frame delays the first present of the next one. No second window thread, no GL context hand-over. With vsync on, the swap
interval must be 1 and the cap loop must not sleep (the vsync is the clock): M is the display rate / 60 (rounded), sub-frame i
shows t = i / M. A display that is not a multiple of 60 (144 Hz) needs a time-based t (blend factor from the present clock)
rather than a counter, which also fixes jitter.

Input delay (content shown vs the game's latest state):
- To show anything between keyframes K_i (game frame a) and K_{i+1} (frame b = a + gap) the game must have computed K_{i+1}.
  The interval is shown while the game computes the next frames, so the picture is always `gap` game frames behind the newest
  keyframe: gap 1 (battles, intro, menus): 16.7 ms; gap 2 (the overworld's 30 fps 3D): 33.3 ms. That is +0 ms against today's
  vcore (it is already one frame late through the pipeline) for gap 1, +16.7 ms for the field; against the gpu path (no
  pipeline) +16.7 / +33.3 ms. The worker needs the next keyframe's state gathered (known at the call) and has the whole
  interval to draw the mids and the real frame: a mid shown 1/M of an interval after the call must be drawn in that time
  (8.3 ms for M = 2, 4.2 ms for M = 4 at 1x), or the interval is played one interval later (+1 gap of delay).
- Extrapolation (predict K_{i+1} from K_{i-1}, K_i) adds no delay but overshoots at every stop/turn (the player stops mid-tile,
  the camera eases, a menu opens): visible rubber-banding; not recommended.
- The DS itself shows the field 30 fps, so the real game has the same 0-1 frame of quantization.

## 5. Cost (vcore, per EXTRA frame: only the 3D is redrawn, the 2D reused)

Measured with `vcore_render --bench` on corpus frames, 8 CPUs pinned (taskset 4-11) on the shared pool with other agents running,
so the numbers are noisy by 20-50 % (CPU times are steadier than the wall). ms of 3D per frame (geometry + clear + raster + post),
wall:

| frame | 1x, 1 thread | 1x, 8 bands | 2x, 8 bands (N x raster) | 4x, 8 bands | 4x, 14 bands |
|---|---|---|---|---|---|
| town (597 lists, 322 polygons) | 7.7 | 3.5 (raster 2.4 + geometry 0.7 + post 0.3) | ~19 (raster 17) | ~45 | ~14 (raster 13) |
| battle | 2.9 | 1.1 (raster 0.8) | ~3.3 (raster 2.9) | ~11 (raster 10) | |

A midpoint frame also pays the blend itself (one vertex copy: noise) and the picture stage (2x: 1.5-2.5 ms, 4x: 3-17 ms: the
N x N compose of the 3D with the 2D). To go from 60 to 240 fps the worker must draw 3 extra pictures + 1 real one every 16.7 ms:
at 1x ~14 ms of wall with 8 threads for the town (all 8 threads busy: about 5-6 cores of work), fine on the maintainer's 14700KF-class
CPU, tight on 4 cores; at 2x ~76 ms: not possible; at 4x ~180 ms (raster 4 x 13 ms even on 14 bands): not possible on vcore.
240 fps is therefore a 1x (or gpu) feature; 120 fps (one extra) works at 1x and at 2x on >= 8 cores for light scenes. The gpu path
pays its CPU geometry (0.7 ms town) per extra frame plus one GL raster pass; not measured here (no GPU on this host: llvmpipe
timings say nothing).

## 6. Prototype and what it showed

`vcore_interp` renders the midpoint of two real captured frames (town: the walk of the whole map 2.1 px, the battle: the camera
sweeping the trainer). The midpoint is a correct picture (checked by eye; all three differ; no tearing, polygons clip normally).
Frames came from BWPORT_VCAPTURE (frames 2776-2790 of the town run and 16380-16470 of the boot run).

## 7. vcore at N x across cores

N x IS split like the 1x bands: p20 raster_n cuts the N x rows into strips of equal estimated cost (2 per thread) on the same
pool, bands = `vc_bands + 2 (N - 1)` (at most 16), the picture stage (the N x N compose) runs on the pool too; geometry, clear
and post stay on the 3D thread (small: 0.7 + 0.4 ms). Raster wall time of the town frame, 1 / 2 / 4 / 8 bands (BWPORT_VCORE_BANDS
pins the count): 1x 6.6 / 2.8 / 2.5 / 2.4 ms; 2x 51 / 28 / 19 / 17; 4x 129 / 110 / 41 / 13 (the 14-band default). Battle 4x 36 /
21 / 13 / 10. So 8 threads give 3-10x on the raster up to 4x, not linear (the strips are cut by polygon width: a few big ground
polygons dominate; and the pinned 8 CPUs were shared); past 8 threads the picture stage and the single-threaded geometry become the floor
(town 4x: picture 1.6-17 ms depending on bands). A finer split at N x (by tiles instead of rows, 32 strips) would gain at most the
gap between the measured 13 ms and the ideal 129 / 16 = 8 ms on 16 threads. For the maintainer's question: yes, vcore's 3D is CPU
work that scales with cores, which is why the other project says the processor matters; our N x is the same design.

## Is our renderer less efficient than theirs, or is Pokemon heavier?

Not answerable: their README has no numbers and nothing of their code was read. What we measured: a town frame is about 600
BEGIN_VTXS lists, 7-9k submitted vertices, 280-320 polygons after clipping and ~7.7 ms of one-thread CPU at 1x; our raster
costs about 40 ns per emitted pixel at N x (doubles, a perspective division per pixel: docs/vcore.md), so a tuning pass on the
span interpolation is a lever independent of interpolation. Which game is heavier needs the same scene measured in both.

## Recommendation, plan, effort, risks

Do it, in this order, as an option `Interpolation=off|on` (sim_config.ini, Config window), off by default, 1x first:

1. (1 day) Keyframe logic in the vcore bridge: compare each gathered frame's vertex record with the previous KEY frame (the p23
   record is a by-product of the geometry run; cost: one pass over ~8k vertices + the LCS prefix/suffix strip, < 0.2 ms).
   A frame whose 3D equals the keyframe's (the field's odd frames) is not a keyframe. Fallback classes as in section 1.
2. (1-2 days) The worker draws, per keyframe pair, M-1 mids (`vc_ip_apply_begin` on a copy of the older state; the 3D geometry
   +raster only, reusing the 2D layers: the bridge's 3D layer buffer is composited with frame t's 2D; at N x only the 3D layer is
   redrawn and re-composed) into a ring of extra pictures, plus the real keyframe picture.
3. (2 days) libntr patch: SIM_Render's cap wait becomes the present loop of section 4 (time-based t, M from the display's refresh
   or the setting), vsync interplay, the frame skip and switch-test unaffected when off; the status line shows the effective fps.
4. (1-2 days) gpu path: the same p23 hook is in the shared geometry; mids need the GL raster pass again with the 2D reused (pass A/B
   only, then C): per extra frame = CPU geometry + the raster pass + one compose.
5. (1 day) tests: p23 unit tests (pairing, blend at f = 0 and f = 1 equals the frames, 2126 vcore_same scenes unaffected),
   a lockstep check that off changes nothing, a visual check with a fast scene.
Effort about 1-2 weeks. Risks: the 2-frame delay in the field (33 ms: mention in the option's text; it is the price of
interpolating a 30 fps 3D), CPU: 120 fps = +1 draw per frame, 240 = +3 (the vcore budget above: only at 1x or with the gpu path),
popping of lists at the view edge, rotations (shrink), the lab/town palette-animated lists (the lab's 15 % MOSTLY pairs are a map
palette animation, harmless), texture animation (frame t's texture used for the whole interval), a display that is not a multiple of
60 Hz (needs time-based blend), win64's `SDL_Delay` granularity (timeBeginPeriod is already set by the cap). Not interpolated:
2D (held), the touch screen, display-capture frames (drawn once as now).

## Reproduce

`BWPORT_INTERP_STATS=<scratch> BWPORT_KEYS=... python3 tools/port_run.py --fast --save town ...`; `python3 port/vcore/tools/interp_stats.py LOG --range 2700 4000`; the
captured pair: `BWPORT_VCAPTURE=DIR,2776-2790`; then `build/vcore/vcore_interp DIR/f002780.vcs DIR/f002782.vcs OUT`.
Not done in this round: the gpu path's extra-frame cost, a measured input-delay figure with a real display, wiring.

## STATUS: wired live on vcore at every Resolution (not measured above 1x), gpu not yet

Done (commits on top of ip75's):
- `SmoothMotion=off|on`,
  libntr-ib78.patch (the LAST in diff_files; touches sim_config.h/.c, sim_gui.cpp, sim_main.cpp). libntr gives the bridge the
  tick's present slots (`SIM_InterpPace`: slot offsets from the tick's nominal time, the next tick's first slot, the game tick's
  length) and presents the bridge's picture in the cap loop's later slots (`SIM_SetInterpPicture`; a slot with no picture shows
  the one before again, as before). BWPORT_PRESENTLOG's P lines now also carry the slot's kind and, with BWPORT_PRESENTHASH=1,
  a hash of the pixels shown (the vcore path), so distinct pictures per second can be counted.
- port/bridge/vcore_bridge.c (IpDraw): per call the worker records the frame's pre-clip vertices (vc_ip_record), tells a keyframe
  (3D differs from the last keyframe's) from a repeat, pairs a new keyframe with the previous one (vc_ip_pair, 48 px limit), and
  draws for each later slot of the tick, and for the next tick's first slot, the call's state with its vertices blended toward the
  previous keyframe by 1 - f (f = (time - keyframe time) / gap, gap clamped to 1..2 game ticks; the keyframe shows at f = 1, one gap
  after it was gathered: +0 ms against today for 60 fps 3D, +1 game tick in the 30 fps field). Fallbacks: no pair yet / 3D off /
  fewer than half the vertices paired (cut) / f >= 1 = the plain frame or the picture before again. 2D is the call's own state.
  BWPORT_INTERP_REPORT=FILE: counters and per-render ms. BWPORT_INTERP_GRACE_US=N: wait N us for a late mid.
- Off is untouched (the bridge and libntr paths are guarded; vcore_same 0 differ of 2126 with the hooks off, run against a main-built
  vcore_render; the town walk's presents and reached-frame as before).

Measured (town save, walking, vcore 1x, DisplayCap 120, Speed 1, this host/llvmpipe): presents 120/s; distinct pictures per second
~115 on, ~29 off (30 fps field 3D). Report of the walk: 451 keyframes, 1767 repeats, 99.8 % of lists and vertices paired, 1 cut,
889 mids drawn and all shown (0 late), 2.0 ms per in-between render, 1.65 ms per plain render, 0.25 ms to record.

NOT done / next agent: gpu path (gpu_bridge.c: same IpDraw idea, p14 hook is shared; mids are GL textures: libntr needs a texture
variant of SIM_SetInterpPicture); Resolution 2+ measurements (cost, late mids: BWPORT_RESOLUTION=2); battle and route1 fallback
counts; input delay on vs off (BWPORT_KEYS to the first changed hash in the P log); screenshot strips; the live switch test with
Interpolation on; Speed 2 at cap 120; per-frame 2D reuse (a mid redraws the 2D engines too).

## STATUS: gpu path wired; 2D reuse measured; the rest of the checks not done

Done (commit "In-between frames on the gpu path"):
- Renderer=gpu shows in-betweens (gpu_bridge.c Stage1Interp / InterpTexture, libntr-ib79.patch = sim_main.cpp only: SIM_SetInterpTexture,
  the cap loop calls it right after the previous slot was presented so the GPU works while libntr sleeps for the slot, BWPORT_PRESENTHASH
  now hashes GL textures too). The worker does the 2D once per tick (engine B's lines, g07_stage_build) and for every present slot after
  the first the blended geometry (gpu_geometry_again) and its span records (one GpuPrep per slot); the GL thread runs gpu_render_prepared and
  pass C with the keyframe's stage. So the gpu path reuses the keyframe's 2D by construction. GpuPrep keeps swap_param (the GL half no longer
  reads s->geo, which the worker rewrites for the next slot). The record/pair/gap code is shared: port/bridge/ip_common.h.
  Measured (llvmpipe, town save, hold-walk left*f80@f2600 up*f130@f2700, cap 120, Resolution 1): 120 presents/s; while walking
  all 120 differ (115-118 transitions/s); 526 mids drawn, 521 shown, 5 late; 100 % lists paired, 0 cuts; CPU half 1.34 ms per in-between,
  GL half 2.08 ms (llvmpipe).
Done (commit "vcore: ..."): vcore_render_again / vc_render_3d_again (only the 3D redone, 2D of the first render kept), BWPORT_INTERP_REUSE=1,
  BWPORT_INTERP_VERIFY=1 (compares with the full render: 0 differ of 1129 at 1x and of 1009 at 2x). MEASURED SLOWER in wall time, so the
  default is OFF: ms per in-between (render of the call's first picture included) reuse on / off: 1x 2.74 / 1.97, 2x 5.44 / 3.84, 4x 13.1 / 9.8
  (4x on vcore is too slow either way: mids shown 1 of 475). The full render overlaps engine A/B's 2D with the 3D on other threads, so the 2D
  is not on the critical path; reuse would save CPU, not wall time.
NOT done / next agent: the checks of the brief: distinct pictures and fallback counts for route1 walking and a battle (vcore/gpu, Resolution 1
  and 2), input delay on vs off, screenshot strips, the live switch test with Interpolation on, Speed 2 at cap 120, gpu at Resolution 2+.
  run helper: C:\stress\ib79\run.sh (TAG SECS SAVE; env RENDERER RES INTERP CAP WALKKEYS) and C:\stress\ib78\pl.py.

## STATUS: the walking character's cell-mix glitch fixed (both renderers)

The maintainer's "character texture jitters/glitches when walking with Smooth motion". CAUSE (measured, suspect (b)): the player's and NPCs' sprite is ONE
4-vertex quad on a 32 x 1024 texture atlas of walk cells; the walk animation picks the cell by TEXCOORDS (t steps by 32 texels per cell, the list's
texparam, palette and attr never change: not (a), (c), (d), (e); the quad itself is paired and its position follows the camera: not (f)). The blend
lerped texcoords that differ by < 128 texels, so each cell step swept the quad through the atlas: for the in-betweens of the step (f = .25/.5/.75) a
strip of two cells (a head floating over a body, the shadow ellipse beside the feet): strips C:\stress\shots\jt81_strip_before_town.png / _after_town.png.
FIX (port/vcore/src/p23_interp.c vc_ip_texhold + vc_ip_vertex, port/bridge/ip_common.h; shared, so vcore and gpu both): a paired list at most 64 px
big on screen whose texcoords change by >= 8 texels between the keyframes keeps the NEARER keyframe's texcoords (old for f <= .5, new after), its
position/colour still blended. The histogram of max texcoord change per paired list (BWPORT_INTERP_REPORT) is bimodal: 0 / < 1 texel (water) / 2-4 (route 1
scrolls) vs >= 32 (cells): 8 is in the empty gap. The size limit matters: without it the battle's big floor/sky lists (5000+ px, they scroll 8+ texels per
keyframe) were held too and lost their smooth scroll.
METRIC (tools/interp_metric.py over BWPORT_INTERP_CHARLOG=FILE, which logs per picture the small lists' boxes and texel rectangles as the blend makes
them; run C:\stress\jt81\m.sh): sprite lists (n = 4, 12-40 px) in in-between pictures: cell steps seen / MIX (shown rectangle is neither keyframe's) before -> after:
town vcore 1x 368/338 -> 0, vcore 2x 372/342 -> 0, gpu 1x 372/342 -> 0, gpu 2x 287/266 -> 0; route 1 vcore 301/297 -> 0; battle (route 1 into the grass) 299/296 -> 0.
Off-path (shown centre outside the two keyframes' centres +- 0.6 px): 0 everywhere. Unpaired sprite lists in in-betweens: 0 in town; route 1 / battle 88-126 in
36-50 pictures: the grass/dust effect particles and the battle's first frames (lists appearing, as at 60 Hz), not the characters.
Tools: BWPORT_INTERP_TEXHOLD=N (1/16 texel; 0 = the old behaviour, for before/after), BWPORT_INTERP_DUMP=DIR (vcore: every in-between as PNG, picture numbers of
the CHARLOG P lines), tools/interp_strip.py (crops side by side), C:\stress\jt81\{m.sh,t8.sh,sw.sh}.
CHECKS: vcore_same 0 differ of 2126, vcore_tests 420 passed (the hooks off are untouched). Distinct pictures per second while walking in town (pl.py): 100-119,
same before and after on gpu; the keyframe / pair / cut counters are identical before and after in the deterministic runs (route 1 walk: 2048 keyframes, 2006 pairs,
35 cuts both ways). Live switch test (BWPORT_SMOOTH=on, cap 120, vcore:1,gpu:1,gpu:2,vcore:2,gpu:1,vcore:1/60): PASS, 6 ok. Timings were noisy (pf81 builds on the same
pool; some runs 43-53 fps with late mids): not a cost measurement.
NOT done: the Windows exe check; a Pokemon sprite in the battle (MCSS: texture contents rewritten per frame, suspect (c)) was not examined separately.
