# Performance at higher Resolutions

The maintainer: "i feel like we should have better performance when the resolution is increased" (Intel UHD 770; vcore 1x ~550 fps,
gpu 1x ~460, both fall steeply with Resolution). This file: where a frame's time goes, what was fixed, what is left.

## How measured (this host)

AMD Ryzen 9 9955HX under WSL, shared with one other agent: SMT siblings are CPU pairs (4,5), (6,7)...; `taskset -c 4-19` is 8
cores / 16 threads. vcore: `vcore_render F.vcs --scale N --bench K` (wall ms per frame, `cpu` = all threads; the N x line gives
the 3D thread's wall ms per stage: plan, clear, raster, post, picture), corpus frames town_f002800 (field, walking) and
battle_f017000. CPU time doubles when two threads share a core (bands 1 -> 2 on CPUs 4-5: cpu 168 -> 266 ms, raster wall
141 -> 122 ms at 8x), so compare walls, or CPU on one pinned CPU. `perf` works under WSL (`apt install linux-perf`;
`perf record` + `perf report --sort srcline`). This host has no GPU: the gpu path's GPU work is counted from the code, not timed.

## vcore: before / after (ms per frame, wall, 16 threads; fps = 1000 / ms of the renderer alone)

| Resolution | town before | town after | battle before | battle after |
|---|---|---|---|---|
| 1 | 2.52 (397 fps) | 2.63 (380) | 1.57 (637) | 1.65 (606) |
| 2 | 4.30 (233) | 4.15 (241) | 3.72 (269) | 2.60 (385) |
| 3 | 8.98 (111) | 6.85 (146) | 5.62 (178) | 4.98 (201) |
| 4 | 11.59 (86) | 10.80 (93) | 9.85 (102) | 7.32 (137) |
| 6 | 23.95 (42) | 17.47 (57) | 15.63 (64) | 12.81 (78) |
| 8 | 36.10 (28) | 27.70 (36) | 26.31 (38) | 20.75 (48) |

1x is untouched code (the difference is noise). Stages, town 8x before -> after: clear 3.7 -> 1.4, raster 22.7 -> 21.2, post
6.4 -> 2.4, picture 2.8 -> 2.6, geometry 0.33 (one thread, unchanged), 2D A + B 1.1 (unchanged). One thread at 4x: raster
47.2 -> 39.2 ms; at 8x 141 ms (so the 16-thread raster is a 7x speedup on 8 cores: the strip split is not the problem).

Fixes (both checked byte-identical with vcore_same.py, 2126 scenes, 0 differ at --scale 1, 2, 3, 4, 8, at 2 with --aa 2, at 4
with --edge fine --aa 2; vcore_same.py gained `--scale N` and `--opts`):
1. The N x clear and post ran on the 3D thread alone (10 ms of 36 at 8x): now pool jobs over row strips (p20 `clear_n`,
   `post_n`; p21 `vc_n_clear_rows`, `vc_n_post_begin` / `vc_n_post_rows` in 4 phases, Edge=ds thickening into its own buffer).
2. The raster's per-pixel struct copy right after scalar stores (a store-forwarding stall, 9 % of the raster) and shade_n's
   floor/clamp through a 64-bit integer: removed (`chan6`).

Measured and ruled out: the per-pixel divisions (5 at most) are ~6 % of the raster (replacing them by multiplications, not
identical, gave 47.2 -> 44.5 ms); the texture context per polygon per strip is a 1 KB memset.

## vcore: the rest of the frame at N (from the code)

The picture (256 N x 384 N RGBA8, the top screen composed by p09 N x N times per DS row, the bottom screen's 1x lines scaled)
goes to libntr's sized hook, which uploads it whole every frame: 8x = 25 MB a frame (1.5 GB/s at 60 fps) through
glTexSubImage2D, a synchronous CPU copy on most drivers. The bottom screen (and every line without 3D) is 1x content
scaled N x N: half of that upload is N^2 copies of 1x pixels.

## gpu path: what each frame asks of the GPU (from port/gpu, port/bridge/gpu_bridge.c)

CPU per frame: geometry (vcore p13-p15, shared), the span records (g02, one record per polygon per N x row: grows with N, not
N^2), the bin lists (build_bands), engine B's 1x lines and engine A's 2D stage (1x). Nothing N x N on the CPU in the normal
path (BWPORT_GPU_CHECK's readback is a debug option).

| pass | size | per frame |
|---|---|---|
| uploads | poly/span/band/list data textures (TexSubImage2D into persistent textures, grown by half); textures and palettes only the changed rows (`_diff`) | span bytes grow with N |
| g07 stage + src | 256x192 RGBA32UI (786 KB) + 256x384 R32UI (393 KB) | **TexImage2D (re-specified) every frame** |
| R (3D) | 256N x 192N, 2 colour targets; each pixel loops over its bin's polygon list (bins 16N x 8N px, so the list per pixel is the same at every N: cost ~ N^2) | 1 draw |
| E (edge map) | 256x192 x 2 | only N > 1 with edge marking |
| P (post) | 256N x 192N | 1 draw |
| A (AA resolve) | 256N x 192N from the N k grid | only with AA |
| C (compose) | 256N x 384N RGBA8 | 1 draw |
| present | libntr draws pass C's texture to the window | 1 quad |

No glReadPixels, glFinish or fence waits in the normal path (glFinish only with gpu_set_timing); uniforms are looked up by
name every frame (~30 glGetUniformLocation calls: CPU only). Full-screen work at N: R + P at 256N x 192N and C at 256N x 384N,
so ~2 x 256N x 192N x (bytes per pixel) of writes plus R's polygon loop: at 8x 3.1 M pixels per pass.

## What's left, ranked by expected gain

1. **GPU timer readout (not done)**: GL_TIME_ELAPSED queries around R, E, P, A, C (read 3 frames late) + the CPU stages, shown
   in the Metrics window (a libntr hook, libntr-pf81.patch, last in diff_files) or written by a Debug-menu item next to the save.
   Without it the maintainer's iGPU numbers are guesses; it decides 2-4 below. Effort: medium (bridge + one libntr patch).
2. gpu: pass R's per-pixel polygon loop is the likely iGPU cost at N (N^2 pixels x list length): a coarser early-out per bin
   (skip polygons whose row run misses the pixel's row before loading the record) or hardware triangles for the coverage.
   Effort: large; measure with 1 first.
3. Present at N (both paths): vcore uploads 25 MB at 8x every frame; upload the top screen's N x picture and the bottom
   screen at 1x (scaled by the quad), or through a PBO. Gain: half the upload, and no stall on the iGPU. Effort: medium (libntr
   present + vcore picture layout).
4. gpu: stage/src textures TexImage2D -> TexSubImage2D (allocated once), uniform locations cached. Small, safe. Effort: small.
5. vcore picture stage: `(x * n + i + hofs * n) % (2 * W)` is an integer division per pixel (3.1 M at 8x): step it; skip
   vc_compose_line for sub-pixel phases whose 3D row equals the previous phase's. ~1-2 ms at 8x. Effort: small.
6. vcore raster per pixel: the rest is spread (shade, plot, texture sample, lerp); SSE2 pairs (divpd, identical lanes) or a
   fixed-point span walk (not byte-identical: needs a new reference). 20-40 %. Effort: large.
7. vcore clear at N: fold each strip's clear into its raster job (one pass over memory instead of two). ~1 ms at 8x.

## (pf82): the game on the N x vcore code, the timing readout, gpu small fixes

**The game on pf81's vcore code** (town save, `--fast`, BWPORT_FPSLOG, this host's llvmpipe present): Resolution 1 / 2 / 4 reach the
frame, no crash, 240 / 250 / 116 fps (work 1.5 / 1.9 / 6.0 ms, swap 2.7 / 2.0 / 2.5 ms). The live switch test from the route1 save
(vcore:1, 2, 4, 8, 3, gpu:2, vcore:2a2, 4a2, 6, gpu:4, vcore:1; and gpu:1, 2, 4a2, 8, 3, vcore:2, gpu:2a4, gpu:1): PASS.

**The readout (libntr-pf82.patch): how the maintainer reads it.** View > Metrics, then click **Breakdown** in the Metrics panel (closed,
nothing is timed; open, the metrics take the whole side). ms per frame, the mean of the last refresh (Metrics refresh) and its max:
- *Frame thread*: the main thread's parts; they add up to the frame period (the `sum` line). `renderer` is the renderer's call;
  `picture upload + quad` vcore's upload of its picture (the GPU path: the quad); `swap` is where a driver waits for the GPU;
  `frame-rate wait` is idle time under the cap (60 fps: what is left of 16.7 ms).
- *Renderer call*: inside `renderer`. vcore: gather, wait for the worker (the worker's draw took longer than the game's frame: the
  draw is the bottleneck), start. gpu: gather, wait, `GL half: 3D` (uploads + passes R, E, P issued), `GL half: compose`.
- *Renderer workers* (beside the game, overlapping it): vcore's draw (whole) and its stages (geometry, N x plan, clear, raster,
  post, 2D A/B, N x picture; at 1x clear/raster/post are the 3D thread's CPU time, at N x wall time); gpu's stage 1 (geometry,
  spans + bins, 2D).
- *GPU*: GL_TIME_ELAPSED per pass (uploads, R: 3D raster, E: edge map, P: 3D post, A: AA resolve, C: compose), the present and
  the UI, read 5 frames late (never a stall). `sum (GPU busy)` above ~16 ms = the GPU cannot hold 60 fps at that setting. On GLES
  or a GL without timer queries the line says n/a and the CPU times stay. On llvmpipe the split between passes is skewed (it
  rasterizes a pass when the next one flushes it); the sum is right.
- **Debug > Save performance log**: 5 s of every frame's stages into `perf_log.txt` next to save.bin, with the GL renderer and
  version, Renderer, Resolution, Antialiasing, Smooth motion, Frame rate, Game speed, Frame skip, window size; a summary table
  (avg, max per stage) then one row per frame. Ask the maintainer for one log per setting he finds slow (e.g. gpu 4x, vcore 4x).
  Tests: BWPORT_BREAKDOWN=1 opens the section, BWPORT_PERFLOG=N starts the log at frame N (port_win64.sh run passes both).

Examples on this host (town, Unlimited, llvmpipe): vcore 4x: worker draw 9.3 ms (raster 6.6, post 0.8, picture 1.0), the frame
waits 5.0 ms for it, upload 0.5, swap 2.3. gpu 4x: GL half 11.4 ms CPU (llvmpipe), GPU busy 19 ms (R 8.4, E 3.2, P 2.0, C 2.4,
uploads 1.3, present 1.5).

**gpu small fixes** (same bytes: gpu_same vs the old build 0 of 56 differ at 1x/2x/4x for the 3D layer, with `--compose gpu`, and
with `--compose gpu --aa 2`; gpu_tests PASS): pass C's stage and 1x-line textures are allocated once and filled with
glTexSubImage2D; uniform locations are looked up once (gpu_gl_uniform). Small CPU gains (two re-allocations and ~40 name
lookups a frame); not separately timed.

**Not done: the vcore present (step 4)**, the design for the next agent: libntr uploads the whole 256N x 384N picture each frame.
Exact and cheap: vcore records per picture DS row (384) the 1x line it is a scaled copy of (fill_row in p21_scale.c: rows of
engine B, engine A rows without the 3D layer, vc_n_picture_from_screens), NULL for N x 3D rows; the bridge keeps a copy of the
last returned picture's 1x lines (384 KB) and marks a row clean when both are 1x lines and equal; libntr (SIM_FramePictureRows)
uploads only the dirty runs when its texture still holds the last picture (it uploaded it, same size, no in-between picture
written into s_frameTexture since: the cap loop's mid upload clears that), else the whole. Frame skip returns the same picture:
no rows. Prove with BWPORT_PRESENTLOG + a PRESENTHASH mode that hashes s_frameTexture by readback (SpHashTex) after the upload,
against the old full upload's rgba hashes, capped at 60 fps from the same save and keys. Expected: the field's static bottom
screen (half the upload at every N), static menus (all of it). Next: the whole input unchanged (30 fps 3D: io, VRAM, palettes,
OAM, the gx stream equal to the last frame's, ~700 KB to compare) = the same picture: no upload and no draw.

**What's left, ranked** (update of the list above): 1. get the maintainer's perf_log.txt for gpu and vcore at the Resolutions he finds
slow: it decides 2-4. 2. the vcore present (above). 3. gpu pass R's per-pixel polygon loop at N (if R dominates his GPU sum).
4. vcore picture stage (the division per pixel, repeated compose calls). 5. vcore raster per pixel (SSE2 / fixed point).


## M2 macOS, 2026-11-01 (iPhone sizing)

Apple M2 (8 cores), macOS, Metal-backed GL 4.1, hidden window, `tools/mac_perf.py matrix` (one run folder, nice 10, SDL dummy audio, the
maintainer's Mac in use, **an iOS build and Simulator overlapping part of it**). Per run: fast-forward at the unlimited rate to the scene with a
key script, `tab` to the 60 fps cap 40 frames before, BWPORT_PERFLOG records 5 s (frames 30..300 used; the unlimited run uses the first
600 frames). `tools/mac_perf_table.py` makes the table: the steadier (lower p99 of work) of two capped runs; `work` = the frame's busy ms
(game logic + V-blank + sound, renderer call + picture upload, UI build + UI draw + swap; the frame-rate wait is not counted), med / p99;
the cap holds while the median is well under 16.7 and the p99 near it. `worker` = vcore's draw (whole) / gpu's stage 1, on its own threads
beside the game. `GPU busy` = the sum of the GL timer queries (gpu path; vcore: only the present). `cores` = the process's CPU time / wall
over the window (vcore's threads + game + driver). `uncapped fps` = one run, Frame rate Unlimited. `load before` = the load average at the
start of each run (capped, capped, unlimited); **DISTURBED** = a load average of 12 or more at the start (the iOS agent's build and the
Simulator; the Mac's usual is 3-9, partly my own previous run): do not trust those rows (they were not re-run: the maintainer stopped the
sweep). The panel's draw (present column, about 1.5-2.5 ms here) is in every number; the iPhone has no side panel.
**Missing: gpu at Resolution 8 (all scenes), vcore 8 room uncapped.** The sweep stopped at 62 of 72 runs.

Scenes: title = the 3D Reshiram with the display-capture fade, which both renderers draw with vcore's CPU 3D at 1x by design (docs/gpu.md),
so the title measures vcore 1x plus a scaled present, nearly independent of Resolution and renderer; dialog = Juniper + Minccino; room =
the player's room (3D, Cheren and Bianca talking). Key scripts (frames = the V-blank count, deterministic): `BWPORT_KEYS="start@f600
start@f900 start@f1300 a*4@f1700 a*4@f1730 a*4@f1760 ... "` = A held 4 frames every 30 frames from f1700; up to `a*4@f5780` it reaches the
dialog (measured from f5900), up to `a*4@f11000` the room (from f11100); the title is `start@f600` alone (from f900). Nothing is pressed after.

| renderer | res | scene | work ms med / p99 | game / render / present (med) | worker med | GPU busy med | cores | capped reps (work med) | uncapped fps | load before |
|---|---|---|---|---|---|---|---|---|---|---|
| gpu | 1 | title | 7.7 / 12.6 | 1.6 / 4.5 / 1.4 | 3.7 | 0.6 | 0.6 | 7.8 , 7.7 | 157 | 3 3 3 |
| gpu | 1 | dialog | 5.8 / 10.4 | 0.8 / 2.6 / 2.3 | 4.3 | 0.9 | 0.6 | 5.8 , 4.0 | 384 | 3 3 3 |
| gpu | 1 | room | 3.9 / 8.5 | 0.6 / 1.3 / 2.0 | 3.3 | 0.8 | 0.4 | 3.9 , 3.0 | 358 | 3 3 4 |
| gpu | 2 | title | 11.7 / 23.8 | 2.4 / 7.0 / 2.1 | 5.8 | 0.7 | 0.9 | 12.7 , 11.7 | 79 | 46 37 33 DISTURBED |
| gpu | 2 | dialog | 4.0 / 6.8 | 0.5 / 1.7 / 1.8 | 1.6 | 1.0 | 0.3 | 3.6 , 4.0 | 254 | 27 21 15 DISTURBED |
| gpu | 2 | room | 5.1 / 10.4 | 0.5 / 1.7 / 2.6 | 3.1 | 1.6 | 0.4 | 5.1 , 5.1 | 217 | 12 18 11 DISTURBED |
| gpu | 4 | title | 9.1 / 13.9 | 1.6 / 5.4 / 1.6 | 4.5 | 0.6 | 0.7 | 9.1 , 9.0 | 123 | 26 22 20 DISTURBED |
| gpu | 4 | dialog | 4.9 / 7.7 | 0.6 / 1.8 / 2.7 | 2.4 | 1.9 | 0.3 | 4.9 , 5.3 | 215 | 18 13 10 DISTURBED |
| gpu | 4 | room | 7.4 / 9.7 | 0.4 / 1.2 / 5.7 | 3.0 | 4.5 | 0.4 | 7.3 , 7.4 | 125 | 9 6 6 |
| vcore | 1 | title | 8.5 / 10.9 | 1.8 / 5.1 / 1.4 | 4.2 | 0.6 | 0.7 | 8.3 , 8.5 | 163 | 8 8 7 |
| vcore | 1 | dialog | 5.2 / 9.3 | 0.8 / 1.9 / 2.5 | 4.0 | 0.8 | 0.6 | 5.2 , 3.6 | 432 | 6 5 4 |
| vcore | 1 | room | 3.3 / 4.8 | 0.6 / 1.0 / 1.6 | 2.8 | 0.6 | 0.7 | 3.2 , 3.3 | 416 | 4 3 4 |
| vcore | 2 | title | 11.3 / 14.1 | 2.5 / 7.2 / 1.6 | 6.0 | 0.6 | 1.0 | 11.6 , 11.3 | 83 | 6 6 7 |
| vcore | 2 | dialog | 4.8 / 8.8 | 0.6 / 1.9 / 2.2 | 2.9 | 0.7 | 0.7 | 4.8 , 5.0 | 223 | 7 9 10 |
| vcore | 2 | room | 3.4 / 6.8 | 0.4 / 1.3 / 1.5 | 3.7 | 0.6 | 1.2 | 3.4 , 3.4 | 68 | 10 8 8 |
| vcore | 4 | title | 12.1 / 38.0 | 1.8 / 7.5 / 2.1 | 5.2 | 0.7 | 0.8 | 12.1 , 15.0 | 67 | 9 10 12 DISTURBED |
| vcore | 4 | dialog | 7.3 / 25.1 | 0.4 / 4.7 / 2.7 | 5.5 | 0.7 | 1.4 | 7.3 , 54.6 | 19 | 14 17 20 DISTURBED |
| vcore | 4 | room | 10.2 / 39.9 | 0.4 / 6.8 / 2.6 | 10.2 | 0.8 | 3.0 | 10.2 , 13.9 | 77 | 23 32 32 DISTURBED |
| vcore | 8 | title | 9.2 / 12.8 | 1.3 / 5.9 / 1.9 | 4.2 | 0.7 | 0.7 | 9.2 , 9.2 | 112 | 7 7 8 |
| vcore | 8 | dialog | 10.5 / 14.8 | 0.3 / 4.6 / 5.6 | 10.3 | 1.0 | 4.3 | 10.5 , 11.1 | 85 | 8 11 15 DISTURBED |
| vcore | 8 | room | 25.6 / 38.8 | 0.6 / 22.7 / 2.1 | 25.9 | 0.7 | 6.8 | 25.6 , 37.3 | - | 16 18 DISTURBED |

Reading it: at Resolution 1 both renderers use 3-9 ms of the 16.7, the title being the costliest scene (display capture, 3D at 1x on the
CPU; game logic 1.6-2.5 ms). The gpu path's CPU cost is flat in Resolution (0.3-0.7 cores; GPU busy in the room 0.8 ms at 1x, 1.6 at 2x,
4.5 at 4x), vcore's grows: the room's draw is 2.8 ms at 1x, about 10 ms at 4x (flagged), 26 ms at 8x (about 40 fps, 6.8 cores busy); the
dialog 10 ms at 8x (4.3 cores). vcore at 2x costs little more than 1x. Unflagged capped pairs agree within about 0.2 ms except a config's
first run (warm-up: dialog and room at 1x, 5.2 vs 3.6 ms).

**ESTIMATE (not measured; the phone is not here yet).** Guess: an iPhone core is about 1.3-1.6x slower than an M2 performance core and its
GPU 1.5-2x slower, so per-frame cost is 1.5-2x these numbers, before thermals. Renderer=gpu at Resolution 1-4 (CPU 3-9 ms here, one main
thread plus a small worker) should hold 60 fps. gpu at 8x is unmeasured: the room's GPU time grew 0.8 -> 1.6 -> 4.5 ms for 1x/2x/4x, so 8x
is perhaps 15 ms on the M2 and probably not 60 fps on a phone. Renderer=vcore should hold 60 at 1x and 2x (3-12 ms), is doubtful at 4x
(about 10 ms here, flagged, 3 cores) and fails at 8x (26 ms, 7 cores: more than 2 performance cores give, and efficiency cores will not
carry the raster threads). The title (8-12 ms at any setting) is the scene to watch. Uncertain: sustained thermals (the phone throttles
after minutes), scheduling of vcore's threads on efficiency cores, the GLES 3.0 path (no desktop GL timer queries, driver stalls in
`swap`), the full-screen Retina present. Suggested first phone setting: gpu, Resolution 2 (4 if the room stays under about 10 ms).

### Room only, live-switch sweep (2026-11-01, later)

`tools/mac_perf.py matrix --scenes room`: one run from the PC's `room` save (the idle player's room, not the earlier room with Cheren
and Bianca talking), 8 configs stepped live, one capped and one unlimited window each; 405 s wall in all (3 scene runs of 139 / 109 /
96 s: the first two had configs disturbed and were re-run), the other agents' builds and Simulators kept the load at 5-18, so most
rows are still flagged (DISTURBED = other processes used 1 core or more during the window, or the load rose by 2). Fills the two
missing cells: **gpu 8x room 15.8 ms work, 69 fps uncapped (GPU busy 12.2 ms; flagged)** and **vcore 8x room 33.6 ms, 28 fps (6.6
cores; clean)**. The other rows agree with the table above within its noise (gpu 4x 8.9 vs 7.4 ms, vcore 4x 7.0 vs 10.2). One capped
rep only. The title and dialog scenes were not re-run.

| renderer | res | scene | work ms med / p99 | game / render / present (med) | worker med | GPU busy med | cores | capped reps (work med) | uncapped fps | load before |
|---|---|---|---|---|---|---|---|---|---|---|
| gpu | 1 | room | 4.1 / 10.6 | 0.6 / 1.2 / 2.2 | 3.3 | 1.0 | 0.4 | 4.1 | 303 | 7 7 |
| gpu | 2 | room | 4.8 / 10.7 | 0.6 / 1.1 / 2.9 | 4.0 | 1.9 | 0.4 | 4.8 | 198 | 5 5 DISTURBED |
| gpu | 4 | room | 8.9 / 13.5 | 0.5 / 1.3 / 6.9 | 6.0 | 5.7 | 0.6 | 8.9 | 124 | 6 6 DISTURBED |
| gpu | 8 | room | 15.8 / 22.7 | 0.4 / 1.6 / 13.8 | 5.9 | 12.2 | 0.5 | 15.8 | 69 | 6 6 DISTURBED |
| vcore | 1 | room | 3.2 / 8.6 | 0.6 / 1.0 / 1.6 | 3.6 | 0.6 | 0.8 | 3.2 | 404 | 4 4 DISTURBED |
| vcore | 2 | room | 3.3 / 10.0 | 0.4 / 1.2 / 1.5 | 4.9 | 0.6 | 1.4 | 3.3 | 240 | 12 5 DISTURBED |
| vcore | 4 | room | 7.0 / 12.5 | 0.4 / 5.0 / 1.5 | 10.2 | 0.8 | 3.5 | 7.0 | 95 | 5 5 DISTURBED |
| vcore | 8 | room | 33.6 / 53.5 | 0.7 / 30.5 / 2.2 | 33.8 | 0.7 | 6.6 | 33.6 | 28 | 9 10 |


## Settings combinations: the pacing audit (2026-11-01, M2 macOS; libntr-ios-pace.patch)

The maintainer's report from the iPhone 16 Pro Max (A18 Pro, 120 Hz ProMotion): some combinations of settings cost far more than their
extra work. True for five of them. Four were bugs and are fixed. The fifth is the iOS present path: it stays open until the phone test
below has run.

### The per-frame pipeline (one game tick = one `SIM_Render` call, sim_main.cpp)

Everything below runs on the game's own thread, which also owns GL; only the renderers' workers run beside it.
1. The game's logic until it waits for the V-blank (`game logic`), then events (`SDL_PollEvent`; on iOS that pumps the UIKit run loop),
   touch, the ImGui frame.
2. `SpPaceBegin`: capped (60 / 120 / Monitor, no fast forward): tick n is due at t0 + n x 16.715 ms / GameSpeed; present slots are
   t0 + phase + k x Q (Q = 16.715 ms at 60, 8.36 ms at 120, the display's period at Monitor); the slots inside [due(n), due(n+1)) are
   the tick's presents: 0 (Game speed above the frame rate: the tick is neither gathered, drawn nor presented), 1, or 2 (1x at 120).
   Uncapped (Unlimited, fast forward): every tick presented (now: fast forward at most 60 a second, fix 3).
3. `SIM_FrameSkipDecide`: no slot = not drawn; FrameSkip N / auto may skip a tick that has one (the picture before is shown again).
4. The renderer call. vcore: gather the DS state (VRAM, registers, the 3D list: a copy), join the worker that drew the last frame,
   start the worker on this one (vcore's task threads: 3D, two 2D engines, N x tiles), return the LAST frame's picture (one frame of
   latency); libntr uploads it (`glTexSubImage2D`, 256N x 384N RGBA: 0.4 MB at 1x, 1.6 at 2x, 6.3 at 4x, 25 at 8x) and draws the
   screen quad. gpu: gather, join the last worker (a `pthread_create` per drawn frame), start the worker on this frame's CPU half
   (geometry, span records), then run the LAST frame's GL half here (3D passes, AA resolve, compose), return its texture; quad.
5. The game's H-blank and V-blank handlers, the V-blank count; ImGui draws (iOS: the touch controls and the overlay).
6. Present: `SpWaitSlot` sleeps to the slot (Monitor: 1.5 ms early, as its swap waits for the V-blank), `SDL_GL_SwapWindow`.
7. The tick's other slots (1x at 120 / Monitor 120 Hz): Smooth motion's in-between picture (gpu: `InterpTexture` runs the mid's GL half
   now, waiting for the worker's CPU half by spinning with `sched_yield` up to the slot less 1.5 ms; vcore: the worker drew the mid after
   the frame, it is uploaded now), else the same picture again; quad + ImGui again + swap.
8. Sleep to the next tick's due time (absolute deadlines: a slow tick is caught up; more than 3 ticks behind restarts the timeline,
   which loses that time for good), then the sound step (acore: one frame of the ARM7).

No readback in normal play (`glReadPixels` only for screenshots, BWPORT_PRESENTHASH, BWPORT_GPU_CHECK). Display-capture frames (the title,
some fades) are drawn by vcore at 1x on both renderers. Double pacing: only Monitor on the desktop (own sleep, then a vsync'd swap): the
hidden Mac window's swap then blocks 13-15 ms a present (820-920 ms of each second in `swap`); the speed held (0.99-1.00x), the cost is
latency. iOS has no swap interval (below).

The iOS present path (SDL 2.32's UIKit code, ~/bw-cache/ios-deps/src/sdl2/src/video/uikit): `SDL_GL_SetSwapInterval` is not
implemented there (libntr asked for 1 at Monitor every frame and failed each time; fixed), and `SDL_GL_SwapWindow` is
`[EAGLContext presentRenderbuffer:]`: Core Animation shows the renderbuffer at a later display refresh, never torn, so "swap interval 0"
does not mean "no vsync" there; presenting faster than the panel makes GL wait for a free drawable (Apple's EAGL model; the phone test's
`gpu2-unlimited` row shows it: presents a second stuck near 120 with most of each second in `swap`). SDL's view controller creates a
`CADisplayLink` with preferredFrameRateRange (80, 120, 120) when the screen reports more than 60 Hz (CADisableMinimumFrameDurationOnPhone
in Info.plist), which keeps ProMotion at 120 Hz while the app runs; SDL reports 120 Hz, so Monitor = 8.333 ms slots and 120 = 8.358 ms
(the same within 0.3%). At 60 the DS's 59.83 Hz on a 120 Hz panel holds one picture for three refreshes every ~5.7 s. Our presents are
timed by our own clock with no V-blank phase on iOS (Monitor's phase follow works only when the swap blocks), so a present near a refresh
boundary can land a refresh early or late; a CADisplayLink timestamp would give the phase (not done).

### How measured

`BWPORT_PACELOG=1` (a line a second) and the cfgscript's `bench=NAME` step (4 s windows: the game speed reached, presents, renderer draws,
in-between pictures, picture upload MB, ms in swap, the longest gap between presents, the process's CPU in cores and per game tick); new
cfgscript keys `speed= frameskip= smooth= ff= restore= perftest=` (the maintainers' notes, Mac). Hidden window, `--save town` with a
left/right walk, no frame skip, SDL dummy audio. Work counts and CPU per tick are the numbers to trust (the Mac was shared; one run at
load 110 is not used). Before/after: the same script with the binary before and after the fixes, two rounds each, load 3-20.

### Cliffs (before -> after; the two rounds agree within a few %)

| combination | before | after | root cause and fix |
|---|---|---|---|
| Game speed 2-4 + Frame skip 1-3 (at 60 or 120) | gpu 2x, speed 4 + skip 3: **0.5 pictures/s**; speed 4 at 120 + skip 1: 1.2/s; speed 2 + skip 1: 59.7/s (skip had no effect) | 15.0 / 60.0 / 29.7 drawn/s | FrameSkip drew the ticks whose V-blank count is a multiple of N+1; at Game speed S only every S-th tick has a present slot, and whether the two sets meet depends on the timeline's phase: often never, so the picture froze until the 104-skip guard (~2 s). Fix 1: N counts presented ticks when not every tick presents (the tests' all-ticks runs keep the V-blank count) |
| Fast forward, any renderer | gpu 2x **5.9x** (355 presents/s, 4.5 ms CPU/tick); vcore 4x **1.04x** (FF did nothing); iOS: at most the panel's 120 presents = 2x, skip or not (from the code) | gpu 2x **23x** (60 presents/s, 0.85 ms/tick); vcore 4x **5.4-6.6x** | FF drew and presented every tick, which costs more than the game's own logic (gpu 2x: ~3.6 of the 4.5 ms; vcore 4x: 105 ms CPU a draw). Fix 3: FF presents at most 60 a second; the ticks between are neither gathered nor drawn |
| Fast forward + Frame skip | gpu 2x + skip 3: 9.3-9.7x, half of each second in swaps of unchanged pictures (554 presents/s) | 24x | the same: skipped ticks were still presented |
| vcore + Frame skip at 2x-8x | vcore 4x skip 1: **376 MB/s** uploads (every tick, drawn or not) | 188 MB/s | a skipped tick uploaded the unchanged picture again (6.3 MB at 4x, 25 MB at 8x). Fix 4. CPU on the M2 is unchanged (the driver's copy is cheap here); on the phone it is memory bandwidth |
| vcore 4x + Smooth motion at 120 | **0.62x speed**, 75 presents/s, **0.2 in-between pictures/s** (6.5 cores) | 1.00x; smooth turns itself off (a log line) | the worker draws the frame and then its in-between picture before the next tick can start; when both don't fit in a tick, every tick runs late, the game slows down, and the late in-betweens are dropped, so all the extra work is thrown away. Fix 5: `SpInterpGuard` (a tick a quarter late turns them off for 2 s, then 4, 8 ... a minute) |
| Frame skip Auto at Game speed 2-4 | judged against 16.7 ms a tick | the tick at that speed (8.4 / 4.2 ms) | fix 2: auto skipped too little and the game fell behind (a timeline restart then loses time) |
| iOS overlay | "fps" counted game ticks: 120 presents at 1x read 60, Game speed 2 at 60 read 120, FF read hundreds | presents a second and the speed reached ("60 fps 2.0x"), green while the set speed holds | the maintainer judged combinations by a number that did not count presents. Fix 6 |

Explained costs (not cliffs), the sweep after fixes 1-4 (cores = the process's CPU; gpu rows leave out the GPU's own time):

| renderer | 60 | 120 | 120 + Smooth (mids/s) | Monitor (ms/s in swap) | speed 2 | speed 4 | FF | AA 4x |
|---|---|---|---|---|---|---|---|---|
| gpu 1x | 0.38 cores, 6.4 ms/tick | 0.38 | 0.63 (49) | 0.39 (862) | 0.50, 4.1 ms/tick | 0.56, 2.4 | 23x, 1.15 cores | 0.50 |
| gpu 2x | 0.45, 7.5 | 0.44 | 0.70 (58) | 0.43 (818) | 0.47 | 0.56 | 22.6x | 0.61 |
| gpu 4x | 0.49, 8.2 | 0.49 | 0.84 (52) | 0.47 (491) | 0.55 | 0.60 | 22.6x | 0.61 |
| vcore 1x | 0.86, 14.5 | 0.87 | 1.10 (60) | 0.74 (908) | 0.83 | 0.93 | 23.6x | - |
| vcore 2x | 1.74, 29.1 | 1.72 | 3.02 (56) | 1.76 (917) | 1.71 | 1.94 | 21.2x, 2.6 cores | - |
| vcore 4x | 6.26, 105 | 6.28 | 0.62x before fix 5 | 6.50 (829) | 6.59 | 3.81x (short) | 2.5x | - |

Reading it: Game speed 2 and 4 at 60 cost little (ticks without a slot are not drawn: CPU per tick halves and quarters). 120 without
Smooth adds a present of the same picture (no CPU cost here; on the phone a full-screen quad, the touch controls and a swap). Smooth
motion adds +60% CPU on gpu (0.38 -> 0.63 cores), +30% on vcore 1x and +75% on vcore 2x (each in-between is a full N x render on the
worker plus an upload). vcore's own cost grows about 4x per resolution step (0.9 -> 1.7 -> 6.3 cores): vcore 4x does not fit two
performance cores and is not a phone setting.

### Open

- The iOS present: whether `presentRenderbuffer` blocks at the panel's rate (it should, by Apple's model) and how own-timed presents
  meet a 120 Hz panel. The phone test answers the first; the second needs CADisplayLink timestamps if the maintainer sees judder.
- gpu Smooth motion waits for the worker by spinning (`sched_yield`, gpu_bridge.c InterpTexture) up to ~7 ms a slot: on a 2P+4E phone the
  spin competes with the worker. Not measured (the M2 has spare cores); try a short sleep if the phone's `gpu2-120-smooth` row costs much
  more than `gpu2-120`.
- Monitor on the desktop is double pacing (sleep, then a vsync'd swap): it holds the speed but costs up to a frame of latency.
- Fast forward now runs as fast as the logic allows (23x on the M2); the speed model below gives it a set speed.

### The phone test

Settings > About > **Run performance test** (port/src/ios_settings.m -> `SIM_PerfTestStart`): from where the player stands (somewhere
with 3D, e.g. a town), 27 rows of 1 s settle + 4 s measure, no frame skip, each row setting every key it depends on (renderer,
resolution, antialias, speed, frame rate, frame skip, smooth, fast forward, passes; trimmed 2026-11-03, libntr-ios-metaltune.patch):
gpu 2x at 60; gpu 3x at 60 with AA off, 4 samples and SMAA; gpu 3x Monitor + Smooth with AA off, 4 samples and SMAA (the maintainer's
play setting) and at speed 4 (what the fast-forward button sets) with AA off and 4 samples; Max fast forward at gpu 2x AA off and at
gpu 3x with 4 samples and SMAA; gpu 3x Unlimited with AA off and 4 samples (drawn/s bounds the GPU's frame); gpu 3x at 60 with 4
samples, AA off and SMAA with cfgscript `passes=1` (libntr-ios-passes.patch: each pass timed alone; a `passes:` line after the row gives the
CPU half's geometry, span records and 2D and the GPU's uploads, R, E+P, SMAA and compose in ms a frame: GL waits with glFinish
after each pass, Metal reads each pass's command buffer's GPU time); vcore 1x at 60. Every gpu row sets `renderer3d=exact` (the 3D renderer); the nine rows at 60, Monitor + Smooth and passes with AA off, 4 samples and SMAA run again right after their exact row with `renderer3d=fast` (name + `-fast`, libntr-ios-perfrows.patch), so one log answers Fast vs Exact and clock drift hits both alike; `restore=1` brings back the player's own renderer3d. About 3.2 minutes on the Mac (measured; the 18-row test was a third shorter), then the settings come back; opening
the settings stops it (`perftest: aborted`). The `passes:` times are wall clock at whatever clock the GPU runs at a 60 Hz load
(M2, load 2.5: R 6.8 ms with 4 samples where gpu_render's min at full clock is 2.8): compare them with each other and across
phone runs, not with gpu_render. "uploads" and "compose" are the GL thread's time to the GPU's finish, so they include CPU work on that
thread (the texture flatten and compare, the stage pack, the driver's conversion of the uploads), not GPU time alone. With Smooth
motion on, the CPU half is the keyframe's own (the in-betweens' geometry and records are not in it). A warm phone gives lower numbers: the `sys:` lines' clock shows it. The table (`bench:` lines) goes to the log: Share latest log. On the Mac: cfgscript `f1150:perftest=1` (75 s here).

### Recommendation: a simpler speed model

Today five knobs (Game speed, Frame rate, Frame skip, Smooth motion, fast forward) each change the pacing, and some pairs fought (above).
For the follow-up UI job:
- **Normal play is always 1x.** Replace "Game speed" with **"Fast-forward speed": 2x / 3x / 4x / Max** (default 4x). The FF button (and
  Space on the desktop) switches between 1x and that speed. FF at 2-4x is the existing Game speed timeline (`s_SIM_config.speed`): it
  presents at the frame rate and does not draw the ticks between, so it is cheap (speed 4 costs 0.56 cores on gpu 2x, less per tick than
  1x), and the sound follows it with the music on its real-time bus. "Max" is today's fast forward (after fix 3). The FF button shows the
  speed it gives ("4x") and is drawn like the other touch buttons (shape, colour, press state) instead of a separate toggle.
- **Frame rate becomes automatic**: 60 when Smooth motion is off (a second present of the same picture buys nothing), the display's rate
  (Monitor: 120 on the phone) when it is on. Unlimited stays a desktop/debug choice (benchmarks), not a player setting on iOS.
- **Frame skip becomes one switch, "Skip frames when slow"** (today's Auto, default on); fixed 1/2/3 go: FF no longer needs them, and at
  1x they only lower the picture rate, which Auto does when it must.
- **Smooth motion stays, as a Display option** (a look, not a speed): the overworld's 3D runs at 30 fps and Smooth gives it 60/120 for
  +60% CPU on gpu. Default on for gpu, off for vcore at 2x and above; it now turns itself off while the game can't keep its speed.
- **Keep "Music at normal speed"** (default on) as the only sound-and-speed knob.

The player then sees: Renderer, Resolution, AA, the look options, Smooth motion, FF speed, Skip frames when slow, Music at normal speed.

## iOS input and the run loop (2026-11-01, libntr-ios-input.patch, port/src/ios_input.m)

The maintainer's three responsiveness reports (iPhone 16 Pro Max, 120 Hz, build bac65fb61) and what was found:
- **Buttons dead after changing settings**: a finger-slot leak, not the run loop. The finger that taps the menu button lifts while
  the sheet is up and SimTouchSettingsWait flushed its SDL_FINGERUP; sim_touch.cpp's table has 10 slots, so the tenth opening was the
  last and every touch after it (the menu button too) was ignored while the game ran on. Reproduced in the Simulator (12 menu taps
  with the finger lifting during the sheet: 10 openings, then no pad change); fixed: the sheet's close forgets all fingers.
- **Jittery sheet**: its wait was SDL_PumpEvents + a 10 ms sleep, so UIKit ran every 10-11 ms at a phase unrelated to the display
  (8.3 ms frames at 120 Hz). Now the run loop sleeps until its next event (bwport_ios_settings_run). Simulator (60 Hz): a CADisplayLink
  callback's lateness spread with the sheet idle 6-7 ms -> 0.1 ms. Not explained yet: the phone logged once "invalid mode
  'kCFRunLoopCommonModes' provided to CFRunLoopRunSpecific" on the 6th sheet opening. Neither our code nor SDL runs a common-modes loop
  (grep), and the Simulator never logged it: it comes from UIKit/the system; watch whether it still shows with the new wait.
- **Button lag up to ~1 s**: no cause shown yet; the Simulator is too slow to show it (~25 ticks/s, so its ~100 ms finger-to-present
  is the expected 3 ticks; the desktop gives 48-59 ms at 60). Measures for the phone: the overlay's Detailed mode shows `input N ms
  (UIKit M)` for the last press (finger on the glass to the first present that can show the game's answer, two ticks after the tick
  that set the pad bits; M = how long UIKit held the touch before SDL got it, i.e. the wait for the next pump plus anything the system
  held); presses over 100 ms log an `input:` line; BWPORT_PACELOG adds `pace input` and `pace runloop` (turns, longest gap, display
  frames serviced / missed, callback lateness) each second. Read it as: M large = touches wait before the game sees them (pump
  starvation or the system); total large with M small = our pipeline (slow ticks, frame skip, presents queued).
- **Smooth motion never ran on the phone** (the maintainer's perf-test log: every Smooth bench row 0 in-between pictures at 1.00x; "off for
  60 s" about once a minute): SpInterpGuard took one tick starting a quarter tick late as falling behind. A present that waits for the
  120 Hz panel can run into the next tick (the bench's longest present gap 10-13 ms at 120 against 8.3; 18-20 at 60 against 16.7: a
  few ms of jitter, not a dropped refresh), so it tripped at random and the backoff climbed to a minute. Now: off when a second's ticks
  fall under 95% of the speed (Mac: gpu 2x 120 keeps 55.6 mids/s; vcore 4x still trips at 0.62x).
- Open: driving frames from CADisplayLink (phase-locked presents, no present running into the next tick) is not done; it needs the
  phone's `input` / `pace runloop` numbers first. The phone's "Unbalanced calls to begin/end appearance transitions for
  SDL_uikitviewcontroller" at start is SDL's window setup (harmless).

## Sound quality cost (2026-11-01, M2 macOS; docs/acore.md "Sound quality")

acore per second of audio (offline render of a 41.8 s town stream; machine at load 100, so +-15 %): DS (exact) 11.6-12.9 ms, Enhanced
16.8-20.8 ms (16-tap sinc per active PCM/ADPCM channel, the 16-bit mix). The device-rate resampler (64-tap sinc, 12 M multiply-adds per
second at 44.1 kHz) 3.3 ms per second of output, the old cubic 0.8. Enhanced in all about 2 % of one core.

### Input ahead of the game (2026-11-01, later; libntr-ios-inputprio.patch, port/src/ios_input.m)

The maintainer (iPhone 16 Pro Max, build 1c9d740fb): Down, Left and B stayed pressed about a second after every finger had lifted (the
player kept walking); the overlay read `input 60+ ms (UIKit 25)`.
- **Stuck release: the system's gesture gates.** The game's UIWindow carries `_UISystemGestureGateGestureRecognizer` (one of the two
  with delaysTouchesBegan and delaysTouchesEnded on) and `_UIHomeAffordanceGateGestureRecognizer` (delaysTouchesEnded on), logged in
  the Simulator. While a gate decides whether a touch near an edge is the system's swipe, UIKit holds that touch's END back from the
  view (SDL's touchesEnded) until the gate gives up. The stuck buttons were the bottom ones (B at y 895, Down ~891 of 956; the home
  indicator's band ends at 922). SDL already defers the system gestures on all edges (fullscreen + borderless window: its view
  controller returns UIRectEdgeAll; logged `edges deferred 15`), so the hint SDL_HINT_IOS_HIDE_HOME_INDICATOR=2 would change nothing;
  deferral is what makes the gate wait for a second swipe, and it is the recognizers' delays that held the ups. Now every recognizer on
  the window and the root view has both delays off (bwport_ios_touch_setup; again after a layout change or the settings sheet: logged
  only when it changes one). Not reproducible in the Simulator (simctl has no real touches): the phone's log decides. Every release
  that reaches the pad bits more than 50 ms after the finger left the glass now logs `input: release N ms late (glass to app A, app to
  touch layer B, touch layer to pad C)`: A = iOS/the main thread held the touch (UITouch.timestamp to -[UIApplication sendEvent:]),
  B = held inside the app after that (a gesture gate's delay), C = the touch layer to the registers. A lost up would show no line
  and the button stuck for good; a gate shows B large; a busy main thread A large. BWPORT_TOUCH_LOG=1 logs every finger down and up.
- **Input no longer waits for a frame.** (1) The touch layer takes each finger the moment SDL gets it: an SDL event filter on the main
  thread (SDL_SetEventFilter, sim_touch.cpp) handles finger events inside UIKit's delivery and drops them from the queue, so the
  button state, the menu and the FF button change at delivery; events pushed from another thread (the key script on Linux/Windows) still
  go through the event loop. (2) On iOS the game's frame-time waits (SpSleepUntil: the slot wait before a present, the in-between
  slots, the wait for the next tick) run the main run loop (bwport_ios_runloop_for: CFRunLoopRunInMode in the default mode, then SDL's
  GL context made current again, as SDL's pump does) instead of nanosleep, so UIKit delivers touches while the game waits instead of
  at the next frame's pump. (3) The touch buttons are written to the pad registers again after the waits (SimTouch_PadNow), so a
  finger that came during them reaches the tick that starts next, not the one after (one tick less). (4) The game still reads the pad
  once a tick, as on a DS. Background: a wait stops running the run loop once SDL's resign-active callback has flagged it, and the
  presents after a wait are skipped then (no GL in the background).
- **Measured** (Simulator; it runs ~25 ticks/s, slower than the pace, so the waits only exist with the pace slowed 10x by a local
  hack, 6 ticks/s, not committed): the key script's finger ups pushed on the main queue (the route UIKit's touches take) reached the
  touch layer in median 6 ms (7 of 16 at 0 ms, the rest waited for a busy tick) with the run-loop waits against median 111 ms
  (max 144) without. Desktop BWPORT_TOUCH: 15-17 ms (the next pump). Long frame: `cfgscript stall=3000` with two fingers lifting
  during it: both ups reached the pad right after the stall (`input: release 3027 ms late (glass to app 0, app to touch layer 3027,
  ...)`), nothing stuck or lost. A frame that holds the main thread holds UIKit's delivery: the game runs on the main thread (SDL 2
  on iOS), so input that comes during a 500 ms frame is taken when it ends, before the game's next read; the game could not have
  acted sooner. Moving the game off the main thread would let the touch layer run during it, but nothing acts on input between
  ticks except the overlay's drawing, which needs the game thread anyway: not done.
- **The overlay's `input N ms (UIKit M)`** keeps its meaning: N = finger on the glass to the first present that can show the game's
  answer (two ticks after the tick that read it), M = glass to the app (sendEvent). Expect M to drop to the system's own delivery
  latency (a refresh or so) plus whatever of the tick's work the touch lands in (on the phone ~8 ms of a 16.7 ms tick at gpu 2x).
  N cannot go under about two ticks plus a refresh (~40 ms at 60): the renderers show the last tick's picture.
- **Desktop**: the keyboard and the gamepad are read in SIM_Render's event loop at its start, and its waits sleep, so a key that comes
  during the render, the present or the wait reaches the game a tick later than it could (up to ~16 ms at 60; presents don't block:
  swap interval 0 except Frame rate=Monitor). Not changed: the fix is moving the event loop after the waits (SIM_Render's input block
  and its mouse/stylus code), a larger change in shared code. The desktop has no stuck-release problem (no gesture gates).
- Left: the bottom screen's stylus is still written at the frame's start only (a touch there that comes during the waits is read a
  tick later); the phone log has to show whether the gates were the stuck release (`input: release` lines, B large or not).

### The queue between drawing and the glass (2026-11-01, later; libntr-ios-lag.patch)

The maintainer (iPhone 16 Pro Max, gpu 2x, AA 2x, Smooth motion on): at times the controls feel queued, the player walks on for up to
about a second after every finger lifted (any D-pad direction), and the touch buttons' own highlight comes on and goes off late too.
The highlight is drawn from the touch layer's live state into the same GL frame as the game, so the picture itself is old: a
backlog after drawing (GPU work or presents) or presents shown late, not the game reading input late.

Every queue on the path and how deep it can get (code; Mac measurements):
- **Touch layer**: state, not a queue (live mask + a 3-frame latch per press; releases reach the pad at delivery since inputprio).
- **Game ticks**: one pad read a tick, as on a DS. Catch-up after a slow tick: absolute deadlines, at most 3 ticks behind, then the
  timeline restarts (SpPaceBegin): at most ~50 ms of back-to-back ticks.
- **Renderers**: one tick of latency by design (vcore and gpu return the last tick's picture; gpu_bridge.c joins the worker every
  drawn frame: two slots, no queue). Smooth motion's in-between is drawn after the frame's present and shown at the next slot; its
  wait (InterpTexture) ends 1.5 ms before that slot and the next tick is on the absolute timeline: it cannot push a real frame later
  than one present.
- **Sound**: the sound clock follows the game's frames (soundclock patch) and never blocks the game loop: not a pacing source.
- **The game itself** finishes a step after the release. Desktop baseline (vcore 1x, room save, picture hash per present): a hold of
  1000 ms moved the picture for 1035 ms, 500 ms for 638, 200 ms for 369: the player stops at most about one tile (~270 ms) after the
  release, mostly 50-200 ms. A full second is not the game.
- **GPU work** (new measure): a GL fence per present (just before the swap), polled with timeout 0 each frame and about every
  millisecond while the pacing sleeps. Mac (M2, Apple GL 4.1 on Metal, gpu 2x AA 2x Smooth 120, town walk): GPU lag (swap call to
  GPU done) 5.8-6.8 ms mean, 12-20 max, at most 2 in flight. Forced slow GPU (BWPORT_GPUSLOW=2000, ~45 ms of GPU a present): no
  bound **46-50 ms lag, 4 in flight** (macOS's GL stops the CPU itself at 3-4 frames: swap 200 ms/s); bound 2 (the default) 26-27 ms,
  2 in flight; bound 1 14 ms, 1. The bound costs nothing while the GPU keeps up (bound wait 0 ms/s at normal settings). Releases under
  the slow GPU, glass to GPU done: 113-136 ms without the bound, 108-122 with it. Apple's OpenGL ES on the A18 may let more frames
  queue than macOS's GL does; the bound holds it at 2 on every platform (BWPORT_GPUQUEUE=N, 0 = off).
- **Presents waiting for the panel** (iOS, an estimate): EAGL gives no presentation time, so a model from the CADisplayLink's phase
  and period: a present is on the glass at the first refresh after its GPU done, one picture a refresh. If presents outrun the panel
  (the panel slower than the pacing assumes: Low Power Mode caps ProMotion at 60 Hz, ProMotion lowering the rate) and the swap does
  not block, presents queue there; when the model says two are already waiting, the present is dropped (BWPORT_PRESENTDROP=0: never)
  and the log says `display: N presents dropped ... (the panel at H Hz ...)`. Simulator (60 Hz, software GL, load 65): the panel
  read 60.0 Hz, shown lag = GPU lag + up to one refresh, 0 waiting, no drops; the overlay line fits. Unverified on a phone.
- Resolution: a fence's GPU-done time is when it was polled. While the pacing sleeps that is within ~1 ms; when ticks run back to
  back (no sleeps: the Simulator, a game slower than its speed) it is the next frame's start, so the GPU lag reads high by up to a
  tick there.

What to read on the phone (Overlay, Detailed):
- `input N ms (UIKit M)`: press, glass to the present call (as before).
- `to GPU: press P, release R ms`: glass to the GPU done with the first picture that can show the game's answer.
- `display lag S ms (GPU G) / K in flight, W waiting, H Hz` (the last second): S = swap call to the estimated refresh that shows
  it, G = swap call to GPU done, K = most frames submitted and not GPU-done, W = most presents waiting for the panel, H = the
  panel's real rate (CADisplayLink).
- Log: `input: press|release shown N ms late (glass to pad A, pad to tick end B, to present C, to GPU done D; K in flight)` when N
  passes 100 ms (every one with BWPORT_LAGLOG / BWPORT_PACELOG); `display: N presents dropped`; the pace log adds `pace gpu` and
  `pace panel` lines.
- Run performance test (`bench:` rows in the shared log): new columns `gpu lag ms`, `max`, `in flight`, `shown ms`, `drops`.
**Smooth motion on: presents outran the panel** (libntr-ios-panel.patch). a test phone: the lag comes only with Smooth motion
on, on vcore and gpu alike; off, none. Smooth on means Frame rate=Monitor, and Monitor's slot length came from SDL's display mode,
which on iOS is the screen's maximum (120) whatever the panel runs at for us; the earlier perf test showed 119.6 presents/s with
~20 ms/s in swap, so the swaps do not wait for the panel. If the panel runs slower (Low Power Mode caps it at 60, ProMotion or heat
lowers it), every second present has nowhere to go and EAGL queues it. Now Monitor's slot length is the CADisplayLink's refresh
period (port/src/ios_input.m, which asks ProMotion for 80-120 Hz, preferred the maximum), rounded to whole Hz (`bwport: the panel
runs at N Hz` when it changes), so at 60 the tick has one slot and no in-between picture; the slots are phase-locked to the
link's refreshes (each present ~1 ms after one) instead of our own clock. The drop above stays as the safety net. Mac check with
BWPORT_PANELHZ=N (pretends the rate; every platform): gpu 2x AA 2x Smooth at Monitor, panel 60: 60.0 presents/s, 0 in-between,
1.00x, GPU lag 16.7 ms; Smooth off at 60 unchanged (59.6 presents/s, 1.00x, lag 8 ms). The same mismatch on the desktop (Monitor
swaps wait for the vsync) slowed the game instead: BWPORT_PANELHZ=120 on the hidden window's 60 Hz gave 0.42x at 60 presents/s (since libntr-ios-ffpace.patch PANELHZ on a desktop pretends a whole display link and the swaps stop waiting: below).
Simulator (60 Hz link, Smooth on at Monitor, vcore): `the panel runs at 60 Hz`, 0 waiting, no drops, shown lag 12-24 ms. It still
presented twice a tick: it runs at ~10 ticks/s, so the timeline restarts every tick, and a restarted tick's window [0, 16.715 ms)
holds two 16.667 ms slots (true before this change too; at speed a tick gets two slots only every ~350 ticks). Unverified on a phone:
the maintainer's overlay should read `60 Hz` or `120 Hz` in the display lag line, with 0-1 waiting and no `display: ... dropped` lines.

Reading: G or D large with K at 2 = the GPU is the bottleneck (lower AA or Resolution); W at 2 with drops, or H below 120 = the panel
runs slower than the presents (Low Power Mode, heat); all small while the lag is still felt = before the present (A, B, C).

### Fast-forward toggles (2026-11-01, later; libntr-ios-ffpace.patch)

The maintainer (iPhone 16 Pro Max, build cf307c5a6, gpu 3x, Smooth on at Monitor, FF 4x, later AA 2x): after each FF on/off, more
`display: N presents dropped` lines (1-5 a second at first, 13-38 after several toggles) and a slowdown felt to build up.

The FF button is Game speed 4x on the capped timeline (cfgscript `speed=4`/`speed=1`; `ff=1` is the uncapped Max path). On the Mac
with `BWPORT_PANELHZ=120` (now a pretended display link: the iOS present path with its slot lock and panel model, swaps not waiting
for the V-blank), gpu 3x, Smooth, Monitor, 20 toggles (4x for 480 frames, 1x for 120; `tools/mac_fftoggle.py`):
- **What accumulated: Smooth motion's backoff.** At 4x a tick (4.2 ms) is shorter than a 120 Hz slot, so no in-between picture is
  made, yet SpInterpGuard judged the speed: whenever 4x was not reached the backoff doubled (2, 4, 8, 16, 32, 60 s) and the 2 s of 1x
  between toggles never reset it (4 good seconds do). Before: 50-60 in-between pictures a second; after 20 toggles: 0, Smooth motion
  off at 1x for a minute. Its one-second window also counted ticks run at the old speed against the new one. Now ticks with no room
  for an in-between picture (fewer than 1.5 slots a tick) are not judged and a speed change starts the second afresh.
- **Why the drops: the panel model read late polls.** It stamped a present's GPU done at the poll that found it done. Smooth's
  in-between picture (gpu_bridge.c InterpTexture) spin-waits for its worker and draws without polling, until 1.5 ms before the next
  slot, i.e. just past the next refresh; the first present of each tick was then put one refresh late, the next chained behind it, and
  at two "waiting" a real present was dropped. With in-between pictures on: 11-19 drops per 4 s bench (3-5/s, the phone's baseline);
  with them backed off (after the toggles): 0-1. More GPU per present (AA 2x on the phone) lengthens the in-between and the chances,
  which fits 20-38/s there. It drifted no further with each toggle: it rose and fell with in-between pictures and GPU load. Now the
  model takes the earliest time the GPU can have finished (the last poll that found it busy, or the swap call): 0-5 per 4 s. Presents
  that really outrun the panel still pile up in the model's one-a-refresh chain and are dropped.
- **Timeline restarts** (every speed change, a sheet closing, a hitch) set the slot phase to 0, so presents aimed at "now" until the
  lock re-formed (e/8 a present, ~20 presents). With a link the restart now puts the slots on its refreshes at once.

`tools/mac_fftoggle.py` (benches before / after 20 toggles; speed, presents/s, drawn/s, in-between/s, drops/s):
- ae520fe9f + the pretended link only: 1.00 / 1.00, 116.5 / 119.9, 59.7 / 59.7, 50.4 / **0.0**, 3.4 / 0.1: FAIL.
- libntr-ios-ffpace.patch: 1.00 / 1.00, 119.9 / 119.9, 59.7 / 59.6, 52.7 / 60.2, 0.0 / 0.0: PASS.

Simulator (pool device, vcore 1x, Smooth at Monitor, 6 toggles; ~0.3x as it always runs there): `the panel runs at 60 Hz`, 0 drops
before and after, no crash. Not covered: the in-between picture's spin-wait itself (gpu_bridge.c InterpTexture, up to ~6.8 ms of
sched_yield a slot on the main thread, which also holds the run loop) is unchanged; only the model no longer misreads it.
On the phone: any number of FF toggles should leave the overlay's in-between count and the display line as they were before them;
`display: N presents dropped` lines should be rare (0-1 a second) at gpu 3x Smooth, AA 2x included, unless the panel really runs
slower (Low Power Mode, heat: the display lag line then reads below 120 Hz); `smooth motion off ... fell behind` should no longer
appear during FF, only when 1x itself cannot be held.

### Smooth motion: an even step every refresh (2026-11-24; sim_main.cpp SIM_InterpPace, port/bridge)

The maintainer (iPhone 16 Pro Max, gpu 3x, 4 samples, Metal, Smooth on, Monitor, HG field): 120 fps on the overlay, GPU 63-69% busy,
yet it "doesn't feel quite as smooth as 120 fps should". Measured on the Mac with the pretended 120 Hz link (BWPORT_PANELHZ=120,
the iOS present path; SoulSilver Cherrygrove walk, the phone's settings, 11.3 s windows, load 2-3) with BWPORT_PRESENTLOG, which now
logs per present the slot, the swap call, the game time the picture shows (the bridges' SIM_InterpShows) and the tick's due time,
an S line with the panel model's refresh for it and a D line per drop; the per-refresh histograms are a scratch script's.

- **The cause: every fourth picture showed the game up to a refresh early.** A slot's picture is the blend of the bridge's last two
  keyframes at f = (slot - key) / gap, so it shows the game at slot - gap. The next tick's first slot is drawn from this tick's
  state (the bridge's pipeline: GpuFrame draws the previous call's picture), and that slot comes x = 0..Q into the next tick
  (Q = 8.33 ms at 120 Hz). When the next tick brings the new keyframe (the HG field's 3D is 30 fps: every other tick), the slot
  lies past the newest keyframe the picture can have: f clamped at 1, the keyframe shown as it is, x too early. x slides 0.048 ms a
  tick (59.83 Hz ticks, 120 Hz refreshes) and wraps when a tick gets three slots, so the error sweeps 0-8.3 ms every 2.9 s: steps
  between refreshes of 8.3 - x and 8.3 + x. Before: **50.6% of the steps 8.3 ms**, the rest spread evenly over 0-16.7 ms; the
  shown picture 48 ms behind its refresh on 74% of the refreshes and 49-56 ms on the rest. 60 fps content has the same error on
  every tick's first slot; a 60 Hz panel the same with Q = 16.7 ms.
- **The fix:** SIM_InterpPace hands the bridges each slot's time less one slot period (Q) under Monitor, whose slots slide against
  the ticks: every picture then shows the game Q + gap before its slot, a constant, and the next tick's first slot always falls
  between keyframes it has. 60 and 120 (no link) keep their slots on the ticks (lead 0, unchanged). A blend at f 0 (the lead puts a
  slot exactly on a keyframe's tick start) now shows the older keyframe instead of the newer (both bridges). BWPORT_INTERPLEAD=0:
  as before. Cost: the 3D motion is shown one refresh later (the model's picture-to-glass 48 -> 56 ms).
- **Slots are handed out in a chain:** a tick's first slot is the one after the last one the tick before had (BWPORT_SLOTCHAIN=0:
  each tick takes the slots inside its own interval, as before). The phase loop moves the slots at every present to follow the
  link; a slot within that much of a tick boundary was counted by both ticks (two presents in a refresh) or by neither, and the
  next tick's first slot was not the one its picture was made for. No difference on the Mac's exact pretended link, nor with
  BWPORT_LINKJITTER=300 (a test aid: each reading of the pretended refresh off by up to 300 us); a real link's jitter is the case.
- **The fps number:** SIM_PresentRate counted over whole ticks (60 ticks = 1.0029 s hold 120 or 121 presents of a 120 Hz panel:
  "121 fps" a third of the time, none extra). It now counts from present to present: 120.

Results (before = BWPORT_INTERPLEAD=0 BWPORT_SLOTCHAIN=0 on the same build; even = steps within 0.6 ms of the mode):

| run | even steps before | after | presents/s before / after | drops / 11 s |
| --- | --- | --- | --- | --- |
| 120 Hz link, Smooth, 1x | 50.6% (0-16.7 spread) | 97.8%, 97.6% (8.3 ms; 15-17 stalls + 15 skips) | 119.5 / 119.7, 119.4 | 5 / 3, 7 |
| 120 Hz link, Smooth, Game speed 2x | 6.9% | 98.8% | 119.6 / 119.5 | 3 / 4 |
| 120 Hz link, Smooth, Game speed 4x | 97.0% (8.4) | 97.4% (8.3) | 119.6 / 119.6 | 0 / 0 |
| 120 Hz link, Smooth off | 50.2% (0 / 16.7 by design) | 50.1% | 119.1 / 119.0 | 12 / 13 |
| Frame rate 60, Smooth | 50.1% (0 / 16.7: 60 a second) | 50.1% | 59.9 / 59.9 | 0 / 0 |
| desktop Metal Monitor (the Mac's 60 Hz CVDisplayLink), Smooth | 3.8% | 99.7% (16.7 ms) | 60.1 / 60.1 | 0 / 0 |
| desktop GL Monitor (real vsync, no link), Smooth | | | 59.9 / 60.0 | 0 / 0 |

tools/mac_fftoggle.py (needs BWPORT_FFMAX=0 since mac_run.sh's default Max fast forward; in a worktree without build/port/macos the
run folder must be a clone of ~/bw-cache/runbase): PASS, 1.00 / 1.00x, 119.5 / 119.8 presents/s, 53.1 / 53.3 in-betweens/s, drops
0.38 / 0.00 a second.

What is left (Mac): the stalls and skips after the fix are presents that miss their refresh. A tick's first present waits for that
tick's game logic and render call (its picture is ready a tick earlier), so when its slot comes early in the tick it goes out late:
43% of first presents 1-9 ms late here (2-8 ms of work before it on the M2), about 45% up to ~4 ms on the phone (0.9-1.0 ms game +
1.3 ms render). The panel model then chains every later present a refresh behind (display lag ~14 ms, "1 waiting", the state the
phone's overlay shows), and the next miss is dropped. Presenting the next tick's first slot from this tick's loop (before the next
game logic) is the next step; the compositor's real behaviour on a miss is still the glass line's open question (docs/gpu-backends.md).

On the phone (Overlay, Detailed): the field walk should look steady, with no beat every ~3 s; the fps reads 120 (no 121); the
display line as before (lag 14-15 ms and 1 waiting while the model's chain stands); `display: N presents dropped` lines rare.

### Smooth motion: a tick's first present on time (2026-11-24, later; sim_main.cpp's early present, port/bridge gpu_bridge.c NextPicture)

What the entry above left: the stalls and skips after the even-step fix were presents that missed their refresh. Measured with the
same BWPORT_PRESENTLOG (Mac, pretended 120 Hz link unless noted, SoulSilver Cherrygrove walk, the phone's settings: gpu 3x, 4 samples,
Metal, Smooth, Monitor; 11.3 s windows; load 2-4). Before = ba8975fe0's build, after = this one, run in pairs back to back (the
M2 switched between ~2.3 and ~6.5 ms of CPU a tick during the session, so only a pair compares like with like).

- **The cause:** a tick's first present shows the picture the tick before prepared (the GPU path's pipeline), but it was made after
  the tick's game logic and renderer call, and that call also ran the picture's GL half (Stage2). The slots slide through the ticks
  (59.83 Hz against the panel), so when the first slot came within that work of the tick's start the present went out late and
  its GPU work went in late: 18% of first presents 3 ms or more late, 27% of all presents done after the refresh they were aimed
  at (the swap call or the GPU after it: the S line's done against the refresh after the slot's). Once one present lands a
  refresh late, every later one does too: one present a refresh never lets the panel's queue drain (FIFO), the overlay's
  "1 waiting" and a refresh more of display lag (14 ms instead of 6), and a present more than a refresh late is then dropped.
- **The fix:** with a display link, under Monitor, with the Metal present and the GPU path's pictures (Smooth on or off), when the
  next tick could not have its first present out by its slot, the bridge draws that picture ahead (SIM_SetNextTexture ->
  NextPicture: Stage2 now; the next call no longer draws it) and it goes out at its slot (less SpWaitSlot's 1.5 ms) as that tick's
  first present, which the tick then does not make (sPace.early0, found by the slot's time):
  - from this tick's loop, right after its last present, when the slot comes sooner than the next tick's game logic can be done
    (the longest of the last 64 ticks', from the cap's sleep to the top of the frame: SpTopMark); that logic waits for it;
  - else at the top of the next tick's frame, before its renderer call, when it comes sooner than that call and the rest can be
    done (the longest of the last 64 ticks', SpPreMark); that call waits for it.
  Either waits less than the part it stands in front of (logic 1-3 ms, the call 0-1 ms here: about 45% of first presents from
  the loop, 10% from the top). A later slot is left to the tick as before, its picture drawn in the renderer call: drawing every next picture right after
  the last present (a first version) put heavy GPU work right behind each present and lengthened the GPU's tail (p90 done - call
  +1 ms at Game speed 2x), and holding the next tick's logic back for the whole work made more in-betweens' CPU halves late. The
  "behind" rule does not count the wait (it never counted a tick's own present). GL's present is left out: its swap may wait for
  the panel (a pretended-link GL run: 95.6% -> 85.7% even steps with it on). BWPORT_EARLYPRESENT=0: off (the A/B).
- BWPORT_PRESENTLOG: a P line's 12th field is 1 for a present made from the tick before's loop, 2 for one made at the top of the
  tick's frame; an X line per decision (now, the slot less 1.5 ms, the next tick's due time, the longest logic and renderer parts).

| run (pairs, seconds) | first presents >= 3 ms late | presents done after their refresh | drops | repeats + skips on the glass | even steps | waiting, model's display lag |
| --- | --- | --- | --- | --- | --- | --- |
| phone's settings, 1x (4, 45 s) | 18% -> 2% | 26.9% -> 7.6% | 21 -> 11 | 68 + 66 -> 40 + 38 | 97.6% -> 98.5% | 100%, 13.1 ms -> 100%, 13.5 ms |
| Game speed 2x (2, 48 s) | 18% -> 1% | 27.2% -> 7.0% | 21 -> 7 | 73 + 68 -> 42 + 42 | 97.5% -> 98.6% | 100% both |
| Game speed 4x (1, 49 s) | 20% -> 0% | 29.8% -> 4.4% | 15 -> 5 | 58 + 55 -> 23 + 23 | 98.1% -> 99.2% | 100% both |
| Smooth off (1, 11 s) | 22% -> 0% | 31.8% -> 4.9% | 3 -> 3 | (a repeat every tick by design) | 50.1% both | 100% both |
| gpu 2x, no AA: the GPU keeps up (1, 11 s) | 13% -> 0% | 4.4% -> 0.1% | 1 -> 0 | 5 + 4 -> 1 + 1 | 99.3% -> 99.9% | 88%, 12.7 ms -> **0%, 5.6 ms** |
| the Mac's real 60 Hz link (CVDisplayLink), Smooth (3, 34 s) | 30% -> 22% (the rest: the sleep's overshoot, below) | 1.9% -> 0.3% | 0 / 0 | 2 + 1 -> 3 + 0 | 99.9% both | 98%, 29.6 ms -> 84%, 27.6 ms (below) |
| real 60 Hz link, Smooth off (1, 11 s) | 17% -> 3% | 1.2% -> 0.1% | 0 / 0 | 3 / 3 | 99.6% both | 96%, 29.8 ms -> 84%, 28.2 ms |
| Frame rate 60; Present=gl | unchanged (no early present: not Monitor; not Metal) | | | | | |

The pictures' timing is untouched: at 1x the shown picture is 56 ms behind its refresh in both (pa.py "display - game"). Where no
present misses, the queue never forms: gpu 2x no AA, the model's landing 5.8 ms after the slot for 1357 of 1359 presents (before:
14.2 ms from 2.4 s on, after a miss), 48 ms display - game. On the real 60 Hz link the queue formed in every before run within
0.04-1.7 s (a late first present); after, in all four runs of this build at 2.85 s, at the GPU path's pass S self-check (its
frame 300 draws the frame once more: one GPU miss), and not at all in the three pairs of an earlier variant of this fix that drew
every next picture early (waiting 0%, display lag 13.2-13.5 ms against 27.2-29.6 ms).

tools/mac_fftoggle.py (BWPORT_FFMAX=0, run folder a clone of ~/bw-cache/runbase): PASS, 1.00 / 1.00x, 119.95 / 119.75 presents/s,
59.45 / 59.65 in-betweens/s, drops 0 / 0.12 a second; --smooth 0: PASS. tools/check.sh: 9/10 (b2 reset-save: no save b2-champ in
this environment, as before).

What is left (Mac): the GPU at gpu 3x, 4 samples still finishes 6-8% of presents after their refresh (two such pictures a tick),
so the panel's queue still stands there; once it stands it absorbs misses of under a refresh (fewer drops), and only a refresh
without a present drains it (draining on purpose would cost a skipped picture a miss). The first presents still 1-3 ms late in
both builds are the sleep's own overshoot in this test setup: a game started from the agent's shell wakes ~25-50% of a sleep's
length late (timer leeway; a C test from the same shell shows it, a real-time thread does not), so the real 60 Hz link's long
sleeps show it most.

On the phone (Overlay, Detailed): "waiting" should read 0 and the display lag about 6 ms (one refresh and the GPU) instead of
"1 waiting" and 14-15 ms, from launch on; a real hitch (a load, the self-check's extra draw ~5 s after the renderer starts) can
bring "1 waiting" back, and it then stays until a pause (the queue drains only at a refresh with no present): that state is
harmless to smoothness. `display: N presents dropped` lines should be rarer, the fps 120.

### Stuck buttons (2026-11-02; libntr-ios-stuckinput.patch, port/src/ios_input.m)

The maintainer (iPhone 16 Pro Max, on-screen touch controls only, never a controller): with every fix above in and Smooth motion off,
buttons still stay held after the fingers left the glass at times (earlier: Down, Left and B for about a second). The session's
log was lost, so the job was making the next one say where the release went.

Read and ruled out (code): the touch layer's state cannot hold a button no finger holds (the live mask is recomputed from the
finger table at every finger event; a tap's 3-frame latch ends on its own); SDL sends a lost finger up itself when a UITouch
address comes down again, maps touchesCancelled to touchesEnded and drops an up only for a finger it never had; SDL's view is
multi-touch and no UIKit view sits over it; the sheet, the background and a turn let go of every finger; the pad register is
written only from the three source masks (PadSrcSet); ticks run back to back after a stall read the same state (no queue). Not
the game: a step finishes at most about one tile (~270 ms) after the pad lets go (desktop baseline above); the checks below time
the pad, not the picture, so a step never trips them. What the code cannot rule out, and the checks now watch: a recognizer that
holds the view's touch end back (bwport_ios_touch_setup only walked UIApplication.windows and their root views), a touch UIKit
never ends, a stall of the main thread, a phantom keyboard or controller source.

The checks (always on; lines only when something is off; "tick T" = the V-blank count):
- `anomaly: touch finger F (B+Down, kind 2, 840 ms) ended by UIKit but the end did not reach the game's view: let go, tick T`:
  after each touch event UIKit dispatched (-[UIApplication sendEvent:] wrapped), a touch it ended or cancelled whose finger the
  touch layer still holds: the view's touchesEnded was held back (SDL turns it into the finger up inside the dispatch). The finger
  is let go at once, so this also cures that case. `cancelled` instead of `ended` when UIKit cancelled it.
- `anomaly: touch gesture CLASS on VIEW delays touches (began 1, ended 1): switched off (N times)`: a recognizer a new touch met
  (UITouch.gestureRecognizers after the dispatch, wherever it sits) that delays touch starts or ends; logged the 1st, 10th, 100th
  time (one the system turns on again shows as a rising count).
- `anomaly: touch finger F (...) ended in UIKit N ms ago and no finger up came: let go` (the net under the first, once a frame);
  `... missing from UIKit's touch events for N ms, no end seen: still held` (UIKit's later touch events no longer list it; only
  logged); `... cancelled by UIKit (a recognizer or the system took it)` (SDL's touchesCancelled; SDL then sends the up);
  `touch finger F down again while the layer still holds it` (should never show).
- `anomaly: keyboard|controller holds B but the key state|controller reports it up (150 ms): let go` (the keyboard on iOS only,
  not under BWPORT_KEYS / BWPORT_VPAD; the controller on the SDL game-controller path); `anomaly: controller disconnected while
  holding X: let go` (SDL normally sends the ups itself first: no line).
- `input: release N ms late (glass to app A, app to touch layer B, touch layer to pad C): touch B+Down, tick T` (as before, over
  50 ms, now with the buttons and the tick, 30 a minute).
- `stall: tick T: N ms (game G, events E, touch S, renderer R, vblank V, ui U, slot + present P, waits W, after A; a wait overran
  by O), held touch X key Y controller Z`: a frame over 50 ms (iOS; BWPORT_STALLLOG=1 elsewhere, 0 off), from the frame stamps
  (now always taken): game = the game's code up to SIM_Render, events = the event loop with SDL's pump (UIKit work in it), waits =
  the cap's waits and in-between pictures, overran = the run loop kept a wait past its end. 10 a minute; not the first 10 ticks.
- `bwport: stuck-input checks on (..., SDL view's cancels hooked)` once at start: the checks are in the build.
- Anomaly lines: 20 a minute, the next one says how many were left out.

Reading the next stuck session: no line at all = the pad let go in time, so the picture was late (`input: release shown`,
`display: N presents dropped`) or it was the game; `ended by UIKit but ... did not reach` = a recognizer held the end (now cured,
the gesture line names it); `input: release` with A large = UIKit or the main thread held the touch (look for a `stall:` line at
that tick), B large = held inside the app after sendEvent; `stall:` lines with a button held = the main thread was busy, their
parts say where.

Tested (M2, load 2 then 54-76 while other agents built): desktop BWPORT_TOUCH holds of B and the D-pad with `cfgscript stall=300`
and `stall=120` inside them (the stall lines name the held buttons, the releases reach the pad in time, no anomaly), a virtual
game controller detached while holding (SDL's own ups clear it: no line); Simulator (pool device, load 76): the checks are on and
hooked, no crash, a touch hold through a stall. Not testable here: the UIKit paths (simctl has no real touches; the key script's
fingers are pushed SDL events). On the Simulator the stall line fires on most frames (it runs at ~10-25 ticks/s): read it there
only for its parts.

## Antialiasing on the GPU path: 4x vs 2x at Resolution 3, and multisampling (2026-11-02, M2 macOS)

Step 0 (the maintainer: iPhone 16 Pro Max, gpu Resolution 3, Smooth motion, 120 Hz, fast forward 4x: AA 4x looked worse than 2x).
Before the change, 4x at Resolution 3 WAS 2x: gpu_bridge lowered k to 2 (3 x 4 > 8), Setup did not run again (k unchanged) and the
frame's options carried k = 2; the raw setting was read only by the status text, the switch test, the perftest print and
vcore_bridge. Measured (town, `--noskip`, BWPORT_PANELHZ=120, load 11 -> 5), alternating 2x 4x 2x 4x at 60: GPU pass R
7.33 / 7.22 / 7.23 / 7.15 ms, E 0.52, P 0.68, A 0.07, stage 1 5.5 ms, CPU ms/tick 8.85 / 8.83 / 8.74 / 8.78; at 120 + Smooth: mids/s
56.6 / 58.4 / 53.2 / 52.1, CPU 13.3 / 13.4 / 13.3 / 13.2. Identical within noise: the phone's difference did not come from the
settings or the renderer (a thermal state, or the order the two were tried, are the candidates left). Resolution 3 AA off: R 2.8 ms;
Resolution 6 AA off: R 6.9 ms (a loaded run).

After multisampling (docs/gpu.md "Antialiasing: multisampling"; same scene, load 4-6, GPU timer queries): Resolution 3 AA off
R 2.26 ms, E 0.13, P 0.21; 4 samples R 6.3, E 0.83, P + resolve 1.6 ms. Supersampling 2x was R 7.3 + E 0.52 + P 0.68 + A 0.07.
The 1.5x target for 4 samples is not met (R ~2.8x AA off); AA off got ~15% cheaper (the row tests). The perftest now has
gpu3-60 and gpu3-aa4s-60 rows (and gpu2-aa4s-60) for the phone's numbers.

2026-11-02 (aa-speed, M2, load 2.5-3.5): the timer-query numbers above are not the passes' work: the GPU's clock states (~2x
apart within one run) and other GPU users land in them (an empty polygon loop measured as much as the full one). gpu_render on
captured frames instead (Resolution 3, min of 300 back-to-back renders, R + E + P), before -> after the aa-speed commit:

| scene | AA off | 4 samples before | 4 samples after | after / off |
| --- | --- | --- | --- | --- |
| town (f1400 of the town save) | 1.77 (R 1.05, E+P 0.72) | 5.59 (R 3.15, E+P 2.44) | 4.50 (R 2.83, E+P 1.67) | 2.5x |
| battle (Route 1, wild) | 1.32 (R 0.89, E+P 0.43) | 3.42 (R 2.47, E+P 0.95) | 2.91 (R 2.21, E+P 0.70) | 2.2x |

In the game (town, Res 3, 4 samples, Smooth motion at a pretended 120 Hz): 57 in-betweens a second, gpu lag 7 ms (AA off 4.4),
CPU 13.2 ms a tick (off 8.5: the span build on the N k grid).

2026-11-02 (aa-2pass, M2, load 2-3, same method; before = the aa-speed build):

| scene | AA off | 4 samples before | 4 samples after | after / off |
| --- | --- | --- | --- | --- |
| town | 1.67 (R 1.04, E+P 0.63) | 4.24 (R 2.77, E+P 1.47) | 3.92 (R 2.78, E+P 1.14) | 2.3x |
| battle | 1.23 (R 0.89, E+P 0.34) | 2.84 (R 2.23, E+P 0.61) | 2.65 (R 2.26, E+P 0.39) | 2.2x |

Pass P's alike pixels (sample 0 alone) and the per-sample fine-mark map did it; the two-pass raster (a one-state pass marking
7.8% / 5.0% of pixels by stencil, the four-state pass on them) was exact but slower (town R 4.3-4.4): docs/gpu.md
"Antialiasing". In the game (town, Res 3, 4 samples, Smooth, Frame rate Monitor at a pretended 120 Hz, speed 4/1 toggles):
120 presents/s, 60.4 / 60.1 / 60.4 in-betweens/s, 0 drops at speed 1, gpu lag 6.8-7.4 ms, CPU 13.3 ms a tick.

### The phone throttles in fast forward (2026-11-02, iPhone17,2 A18 Pro, build 86c22fb77)

Town, gpu 3x, 4 samples, Smooth, Monitor at 120 Hz, the fast-forward button (speed 4) held on for ~170 s: the `sys:` lines read
~4.0 GHz, 0.8 cores, gpu <=7.2 ms a frame for the first ~50 s; then within 20 s ~2.0 GHz, 1.4 cores (the same work at half the
clock), gpu <=9-10 ms, for the rest. The thermal state said nominal throughout, so it is no throttle signal; the clock under load
is. The run loop missed 3429 of ~22300 display refreshes over that stretch (~15%). At light load (60 Hz rows, 0.5 cores) the clock
reads 1.6-3.6 GHz anyway: the phone slows an idle core itself, so only a low clock with high load means throttling.

Mac (M2, PANELHZ=120, load 3-5) for the same setting: speed 4 with 4 samples 118.7 presents/s, 2 drops; AA off 119.7, 0. Open:
for the first ~5 s after 4 samples comes on (or the game starts with it), Smooth shows only half the in-betweens (26-31/s), with
Frame rate Monitor or 120 alike; the next 5 s steps measure Monitor 59.8-59.9/s (1-2 drops) and 120 55.7-56.7/s (3-4 drops). AA off
has no such start (59.9/s). The perftest's 4-sample Smooth rows come after a 4-sample step, so they measure after it.

The design study that follows from these numbers (span prep, uploads, antialiasing, the budget for the phone at 120 Hz):
docs/gpu-cost.md.

### Smooth motion drops one picture at a time (2026-11-02, M2 macOS, libntr-ios-smoothdrop.patch)

a test phone log (iPhone17,2, perf test, cool phone): `smooth motion off for 2 s ... (0.92 of its speed)` right after 4
samples + Monitor + Smooth came on, then 0.0 in-betweens/s in the whole 4 s bench (3 s of it with the guard already back on), 14
more "off" lines in play; AA off 60.4/s; speed 4 with 4 samples drew 119.6 real frames a second. The judder was the guard (all
in-betweens off for 2-60 s) switching on and off. Measured on the Mac (BWPORT_PANELHZ=120, town, load 2-4 throughout, other agents):
- The "~5 s start" (26-31/s after 4 samples comes on) did not come back: the first second after the switch had 39, then 60/s.
- A tick's start is often late with every in-between on and the game at 1.00x: half start > 1 ms after their due time, a few > 8
  ms (gpu 3x 4 samples); without Smooth up to ~4 ms. The speed only falls when the lateness builds past 3 ticks and the timeline
  restarts, so "a second below 95%" judged the symptom long after the cause.
- The in-between's GL half was not flushed: its GPU work began at its own swap. A glFlush after it: GPU lag 6.3 -> 4.8 ms at gpu 3x
  4 samples; with a GPU near its limit (BWPORT_GPUSLOW=120) 11-12 -> 14-16 in-betweens/s, drops 30-34 -> 20-25 per 4 s.
- At gpu 3x 4 samples an in-between's fence comes ~6 ms after its swap, 15-25 a second past the refresh after its slot; that alone
  made no more drops than showing every one, so the GPU rule waits only for one done past the second refresh.

Now each slot decides (sim_main.cpp, the comment above SpPaceBegin): none on a tick that starts a quarter tick late (behind), none
while the GPU has the presents' bound in flight or after one it finished that late (gpu), none for 1, 2, 4 ... 32 ticks after one
whose CPU half missed its slot (late). Old -> new, bench rows of 4 s (in-betweens/s, speed, presents/s, drops per 4 s):

| scene | old | new |
| --- | --- | --- |
| gpu 3x 4 samples, Smooth, Monitor | 60.1-60.4, 1.00x, 0-1 drops | 59.3, 1.00x, 0 drops (behind 1/s) |
| the same, BWPORT_GPUSLOW=90 (GPU near its limit) | 58, 1.00x, lag 16.3 ms, 6-12 drops, gap 34-36 ms | 52-55, 1.00x, lag 9.0-9.5 ms, 3-7 drops, gap 18-20 ms |
| gpu 5x 4 samples | 6.5-9.7, 0.97-0.98x | 29.5, 1.00x (behind 17, gpu 7, late 6 /s) |
| gpu 6x 4 samples | 5.7-6.0, 0.93-0.94x | 1.7, 1.00x (Smooth off: 1.00x) |
| vcore 4x at 120 | 0.2, 0.94-0.96x, 112 presents/s | 0, 1.00x, 119 presents/s (late 59/s; Smooth off 119.5) |
| FF toggles (mac_fftoggle.py, gpu 3x) | | PASS: 60.2 / 60.0 before / after, 0 drops |

On the phone: the `smooth motion off for N s` lines are gone; a `smooth motion: N in-between pictures shown, M not in the last 10 s
(the game behind B, the GPU behind G, late L, nothing to blend X)` line comes every 10 s while any were not shown, and the bench rows
end with behind/s gpu/s late/s none/s. As the phone warms, mid/s should fall gradually (not to 0) and every Smooth row should hold
1.00x. The new rows gpu3-aa4s-mon-sm-b (a second window) and gpu5-aa4s-mon-sm (an overload) show the fall-off. If the 4-sample rows
read late/s near 60, the worker's CPU half (2D, the in-between's geometry and span records at 6x) misses its slot on the phone:
that is the next thing to look at (the old 0.0/s was one of late, nothing to blend or the guard; the log could not tell).

### Smooth motion's skip rules, fixed (2026-11-02, M2 macOS, libntr-ios-smoothfix.patch)

a test iPhone (iPhone17,2, real 120 Hz, build 88af9f3e4, perf test, Monitor) after libntr-ios-smoothdrop.patch: gpu 3x 4 samples
+ Smooth 0.0 in-betweens/s with behind 60.3/s at 1.00x (two windows), AA off 33.7/s (behind 12.7, gpu 13.7), SMAA 19.5/s (behind
24.2, gpu 16.7); the old all-off guard gave AA off 60.4/s. What was wrong (a review; sim_main.cpp, the comment above SpPaceBegin):
- **behind** was judged at the tick's top: after its game logic, the event pump (a UIKit run-loop turn), the UI and the post-sleep
  glClear, which on iOS waits for a free drawable longer the more the GPU lags. Now: how late the cap's sleep ended past the next
  tick's due time, right after it (before that glClear), and only its growth over the least of the last 64 ticks by a quarter tick.
- **gpu** counted the present just sent (`sLagN >= 2` right after the tick's own present: normal pipelining). Now: LagSubmit would
  still block without it, or the oldest fence is older than an in-between's deadline window (2 slots less 2.5 ms). Not "older than
  one slot" as the review proposed: the phone's lag is 8-12.5 ms at 120 presents/s with its throughput fine (speed 4 draws 119.5
  frames a second), so that would fire half the time.
- **a dropped in-between's deadline carried over** to the next present (a global set before `if (!LagSubmit()) continue;`). Now
  the deadline is LagSubmit's argument.
- **behind presented every past slot's repeated picture** back to back (a swap that waits for the refresh paid a refresh each). Now
  a repeated picture whose slot has passed is not presented while behind (Smooth on only; Smooth off is unchanged).
- **the doubling reset only on a poll before the deadline**: a fence done in time but found after it was neither. Now the earliest
  done time decides both ways.
- **counters**: slots held after a late one were counted as late or gpu (vcore 4x "late 59/s" for ~2/s real misses), in-betweens
  dropped at the present nowhere, GPU-late ones nowhere. New bench columns held/s, mdrop/s, glate/s (also the pace line and the 10 s
  line); the 10 s line no longer prints for "nothing to blend" alone (static and 2D scenes printed it forever).

**The test aid.** BWPORT_PANELHZ=120 never blocks a swap or a draw, which is why the Mac showed none of this. BWPORT_DRAWABLES=N
(with BWPORT_PANELHZ) gives the window N drawables as iOS's GL layer has (3): a present holds its drawable until the present after it
is on the glass (the first pretended refresh after its GPU done, the fences' earliest time; one refresh after the previous at the
earliest), and the cap's glClears (after the sleep, in a slot) wait for a free one. A `drawables:` line after each bench row gives the
waits a second and their ms a second. Town, gpu 3x, Monitor, Smooth, BWPORT_PANELHZ=120, in-betweens/s (speed; why not, /s), load 2-4:

| | 4 samples | AA off | SMAA | 5x 4 samples | 6x 4 samples |
| --- | --- | --- | --- | --- | --- |
| before, no aid | 60 | 60 | 60 | 29.5 | 1.7 |
| before, DRAWABLES=3 | 4-6, 0.97x (behind 52-54) | 0.0 (behind 60.4) | 44 (behind 16) | | |
| after, no aid | 59.7-60.3 | 60.4 | 59.6 | 33.2 | 6.2 |
| after, DRAWABLES=3 | 26-54 (late 1-4.5, held 3-28) | 60.4 | 59.9 | 12.0 | 0.2 |

Every "after" row holds 1.00x. The aid waits 250-330 ms a second at 4 samples (5 ms a wait), 100-135 with AA off. With it, the
4-sample loss is CPU-half late (gpu_bridge.c's worker not done by the slot less 1.5 ms: the tick starts after the drawable wait, so
the worker launches later) and the holds after it; the phone may show the same: if its 4-sample rows read late/s and held/s, that is
the next thing to look at. A slower GPU (no aid, 4 samples): BWPORT_GPUSLOW=90 57.4-58.2/s, lag 10-11 ms, 4-5 drops per 4 s (before
52-55, 9.0-9.5 ms, 3-7); GPUSLOW=110 31-37/s (glate 10.5/s), 130 4.7-5.0/s, 150 4-8/s, all 1.00x with behind/held taking the rest. vcore 4x at 120: 1.00x, 119.4
presents/s, late 1.7/s, held 58/s. mac_fftoggle.py PASS (59.8 / 58.2). Speed 2-3, fast forward, Unlimited and back: as before.

On the phone the perf test's 4-sample and AA-off Smooth rows should read near 60 in-betweens/s at 1.00x with behind/s near 0; the
new held/s, mdrop/s and glate/s columns say what is left.

### In-betweens when nothing moved (2026-11-02, M2 macOS, load 2-3; docs/gpu-cost.md P5)

Measured, nothing changed. gpu 3x, Smooth, Monitor, BWPORT_PANELHZ=120 BWPORT_DRAWABLES=3, no skip, standing from each save,
BWPORT_INTERP_REPORT plus a local counter of keyframes whose paired vertices all equal the previous keyframe's:

| scene | keyframes / calls | all pairs equal | mids drawn /s | none /s | CPU half per mid |
| --- | --- | --- | --- | --- | --- |
| town, standing | 470 / 938 | 0 | 60 | 0 | 0.46 ms |
| town, start menu open | 470 / 938 | 0 | 60 | 0 | 0.59 ms |
| Route 1, standing | 470 / 938 | 0 | 60 | 0 | 0.34 ms |
| lab, standing | 470 / 938 | 0 | 60 | 0 | 0.40 ms |
| room, standing | 1 / 938 | - | 0 | 57-60 (Smooth's "none") | - |
| town, walking | 470 / 938 | 0 | 60 | 0 | 0.50 ms |

The field's 3D is never still: the water's and grass's texcoords scroll by under a texel every 3D frame (30 a second) and in town
a few lists move, so every 3D frame is a new keyframe and its in-betweens are real blends. A scene whose 3D does not change (the
room) is all repeats, and its mids already settle "none" with no geometry, prep or draw. Skipping a texcoords-only change under a
texel would save the mid on Route 1 and in the lab, but the scroll would then step at 30 Hz: a picture change, not done. For
scale (bench rows): the room runs at 0.21-0.25 cores and 3.5-4.1 ms of CPU a tick, the standing town at 0.30-0.32 and 5.0-5.4.

### The game threads' host waits block (2026-11-02, M2 macOS; port/src/main_threads.c)

A game thread's host (the BGM loader's, "bwport game thread") polled: HostMain with SDL_SemWaitTimeout(t->wake, 10) between game
threads, WaitTurn with SDL_Delay(1) while its thread was put to sleep (main.c sleeps the BGM loader around each V-blank wait). Both
now block on the slot's semaphore, which the wakeup, the destroy and a remake post after changing the state under the game lock.
Town save, gpu 3x, Smooth, Monitor, BWPORT_PANELHZ=120 BWPORT_DRAWABLES=3, no skip; `sample` 10 s and `top -c d` (two runs each,
old/new alternated; load 2-3 for the first pair, 12-44 for the second):

| | old | new |
| --- | --- | --- |
| "bwport game thread" busy samples in 10 s (about ms of CPU) | 109, 110 | 0, 0 |
| process context switches per 2 s | 7390-7510, 7400-7670 | 7320-7390, 7290-7410 |
| cores / CPU ms a tick (8 bench rows) | 0.28-0.37 / 4.7-6.2 | 0.29-0.38 / 4.8-6.5 |

The other 1 ms polls in port/src, none of which ran in these samples (0 samples): PORT_JoinThread, PORT_ReceiveMessage and
PORT_SendMessage (PORT_Sleep(1) loops; the game's blocking message calls are online code), acore_glue.c's queue-full retry
(SDL_Delay(1), only when the sound command queue is full), keys_script.c's 1 ms loop (BWPORT_KEYS test runs only). The libntr
threads beside it in the same samples (not port/src): CTRDGi_TaskThread 86-95, WMSP_RequestThread 6-68, CARDi_OldTypeTaskThread
6-90, NET 7-39.

### Smooth motion's hold after a late GPU in-between ends (2026-11-04, M2 macOS, libntr-ios-smoothhold.patch)

The phone log `~/bw-cache/logs/smooth-held553-2026-11-04.log` (iPhone17,2, gpu 3x 4 samples, Metal, Monitor 120 Hz, town,
unpinned bench; the maintainers' notes): in-betweens 58-60/s at w1-p2 (late on the GPU 0.2-0.7/s), then 4.5, 3.5, 3.5, 3.0/s at
p3-p6 with held 55/s, `late 0`, and **every shown one late on the GPU** (34 of 34 in the last 10 s; glate/s = mid/s in each row).
Meanwhile the tick's presents got slower with less work (GPU ms a present 6.2 -> 7.5, gpu lag 7.6 -> 9.2 ms).

Cause: a mid the GPU finished past its deadline (LagDone) went through SpIpLate, the CPU half's rule: k late in a row hold the
next min(2^(k-1), 32) ticks and only an in-time mid ends the doubling. Its probes are one lone mid after up to 32 ticks of none,
and a lone mid is not like one in a steady run: on the phone all were late (the likely reason: the GPU clocking down once the
mids' work was gone, which also slowed the tick's presents), so the hold renewed itself with every probe and never ended, though
the GPU had kept up with a mid in every slot before. The M2 shows the same bias on the CPU side: the bridge's CPU half was late
for 70 of 112 mids right after a held tick and for 72 of 710 otherwise (gpu 3x 4 samples, load ~3).

Fix: a mid late on the GPU holds the next tick's in-between only (SpIpLateGpu; a hold already running keeps its length) and does
not touch the late run; a GPU really behind is still caught each slot by the gpu rule (the oldest present in flight past the
window). The CPU half's doubling stays (vcore 4x at 120: every mid late). A hold that outlasts the ticks it was set for logs
`smooth motion: a hold outlasted the N ticks it was set for` (none in any run here). Shared code: Windows, Linux and Android
(GL fences) get the same rule; 60 Hz, Game speed x4 and fast forward make no mids and are unchanged.

Reproduced on the Mac with two cfgscript test aids: `glate=N` (the next N mids judged late on the GPU) and `glowclock=1` (a mid
is judged late while fewer than 8 of the last 32 ticks showed one: the phone's lone-probe lateness, modelled). Script:
`f1100:renderer=gpu,resolution=2,antialias=1,framerate=monitor,smooth=1;f1900:bench=w1;p1:glowclock=1,bench=b1;p1:glate=20;p1:bench=a1;p1:bench=a2;p1:bench=a3;p5:quit=1`,
BWPORT_PANELHZ=120 BWPORT_DRAWABLES=3, `--noskip --save town`, load 2-4:

| rows | mid/s before (w1, b1) | after the 20 (a1, a2, a3) | held/s after | glate/s after |
| --- | --- | --- | --- | --- |
| old rule | 60, 60 | 0.7, 1.0, 2.0 | 57 each (stuck, glate = mid/s: the phone's signature) | 2.5, 2.0, 1.5 |
| new rule | 60, 60 | 55.6, 60.2, 49.1 | 4.7, 0, 9.7 (CPU late) | 4.7, 0, 2.0 |

At the maintainer's settings (3x 4 samples) the new rule's rows after the trigger read 55.4, 59.1, 59.6 mid/s (held 4.7, 0.2, 0.2);
the GL present (BWPORT_PRESENT=gl) the same (54.4, 59.7, 58.9). 6x 4 samples (the M2's GPU and CPU behind): 1-7 mid/s either way
(behind 13-20/s, CPU late 2-5/s: the CPU rule's holds). tools/mac_fftoggle.py (10 toggles): PASS, mids 59.95 -> 59.75.
On the shared Mac, the bridge's CPU half still makes held 15-30/s in some rows (the CPU rule, unchanged).

### The Metal present's pacing (2026-11-03, M2 macOS, load 2-5 unless noted; libntr-ios-metal.patch, docs/gpu-backends.md "M4 status")

Present=metal (vcore only until M3) keeps the pacing model: the same slots, LagSubmit's bound of 2 in flight, the panel model
and its drops, Smooth motion's skip rules. What changes where:
- **The drawable wait** is nextDrawable, inside the present (SIM_Swap), after the slot's sleep: it shows in `swap ms/s` and in
  the `metal:` line's waits, never in the "behind" rule (that is the cap sleep's lateness, SpIpLateMark, before the next tick).
  The GL path's post-sleep glClear (iOS's EAGL drawable wait, and the BWPORT_DRAWABLES aid's SpDrawable) does nothing under
  Metal except the aid, which still runs if asked.
- **The fences** are the command buffers' completion handlers; the done time is GPUEndTime on the pacing clock, so `gpu lag`
  is the real swap-to-GPU-done (GL's is the poll that found it, up to a millisecond or a tick late) and `glate` judges the
  real end against the deadline.
- **GPU time** per present is measured, not bounded: `metal:` after each bench row, the sys line every frame.

Bench (town, vcore, BWPORT_PANELHZ=120, GL / Metal): 2x 120 Smooth 119.5 / 119.5 presents/s, 58.9 / 58.9 in-betweens/s, 1.00x
both, behind 0 / 0; 1x the same with 59.4 / 59.9; 3x and 4x Smooth (both with BWPORT_DRAWABLES=3) 0.0 / 8.5 and 0.2 / 0.0
in-betweens/s, held 51-57 / 38-58 (the CPU half late, as before). Metal's GPU time a present 0.35-0.41 ms (the picture quad and
ImGui; the upload is the CPU's replaceRegion). The hidden window's layer frees drawables at the Mac panel's 60 Hz, so Metal waits
for one ~100 times a second above 60 presents (160-700 ms/s at 1x-4x), harsher than an iPhone at 120; every row still 1.00x.
Desktop Monitor without a pretended link (load 30-47): Metal 59.8 presents/s with swap 8 ms/s (the present never blocks; GL's
swap waits for the V-blank, 886-914 ms/s) and no V-blank lock: a CVDisplayLink for SpLink is owed before Metal is the Mac default.

**The drawable thread (2026-11-03, M5, libntr-ios-metaltune.patch; docs/gpu-backends.md "M5 status").** The phone's Metal rows at
Monitor + Smooth waited ~5.7 ms in nextDrawable at every present (686 ms/s of the game thread at gpu 3x AA off), and although the
wait came after the slot's sleep, it ate the time the next tick and the in-between's CPU half needed: behind 9-13/s, 45-49
in-betweens/s. The drawable is now fetched ahead by a thread of the present's own; the present takes the waiting one (2 ms at most,
else it goes without one, `no drawable N/s`), so the game thread pays nothing and the present goes out at the slot (gpu lag 4.6 ->
2.2 ms on the Mac). The `metal:` line's `drawable waits` are now the game thread's (should read ~0) and `fetch thread waits` the
thread's. Under GL nothing changed (EAGL's waits are inside the post-sleep glClear, as before).

**The Mac's display link (2026-11-03, libntr-ios-maclink.patch; docs/gpu-backends.md "The Mac's display link").** Monitor under Metal
on macOS now follows a CVDisplayLink as iOS follows its CADisplayLink (60, 120, Unlimited, fast forward unchanged). A bench row's
`glass:` line counts the present calls on the link's refreshes: town, gpu 3x, Monitor (with and without Smooth), before 6-12
refreshes a second with no new present and as many with two (calls 0-16.7 ms into the refresh), after 0 and 0 (1-8 ms in) over
40 s; presents/s 59.7-59.9 both. The line's glass half (presentedTime) is empty for the hidden test window.

## Where the CPU goes, thread by thread (2026-11-03, M2 macOS; scaled to the iPhone 16 Pro Max)

a test phone (Metal, gpu 3x, AA off, 60 fps, ~1.1 GHz, cool; bwport-20261103-144959.log): 0.60 cores = ~10.1 ms of CPU a
drawn frame, split by the sys line only as main 0.2, other 0.4, audio 0.0. The readout now names the threads (port/vcore's pool
threads are `bwport-worker`; `unnamed` = GCD's workers: Metal's submission and completion handlers, CoreAnimation): a `sys
threads:` line every period, and after every bench row a `threads:` line with the row's CPU a drawn frame by thread, the
process's instructions, cycles and context switches a drawn frame (the maintainers' notes, "CPU BY THREAD"). The performance test
records a perf log after its gpu3-60 row, so the phone's next log also has the game thread's stages (`stages:` lines).

**Method.** Town save, `tools/mac_run.sh --noskip`, cfgscript `renderer=gpu,resolution=3,framerate=60,antialias=1`, 4 bench rows,
then `ff=1` (Max fast forward) and a row; `sample PID 8-10` on the hidden run for the top functions (busy samples: leaves not in a
wait; `sample` counts a runnable thread waiting for a core as busy, so under load it overstates some threads, e.g. SDL's audio thread and a CoreAudio logging thread in the Simulator; the CPU times below are the threads' own). Load 3-7 throughout (other agents'
builds and runs). CPU ms move with the clock and the core kind the scheduler picks (the same build read 1.15 or 2.0 ms on the
game thread between runs: ~2.45 GHz mean, the E cores, against ~3.1), so before/after is alternating A/B runs compared by
instructions and cycles a drawn frame; the ms are the P-core runs.

**Per thread, ms of CPU a drawn frame** (Mac before = ios 5919887b2, P-core rows of alternating runs; Mac after = this round's
Mac-side commits; phone = the Mac x3.1, the ratio of the phone's 10.1 ms to the Mac's 3.25 for the same rows, which the phone's
passes line bears out for the worker: 2D 3.6 + geometry 0.8 + spans 0.3 = 4.7 against 1.5 x 3.1):

| thread | what runs there | Mac before | Mac after | phone before (est.) | phone after (est.) |
| --- | --- | --- | --- | --- | --- |
| game | the game, libntr's OS and hardware, the sound step, stage 2, the present, ImGui, (iOS) the run loop | 1.15-1.28 | 1.00-1.02 | 3.3 (the sys line's main) | ~2.0-2.7 |
| bwport-worker | the bridge's worker: 2D, 3D geometry, span setup (stage 1) | 1.51-1.64 | 1.49-1.55 | ~4.7 | ~4.7 |
| unnamed | GCD: Metal's command-buffer submission (IOKit), completion handlers, frame pacing | 0.28-0.42 | 0.28-0.30 | ~1.0 | ~1.0 |
| SDLAudioP15 (Mac) / AudioQueue, IOThread.client, AQConverter (iOS) | the device callback: the ring's 64-tap resampler, then the system's | 0.09 | 0.09 | ~0.3-1.0 | same |
| bwport-drawable, CVDisplayLink, PXI, NET, CTRDGi, CARDi, WMSP | the drawable fetch, the link, NitroSDK threads (blocking) | 0.03-0.05 | 0.03-0.05 | ~0.1 | ~0.1 |
| all | | 3.13-3.41 | 2.93-2.99 | 10.1 | ~8.5-9.2 |

The Simulator (iPhone 17 Pro, the same rows, load 20-90 so its ms are rough): game 1.67-1.87 before the safe-area fix and
1.46-1.49 after, worker 1.8-2.6, unnamed 0.84-0.98, CoreAudio's three threads 0.33-0.43, 113-129 context switches a drawn frame
(the Mac: 30). What the phone's own run loop and audio cost the next phone log's `threads:` line names.

**The game thread at 60 fps** (perf log stages, Mac before; ms a frame): game logic 0.57 (of which libntr's geometry engine and the
3D recorder ~60 %), sound step 0.35, renderer 0.33 (gather 0.04, stage 2's 3D handoff 0.20, compose 0.08), events + UI build 0.17,
swap 0.12; the rest is the frame-rate wait. `sample` (568 busy samples in 10 s): SIM_Render 70 % = the sound step 25 % (acore's
hw_sample: ac_ch_output, ac_hw_read, ac_mix), stage 2 17 % (gpu_mtl_render 10 %: its memcpy into the shared buffers 5 %, render
encoder creation 3 %; g07_compose_packed 3 %; SIM_VCapGather 3 %, the VRAM copy), the GUI 11 % (the Mac's side panel 6 %, the
Metrics recorder 4 %), the present 5 %, SDL's event pump 4 %, the sleep's last 150 us spin 3 %; the game's own update 29 %, of
which the 3D commands 23 % (libntr's geometry engine 15 %, the recorder for the frame renderers 6 %).

**A tick against a drawn frame.** Max fast forward (one drawn frame in ~34-44 ticks) puts the game thread at one core: before 16.4 ms
a drawn frame at 34x = 0.48 ms a tick, so at 60 fps (1.13 ms) about 0.5 ms is the tick and 0.65 ms the drawn frame's own work
(stage 2, the present, the GUI build, the pump). In a fast-forward tick the 3D path is 72 % of the game thread (libntr's geometry
engine 48 %, the recorder 20-30 %), the sound step 10 %, the GUI build ~8 % (ImGui's frame is built every tick, presented or not).

**What changed** (each alternating A/B on the Mac, pictures checked):

| change | where | before -> after |
| --- | --- | --- |
| acore's mixer reads its registers directly (no byte loop over the driver's pending writes outside a wake) and skips silent channels | port/acore (all platforms) | offline render of a 57 s town capture: DS 11.95 -> 9.20 G instructions, Enhanced 13.67 -> 10.87 G, byte-identical; game 34.8-35.6 -> 34.0-34.8 M instructions a drawn frame |
| macOS and iOS built -O2 -g (buildtype debugoptimized): the game C, NitroSDK and libntrsystem were -O0 | port/machine (Apple only) | 34.0-34.7 -> 32.9-33.7 M instructions a frame; game thread 1.11-1.16 -> 1.04-1.12 ms; Max FF 33.9-34.4x -> 37.3-38.3x; room walk pictures byte-identical |
| iOS: the window's safe-area insets kept (read on a size change, else at most twice a second), not asked of UIKit every frame | port/src/ios_safearea.m | Simulator: -[UIWindow safeAreaInsets] 173 of the game thread's 763 busy samples -> 37 of 568; perf log "events + UI build" 1.06-1.67 -> 0.22 ms; game thread 1.67-1.87 -> 1.46-1.49 ms; 0.7 M fewer instructions a frame; the layout the same |
| the 3D recorder (sim_vcapture.cpp) no longer copies the vector and texture matrices at every NORMAL / TEXCOORD | libntr-ios-vcapcost.patch (all platforms) | same streams (hash of every frame's 3D stream equal); Max FF 38.3-38.5x -> 43.3-44.5x (13 % fewer cycles a tick); game thread 1.04-1.05 -> 1.00-1.02 ms |

Tried and dropped: appending the recorder's words with one resize + memcpy instead of push_back (more instructions for the usual
1-2 word command).

**Findings for the parts others own** (not changed here):
- port/gpu gpu_mtl.m: the frame's data (span records, clear image, texture slots) is built in CPU arrays on the worker and copied
  into the shared MTLBuffers on the game thread (gpu_mtl_render's memcpy ~5 % of the game thread at 60 fps on the Mac, mdata_put's 7 % in the Simulator); building into a ring
  of shared buffers on the worker would drop the copy. Each pass begins a new render encoder (pass_begin ~3 %).
- SIM_VCapGather (libntr-vbridge/vcapture): 656 KB of VRAM copied every drawn frame on the game thread (~3 %), and the 3D stream
  copied twice (Fill3D into a vector, then into the slot).
- libntr's own geometry engine (clean room: not read) runs every tick for what the game reads back (box, position and vector tests,
  counts) and is the largest single cost of a tick (15 % of the game thread at 60 fps, ~half a fast-forward tick); a path that
  computes only what the game reads, designed from GBATEK, would be the next big cut, and the bridge's geometry repeats the transform
  on the worker.
- ImGui's frame (the touch overlay on iOS, the side panel on the Mac) is built every tick, also the ticks that are not presented:
  ~8 % of a fast-forward tick on the Mac. Building it only for presented ticks needs the present decision before the UI build
  (sim_main.cpp orders them the other way: SpPaceBegin comes after).
- Two command buffers a frame (the renderer's and the present's): the `unnamed` submission cost (~0.3 ms a frame on the Mac) would
  roughly halve with one.
- The app asks SDL for 44.1 kHz and the phone's device runs at 48 kHz: the ring's 64-tap resampler and then SDL's own resample run on
  the audio thread; asking for the device's rate would drop SDL's.
- The pacing sleep's last 150 us is a spin (~0.15 ms a tick on any clock): ~5 % of the phone's game thread; shortening it is a
  pacing decision (libntr-ios-pace / smoothfix) for whoever owns it.
- The libntr file capture (BWPORT_VCAPTURE) crashes in SIM_VCapFrameEnd at -O0 and -O2 alike (a null read): not used by the game's
  path, found while checking.

## CPU cuts at the maintainer's settings (2026-11-04, M2 macOS; port/acore, libntr-ios-cpubatch.patch)

Settings: Metal, gpu 3x, 4 samples, Smooth on, Monitor at a pretended 120 Hz (BWPORT_PANELHZ=120, BWPORT_DRAWABLES=3), town
save, the pinned bench (BWPORT_BENCH_CLOCK=2026-10-29T12:00:00, BWPORT_BENCH_SEED=990), `--noskip`. Rows: own (Monitor +
Smooth), 60 Hz, x4 (the FF button's Game speed 4) and Max fast forward (`ff=1`). Three alternating pairs, the old way on the same
build by switches (a temporary AB_OLD for acore's block and the UI build, BWPORT_VCAPGATHER=copy, BWPORT_SMOOTHWARM=0); load
2.5-3.6 throughout (other agents' builds and runs): the ms and cycles moved 2-3x with the core the scheduler picked (the game
thread's sound step 0.23-0.69 ms a tick in the same build), so the comparison is M instructions a drawn frame (process).

| row | before (3 runs) | after (3 runs) | change |
| --- | --- | --- | --- |
| 60 Hz | 16.84, 16.85, 16.90 | 16.79, 16.76, 16.74 | -0.5 % |
| x4 (FF button) | 21.33, 21.29, 21.31 | 20.39, 19.96, 20.14 | -5.5 % |
| Max FF, per tick | 5.90, 5.88, 5.83 | 4.81, 4.82, 4.81 | -18 % (43-46x -> 52-54x) |
| own (Monitor + Smooth) | 19.0-19.9 | 19.1-19.6 | within the mids' spread (instructions follow mids/s) |

**1. The sound step** (acore, on the game thread every tick: 0.71 ms a tick on the phone). 547 mixer samples a tick, ~10 of the
16 channels running in the town (all PCM / ADPCM), Enhanced: each running channel's timer, fetch, 16-sample history and the
16-tap interpolation, then the mix decoding every channel's CNT again, one sample at a time with every channel and the event
scheduler in between. Now acore_run hands the samples before the next event to a block: each channel's samples in a row
(ac_ch_run, the same read-then-step), mixed with each CNT decoded once (ac_mix_block); the history is a ring stored twice (no
15-float move a fetched sample), the per-sample output is inlined into the block and the Enhanced table's one-time setup is out
of line. A block needs no capture unit running (a capture writes memory a channel may play), no solo and no quality fade; else
the old per-sample path runs. Exact: acore_render of the 57 s town capture byte-identical in DS and Enhanced, acore_tests 212
passed. That render: Enhanced 10.92 -> 9.17 G instructions (-16 %), 2.23 -> 1.87 G cycles (-16 %); DS 9.19 -> 7.41 G. In the
game it is ~0.4 M instructions a tick of the 60 Hz row's 16.8 M (the bench above had the first of the two commits). Moving the
step off the game thread was not done: the game reads the driver's shared work and info after each step (acore_glue.c's
CopyBack), so a parallel step would change what the next tick sees; and the phone's cost is energy, which a thread move keeps.

**2. ImGui's frame** (the touch overlay, the Mac's panel) was built every tick, presented or not. The pacing decision
(LagPoll, SpPaceBegin, SIM_FrameSkipDecide) now comes before the UI build, and NewFrame .. Render runs only for a tick with a
present slot; the renderer toast (SIM_RendererPoll's DrawStatus) is drawn only into a frame that is open. Input is unchanged:
SDL's events still reach ImGui's queue each tick and the touch controls' state is made in the event handling (sim_touch.cpp's
Event), not in the drawing; a UI change made in a tick's build reaches the pacing a tick later. At 1x every tick presents, so
the gain is at x4 (half the ticks presented) and Max fast forward (1 in ~50): the x4 and FF rows above (with item 3).
BWPORT_UIEVERY=1 builds every tick (A/B).

**3. SIM_VCapGather** copied 656 KB of VRAM into the bridge's slot and the 3D stream twice every drawn frame. The slot's
buffers hold that slot's earlier frame (two slots alternate), so the gather now compares each 4 KB piece and writes only those
that differ (the same bytes after it as the copy), and the stream goes from the recorder's last frame straight into the slot
(the prologue alone through the vector). BWPORT_VCAPGATHER=copy: the old way. Pictures: room and town saves, pinned, frame-exact
shots at f1500/1800/2100/2400 with a walk between, gpu and vcore: the game area identical with every switch on and off.

**4. Smooth motion's CPU-late hold.** After libntr-ios-smoothhold.patch the CPU half's rule (k late in a row hold the next
min(2^(k-1), 32) ticks) still probed with one lone mid after each hold. Now the first mid after a CPU hold is a warm-up: its
lateness neither renews nor doubles the hold, the one after it judges (an in-time one still ends the doubling). The 10 s
`smooth motion:` line now says how often the first and the second mid after a CPU hold were late. Measured (town, the maintainer's
settings, 4 rows each, load ~3, one run each in turn): rule off, the first mid late 18 of 23, the second 0 of 6, held 14, 17.5,
2.5, 35.5/s: the lone probes kept the holds going though the mid after them was in time. Rule on: held 0, 0, 0, 12.7 and 7,
24, 0, 1/s; where it still held, the second mid was late as well (6 of 7, 13 of 15, 5 of 5): bursts in which the worker really
was late (other agents' load), which the rule is for. vcore 4x at 120 (every mid late, the first and the second after
each hold: 42 of 42 and 41 of 41 in 40 s) still shows none: mids 0/s in all 5 rows with the rule on and off, speed 0.91-1.00x on
against 0.81-1.00x off, held 1-22/s on against 12-52/s off (the behind rule takes those slots instead: behind 35-53/s on, 7-35/s
off). The iOS device target builds. The earlier pacing rows of the alternating pairs swung 0-22 held/s either way (one row in three).
The rule is OFF by default until the phone confirms it (2026-11-04: on the loaded Mac one or two Smooth rows of three showed held 8-22/s with it on); BWPORT_SMOOTHWARM=1 turns it on.

## 3D renderer: Exact / Fast (2026-11-03, M2 macOS; docs/gpu-hwraster.md "The build")

A setting beside Renderer: `Renderer3D=exact|fast` (sim_config.ini, default exact; BWPORT_RENDERER3D; cfgscript renderer3d=;
the Video window's and the iOS sheet's "3D renderer" row). Fast draws the GPU renderer's 3D with the GPU's own rasterizer (Metal
only; GL draws exact): no span records, no pass S, pass R's slot 59-63 % cheaper with AA off and 30-38 % with 4 samples on the
M2 (town and battle captures at 3x), 27-54 % of the 3D frame (R+S+E+P); about 11 % of the 3D pixels differ slightly with AA off,
8 % with 4 samples (gradients by a step, texels by one N pixel, outlines at slanted edges). The CPU is unchanged (12.5-12.7 M
instructions a drawn frame either way). Live; the first switch costs one 0.1-0.6 s pipeline compile. Phone: pending (the A/B
in docs/gpu-hwraster.md).


## The game thread and the worker, by function (2026-11-04, M2 macOS; `sample`, the pinned bench)

Build postfast 4448c061e (ios + Fast's 4-sample direct layers), Metal, Fast, gpu 3x, 4 samples, Smooth on, Monitor with
BWPORT_PANELHZ=120 and BWPORT_DRAWABLES=3, town save, `--noskip`, the pinned bench (BWPORT_BENCH_CLOCK, BWPORT_BENCH_SEED=1), hidden
window. `sample PID 10 1` (own, 1x Smooth; 3008 samples a thread, the sampler ran at ~3.3 ms real) and `sample PID 12 1` at x4
(4664 samples a thread), each inside a run of `p60:bench=` rows, load 4.5-6.6 (four other agents measuring; one run of mine at a
time, under the timing lock). The sampler disturbs Smooth (the own rows' mids fell 60 -> 48/s while it ran), so the shares are
read from busy samples (the thread's samples minus `__semwait_signal`, `__psynch_cvwait`, `mach_msg2_trap`, `__ulock_wait2`,
`__workq_kernreturn`), and the sizes from the bench's `threads:` lines (the CPU counters), not from the sampler. The parser:
scratchpad cpu-profile/parse.py (self = a node's count minus its children's).

Sizes, the `threads:` lines (own rows 3 of them, x4 rows 5): own 3.17-4.10 ms a drawn frame = game 1.43-2.01 / worker 0.72-0.85 /
unnamed 0.80-0.94; 19.2-19.7 M instructions, 9.0-9.3 M cycles a drawn frame (2.7-2.8 GHz; the game thread ~45 % of the ms, so about
8.6-9 M instructions; the worker ~23 %, about 4.4 M). x4: 2.02-2.91 ms = game 1.15-1.69 / worker 0.44-0.49 / unnamed 0.35-0.55;
20.3 M instructions, 6.2 M cycles (3.07 GHz). The docs' earlier 6.4 ms / 3.3 game figure was taken at ~1.5 GHz: same instructions,
half the clock.

**The game thread (main thread: NitroMain runs there; SIM_Render inside OS_WaitIrq)**, share of its busy samples, own / x4:

| piece | own | x4 | what it is (file) |
| --- | --- | --- | --- |
| **The G3 command processor** (`SIM::G3::HandleCommandMessage` inclusive) | **54 %** (809 / 1500) | **45 %** (596 / 1319) | the DS GX FIFO interpreted on the game thread, under `MI_SendGXCommandAsync` from NNS_G3dDraw (field_g3d_map.c: FieldChunk_RenderMap + RenderProps = 493 of the 607 samples of the field's 3D draw) |
| - of it: libntr's own geometry engine | 13 % (G3SIM_Vtx 81, SubmitPolygon 38, Begin 32, Normal 10, FX_MulFunc 41, calculateVtxFx) | 11 % | the vertex transform and polygon assembly kept by libntr-norender.patch ("results, counts, swap state") although nothing rasterises it: the worker transforms every vertex again (vc_ip_record) |
| - of it: the capture's per-command walk | 11 % (SIM_VCapList 131, VCapMsg 41: Record 94, NumParams 26, G3Apply) | 10 % | libntr-vcapture.patch: each command pushed to the stream one at a time and the state followed a second time (G3Apply) |
| - of it: the decode | 16 % (RunCommandList self ~65, DecodeG3Op 41, HandleCommandMessage self ~56, memmove 52) | ~15 % | sim_g3_CommandProcessor.cpp (clean room: not opened) |
| The GPU path's CPU half (`GpuFrame`: Stage2 = gpu_render_prepared 121, SIM_VCapGather's memcmp 38-47, rest) | 17 % (253) | 15 % (201) | gpu_bridge.c:1174/1211, g00_frame.c:443: the encoders, the 2D gather's compare |
| The sound (`SIM_Audio_FrameTick`) | 12 % (178: hw_sample 68, ch_output ~50, adpcm 21, mix 9) | **19 %** (255: four ticks of sound for two drawn frames) | sim_audio.cpp:727, acore (ac_state.c hw_sample, ch_output) |
| The present (`bwport_mtl_present`, Encode) | 9 % (138) | 12 % (159) | metal_present.mm:322 (agent 3's area) |
| The GUI (`SIM::GUI::DrawSide` 121, ImGui NewFrame 24, Begin 27, RenderDrawData 30) | 9-11 % (142-172) | 6-9 % (65-120) | sim_gui.cpp:2649 (the side: metrics + controller), rebuilt and re-encoded every present, 120/s |
| SDL's event pump (`SDL_WaitEventTimeout` -> Cocoa) | 4 % (65) | 3 % (44) | sim_main.cpp:3934 |
| NNS_G3d's SBC walk itself (game code, above the FIFO) | 8 % (114) | 4 % (57) | lib/nnsys sbc.c: untouchable |
| The rest of the game's logic (NitroMain minus the 3D draw) | **4 %** (61) | 3 % (41) | the overworld, scripts, sprites: untouchable, and tiny |
| `mach_absolute_time` self | 1 % | 6 % (85) | SpSleepUntil's spin (sim_main.cpp:2838-2839) and LagPoll at x4; the `spin` key showed no instruction change, so it is time in the clock call, not work |

**The worker (bwport-worker)**, busy 670 (own) / 660 (x4) of 3008 / 4664 samples: `vc_ip_record` (the geometry interpreter: vc_cmd_vtx
146 of which assemble 97 and emit_polygon 31; vc_cmd_light 31; vc_vertex_attrs 23; vc_p14_clip, facing) **59 %**; `GeometryAgain` (the
in-betweens' replay) 7 % at own, 0 at x4; `gpu_prepare` 6 %; `gpu_texdata_build`'s memmove 4 %; `vc_ip_texhold` 4 %; the rest is
the task pool's wake and small stages. The in-betweens' CPU is in the "unnamed" (Metal's queues) and the worker's GeometryAgain,
both already measured by the `gpu` key (the maintainers' notes).

The one surprise: **the game's own logic is 4 % of the game thread**; 54 % is the DS geometry FIFO interpreted on that thread, and
the vertex half of it is done twice (libntr's engine, then vcore's). Nothing in src/ reads the engine's vertex results: the game
reads only `G3X_GetBoxTestResult` (src/ov010/field_g3d_map.c:473; the box test goes to the simulator as DRAW_CMD_G3_BOXTEST,
libntr-g3imm.patch, so the matrix stack must stay), no position/vector test, no vertex/polygon RAM counts (grep of src/).

### Ceilings (M instructions a drawn frame at own; the game thread ~8.8 M, the worker ~4.4 M; x4 in brackets)

| # | candidate | ceiling | the share's basis |
| --- | --- | --- | --- |
| C1 | the G3 processor's vertex engine when no raster reads it | 1.1 M (13 % of the game thread; x4 11 %) | self samples of G3SIM_* + FX_MulFunc + calculateVtxFx |
| C2 | the capture's per-command walk (push whole lists; follow the state once) | 1.0 M (11 %; x4 10 %) | SIM_VCapList + SIM_VCapMsg inclusive |
| C3 | the GUI rebuilt every present | 0.8-1.0 M (9-11 %; x4 6-9 %) | DrawSide + NewFrame + Begin + RenderDrawData inclusive; the `overlay=0` rows below |
| C4 | the sound at FF (x4: 19 %) | 1.0 M at own, 1.7 M at x4 | SIM_Audio_FrameTick inclusive |
| C5 | the decode itself (RunCommandList, DecodeG3Op) | 1.4 M (16 %) | self samples; a cheaper decoder is the clean-room files' business |
| C6 | SDL's event pump | 0.35 M (4 %) | poll less than every tick (it is read once a V-blank wait) |
| W1 | vc_ip_record on the worker | 2.6 M (59 % of the worker) | not a cut: the geometry must be transformed once somewhere; C1 removes its twin |

Measured by key, the corner overlay only (`overlay=0` governs the fps text in the corner, sim_gui.cpp:2538, not the side that
DrawSide draws; a run of its own, load 3.2): x4 rows (no in-betweens, so comparable) 20.06 / 19.85 / 19.88 M instructions (ref,
overlay off, ref again): **-0.1 to -0.2 M, under 1 %**; the own rows 18.52 / 18.81 / 19.32 / 18.48 moved with their in-betweens
(31 / 43 / 60 / 34 a second) and say nothing. So the corner text is nothing; C3's 9 % is the side (metrics + controller) and the
ImGui frame, measured by the sampler only: no key turns the side off, and the first step of item 3 below is to add one
(`side=0`) and take the x4 rows.

### Build plan, ranked

1. **C1: skip the vertex transform and polygon assembly in the G3 processor while SIM_G3RasterOn() is 0** (libntr-norender.patch's
   switch exists; it drops the batch after the engine ran). Keep the matrix stack, the box test, the swap state and the counts
   (GX_GetVtxListRamCount is unused by the game, but keep it cheap: count, don't transform). Expected: -1.1 M instructions a drawn frame
   (-13 % of the game thread, -6 % of the process), all four platforms and both renderers (vcore too). Effort: a day; risk: low for the
   picture (vcore/gpu never read those results), medium for the box test if it shares code with the vertex path. **Blocker: the files
   are sim_g3_CommandProcessor.cpp / g3_handler.cpp, which the clean-room rule closes to the port/gpu agents**; it needs an agent with
   the maintainer's leave for libntr's geometry code, or a route through g3imm.h (the port's own header patch): send vertex commands to the
   capture only, and the matrix / box-test commands to both. Checks: pixel-identical town and battle captures (gpu_render against the
   vcore reference), the box test still culls chunks (field_g3d_map.c:473 path: walk the town edge), 100 renderer switches.
2. **C2: the capture pushes a list in one memcpy and follows the state once.** SIM_VCapList calls Record per command (a push_back
   each word, NumParams, G3Apply); the worker's vc_ip_record decodes the same stream again. Move the state following to the stream's
   consumer (the `start` state is needed only at SWAP: keep G3Apply for the matrix commands and skip it for VTX/NORMAL/TEXCOORD/COLOR,
   which are 80-90 % of the words). Expected -0.6 to -1.0 M (-7 to -11 % of the game thread). Effort: half a day in
   libntr-vcapture.patch + port/vcore p23_interp.c (port-owned, no clean-room issue). Risk: low; check with the capture's own
   compare (BWPORT_VCAP tests) and pixel-identical captures.
3. **C3: cache the GUI's draw lists between presents.** The side (metrics + controller) is rebuilt by DrawSide and re-encoded 120
   times a second at Monitor; its inputs change only on touch, a metrics tick (1/s) or a settings change. Keep the ImDrawData of the last
   build and skip NewFrame/DrawSide when nothing changed; the Metal backend re-encodes from the cached lists. Expected: the measured
   overlay=0 drop (below), ~-0.8 M at own; all platforms with the overlay on (the phone always has the controller). Effort: a day in
   sim_gui.cpp (readable) and the ImGui backend; risk: stale pictures on a missed invalidation (the perf overlay's numbers, the
   FF badge): invalidate on every input event and every second.
4. **C4: the sound at FF.** 19 % of the game thread at x4; acore's block render already cut it 16 %. The next cut is sampling
   hw_sample at a block granularity for x4+ only (the maintainer's SoundQuality setting could choose it). Measure before building:
   `soundquality=` rows at x4.
5. C6: poll SDL's events once a presented frame instead of once a tick at x4 (4 % at own, cheap, Mac/Windows/Linux only).

Not a cut: W1 (vc_ip_record, 59 % of the worker) is the one necessary transform; its SIMD-ing is a separate study. The decode (C5)
sits in the clean-room files.

## Three CPU ceilings by key (2026-11-04, M2 macOS; libntr-ios-testkeys.patch, the pinned bench)

Build ios + the testkeys patch, Metal, gpu 3x, 4 samples, Smooth on, BWPORT_PANELHZ=120, town save, `--noskip`, bench clock/seed 990,
hidden window, under the timing lock, load 2.4-3.6 (other agents measuring). One run, 39 `p60:bench=` rows: per speed row (own =
Smooth + Monitor, x4, 60 Hz) a warm row, then for each key B A B A (base, variant, base, variant). The CPU counters were present
(`threads:` lines: process M instructions and M cycles a drawn frame). New test-only cfgscript keys, off unless set: `side=0` (DrawSide is
not called: the picture is wrong), `pump=1` (the tick's SDL_PollEvent loop runs only on a tick after a presented one), `soundquality=0|1`
(the existing SoundQuality field; cfgscript had no key for it, nor `overlay`: the 'overlay=0' rows above ran as no-ops, which fits
their 'under 1 %'). A control run with the keys unrecognised (same script) gave the spread: instructions within 1-2 % per pair at x4/60 Hz,
3 % at own (the in-between count moves 59-60/s; the cycles and ms vary 1.3x with core placement, 60 Hz rows' later pairs ran 3x slow in
ms at the same instructions: the E cores).

M instructions a drawn frame, mean of two (B -> A), the game thread's ms in brackets:

| ceiling | own | x4 | 60 Hz |
| --- | --- | --- | --- |
| 1. side panel off (`side=0`) | 18.28 -> 17.54 = **-0.74 (-4.0 %)** [0.95 -> 0.90 ms] | 17.16 -> 16.60 = **-0.56 (-3.3 %)** [0.91 -> 0.85] | 15.12 -> 14.63 = **-0.49 (-3.2 %)** |
| 2. event pump once per presented tick (`pump=1`) | 18.25 -> 18.10 = -0.15 (inside the 0.55 spread) | 17.15 -> 17.09 = **-0.06 (-0.4 %)**, cycles 5.19 -> 5.19 | 15.26 -> 15.23 = 0 |
| 3. sound at the cheapest key (`soundquality=0`, DS exact) | 18.14 -> 17.42 = -0.72 (-4.0 %) [0.98 -> 0.88 ms] | 17.18 -> 16.89 = **-0.29 (-1.7 %)** [0.91 -> 0.87] | 15.07 -> 14.51 = -0.55 (-3.7 %) |

Reading them:
1. The side panel is 3-4 % of the process's instructions (the sampler said 9-11 % of the game thread's busy samples, about 0.8 M of 8.8:
the key took 0.5-0.7, the rest is ImGui's frame that still runs for the settings windows and the corner). Building the cache of C3 can
take at most 0.5-0.75 M (3-4 %), not the 1 M the table above hoped for; the part left (NewFrame/Render of the other windows) is not in it.
   To build: skip NewFrame/DrawSide when no input, metrics tick or setting changed (a day in sim_gui.cpp, the sheet's Metal backend
   re-encodes the kept ImDrawData; invalidate on input, the 1 s metrics tick, a settings change, the FF badge).
2. The event pump is a ceiling of about 0 in instructions and cycles: SDL_PollEvent on an empty queue costs under 0.1 M a drawn frame.
   x4 runs 2 ticks per drawn frame (240 ticks, 120 drawn/s), so the key halves the polls with no change. The sampler's 3-4 % was time in
   Cocoa's run loop (a 2 us pump per call, walltime not work). Not worth building; C6 can be dropped.
3. Sound's DS-exact setting (no interpolation, 10-bit output) is the cheapest key that exists: -0.29 M at x4 (-1.7 %), -0.7 M at 1x. The
   sampler's 1.7 M ceiling for sound at x4 is the whole of it; the key removes a sixth. Anything more is block-granularity hw_sample in
   acore for x4+ (the 'four ticks of sound for two drawn frames' part): to build, render sound once per two ticks at x4 when no
   channel retriggers inside the pair; effort a day in port/acore, risk audible. Only 0.3-1.4 M, so after C1/C2.

## The capture's command walk, cut (2026-11-04, M2 macOS; libntr-ios-vcaplist.patch, build plan item 2 / C2)

Build ios 0fd405cb5 + the patch, Metal, gpu 3x, 4 samples, Smooth on, BWPORT_PANELHZ=120, town save, `--noskip`, the pinned bench
(BWPORT_BENCH_CLOCK=2026-10-29T12:00:00, BWPORT_BENCH_SEED=990), hidden window, under the timing lock, load 2.2-3.4 (other agents
measuring). Rows own (Monitor + Smooth), x4 (speed=4), 60 Hz; M instructions a drawn frame (process) from the `threads:` lines.

**The ceiling first**: a new test-only cfgscript key `vcapwalk=0` makes SIM_VCapList / SIM_VCapMsg return at once (nothing recorded or
followed; the renderers keep drawing the last stream, so the picture is wrong and, at own, the in-betweens stop: own is not comparable).
One run, B A B A per row: x4 17.24 / 17.10 -> 14.54 / 14.49 = **-2.7 M a drawn frame** (2 ticks each), 60 Hz 15.27 / 15.22 -> 13.70 /
13.70 = **-1.55 M** a tick. That is more than the sampler's 1.0 M: part of it is downstream (a stream that does not change), so it is an
upper bound. What the walk did per command (the town: ~7400 commands a frame, 6970 of them from lists, 2700 TEXCOORD, 2800
vertices, 450 NORMAL, ~200 matrix commands): SIM_VCapList called Record for each command (NumParams twice, a push_back per word, G3Apply
through a call), and SIM_VCapMsg, which libntr calls again for every command of a list as it decodes it (with the capture suppressed),
built its 0x70-byte stack frame and stack guard before testing the suppression: ~35 instructions a command for nothing.

**The cut**, the same words and the same state (the .vcs format and the in-memory stream unchanged):
1. A list is recorded by one loop (RecordList): the parameter counts from a 256-entry table, the room for the list's worst case grown
   once (the stream's buffer is a plain realloc'd array now, not a std::vector), the header and parameters stored straight, the commands
   that are most of a list (the vertices, NORMAL, TEXCOORD, COLOR, BEGIN/END) followed inline, the rest (matrices, materials, lights)
   through G3Apply; a SWAP_BUFFERS inside a list still ends the stream there. The immediate commands still go through Record.
2. SIM_VCapMsg tests sSuppress first and returns (4 instructions); the recording is a separate function.
Off the game: a list walk alone, replayed from the town capture's list commands (scratchpad vcaplist/vb/bench.cpp, the old and the new
code side by side, the same words and state out): 0.77 -> 0.37 M instructions a frame, of which G3Apply is 0.04 M.

| row | before (base, 2 runs x 2 rows) | after (2 runs x 2 rows) | change |
| --- | --- | --- | --- |
| x4 | 17.40, 17.32, 17.43, 17.35 | 15.45, 15.30, 15.44, 15.29 | **-2.0 M (-11.6 %)**, ~1.0 M a tick |
| 60 Hz | 15.20, 14.98, 15.27, 14.87 | 14.22, 14.02, 14.25, 14.02 | **-0.95 M (-6.3 %)** |
| own | 18.49, 18.25, 18.50, 18.23 | 17.70, 17.29, 17.51, 17.31 | **-0.9 M (-5 %)** (mids 59-60/s both) |

Step 1 alone (an earlier pair of runs): x4 -1.2 M, 60 Hz -0.67 M; step 2 added the rest. Cycles and ms moved with the cores (load ~2.5)
and say nothing here. Left in the walk: ~0.37 M a frame for the list loop (header + parameters + the state, ~50 instructions a command)
and the immediate commands; following the state on the worker instead would need every frame's stream there, dropped ones too.

Checks: an in-run self-check build (both the old and the new list walk on every list, words and state compared): 131072 lists in a
walking town run, 0 differ. File captures (BWPORT_VCAPTURE at f1500 and f1700 of a pinned walking run, BWPORT_PRESENT=gl) by the base and
the new build: byte-identical .vcs files. Pinned shots at f1500 and f1700, still and walking ("right*f120@f1300 down*f60@f1450
left*f180@f1550"), gpu Exact, gpu Fast at 4 samples and vcore, base against new: the game region (columns 0..1024) identical in all 12.
picsab2 444/444 (vcore empty md5 lines 0) and vcab 74/74 (port/gpu and port/vcore are not changed). The iOS device target builds.
The file capture crashes under the Metal present (SIM_VCapFrameEnd reads the window with GL calls, before and after this patch): record
.vcs files with BWPORT_PRESENT=gl.

## Instructions per frame on Linux (2026-11-19, tools/perf_frame.py; first cut, repeatability NOT yet reached)

`python3 tools/perf_frame.py platinum town-walk [--start F --window N --repeat R --env BWPORT_RENDERER=vcore BWPORT_RESOLUTION=3 --json out]`
runs the Linux port headless (tools/port_run.py `--no-gdb --skip 1`, uncapped, the play's pinned clock and seed) twice, to V-blank
START and START+WINDOW (`--frames`), counts instructions, core cycles and task clock (user space) from the exec on with
perf_event_open through ctypes (no perf binary; `BWPORT_RUN_WRAP` in port_run puts the counting wrapper in front of the exe), and
prints M instructions, M cycles and CPU ms per drawn frame of (B - A) / WINDOW: the whole process (an inherited counter, exact),
each named thread (a counter opened on each tid by a 2 ms poll: `bwport` = the game thread, `llvmpipe-N` = the software GL's
workers, `SDLAudioP1`, `PXI`), and `other` (short threads). Needs perf_event_paranoid <= 2 and a `cpu` PMU.

Where it works: a desktop's WSL and a build server's WSL list the `cpu` PMU (paranoid 2; the counters read: tested here only, the build server
only listed); the laptop's WSL (Core Ultra 7 256V) has no `cpu` PMU in /sys/bus/event_source/devices, so no instructions there.
Windows-native counters are out of scope: they would need the ETW PMC provider (an elevated `xperf`/`wpr -start CPU` with
InstructionRetired as a profile source, or the Intel/AMD driver route), a separate collector, and are not in the process.

Findings (the town-walk, Platinum, this PC, load 3-23 from other agents):
- the counters are exact: the same run to V-blank 2000 gives the game thread 9936 / 9941 / 9939 M instructions (0.05 %), the
  whole process 14436 / 14313 / 14311 M (0.9 %: the llvmpipe workers' share moves with scheduling, 4.27-4.41 G).
- up to V-blank 7600 the totals agree within 0.3 %, but after the walk starts (V-blank 7700-8100) the game thread's total moves
  between runs: 7800: 78 / 85 / 52 G, 8100: 109 / 130 / 140 G, and its rate is a steady ~2 G instructions a second whatever the
  frames done: the game thread burns instructions while it waits for something that depends on wall time or the other threads
  (a spin, not work). So a window difference has a spread of 30-800 % here, and Black's route1-battle (400..1000) is as bad.
  With `BWPORT_CFGSCRIPT=f1:ff=0` the totals agree (505-520 G) but are 5x larger (the paced wait spins). The instructions/frame
  number is therefore only as stable as the game thread's waits: finding that wait (a sleep or a yield instead of a spin in the
  port, the way the Mac bench avoids it by running at its real pace) is the open item; it is not found yet. gdb attached from
  outside gave nothing in the time available.
- the GPU line: llvmpipe is the GL here (software), the timer-query readout says 0-7 ms of CPU time that is already in the
  threads above; no real GPU number on this PC.
- `ff=1`/uncapped, Renderer gpu on llvmpipe: Exact and Fast are the same (Fast is Metal only); vcore and Resolution 1/3 are
  `--env` settings, not measured because of the spread above. No docs/perf-ledger.jsonl rows were added for the same reason.

Next: locate the game thread's wall-time-dependent wait (sample the game thread's PC in the wrapper with a perf_event
PERF_SAMPLE_IP ring or by gdb at frame 7800), remove or account for it, then rerun `--repeat 3` idle and under load and record the
baseline with `tools/perf_ledger.py add ... where=linux`.

### The "spin" was fast forward's present limit (pf2, 2026-11-19)

Not a spin: the per-thread counters (perf_frame's JSON now keeps each thread by tid, `tids`) and gdb samples showed the game thread
blocked in GL waits and the extra instructions in llvmpipe's threads and the renderer's workers (vcore's p22/p20 pools). The cause is
libntr-ios-pace.patch's fast-forward rule in SpPaceBegin (sim_main.cpp, the `!capped` branch, `limit = sFastForward`): with ff=1 a
tick is drawn and presented only when 1/60 s of WALL time has passed since the last present. A play runs with ff=1, so the drawing
cost per game frame followed the run's speed: ~1700 fps idle drew 1 tick in ~30, a loaded run at ~40 fps drew every tick. Same
8100 frames of town-walk: total 73 G idle, 251 G with 26 busy loops beside it (llvmpipe 4.5 -> 30 G a thread, the workers 3.7 -> 21 G).
pf1's ff=0 runs (Unlimited: every tick drawn) agreed for that reason, and were 5x larger because every tick was drawn, not because a
wait spun. It is the intended player behaviour (fast forward costs at most 60 pictures a second) and stays.

Fixes (tools only, no game or libntr change):
- tools/perf_frame.py runs with `BWPORT_CFGSCRIPT=f1:ff=0` (every tick drawn and presented) and `BWPORT_PRESENTDROP=0` (LagSubmit
  drops a present while two are waiting on the GPU: timing-dependent too) unless `--env` overrides them; each run's counters are kept
  (build/perf_frame/aN.json, bN.json), the main thread is its own row (`bwport (main)`), `bwport` = the renderer's workers, and
  `port (main + workers)` is their sum; each row prints its run-to-run spread.
- tools/port_run.py: `--play NAME --realtime` set ff=1 anyway, so a "60 fps timing run" ran uncapped (route1-battle --realtime: 700
  frames in 1 s). --realtime now keeps the cap (59 fps). Timing numbers taken with `--play --realtime` before this were uncapped.

Real players at 60 fps (Linux): SpSleepUntil spins its last 150 us; `ablate=spin` (no spin) changed the total by 0.04 % (151.82 /
151.76 M instructions a frame, route1-battle --realtime, 2 runs each, spread 0.06 / 0.18 %): nothing to win on Linux. Windows
spins 600 us (2.5 ms without the high-resolution timer) a sleep, ~3.6 % of a core at 60 fps by that arithmetic; not measured (no
Windows run here), the next candidate if battery on Windows laptops matters.

Repeatability now (Black route1-battle, V-blank 400..700, Renderer auto (vcore on llvmpipe), Resolution 1, `--repeat 3`):

| | total M instr/frame | spread | port (main + workers) | spread | main | workers |
| --- | --- | --- | --- | --- | --- | --- |
| load 15-16 (other agents) | 147.6 | 0.36 % | 81.6 | 0.37 % | 6.1 | 75.5 (0.03 %) |
| + 26 busy loops (load 26-37) | 153.0 | 1.54 % | 85.5 | 0.38 % | 9.8 | 75.7 (0.16 %) |

The workers are exact; the game thread grows under load (6.1 -> 9.8 M): it still holds wall-clock work, most likely the sound
mixing that runs on it (acore's ac_mix / hw_sample were in its samples; the music keeps its real-time clock in uncapped runs,
SIM_FastForwarding) and the GL fence polls (LagPoll). Next: count the game thread with the sound off or pinned to game frames
(a bench switch in acore_glue: mix by V-blank, not by the callback's clock), then record the Platinum town-walk and Black
route1-battle rows for Renderer gpu / vcore x Resolution 1 / 3 (`--env BWPORT_RENDERER=... BWPORT_RESOLUTION=...`; a Platinum
pair of runs to 7600/7900 drawing every tick takes ~3 min here). Platinum town-walk with the new defaults (window 300, 2 runs,
load ~38): the workers exact (405.75 / 405.75 G at 7600), llvmpipe 1.1 %, the main thread 0.5 %.

### The main thread under load: the music bus and the GUI (pf3, 2026-11-20)

`BWPORT_PERF_SAMPLE=PERIOD [BWPORT_PERF_SAMPLE_FROM=V-blank]` in perf_frame's wrapper samples the main thread's instructions with
their user call chains (perf_event ring, no perf binary) and writes `profile` into the run's JSON: exe functions by addr2line,
library samples as `lib <- the exe function that called it`. WSL caps samples at 2500 a second (perf_event_max_sample_rate): use a
period of 1 M (100 k lost two thirds of the samples, unevenly). Black route1-battle 400..700, main thread, M instructions a frame:

| | 246 / 180 fps (other agents' load) | 19-24 fps (+26 busy loops) |
| --- | --- | --- |
| music bus on (the default), all | 6.14 | 10.33 |
| music bus on, hq_output (acore's channel output) | 0.58 | 1.91 |
| BWPORT_MUSICBUS=off, all | 8.48 | 9.63 |
| off, hq_output | 2.05 | 1.96 |
| off, libgallium <- ImGui_ImplOpenGL3_RenderDrawData (+ its `?`) | 2.70 | 3.69 |

1. The music bus (libntr-mu83.patch) runs in fast forward and Unlimited: the music is made as the device plays it, on the game
   thread, so it followed the wall clock. perf_frame now sets `BWPORT_MUSICBUS=off` (the music in each V-blank's sound at Speed 1:
   what a 60 fps player pays); its share then holds (2.05 / 1.96).
2. The rest of the growth (+1.0 M) is the GUI's GL drawing (sim_gui.cpp's panel: FrameGraph, the perf text), whose content
   follows the frame times. perf_frame now hides the panel (cfgscript `side=0`; pf4 below): the main thread holds.
3. A new run dir's first start hashes the ROM on the main thread (launcher_core.c LC_Migrate, ~60 G instructions): it landed in
   run a only and gave a negative main thread. perf_frame now does a warm-up run first, and keeps its files and run dir in
   `--work DIR` (default build/perf_frame) so that two measurements can run side by side.

Windows sleep spin: SpSleepUntil spins its last 600 us (2.5 ms without the high-resolution timer), calling LagPoll (a
glClientWaitSync(0)) each turn while a present is in flight. Measured (pf4 below): kept as it is.

### The bench without the side panel; the Windows sleep spin measured (pf4, 2026-11-20)

perf_frame's settings are one dict, `BENCH_ENV` (--env's K=V over them): cfgscript `f1:ff=0,side=0` (every tick drawn; `side=0`,
the existing measurement switch, draws no side panel), `BWPORT_PRESENTDROP=0`, `BWPORT_MUSICBUS=off`, `BWPORT_SYSBURST=frames`
(new, port/src/sysmetrics.c: the sys line's GPU timer burst on 14 of every 60 frames, not of every wall second). Black
route1-battle 400..700, Renderer vcore, BD (30 threads, load 3 idle), 3 + 3 runs, M instructions a frame:

| | idle | + 26 busy loops |
| --- | --- | --- |
| side panel drawn (the old bench): main | 8.208 | 8.400 (+2.3 %) |
| side panel drawn: port (main + workers) / total | 83.67 / 148.9 | 83.89 / 149.7 |
| side=0: main | 5.195 | 5.196 (+0.02 %) |
| side=0: port / total | 80.67 / 100.6 | 80.70 / 100.8 |

The panel cost ~3 M on the main thread and ~48 M in all (llvmpipe drawing its frame graph and text): a frame-time-driven picture,
not the game's. (BD's 30 threads take 26 loops better than the desktop: there the panel moved the main thread ~10 %.)

Platinum town-walk 7600..7900 (pf3d's rows: main spread 52-98 %, negative cycles): with every run's counters and the sampler,
Renderer gpu's main thread to 7900 went 131.5, 106.7, 100.3, 100.3 G in consecutive runs (its GL programs, Mesa's cache, settle
over the first runs), while the window's own work was the same 6.38 G in each. So one warm-up run to frame 2 left a0..b0 at
-39 M a frame and one to 7900 at 6.9 against the settled 21.3. perf_frame now warms up to the window's end until two runs agree
on the main thread (0.5 %, at most 4: warm_up) and gives Mesa a cache dir under --work. The four rows (BD, 3 runs, ledger):

| town-walk | total | port (spread) | main |
| --- | --- | --- | --- |
| vcore 1x | 105.88 | 85.99 (0.06 %) | 5.16 |
| vcore 3x | 488.54 | 468.44 (0.08 %) | 5.32 |
| gpu 1x | 175.32 | 27.70 (0.22 %) | 21.32 |
| gpu 3x | 1118.91 | 58.61 (0.25 %) | 52.19 |

Black's four rows (pf3d) predate side=0 and this warm-up: gpu 1x there had the same first-pair effect (a re-run: main -1.4 in
run 0, 13.04 after); re-record them with the current perf_frame before comparing.

Windows sleep spin, measured on the laptop's Windows (idle, tools/win_sleep_bench.c, 600 ticks at 60 Hz with 2 ms of work each, two
runs): the high-resolution timer's own wake lateness (spin 0) is p50 390 us, p90 630, p99 930-950, max 1.1-1.2 ms.

| spin (high-res timer) | CPU a tick beyond the work | tick late p50 / p90 / p99 |
| --- | --- | --- |
| 600 us (the game) | 276-282 us (1.7 % of a core) | 0 / 1 / 150-280 us |
| 300 | 67-70 | 8 / 270 / 500-520 |
| 150 | 27-36 | 220 / 460 / 730-780 |
| 0 | 26 | 390 / 630 / 930-950 |
| no high-res timer, 2500 | 1980-1990 (11.9 %) | 0 / 0 / 7-38 |
| no high-res timer, 1500 | 970-980 | 0 / 0 / 16 |

600 us is this timer's p90 here (the p99 would need ~950, more CPU), and a shorter spin makes most wakes late for ~1.5 % of a
core; the desktop (pf3c, busy: p90 490-570) would gain at most ~100 us a tick from an adaptive clamp 100..600 (it sits at 600 on
the laptop), so nothing was changed. The no-high-res path (Windows before 10 1803) could spin 1500 on an idle laptop, but pf3c's
desktop had that timer's p90 at 1.0-1.6 ms under load: left at 2500.

## Where the cost goes (2026-11-25)

The test phone (iPhone 16 Pro Max, A18 Pro, 120 Hz; gpu 3x = 768 x 1152 for both screens, Metal, Smooth motion at Monitor, Exact,
Antialiasing off or 4 samples, HeartGold field): the overlay's GPU busy reads 40-50% with Antialiasing off and 60-85% with 4 samples
("gpu 10.5-11.5 ms ~63-69% busy", "cpu 0.6 (main 0.2 oth 0.4 aud 0.0)"). Is the chain well optimized, what costs most, and can our
tools tell? Measured on the M2 (SoulSilver ss-r30, Cherrygrove warp 553:403, a back-and-forth walk, the reference ini, the pretended
120 Hz link BWPORT_PANELHZ=120, real time BWPORT_FFMAX=0, load 2-4), build perf 4d561271e.

### The tools, and how far to trust them

| tool | what it gives | Mac | phone | trust |
| --- | --- | --- | --- | --- |
| sys line / overlay "gpu X ms ~Y% busy" (sysmetrics.c) | GPU ms a tick: every renderer and present command buffer's GPUEnd - GPUStart, summed | yes | yes (Detailed) | **overstates**: the present's command buffer starts (its vertex stage) before the frame's is done and waits for it, so the overlap counts twice (M2: 13.5 ms/tick ~80% where the timestamps say 10.5 ms, 63%). And any busy % is at the clock the GPU's governor picked (below) |
| bench rows' metal: line | GPU ms a present (spans), renderer ms/s, "GPU spans ~N% (overlaps count twice)" | yes | perftest log | same overlap; good for A/Bs of the same setting |
| Breakdown / perflog= (perf_NAME.txt) | CPU per stage (frame thread, the render call, the worker's stages) and, on Metal, GPU per pass from one command buffer a pass | yes | cfgscript only | CPU rows good. **The Metal GPU rows are wrong**: each split command buffer's span includes the wait for the one before (R, E, P, C ~10 ms each at 60 fps). Not fixed: use the GPU passes |
| passes=1 rows (perftest) | each pass its own command buffer, committed and waited for | yes | perftest | real per-pass times but serialised, the GPU idles between passes and clocks down ("bottom clock"): proportions only |
| ablate= | a pass taken out: what the frame costs without it | yes | cfgscript | good for ceilings; the picture is wrong |
| threads: line (bench rows) | CPU ms a drawn frame per thread name, instructions and cycles | yes | perftest log | good; cycles move with the clock, instructions do not |
| BWPORT_PRESENTLOG / PACELOG | per present and per second pacing, the W/I/G lines (worker, pairing, Stage2) | yes | log | timing of the pipeline, not cost |
| gpu_render --time, gpu_fastdiff --time | per-pass mins on a corpus frame, at full clock, glFinish/wait after each | yes | no | best for shader A/Bs; not the game's mix or clock |
| Instruments (ios_device_profile.py --template metal, xctrace on the Mac) | per encoder GPU time, counters (occupancy, limiters, bandwidth) | yes | cable + maintainer | the reference; heavy, not for a quick look |
| **GPU passes (new, 494237544)** | per present: ms by pass (S, R, E, P, SMAA, C, capture, present), busy % from the union of the timed stages, keyframe and in-between renders a second | yes | **yes: overlay Detailed, log** | stage-boundary timestamps in the frame's own command buffers (no split, no wait); off by default; overhead within noise (renderer 733 vs 741 ms/s, 3 runs each); pictures identical. Per-pass ms are at the governor's clock |
| **tools/mac_gpu_profile.py (new, 4d561271e)** | one run, a row per setting (Exact/Fast x AA x Smooth, optionally Frame rate Unlimited for the top clock), the table below | yes | --parse a phone log | as the GPU passes and the bench rows |

Gaps left: the GPU's clock (no public API; the Unlimited rows stand in for "top clock"), memory bandwidth (Instruments only), and
the Breakdown's Metal GPU rows (should read the timestamps instead of splitting).

**The clock matters more than anything else in reading busy %.** Apple's GPUs run at the lowest clock that keeps up: at the reference
settings the M2's pass R takes 3.98 ms paced at 120 Hz and 2.08 ms with Frame rate Unlimited (top clock); Antialiasing off paced
reads 62% busy against 64% with 4 samples, though its work at the top clock is 25-40% less. So 60-85% busy on the phone does not mean
the phone is 60-85% loaded: it means the governor runs the GPU at a clock where the frame takes that share. The cost is the work at
the top clock (or energy); the headroom is what Unlimited draws.

### The M2's table (tools/mac_gpu_profile.py, two runs agree within ~5%)

ms a render (keyframe or in-between) by pass; pres = the final present (the picture's quad to a 2232 x 1780 drawable plus the UI),
ms a present. Paced rows: 120 presents/s on the pretended 120 Hz link.

| row | renders/s | GPU busy | ms/present | S | R | E | P | C | pres | CPU cores | CPU ms/tick |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Exact, 4 samples, Smooth (the reference setting) | 60 key + 59 mid | 64% | 5.3 | 0.50 | 3.98 | 0.33 | 0.22 | 0.18 | 0.43 | 0.42 | 7.0 |
| Exact, AA off, Smooth | 60 + 59 | 62% | 5.1 | 0.40 | 3.58 | 0.28 | 0.32 | 0.26 | 0.60 | 0.40 | 6.6 |
| Fast, 4 samples, Smooth | 60 + 58 | 61% | 5.1 | - | 4.07 | 0.34 | 0.23 | 0.19 | 0.43 | 0.38 | 6.4 |
| Fast, AA off, Smooth | 60 + 59 | 46% | 3.8 | - | 2.40 | 0.28 | 0.33 | 0.26 | 0.53 | 0.38 | 6.4 |
| Exact, 4 samples, Smooth off | 60 | 50% | 4.2 | 0.67 | 6.04 | 0.44 | 0.30 | 0.25 | 0.57 | 0.35 | 5.9 |
| Exact, AA off, Smooth off | 60 | 35% | 3.0 | 0.41 | 3.75 | 0.27 | 0.32 | 0.26 | 0.58 | 0.35 | 5.9 |
| Fast, 4 samples, Smooth off | 60 | 47% | 4.0 | - | 5.95 | 0.45 | 0.31 | 0.25 | 0.48 | 0.35 | 6.0 |
| Fast, AA off, Smooth off | 60 | 26% | 2.2 | - | 2.37 | 0.27 | 0.33 | 0.26 | 0.53 | 0.35 | 6.0 |
| **top clock** (Unlimited, Smooth off): Exact, 4 samples | 360 | 90% | | 0.25 | 2.08 | 0.16 | 0.10 | 0.09 | 0.20 | | |
| Exact, AA off | 400-580 | 80-89% | | 0.14-0.20 | 1.20-1.57 | 0.10 | 0.10-0.13 | 0.09-0.12 | 0.19-0.24 | | |
| Fast, 4 samples | 415 | 90% | | - | 1.78 | 0.15 | 0.11 | 0.09 | 0.20 | | |
| Fast, AA off | 430-525 | 81-83% | | - | 1.09-1.38 | 0.12-0.15 | 0.16-0.20 | 0.13-0.16 | 0.21-0.22 | | |

At the top clock the reference setting is 120 renders x 2.68 ms + 120 presents x 0.20 ms = ~345 ms of GPU a second: **about 35% of the
M2**, paced at ~half clock (64%). The GPU timeline of one render (BWPORT_GPUPASSES_RAW): the vertex stages of every pass run ahead
in the first 0.1-0.4 ms; pass S (compute) 0.4-0.7 ms; then the fragment stages back to back (R, E's three draws, P, C); the present's
command buffer starts at once and its fragment stage waits for C. CPU (perf logs, threads lines): 0.35-0.42 cores; a tick's game
thread 2.1 ms (game logic 0.23, events + UI build 0.18, the render call 0.29, sound 0.22, the present call 1-2 ms of which most is
waiting for a drawable), the worker 3.4 ms with Smooth motion (the in-between's geometry and span setup), 1.0-1.4 without.

### The phone's readings, in ms a refresh

At 120 Hz a refresh is 8.33 ms. The overlay's 40-50% (AA off) is 3.3-4.2 ms a refresh, 60-85% (4 samples) 5.0-7.1 ms, both with
the present's wait counted twice (on the M2 that is ~20% of the figure) and at the governor's clock. The earlier phone passes rows
(serialised, bottom clock; gpu-cost.md "Pass R's fixed part on the A18 Pro") had R 6.6, E 0.5, P 1.7 ms at 4 samples: the same
proportions as here (R about three quarters). What scales differently on the A18 Pro: pass R is ALU-bound (Instruments on the M2:
ALU limiter 59%, occupancy 21%), so it follows shader throughput (6 cores against the M2's 8-10); the 4-sample layers' tile stores
and E/P's neighbour reads follow bandwidth (lower on the phone); the present renders at the panel's 1320 x 2868 (SDL's high-DPI
Metal view, nearest filter, the whole drawable cleared, then the touch controls drawn by ImGui): 3.8 M pixels against the Mac
window's 4.0 M, so the M2's 0.2 ms (top clock) is the order to expect, more for the touch controls' blended quads. These are the
open numbers; the overlay's new lines give them.

### Ranked costs at the reference setting (M2, top clock, Exact, 4 samples, Smooth at 120)

| # | cost | share | DS work or overhead | candidate | gain (est.) | effort / risk |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | pass R, 2.08 ms a render | ~72% | the DS work at that quality: every pixel walks its bin's polygon list, 4 samples ~1.5x AA off | gpu-cost.md's R cuts (row pairs, the inner run, done on the Mac, phone A/B pending); SMAA instead of 4 samples (R at the AA-off cost) | 25-40% of R | known work; the picture judged by eye |
| 2 | in-betweens: a whole S + R + E + P + C a refresh | ~50% of the 3D | Smooth motion's price; overhead when nothing moved | skip a mid whose paired vertices equal the keyframe's (P5); draw mids at AA off or a cheaper R | up to 50% standing, 0 walking | P5 is designed; small risk |
| 3 | repeat keyframes: the HG field's 3D runs at 30 fps, half the ticks bring the same vertex record (I lines: 224 repeats, 224 new in ~7.5 s), yet every tick renders | 25% of renders (Smooth on), 50% (off) | **overhead if the picture is the same**: to prove (the 3D input is more than the vertices: textures, palettes, registers) | reuse the last 3D layer (skip S, R, E, P) when the GPU path's whole 3D input equals the last frame's (pf82's "Next") | 25-50% of the 3D in 30 fps scenes | a hash of the input; risk: a missed input shows a stale frame |
| 4 | pass S, 0.25 ms | ~9% | DS work moved to the GPU (span records) | - | - | - |
| 5 | E + P, 0.26 ms | ~10% | DS work (edge marking, 4-sample resolve) | gpu-cost.md E/P items | small | - |
| 6 | the present, 0.20 ms at 4 M pixels | ~7% | overhead of scaling to the panel | present into a smaller drawable (the compositor scales) or C straight into the drawable | 0.1-0.2 ms a present | small; the look of nearest scaling |
| 7 | pass C, both screens at 3x, 0.09 ms | ~3% | the bottom screen re-composed every present even when it did not change: overhead, but small | skip the bottom half when its 2D input is unchanged | ~0.05 ms | not worth it now |
| CPU | 0.4 cores (M2), 0.6 on the phone | | span setup and geometry for every in-between; the present call's drawable wait | item 2 and 3 cut it too | | |

Not wasteful: no CPU readback or wait in the normal path; uploads are memcpys into shared buffers; one command buffer a render.

### What can be read on the phone

Settings, section Metrics: Metrics = **Detailed**. Two more lines at the overlay's bottom, refreshed each second:
"GPU ms/present S .. R .. E .. P .. C .. pres .." (each pass's GPU ms a present) and "= X ms, Y% busy; K key + M mid /s" (the
union of the passes, so the present's wait is not counted twice; key/mid: renders a second). Compare it with the old
"gpu .. ms ~..% busy" line above it. Then set Frame rate to Unlimited for ~10 s (Smooth motion off): the GPU runs flat out, and the
R value then is pass R's cost at the top clock (keep it short: the phone heats). Settings, About: Share latest log; the log has a
"gpu passes:" line every 10 s (and after each perftest row), which tools/mac_gpu_profile.py --parse turns into the table above.

## Work whose result cannot change: the 3D layer and pass C reused (2026-11-25, M2 macOS)

The audit above found two kinds of repeated GPU work (items 2, 3 and 7): HeartGold's field 3D changes at 30 fps yet every tick
drew it again, and pass C composed both screens every present though the bottom one rarely changes. Built (port/bridge/gpu_bridge.c,
port/gpu/src/g07_compose.c, commits a5fe7cc7d and 00548c851 on `reuse`):

- **The 3D layer reused.** The worker numbers each picture it makes, keyframe or in-between (R3Id): the number of the picture before
  when everything passes S, R, E, P read is equal, else a new one. Compared: the geometry's output (vertices after transform,
  lighting and clipping, field by field; polygons with their attributes; drawing order; SWAP_BUFFERS' parameter), the 3D registers
  (DISP3DCNT, edge colours, alpha test, clear colour/depth/image offset, fog, toon table, 1DOT_DEPTH), the texture and palette VRAM
  with the toon table (the slot's GpuTexData: by its build within a slot, by its bytes across slots), the texture regions' sizes, the
  options and the grid. An in-between is compared after its blend, so one whose lists did not move takes the number of the picture
  before (P5 above, on the GPU side). The GL thread draws nothing for a picture whose number the 3D layer holds. Not with the texture
  cache (Mipmaps on GL: its layers carry state) nor in the test modes that draw twice (TCAB, the timing mode, ablations).
  BWPORT_3D_REUSE=0 off (the numbering still runs: an A/B differs only in the drawing), =check.
- **Pass C per screen.** Each half of pass C's output keeps the key it was drawn from: the engine, its 2D (the pack's serials, carried
  by the 2D skip), the uniforms, the 3D layer's number, SMAA's weights, the 2D filter. One half changed: that half is drawn alone (a
  scissor, the other half loaded); none: nothing is drawn or uploaded. BWPORT_COMPOSE_REUSE=0 off, =check.
- BWPORT_PRESENTHASH now works under Metal (cd5a8940f: the presented picture read back and hashed), for the proofs below.

**Pictures identical.** (a) Smooth motion off is reproducible present by present: per-present hashes with both reuses on and both off,
each run's pictures in the order they changed compared (difflib; logs in scratch/reuse/logs): SoulSilver Cherrygrove
(ss-r30, warp 553:403: east 600 frames, the menu opened and closed, standing) in Exact and Fast with Antialiasing off, 4 samples and
Smooth edges: 244 pictures each, **0 in one run only**; Black's town walk to the lab and Cheren's text: 720, 0; Black's room, the
stairs' fade and downstairs' text: 265, 0; Black 2's battle with Hugh: 1121, 0; the GL present (Present=gl) with Filter2D scale2x,
mmpx, and Mipmaps: 244 each, 0. (b) Smooth motion on is not reproducible present by present even with both off (off against off: 781 of
791 matching presents differ: the in-between's blend follows the display link's phase), so the check modes prove it in the run: every
picture drawn, and each one the reuse would have kept compared byte for byte with what it would have kept. Cherrygrove in the six
settings: 460-885 3D layers and 1536-2548 screens a run, 0 differ; the 3D check alone (more in-betweens drawn): Exact 4 samples 451,
Fast 4 samples 482, Exact AA off 922 (501 in-betweens), the Black 2 battle 718, 0 differ; pass C's alone: Cherrygrove 2617 screens,
Black's town 597, 0; Black's town, room and battle with both, 0; GL: 436 layers and 836 screens, Filter2D scale2x 3598 and mmpx 3559
screens, 0. (c) Fast-forward toggles (tools/mac_fftoggle.py's steps, 8 x 4x/1x, Black's town, BWPORT_FFMAX=0): the checks with Smooth
motion off (899 layers, 4492 screens) and on (4208 screens) 0 differ; the pacing check PASS with the reuses on and off. (At 4x the
presents sample frames by timing, so a per-present A/B is not reproducible there: on against off 105/262 pictures in one run only,
off against off 142/115.)

**Cost** (tools/mac_gpu_profile.py, Cherrygrove, the test phone's ini, the pretended 120 Hz link, real time, load 1.6-2.2; off = both
BWPORT_*_REUSE=0 on the same build; command buffers a second with GPU work, GPU busy, GPU ms a present, C ms a render):

| row | walking: off | walking: on | standing: off | standing: on |
| --- | --- | --- | --- | --- |
| Exact, 4 samples, Smooth (the test phone's) | 60+60/s, 66%, 5.53 ms, C 0.19 | 59+60/s, 65%, 5.47 ms, C 0.14 | 60+60/s, 68%, 5.73 ms | **4+11/s, 16%, 1.38 ms** |
| Exact, AA off, Smooth | 60+60/s, 59%, 4.93, C 0.25 | 60+60/s, 58%, 4.82, C 0.19 | 60+60/s, 61%, 5.08 | **2+9/s, 12%, 1.02** |
| Fast, 4 samples, Smooth | 60+60/s, 62%, 5.14 | 60+60/s, 63%, 5.27 | 60+60/s, 64%, 5.34 | **4+12/s, 17%, 1.42** |
| Fast, AA off, Smooth | 60+60/s, 48%, 3.98 | 60+60/s, 47%, 3.90 | 60+60/s, 49%, 4.11 | **6+14/s, 14%, 1.17** |
| Exact, 4 samples, Smooth off | 60/s, 51%, 4.29 | **38/s, 35%, 2.95** | 60/s, 53%, 4.42 | **14/s, 18%, 1.51** |
| Exact, AA off, Smooth off | 60/s, 37%, 3.10 | **38/s, 26%, 2.19** | 60/s, 37%, 3.10 | **8/s, 12%, 0.96** |
| Fast, 4 samples, Smooth off | 60/s, 49%, 4.09 | **37/s, 33%, 2.76** | 60/s, 49%, 4.06 | **9/s, 14%, 1.18** |
| Fast, AA off, Smooth off | 60/s, 28%, 2.31 | **38/s, 20%, 1.68** | 60/s, 28%, 2.37 | **8/s, 11%, 0.90** |

Standing still the GPU's work falls by 70-80% in every row. Walking with Smooth motion off (and in fast forward, keyframes only) the
3D is drawn 38 times a second instead of 60 (the 30 fps 3D's repeats) and the GPU's time falls by about 30%. Walking with Smooth motion
on, every refresh shows a new blend (f 1/4, 1/2, 3/4 of the step, then the keyframe), so nothing repeats in the 3D (the audit's 25% was
an estimate: none of the four repeats); there pass C's bottom screen is the saving, 0.05-0.06 ms a render (C 0.19 to 0.14). CPU ms a
tick: within noise walking (6.23 against 6.06), less standing (5.55 against 5.87 with Smooth motion, 5.55 against 6.17 without: the GL
thread's uploads go with the draws). The checks' own cost: the worker's compares take 11 us a picture walking and 34 us standing (the
608 KB texture compare once a tick when the geometry is the same): 1.3-4 ms a second, 0.1-0.4% of a core; pass C's key is two
300-byte compares. In a whole run (the fast forward, walking, standing) the 3D was reused for 1892 of 2267 keyframes and 1075 of 1333
in-betweens standing, 362 of 2271 and 47 of 1329 walking.

**The present ("pres")**, measured, not changed: it scales with the drawable's pixels. M2, Exact 4 samples, standing: the 2232 x 1780
drawable (3.97 M pixels) 0.55 ms a present paced and 0.23 ms at the top clock (Frame rate Unlimited); a 960 x 540 window (0.52 M) 0.12
and 0.03: about 0.06 ms a million pixels at the top clock, 0.14 paced. The Mac's side panel (ImGui) adds nothing measurable (0.58
against 0.55, 0.22 against 0.23 without it). The phone's 0.57 ms at 1320 x 2868 (3.79 M) is the same rate per pixel. What would cut it:
(1) no present when neither the picture nor the UI changed (standing, pass C drew nothing for 513 of 600 composes: the picture was
the one before; the panel keeps the last picture, and ProMotion could lower the refresh): the largest cut, but it touches the pacing and the touch controls' feedback;
(2) a drawable smaller than the panel (the compositor scales it): 2/3 of the panel's size a side is 44% of the pixels; (3) skipping the
clear saves nothing on a tile GPU (the store of every tile is the cost).

**What the test phone shows** (Metrics Detailed): "GPU ms/present" R, E and P fall with the renders reused and C with the screens kept;
"= X ms, Y% busy; K key + M mid /s" counts command buffers that did GPU work, so standing still in the field it drops from about 60 + 60
to about 4 + 11 a second and the ms a present to about a quarter; in fast forward (keyframes only, as the test phone's reading R 1.53, E
0.48, P 0.34, C 0.42, pres 0.57 likely was) walking about 37% fewer renders. The present is then the largest item standing.

Left: the present when nothing changed (above); the worker still prepares (spans and bins) every picture, also the reused ones (a
reused keyframe could skip its prep: the GL thread can prepare from s->geo the rare time it must draw it); the texture cache's
statefulness (Mipmaps on GL, where the 3D reuse is off); the phone's own numbers (overlay, Detailed).

## The same picture not sent again (2026-11-25, M2 macOS)

The present ("pres": the picture scaled to the panel plus the touch controls) was the largest GPU item left standing still (the entry
above: the test phone's 0.57 ms at 1320 x 2868, 120 times a second, about 68 ms of GPU a second), and most of those presents drew
what the panel already showed. Built (port/src/metal_present.mm bwport_mtl_same / bwport_mtl_keep, sim_main.cpp SpKeep / PaceKept,
port/bridge/gpu_bridge.c MtlPicGen, port/gpu g07_out_gen, port/src/ios_input.m):

- **What is compared.** A present clears a drawable and draws the picture texture with the screen quads, then ImGui's draw data. It
  is the same as the last one sent when the picture is the GPU renderer's pass C output (the same texture) whose number has not changed
  (g07_out_gen: a new number whenever pass C draws into it, either half, or another pass does, g07cap_show; numbers are never given
  twice, also across a renderer made anew); the quads, viewport and filter are equal; ImGui's draw data hashes the same (vertices,
  indices, each command's clip rectangle, texture and ranges, the display size; one hash a frame) and no ImGui texture update is due
  (the dynamic font atlas uploads at the next present drawn); the drawable has the same size; and the last one sent had a drawable.
  vcore's pictures (uploaded), a present with a screenshot, and the first present after the background or the settings sheet are
  always sent. The 2D, the 3D layer, fades and brightness reach the picture through pass C; the overlay's text and the touch controls
  through ImGui. The Metal present has no filter of its own that animates (nearest, no time input).
- **A present kept** (SpKeep, at each of the three places a present is made: a tick's own, an in-between or repeat, the early first
  present) takes no drawable, encodes nothing, commits no command buffer and puts no fence in the GPU queue; the panel keeps its
  picture. For the pacing it is a present at its slot: the slot is used (the early present's slot is not presented again), the link's
  phase follows it, the fps and the longest gap count it, a kept in-between counts as shown and in time, and a press whose first
  answering picture it was is "shown" at it (and GPU-done at once once the present that sent that picture is).
- **The panel's queue (kKeepStill).** A present not sent is a refresh with no new picture, which drains the panel's queue (the panel
  model's "waiting"). In motion that brings the next new picture a refresh early, an uneven step, and another when the queue forms
  again. The first build kept every repeat: on the M2 at gpu 3x 4 samples, where the GPU is late often enough that the queue stands,
  a back-and-forth walk kept 6.5 repeats a second (the turns) and even steps fell from 97.5% to 90.0%. So a present is kept only when
  the last one sent is no longer on its way to the glass (not in flight on the GPU, not waiting in the model; LagPoll first), or when
  the picture has stood still for 4 presents in a row: the queue then drains in a still spell, where nothing moves to be uneven, and
  the first picture after it lands a refresh sooner. This keeps the queue alive on purpose while things move; it is the stopgap until
  the pacing decides the queue's depth itself (scratch/fable_pacing/PACING-ARCH.md I6, M1): with such a pacer a kept present is
  simply the free drain and the rule goes.
- **ProMotion's range.** The display link asked 80-120 Hz while Smooth motion runs. With few presents sent iOS may lower the panel inside
  that range; each rate that holds 0.5 s restarts the pacing's timeline (the "the panel runs at N Hz" path), a hitch when motion resumes
  (PACING-ARCH.md I7 / B9). It now asks exactly the panel's rate (ios_input.m), as Smooth off asks exactly 60. Lowering the rate on
  purpose while nothing changes (the panel's own power) was considered and not built: the slots follow the link's period, so a lowered
  link moves them to its coarser grid and the first changed picture would wait up to a low-rate period, then re-lock; keeping the slots
  on the 120 Hz grid while the link ticks slower needs a panel that shows a present at the next 120 Hz refresh from a lowered rate,
  which iOS does not promise and the Mac cannot test (the pretended link has no panel). It wants the phone: lower to 30-60 after ~0.5 s
  of kept presents, keep the pacing's period at the panel's maximum, raise at the first present sent, and read the first changed
  picture's presentedTime against its slot.
- **Touch feedback** never waits on it: a pressed touch button is drawn lit, which changes ImGui's draw data, so that present is sent.
  The draw data changes at a tick's UI build, as before; the skip adds nothing to a press's latency (overlay "input" / "to GPU": a
  kept present that answers a press is counted as shown at its slot).
- **Off:** sim_config.ini PresentSkip=off (default on; the Apple builds write the key) or BWPORT_PRESENTSKIP=0. =check (with
  BWPORT_PRESENTLOG and BWPORT_PRESENTHASH): every present made, and each one it would keep compared by its picture's hash with the
  last one it would send (a log line every 600, and at a normal exit). Metal only (Present=gl unchanged).
- **Readouts:** the overlay's first line in Detailed reads "120 fps (N sent)"; the pace log's line a second ends "kept N"; a bench
  row's metal: line ends "kept (not sent: the same picture) N/s" (its present count and the GPU passes' presents/s are the ones sent;
  the bench row's pres/s counts both, as the fps does); BWPORT_PRESENTLOG writes a K line for a kept present (a P line's fields; its
  lag serial is the last one sent's, so it has no S line).

**Pictures identical.** Smooth motion off, per-present hashes with the skip on and off (BWPORT_PRESENTHASH; the pictures in the order
they changed, difflib, as the entry above): SoulSilver Cherrygrove (ss-r30, warp 553:403, east, the menu opened and closed, standing)
244 pictures each, 0 in one run only (956 presents sent of 2701 made without the skip); Black 2's battle with Hugh 1121 each, 0 (3203
sent of 3838). Smooth motion on (not reproducible present by present), the check mode in the run: Cherrygrove walk, menu and standing
more than 1800 presents it would keep compared, 0 differ; the battle more than 1800, 0.

**Cost** (tools/mac_gpu_profile.py's GPU passes in bench rows, the test phone's ini, the pretended 120 Hz link, real time, load 2-7;
off = BWPORT_PRESENTSKIP=0 on the same build; presents sent a second (kept), GPU ms a second (the passes' busy share), CPU cores):

| scene | off: sent/s, GPU ms/s, cores | on: sent/s (kept/s), GPU ms/s, cores | GPU |
| --- | --- | --- | --- |
| Cherrygrove standing (NPCs, sea: the picture changes ~15-40 times a second) | 117, 173, 0.30 | 17 (103), 97, 0.28 | -44% |
| Cherrygrove, walking back and forth | 120, 629 / 597 | 119 / 120 (0 / 0), 620 / 610 | same |
| the field menu open | 120, 134, 0.30 | 11 (109), 55, 0.27 | -59% |
| Smooth off: standing / walking / menu | 117 / 120 / 120: 149 / 325 / 130 | 13 / 119 / 13 (106 / 1.5 / 108): 68 / 342 / 57 | -54% / same / -56% |
| Black's room, standing | 98, 59, 0.22 | 1.5 (118), 1, 0.05 | -98% |
| Black 2's battle with Hugh: text and intro / waiting / moves | 120: 409 / 263 / 461 | 109 / 55 / 110: 392 / 205 / 471 | -4% / -22% / same |

Where the picture stands still the present's whole cost goes (0.4-0.6 ms a present here, the phone's 0.57) and with it the swap's CPU
(swap ms/s 10-30 -> 1-4) and the CPU time around the present (Black's room 3.6 -> 0.8 ms a tick); walking nothing changes. Smooth off
at 120 Hz shows each picture twice; at gpu 3x 4 samples the M2's second present follows the first while that one is still on its way
(the queue stands), so the rule above sends it (walking keeps 0-1.5 a second). Where the GPU keeps up (gpu 2x, Antialiasing off) a walk
with Smooth off keeps 74 of 120 presents a second (the repeats and the 30 fps 3D's), with the same steps on the glass (0 / 16.7 ms:
630 / 628 refreshes against 631 / 628) and each picture a refresh sooner (pa.py's display - game 23-34 ms against 31-42: the repeats
no longer refill the queue).

**Pacing.** The walk back and forth (Cherrygrove, Smooth on, 10.5 s windows, pa.py with K lines counted, scratch/presentskip/pa2.py):
the skip keeps 0 presents walking at gpu 3x 4 samples (even steps on 95.1% / 85.5%, off 82.3% / 98.2%: the load's spread, the presents
are the same) and 0-0.7 a second at gpu 2x without antialiasing (on 99.2% / 99.4%, off 98.6% / 100%); drops 0-9 both. Frame rate 60,
Smooth off: even steps 50.2% / 50.1% (0 and 16.7 ms by design), 17 of 60 presents a second kept walking (the 30 fps 3D's repeats).
tools/mac_fftoggle.py (BWPORT_FFMAX=0, 20 toggles, Black's town): PASS, 1.00 / 1.00x, 118.9 / 120.0 presents/s, 57.5 / 60.2
in-betweens/s, drops 1.0 / 0.0 a second; --smooth 0: PASS (kept 17-64 a second there). Standing, the model's queue drains: every sent
present lands one refresh after its slot (Cherrygrove standing: 31 of 31; off: 0 of 399, all two), so the display lag standing is a
refresh shorter.

**The first picture after a still spell** (Black's room, a step every 3.3 s): the 39 presents sent after 30 or more kept in a row all
landed one refresh after their slot (on time; called 0.5 ms after the slot on average, 3.8 at most). The walk's first second after 4 s
standing in Cherrygrove (PACING-ARCH.md I5, the GPU clocked down by the reuses and the skip): the queue forms again at once on the M2
(98% of the first second's presents land two refreshes after their slot; off 100%, where it never drained), and the GPU lag of those
presents is the off run's (2.36 ms mean both; max 14.3 against 11.0): no sign on the M2 of a slower GPU at the start of a walk.

**On the test phone** (Metrics Detailed): standing still in a quiet place "120 fps (N sent)" with N small (Black's room ~2, a busy town
15-40), walking N about 120; "GPU ms/present" with "= X ms, Y% busy" falling standing as above; touch buttons light at once; "waiting"
reading 0 more often (a still spell drains it) and the display lag a refresh shorter standing; "the panel runs at N Hz" lines should
not appear (the link now asks exactly 120). If anything looks stale (a picture that does not update), PresentSkip=off in the ini is
the A/B.

Left: a present that got no drawable is still entered in the panel model as shown (PACING-ARCH.md B2; the skip itself records a
present as sent only with a drawable, so it sends the next one); the ProMotion lowering above; the skip under Present=gl and for
vcore's uploaded pictures.
