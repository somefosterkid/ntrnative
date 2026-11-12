# Lockstep: the PC port against melonDS, frame by frame

The maintainer's plan for boot driving: run the same ROM, save, clock and key script on our PC port and on melonDS, dump
the same game variables and screens every game frame on both, and let a compare tool name the first frame where they
diverge. Then one agent per divergence fixes it. Three pieces, one contract (this file):

| piece | where | what |
|---|---|---|
| melonDS side | <ref>/lockstep/lstep_melon (outside the repo: it links melonDS, GPL) | `lstep_melon ROM SAVE INPUTS OUT.jsonl [--png DIR] [--frames A-B]` |
| port side | port/src (BWPORT_LOCKSTEP) | the exe dumps the same OUT.jsonl from the same inputs |
| compare | tools/lockstep/ | the first divergent frame and var, the screens side by side |

Status: this contract, tools/lockstep/vars.txt (the variables, with their DS paths and the port's
expressions) and tools/lockstep/ds_offsets.py (DS struct offsets from mwccarm) are done; the three programs are not
written yet. Facts marked CHECK are design, not yet seen running in melonDS.

## The run

1. Power on with the save (below) and the pinned clock, the RNG pinned (below).
2. The boot keys, by game frame from power-on: `start@f600 start@f900 start@f1300 a@f1700` (the start menu's CONTINUE,
   as runs.sh presses them on the port; CHECK on melonDS: its loads take more frames, so these may need to be later
   there, or the boot driven by the state: press only while `field` is null).
3. The anchor (below): frame 0 of the comparison. The RNG is reseeded there.
4. The INPUTS script, its frames counted from the anchor. Each side dumps a record per game frame from the anchor on
   (or `--frames A-B` of them), until the script's last key plus 300 frames or `--frames`' end.

## Game frames

A game frame is one iteration of main()'s loop (src/system/main.c): GFLAppUpdate (GCTX_HIDUpdate reads the keys),
GameUpdate (the logic), the 3D swap, OS_WaitIrq(TRUE, OS_IE_V_BLANK), GameUpdateVBlank (GFL_VBlankUpdate: the V-blank
tasks, the BG transfers, and `data_020aa1a0++`). `data_020aa1a0` (gfl_use.c, a static at DS 0x020aa1a0, zeroed by
GFLAppInit) is the game frame counter `g`; during iteration g and at its end it holds g. On the DS an iteration that
runs past the V-blank (a lag frame) makes the next one start a V-blank late; the counter counts iterations, not
V-blanks, so both sides agree on it whatever their speed.

The END of game frame g, where a record's vars are read: the main thread asleep in that OS_WaitIrq, before the V-blank
handler runs.
- melonDS: run each frame in two parts, stopping when VCount becomes 192 (the scheduler's StartScanline(192) event
  has run, the V-blank IRQ is raised, the ARM9 hasn't served it): copy NDS::RunFrame's inner loop (NDS.cpp; members
  public through `#define private public`, as melondump.cpp does) and check `GPU.VCount == 192` after each
  RunSystem. The main thread is in the wait when OSi_LauncherThread's `queue` (OSThread +0x78) is OSi_IrqThreadQueue
  (DTCM 0x02fe00a0). OSi_LauncherThread has no name in symbols.txt: it is the OSThread with id (+0x6c) 0 in
  OSi_ThreadInfo's list (0x02150fec; OSThreadInfo +0x8 = the list, linked through OSThread +0x68). If the main
  thread isn't waiting at VCount 192, the frame lagged: no record; it ends at a later V-blank (the counter is still g).
- port: in OS_WaitIrq (libntr) on the game's main thread, for OS_IE_V_BLANK, before the V-blank callback runs.

The keys of game frame g are the ones GCTX_HIDUpdate sees in iteration g. melonDS: at each stop, SetKeyMask for frame
counter+1 (the next HID read is the next iteration's, lagged or not). Port: set the key bits for g+1 on the way out of
the same OS_WaitIrq, before GameUpdateVBlank (the counter is still g there). The X and Y buttons reach the ARM9
through the ARM7 (HW_BUTTON_XY_BUF), a little later: hold every key 2 frames at least. Measured: melonDS
delivers X/Y one game frame after the other keys, so the port's dump (lstep_dump.c ApplyKeys) sets the XY word from
the keys of the frame before; the field then starts the menu on its own half-cycle (1 or 2 frames after the press).

## The anchor

Frame 0 is the first end of a game frame, after the boot keys, where
`field != null && field.routine == 3 && field.loaded == 1 && event == null && field.phase == 0` (vars.txt): the field
map runs its update routine (fieldmap.c's Field_CallRoutines, data_ov021_021d36b8[3]), its map is loaded
(Field_CheckMapLoadFinished), no event runs (the player has control) and the field's two-frame cycle is at its first
half (routineAlternator: the field updates at 30 fps in two halves; the phase must match). Record f = g - g(anchor).
CHECK: that these hold together after CONTINUE on both sides, and that the player answers keys from there.

## Inputs

The INPUTS file is a BWPORT_KEYS script (port/src/keys_script.c): tokens `KEY[*HOLD]@fN`, space- or comma-separated,
`#` to the end of a line a comment. KEY: a b select start right left up down r l x y. N: the game frame after the
anchor when the key goes down (seconds are not allowed here); HOLD: `fM`, M game frames (default 4; 2 at least). A
key is down for the HID reads of frames N .. N+M-1. Touch: none (not in BWPORT_KEYS yet).

walk.py's moves (`up2 right5`, <scratch>) can be turned into such a script: one grid step is a hold of
16 frames when walking.

## The save

<saves>/NAME.bin: the game's raw 512 KB flash image (<saves>/README lists them). melonDS: the cartridge's SRAM
(NDSCart::ParseROM with NDSCartArgs{SRAM, SRAMLength = 0x80000}; melonDS's ROM list gives Black 512 KB flash); the port:
save.bin (`port_run.py --save NAME`). Neither side writes the file back: each run starts from the same bytes.

## The clock

The RTC is frozen at 2026-10-29 12:00:00, a Thursday (day of week 4, 0 = Sunday), local time, no time zone (day in
the field; October is summer in Black's seasons). It never advances: the game reads the same date and time every
time. melonDS: RTC::SetDateTime(2026, 10, 29, 12, 0, 0) after Reset and again at every stop, with the RTC's
ClockCount set back so that no second ticks between stops. Port: libntr's RTC reads return it (the port side's
patch; BWPORT_LOCKSTEP_CLOCK may override it, both sides the same).

## The random number generators

The game seeds from entropy that no clock pins: buildSeed (lib/gfl/src/gf_standard.c) hashes OS_GetLowEntropyData and
an uninitialised stack buffer. It runs at boot (func_020056d0, gfl_use.c:224, then the Mersenne Twister is seeded
from the LC: GFL_RandomUpdateMT(GFL_RandomLC(0))) and at each battle's start (btl_main.c:209, the battle's own LC).
And the anti-piracy V-blank task (func_02011d34) draws the MT twice a frame, so its state at the anchor depends on
how many frames the boot took. So:
- buildSeed always gives `x = 0xFEDCBA9876543210`, `mul = 0x5D588B656C078965`, `add = 0x269EC3` (MATH_InitRand32's).
  melonDS: overwrite buildSeed (DS 0x0203f258, ARM, 0xa4 bytes) in main RAM, once the ARM9 binary is in place and
  before the anchor, then ICacheInvalidateAll: the words E59F101C E59F201C E59F301C E59FC01C E8A0100E E59F1018
  E3A02000 E8800006 E12FFF1E 76543210 FEDCBA98 6C078965 5D588B65 00269EC3 (ldr r1-r3, r12 from the pool;
  stmia r0!, {r1-r3, r12}; ldr r1; mov r2, #0; stmia r0, {r1, r2}; bx lr; the pool: x, mul, add). Port:
  `-Wl,--wrap=buildSeed` (its callers are in other files), __wrap_buildSeed sets the same three.
- At the anchor, before its record is read: `data_020aa1a4->rand.x = 0x0123456789ABCDEF` (mul and add stay) and the
  MT reseeded as GFL_RandomUpdateMT(0x01234567) does: mt[0] = seed, mt[i] = 1812433253 * (mt[i-1] ^ (mt[i-1] >> 30))
  + i for i < 624, mti = 624 (DS: the GFLStdTables at *data_021469f8: mt at +0, mti at +0x9c0; the port calls
  GFL_RandomUpdateMT).
Frame 0's record shows these values on both sides; from there on the generators are the game's own.

## The dump: OUT.jsonl

One JSON object per line. The first is the header:

    {"lockstep": 1, "side": "melonds"|"port", "rom": "...", "save": "town.bin", "inputs": "...",
     "clock": "2026-10-29T12:00:00", "anchor_g": 2411, "anchor_vblank": 2533, "vars": "tools/lockstep/vars.txt"}

then one record per game frame from the anchor:

    {"f": 0, "g": 2411, "vb": 2533, "lag": 0, "keys": "up",
     "top": "1a2b3c4d", "bot": "5e6f7a8b", "gtop": "...", "gbot": "...",
     "vars": {"game.frame": 2411, "rng.lc": "0x0123456789abcdef", "player.x": 776, "event.cb": null, ...}}

- f: game frames since the anchor; g: the game frame counter; vb: the side's V-blank count (OS_GetVBlankCount on the
  port, frames run on melonDS), for information; lag: V-blanks this game frame took beyond one (melonDS; 0 on the
  port); keys: the keys the HID read of this frame saw, names joined by `+` ("" for none).
- top, bot: crc32 (zlib's) of each screen as 256x192 15-bit colours, row by row, each pixel a little-endian u16
  `r5 | g5 << 5 | b5 << 10` (r5 = the 8-bit value >> 3; melonDS's 6-bit value >> 1). Equal hashes = the same
  picture at the DS's own colour depth.
- gtop, gbot: an 8x6 grid of 32x32-pixel cells, row by row, each cell the mean r5, g5, b5 rounded down, as 2 hex
  digits each (288 characters a screen): a coarse picture to compare with a tolerance where the hashes differ (the
  port's GL 3D will rarely match bit for bit).
- The screens of record f are the picture shown in the frame after the V-blank that ends game frame f (on the DS:
  the lines drawn after that V-blank, with the 2D state GameUpdateVBlank transferred in it and the 3D list swapped at
  it; melonDS: the framebuffer after the following RunFrame). The port side picks its libntr picture that matches
  (CHECK on a walk: the screens must not be a frame off).
- vars: every var of tools/lockstep/vars.txt, by name, in the file's order: numbers, "0x..." for u64, symbol names
  for fn, "crc32" hex for hash, null when unreadable or its `if` fails.
- --png DIR: DIR/f000123.png, 256x384 RGB (top screen above the bottom one), for the frames of --frames (or all).

## The variables: tools/lockstep/vars.txt

One per line: NAME, TYPE, the DS path, the port's C expression, an optional `if`. The file's header explains the
types and the path syntax (`g_GameSystem->0x20->0x8c` = load the GameSystem pointer, add 0x20, load the field
pointer, add 0x8c). The DS offsets come from tools/lockstep/ds_offsets.py (mwccarm on the decomp's headers: the DS
layout, not the PC build's). The port's expressions name statics (data_020aa1a0, data_020aa1a4, data_021469f8) and
types local to a .c file (EventScriptCallData, script_sys.c: {ScriptWork *work; FieldScriptSupervisor *supervisor})
that port code can't name: read those through the exe's ELF symbol table (`nm build/port/linux/bwport` lists them as
local symbols) with the layout spelled out, or evaluate the expressions in gdb. Adding a var: its path on both sides,
the DS offsets from ds_offsets.py; hash only pointer-free data (a host pointer is 8 bytes and another value).

Covered: the frame counter, both RNGs, the running event and process, the field's routine, the player's actor (grid
and world position, facing, zone), the saved player state, the running field script (ID, VMs, VM 0's PC as an
offset in its script), the event flags and work values, the party, the battle's RNG and main function (UNVERIFIED:
that the battle process is g_GameSystem's; check proc.fn in a battle).

## Expected differences (not bugs)

- Loads: the DS reads the card asynchronously across frames (map blocks, models, the script data); the PC loads at
  once. Frames that wait on a load are lag on the DS or extra iterations of a polling loop: a divergence that starts
  at a load and closes again a few frames later is timing, unless the logic then differs.
- The screens: libntr's GL renderer against melonDS's software one (port/vcore is the exact renderer to come); compare
  the grids with a tolerance and look at the PNGs.
- Sound (not dumped).

## Building lstep_melon (the next agent)

melonDS: <ref>/melonDS (commit 1e654443e4c0, unmodified; build its libs with <ref>/oracle/build.sh). melondump.cpp
(<ref>/oracle) shows the boot: NDSArgs with the defaults (FreeBIOS, generated firmware), JIT off, ParseROM,
SetNDSCart, Reset, SetupDirectBoot("rom.nds"), Start. Read DS memory without side effects: main RAM
`MainRAM[addr & MainRAMMask]`, DTCM `ARM9.DTCM[(addr - ARM9.DTCMBase) & ARM9.DTCMMask]` (melonDS emulates no data
cache, so RAM is what the CPU wrote). Keys: SetKeyMask (active low: bit set = released; bits 0-9 KEYINPUT, 10 X, 11
Y: check NDS::SetKeyMask). The ROM: <repo>/orig/baserom_b_us.nds (read only). Validate: town.bin, a walk
(`left*f16@f10 left*f16@f30 up*f16@f50 ...`): player.x/z follow the walk, the PNGs look right.

## (lf59): the first sweep

The compare ignores `game.frame` by default (`--compare-frame` compares it), treats a DS overlay alias ("a|b") as
equal to a port name among them, and has `--skip-lag` (ignore var differences on frames where a side lagged).
The runner passes its own checkout's vars.txt and symbols to lstep_melon; with FRAMES=A-B it compares only A-B.
`tools/lockstep/lstep_sweep.sh [NAME...]` runs the eight save/input pairs of tools/lockstep/inputs/. The divergences
it found: the maintainers' notes. Gotchas: vars.txt columns need two spaces (one space = the path swallows
the expression and the var reads null on melonDS); `state.*` is GameData's own player, playerStates[4]. The battle
vars work: proc.fn is data_ov093_021efdac in a wild battle on both sides. lstep_melon's source is in
<ref>/lockstep (outside the repo): build it with `cd <ref>/lockstep && sh build.sh` after copying
lstep_melon.cpp where build.sh expects it (<scratch>).

## Using it

Compare tool: `tools/lockstep/lstep_compare.py MELON.jsonl PORT.jsonl [options]`; runner: `tools/lockstep/lstep_run.sh
SAVE INPUTS [OUTDIR] [-- compare options]` (runs lstep_melon and the port exe on the same save and inputs, then the
compare; `FRAMES=A-B`, `PNG=1`, `LSTEP_MELON=`, `LSTEP_PORT_CMD=` override; it exits 3 when a producer is missing or
failed). Tests: `python3 tools/lockstep/test_compare.py`.

The compare's exit status: 0 the dumps agree, 1 they diverge, 2 unreadable input. Its last line is always one summary
line (`-q` prints only that):

    SAME frames=412 renderer-only-frames=380
    DIVERGED f=17 g=117 kind=vars groups=position,rng vars=rng.lc,player.x screens=same
    DIVERGED f=12 g=112 kind=screens-only top=renderer(bad=0,worst=2) bot=DIFF(bad=1,worst=237)
    DIVERGED f=8 kind=missing-frame absent-in=port
    DIVERGED f=35 kind=length shorter=port melonds=40 port=35

Above it, the report: the frame (f, g, vb, lag, keys of both sides); the differing vars grouped (position/zone, script
VM, RNG, flags/work, battle, party), each `melonds -> port`; per screen whether the hash differs and, if so, how many of
the 48 grid cells are beyond the tolerance (`--tol`, 5-bit units, default 3) with a 8x6 map of them: "renderer only"
(hash differs, grid within tolerance) is told apart from "PICTURE DIFFERS"; and the last N frames (`-n`, default 5)
before it, with the differing vars' values on both sides. A frame is divergent when a var differs or a screen's grid
differs; a frame in one dump only, or one dump ending early, is a divergence too (`--allow-missing`, `--allow-short`
for lag frames and cut runs). Options: `--after F` / `--until F` (compare frames f > F / <= F, to look past a known
divergence), `--ignore-var PAT` (fnmatch, comma lists, repeatable), `--ignore-group G`, `--ignore-screens`,
`--screens-only`, `--min-cells K`, `--json`.

Taking one divergence (an agent's routine):

1. Run `bash tools/lockstep/lstep_run.sh SAVE INPUTS` with the save and input script you were given (the brief names
   them; the dumps stay in build/lockstep/SAVE-INPUTS/). Keep the same pair for every re-run: the whole run is
   deterministic, so the divergence reproduces at the same f. Re-run the compare alone with `lstep_compare.py
   build/lockstep/.../melon.jsonl .../port.jsonl`; `--after F` moves on to the next divergence once yours is fixed.
2. Read the summary and the report. The first divergent frame f is where to look, g = anchor_g + f its game frame; the
   keys it saw are in the report, and the input script's `KEY@fN` lines (frames from the anchor) say which press led
   there. Check the "last N frames" first: a var that was equal for N frames and then differs is the cause, one that
   jumped earlier than f in the other side's column is a timing difference.
3. Decide what kind it is. `kind=screens-only` or `screens=renderer-only`: not game state, usually the renderer; look at
   both sides' PNGs (`PNG=1 FRAMES=f-5-f+5`) before touching anything. A `groups=position` difference after a
   `script` or `event` one is a consequence: fix the earliest group (order in the report: script and event first, then
   position). `rng` diverging with everything else equal means somebody drew the generator a different number of
   times: find which code ran in one side only (the event callback, the proc function and the script ID are in the
   vars). A divergence that opens at a load and closes a few frames later is timing (see "Expected differences").
4. Find the code: the var's DS path and port expression are in vars.txt (the port's names are the decomp's); the frame's
   lines in the port's log, `port_run.py --gdb` with a breakpoint at game frame g (data_020aa1a0), and the decomp source
   of the function that writes the var (grep its field name in src/). To see the DS side's behaviour, run melonDS with
   the same inputs and `--frames f-30-f` and read the vars and PNGs, or use <ref>/oracle's dumps.
5. Fix it in the port (port/src, a libntr patch; decomp C only under the byte-identical rules of the maintainers' notes),
   re-run, and confirm the first divergence moved later (or the summary says SAME). Put the save, inputs and the f
   before and after in the commit message.

##: four divergences, what they taught

Four agents took lf59's divergences 1-4 (the maintainers' notes has each cause). One was a port bug, three were how the two sides look at the game:

- **Sound waits are real time.** The battle intro ran 24 frames ahead because libntr ignored wave-out (cries): every cry wait lasted one frame (libntr-wavout.patch). Cries and sound effects run on the audio callback's clock, so the port side ran at 60 fps for a round.'s frame-driven sound clock (libntr-soundclock.patch) made the dumps byte-identical at any speed, so the port side is uncapped again (`LSTEP_FAST=0` for 60 fps), and lstep_sweep.sh runs the saves in parallel (a run dir each, `LSTEP_JOBS`, default 8): the 8-save sweep takes about 70 s instead of about 10 minutes. A sound clock driven by game frames would make these waits deterministic (open).
- **The sample point.** lstep_melon records at the first V-blank wait of each game frame, also one inside an event callback (the door transition waits twice in GameUpdate); the port dump does the same now (it wraps OS_WaitVBlankIntr). Sign: `vb` jumps and the next record has `lag` 2.
- **Beam race.** A one-frame difference in the top screen's bottom rows with all vars equal is the game's logic writing VRAM while the DS draws (melonDS shows the new picture in the rows below the beam); the port draws a frame at once. Not a bug; check by checksumming VRAM in chunks around GFL_VBlankUpdate. `lstep_compare.py --ignore-beam-race` (on in lstep_sweep.sh) skips it, and its whole-screen variant: a one-frame difference with equal vars where the next frame agrees again and melonDS's frame f holds the port's f+1 picture (or cells that change between the port's f-1 and f+1), or only the top screen's bottom row differs; a port-only blip is not skipped (unit tests in test_compare.py). The summary line says `beam-race-frames=N`.
- **Fades look like geometry.** The "battle platform" was a fade starting early; sample a few pixels over many frames on both sides before touching the renderer.
- Tools: lstep_run.sh makes INPUTS and OUTDIR absolute (relative ones broke the port side); `FRAMES=A-B` is sane but the compare is limited to A-B; port PNGs are A-B too), still prefer `--after F` on a full run for a later divergence; extra DS-side vars without touching vars.txt: copy it, append lines (DS path + a dummy port expression), pass `lstep_melon --vars`; a gdb conditional breakpoint on the state function printing its changes by game frame finds which wait a frame offset comes from.

## (battle, lb62)

- The route1 / caught battle-intro divergence (rng.mt.index) is only the wild cry's wait length (waitKind 16: 25 game frames on
  melonDS, 7-116 in the port from run to run); with that wait pinned to the DS's 25 frames route1 is SAME for all 686 frames
  (details: the maintainers' notes,). The sound clock is the fix.
- A whole wild battle: tools/lockstep/inputs/battle_long.txt (save route1). New var `battle.calc.rng` = the battle's live
  generator (btl_calc.c BattleRandom: damage, accuracy, crits); `battle.rng` and `rng.lc` stay constant in a battle. Outcome
  check = party hash + battle.calc.rng + battle.result. Without frame-locked sound waits the A presses of battle_long land on
  other menu states in the port, so its outcome differs run to run; with the pin it is the same, 24 frames earlier.

## Frame skip: LSTEP_SKIP=N

`LSTEP_SKIP=N lstep_run.sh ...` / `LSTEP_SKIP=N lstep_sweep.sh` (default 1: every frame, the dump unchanged byte for byte):
the port draws one frame in N. Its dump (BWPORT_LSTEP_SKIP=N, port/src/lstep_dump.c) carries the screens (hash, grid, PNG)
only on frames `f % N == 0`; the other records have `"top": null, "bot": null, "gtop": null, "gbot": null`; the vars are in
every record. It also sets BWPORT_FRAMESKIP=N, so the bridge skips the rest: gather, vcore, the texture (the rasterizer and the 2D
compositing nothing reads back), except the capture frames (DISPCAPCNT: the game reads the capture's bank next).
lstep_compare.py compares screens only on frames where both sides have them (melonDS always has) and says how many:
`SAME frames=701 screens-compared=176`. Without the next frame's screens the beam-race tolerance cannot run, so a one-frame
difference it forgives at N = 1 shows as a screens-only DIVERGED at N > 1: `lstep_sweep.sh` with LSTEP_SKIP > 1 therefore
compares vars only (`--ignore-screens`).

When to use it: a vars-only hunt (a game-logic divergence: an RNG, a script, a battle outcome), where the pictures do not
matter. Screens and PNGs need N = 1. Measured (8 saves x 700 frames, melonDS included): N=1 73 s, 2 59 s, 4 53 s, 8 51 s;
the VARS of all 8 saves identical to N=1, and the screens of the drawn frames identical to N=1's.

## The port side: port/src/lstep_dump.c

Linux build only (the vars read the exe's ELF symbol table). Options, all environment variables of the exe (full list at
the top of the file): `BWPORT_LSTEP=OUT.jsonl[,FRAMES]` (on), `BWPORT_LSTEP_INPUTS=FILE|"TOKENS"`, `BWPORT_LSTEP_BOOT`
(default `start@f600 start@f900 start@f1300 a@f1700`, game frames from power-on, held 4 frames), `BWPORT_LSTEP_PNG=DIR[,A-B]`,
`BWPORT_LSTEP_SAVE=NAME` (the header only; the save itself is `port_run.py --save NAME`). Example:

    BWPORT_LSTEP=<scratch>,300 BWPORT_LSTEP_INPUTS=walk.txt BWPORT_LSTEP_SAVE=town.bin \
      python3 tools/port_run.py -t 120 --fast --save town

The exe exits (status 0) after FRAMES records (3 if no anchor by game frame 20000). How it follows the contract:
- the hooks are link-time wraps (port/meson.build, Linux: `-Wl,--wrap=PORT_WaitIrq,GFL_VBlankUpdate,buildSeed,time`),
  no libntr patch: the end of game frame g is main_threads.c's PORT_WaitIrq called from NitroMain (the game lock is
  held again there, the V-blank handler not run yet); the keys for g+1 are set there too (a whole-frame KEYINPUT and
  X/Y word, active low); `buildSeed` gives the contract's x/mul/add; `time()` returns the frozen clock (and TZ=UTC), which
  libntr's RTC reads; the anchor reseeds `data_020aa1a4->rand.x` and calls GFL_RandomUpdateMT(0x01234567).
- the statics (data_020aa1a0, data_020aa1a4, data_021469f8, the battle proc table) by symbol name; the types local to a
  .c file (GameEvent, GameProc, GameProcManager, EventScriptCallData) as mirrors in lstep_dump.c.
- screens: the port's picture is vcore's (forced `BWPORT_RENDERER=vcore`), exported by port/bridge/vcore_bridge.c
  (`bwport_last_rgba`, `bwport_render_now`). libntr draws inside PORT_WaitIrq, before GFL_VBlankUpdate's transfers, so
  record f's screens come from an extra render right after frame f's GFL_VBlankUpdate (`BWPORT_LSTEP_SHIFT=2`, default:
  the 3D swapped in f and the 2D transferred in it, as the DS shows after that V-blank). On a walk it equals the render
  inside the wait (SHIFT=0); SHIFT=1 (the next wait's) is a frame late. The field's 3D picture changes every second frame.
- `lag` is always 0, `vb` = OS_GetVBlankCount. An `fn` var whose pointer is null is `null`; `script.vm0.pc` is the u32 difference.
- two runs from town.bin with a walk give byte-identical dumps (every hash, grid and var).
