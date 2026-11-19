# How NTR Native works

A map of the code for contributors: the layers from the DS hardware up, how one executable holds seven games, how a
build that holds no game data still plays, how the Generation IV games are built, and how the port is tested. Each
section names the files to start from and links the deeper document. Everything here was checked against the code at
the time of writing; the few things that were not are marked **(not verified)** and listed at the end.

## The two halves

The repository is two projects that share one `src/`:

1. **A matching decompilation** of Pokémon Black and White. `configure.py` writes `build.ninja`, and `ninja` compiles
   `src/` and `lib/` with the original Metrowerks compiler into `build/pokeblack_us.nds` (and White's), each checked
   against the retail ROM's SHA-1. White builds from the same source with a few functions and tables switched
   ([docs/white.md](white.md)). Functions not yet byte-matched are listed in [docs/nonmatching.md](nonmatching.md).
2. **A native port** (`port/`, built with meson): the same C, plus the C of the other games, compiled for the host
   and linked into one program, `bwport`. This document is mostly about this half.

The rule that ties them together: a change to the game's C for the port must leave both ROMs byte-identical, so the
port's fixes go in `#ifdef` blocks, in `port/` files, or in patches (below), never in a way that moves a byte of the
DS build.

## The layers

```
  front-ends    desktop launcher (ImGui)   Android launcher (Java + JNI)   iOS launcher (coming)
                         \                     |                            /
  launcher core              port/src/launcher_core.c  (game table, ROM import, saves, settings)
                                               |
  host runtime     port/src: main_start.c, machine.c (restart), games.c, ovl.c, asset_load.c, stand-ins
                                               |
  game modules   Black   White   Black 2   White 2   Platinum   HeartGold + SoulSilver
                 (src/)  (src/,  (third_party/       (pret's,   (third_party/
                         renamed) pokebw2)           fetched)   pokeheartgold)
                                               |
  NitroSystem      lib/nnsys (decompiled, ours), one copy for every game
                                               |
  DS hardware      libntr (NitroSDK / TwlSDK for PC, patched)  --  renderers: port/vcore, port/gpu
                                                                --  sound core: port/acore
                                               |
  platform         SDL2, OpenGL 4 / OpenGL ES 3, the OS
```

## libntr: the DS hardware layer

[libntr](https://github.com/cybervisi0n/libntr) (MIT) is a NitroSDK and TwlSDK for PC: it implements the SDK's API on
the host and simulates the parts of the DS the SDK talks to (main memory, VRAM, OAM, palettes, registers, the ARM7's
side of the PXI messages, the sound driver), with an SDL2 window and an OpenGL context. Its `main()` sets up the
simulated DS and then starts the game (upstream: its entry, `NitroMain`; in the port, through `SIM_RunGame`, below).
The game's C calls the SDK as it would on a DS.

- Pinned by `port/subprojects/libntr.wrap` (a meson wrap: one commit). Our changes to it are the patches in
  `port/subprojects/packagefiles/libntr-*.patch` (about 150), listed in the wrap's `diff_files`, each with a comment on
  what it changes and why: SDK gaps, VRAM bank mapping, the V-count, the renderer and sound hooks, the restart, Android
  and Windows details.
- The game's frame is its own V-blank wait: inside it libntr draws both screens, runs the game's interrupt handlers,
  counts the V-blank and polls input. Details: [docs/port-runtime.md](port-runtime.md) ("How libntr starts a game").
- Build notes for libntr itself: [docs/libntr/build.md](libntr/build.md).
- Settings live in `sim_config.ini` (libntr's file; `port/src/main_start.c` writes the port's defaults when there is
  none: `Renderer=auto`, `Sound=acore`, ...).

The SDK the DS build uses (`lib/nitro`, `lib/twl`) is not compiled for the port: libntr stands in its place. DSi-only,
online and hardware-driver files are left out by `port/exclude.txt` (read by `tools/port_sources.py`), and
`port/src/stub_*.c` answer for what they did (online play is stubbed).

## Our NitroSystem (lib/nnsys)

NitroSystem is the middleware layer above the SDK (2D cell and character loaders, the 3D model library, sound
archives, heaps). The port uses `lib/nnsys`, our decompiled NitroSystem, compiled for the host:

- `port/meson.build` builds it as the `nnsys` static library with the game's flags and
  `port/include_nnsys/nnsys_lib.h` first; the port's side of it (`port/src/nnsys_*.c`, e.g. the 64-bit 2D loaders in
  `nnsys_load64.c`, the geometry staging in `nnsys_g3stage.c`, the heap checker in `nnsys_heapcheck.c`) is a second
  library. Files with a host stand-in are compiled through `port/extern/lib/nnsys/`.
- One copy serves every game in the executable; the Generation IV modules reach it through the headers in
  `port/include_gen4/`.
- Design, history and the 64-bit spots: [docs/nnsys-port.md](nnsys-port.md); the files that are not yet byte-matched:
  [docs/nnsys-parity.md](nnsys-parity.md).

## Renderers and the sound core

libntr calls a registered frame renderer at each drawn frame; the port registers its own. Each is its own meson
project too (unit tests and tools in its directory) and is linked into `bwport` as a static library behind a bridge
that keeps its types apart from the game's.

| Piece | What | Bridge | Doc |
| --- | --- | --- | --- |
| `port/vcore` | clean-room CPU renderer of the DS's 2D engines and 3D, exact at 1x, upscaling on the CPU; renders the 3D and the two engines on threads | `port/bridge/vcore_bridge.c` gathers the frame's whole video state from libntr (`SIM_VCapGather`) into a `VCoreState` | [vcore.md](vcore.md) |
| `port/gpu` | the DS 3D at N x on the GPU (GLSL shaders embedded at build time by `port/gpu/tools/embed_glsl.py`) | `port/bridge/gpu_bridge.c`, the same gather, then the GL path | [gpu.md](gpu.md), [gpu-compat.md](gpu-compat.md) |
| `port/acore` | clean-room DS sound core: PCM/ADPCM/PSG channels, the sequence player, envelopes, the mixer | `port/bridge/acore_bridge.c` runs it from the audio callback at the DS rate and resamples to the device; `port/src/acore_glue.c` and `port/src/acapture.c` are the game's side (the ARM9's command lists) | [acore.md](acore.md) |

`Renderer=` in `sim_config.ini` (`auto`, `vcore`, `gpu`; `BWPORT_RENDERER` overrides it) picks the renderer;
`Sound=acore` (or `BWPORT_SOUND=acore|libntr`) picks the sound path. In-between frames above 60 fps:
[docs/interp.md](interp.md). Performance work: [docs/perf.md](perf.md).

## Game modules: one executable, seven games

Each game's C is compiled and linked once into the same executable. What the host calls in a game goes through that
game's **table**, a `PortGame` (`port/include/port_game.h`): its ROM game code, its `NitroMain`, its sound heap, its
random number generators (for pinned test runs), its overlay table, the overlay numbers the speed options watch, and a
per-frame hook.

- `port/game/game_table.c` defines every table. It is compiled into each game with that game's defines, so it yields
  `bwgame` (Black), `w_bwgame` (White), `b2_bwgame`, `w2_bwgame`, `pl_bwgame` (Platinum), `hg_bwgame` and `ss_bwgame`
  (HeartGold and SoulSilver: one module, the version is a value its table's `main` sets).
- `port/src/games.c` lists the tables this build has (`BWPORT_WHITE`, `BWPORT_B2`, `BWPORT_W2`, `BWPORT_PLAT`,
  `BWPORT_HG`) and switches between them: `PORT_SetGame(code)` makes one of them `PORT_Game` and hands its overlay
  table to `port/src/ovl.c`. A game the build left out falls back to Black with a message.

### Keeping seven games' symbols apart

The games share thousands of function and variable names. Two ways keep them apart:

- **White on the PC** (GNU ld, lld, mingw): its objects are renamed with a `w_` prefix (`tools/port_rename.py`); the
  files both games share are compiled once.
- **Black 2, White 2, Platinum, HeartGold/SoulSilver** (and White on Apple): each game's objects are linked into one
  relocatable object (`b2.o`, `w2.o`, `pl.o`, `hg.o`) whose only global symbols are its tables; every other global
  becomes local, so nothing clashes and the game's references bind to its own definitions or to what all games share
  (libntr, our NitroSystem, the host). `tools/port_relink.py` does this with the linker's `-r` and objcopy (on Apple,
  ld64's `-exported_symbol`).

### Overlays

On the DS an overlay is loaded into memory it shares with other overlays, and each load resets its `.data` to the
ROM's values and zeroes its `.bss`. In the port every overlay's code stays linked, so only the reset is left to do,
and `port/src/ovl.c` does it in `FS_LoadOverlay` (replacing libntr's):

- For Black and White, `tools/port_overlays.py` writes a GNU linker script (an INSERT script, for ELF and PE) that
  gathers each overlay's writable data into `.ovldata` / `.ovlbss` in overlay order, with a table (`.ovltab`) of each
  overlay's offsets and its DS layout (address, sizes). `ovl.c` snapshots `.ovldata` at start and copies an overlay's
  range back on each load.
- For the relinked modules (and every game on Apple, which has no linker script) the overlay files are archived in
  overlay order with a **marker object** before each overlay and one at the end (`ovlmarkNNN.c`, generated by
  `port_overlays.py --macho-files`, with a `--gnu-tag` per game on GNU targets). The ranges are the gaps between
  markers; `port_relink.py --ovl-tag` checks that the markers survived the relink in order and in the right sections
  (`port_overlays.py`'s `check_markers`).
- Platinum's and HeartGold's overlay lists come from pret's linker spec (`main.lsf.in`) via
  `port_overlays.py --plat` / `--hgss`; their DS addresses from the ROM's overlay table, at build time when the ROM is
  in `orig/`, else at start (`asset_load.c`'s `PORT_AssetRomOverlays`).
- Debug switches (`BWPORT_OVL_CHECK`, `BWPORT_OVL_LOG`, `BWPORT_OVL_POISON`) are described at the top of `ovl.c`.

## Startup, game switching and the restart

1. `port/src/main_start.c` runs as a constructor before libntr's `main()`: it picks the working directory, writes the
   default `sim_config.ini` when there is none, and takes the game lock.
2. libntr's `main()` sets up the simulated DS and, with `libntr-ios-reset.patch`, calls the host's `SIM_RunGame`
   (`port/src/machine.c`) instead of the game's entry.
3. With no game chosen (or `Start=launcher` in `launcher.ini`) `SIM_RunGame` runs the launcher
   (`PORT_RunLauncher`). Play calls `PORT_EnterGame` (`main_start.c`): `PORT_SetGame`, the ROM's path for the data
   fill, and a `chdir` into the game's folder, where libntr finds the extracted files and `save.bin`.
4. `PORT_AssetsLoad` fills the game's data from the ROM (below), then `PORT_Game->main()` runs the game.
5. **In-process restart.** When the game resets (`OS_ResetSystem`: a soft reset, or a return to the launcher), `machine.c`
   leaves the game's frames with a `longjmp`, puts the machine's writable data (the game's C, lib/nnsys, libntr's SDK
   libraries and the simulated DS) back to the snapshot it took before the first start, and starts again in the same
   process. The window, GL, audio device and settings stay up. Design: [docs/launcher.md](launcher.md), section 1.

## The launcher: core and front-ends

- **Core**: `port/src/launcher_core.c` / `.h`, plain C with no SDK or UI: the game table (game code, title, the
  supported dump's SHA-1), the game folders under the root, ROM import (header check, copy while hashing, refusal of a
  wrong SHA-1, extraction of the ROM's files), removal, save export/import and backups, `launcher.ini`, and the
  migration from the older flat layout. Cover art is fetched on the user's device (`port/src/launcher_cover.c`).
- **Front-ends** list and call the core:
  - desktop (Windows, Linux): `port/src/launcher_gui.cpp`, an ImGui window drawn in libntr's frames before the game boots;
  - Android: `port/src/android_launcher.c`, the JNI bridge for the Java `LauncherActivity` in `tools/android/`; the
    game runs in its own process;
  - iOS: a UIKit launcher on the same core, with the Apple port (not in the public tree yet).
- Design: [docs/launcher.md](launcher.md) (restart, more than one game, UI, ROM library, saves, settings).

## Building without game data

No ROM, game asset or game data is in the repository, and the shipped packages hold none either: the game's data comes
from the player's ROM when the game starts. Three mechanisms do it.

### Asset rows and placeholders (Black, White, Black 2, White 2)

The decompilation keeps each data table the game reads as a row in `config/<version>/assets.tsv` (a name, the DS
module and address range, a layout; no bytes), included by the C as `asset/<name>.inc`. The ROM build writes those
files from `orig/`. The port does not need them:

- `tools/port_assets.py` writes each row as a **placeholder** of the same shape (zeros in the host's layout; pointer
  fields named from `config/*/asset_ptrs.tsv`), and compiles the files that include one from rewritten copies (const
  dropped, a constructor registers each table).
- It also writes `asset_rows.c`, every version's rows (names, ranges, layouts), for `port/src/asset_load.c`.
- At start `PORT_AssetsLoad` reads the ARM9 module and overlays from the chosen ROM (decompressed as the DS does), and
  copies each row's range into its table, and into `ovl.c`'s and `machine.c`'s snapshots so a reload or restart keeps
  the ROM's values.

Black 2 and White 2 (pokebw2's C) are handled the same way; their tables were moved to rows when the tree was vendored
([docs/black2-white2.md](black2-white2.md), "The vendored tree").

### Blanks (HeartGold, SoulSilver)

The vendored HeartGold/SoulSilver tree had ROM bytes in its data (C initialisers and `.s` data). `tools/hg_assets.py`
turned each such object into a **blank** (same layout, every number 0) plus a row per game in
`third_party/pokeheartgold/port/assets_hg.tsv` and `assets_ss.tsv`; a constructor registers each blank with
`asset_load.c`, which fills it from the running game's ROM like any other row.

### The whole-image ROM fill (when the build machine has the ROMs)

A matching decompilation's compiled data is the ROM's data. When every built game's ROM is in `orig/`, the link of
`bwport` goes through `tools/port_romfill.py`, which needs no knowledge of C types:

1. The linker script groups each game's read-only data in sections of its own (`.bwrom_b`, `.bwrom_w`, ..., `.bwrom_s`
   for what the games share).
2. Between the image's relocation slots, the bytes are matched against the game's ROM modules; each match becomes a copy
   (module, offset, length) and the bytes are zeroed in the image. The tool checks itself by applying its own table to
   the zeroed image.
3. The copies go into the image's `.bwfill` section with a hash per game. At start `asset_load.c` applies them; a ROM of
   another revision fails the hash and the game does not start.

A build without the ROMs skips this step: the executable is the plain link, data compiled in as the C has it.
`tools/rom_leak_check.py` finds ROM bytes in files and packages; `tools/release_check.sh` runs it over every package
before it may leave the build machine; `tools/rom_text_check.py` keeps the game's text out of committed files.
Full description: [docs/port-build.md](port-build.md), "Game data from the ROM".

## Where each game's C comes from

| Game | Source | How the port gets it |
| --- | --- | --- |
| Black, White | `src/`, `lib/` (this repository) | compiled directly; White with `-DWHITE` and the `w_` rename |
| Black 2, White 2 | [pokebw2](https://github.com/fuddlesworth/pokebw2) (GPL-3), vendored in `third_party/pokebw2` | `tools/pokebw2_fetch.sh --vendor` updates the vendored copy: a sparse checkout at the pinned commit, the port's patches (`port/subprojects/pokebw2-*.patch`) applied, the file set the build reads copied in. Builds need no network |
| Platinum | pret's pokeplatinum, git submodule `third_party/pokeplatinum` (pinned) | `tools/pokeplat_gen.py` (below) |
| HeartGold, SoulSilver | pret's pokeheartgold with further decompilation and 64-bit work, vendored in `third_party/pokeheartgold` (`PIN` names the commit) | `tools/pokehg_gen.py` (below) |

Black 2 / White 2 details: [docs/black2-white2.md](black2-white2.md).

## The Generation IV build pipelines

Both Gen 4 games follow one pattern (shared code in `tools/pokegen4.py`), run by `port/meson.build` at setup:

1. **A working copy** in `build/pokeplatinum` or `build/pokeheartgold` (git-ignored). The source tree itself is never
   edited: Platinum's is cloned from the submodule's objects; HeartGold's vendored files are copied into a one-commit
   git repository.
2. **The port's patches** are applied in name order: `port/subprojects/pokeplatinum-*.patch` (29) and
   `port/subprojects/pokeheartgold-*.patch` (13). Each starts with a comment saying what it fixes and why: 64-bit
   pointer widths (`*-widepointers*`), host struct layouts, evaluation order, heap alignment, the route test hooks,
   win64 (LLP64) and Mach-O details.
3. **Generated headers**: pret's own generators (message IDs, NARC member indexes, enums) are run by meson with a
   header-only top-level `meson.build`, no ROM build and no DS compiler; the headers the C includes are copied to
   `gen/`. The vendored HeartGold tree already carries its `gen/`, so no generator runs for it. A stamp
   (`.bwport-stamp`) makes a re-run a no-op while the pin and the patches are unchanged.
4. **Compile and relink**: `tools/pokeplat_cc.py` / `tools/pokehg_cc.py` give the compile flags; HeartGold's data that
   pret keeps in assembly becomes `hg_data.o` (`tools/pokehg_asm.py`); each module is relinked to `pl.o` / `hg.o` as
   above. HeartGold files that do not compile yet are listed in `port/hgss_exclude.txt`.

**Generated patches.** Several HeartGold patches are not written by hand but made by a tool on a tree with every
earlier patch applied, and must be remade after a re-pin (each patch's header gives the exact command):

- `pokeheartgold-widepointers.patch`, `-widepointers2.patch`: `tools/pokehg_widen.py` (pointer casts and fields that
  must be pointer-wide on a 64-bit host);
- `pokeheartgold-wideprotos.patch`: `tools/pokehg_protos.py` (declarations whose int/pointer types disagree with the
  definition);
- `pokeheartgold-zfsview.patch`: `tools/pokehg_fsview.py` (per-file views of the field system struct placed at the
  host offsets of the real one, each checked with a `_Static_assert`);
- `pokeheartgold-zzabi.patch`: `tools/pokehg_abi.py` (declarations narrower than the definition, e.g. a `u16`
  parameter defined as 32 bits, which SysV hides but win64 does not).

The `z` / `zz` prefixes make these sort, and so apply, after the patches they are made against.

The Gen 4 host-side files: `port/include_gen4/`, `port/include_plat/`, `port/include_hgss/`, `port/game/route_gen4.c`
(route hooks), `port/game/plat_online.c`, `port/game/hg_online.c` (online stand-ins).

## Testing: the route pilot

The regression harness plays the games. A **route** is a list of legs in `tools/routes/<game>.txt` (`b`, `w`, `b2`,
`w2`, `pt`, `hg`, `ss`), one per line:

```
name | start | pilot tokens | end | save | flags
```

- `start` is a save from a save library (not in the repository) or `NEWGAME`; `end` is the zone and grid position the
  leg must finish on; `save` is the save the leg produces, which the next leg can start from (`--chain` plays a whole
  route from a new game in one go).
- The **pilot** (`port/src/route_test.c`, `BWPORT_PILOT`) feeds the keys from the game thread once a frame: walk to a
  grid cell, hold keys for N frames, and event waits that wait on the game's state (the field is free, no text is
  printing) rather than a count of frames, so a leg plays the same with the speed options on. The header of
  `route_test.c` lists every token; the other test hooks (`BWPORT_NOENCOUNTER`, `BWPORT_TESTCHEAT`, `BWPORT_POSLOG`)
  are described there too.
- `tools/route.py GAME [LEG ...]` runs legs headless (`tools/port_run.py`'s Xvfb on Linux), uncapped, with the clock
  and the random seed pinned, so the same keys always play the same frames. Each leg prints one verdict (`ok`,
  `off-route`, `crash`, `hang`, `stuck`, `timeout`, `save-failed`) and the game frames it took. `--repeat N` checks a
  leg is deterministic; equal frame totals for a whole chain on two machines check the build is.
- Everything that affects what the game does runs on the game's own frame count (V-blanks), never on wall-clock time,
  so results do not depend on the machine's speed.
- `--quick` runs the legs flagged `quick` for the Gen 5 games, the short check (`tools/check.sh`).
- Writing legs: [docs/route-writer.md](route-writer.md) (the one command `route.py GAME add`, tokens, map dumps).
- Frame-by-frame comparison against an emulator (melonDS): [docs/lockstep.md](lockstep.md).
- Unit tests: `tools/*_tests.py` for the Python tools, `port/tests/` for host C (launcher saves, resource loading,
  the SDK names), and the vcore/gpu/acore projects' own tests.

## Platforms

| Platform | How | Notes |
| --- | --- | --- |
| Linux | `meson setup build/port/linux port --native-file port/machine/linux.ini && ninja -C build/port/linux bwport` | links with lld when available; `tools/port_run.py` builds and runs headless |
| Windows | `tools/port_win64.sh`: a cross-build from WSL or Linux (`port/machine/win64.ini`: clang `--target=x86_64-w64-mingw32` for C, mingw g++ for libntr's C++) | a folder with `bwport.exe`, its DLLs, `bwport.bat` and a bundled Mesa software GL for systems without one ([docs/port-build.md](port-build.md), "win64") |
| Android | `tools/android_deps.sh`, then `tools/android_package.sh --build` (`port/machine/android-arm64.ini`, `android-x86_64.ini`; the Gradle project in `tools/android/`) | an APK with both ABIs; OpenGL ES 3; the Gen 4 games included. Checked on the emulator ([docs/android-parity.md](android-parity.md)) |
| macOS, iOS | ld64 / Mach-O paths exist throughout the build (`BWPORT_OVL_MACHO`, `port_relink.py --macho`, Mach-O section groups for the ROM fill) | coming; its front-end and Metal code are not in the public tree yet |

The meson options (`port/meson.options`): `white`, `b2w2`, `platinum`, `hgss` (each game in or out; all on by
default), `rom_dir` (where the ROM fill looks; an empty folder = a build that sees no ROM), `lld`, `allow_undefined`.
Commands and details: [docs/port-build.md](port-build.md).

## Further reading

| Topic | Document |
| --- | --- |
| The PC build: commands, design, data from the ROM | [port-build.md](port-build.md) |
| The runtime: libntr's start, threads, files, save | [port-runtime.md](port-runtime.md) |
| The boot's crash log (history) | [port-boot.md](port-boot.md) |
| Launcher, restart, more than one game | [launcher.md](launcher.md) |
| Our NitroSystem | [nnsys-port.md](nnsys-port.md), [nnsys-parity.md](nnsys-parity.md) |
| Video | [vcore.md](vcore.md), [gpu.md](gpu.md), [gpu-backends.md](gpu-backends.md), [interp.md](interp.md) |
| Sound | [acore.md](acore.md) |
| Black 2 / White 2 | [black2-white2.md](black2-white2.md) |
| Online libraries | [online-libs.md](online-libs.md) |
| Routes and lockstep | [route-writer.md](route-writer.md), [lockstep.md](lockstep.md) |
| Android | [android-parity.md](android-parity.md) |

## Not verified here

- The Apple port's state is described from the build files and the README only; its own sources are not in the public
  tree.
- What `Renderer=auto` chooses on each platform was not traced through libntr's patches.
- Android testing on real devices: the README says the emulator only; not checked further.
