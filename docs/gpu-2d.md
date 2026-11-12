# The DS 2D layers in the GPU renderer: CPU or GPU? (design study, 2026-11-03)

A study; the maintainer chose option 1 (c1), which is now built ("Option 1 status" below); the rest is not implemented. Measured on the M2 (gpu 3x, 60 Hz, `--noskip`,
a scratch build with timers around the four calls the bridge's "2D" figure covers, load 3-4 throughout: the numbers are
~10% high) and scaled to a test iPhone 16 Pro Max by the passes line's 3.42-3.74 ms (~2.7x the M2's).

## Option 1 status (2026-11-03): built, on by default under both renderers

**What runs.** On the frame thread, after the previous frame's worker is joined (its state and pack are then stable until
the next call's Gather), the GPU bridge's `Skip2D` asks `vc_2d_same` (port/vcore/src/p24_same.c) for each engine whether
this frame's 2D inputs equal the previous frame's; the worker's `Build2D` then reuses that frame's half of the pack.
Neither engine changed: the slot takes the previous frame's pack object itself, serial and all, so pass C uploads
nothing (GL and Metal) and Smooth's in-betweens read it as before. One changed: a new pack in the other of two shared
buffers (`sPack`: the one the previous frame does not use), the unchanged half copied (`g07_pack_reuse`), the other built.
No reuse: the first frame, a frame after a capture frame (vcore's: no pack), and anything that runs `ReleaseAll`
(renderer switch, Resolution, antialiasing; options and scale do not enter the 2D, which is 1x). Renderer=vcore: the
same compare for engine B only (`Skip2D` in vcore_bridge.c), and vcore copies the previous frame's engine B picture
(`p20.keep_b`) instead of drawing it or starting its thread; engine A carries the 3D layer, so it is always drawn.

**What is compared** (from the code, p01-p12 and p20: no piece keeps state between frames): the engine's 0x70 bytes of
registers (DISPCNT, BGxCNT/scroll/affine, windows, mosaic, BLDCNT/ALPHA/Y, DISPCAPCNT in A's block, MASTER_BRIGHT),
POWCNT1 (engine power, the screen swap; `composed()` reads it), VRAMCNT A-I (so the regions map the same banks), its
1 KB OAM and 1 KB palettes, the bytes of every bank page mapped in its BG, OBJ, BG-ext and OBJ-ext regions, and for
engine A in display mode 2 the LCDC bank it shows. Not compared, because the pack does not hold it: the 3D layer
(composed apart in pass C; `ln->l3d` is not packed). Pass C's uniforms are made from the current frame's registers.

**Checks.** `BWPORT_2D_SKIPCHECK=1` builds every frame anyway and compares the fresh pack's halves with the ones the skip
would have reused (`g07_pack_differs`; vcore: engine B's picture), logging any frame that differs: **0 mismatches** in
all of these (M2): town idle, walk, X menu and bag at gpu 3x (Metal, Smooth off; Metal, Smooth on at 120; frame skip 4)
and 1x (GL, Smooth on); Route 1 grass walk, encounter (its ~270 capture frames), battle intro and command screen at gpu
1x (Metal, Smooth on) and 3x (Metal Smooth off; GL Smooth on); Renderer=vcore town at 3x and the battle at 1x (GL) and
3x (Metal), Smooth on. Screenshots skip on vs `BWPORT_2D_SKIP=0`: the bag (both screens, f2700 and f2850) byte-identical
under gpu 3x and vcore 3x; in the other scenes the bottom screens are identical and the top screens differ only where
the game itself differs between runs (NPC walks: the Mac has no lockstep clock, so two runs are not the same game).

**Skip rates** (the log's `2D skipped` line every 600 frames, no frame skip): town idle A 90% B 93%, walk A 61-98% B
93-100%, X menu A 100% B 16-33% (its bottom screen animates), bag A 67-79% B 50%, grass walk A 80% B 98%, battle A 87%
B 89-90%. Renderer=vcore engine B: town 97% / 72% / 61% (field / menu / bag), battle 89-93%.

**Before / after on the M2** (gpu 3x, 60 Hz, Smooth off, bench rows of 4 s, no frame skip; the off runs are the same
build with `BWPORT_2D_SKIP=0`; three town runs and two caught runs each way, ranges over them; `uptime` load 1.6-4.8,
one town pair at 2.4-6.4). "2D" is the passes line (`passes=1`); the cores and CPU ms per tick are from rows with
`passes=0` (with passes on, each pass's wait hid the change: 0.24-0.31 cores either way):

| scene | "2D" ms off | "2D" ms on (skipped A, B) | cores off / on (passes=0) | CPU ms/tick off / on |
| --- | --- | --- | --- | --- |
| town idle | 1.52-2.03 | 0.49-0.55 (68-90%, 93%) | 0.19-0.30 / 0.13-0.15 | 3.2-5.0 / 2.3-2.5 |
| town walk | 1.65-1.84 | 0.13-0.19 (90%, 100%) | 0.20-0.29 / 0.13-0.14 | 3.4-4.8 / 2.3-2.4 |
| X menu | 2.06-2.46 | 1.16-1.68 (100%, 16%) | 0.18-0.27 / 0.16-0.17 | 3.0-4.5 / 2.7-2.8 |
| bag (no 3D: no passes row) | - | - | 0.15-0.23 / 0.09 | 2.5-3.9 / 1.6 |
| Route 1 grass walk | 1.70-2.31 | 0.23-0.42 (80%, 98%) | - | - |
| battle (f5300) | 2.28-2.69 | 0.31-0.51 (87%, 89%) | - | - |

The "2D" figure still measures what the frame spent (the compare is in it: walking, with nearly everything skipped, the
whole figure is 0.13-0.19 ms). The encounter's timing differs between runs, so the battle rows are not the same frames;
the off rows' 2D is ~20-60% above the study's (the passes rows' per-pass waits, and load).

**What the phone should show** (its 2D was 3.42-3.74 ms a frame walking, ~2.7x the M2's): the passes line's "2D" at
~0.4-1.4 ms in the field and in battle, the X menu ~30% lower only (~3 ms: its engine B redraws every frame), the uploads
figure lower on unchanged frames (no 1.2 MB stage), and the bench rows' cores ~0.1-0.15 lower at 60 fps in the field
(the M2 dropped 0.06-0.15 cores; at most ~3 ms x 60 = 0.18 cores on the phone). The passes line now reads
`2D 0.31 (skipped A 87%, B 89%)` (libntr-ios-2dskip.patch). Not yet measured on the phone.

**Left.** Option 2 (the X menu's engine B) only if the phone's numbers say the menus matter. Windows / Linux / Android owe
the SKIPCHECK run (the maintainers' notes, "Checks owed").

## The answer

1. **Do not move the 2D engine to the GPU first. Skip it when its inputs did not change.** Each engine's 2D picture is a
   pure function of ~190-260 KB of state (its 0x70 bytes of registers, 1 KB OAM, 1 KB palettes, the VRAM pages mapped to it).
   In every scene measured, that state is unchanged on most frames: the field's bottom screen 97-100% of frames, its top
   2D layers 60-100% (walking / idle), the battle 87% (A) and 62-83% (B), the bag 50% (both engines update every other
   frame). Only the X menu's bottom screen animates every frame. A per-engine compare against the previous frame (memcmp,
   ~20 us on the M2, ~60 us on the phone) and reusing the previous lines / stage / pack cuts the phone's 3.5 ms a frame to
   **~0.2 ms idle, ~0.5-1.5 ms walking, ~0.8 ms in battle**, and the 1.2 MB stage upload a tick goes with it (the pack's
   serial does not change, so the GL half uploads nothing). It is exact by construction, ~150 lines, one rounds of work, and
   checkable by a mode that renders anyway and compares. Expected on the phone: 0.60 -> ~0.45-0.5 cores at 60 fps.
2. **Then the whole 2D on the GPU (option a) only if the remaining menus matter.** After (1) the 2D costs ~2 ms a frame
   on the phone in the X menu only (the bag and battle are halved or better). A GPU 2D is 3-5 rounds of work, a second
   compose pass for engine B, a VRAM-as-texture upload path, and an exactness campaign over text BGs, affine / extended
   BGs, 4 OBJ kinds, windows, mosaic and the display modes; its own CPU cost (the VRAM diff + uploads) is ~0.3 ms a tick on
   the phone, its GPU cost ~0.3-0.8 ms a tick. The right shape, if built, is per engine per frame with vcore as the exact
   fallback for any register state the shaders have not been checked for. Not before (1) has been measured on the phone.
3. Threads (option d) only shorten the worker's wall time (B's lines and A's stage are independent: ~0.6-1.2 ms of the
   M2's critical path), not the CPU-seconds, so not the battery. SIMD on the compose and the text-BG fetch (option c2) is
   a 20-30% trim of what (1) leaves; optional.

## The data

### 1. The 3.5 ms split (M2, Resolution 3, 60 Hz, means over 120 frames; `uptime` load 3.3-4.2)

| scene | B lines (vc_render_line x 192) | A stage (g07_stage_build) | g07_pack | gpu_texdata_build | total | phone (x2.7) |
| --- | --- | --- | --- | --- | --- | --- |
| field, idle (town) | 0.62-0.66 | 0.32-0.35 | 0.04-0.05 | 0.02 | 1.00-1.06 | ~2.8 |
| field, walking | 0.70-0.98 | 0.35-0.57 | 0.05-0.06 | 0.02-0.03 | 1.2-1.4 | ~3.5 |
| X menu (B: 4 text BGs + 16 OBJ, 5 affine) | 1.17-1.30 | 0.40-0.44 | 0.05-0.06 | 0.02-0.03 | 1.6-1.8 | ~4.6 |
| bag (both engines animate every other frame) | 0.83-1.16 | 0.54-0.77 | 0.05-0.06 | 0.03 | 1.5-2.0 | ~4.8 |
| battle (B: bgmode 3, text + 16-bit affine BG) | 0.86-1.35 | 0.45-0.70 | 0.05-0.07 | 0.03 | 1.4-2.1 | ~4.8 |

The field-walking row matches the phone's 3.42-3.74 ms at x2.7. Engine B's software lines are 55-65% (they include
B's compose, display mode and master brightness at 1x), engine A's layer staging 30-40%, the pack 4%, the texture data
2%. Nothing else is in the figure (SIM_VCapGather's VRAM copy is outside it).

Where the time goes (`sample` of the town walk, leaf symbols): `vc_bg_text_line` 121 samples, `vc_compose_line` 74,
`vc_obj_line` 6, `g07_pack` 6, the rest of the 2D under 5 each. So ~80% is two per-pixel loops: the text BG fetch (the
map entry and the tile row are already cached per 8 pixels; what is left is the palette lookup and the store, 3-4 BGs x 2
engines x 49,152 pixels) and the compositor (the layer priority sort, window mask and colour effect per pixel).

### 2. What Black uses (the probe's register and OAM summary, every change logged; 4 scenes, ~15,000 frames)

- **Display modes**: 1 (graphics) nearly always; 2 (VRAM display: a captured bank as the picture) for the continue
  screen's loading step and transitions; 0 at boot. Never mode 3 (main memory).
- **BG modes**: engine A mode 0 everywhere (BG0 = the 3D in the field and battle; text BGs 1-3, 16-colour), mode 5 with
  two **direct-colour bitmap** BGs on the title / continue screen. Engine B mode 0 (text BGs, 16-colour; the menus' BGs
  256-colour) and **mode 3** in battle (BG3 a **16-bit affine (extended) BG**, the command pane).
- **Extended palettes**: DISPCNT bit 30 on the title screen only; never for OBJs.
- **OBJs**: 1D mapping, tile OBJs 4-bit only; **affine OBJs** on engine B (the field's bottom screen: 1-5, the X menu's
  icons: 5). No bitmap OBJs, no semi-transparent OBJs, no OBJ windows, no 8-bit OBJs, no mosaic in any scene.
- **Windows**: none (WIN0/1/OBJ never enabled). **Mosaic**: never.
- **Colour effects**: engine A alpha blending (BLDCNT 0x1148 / 0x1b44 / 0x3d42 in the field and battle: the 3D or BG
  layers over the rest), brightness up / down (mode 2 / 3) for the fades; engine B blending on the continue screen and
  the bag; master brightness fades on both engines at every scene change.
- **Captures**: DISPCAPCNT bit 31 set on single frames at menu transitions (the bridge draws those frames with vcore
  alone at 1x: nothing changes for them).
- **Mid-frame register changes**: none reach the renderer today. libntr runs the game's H-blank handler once per frame
  (libntr-vcount.patch), SIM_VCapGather snapshots the registers at V-blank, and vcore draws the frame from that one
  snapshot (affine reference points advance by PB / PD from the latched value: GBATEK's per-line rule, no per-line
  writes). The game registers H-blank tasks (gfl_use.c, wipe.c), so a future per-line model would be a 192-row register
  texture in any design; neither option here changes that.

Not seen in these scenes but in the game (to keep an exact fallback for): the large bitmap BG (mode 6), 256-colour
bitmap BGs, OBJ windows, mosaic, semi-transparent and bitmap OBJs, OBJ extended palettes.

### 3. The options

| option | work | phone CPU a frame (from 3.5 ms) | phone GPU | exactness | parity |
| --- | --- | --- | --- | --- | --- |
| **c1. Skip an engine whose inputs are unchanged** (compare its regs + OAM + palettes + mapped VRAM pages with the previous frame's copy; reuse the lines / stage / pack) | ~150 lines in port/bridge/gpu_bridge.c (the copy, the compare, the two reuse paths) | ~0.2 idle, 0.5-1.5 walking, ~0.8 battle, ~1.8 bag, ~2.9 X menu (its B animates) | none | exact by construction; the risk is a missed input (VRAMCNT mapping, POWCNT1, DISPCAPCNT, DISPCNT's 3D bit, the ext-palette banks) | all platforms, no shader |
| **c2. Cheaper CPU loops**: NEON / SSE on the compositor's per-pixel sort and blend, the text BG's 8-pixel inner loop written per tile row | ~300 lines in p01 / p09 (shared with vcore 1x) | -20-30% of what c1 leaves | none | checked by vcore_same (byte-identical at 1x over the corpus); modest risk | all (NEON on ARM, SSE on x86: two code paths or plain C that the compiler vectorises) |
| **d. Engine B on a second worker thread** beside A's stage | ~60 lines (vcore_task_start) | CPU-seconds unchanged; the worker's wall -0.6-1.2 ms (M2) | none | exact (the two engines share no buffers) | all; costs a core wake per frame (battery), gains only the late-slot margin |
| **b. Hybrid: text BGs on the GPU, OBJs on the CPU** | ~1,200 lines: a BG pass per engine (the GPU reads VRAM as a texture), the OBJ lines still uploaded, B gets its own compose pass | ~1.2 (the OBJ lines + uploads) | ~0.3 | the BG shader checked against vcore over the corpus; OBJ exact | shaders in GLES 3.0 (usampler2D + texelFetch), MSL through gpu_msl.py |
| **a. The whole 2D on the GPU**: VRAM (the mapped pages, diff-uploaded like the texture slots), palettes, OAM and the registers up; per engine a BG pass (text / affine / extended / bitmap), an OBJ pass (per fragment: the OAM walk with the DS priority rule, or OBJs as quads with depth = prio x 128 + index and inverse-affine texcoords), the window mask, then pass C's compose (engine B gets the same shader with its uniforms, its display mode and master brightness) | 3,000-4,000 lines: 6-8 shaders, the upload path, B's compose target, the per-engine fallback switch, the test harness | ~0.3 (the diff + uploads) | ~0.3-0.8 a tick (10 passes at 256x192 with integer fetches; the OBJ walk 128 x 49,152 x 2 fetches of cached OAM) | the long pole: every rule in p01-p10 re-derived in GLSL (the 9-bit scroll wrap, the affine 24.8 steps and the wraparound bit, the 16-bit affine map's flip bits, OBJ x / y wrap, the priority rule, the two alpha paths, brightness in 6 bits); mosaic, windows, bitmap OBJs, mode 6 and captures left to vcore per frame | GLES 3.0 has what it needs (integer textures, texelFetch, no SSBO or compute: the OBJ pass is a fragment loop); Metal via the existing MSL generation; one shader source for both |

**Smooth motion's in-betweens** are unaffected by every option: the 2D is built once per tick now (the mids reuse the
pack's upload); c1 reuses it across ticks too, and (a) would produce the stage texture once per tick on the GPU and let
the mids sample it. **Fast forward** draws fewer frames per tick (frame skip), so the 2D's share is smaller there;
c1 still skips the drawn frames whose engines did not change.

**The uploads.** Today 1.2 MB a tick (the stage 786 KB + the 1x lines 393 KB through PBOs on GL; Metal shared buffers).
Under c1 a tick whose two engines are unchanged uploads none of it. Under (a) the upload becomes the VRAM diff rows
(usually nothing: the banks rarely change in a scene), OAM 2 KB and the palettes 2 KB, and the stage never leaves the GPU.

**Why not (a) now.** It is the largest piece of work in the renderer's history for ~2 ms a frame in the X menu after c1,
and its exactness must be proven rule by rule where c1's is free. It also puts a new per-tick GPU cost (0.3-0.8 ms) on a
phone whose GPU budget at 120 Hz is the tighter one (docs/gpu-cost.md "Budget"). Its CPU side (uploads) is no cheaper
than c1's in the scenes that matter.

### 4. Exactness: how each option is checked

vcore's 1x picture is the oracle for all of them (it is checked against melonDS / NooDS / DeSmuME on the PC).

- **c1**: `BWPORT_2D_SKIPCHECK=1`: render anyway and memcmp the reused lines / stage / pack against the fresh ones
  every frame, and print any frame that differs (a missed input shows up as a differing frame; the fix is adding the
  input to the compare). Plus `BWPORT_GPU_CHECK=1` at 1x (pass C against vcore, exact today) over the saves' key scripts
  (field walk, X menu, bag, battle, a map change with fades, the continue screen with its captures) and the intro.
  Divergence sources: an input not in the compare; the VRAM pages' identity (a VRAMCNT change that keeps the bytes
  equal but moves a bank: compare VRAMCNT A-I and the page src list, not only the bytes); POWCNT1's engine-on and
  swap bits; DISPCAPCNT (a capture frame is vcore's anyway); the 3D's presence in A's stage (bg3d: from DISPCNT, in the
  compare). The compare copy costs ~400 KB of memory per slot.
- **c2**: `vcore_same.py` at 1x over the corpus and fuzz (byte-identical), gpu_same for pass C; the PC's lockstep sweep.
- **d**: the same pictures by construction; the GPU check at 1x after the threading.
- **b / a**: a `--2d gpu` mode in `gpu_render` (one .vcs through the GPU 2D, compared with vcore's 1x screens) over the
  PC's 37-capture corpus and new captures from the Mac's saves (`BWPORT_VCAPTURE=DIR,FRAME`; vcs_dump.py --tags lists
  what a capture uses); the 2D fuzz scenes of p01-p10 (tests/t01-t10 generate register / VRAM states; a harness that
  feeds them to the GPU path is new work); in-game `BWPORT_GPU_CHECK` extended to engine B's lines. Where it can diverge:
  every rule above; also the GPU's integer division and shifts are exact, so no float enters the 2D path (keep it all
  `uint`), and the OBJ priority rule must be the DS's (priority, then the lower OAM index) not the draw order.

### 5. Recommendation and phases

**Phase 1 (recommended now): c1, the unchanged-engine skip.** One agent, ~150 lines in gpu_bridge.c (both
Stage1 paths) and the pack's serial rule; `BWPORT_2D_SKIPCHECK`. Two details: the slots each own a pack (g07_compose_packed
uploads when `k->serial != c->up`, GL and Metal alike), so a skipped tick must hand pass C the previous tick's pack object
(or copy its serial with its bytes) rather than its own slot's stale one; and the engines skip independently (A's stage
unchanged but B's lines not: rebuild only B's half of the pack's `src`, with a new serial). Checks: the skipcheck over the five key scripts with 0
differing frames; `BWPORT_GPU_CHECK` at 1x 0 px; the Mac's 2dprobe-style timers before / after (town idle, walk, menu,
bag, battle); then the phone's perf test (the passes line's "2D" and the bench row's cores at 60 and 120 Hz). Expected:
"2D" 3.5 -> 0.2-1.5 ms in the field, ~0.8 in battle; ~0.1-0.15 cores off the phone's 0.60.

**Phase 1b (optional, after the phone numbers): d and c2.** d if the phone's late-slot count (docs/perf.md, "late/s")
is still driven by the worker's CPU half; c2 if the X menu or battle rows still show the 2D above ~1 ms.

**Phase 2 (only if the maintainer wants the menus' 2 ms too): a, per engine with the vcore fallback.** Order: (i) the
upload path (VRAM pages, OAM, palettes, registers as textures / uniforms) and engine B's compose pass reusing
g07_compose.frag with B's uniforms, fed by CPU-built layers at first (no new rules yet; checked: byte-identical to today);
(ii) text BGs in a shader (p01's rules), 16-colour then 256-colour and extended palettes; (iii) normal OBJs (p05's
priority rule); (iv) affine OBJs and affine / extended BGs (p02, p03, p06); (v) the fallback switch per engine per frame
on the register state (mosaic, windows, bitmap OBJs, mode 6, display modes 0 / 2 / 3, captures -> vcore). Each step:
gpu_render `--2d gpu` over the corpus and the piece's fuzz scenes byte-identical at 1x, then in-game GPU check, then the
phone's passes line. 3-5 rounds of work; GPU +0.3-0.8 ms a tick on the phone, CPU ~0.3 ms a tick.

## Method (so the next agent can repeat it)

Scratch build (not committed): four timers around `vc_render_line(B)`, `g07_stage_build`, `g07_pack`,
`gpu_texdata_build` in both Stage1 paths of port/bridge/gpu_bridge.c, means every 120 frames; a per-engine FNV hash of
the registers, OAM, palettes and mapped pages counting "same as the previous frame"; a register / OAM feature summary
printed on change. Runs: `tools/mac_run.sh --noskip --save NAME` with `BWPORT_CFGSCRIPT=f1100:renderer=gpu,resolution=3,framerate=60;...`,
saves town (idle, walk `left*f300@f1300 right*f300@f1700`, X menu `x@f1500`, bag `down down a`) and caught (grass walk
`left*f90@f1200 right*f90@f1300 ...` until an encounter; the battle's command screen by f5200); `sample PID 5` for the
profile. The probe sources are in the session scratchpad's study2d folder (probe2d.h, run.sh, run_sample.sh).
