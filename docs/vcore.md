# port/vcore: a DS video core from the hardware

port/vcore is a new video core for the PC port, written from the hardware's description (GBATEK) rather than the
NitroSDK's functions: the two 2D engines, the 3D geometry and rendering engines, the VRAM banks and display capture,
in software at native resolution (pixel-exact first; upscaling comes later). Nearly every port bug so far was in
libntr's graphics simulator, which imitates the DS at the level of toolkit calls; a core that copies the hardware
can be checked pixel for pixel against a reference.

It is also a measurement: how much of it 20 agents, each owning one piece, can build at once, each piece checked
automatically against melonDS. This file is the plan they work from: the architecture, the interface, the build and
test commands, and for each piece its files, functions, GBATEK sections, the corpus features that exercise it, the
unit tests to write and its done-check.

The groundwork: g51c this skeleton and file, g51a the frame capture and corpus, g51b the melonDS oracle,
g51d the compare tool; g50a's plan and .vcs format before them.

## Layout and ownership

```
port/vcore/
  meson.build, list_sources.py   its own meson project (not part of the port's build); sources = src/*.c, tests/*.c
  include/vcore.h                the public interface                                       SKELETON
  src/vc_types.h                 shared types, constants, register offsets, opcodes         SKELETON
  src/vc_shared.h                VCoreState, inline register/VRAM access, every cross-piece
                                 function's declaration (with its owner)                    SKELETON
  src/vc_state.c, src/vc_png.c   the .vcs loader, the outputs, the PNG writer               SKELETON
  src/pNN_<name>.c / .h          piece NN: its code / its own state struct VC_PNN           PIECE NN
  tests/vct.h, tests/vct.c       the unit-test framework and runner                         SKELETON
  tests/t00_core.c               the skeleton's tests                                       SKELETON
  tests/tNN_<name>.c             piece NN's unit tests                                      PIECE NN
  tools/vcore_render.c           vcore_render FILE.vcs OUT.png                              SKELETON (+ piece 20)
  tools/mkvcs.py                 synthetic .vcs frames                                      SKELETON
  tools/vcs_features.py          the features a frame uses (the vocabulary below)           SKELETON
  tools/vcs_mask.py              a frame with only some layers / no effects                 SKELETON
```

- A piece edits only its own three files (pNN_*.c, pNN_*.h, tests/tNN_*.c) and may add more files named pNN_* in
  src/ or tests/ (the build picks every *.c up: `meson setup --reconfigure build/vcore port/vcore` after adding one).
- The SKELETON files are frozen while the pieces run: the main session (or piece 20, by request) changes them. A
  piece that needs a shared type or a cross-piece function changed writes the request in its report; meanwhile it
  keeps its data in its own VC_PNN struct (in its header, reached as `s->pNN`), indexed by polygon or vertex number
  when it is per-polygon.
- A function another piece calls is declared in vc_shared.h with its owner; its body is in the maintainer's file. The
  stubs compile, draw nothing and return 0 (vc_mosaic_*_y return y).
- No libntr, no NitroSDK headers, no GPL code: melonDS (<ref>/melonDS) is the reference to compare against, never
  copied. GBATEK (<ref>/gbatek/, INDEX.txt) is the spec. Reading melonDS's code to settle a detail GBATEK leaves
  open (an interpolation's precision) is allowed only to understand the hardware; write the code yourself and say in
  a comment which GBATEK section it follows.
- C11, no compiler extensions beyond `__attribute__((constructor))` (the tests'); it must build with gcc, clang and
  mingw (the win64 exe will link it later).

## The interface

`include/vcore.h`: `vcore_create`, `vcore_load_vcs(s, path, err, len)` (or `_mem`), `vcore_render(s)`,
`vcore_screen(s, 0|1)` (top / bottom, 256x192 pixels r6 | g6 << 8 | b6 << 16, the DS's 18-bit colour as melonDS
holds it), `vcore_screen_rgba` / `vcore_frame_rgba` (RGBA8, c8 = c6 << 2 | c6 >> 4; the frame is 256x384, top first),
`vcore_libntr_shot` (the .vcs file's LSHT picture), `vcore_write_png`.

The input is a .vcs file: one frame's whole video state, captured from the port (BWPORT_VCAPTURE, g51a) or written
by a tool. The format is <ref>/VCS_FORMAT.md (g50a's design; g51a documents it as built): "VCST", version 1, then
chunks (4-byte tag, u32 size, data padded to 4):

| chunk | size | content | in VCoreState |
|---|---|---|---|
| INFO | text | key=value lines (frame, source, notes) | `info`, `vc_info()` |
| IO9  | 0x700 | 0x04000000-0x040006FF: engine A, 3D registers, VRAMCNT, POWCNT1 | `io9`, `vc_io16/32`, `vc_eng16/32(s, 0, ofs)` |
| IO9B | 0x70 | 0x04001000-0x0400106F: engine B | `io9b`, `vc_eng16/32(s, 1, ofs)` |
| VRAM | 0xA4000 | banks A-I, each its own memory, in order (A-D 128K, E 64K, F G 16K, H 32K, I 16K) | `vram`, `vc_bank(s, b)` |
| PLTT | 0x800 | A BG, A OBJ, B BG, B OBJ (0x200 each) | `pltt`, `vc_pal()` |
| OAM  | 0x800 | A, B | `oam`, `vc_oam()` |
| GX3D | words | the 3D commands of the frame the 3D layer shows: per entry a header `op \| n << 8 \| flags << 16` (flags 1 prologue, 2 from a display list, 4 a register write) and n parameters; a prologue first rebuilds the geometry engine's state (matrices, stacks, lights, material) | `gx`, `gx_words` |
| LSHT | 256x384x4 | libntr's own picture (RGBA8) | `lsht` |
| L3D  | 256x192x4 | libntr's 3D layer | `l3d` |

Missing chunks leave their memory zero; unknown chunks are skipped. POWCNT1 is read as the oracle reads it by
default: the LCD bit forced on, and 0 (a capture that never saw the register) taken as 0x820F. The 3D registers in IO9 are the ones in force
when the frame was taken. Main memory display (display mode 3) and the main-memory capture source have no data in
the file (they show / capture zero).

## The frame

`vcore_render` -> `vc_render_frame` (piece 20, p20_frame.c; the skeleton wrote a working driver):

1. `vc_vram_map` (12): VRAMCNT_A-I -> `s->rgn[]`, the VRAM as each unit sees it (16 KB pages, each the OR of the
   banks mapped there; read with `vc_rd8/16/32(region, addr)`, the address wrapping at the region's size). Regions:
   engine A/B BG and OBJ, LCDC, the BG/OBJ extended palettes of each engine, the texture and texture-palette slots.
2. `vc_3d_frame` (20): resets the geometry engine (`vc_mtx_reset` 13, `vc_poly_reset` 14, `vc_light_reset` 15), runs
   every GX3D entry through `vc_gx_command` (13: dispatches by opcode to `vc_cmd_mtx` 13, `vc_cmd_vtx` /
   `vc_cmd_poly` 14, `vc_cmd_light` 15, `vc_cmd_swap` 18). The geometry engine fills `s->geo` (vertex RAM, polygon RAM
   in screen coordinates); SWAP_BUFFERS (18) marks the translucent polygons and sorts `s->geo.order`. Then the
   rendering engine: `vc_3d_clear` (19), `vc_raster_poly` (16) for each polygon in order, which samples textures
   (`vc_tex_sample`, `vc_tex_blend` 17, `vc_toon_blend` 19) and writes each pixel through `s->plot` = `vc_3d_plot`
   (18: depth test via `vc_3d_depth_test` 16, alpha test, blending, polygon IDs, shadows); `vc_3d_post` (19: edge
   marking, fog, antialiasing) leaves the 3D layer in `s->f3d.out`.
3. For each engine and line, `vc_render_line` (20) into a `VC_Line`:
   `vc_obj_line` (5, with 6 and 7) -> `obj[]`, `objwin[]`; `vc_bg_line` (20: the BG mode table) for BG0-3 ->
   `vc_bg_text_line` (1) / `vc_bg_affine_line` (2) / `vc_bg_ext_line` (3) / `vc_bg_large_line` (4) / the 3D layer
   `vc_3d_layer_line` (20) -> `bg[n][]` or `l3d[]`, with the BG mosaic (4); `winmask[]` preset to 0x3F then
   `vc_window_line` (8); `vc_compose_line` (9) -> `out[]`; engine A: `vc_capture_line` (11); `vc_display_line` (10)
   -> `s->eng_out[e][y]`.
4. `vc_capture_end` (11); the screens from POWCNT1 bit 15 (engine A on top when set) -> `s->screen[0/1]`.

Pixel formats inside: a 2D layer pixel is a u32, 0 = transparent, else `VC_PX_OPAQUE | BGR555` plus, on the OBJ line,
the OBJ's priority (bits 16-17) and flags (VC_PX_SEMI, VC_PX_BMPOBJ with alpha in bits 24-27, VC_PX_MOSAIC). The 3D
pixels and every composited / output pixel are RGB6 (`VC_RGB6(r, g, b)`), a 3D pixel's alpha 0-31 in bits 24-28.
Matrices are 4x4 row-major 20.12 as the commands send them, row vectors (`v' = v * M`); vertex colours 0-31 per
channel; texture coordinates 12.4. vc_types.h has the details.

### Threads and the bridge's pipeline

`vc_render_frame` runs the 3D frame (`vc_3d_frame`: geometry, raster, post) on a thread, engine B on a second one and
engine A on the caller's: A draws everything but its 3D layer (`vc_render_line_a1`), joins the 3D thread, then
composites, displays and captures (`_a2`). Each engine has its own `VC_Line` (`s->p20`), the 2D pieces keep no state, so
the picture is the sequential one byte for byte; with a display capture running B waits for A (the capture writes the
VRAM B may read). A piece must therefore not keep state in statics or in `s` that two engines or the 3D and a 2D
piece both write. `vcore_render --seq` runs the old order (one thread); `--bench N` prints ms per frame by stage (the
stages are the running thread's CPU time, so they are steady on a loaded machine; the first number is the wall time
and `cpu` is every thread's).

Pass (speed pass 2, rasterizer, same bytes): the raster runs in 4 horizontal bands on threads
(`BWPORT_VCORE_BANDS`, 1 = off; `--seq` = one band): `vc_raster_band(s, p, y0, y1)` draws all polygons in order but
only rows y0..y1-1, and rows are independent (the default plot writes only its pixel's row of `s->f3d`; `p16.st` is
now written only when a test replaced `s->plot`), so the bands share `s` with no copies. The 3D thread's `3d_raster`
stage in `--bench` is then band 0's CPU time only (`cpu` has the sum). Per pixel: a plain polygon's depth test runs
before the colour, texture and sampling work (a failing pixel changes nothing; shadow polygons excepted), z is lerped
alone, `floor` is a cast, and `vc_tex_ctx_init`/`vc_tex_sample_c` (p17) decode a polygon's texture once (direct page
reads, palette entries converted once per index, last texel). Town raster 10.7 -> 3.7 ms on one thread, 1.0 ms in
4 bands.

Pass (speed leftovers, same bytes): format 5 (4x4 compressed) textures use the polygon's context too (the block's
four colours are built when the block changes). The raster rows are cut into strips of about equal estimated cost (a
polygon's bounding-box width on each row it covers; 2 strips per thread) that the threads take as they finish; a
polygon missing a strip is skipped in p20 before `vc_raster_band` (which built the texture context first). The threads
are a pool in `s->p20.pool` (created on the first banded frame, joined by `vcore_destroy` through `vc_p20_destroy`; a
changed `vc_bands` rebuilds it). Flyover raster on the 3D thread 2.9 -> 2.0 ms (one thread: 8.7 ms), wall 4.0 -> 3.5.
Process CPU time (`cpu` in --bench) is unreliable between the two versions on a shared machine; judge by wall and the
3D thread's raster time. win64: tools/port_win64.sh run passes BWPORT_VCORE_THREADS / PIPE / BANDS to the exe.

The bridge (port/bridge/vcore_bridge.c) pipelines: it gathers frame N on the game's thread into one of two slots,
draws it on a worker while the game runs frame N+1, and returns frame N-1's picture (the same pictures in the same
order, one frame later; a frame that captures is drawn before the call returns). `BWPORT_VCORE_PIPE=0` draws each
frame before returning, `BWPORT_VCORE_THREADS=0` also turns the engine threads off.

`port/vcore/tools/vcore_same.py` is the check for any speed change: it renders the pieces' emitted and fuzz scenes (seed
1, 100 each, kept in <scratch>) and the corpus with a reference `vcore_render` (build it from the
commit you start from, <scratch>) and with the new one, and compares the screens, the 3D layer, the
VRAM dump and the geometry dump byte for byte.

### vcore at N x

`vcore_set_scale(s, N)` (N = 1..8; the bridge passes `SIM_RenderScale()`, `BWPORT_VCORE_SCALE=N` overrides it) makes
`vcore_render` draw the 3D at N times the DS resolution on the CPU and hand the bridge a 256 N x 384 N picture
(`vcore_picture`; libntr's sized hook, libntr-vscale.patch). port/vcore/src/p21_scale.c, driven by p20's
`vc_render_frame_n`; scale 1 runs the old `vc_render_frame` and is not touched (vcore_same 2126 scenes, 0 differ). The
rules are port/gpu's (docs/gpu.md, "The DS rasterizer at N x"), so vcore N x is the GPU path's exact reference:

- Positions: every polygon vertex is the viewport transform of its clip coordinates at N times the polygon's own
  viewport (`VC_Poly.viewport`), 64-bit integers as p14; p16's own edge walk (`vc_span_poly` / `vc_span_row`) runs on the
  256 N x 192 N grid (the dropped right and bottom edges are one N x pixel), attributes per N x pixel by the same
  formulas (texels nearest at the finer coordinates, 9-bit colour, depth 24-bit). A polygon that collapses to one DS
  pixel draws an N x N block (`Dot=block`, default) or one N x pixel (`dot_fine`); lines (zero-area polygons) and
  wire-frames are walked on the DS grid and N wide (default) or on the N x grid (`line_thin`).
- Per pixel: p16's emit_pixel, p18's plot (depth, alpha test, translucency, IDs, shadows) and p19's clear (the rear-plane
  bitmap sampled at (x / N, y / N)), edge marking (a flagged edge pixel with a 4-neighbour of another ID and greater depth;
  the default outlines one DS pixel: the same-ID pixels up to N - 1 away along a row or column too; `edge_fine`: one N x
  pixel), fog (a density table by depth >> 9) and antialiasing (the coverage from the edge windows of the N x walk,
  neighbours one N x pixel away) are copies of the 1x code over the N x buffers (`VC_NFrame`: colour, depth, IDs, flags,
  coverage, 16 bytes a pixel), kept apart so that the 1x code stays byte for byte what it was.
- 2D: engine A's BGs, OBJs and windows are drawn at 1x as now (`vc_render_line_a1`); the picture composites each DS
  pixel's N x N sub-pixels with p09's `vc_compose_line` (N * N calls per DS row; the 3D layer's pixel under sub-pixel
  (i, j), scrolled by BG0HOFS times N, the 2D layers at the DS pixel), then master brightness. Lines without the 3D layer,
  engine B and the other display modes (off, VRAM, main memory, forced blank) are the 1x lines scaled by N.
- Capture: a frame with DISPCAPCNT's enable bit also draws the 1x 3D and runs the 1x lines, the capture is the DS's pixels
  (the bridge's StoreCapture as at 1x; checked: the VRAM dump equals the 1x core's on all 2126 scenes at N = 3).
- Threads: the raster pool's bands grow with N (`vc_bands` + 2 (N - 1), at most 16; BWPORT_VCORE_BANDS overrides), rows cut
  by the same cost estimate; the picture's compositing runs on the pool too; geometry, clear and post are on the 3D thread.
- Checks: `BWPORT_VCORE_NPATH=1` forces the N path at scale 1: its picture equals the 1x screens on all 2126 vcore_same scenes;
  tests/t21_scale.c; `python3 <scratch> N SCENES` (vcore --3d --scale N against gpu_render --scale N --hi).
  `vcore_render F.vcs OUT.png --scale N [--3d LAYER.png] [--dot fine] [--line thin] [--edge fine] [--bench K]`.
  Memory: 16 bytes a pixel plus the picture, per slot (two slots): N = 4 about 45 MB, N = 8 about 180 MB.
- Not done: a texture filter for the 2D layers (nearest), per-polygon viewports are stored by p14 and used; the raster costs
  what the 1x raster does per pixel (about 40 ns an emitted pixel: doubles, perspective divisions), so it grows as N squared
  (town frame on one thread: 1x 3.7 ms, 4x 52 ms; with the pool 10 ms wall on an idle machine).

## Build, run, test

From the worktree (`~/bw <id> '...'`):

```
meson setup build/vcore port/vcore            # once (and --reconfigure after adding a file)
ninja -C build/vcore                          # the library, vcore_tests, vcore_render
build/vcore/vcore_tests                       # all unit tests: PASS / FAIL / TODO lines and a summary; exit 1 on a FAIL
build/vcore/vcore_tests p05 -q                # one piece's tests (a substring of the test or file name), failures only
build/vcore/vcore_render F.vcs OUT.png        # both screens, 256x384
    [--3d L.png] [--geo G.txt] [--bank X B.png] [--dump-vram V.bin] [--libntr LS.png] [--info]
python3 port/vcore/tools/mkvcs.py --all DIR   # synthetic frames: blank textbg obj tri mixed
python3 port/vcore/tools/vcs_features.py DIR [--index DIR/index.txt]   # each frame's features
python3 port/vcore/tools/vcs_mask.py IN OUT --only bg0,obj --plain     # isolate layers (file or directory)
python3 tools/vcore_compare.py compare A.png B.png --diff D.png        # match % per screen (g51d)
python3 tools/vcore_compare.py corpus --vcore build/vcore/vcore_render --oracle <ref>/oracle/oracle \
        --corpus DIR --out OUTDIR [--libntr]                           # every frame, per frame and per feature
```

The reference: `<ref>/oracle/oracle F.vcs OUT.png` renders the same file with melonDS's software renderers
(g51b, <ref>/oracle/README). The corpus: <ref>/corpus/*.vcs, 50-100 frames of the game (g51a: title, intro,
name entry, fly-over, fields, battle, menus, a capture fade) with an index; game-derived, never in git. Compare
match % = pixels equal after reducing both to 6 bits per channel.

### Unit tests

tests/vct.h: `VCT_TEST(pNN_name) { ... }` registers itself; `VCT_CHECK(cond)`, `VCT_EQ(a, b)` end the test on a
failure; `VCT_TODO("...")` marks a test not written yet. Helpers: `vct_state()` (a zeroed state, freed after the
test), `vct_io16/32`, `vct_eng16/32(s, e, ofs, v)`, `vct_pal`, `vct_map_flat(s, VC_RGN_ABG, buf, size)` (VRAM
without piece 12), `vct_gx(s, op, n, params)` (a 3D command), `vct_write_vcs`, `vct_emit(s, "pNN_scene")` (with
`VCT_EMIT=DIR` set, writes the test's state as DIR/pNN_scene.vcs: a synthetic frame for the oracle). Test a piece's
own functions with explicit inputs: a BG renderer on a hand-made tileset, `vc_obj_affine_uv` on numbers, the
rasterizer through a recording `s->plot`, the sampler on a 2x2 texture. Expected values come from GBATEK, worked by
hand, not from running the code.

### The geometry dump

`vcore_render --geo G.txt` writes the geometry engine's output in drawing order, one line per polygon
(`poly K index I nv N attr HEX tex HEX pltt HEX front F trans T y YMIN YMAX`) then one per vertex
(`  v SX SY z HEX24 w W rgb R G B st S T clip X Y Z W`). Pieces 13-15 compare numbers with the oracle's dump of
melonDS's polygon RAM in the same format (`oracle --geo`, requested from g51b; until it exists, against hand-worked
GBATEK examples).

## Oracles

Three independent emulators render a `.vcs` through harnesses with one command line (`OR FILE.vcs OUT.png [--3d L.png]
[--dump-vram V.bin] [--no-3d] [--no-capture] [--frames N] [-q]`; GPL, outside the repo): melonDS `<ref>/oracle/oracle`
(the reference, README there), NooDS `<ref>/noods/oracle` (`build.sh`, README), DeSmuME `<ref>/desmume/oracle`
(branch oracle-harness, `build.sh`). `vcore_check pNN --oracle melonds --oracle noods ...` (or `$ORACLES=a,b`) checks a piece
against any of them; `python3 tools/oracle_agree.py --all-pieces --corpus <ref>/corpus --synthetic` runs vcore and every
harness present over every piece's fuzz scenes and prints the pairwise agreement and the suspect list (scenes where the
emulators disagree; the scenes where all emulators agree and vcore differs are vcore's bugs). The r59 matrix is
<ref>/oracle_agree_r59.txt (1961 scenes). Read it as: vcore~melonDS is the copied rule; when NooDS and DeSmuME agree
with each other against melonDS, the melonDS rule is suspect.

Known limits (not harness bugs unless noted):
- NooDS: no ALPHA_TEST_REF and no DISP_1DOT_DEPTH (writes ignored); draws the 3D only while DISPCNT bit 3 is set (the
  harness runs one extra frame with it forced on when the capture has it clear); ignores DISP3DCNT bit 0 (the harness
  strips the textures); read texture/palette VRAM out of range on ~2% of the 3D fuzz scenes (SIGSEGV; the harness patches
  it: noods-guard.patch). Its 5->6 bit colour conversion and 2D/3D blend rounding differ by a level. p19 (post-processing:
  fog/edge/AA) agrees with melonDS on only 55% of the pixels: not looked into.
- DeSmuME: the 2D picture is 5-bit (the harness maps the pixels that come from it to melonDS's v5<<1 six-bit value, so the
  screens compare at 6 bits); master brightness is done in 8 bits (p10 differs); with POWCNT1 engines off it still draws
  (p10), VRAM display of a non-LCDC bank shows the raw bank (p11, p12); `--hires0/1` are accepted and reach the renderer
  but change nothing (its two clipper modes run the same code). Clipped polygons' vertex colours come out a level or two
  darker than melonDS's (up to 12 of 255 across a whole polygon, e.g. p14 fz0017/0019): NooDS agrees with DeSmuME there,
  so it is melonDS's rule that is the odd one (flat colours and unclipped gradients agree within 1 level of 6 bits).

## Checking a piece

Every piece agent runs this before reporting; nobody builds an oracle harness of their own any more:

```
port/vcore/tools/vcore_check pNN [--fuzz N] [--seed S] [--keep DIR] [--no-emit] [--scene F.vcs] [--show K] [--green-lsb]
```

(`pNN` = p01..p19; `--list` names them.) It builds `build/vcore` if needed, then renders with `vcore_render` and with the
oracle (`<ref>/oracle/oracle`, black box; renders cached by scene hash in `~/.cache/vcore_check`), in parallel over
`BW_JOBS` (default all cores; ~20 s for all 19 pieces at 100 scenes each), two scene sets:

1. **emitted**: the scenes `VCT_EMIT=DIR build/vcore/vcore_tests pNN` writes (your unit tests' `vct_emit`);
2. **fuzz**: N random scenes (default 100) from `tools/vcore_gen.py`, one generator per piece: random registers, VRAM
   banks and mappings, OAM, palettes or 3D command streams inside the piece's feature, everything else plain. A scene
   is a pure function of (piece, seed, index): `--seed S` gives a new set, a failing `fz_0042` can be re-made.
   Piece 11 also compares the VRAM banks after the frame (`--dump-vram`), pieces 13-19 compare the 3D layer
   (`--3d`, RGBA: colour at 6 bits where alpha > 0, alpha at 5 bits) and the rest both screens (6 bits per channel).

The output: scenes passed and pixel match % for each set, then for each failing scene (first `--show`, default 5) its
saved file, how many pixels differ, the first differing pixels (screen, x, y, mine, oracle) and the scene's register
values (DISPCNT, BGxCNT, scroll, windows, blend, VRAMCNT, POWCNT1, the OAM entries or the 3D piece's parameters), then
the names of the other failures. Failing scenes stay in `--keep DIR` (default `build/vcore_check/pNN/`) as
`NAME.vcs` + `.mine.png` + `.oracle.png` + `.diff.png`: open the .vcs in `vcore_render` / `oracle` to dig further.
`--scene F.vcs` checks any frame (a corpus frame isolated with vcs_mask.py). Exit status 1 on any failure.

What it does not do: it runs the whole merged pipeline, not the piece alone with stand-ins for its neighbours. The
generators keep the other pieces to their simplest settings (text BGs for the compositor's inputs, no effects unless
the piece is the effect), so a mismatch is usually the piece's; when the first differing pixel is on a layer you do not
own (VRAM mapping, the compositor), check the scene's registers before blaming your code.

Palette bit 15: melonDS turns a palette entry's bit 15 into the green channel's LSB (the "green-lsb" quirk, p01
extended palettes and others). The fuzz scenes clear bit 15 of every palette and of the extended / texture palette banks
E-I so that it does not drown the other mismatches; `--green-lsb` leaves it random to show the quirk. The emitted
scenes are not touched, so a piece whose own scenes use bit 15 fails them until the quirk is modelled.

Fuzz generator coverage: p01 text BGs (4/8bpp, sizes, scroll, engine A char/screen bases, extended palettes); p02 affine
modes 1/2/4; p03 modes 3-5 map16 / 256-colour / direct bitmaps; p04 mode 6 large bitmaps, BG mosaic on text and affine;
p05 normal OBJs (1D/2D, boundaries, 4/8bpp, extpal, flips, semi); p06 affine/double-size OBJs; p07 bitmap OBJs, OBJ
window, OBJ mosaic; p08 windows over BGs+OBJs; p09 BLDCNT/BLDALPHA/BLDY over BGs+OBJs+windows; p10 display modes,
POWCNT1, forced blank, master brightness; p11 DISPCAPCNT over random sources and banks; p12 random VRAMCNT for all nine
banks under BGs, OBJs, extended palettes and display mode 2; p13 random matrix command sequences; p14 triangles, quads,
strips, culling, clipping (big polygons), viewports; p15 lights, normals, materials, texgen; p16 plain polygons with
perspective/ortho, W-buffer swap bit, alpha 0 wireframe; p17 all seven texture formats, sizes, repeat/flip, colour 0,
modes; p18 translucent polygons, IDs, shadows, alpha test and blend; p19 clear colour/depth/image, edge, fog, toon,
AA. Piece 20 is checked by the corpus (`vcore_compare.py corpus`). To widen a generator, edit `tools/vcore_gen.py`
(its functions are one per piece, ~30 lines each).

### Finding a rule: the probe methods that worked

Earlier twelve accuracy agents (one per piece, a model) closed six pieces to 100% with these; use them before
editing code:

- **Does the game use it?** `python3 port/vcore/tools/vcs_features.py <ref>/corpus` lists every feature the 56 captured game frames use. The corpus has no shadow polygons, decal, toon, OBJ mosaic or OBJ window, and never sets the shininess table: three round-56 agents saved their budget by checking this first. A feature the game never uses gets its quirks noted in <scratch> not fitted.
- **Speed work is byte-identical work.** `python3 port/vcore/tools/vcore_same.py --ref OLD --new NEW` (2126 scenes) must show 0 differ after every step; reject work early.
- **Tabulate first, then fit.** Patch one register (or a few) on a small generated scene, sweep the inputs that
  matter, render with the oracle, read known pixels, and test candidate formulas against the whole table at once (p10:
  15 POWCNT1 x 4 modes x blank x brightness gave the rule table in one run; p11: 20480 blend cases, one rounding rule
  fit all). Fitting one axis at a time over-fits (p15).
- **Check the generator covers your feature.** Two generators never produced their piece's feature (p07 put the OBJ
  mode in OAM bits 8-9; gen_3d had TEXIMAGE_PARAM's format at bit 20): read the bits it sets before trusting a pass.
- **Isolate from other pieces.** Narrow your piece's fuzz to its own features (axis-aligned pixel rectangles, flat
  colours, ortho and whole-texel steps keep the rasterizer exact for p17/p18/p19). Equivalence probe (p13): replace
  your stage's commands with your computed result and render both with the ORACLE; identical images prove the miss is
  not yours. The oracle's `-v` polygon/vertex counts vs `vcore_render --geo` check geometry without pixel noise (p14).
  Switch the feature off on a failing scene: if it then matches, the miss is ordering, not rendering (p11).
- **Sanity-check every probe.** It must draw something (more than one colour on the layer), and a known-different
  pixel must show as different: a closure over a loop variable once compared mine with mine for 40k tokens (p07).
  The oracle draws no 3D until a VIEWPORT command is sent (vcore defaults to full screen).
- **Tags and levels.** Tag VRAM pages with bit-disjoint values so an OR of two banks differs from one bank winning
  (p12). To read a hidden level (a light level), intersect the floor intervals from many colour combinations; when they
  do not intersect, the level has more fraction bits than assumed (p15: 9 bits).
- **Use the cores.** Every command runs in the shared pool (bwworkers.slice: 28 CPUs at 80% each, shared by all agents; `$BW_JOBS` = 28): render probes with `xargs -P $BW_JOBS` or `multiprocessing.Pool(int(os.environ['BW_JOBS']))`, and never compare thousands of images in a single-core Python pixel loop (q16 waited minutes per hypothesis on one). Work on bytes: the alpha plane is `px[3::4]`; a row's first/last drawn pixel comes from `lstrip`/`rstrip(b'\0')`. To test a candidate rule, skip rendering: hwfit's C harness (`hwfit.py pack` -> `hwfit_raster.c`) scores a rule against every probe in seconds.
- **Cross-piece findings** go to `<scratch>` with a repro; the main session hands them to the
  owner of that piece. Keep piece-specific generator code inside `if P(NN):` blocks (several pieces editing the shared
  `gen_3d` lines conflicted at merge).

## The done-check (every piece)

A piece is done when all three hold; its report gives the numbers.

1. **Unit tests**: `vcore_tests pNN` passes, no TODO left, covering every case its spec lists below.
2. **Synthetic frames vs the oracle** (`vcore_check pNN` does this and the fuzz run, see above): the piece's own emitted scenes (`VCT_EMIT=<scratch> vcore_tests pNN`,
   each scene exercising one case of the piece in an otherwise plain frame) match the oracle 100% on both screens:
   `vcore_compare.py corpus --corpus <scratch> ...`. Needs the wave-1 pieces (below) merged; until then 1
   only.
3. **Corpus**: on the corpus frames with the piece's features, isolated with
   `vcs_mask.py <ref>/corpus <scratch> --feature F --only LAYERS --plain`: match % >= 99.5 on each screen
   that shows the feature (100% is the aim; name the frames below it and why). The unmasked corpus table is the
   whole core's score, reported by piece 20.

## Dependencies and waves

Every frame goes through 12 (VRAM), 9 (compositing), 10 (output) and 20 (the frame): they are wave 1, needed by the
others' checks 2 and 3 (not by their unit tests). The 3D chain is 13 -> 14 -> 16 -> 18: 3D pixels appear only once
all four exist; 15, 17, 19 add to them. 5 draws the OBJs that 6 and 7 feed. With 20 agents at once: all start
together on unit tests; the main session merges wave 1 (12, 9, 10, 20) and the 3D chain (13, 14, 16, 18) first, and
the others then run checks 2 and 3 on the merged main.

## The pieces

GBATEK files are under <ref>/gbatek/ (the numbers are INDEX.txt's). Features are vcs_features.py's
(`--list` explains each). "Isolate" is the vcs_mask.py `--only` set for check 3.

### 2D

**1. Text BGs** — p01_bg_text.c/.h, tests/t01_bg_text.c.
`vc_bg_text_line(s, e, n, y, ln)`: BG n of engine e as a text BG, line y (vertical mosaic already applied) into
`ln->bg[n]`. GBATEK 04, 05, 11, 12, 18, 25, 27 (and 24 for DISPCNT's char/screen base). Features: bg.text,
bg.text.8bpp, bg.text.extpal. Isolate: bg0,bg1,bg2,bg3. Tests: 4bpp with palette numbers, 8bpp, the four map sizes
(256x256 to 512x512, the 2 KB screen blocks), HOFS/VOFS scroll and their 9-bit wrap, H and V flip, engine A's
DISPCNT char base (bits 24-26) and screen base (27-29), engine B without them, colour 0 transparent, 8bpp with
extended palettes (DISPCNT bit 30; slot n, BG0/BG1 slot 2/3 by BGxCNT bit 13), the tile number's 10 bits.

**2. Affine BGs** — p02_bg_affine.c/.h, tests/t02_bg_affine.c.
`vc_bg_affine_line` (BG2/3 in the affine modes: 8-bit map entries, 8bpp tiles), `vc_bg_affine_pos(s, e, n, x, y,
&tx, &ty)` (the texel position, 24.8; pieces 3 and 4 use it). GBATEK 06, 12, 25, 04. Features: bg.affine. Isolate:
bg2,bg3. Tests: identity, scale 2x and 0.5x, rotation by 90 degrees, the reference point BGxX/Y (28-bit signed
8.8) and PA-PD (8.8), line y's start = X + y*PB, Y + y*PD, the sizes 128-1024, wraparound (BGxCNT bit 13) vs
transparent outside, the char and screen bases.

**3. Extended BGs** — p03_bg_ext.c/.h, tests/t03_bg_ext.c.
`vc_bg_ext_line`: BG2/3 in modes 3-5: BGxCNT bit 7 = 0 an affine BG with 16-bit map entries (text-style entries:
tile, flips, palette number, 8bpp tiles; extended palettes slot 2/3 when DISPCNT bit 30), bit 7 = 1 a bitmap: bit
2 = 0 256-colour, bit 2 = 1 direct colour (BGR555, bit 15 = opaque). Uses `vc_bg_affine_pos`. GBATEK 25, 13, 06,
27, 12. Features: bg.ext.map16, bg.ext.bmp256, bg.ext.direct. Isolate: bg2,bg3. Tests: each of the three kinds, the
bitmap sizes (128x128 to 512x512 by BGxCNT bits 14-15), the bitmap base (screen base x 16 KB), wraparound, a
direct-colour pixel with bit 15 clear is transparent, flips in 16-bit entries.

**4. The large bitmap and BG mosaic** — p04_bg_large_mosaic.c/.h, tests/t04_bg_large_mosaic.c.
`vc_bg_large_line` (mode 6 BG2: 512x1024 or 1024x512 256-colour bitmap, 512 KB of engine A BG VRAM),
`vc_mosaic_bg_y(s, e, n, y)` (the line a mosaic BG fetches), `vc_mosaic_bg_line(s, e, n, px)` (the horizontal mosaic
on one BG's line). GBATEK 25, 13, 08. Features: bg.large, bg.mosaic. Isolate: bg2 (large); for mosaic the BGs of
the frame. Tests: both large sizes, wraparound, MOSAIC's BG H and V sizes (1-16), the block's top-left pixel
repeated, BGxCNT bit 6 off = no mosaic, mosaic on text and affine BGs alike, a transparent first pixel.

**5. Normal OBJs and the OAM loop** — p05_obj.c/.h, tests/t05_obj.c.
`vc_obj_decode` (an OAM entry -> VC_ObjAttr), `vc_obj_line` (all OBJs of a line: checks DISPCNT bit 12; normal OBJs
here, affine ones through `vc_obj_affine_line` (6), mosaic through 7), `vc_obj_tile_texel` (a tile OBJ's texel),
`vc_obj_plot` (the OBJ line's rules; OBJ-window OBJs go to `vc_obj_window_plot` (7)). GBATEK 14, 15, 17, 26, 27,
18. Features: obj.normal, obj.8bpp, obj.extpal, obj.1d, obj.2d, obj.semi. Isolate: obj. Tests: the 12 shape/size
sizes, x 9-bit and y 8-bit wrap (an OBJ at y 250 shows its bottom on line 0), hidden (attr 0 bit 9), 4bpp palettes,
8bpp, extended OBJ palettes (DISPCNT bit 31, slot of 16 palettes x 256), 1D mapping with 32/64/128/256-byte
boundaries (DISPCNT bits 20-21) and 2D (32 tiles a row, 8bpp steps), H/V flip, priority between OBJs (a lower OAM
index wins at equal priority; a better priority wins), the VC_PX_SEMI flag for mode 1, DISPCNT bit 23 (OBJ during
H-blank) ignored.

**6. Affine and double-size OBJs** — p06_obj_affine.c/.h, tests/t06_obj_affine.c.
`vc_obj_affine_uv(s, e, o, x, y, &u, &v)` (the texel an affine OBJ shows at a screen pixel, 0 outside),
`vc_obj_affine_line` (its part of a line through the texel functions of 5 and 7 and `vc_obj_plot`). GBATEK 15, 16,
14. Features: obj.affine, obj.double. Isolate: obj. Tests: the parameter group (attr 1 bits 9-13; PA-PD in the
fourth halfword of four consecutive entries), rotation about the OBJ's centre, double size (the box doubles, the
texture doesn't), texels outside the OBJ transparent, flips ignored, affine bitmap OBJs, the y wrap of a double-size
OBJ.

**7. Bitmap OBJs, the OBJ window, OBJ mosaic** — p07_obj_bitmap.c/.h, tests/t07_obj_bitmap.c.
`vc_obj_bitmap_texel` (mode 3: direct-colour BGR555, alpha 0 = transparent, the OBJ's alpha from attr 2 bits
12-15), `vc_obj_window_plot` (OBJ mode 2 marks `ln->objwin[x]`), `vc_mosaic_obj_y`, `vc_mosaic_obj_line`. GBATEK
26, 15, 07, 08. Features: obj.bitmap, obj.window, obj.mosaic. Isolate: obj (and for the window the layers it masks,
without --plain). Tests: 2D bitmap mapping (DISPCNT bit 6 = 0; bit 5: a 128x512 or 256x256 bitmap area), 1D (bit
6 = 1, the 128/256-byte boundary of DISPCNT bit 22), bit 15 clear = transparent, window OBJs draw no colour, OBJ mosaic V and H sizes, the H mosaic
across neighbouring mosaic OBJs.

**8. Windows** — p08_window.c/.h, tests/t08_window.c.
`vc_window_line(s, e, y, ln)`: `ln->winmask[]` from WIN0/WIN1 (H and V: x1 <= x < x2, the x1 > x2 and y1 > y2
wraps), the OBJ window (`ln->objwin`), WININ/WINOUT; leave the preset 0x3F when DISPCNT bits 13-15 are all clear.
GBATEK 07, 02. Features: win.0, win.1, win.obj. Isolate: the frame's layers without --plain (the windows stay on).
Tests: each window alone, WIN0 above WIN1 above the OBJ window above outside, empty windows (x1 = x2), the wraps, the
effect bit (5).

**9. The compositor and colour effects** — p09_blend.c/.h, tests/t09_blend.c.
`vc_compose_line(s, e, y, ln)` -> `ln->out[]` (RGB6): per pixel the top two visible layers (BGs by BGxCNT priority
then number, the OBJ pixel in front of a BG of equal priority, the backdrop = BG palette colour 0; only layers the
window mask allows), then BLDCNT: alpha blending (EVA, EVB from BLDALPHA, the sum clamped), brighten / darken (EVY),
the window's effect bit, semi-transparent OBJs (blend with the 2nd target whatever the mode), bitmap OBJs' alpha,
and the 3D layer (`ln->l3d` when `ln->bg3d`: its own alpha against the 2nd target). GBATEK 09, 35, 55, 29, 04.
Features: blend.alpha, blend.bright, blend.dark, obj.semi, 3d; every frame uses the layer order. Isolate: none
(the frame's layers, effects on). Tests: priority ties, the backdrop as 1st and 2nd target, EVA/EVB above 16
clamped, EVY 0 and 16, a semi-transparent OBJ over a non-target, 3D alpha 0-31, the colour widening (5 -> 6 bits) and
rounding as the hardware does it.

**10. Master brightness and the display modes** — p10_display.c/.h, tests/t10_display.c.
`vc_display_line(s, e, y, ln, dst)`: POWCNT1's engine enables (bits 1 and 9), forced blank (DISPCNT bit 7: white),
the display mode (DISPCNT bits 16-17: 0 white, 1 `ln->out`, 2 the VRAM bank of bits 18-19 as BGR555 via
`vc_vram_lcdc` (12), 3 main memory: no data, zero), then MASTER_BRIGHT (mode bits 14-15 up / down, factor 0-16).
Engine B has modes 0-1 only. GBATEK 24, 28, 30, 02. Features: bright.master, disp.off, disp.vram, disp.mmem,
disp.blank; every frame. Isolate: as the frame, without --plain. Tests: each mode, brightness up/down at 0, 8, 16
and the factor clamp (> 16), the formula's rounding, an engine powered off, a disabled bank in mode 2.

**11. Display capture** — p11_capture.c/.h, tests/t11_capture.c.
`vc_capture_line(s, y, ln)` (engine A, when DISPCAPCNT bit 31 is set: source A = `ln->out` or the 3D layer
`s->f3d.out` (bit 24), source B = the VRAM bank DISPCNT bits 18-19 select, from the read offset (bits 26-27), or
main memory (bit 25: no data, zero), the mode (bits 29-30: A, B, or the EVA/EVB blend of bits 0-4 / 8-12), the size
(bits 20-21), the destination bank (bits 16-17, in LCDC mode) and write offset (bits 18-19) wrapping in the bank), `vc_capture_end` (clears bit 31 in io9). GBATEK 28, 29, 22. Features: capture (and the
next frame's disp.vram or bitmap BG that shows it). Check 2/3 compare the banks after the frame: `vcore_render
--dump-vram V.bin` against `oracle --dump-vram O.bin` (same 0xA4000-byte layout; `--bank X` draws one to look at);
a frame shows a capture only a frame later. Tests:
each source and mode, the four sizes, write-offset wrap at 128 KB, the blend formula, the alpha bit written (bit 15
set), source B reading while the same bank is the destination.

**12. VRAM bank mapping** — p12_vram.c/.h, tests/t12_vram.c.
`vc_vram_map(s)`: VRAMCNT_A-I (io9 0x240-0x249: MST, offset, enable) -> every `s->rgn[]` (engine A BG/OBJ, B BG/OBJ,
LCDC, A/B BG extended palette slots 0-3, A/B OBJ extended palette, texture slots 0-3, texture palette slots 0-5),
overlapping banks OR-ed (VC_Page holds up to 4 sources), the mirrors of each region, disabled banks unmapped;
`vc_vram_lcdc(s, bank)` (a bank's memory when it is in LCDC mode, for 10 and 11). GBATEK 22, 21, 27, 10. Features:
every frame; vram.extpal, vram.overlap, vram.tex. Isolate: the whole frame (every layer reads VRAM through it).
Tests: every MST/offset combination of GBATEK 22's table for each bank, the BG offsets of F/G (0, 16K, 64K, 80K),
H/I on engine B (and their mirrors), E/F/G/H/I as extended palettes, A-D as texture slots and E/F/G as texture
palette slots, two banks on one page OR-ed, the region wrap, WRAMCNT (0x247) ignored.

### 3D

**13. The matrix engine and the command processor** — p13_matrix.c/.h, tests/t13_matrix.c.
`vc_mtx_reset`, `vc_cmd_mtx` (MTX_MODE, PUSH, POP, STORE, RESTORE, IDENTITY, LOAD_4x4/4x3, MULT_4x4/4x3/3x3,
SCALE, TRANS), the accessors `vc_mtx_clip/pos/vec/tex/proj`, `vc_mtx_apply` (the skeleton's helper: check it),
and `vc_gx_command` (the skeleton's working dispatcher). GBATEK 37, 38, 39, 40, 41, 42, 36. Features: 3d,
3d.mtxstack. Isolate: 3d. Check 2/3 by `--geo` dumps (clip coordinates) once 14 exists, pixels once 16 and 18 do.
Tests: each command in each mode, mode 2 (position and vector together; SCALE leaves the vector matrix), the
projection stack (1 entry) and texture stack, the position/vector stack (31 entries, POP's signed 6-bit offset,
the pointer's range and the error flag), STORE/RESTORE indices, the clip matrix = position x projection after every
change, 64-bit products >> 12.

**14. Vertices, polygon assembly, clipping, viewport** — p14_polygon.c/.h, tests/t14_polygon.c.
`vc_poly_reset`, `vc_cmd_vtx` (VTX_16, VTX_10, VTX_XY, VTX_XZ, VTX_YZ, VTX_DIFF: each vertex through the clip
matrix, its colour and texcoords from `vc_vertex_attrs` (15)), `vc_cmd_poly` (POLYGON_ATTR latched at BEGIN_VTXS,
TEXIMAGE_PARAM, PLTT_BASE, BEGIN_VTXS, END_VTXS, VIEWPORT, BOX_TEST/POS_TEST: no picture). Fills `s->geo`:
triangles, quads, both strips (the strip's alternating winding), culling by facing (POLYGON_ATTR bits 6-7, `front`),
clipping against the six planes with attributes interpolated (`clipped` vertices; far-plane polygons dropped unless
attr bit 12), 1-dot polygons kept (bit 13), the viewport transform to `sx, sy` and the z-buffer `sz`, `ymin/ymax`,
the 6144-vertex / 2048-polygon limits (`overflow`). GBATEK 44, 33, 36, 43, 53, 35. Features: 3d, 3d.strip, 3d.quad,
3d.cull. Isolate: 3d. Tests: each VTX format's fixed point, VTX_DIFF's signed 10-bit /8, a quad strip's vertex
order, a back-facing triangle culled / kept, a triangle crossing one plane and a corner (vertex count and
interpolated colour), the viewport with x1 > 0, the depth formula, the polygon limit.

**15. Lighting, materials, normals, texture coordinates** — p15_light.c/.h, tests/t15_light.c.
`vc_light_reset`, `vc_cmd_light` (COLOR, NORMAL, TEXCOORD, DIF_AMB with bit 15 = set the vertex colour, SPE_EMI
with bit 15 = use the shininess table, LIGHT_VECTOR through the vector matrix, LIGHT_COLOR, SHININESS, VEC_TEST),
`vc_vertex_attrs(s, pos, vtx)` (the colour in force and the texture coordinates: texgen modes 0-3 by TEXIMAGE_PARAM
bits 30-31 with the texture matrix). GBATEK 45, 49, 36. Features: 3d.light, 3d.texgen. Isolate: 3d. Check 2/3 by
`--geo` (vertex rgb and st) and pixels. Tests: the light formula for one and four lights against hand-worked
values, the half-angle specular and the shininess table, the clamps to 31, a NORMAL with no light enabled, COLOR
after lighting, texgen 1 (TEXCOORD x matrix), 2 (normal), 3 (vertex), the 12.4 results.

**16. The rasterizer** — p16_raster.c/.h, tests/t16_raster.c.
`vc_raster_poly(s, p)`: scan-converts a polygon (up to 10 vertices, convex) with the DS's fill rules, interpolates
colour and texture coordinates perspective-correctly with w, the depth (z, or w when SWAP_BUFFERS bit 1), wireframe
(alpha 0: edges only), 1-dot polygons and DISP_1DOT_DEPTH, and for each pixel the texture (`vc_tex_sample`,
`vc_tex_blend` (17)) and `s->plot(s, p, x, y, depth, rgb6a5, edge)`; `vc_3d_depth_test` (less, or equal within the
tolerance with POLYGON_ATTR bit 14), the edge flags and the coverage for antialiasing (`s->f3d.coverage`). GBATEK 33,
35, 44, 51, 52, 54. Features: 3d (every 3D frame), 3d.wbuffer. Isolate: 3d. Tests through a recording `s->plot`
(tests may set `s->plot`): a flat triangle's exact pixel set (left/top edges in, right/bottom out as the DS
does), a thin sliver, a quad, perspective-correct texture coordinates at a known w, z vs w depth values, the equal
test, a 1-dot polygon culled behind DISP_1DOT_DEPTH, wireframe pixels.

**17. Textures** — p17_texture.c/.h, tests/t17_texture.c.
`vc_tex_sample(s, p, s, t)` (TEXIMAGE_PARAM: the address (x 8 bytes in the texture slots), sizes 8-1024, repeat /
flip / clamp per axis, the 7 formats: A3I5, 4-colour, 16-colour, 256-colour, 4x4 compressed (the index data in slot
1 at slot 0/2's offset / 2, its 4 modes), A5I3, direct; colour 0 transparent by bit 29; PLTT_BASE in 16-byte units, 8
for 4-colour, from the texture-palette slots), `vc_tex_blend` (modulation, decal; shadow mode passes; toon /
highlight via `vc_toon_blend` (19); DISP3DCNT bit 0 off = no texture). GBATEK 47, 48, 49, 50. Features: 3d.tex,
3d.tex.a3i5 / pal4 / pal16 / pal256 / 4x4 / a5i3 / direct, 3d.decal. Isolate: 3d. Tests: one texel of each format
from hand-made VRAM (`vct_map_flat` on VC_RGN_TEX / VC_RGN_TEXPAL), each 4x4 mode, repeat, flip at the edge, clamp,
the modulation formula ((a+1)*(b+1)-1)/64 style rounding as GBATEK gives it, decal at alpha 0 / 15 / 31.

**18. Translucency, sorting, polygon IDs, shadows** — p18_translucent.c/.h, tests/t18_translucent.c.
`vc_cmd_swap(s, param)` (SWAP_BUFFERS: `swap_param`, `swapped`; which polygons are translucent (alpha 1-30, or an
A3I5/A5I3 texture); `s->geo.order`: opaque first sorted by Y, then translucent by Y or as sent when bit 0),
`vc_3d_plot` (the default `s->plot`: depth test through `vc_3d_depth_test` (16), the alpha test (DISP3DCNT bit 2,
ALPHA_TEST_REF), opaque writes (colour, depth, opaque_id, fog flag), translucent: alpha blending (DISP3DCNT bit 3),
the same-polygon-ID rule (`trans_id`), the depth update (POLYGON_ATTR bit 11), shadow polygons: ID 0 the mask
(stencil where the depth test fails), others drawn where the mask is set and the ID differs). GBATEK 43, 46, 51,
33, 36. Features: 3d.translucent, 3d.shadow, 3d.alphatest, 3d.blend. Isolate: 3d. Tests: the sort order (equal Y,
manual sort), a translucent over an opaque, two translucent with the same ID, the depth-update flag, alpha 0 vs the
alpha test, the blending formula, a shadow volume's mask and its shadow.

**19. Clear image, edge marking, fog, toon / highlight, antialiasing** — p19_post.c/.h, tests/t19_post.c.
`vc_3d_clear` (CLEAR_COLOR: colour, alpha, polygon ID, fog flag; CLEAR_DEPTH; or the rear-plane bitmap: DISP3DCNT
bit 14, colour from texture slot 2 and depth/fog from slot 3, scrolled by CLRIMAGE_OFFSET), `vc_toon_blend` (POLYGON
_ATTR mode 2: toon (DISP3DCNT bit 1 = 0) or highlight (= 1) from TOON_TABLE by the vertex colour's red), `vc_3d_post`
(edge marking: DISP3DCNT bit 5, EDGE_COLOR by the opaque polygon ID where the neighbour's ID differs and is behind;
fog: bit 7, FOG_COLOR, FOG_OFFSET, the shift (bits 8-11), the 32-entry table interpolated, alpha-only fog (bit 6);
antialiasing: bit 4, the edge pixels' coverage) into `s->f3d.out`. GBATEK 51, 54, 35, 50. Features: 3d.edge, 3d.fog,
3d.aa, 3d.clearimage, 3d.toon. Isolate: 3d. Tests: the clear colour's fields, the rear-plane bitmap's offset wrap,
toon and highlight values, edge marking between two IDs and against the clear ID, fog density at a depth between
two table entries, the fog shift, alpha-only fog, antialiasing coverage on a diagonal edge.

**20. Integration** — p20_frame.c/.h, tests/t20_frame.c (and tools/vcore_render.c options).
The frame driver (`vc_render_frame`, `vc_3d_frame`, `vc_render_line`, `vc_bg_line`: written by the skeleton,
owned by 20 from now on), `vc_3d_layer_line` (the 3D layer's line into `ln->l3d`: BG0HOFS scrolls it horizontally,
pixels with alpha 0 transparent), the screens (POWCNT1 bit 15), and the whole-core score: the unmasked corpus table
(`vcore_compare.py corpus ... --libntr`) per feature, triaged to the pieces. GBATEK 55, 35, 29, 24, 30, 25.
Features: every frame; disp.swap, 3d. Tests: the BG mode table (each mode's BG kinds, engine B without 3D / large),
3D layer scroll and wrap, a 3D pixel with alpha 0, the screen swap, an end-to-end frame (a text BG + OBJ + one
triangle) once the others exist. Done-check 3: the unmasked corpus, per area (title, fields, battle, menus), with
the mismatches assigned to pieces.

## Feature vocabulary

`python3 port/vcore/tools/vcs_features.py --list`: bg.text, bg.text.8bpp, bg.text.extpal, bg.affine, bg.ext.map16,
bg.ext.bmp256, bg.ext.direct, bg.large, bg.mosaic, obj.normal, obj.8bpp, obj.extpal, obj.1d, obj.2d, obj.affine,
obj.double, obj.bitmap, obj.window, obj.mosaic, obj.semi, win.0, win.1, win.obj, blend.alpha, blend.bright,
blend.dark, bright.master, disp.off, disp.vram, disp.mmem, disp.blank, disp.swap, capture, vram.extpal,
vram.overlap, vram.tex, 3d, 3d.mtxstack, 3d.strip, 3d.quad, 3d.cull, 3d.light, 3d.texgen, 3d.tex, 3d.tex.<format>,
3d.translucent, 3d.shadow, 3d.decal, 3d.toon, 3d.alphatest, 3d.blend, 3d.edge, 3d.fog, 3d.aa, 3d.clearimage,
3d.wbuffer. They are read from the frame's own state, so they hold for any corpus; `--index DIR/index.txt` writes
them in the index format tools/vcore_compare.py reads.

## Open items (groundwork)

- The oracle option pieces 13-15 need: `oracle --geo OUT.txt` (melonDS's polygon RAM in the geometry-dump format
  above). The oracle's `--3d` and `--dump-vram` already match vcore_render's.
- The corpus index's features: vcs_features.py's vocabulary, so the compare tool's per-feature table matches the
  piece specs (a hand index may add area names: title, field, battle).
- port/vcore is not linked into the port yet: replacing libntr's simulator is a later step, after the corpus scores.
