# The launcher: games, ROMs, saves, settings (design, 2026-11-04)

A design only: nothing here is built. Where it gives a number, the number comes from the 11-04 macOS arm64 and iOS builds in
the ipa-build worktree (`nm`, `size -m`, `otool -l` on `build/port/{macos,ios}/bwport` and the static libraries next to
it), the sources, or the base ROM's header. Anything not measured says so. The rules, the maintainers' notes and
the maintainers' notes all apply. The ones that matter most here: the game C stays byte-identical to the ROM build,
mods go in port-only code, every platform gets the change, the maintainer's save and settings are sacred, and nothing from
the ROM goes in git.

## In short

- **Restart and game switching: reset in place.** One process and one binary on every platform. A run loop sits where
  libntr calls `NitroMain`. `OS_ResetSystem` parks the game's threads and jumps back to that loop. The loop puts the
  machine's writable memory back to how it was before the first game started, then starts a game again. The machine is
  the game C, libntrsystem, libntr's SDK libraries and the emulated hardware. The host part stays up: the window,
  Metal/GL, ImGui, the audio device, input, the settings and the renderers' threads. This is the overlay reset ovl.c
  already does at every `FS_LoadOverlay`, applied to the whole machine. Confidence: high that it works, medium on how
  much of libntr has to be sorted into machine and host (sizes below). A lockstep test checks the result exactly.
- **More than one game: still one binary.** The second game's C is compiled with a generated header that renames its
  22,052 globals. Black and White (and later Black 2 and White 2) then share one libntr. A module per game (dylib, .so,
  DLL) buys nothing the reset doesn't already give, and it costs a libntr split plus dynamic loading on five platforms.
- **The UI.** Native on the phones (UIKit, Android views), the way the settings already are. ImGui on the desktop. All
  three call one C core in port/src that does the work: the game table, ROM validation, extraction, saves, backups and
  the migration.
- **The files.** One folder per game holds its ROM, save.bin, backups and extracted files. sim_config.ini stays global
  and stays where it is. A one-time migration moves the maintainer's save into Black's folder, after making a verified copy,
  and starts Black with no launcher on screen.
- **The phases.** (1) The in-process restart, on this Mac for macOS and iOS: on the phones, Restart to title then goes
  back to the title instead of closing the app. (2) The library and the launcher. (3) The same work on the PC for
  Windows, Linux and Android. (4) White, once its decompilation matches. (5) Black 2 and White 2, much later.

## 1. Restarting inside the process

### Where the mutable state lives

macOS arm64 build, in bytes. iOS has the same layout, with per-library totals within 1% (iOS `__DATA`: `__data`
47,680, `__ovldata` 16,864, `__ovlbss` 263,728, `__bss` 9,028,360, `__common` 168,602,344). Symbol sizes are the address gaps in the linked binary, attributed to their
library or object file.

| Part | Writable bytes | What it is | At a reset |
| --- | --- | --- | --- |
| Game C, main module (240 objects of libgame.a, libentry.a, port/extern) | data 2,264, bss 29,836, common 1,240,408 | `g_GFLSndHeap` 1,226,144 is most of it | machine: restore |
| Game C, overlays (libgame_ovl.a, 1,071 objects) | `__ovldata` 16,864, `__ovlbss` 261,840 | `g_FieldHeapMemory` 131,078; ovl.c already snapshots these | machine: restore |
| libntrsystem (fnd, g2d, g3d, gfd, mcs, snd) | 45,796 | NNS state; g2d's 64-bit unpack list lives on the host heap | machine: restore, plus free that list |
| libntr's SDK libraries (31, every one but libsim) | 150,720 | fs 57,916, mb 23,524, snd 21,404, os 20,440, sim7 15,176, wm 5,508, card 3,228 | machine: restore |
| libsim: sim_vars.c | 56,885,256 (all zero-fill) | the DS: main memory 8+8 MB, TWL 16+16 MB, SUB 4 MB, VRAM, OAM, palettes, 375 registers | machine: zero the pages in use, put back the start state |
| libsim: sim_main.cpp | data 992, bss 74,912, common 110,410,736 | mostly libntr's own mixer (`win_snd_channels` 73.6 MB, `win_sndi_channels` 36.8 MB, zero-fill, untouched with Sound=acore); 362 globals, machine and host mixed | sort, then reset the machine half |
| libsim: sim_gui.cpp, imgui, glad, shaders, config | sim_gui 4,717,056 (ImGui ring, perf buckets) | the host | keep |
| libsim: sim_audio 256,664, sim_touch, sim_vcapture, g3_handler | 95, 54, 17 and 34 globals | mixed | sort (g3: the walled-off agent only) |
| Bridges and cores (gbridge 1,403,452, gpu 273,664, vcore 251,408, acore 8,524, metalpresent 2,512) | about 1.94 MB | the picture and sound, built from the machine's state | make again (they already do on a Renderer/Resolution switch) |
| port/src (32 objects) | 999,648 | keys_script 655,392, acapture 136,304, main_start, crash_bt, main_threads 11,808 ... | host, except main_threads.c, ovl.c and the speed options' state |

Facts that decide the approach:

- **The game C never touches the host heap or host files.** There is no `malloc`, `calloc`, `realloc` or `fopen` in
  src/, lib/gfl, lib/spl or lib/poke_net. Its heaps come from `OS_AllocFromArenaLo`, which lives in emulated main memory
  (lib/gfl/src/heapsys.c). So a game run's whole state is its globals plus the emulated hardware, and copying memory
  back is enough for it.
- **All the game's threads are the port's.** main_threads.c runs them on 16 host threads it keeps. Every point where a
  game thread can block goes through a `PORT_*` function (docs/port-runtime.md "Threads"), so the port can stop them
  there.
- **`OS_ResetSystem` is called from the main loop's C frames.** There are 8 call sites. The soft reset is
  `GFL_HIDDoSoftReset` (lib/gfl/src/ui.c, called from `GFL_HIDUpdate` in the main loop), which first waits for
  `CARD_TryWaitBackupAsync`. The save is complete, and
  closed, before the reset: libntr opens and closes save.bin on every backup call. No C++ or Objective-C frame sits
  between `NitroMain` and the reset, so a `longjmp` back to the caller of `NitroMain` is safe.
- **libntr sets the machine up before `NitroMain` and never after** (sim_main.cpp's `SIM_main`): the config, registers,
  `header.bin`, `SIM_pxiInit` (queues and semaphores), the two PXI threads, WM, `SIM_RenderInit`, the net thread. A copy
  taken at that point is a DS at power-on whose host objects are all still alive.
- **libntr's frame function already loops with the game stopped**: `do { ... } while (IsGameLogicPaused())` in
  sim_main.cpp. The desktop launcher's frames can run through that loop.

### Options weighed

| Option | Verdict |
| --- | --- |
| **Reset in place**: copy the machine's writable ranges at start and copy them back at a reset; the host stays up | **Recommended.** One mechanism on all five platforms, no dynamic loading, and the desktop runs the same path as the phones, so desktop runs test it. ovl.c has done the same for overlays since the boot work. |
| A dylib or framework per game (or game plus SDK), loaded from the app bundle | Rejected. iOS loads embedded frameworks signed with the app's team, and SideStore re-signs them, so loading is possible. Unloading is not: dyld keeps any image with thread-local variables or Objective-C metadata loaded, and the binary has 4 thread-locals (libos `s_WIN_unknownThread`, main_threads.c `tSelf` and `tDepth`, `t_seenEpoch`). A fresh state still needs the memory copied back. It also needs libntr split into a shared host library, DLL exports on Windows and several .so files on Android. That is more work than renaming, for nothing the reset doesn't give. |
| Shut libntr down completely (window, GL/Metal, ImGui, audio, threads) and run `SIM_main` again | Rejected. It needs a shutdown libntr doesn't have, and every host resource has to come down and go back up: an SDL window and Metal layer made again under UIKit, the window flashing on every restart. It still needs the memory copied back for the machine. |
| Android only: the game in its own process (`android:process=":game"`), killed and started again | Kept as a fallback only. It is simple on Android but a second mechanism, and iOS needs the in-process path anyway. |
| iOS: end the app (today's behaviour) | The maintainer rejected it. |

### The design

**The run loop** (port/src, called by a libntr patch in place of `NitroMain();` at the end of `SIM_main`):

```
snapshot the machine once                        // a DS with no cartridge
for (;;) {
    if no game was chosen: show the launcher, wait for a choice
    enter the game: chdir to its folder, header.bin into HW_ROM_HEADER_BUF, the save's path, its overlay table
    if (setjmp(reset) == 0) game->entry();       // NitroMain; never returns
    reset the machine                             // back here from PORT_ResetSystem
}
```

**PORT_ResetSystem** (reset.c; main thread, game lock held):

1. Note what comes next: the same game (restart), the launcher, or another game.
2. Mute the sound: the acore bridge outputs silence.
3. Park the game's other threads. main_threads.c raises its generation number, lets the lock go and wakes every slot.
   A game thread that takes the lock under an old generation `longjmp`s to the top of its host thread's loop and parks
   there. When all of them are parked, the main thread takes the lock back.
4. Quiet libntr's threads: drain PXI both ways (`SIM_PxiDrain` exists for 9 to 7; 7 to 9 needs the same), and stop
   sim7's per-alarm sound threads (`sim7/src/pxi.c`, a `pthread_create` per alarm). NET stays idle with online off.
5. `longjmp(reset)`. On Windows, if mingw's SEH unwind gives trouble, use `__builtin_setjmp`/`__builtin_longjmp`.

**Resetting the machine** (the run loop, after the jump):

1. Free what the machine put on the host heap after the snapshot, and close the host files libntr's FS opened. The
   audit's list includes g2d's unpack list and the blocks it tracks (g2d_Load.c `s_unpacked`), the open `FSFile`s'
   `pcFilePtr` (fs_file.c; the port layer keeps a list), and the gx, mi and card side tables.
2. Restore the machine's ranges. Initialized data is copied back from the snapshot. For zero-fill memory, zero the
   pages that are resident now and copy back the pages the snapshot kept. On Darwin, Linux and Android, `mincore` finds
   the resident pages; Windows scans. libntr's 110 MB mixer and the TWL memories are never touched, so they never become
   real memory.
3. Reset the machine half of libsim's mixed files (`SIM_ResetMachine` in the patch, or their globals tagged into the
   ranges).
4. Port state that belongs to one run: main_threads.c's slots and the main thread's `tDepth`/`tSelf`; ovl.c (its
   ranges are machine ranges; nothing stays loaded); the speed options (`SIM_SetBattle(0)`, move_speed.c).
5. Picture and sound: the bridges make their cores again, with the destroy and create they already do on a switch.
   Smooth motion drops its previous frame. acore is made again and its output ring emptied.

**The snapshot** is taken once, at the run loop's first pass, before any game. It holds the initialized data (about
21 KB: the game's 19 KB, libntr's and libntrsystem's 1.5 KB) and, of the zero-fill ranges, the resident pages that
aren't zero. Those are expected to be a few pages: the NVRAM config, `KEYINPUT`, the button XY word, and the queue
pointers and semaphores `SIM_pxiInit` stores. It is the DS before a cartridge goes in. Entering a game
reads header.bin again each time.

**The build: one machine range** (two sections), as the overlays have one:

- macOS and iOS (ld64, no linker script): the machine's C files are compiled with
  `-include port/machine/macos_machine_section.h`
  (`#pragma clang section data="__DATA,__machdata" bss="__DATA,__machbss"`, the overlay header's pattern) and
  `-fno-common`. The range comes from ld64's `section$start$__DATA$__machdata` and `section$end$...` symbols, so no
  marker objects are needed. Measured: `-fno-common` costs nothing, because the main module has 17 common symbols and
  each is defined once. Like `__ovlbss` today (flags 0), the pragma's bss takes file space: about 1.5 MB, acceptable.
- Linux, Android, Windows (GNU ld and lld with `-T ovl.ld`): the linker script tools/port_overlays.py writes gathers
  the machine objects' `.data`, `.bss` and `COMMON` into `.machdata` and `.machbss`, with start and end symbols. It
  matches game objects by path and libntr's archives by name (`*libos.a:*`).
- sim_vars.c's big memories: a zero-fill custom section works in ld64 (checked in step 1): `bss="__DATA,__machbss,zerofill"`
  gives an S_ZEROFILL section that takes no file space, so sim_vars.c simply takes the pragma and no table is needed.
- What goes in the range: the game objects, libntrsystem, libntr's SDK libraries (every library except libsim), sim7,
  sim_vars.c, and port/src's ovl.c and main_threads.c state. These are all C: a C++ global that owns heap memory must
  never be put in the range.

### Step 1 as built (2026-11-05, macOS and the iOS Simulator)

Where the code showed the design wrong, step 1 does this instead (port/src/machine.c, libntr-ios-reset.patch,
libntrsystem-ios-reset.patch):

- **Sizes.** `__machdata` 3.9 KB, `__machbss` 58 MB zero-fill (the binary's size is unchanged); the snapshot keeps 407 KB
  (the data and the zero-fill pages that are not zero at the start). A reset takes 10-110 ms on the loaded Mac.
- **libsim's mixed globals are not sorted.** sim_main.cpp's 365 symbols are almost all the host's (pacing, the present,
  perf counters, the cfgscript, the window). Its machine globals are few: `OS_IRQTable`, the four GX VRAM-map structs, the
  sound channel and alarm arrays and the shared work pointer; `SIM_MachineExtra` lists them and machine.c adds them to its
  ranges. sim_vcapture, sim_touch, sim_audio and the 3D processor stay out (the game sets them up again at boot; T1 found
  nothing there). libsim's C files (glad, config, queues, screen quads) are the host's: a second -include puts them back
  in the default sections.
- **A precompiled header hides the pragma.** libntr's C is built with `c_pch: nitro_pch.h`, and meson compiles the PCH
  with the same -include: clang then skips the -included header in each file and the pragma was lost (every SDK global
  stayed in `__bss`). On Apple builds the patch drops the PCH and -includes nitro_pch.h itself, before the pragma.
- **port/src is the host's,** in its own archive (porthost), apart from the DS-side stand-ins (sdk_*.c, stub_*.c,
  linker_syms.c with the DTCM and ITCM arenas, boot_exit.c), which go in the range. main_threads.c and ovl.c are NOT in
  the range (they hold host semaphores, host threads and a host-heap snapshot): they reset their own state
  (PORT_ThreadsReset, PortOvl_Reset). The 16 slots' host threads are made before the snapshot (PORT_ThreadsPrestart), so
  libntr's thread tables in the snapshot already hold them.
- **libos's host side stays out:** os_thread.c's locks, its condition variable and wake epoch, and the host thread table
  (`OSi_HOST`, an explicit `__DATA,__data`). Putting a pthread mutex or condition back while threads wait on it breaks it.
- **The SDK makes threads during the game's boot** (CARD's and CTRDG's task threads, OS_CreateThread from CARD_Init and
  CTRDG_Init): one more of each at every boot. The design missed them. libntr now notes which threads were made after the
  snapshot; at a reset SIM_OsStopThreads ends them (each leaves its function from its next OS_SleepThread) and joins them.
- **Quiet before the jump:** the game lock is let go while the slots park, PXI drains both ways (a 7-to-9 pending count is
  new), sim7's alarm threads are joined, and the SDK's threads end. The restore itself runs with acore's lock held (the
  sound callback writes the game's sound work), then acore is made again (ab_reset) with libntr's last split and quality.
- **Host state that follows the new boot:** the key script and the cfgscript's pN steps count from the new boot's V-blank 0
  (as the exec did: a test's keys replay), BWPORT_BENCH_SEED reseeds again, both renderers give back what they hold at
  the next frame (SIM_ResetHost: the idle hooks, as on a switch).
- **A reset during a save** does not finish the save: the game's soft-reset callback (SaveControlIntr, ov088) cancels it,
  as on the DS. Early in the save (frame 1990 of a save from 1967 to 2556) save.bin stays as it was; late (2300) it is
  partly written; CONTINUE loads both.
- **T1 on the Mac** has no lockstep (its wraps are Linux-only), and two processes cannot be compared byte for byte (ASLR
  moves every pointer). Instead BWPORT_MACHINE_CHECK=FILE hashes each range at every V-blank wait in one process, cold boot
  and every boot after, and BWPORT_MACHINE_DUMP=V,... writes the ranges' bytes at those V-blanks;
  `tools/mac_reset_check.py --dump RUN EXE V` names the symbols that differ. Room save, pinned clock and seed, V-blanks
  600, 1200, 2000 and 2900 (title, CONTINUE, the room): at most 1,174 bytes of 58 MB differ, all host pointers (two
  SDL_Thread handles, g2d's free-delay blocks), the sound driver's state the ARM7 side writes on its own clock
  (sDriverInfo, sSharedWork, sSeqPlayer, sPlayer) and the game's wall-clock timers.
- **iOS (Simulator, 2026-11-05):** the settings sheet's Restart to title (test hook `askrestart;alert=close`) and cfgscript
  `restart=1` return to the title in the app: CONTINUE then loads the room. The alert now says the game starts again at
  the title screen, not that the app closes. Under heavy load UIKit may not present the sheet at all in the Simulator
  (`UIKit did not present the sheet`): that run tests nothing.

### Step 1-PC as built (2026-11-05, Linux and Windows; Android's link follows)

- **The range by linker script.** tools/port_overlays.py's script (`machine_script`) gathers `.data`, `.bss` and COMMON of
  libgame.a, libentry.a, every libntr and libntrsystem SDK library but libsim, and libsim's sim_vars.c, into `.machdata`
  and `.machbss` (inserted after the overlays' two, so an overlay object stays the overlay's), with `__bwmach_data0/1`,
  `__bwmach_bss0/1` and `__bwovl_*_end` symbols that machine.c reads. Matching is by archive path (`*libsim.a:*` with
  EXCLUDE_FILE), so no compile flag, pragma or PCH change is needed off Apple (the Apple-only -include and PCH changes
  were only for ld64). GNU ld (Linux, mingw) takes it; the same text is for lld (Android), not yet linked there.
  port/src's host files are libporthost.a on every platform. libntr's host thread state (`OSi_HOST`) is an orphan
  section `.bwhost` off Apple. Linux: machdata 81 KB, machbss 58 MB zero-fill; the same numbers on Windows.
- **mincore, Windows.** No mincore: the working set cannot tell a page trimmed after the game wrote it from one never
  touched, so on Windows every page of the zero-fill ranges counts (a reset reads 165 MB: 6-8 ms measured, and the
  pages become resident). Linux uses mincore as the Mac does.
- **mingw's longjmp** across the game's frames (SEH unwind of clang's C frames) and the slot threads' longjmp work as
  they are: no `__builtin_setjmp` was needed.
- **reset.c** no longer execs (Linux) or starts a process (Windows); only Android still shows its message box.
- **Tests.** T3 on Linux and on the Windows exe: a room save, CONTINUE by keys, `restart=1` three times in one process
  (cfgscript `f3300:restart=1;p3300:restart=1;...`): each boot returns to the title and CONTINUE loads the room again in
  the same window; save.bin byte for byte unchanged. Reset 1 ms (Linux), 6-8 ms (Windows). Linux T1 (BWPORT_MACHINE_CHECK,
  tools/mac_reset_check.py --dump now reads ELF symbols; gdb runs without ASLR so two processes compare): at V-blank 3
  the cold boot differs from the reset boot by the same host-pointer symbols as two cold boots differ from each other
  (s_portFiles, sSndArc, sSndMutex, cardi_common, CTRDG task list, one s_HW_MAIN_MEM pointer) plus sSharedWork (the
  sound driver's state the ARM7 side writes on its own clock) and current_dir_pos (2 bytes, a heap pointer). At V-blank
  700 two cold boots already differ in the title's VRAM (wall-clock timing), so exactness beyond the first frames is not
  measurable that way. Windows: the process's heap grows about 1 MB per reset and its handles by 1-2 (not chased).
- On win64 the link no longer sets `link_language: 'cpp'` (meson then adds -lstdc++, which finds libstdc++.dll.a before
  the static one: duplicate symbols); meson picks the C++ linker itself there.

### What libntr has to reset, and how big the split is

| Item | Where | Size | How |
| --- | --- | --- | --- |
| Emulated registers, main memory, VRAM, OAM, palettes, WRAM | sim_vars.c (375 globals) | 56.9 MB, zero-fill | table and pages (above) |
| IRQ table, V-blank count, timers, DMA, display capture, G3 FIFO counters, PXI pending count | sim_main.cpp, sim_vcapture.cpp (`OS_IRQTable` is in libsim's `__data`) | part of 362 + 17 globals | sorted by the patch |
| SDK libraries' state (OS alarms, the arena, FS tables, CARD, SND, MI, GX, PXI callbacks, WM, MB) | 31 libraries plus libntrsystem | 196 KB | in the range: generic |
| Host resources those libraries create | `malloc`/`realloc`/`new`/`fopen`/`SDL_Create*`/`pthread_create` sites: g2d 26, gx 12, sim7 10, g3d 8, fs 5, mi 4, card 3, pxi 2, os 2, snd 2 | 74 sites to read | made before the snapshot: kept (the pointers come back valid). Made after it: freed or closed at the reset |
| Threads | PXI 9 to 7 and 7 to 9 (kept, drained), the sim7 alarm threads (stopped), NET (kept), libntr's own `OS_CreateThread` (not used by the game) | | above |
| The overlay loader | port/src/ovl.c | 16,864 + 261,840 | in the range |
| Audio | SDL device (kept), acore (made again), libntr's mixer channels (zeroed if resident) | | above |
| libsim's own host-heap and thread sites | sim_main, sim_audio, sim_touch, ... | 87 sites | sorted with the globals |
| libntr's 3D command processor | g3_handler.cpp, sim_g3_*.cpp | 34 + 3 globals | **only a walled-off agent** (the maintainer's exception, libntr-ios-g3skip.patch) may read or sort it. If it holds no host objects, it simply goes in the range. |

The split: one new libntr patch, `libntr-<platform>-reset.patch` (last in `diff_files`, as the handoff says). It holds
the run loop's call site, the sorting of about 560 libsim globals, the FS file list, the two thread stops, an absolute
path for sim_config.ini and imgui.ini (section 6), the "no game" frame, and the compile flags for the range. Estimate:
a few hundred lines. The port side (port/src/machine.c, the run loop, main_threads.c, reset.c, the bridges) is about
300-400 lines. Nothing in src/ changes.

### Tests (step 1)

- **T1, exact restart (macOS, automated).** Use a pinned clock and seed (`BWPORT_BENCH_CLOCK`, `BWPORT_BENCH_SEED`), a
  test save and a key script, and the lockstep dump (`BWPORT_LSTEP`, docs/lockstep.md). The records of the boot after a
  reset must equal those of a cold boot, in the same process, for 3,000 frames (title, continue, field): every variable
  and every screen hash. A difference names what the reset missed. This is the check that makes "complete" measurable.
- **T2, soak.** 50 restarts in one process (a cfgscript `restart=1` every 600 frames). Print the RSS, the open file
  count and the reset's time after each one. RSS and files must stay flat; the time is expected to be a few ms.
- **T3, by hand.** Restart to title from the macOS menu and from the iOS sheet in the Simulator. The delete-save reset,
  on a copy of a test save. A restart during a save must still be refused (`softResetBlock`).
- **T4, the phone** (main session, with the maintainer). 10 restarts with the memory gauge flat, then the maintainer plays.

## 2. More than one game

### What White changes (from the source; White's ROM is not on this Mac)

- One define: `GAME_VERSION` in include/constants/version.h. 18 files in src/ and lib/ use it, for example
  mb_parent_sys.c's icon paths and the origin stamped on every caught Pokemon. Code already in Black compares at runtime
  with `VERSION_WHITE` in 12 files (trainer_card.c, zone_weather.c, game_clear_save.c, ...).
- The ROM build's config (config/w_us: symbols, delinks, `assets.tsv`). The asset .inc files the port compiles in come
  from the base ROM at build time, and the overlay digests among them differ per ROM. So the White build needs the
  maintainer's White dump at orig/baserom_w_us.nds.
- Code and data beyond that (Black City and White Forest, the version's legendary, exclusives) are unknown until White's
  ROM is delinked against Black's symbols. That is the "White from the same source" phase of the maintainers' notes, and it
  doesn't change this design.

### One binary, the second game renamed

- **They clash on every name.** The game objects define 22,052 global symbols, and White's build of the same C defines
  the same names, so linking both fails on each one. Two games in one binary therefore need a rename or a module each.
- **The shared layer barely calls the game:** libsim calls `NitroMain`, sim7 calls `NVRAM_Init`, and libnnsg2d,
  libnnsgfd and libdemo call 9 to 36 GX/GXS load functions each. Those functions are SDK code that lives in the game's
  directories (src/main/main_0207e3d0.c, main_02083644.c). port/src uses 6 game names: ovl.c's `bwovl_*` tables,
  acapture.c's `g_GFLSndHeap` and bench_pin.c's `GFL_RandomUpdateMT`.
- **So:** Black keeps its names. Every later game's files are compiled with a generated `-include` header that renames
  each global the game defines (`#define GameInit w_GameInit`, ...), from nm of its objects. It is a port-only file, the
  way `#define main NitroMain` already is. The SDK-in-game files are compiled once and shared, after a byte compare of
  White's copies in the ROM. Each game gets its own entry, overlay table and lockstep names. A missed rename shows as a
  duplicate-symbol link error, not as a silent bug.
- **Cost:** about 4.6 MB more code per game (the game objects' `__TEXT` today; libntr plus libntrsystem is 2.8 MB; the
  IPA is 5.5 MB). Each game's globals go in the machine range, and an idle game's pages are never resident.
- **Black 2 and White 2** are another decompilation (pokebw2's tree, GPL-3). They come in the same way: their own source
  tree as a third and fourth game, each with its own prefix, sharing libntr. What they need from libntr that Black
  doesn't is found then. The launcher doesn't change.

### Step 3 as built (2026-11-06, macOS and the iOS Simulator)

- **When:** port/meson.build builds White when orig/baserom_w_us.nds is beside Black's ROM (tools/mac_rom.sh's dir) on an
  Apple build (`build_white`). No option: without the ROM the build is Black's alone, as before. tools/rom_modules.py
  --assets now makes config/w_us/assets.tsv from the delta first (tools/version_config.py; configure.py does it on the PC),
  so build/w_us/asset gets White's 77 .inc files (2 differ from Black's: the overlay digests).
- **No rename header: one relocatable object.** White's C is compiled with `-DWHITE` and `-I build/w_us` (its assets), into
  w_game, w_entry and w_game_ovl (White's overlay layout: `tools/port_overlays.py --version w_us`, its DS addresses and
  sizes, ovlgen_w/). `clang -r -Wl,-exported_symbol,_w_bwgame -Wl,-all_load` links them into white.o: every global but White's
  table becomes local, so nothing clashes with Black, White's references bind to White's own definitions (weak ones too:
  no coalescing with Black's), and what White doesn't define goes to the shared code. A header of 22,000 `#define`s would
  also have renamed struct members and `#ifdef` tests and made every Black edit recompile White; this needs neither.
  ld64 keeps the atoms' order, so White's overlay markers bound its overlays inside __ovldata/__ovlbss as Black's do.
- **Shared, compiled once:** src/main/main_0207e3d0.c and main_02083644.c (the SDK code libnnsg2d, libnnsgfd, libdemo and sim7
  call: GX loads, NVRAM_Init; no version code) and port/src's DS-side stand-ins (sdk_*, stub_*, linker_syms.c,
  boot_exit.c). None of them references a game symbol (checked with nm), so sharing them cannot bind White to Black.
- **The host's way in:** port/include/port_game.h's PortGame (port/game/game_table.c, compiled into each game: `bwgame`,
  `w_bwgame`): NitroMain (machine.c), the sound heap (acapture.c), GFL_RandomUpdateMT (bench_pin.c) and the ovltab markers.
  Those were the only game symbols the host and libntr's libraries used besides the shared ones (nm of every archive).
  PORT_EnterGame sets PORT_Game by the game code (a flat BWPORT_DIR folder is Black's); ovl.c makes a game's table and
  .data snapshot the first time it is chosen (PortOvl_SetGame), when its overlays' data is still as linked.
- **The machine range** holds both games (White's objects take the same section pragmas): __machdata 3.7 -> 6.0 KB,
  __ovldata 16.9 -> 33.7 KB, __ovlbss 264 -> 527 KB, __machbss 58.26 -> 59.52 MB zero-fill. A reset puts both back; the idle
  game's pages are not touched.
- **The stand-ins (port/extern, 17 files):** each includes its game file, so White's `#if GAME_VERSION` code there is
  compiled in; none of the 17 originals tests the version, and none of their functions is among what White changes
  (config/w_us/delta.txt's 101 size and relocation entries, mapped to Black's symbols: 66 files, e.g. title.c, zonedata.c,
  staff_roll_main.c, scrcmd_pokemon_fld.c). So no stand-in changed. The only excluded file with version code is ctvt_draw.c
  (the Xtransceiver, stubbed). lib/nitro's version differences (crt0, os_arena, fs_overlay_digest) are libntr's side.
- **Size:** macOS bwport 14.7 -> 23.1 MB (__text 6.90 -> 11.29 MB); iOS device bwport 24.3 MB (white.o 10.9 MB).
- **Black's cost:** none measured. Town save, no frame skip, pinned clock and seed, two 4 s bench rows, three runs each
  alternating (1-minute load 2.6-3.7): 12.33-12.34 / 12.38 M instructions a drawn frame before, 12.32-12.35 / 12.38-12.40
  after; CPU 4.0-5.6 ms a frame before, 5.2-5.5 after (the scheduler's spread).
- **Tests (macOS, launcher roots, frame skip 4):** White imported (sha1 checked, extracted), its title, a new game through
  the intro by keys (A every 40 frames, START every 400) to the opening dialog and the room, saved there (X, Save), a new
  process's CONTINUE back in the room (~/bw-saves/white-room.bin). Black from the town save: field, Save. One
  process with both games and their saves, Games... six times (game=next, a new test step of both launchers): each boot
  reached its scene (White's room, Black's town), resets 10.6-17.0 ms, 40 threads each time, heap 32.9 MB after Black's
  first boot, 42.9 after White's first, then 46.2-46.3; both saves' sha1s unchanged. The launchers show White's card and
  tab in its light theme. **iOS Simulator** (`ios_sim_run.sh --fresh`, BW_SIM_ROM2 for the second ROM): both ROMs imported
  by the launcher, White played to its title, Games... to Black's title and back to White's (resets 32-42 ms, threads 45,
  44, heap 25.3-25.4 MB). Screenshots: ~/bw-cache/white-shots/ (its README).
- **The PC (Windows, Linux, Android)** has White its own way since 2026-11-05 (the rename header: the next "Step 3 as
  built"); the merge of 2026-11-06 keeps both links and one table: White's PortGame is w_bwgame on every target
  (port/game/game_table.c), and port/src/games.c's PORT_SetGame makes the chosen game's table PORT_Game. What a
  relocatable link would take on the PC instead (not built): `ld -r` then `objcopy --keep-global-symbol=w_bwgame` (ELF, and PE with mingw's objcopy). The
  linker script then has to place white.o: its .data/.bss in .machdata/.machbss (match `*white.o`), and its overlays'
  ranges, which `ld -r` merges into one .data unless White's overlay files are compiled into sections of their own (as
  on ld64, `__attribute__((section))` per overlay group, or -fdata-sections plus per-overlay markers). ovl.c's script path
  then needs a second table (port_overlays.py's .ovltab is Black's). Not built.

### Identifying a ROM

From the header (GBATEK layout, checked on orig/baserom_b_us.nds): the title at 0x00 ("POKEMON B"), the game code at
0x0C ("IRBO"), the unit code at 0x12 (0x02: DS + DSi), the chip size at 0x14 (0x0b: 256 MB, the file's 268,435,456
bytes) and the ROM version at 0x1E (0). The used size is at 0x80 (0x0C3B8E00) and, with the DSi area, at 0x210
(0x0C45D400). Every one of the 62,532,608 bytes after 0x0C45D400 is 0xFF.

| Game | Code | Supported dump | sha1 |
| --- | --- | --- | --- |
| Pokemon Black (USA, Europe) | IRBO | 1.0, the base ROM | 26ad0b9967aa279c4a266ee69f52b9b2332399a5 |
| Pokemon White (USA, Europe) | IRAO | 1.0, the user's dump (header checked: "POKEMON W", IRAO, unit 0x02, chip 0x0b, 268,435,456 bytes) | bc696a0dfb448c7b3a8a206f0f8214411a039208 |
| Pokemon Black 2 (USA, Europe) | IREO | 1.0, the user's dump (the maintainers' notes) | e51e6dfb8678a3d19dcd2a10691b96a569ca0abb |
| Pokemon White 2 (USA, Europe) | IRDO | 1.0, the user's dump | b5d7490be7b415b8f1e672a53e978a9cc667e56a |

The White, Black 2 and White 2 codes are the public ones; confirm them on the dumps. Validation is code first, then
sha1. A trimmed dump is hashed as if it were padded with 0xFF to the chip size (128 KB << byte 0x14), so one sha1 per
dump accepts both full and trimmed files. Other languages of Black (IRBD, IRBF, IRBJ ...) are refused with their name:
the build's C and asset bytes are the USA/EUR 1.0 dump's. The table holds codes, titles and hashes only, never ROM
bytes. Test ROMs (trimmed, one byte changed) are made at test time from orig/, outside git.

### Step 3 as built (2026-11-05, Linux, Windows, Android: White in the same binary)

- **The build.** port/meson.build's `white` option (auto: on when the ROM build made `<rom build>/w_us/asset`, off on Apple
  until the overlay part below is done there) compiles the game's C a second time into libgame_w.a and libentry_w.a: the
  same sources with `-DWHITE`, `-I build/w_us` in place of `-I build` (White's asset .inc files, made by the assets.tsv
  machinery from White's ROM: today they equal Black's but for lib/nitro's overlay digests, which the port does not
  compile), and `-include w_rename.h` first. tools/port_rename.py writes that header at build time from Black's game
  archives with llvm-nm (any object format; `--underscore` for Mach-O): `#define X w_X` for each of the 21,514 globals
  they define. Statics keep their names (no clash). The cost: about 9 MB more text in the Linux exe.
- **Not renamed, not built twice:** port/src's DS-side stand-ins (sdk_*, stub_*, linker_syms.c, boot_exit.c), and the two
  SDK files in the game's dirs that libntr and libntrsystem call by name (`port_rename.py --shared`: src/main/main_0207e3d0.c's
  GX/GXS loads, called by libnnsg2d/libnnsgfd/libdemo, and main_02083644.c's NVRAM, called by sim7). Both games use
  Black's copy, so one copy holds their state (a Begin/Load/End pair split over two copies would lose the bank). Both
  files are complete in both ROMs and do not test the version.
- **Macro traps found:** port/extern's files that `#define` a game name around the game file's `#include` and `#undef`
  it after lost the rename at the #undef: they push and pop the macro now. tr_ai.c's function-like macros hide the
  rename from the command table's bare entries: under the rename those name the w_ versions by asm label
  (BWPORT_RENAMED, from the header). A missed rename is a duplicate-symbol link error.
- **At run time:** port/src/games.c holds the running game, as a pointer to its PortGame table (port/include/port_game.h;
  since the merge with the Mac's step 3 on 2026-11-06: White's table is w_bwgame, compiled from port/game/game_table.c
  under the rename). PORT_SetGame sets it by the game code: PORT_EnterGame from the table row, the flat layout (BWPORT_DIR)
  from header.bin's code; machine.c starts PORT_Game->main (NitroMain or w_NitroMain); acapture.c, bench_pin.c and
  ovl.c reach the running game's sound heap, MT and battle overlay numbers through it. BWPORT_GAME=IRAO picks White at
  start on every platform (a test hook).
- **Overlays and the machine:** White's objects keep Black's member names, so tools/port_overlays.py's patterns put
  each overlay's White data beside Black's in the same range: a load resets both copies; the DS layout table stays
  Black's (White's overlays have the same files and sizes but ov088, 0x20 longer; the overlap relations are the same).
  libgame_w.a and libentry_w.a are in the machine range (MACH_ARCHIVES), so the restart restores White too.
- **Not done:** lstep_dump.c still reads Black's names (a White lockstep needs its game names through games.c, its
  statics by the second ELF match, `--wrap=w_buildSeed,w_GFL_VBlankUpdate`, and lstep_melon with White's addresses:
  frame counter data_020aa1a0 at 0x020aa1c0, data_020aa1a4 0x020aa1c4, data_021469f8 0x02146a18, buildSeed 0x0203f270,
  OSi_ThreadInfo 0x0215100c, g_GameSystem 0x02146268, `--symbols config/w_us/arm9`; and an anchor, as no White save
  exists). Apple (the maintainers' notes, "White").
- **Tests (Linux).** Links with no duplicate symbols. Black: port_run.py -t 30 --fast to frame 14220 (477 fps). White
  (BWPORT_GAME=IRAO, BWPORT_ROM=the White dump, own root): the import checks the sha1 and extracts into Pokemon White/,
  the title with Zekrom, the intro, the boot key script to the room with Cheren and Bianca (f23600); 44 overlay loads
  with BWPORT_OVL_CHECK=1, no FAIL. Switching: BWPORT_LAUNCHER_TEST/AGAIN="game=+1;play" and cfgscript `games=1` every
  2000 frames run Black, White, Black, White in one process (resets 118, 6, 5 ms). T1 by BWPORT_MACHINE_DUMP at
  V-blanks 3 and 300: Black after White against the process's cold Black boot differs only in bss symbols every reset
  boot differs in (sSharedWork, CTRDG's task list, sSndMutex, cardi_common, current_dir_pos, s_freeDelay, 4 bytes of
  main RAM); data, overlay ranges and the DS memory are equal. White after Black against a cold White process: those
  host symbols plus the generators (2496 bytes, the MT's 624 words, and the LC's 8), seeded from boot entropy that
  includes an uninitialised stack buffer; pin them (docs/lockstep.md) for an exact compare. A Black save (town.bin)
  CONTINUEs in White into Nuvema Town, unchanged on disk (what White does with the trainer's version later: not checked).
- **Windows:** the win64 exe links and runs White (log: the import with White's sha1, `game: Pokemon White`, the keys
  to f2000); the desktop screenshots came back blank white, so the title was not seen there. **Android:** both ABIs link,
  the APK packs them; in the headless emulator (BWPORT_DIR run) White's start menu and the intro show.

## 3. The launcher's UI

Recommendation: **native on the phones, ImGui on the desktop, over one C core.**

- The phones need the native pickers anyway: UIDocumentPicker on iOS, the Storage Access Framework on Android. The
  owner already took native settings on both (ios_settings.m's inset-grouped sheet; SettingsScreen.java with the same
  rows). The launcher looks and behaves like the settings next to it.
- The desktop already has ImGui menus, a Config window, and a file dialog linked in (libntr's ImGuiFileDialog,
  `OpenFileDialog` in sim_gui.cpp).
- The work lives in C, so behaviour can't drift between platforms. The C core does the game table, scan, import with
  validation and extraction (progress through the existing `PORT_ExtractPercent`), save export and import, backups,
  restore, removal, last game and the migration. The UIs only list and call, as the settings do through
  `SIM_UISettingGet`/`Set`. Pickers hand the core a readable path: iOS gives the security-scoped URL's path, Android
  copies through ContentResolver into the app's cache. The core writes an export to a temporary file, and the UI hands
  it to the share sheet or to SAF's output stream.
- iOS: ios_import.m becomes the launcher, a UIKit view controller presented over SDL's view the way the settings sheet
  is. While it is up, the main thread pumps SDL events (the `SimTouchSettingsWait` pattern).
- Android: LauncherScreen.java, a full-screen Dialog over SDL's activity like SettingsScreen.java. ImportActivity's
  import code moves into it, and BwportActivity becomes the LAUNCHER activity. A separate activity would mean starting
  SDL's activity again in the same process.
- Desktop: a Games window in libntr's GUI, opened from Game > Games..., which also opens by itself when there is no
  game. Its frames are libntr's frame function with the game logic stopped.
- Leaving a game for the launcher uses the game's own soft reset, the same press as Restart to title, flagged "then the
  launcher". The game keeps its rules (no reset during a save), and unsaved progress is lost as on a DS, so it asks
  first.
- Everything happens in the launcher, with no game running: import and export of saves, removal, switching. YAGNI: no
  export from inside a running game.

### Step 2 on iOS as built (2026-11-05, the Simulator)

- **The run loop's hook** (port/src/machine.c, `ChooseGame`): SIM_RunGame asks main_start.c's `PORT_LauncherWanted()` before
  the snapshot and again after each reset; when it is set it calls `PORT_RunLauncher()` (iOS: ios_launcher.m; elsewhere an
  empty stub in main_start.c), which returns once Play has entered a game (`PORT_EnterGame`: the folders, the tiered backup,
  LastGame, the working directory, iOS's links). Then the game's header.bin is read into `HW_ROM_HEADER_BUF`: libntr's SIM_main
  read it at start from the working directory of then (none at a first start). The game lock stays held while the launcher is
  up: nothing of the game runs. This is the piece the desktop's ImGui Games window and the PC reuse: implement
  `PORT_RunLauncher` (frames of libntr's GUI with the game logic stopped), set `PORT_SetLauncherWanted(1)` from Game > Games...
  before the soft reset, and on Linux/Windows the same two calls go into their SIM_RunGame once the in-process restart exists
  there (step 1-PC).
- **The start (iOS):** Start=last with a game goes straight into it as before (the upgrade from the flat layout: migration,
  then Black, the save's sha1 unchanged). Start=launcher, or no game (no ROM copy and no files), leaves the working directory in
  the root (Application Support/bwport) and the launcher comes up over SDL's window; a ROM copy without its files is extracted
  by the launcher, with its progress, and played.
- **Games** (the settings sheet's row): an alert ("Unsaved progress is lost"), then `PORT_SetLauncherWanted(1)` and the same
  L+R+START+SELECT press as Restart to title (which clears the flag). If the game ignores the press (during a save) the flag
  stays until the next reset. From the launcher the sheet hides Restart, Games and the performance test.
- **The save line** reads the game's own play time and save date (`LC_SaveFileInfo`: the player's block, 0x19400 + 0x24 of each
  copy, the newer copy's), so an imported save shows its own date, not the file's.
- **Artwork:** Documents/<game>/artwork.jpg (JPEG 0.85, the long side at most 1500 px, drawn upright), aspect-fill over the
  gradient in the card and the showcase; Remove Game keeps it (it is not one of the ROM's files).
- **Text size:** the launcher's window sets its own preferredContentSizeCategory to the default (Large) on iOS 17+
  (`traitOverrides`); `maximumContentSizeCategory` on the window did not hold the labels back at the accessibility sizes in the
  Simulator. The test hook's `text=` sets the size on the scene, as the system setting does.
- **Measured** (iPhone 17 Pro Simulator, iOS 27.0, vcore, frame skip 4, 1-minute load 3-19 from other agents): the launcher at a
  first start came up after 3.9 s at low load and tens of seconds at high load (the first UIKit layout in a fresh process; a sample showed Swift
  protocol-conformance lookups on the main thread); after the game had shown the settings sheet, 0.5-0.9 s. Five resets
  through the launcher: 27-68 ms each, threads 48, 51, 48, 47, 45, heap 24.1-24.2 MB, resident 221-311 MB (the extracted
  files' page cache).

### Step 2 on the desktop as built (2026-11-05, macOS)

- **The window:** port/src/launcher_gui.cpp, the Games window in libntr's ImGui, with iOS's two layouts, actions, colours and
  test step names. Its frames are `SIM_LauncherFrame` (libntr-ios-launcher.patch): the window's events, the menu bar, the
  settings windows over the launcher, the present, a 60 Hz sleep. Nothing of the machine runs or is written (no interrupts, no
  V-blank count, no pad, no renderer, no sound step), so the snapshot of a first start, taken after the launcher, is the cold
  boot's. While it is up the Game menu has only Quit; the gear menu's Settings opens the Video, Audio, Controls and View windows.
- **Game > Games...** (macOS only: Linux and Windows still restart as a new process) asks ("Unsaved progress is lost"), then
  `bwport_press_games` (restart_press.c): `PORT_SetLauncherWanted(1)` and Restart to title's press; Restart to title clears the
  flag. cfgscript `games=1` is the same call.
- **Pickers:** libntr's ImGuiFileDialog, one dialog of the launcher's own (directories only for Export Save). It is the same on
  the three systems and works in hidden test runs; an NSOpenPanel needs a Dock icon and a modal run loop.
- **Images:** stb_image / stb_image_write (vendored, public domain) decode a JPEG or PNG and write artwork.jpg (quality 85, the
  long side at most 1500 px, a box filter; EXIF orientation is not applied: a phone photo taken on its side shows on its side).
  The banner icon (4x, nearest) and the showcase's gradient (made at run time for the showcase's size) are ImGui user textures
  (`RegisterUserTexture`): the Metal and the GL backends upload them alike.
- **Size:** the launcher follows the UI's scale (UiScale, 2.25 from this Mac's DPI) and shrinks it in a window under 780 x 560
  of it (800x600 points here), so cards and buttons fit; the library's cards stop at 300 points, the tabs' showcase takes 56 %.
- **Fonts:** the system's UI font read at run time (macOS Helvetica Neue regular and bold; Windows Segoe UI; Linux DejaVu Sans),
  never shipped; ImGui's own font when none is there.
- **Linux and Windows** (built, not run): the window and its first-start path are the same (non-Apple SIM_RunGame calls
  ChooseGame before the boot); their game's own reset sets BWPORT_RESET for the new process (reset.c), so it goes back to the
  title and not to the launcher. Games... needs their in-process restart (step 1-PC) or a flag passed to the new process.
- **Measured** (M2, hidden window, frame skip 4, BWPORT_FRAMERATE=unlimited, 1-minute load about 2-6): five rounds of Games...
  from the room, then Play: resets 29.7, 21.9, 21.8, 21.4, 25.0 ms, threads 39, 39, 38, 38, 38, heap 41.6-41.8 MB, resident
  366-395 MB; the room after the sixth boot. Export, import, restore and export again are byte-equal to the saves they came from.
  Screenshots of both layouts, the questions and the sizes: ~/bw-cache/launcher-shots-desktop/ (its README).
- **Windows and Linux (2026-11-05, wl90):** the window, the first-start path, the migration and the covers run on the win64 exe
  (built and run on Windows, hidden test steps and real clicks). (1) win64 links launcher_gui.cpp (meson: libstdc++.a first with
  -Bstatic, -lwinhttp, -lole32, -luuid; launcher_core.c leaves u8/u32 to libntr there). (2) The root is the working directory:
  `run\` next to the exe (bwport.bat no longer sets BWPORT_DIR, which means "one flat game, no launcher, no migration"; the
  bat still hands the exe the ROM beside it as BWPORT_ROM, which is how the migration finds Black's ROM, kept outside run\).
  The maintainer's run\ folder, copied, migrated as the design says (verified copy in backups\save-before-launcher.bin, save.bin
  sha1 unchanged, sim_config.ini byte-equal) and started Black with no launcher. (3) Windows pickers are the system's
  IFileOpenDialog (a folder picker for Export Save; BWPORT_PICKER=imgui keeps ImGuiFileDialog); a .nds dropped on the
  window imports (SDL_DROPFILE; libntr-pc-launcher.patch; the hook's `drop=PATH` is the same call). (4) PORT_HttpGet: WinHTTP
  (https only, 15 s in all, 10 MB, LC_USER_AGENT) and, on Linux, libcurl.so.4 opened with dlopen (no dev package; none
  installed: the gradient stays). BWPORT_LCTEST cover / gametdb / offline / covers=off pass on both; the Games window shows
  the downloaded cover on Windows. (5) Game > Games... on Linux crosses the restart's new process as BWPORT_GAMES, and the
  root as BWPORT_ROOT (the restarted process starts in the game's folder; LauncherStart went back to the root). **Windows:
  the item is left out until its restart is in the process**: reset.c's CreateProcess path calls exit(0), which is MSL's and
  returns, so the old process lives on and every later reset starts another (a Games... press made a runaway chain of
  processes). wr90's in-process restart removes that; then add `|| defined(_WIN32)` to the menu condition in
  libntr-pc-launcher.patch (the Windows side of PORT_SetLauncherWanted/BWPORT_GAMES then goes with the old path). Windows'
  plain "Restart to title" has the same latent bug today.
- **Not checked:** a .dsv import from the desktop window (the core's, tested by BWPORT_LCTEST), a visible window's real
  fullscreen, the pickers clicked by hand (the test hook calls the actions they call).

### Android launcher as built (2026-11-05, the x86_64 emulator)

- **Shape:** LauncherActivity (tools/android/app/src/main/java/com/bwport/app/) is the LAUNCHER activity, in the app's main process,
  over the C core through JNI (LauncherCore.java, port/src/android_launcher.c; the launcher loads libmain.so for the core only and never
  calls SDL_main). Play starts BwportActivity (SDL's) in its own process, `android:process=":game"`, with the string extra BWPORT_GAME.
  android_main.c then leaves BWPORT_DIR unset, makes the root (the app's external files dir) the working directory, and main_start.c's
  launcher start picks BWPORT_GAME's folder and enters it (sim_config.ini stays in the root). A start with BWPORT_DIR (the test tools,
  tools/android_run.sh) is the old flat run, unchanged.
- **Restart to title and Games (no in-process restart yet):** reset.c's Android branch calls `bwport_android_leave` (android_main.c) ->
  `BwportActivity.leaveGame`, which starts LauncherActivity (singleTask, so it comes to the front of the one task) and kills the :game
  process. Restart to title: the launcher gets `bwport_play=<code>` and starts the game again 0.7 s later in a new process (the game
  boots at its title: a clean memory, the same as the DS). Games (a Settings row, `bwport_press_games` first so "Games" waits for the
  game's own safe reset): the library. A run without BWPORT_GAME keeps the old message box and exit. When the in-process restart reaches
  Android, only `leaveGame` goes away.
- **Look:** the iOS launcher's two layouts. Library: cards (cover aspect-fit over a blurred copy of itself, banner icon, title, "Saved ...
  - N min played", Play/Continue, a ... menu) in a grid of 1-4 columns by width, then Add Game. Tabs: a chip per game, the page's showcase,
  Play, Save and More menus; side by side in landscape. Menu (top right): On Start Open (last game / this screen), Layout, Download
  Artwork, Settings. Views are built in code; targets are 48 dp; insets: system bars and the display cutout, edge to edge.
- **Add Game:** ACTION_OPEN_DOCUMENT; the ROM is read through the picker's descriptor (`/proc/self/fd/N`), so there is no second copy
  in the cache; LC_ImportRom copies, checks the sha1 and extracts, with the two progresses on a dialog. Export Save: the core writes
  to cache/export, then ACTION_CREATE_DOCUMENT; Import Save: copied from the picker into the cache, confirmed ("saved ..., N min
  played"), LC_ImportSave (a backup first). Set Artwork: ACTION_GET_CONTENT image, EXIF orientation, JPEG 85, long side 1500.
- **Covers:** `PORT_HttpGet` is android_launcher.c calling LauncherCore.httpGet (HttpURLConnection, https only, redirects by hand, 15 s, 10
  MB, LC_USER_AGENT); INTERNET permission. LauncherActivity.reload asks LC_CoverWanted per game and fetches on a thread. In the emulator
  GameTDB's coverHQ came through; `BWPORT_COVER_FAIL=offline` gives one log line (`cover: UnknownHostException ...`) and the gradient.
- **The update:** the old APK's flat files (ROM, save.bin, sim_config.ini, extracted files) are migrated by LC_Migrate at the first start of
  LauncherActivity. Checked against the released APK (android-latest): the save's and sim_config.ini's sha1s are unchanged, a
  save-before-launcher.bin backup is made, launcher.ini says LastGame=IRBO Start=last, and Start=last then goes straight into Black.
- **Test aids** (start-intent string extras on LauncherActivity; BWPORT_* ones also become variables of the launcher process; all are
  handed to the game): BWPORT_LAUNCHER=1 (stay on the library even with Start=last), BWPORT_IMPORT_TEST=PATH (Add Game without the picker),
  BWPORT_COVER_FAIL, BWPORT_KEYS ...
- **Owed:** Settings from the launcher (SettingsScreen talks to libntr's live config, which exists only in the :game process: the menu item
  says so; the game's own menu has the full screen); the in-process restart; arm64 on a phone; White and the later games (the table).

### Cover art (2026-11-05, macOS and the iOS Simulator)

The maintainer's decision: the launcher fetches each game's cover by itself, as TWiLight Menu++ (GameTDB) and RetroArch
(libretro-thumbnails) do. Nothing is shipped, bundled or committed: the image is fetched on the user's device into the game's
folder. The logic is C (port/src/launcher_cover.c); only the GET is per platform.

- **Sources:** (1) GameTDB by the header's code: `https://art.gametdb.com/ds/<kind>/<region>/<code>.jpg`, kinds largest first
  (coverHQ: 768x680 for IRBO; coverM 400x352; cover), the region from the code's last letter (O: US, then EN; E: US; P: EN; J: JA
  ...). (2) libretro-thumbnails, the "Nintendo - Nintendo DS" repository's Named_Boxarts, by the table's No-Intro name (Black:
  `Pokemon - Black Version (USA, Europe) (NDSi Enhanced)`, 512x461 PNG), through RetroArch's own server
  `https://thumbnails.libretro.com/Nintendo%20-%20Nintendo%20DS/Named_Boxarts/<name>.png` (its characters `&*/:`<>?\|"` become
  `_`, as the repository names files).
- **Their terms:** GameTDB's FAQ (Developers): "Can I use the database/artwork in my software? Yes, please add a link to GameTDB
  so that users know where to add new game entries and artwork. Please contact us to let us know about your project." No rate
  limit or user agent is asked for. The credit links to the game's GameTDB page; telling GameTDB about the project is the maintainer's
  call. libretro-thumbnails has no licence or usage terms (its README credits the contributors and says the art is the
  publishers'); the credit names it and links to the repository.
- **When:** each launcher reload asks `LC_CoverWanted`: the setting is on, the game has neither artwork.jpg nor cover.jpg, and it
  was not tried in this process. So a first start, an import and turning the setting on fetch, once per game per process; a
  failure (offline, 404) is one log line (`bwport: cover: ...`) and is tried again at the next start. The fetch runs on a
  thread (iOS: a dispatch queue; desktop: an SDL thread) and the launcher reloads when it lands. Download Cover Again (the game's
  menu) fetches whatever is there and replaces cover.jpg only; its failure is told.
- **Storage:** cover.jpg (decoded first: a body that is not a JPEG or PNG replaces nothing; then written as Set Artwork writes,
  JPEG 85, the long side at most 1500, `LC_WriteImage`) and cover.txt (the source and the credit's link) in the game's folder.
  Shown: the user's artwork.jpg, else cover.jpg, else the gradient. Set Artwork and Remove Artwork leave cover.jpg alone.
- **Setting:** launcher.ini `DownloadArtwork=on|off` (on by default), the gear menu's Download Artwork. Off: no request at all
  (LC_FetchCover refuses too).
- **Look:** covers are near square, the showcase and the cards are not: the cover aspect-fits with a margin and a shadow over
  its own copy, aspect-fill, blurred and darkened (iOS: a UIVisualEffectView; desktop: 48 px, three box blurs); in a card it
  stays above the info bar. The tabs' showcase has "Cover art: GameTDB" (or libretro) in its corner; the game's menu has
  "Cover art: <source>", which opens the link.
- **Network:** `PORT_HttpGet` (launcher_core.h): macOS the SDK's libcurl (http_get.c, -lcurl), iOS NSURLSession (ios_http.m,
  an ephemeral session); https only (redirects too), 15 s, 10 MB, `LC_USER_AGENT`. Windows, Linux and Android: a stub ("not
  supported yet"; owed, the maintainers' notes).
- **Tests:** BWPORT_LCTEST `cover`, `cover=CODE` (a code not in the table: three 404s, quiet), `covers=off`;
  BWPORT_COVER_FAIL=gametdb (GameTDB skipped: the libretro fallback) or =offline (every host `offline.invalid`, which does not
  resolve: no answer, nothing more tried). The launchers' test hooks: `cover`, `covers=on|off`, and `idle` waits for fetches.
  Screenshots: ~/bw-cache/launcher-shots-covers/ (its README; they hold the downloaded cover: never commit or upload).

### Skip title and menu (2026-11-06, macOS and the iOS Simulator; Black and White)

- **What it does:** with launcher.ini's `SkipMenu=on` (off by default; the desktop gear menu's and the iOS launcher menu's
  "Skip Title and Menu"; `BWPORT_SKIPMENU=1|0` over it for tests) a start goes into the save as CONTINUE does: no logos,
  opening, title or menu. The log says `bwport: skipmenu: CONTINUE at V-blank 1, no title or menu`. With no save, or a save
  the menu would show a message about (erased blocks), the boot is the game's own and the log says why.
- **Where it hooks:** port/extern/src/system/boot_proc.c, the stand-in of src/system/boot_proc.c: its own `data_020a0480`
  (the boot process main.c queues) whose tick, at state 0 of a marked boot, queues CONTINUE's process
  (`data_ov088_021bd824`, src/ov088/game_start.c) as the start menu's exit does (func_ov088_021ba014), with what the menu
  does on the way: the Dream World mark of its start (func_ov088_021ba114), func_020274c8 of its exit, and the C-Gear
  question answered as the save left it (C-Gear power in the options; no C-Gear: off, as the menu does without asking). The
  season screen and the field's fade-in are CONTINUE's own and stay. port/src/skip_menu.c decides: once per start of the
  process, and again at each `PORT_EnterGame` (the launcher's Play); a Restart to title or the soft reset
  (L+R+START+SELECT) goes to the title, which is the way to NEW GAME and the other items (a reset in a new process,
  reset.c's BWPORT_RESET, counts as a reset).
- **The field's first frame:** port/extern/src/system/game_event.c logs `bwport: field: up at V-blank N (no event running)`
  once a boot: the first frame the player can move (the room save: 217 with the skip, 1243 through the title's keys).
- **Checked (M2):** the room save, the same frame-counted keys after the field (offset 1026 frames) through the field menu's
  Save: the skip's save and the menu path's differ in the same 30 bytes as two menu-path saves (wall-clock fields and
  their checksums). The town: the same scene, its music's envelope correlates 0.92 with the menu path's (the title music:
  0.45). Restart to title after the skip: the opening and the title. The menu works after the skip.
- **Launch to the field (room save, no frame skip, wall from the exe's start to the field's frame):** the default settings at
  60 fps 21.1 s -> 4.0 s, unlimited 5.8 s -> 1.3 s (load 3-4); the maintainer's (gpu 3x, AA 4, smooth, monitor) 25.0-25.3 s ->
  6.7-8.8 s (load 10-13). Black's room at 60 fps: 7.50 / 8.05 / 8.31 M instructions a drawn frame before and 7.51 / 8.07 /
  8.32 after (two rounds each, the title's keys: the same path), unchanged.
- **iOS Simulator:** a fresh install, the launcher's test steps import the ROM and the room save, skipmenu=on, Play: the
  skip and the field at V-blank 217; a relaunch (Start=last) skips again into the room. White (macOS, its room save in a
  launcher root): the skip and the field at V-blank 220.
- **The PC builds (Windows, Linux, Android):** the stand-in, skip_menu.c and launcher.ini's key are shared and compile there
  already; what is owed: the menu item in the Windows/Linux Games window comes with launcher_gui.cpp as it is; Android needs
  a switch in its launcher settings (android_launcher.c's get/set, which now has Start, Layout and DownloadArtwork) calling
  LC_SkipMenu / LC_SetSkipMenu. Android's reset starts a new process (BWPORT_RESET): skip_menu.c does not skip there, and
  Android's Play is a new process too, so it skips on every start of the app; Games... on Linux as a new process
  (BWPORT_GAMES) does not skip at its Play.
- **Black 2 / White 2 (2026-11-06, macOS):** port/subprojects/pokebw2-skipmenu.patch, in pokebw2's src/system/boot_proc.c
  (Black 2 and White 2 share it): under SDK_PORT `StartMenuEventTick`'s state 0 asks the same `PORT_SkipMenu` (skip_menu.c
  is per process, not per game: nothing in it changed) and, with a save the start menu would offer CONTINUE for and show no
  message about (`SaveControl_IsDataAlreadyPresent`, `SaveControl_GetErrors` == 0 as `StartMenu_CheckBrokenSave`, and no
  Key System notice pending: `TrainerGameInfo_GetKeyNotice`, which the menu shows before its items), queues ov162's
  `CONTINUE_PROC_FUNCTIONS` with what the menu does on the way: `StartMenu_Setup`'s Dream World mark, `StartMenu_Exit`'s
  `GameBeaconSys_CheckGenuineCard()`, and `StartMenu_Continue`'s C-Gear question answered as the save left it (C-Gear on and
  wireless on: CONTINUE_CGEAR_ON; otherwise off, which is where the menu ends up without asking or after its "wireless is
  off" message). The menu's Key System data (`KeySystemData_GetKeyInfo`) is only shown there; Memory Link is not on
  CONTINUE's path.
  Checked (M2, b2-pc): the field at V-blank 219 (the title's keys: 2545); the field menu's Save with the same keys after the
  field (offset 2326 frames), twice each way: every byte where a skip save and a menu save differ also differs between
  two saves of the same path (54-66 bytes: the wall-clock fields and their checksums). The soft reset after the skip: the
  boot screens, no second skip. An all-0xFF save: no skip ("no save to continue"), the title. A save with a byte flipped
  every 4 KB in both copies: no skip, and the menu path shows "The save file will be..." there. tools/mac_run.sh --root
  --save b2-NAME | w2-NAME puts the save in the root's "Pokemon Black 2" / "Pokemon White 2" folder (--store reads it
  there) and skips, so no title keys. White 2 (w2-room): the skip and the field at V-blank 220.

### Black 2 and White 2 in the launcher (2026-11-06, macOS and the iOS Simulator)

- **Save layout (from pokebw2's C, src/system/save_table.c, src/gfl/backup_system.c, include/save/playtime.h; checked on
  b2-room, b2-town, b2-pc and w2-room):** the card is 512 KB as before. The save is 0x26000 bytes, not Black's 0x24000: the
  main copy at 0, the backup copy at 0x26000. The footer is the same 16 bytes (count, size, SAVE_MAGIC 0x31053527, a
  CRC) and ends the copy: the size field is the footer's own end, 0x25fa4 (Black and White: 0x23f9c). Blocks are
  256-aligned (block, then a 4-byte count and CRC footer, rounded up to 0x100). The play time and the date of the last save
  are the trainer data block's PlayTime (hours u16, minutes, seconds, then year:7 month:4 day:5 hour:5 minute:6), which sits at
  0x19400 + 0x24 in each copy for all four games (found by scanning the four saves for it; the same offset as Black's).
- **The checks:** `LC_CheckSave` and `LC_SaveFileInfo` take the game (LC_Game's saveEnd and saveCopy). The footer is read at its
  one place in each copy (the old scan of every offset could not tell Black's save from Black 2's: Black's footer is also
  "a footer ending at its own size"). A save of another game in the table is refused by name: "This is a Pokemon Black save,
  not a Pokemon Black 2 save." Black and White share a layout, as do Black 2 and White 2, so a White save in Black's folder
  (or White 2's in Black 2's) passes, as before; the games' own checks do not tell them apart either. `LC_CheckSaveFile` does
  the same for a path, a .dsv included (the UIs use it, so a Black .dsv offered to Black 2 is refused with the same message).
  Android's save summary of a file to import passes no game (the first whose footer fits).
- **Tested (macOS, lctest, game=IREO / IRDO / IRBO / IRAO):** b2-room, b2-town, b2-pc into Black 2: accepted; play time and
  date read (0:00:22 / 0:00:44 / 0:02:10, 2026-11-06); a backup taken before each import (backups/save-...-before.bin);
  export, list. w2-room into White 2: accepted (0:00:22). A Black save (.bin and .dsv) into Black 2: refused as above; a
  Black 2 save into Black or White: "This is a Pokemon Black 2 save, not a Pokemon Black save." A .dsv of b2-town (the save,
  the snip line, the footer) imported for Black 2, play time and date read through it. Black's and White's saves still pass
  with the same play times as before.
- **Switching (macOS, one process, four games from their saves, `game=next;play` and cfgscript `games=1`, two full rounds
  Black, White, Black 2, White 2, Black, ...; the table's order):** seven resets, 9.7-20.7 ms, no CRASH or HANG in the log.
  Black and White with Skip Title and Menu came up in their rooms (V-blank 217 and 220); Black 2 and White 2 have no skip yet, so
  their keys (BWPORT_KEYS, which count V-blanks from each boot) took them through the title to their rooms. Screenshots
  compared by pixels (top screen, 4 px grid): the same game's two rounds are identical (mean absolute difference 0.0), the
  four games differ (42 to 111); Black 2 and White 2 rooms are the same scene (the saves are twins), so their title
  screens were shot too (the POKEMON BLACK 2 / WHITE 2 logos). tools/mac_reset_check.py over three boots of each game
  (BWPORT_MACHINE_CHECK): Black 2 and White 2 differ from their cold boot in machbss only, from V-blank 0 (Black: machbss
  and ovlbss from 13; the unpinned clock and the host's pointers, as before): their data and bss are in the restart snapshot.
- **iOS Simulator (`BW_SIM_FILES` in tools/ios_sim_run.sh puts the other three ROMs and the saves into tmp/; `--fresh`):**
  the launcher's own steps imported the four ROMs (each extracted; sha1 checked) and their saves (the import question,
  `alert=ok`), a .dsv of b2-town over Black 2's save, and a Black .dsv offered to Black 2 (the note names the game; the
  step `alert=ok` now closes a note too). The covers came from GameTDB for all four (coverHQ, 768x680; White 2's first try
  timed out in one run and "Download Cover Again" got it). Each game's page and the card grid were shot: Black 2 dark with
  gold, White 2 light with pink, each showing its cover, "Saved ... 0m played" from its save, Continue. Then
  Play with `game=next` and cfgscript `restart=1`: Black, White (field at V-blank 217 and 220 with Skip Title and Menu),
  Black 2 (its town save, through the title by keys), White 2 (its room), Black again; resets 28-59 ms (the Simulator at
  load 9-13), 44-46 threads, 26-27 MB heap, no CRASH or HANG; the first Black and the last differ by 0.0 (mean absolute
  pixel difference of the whole screen), the four games by 12.6-38.3. A first run died silently 180 s in, after the
  last import and before its first Play, with a note still on screen from the refused import (nothing in the log; it did
  not come back in two more runs): look there if it comes back. Screenshots (they hold the covers: never commit or
  upload): ~/bw-cache/launcher-shots-b2w2/ (its README).
- **Covers:** GameTDB has IREO and IRDO (art.gametdb.com/ds/coverHQ/US/IREO.jpg 235 KB, IRDO.jpg 214 KB; EN and the
  smaller sizes too), and libretro-thumbnails has `Pokemon - Black Version 2 (USA, Europe) (NDSi Enhanced).png` (416 KB) and
  the White 2 one (447 KB): the table's codes and names were right as they were.

## 4. The ROM library

**Where things live.** Each platform's root R stays what it is today: iOS Documents (visible in Files) plus Library/
Application Support/bwport (hidden); the Android app's external files directory; Windows `run\` next to the exe; the
working directory on Linux and macOS.

```
R/sim_config.ini                 global settings, unchanged, not moved
R/launcher.ini                   LastGame=IRBO (the launcher's only state)
R/Pokemon Black/                 one folder per game, named from the table
    Pokemon Black.nds            the imported copy
    save.bin                     the 512 KB flash image the game writes
    backups/                     save-YYYY-MM-DD-HHMMSS.bin
    (desktop, Android: the extracted files here too: a/, header.bin, ... ; this is the game's working directory)
iOS: Library/Application Support/bwport/Pokemon Black/   the extracted files (hidden, as today), the game's working directory
```

The game's working directory is its folder. libntr finds header.bin, the ROM's files and save.bin relative to it.
iOS's save.bin stays visible in Documents through the symlink trick main_start.c uses today, done per game. The libntr
patch makes sim_config.ini's and imgui.ini's paths absolute (taken at start), so a chdir into a game doesn't move them.

**Import.** Pick a file. The core reads the header: an unsupported game code is refused, naming the title and code.
It copies the file into the game's folder while hashing, as ios_import.m does today. A wrong sha1 is refused and the
copy deleted. Then it extracts with the existing progress screen. A failed import leaves nothing behind.
`BWPORT_ROM`, and a .nds in R on the desktop, import by themselves (the test tools rely on that). `BWPORT_DIR` keeps
today's meaning exactly: one game, a flat folder, no launcher, no migration. port_run.py, mac_run.sh and the lockstep
runs are unchanged.

**Removal.** Deletes the ROM copy and the extracted files, by the ROM's own file list and never by wildcard. It keeps
save.bin and backups/: a later import of the same game finds them. Deleting a save is a separate action, and a backup
is made first.

## 5. Saves

- **Format.** save.bin is a raw 524,288-byte image (CARD_BACKUP_TYPE_FLASH_4MBITS_EX, docs/port-runtime.md "The
  save"). That is what melonDS and flash carts use, so export is a copy named "Pokemon Black.sav".
- **Import accepts:** exactly 524,288 bytes; or a DeSmuME .dsv, recognized by its trailing `|-DESMUME SAVE-|` and its
  `|<--Snip above here to create a raw sav ...` marker, cut at the marker, with exactly 524,288 bytes before it.
  Anything else is refused, with its size. The content check is the game's own footer (lib/gfl/src/backup_system.c,
  save_table.c): a 16-byte footer with magic 0x31053527 at +8 and a size field equal to the footer's own end, in the
  main copy (0 to 0x24000) or the backup copy (0x24000 to 0x48000). An all-0xFF file is accepted as "a new game".
  Black and White saves share the layout; Black 2 and White 2 have their own (0x26000 on; "Black 2 and White 2 in the
  launcher"), and a save of the wrong generation is refused by name.
- **Backups.** A backup of the current save.bin is written to backups/ and read back before every import, restore or
  save deletion (the last 20 of those are kept). On top, tiered copies of the save as the game left it: one a day for a
  week, one a week for a month, one a month for a year, none older; plus the migration's, never deleted (the maintainer's
  decisions below).
- **Slots.** Recommendation: none. A DS cartridge has one save. Backups with Restore already cover "keep another save",
  and slots can come when a real use shows up.
- **The game's writes** don't change: it opens and closes save.bin on every backup call. Import and restore happen only
  in the launcher, with no game running, so a running game can't write its RAM copy over an imported save.

## 6. Settings and the migration

**All global, none per game.** Everything in sim_config.ini describes the device or the maintainer's taste: renderer,
resolution, antialiasing, pacing, the speed options, sound, keys, layout. The DS's firmware settings (nickname,
language, MAC address) belong to the one DS all the games share, as on hardware. The speed options mean the same in
White. Per game: the ROM, the save, backups, the extracted files. YAGNI: a per-game settings file waits for a real use.
sim_config.ini keeps its path and format, and the launcher never writes it. Its own state goes in launcher.ini.

**The migration.** It runs once, at a root that has save.bin or header.bin at its top and no game folders, and never
when `BWPORT_DIR` is set.

1. Find Black's ROM in R by sha1: on iOS it is "Pokemon Black.nds" in Documents; on desktop and Android, any .nds in R.
2. Make R/Pokemon Black/backups/. Copy save.bin to backups/save-before-launcher.bin, fsync it, read it back and compare
   the sha1.
3. Rename save.bin and the ROM into R/Pokemon Black/. A rename stays on the same volume, so the bytes don't change.
   Move the extracted files the same way by name (the ROM's FNT root entries plus the extractor's header.bin, arm9.bin,
   banner.bin, overlay9/, overlay7/). If any are missing, extract again from the ROM. iOS: drop the old symlinks,
   which are links and not data.
4. Write `LastGame=IRBO`. sim_config.ini and imgui.ini are not touched.
5. Nothing is ever overwritten: a target that already exists stops the migration. If any step fails, everything stays
   where it was, Black runs from the flat layout this time, and the log says why.

The first start after the update then goes straight into Black, with no launcher. Its save has the same sha1, its
settings file is byte-identical, and the boot is exactly as before. Tested on copies first: a pre-launcher macOS run
folder and a Simulator container from today's IPA. The checks are the save's and sim_config.ini's sha1s before and
after, and a lockstep record at the title equal to the pre-update build's. Only then does it run on the phone.

## 7. Phases

| Step | Where | What | Test |
| --- | --- | --- | --- |
| 1. In-process restart | Mac: macOS, then iOS | The run loop, machine.c, the reset patch, main_threads.c parking, the bridges' reset. Restart to title returns to the title on the phone. reset.c's exec and CreateProcess go once T1-T3 pass on that platform. Built 2026-11-05 ("Step 1 as built"): T1-T3 pass on macOS, exec gone there; T4 (the phone) is the main session's. | T1-T4 (section 1) |
| 1-PC | PC: Linux, Windows, Android | The same code: the linker-script range, mingw's longjmp, Android's SDL thread | T1-T3 on Linux and Windows; T2-T3 in the Android emulator; then the device |
| 2. Library and launcher | Mac: the C core, the ImGui window (macOS), UIKit (iOS) | Per-game folders, the migration, ROM import and validation, saves, backups, return to the launcher | Migration on copies (section 6); ROM cases (full, trimmed, one byte off, another code, not a ROM); save cases (round trip byte-equal, .dsv, wrong size, all 0xFF, a backup before each); T1 through the launcher |
| 2-PC | PC | ImGui on Windows and Linux; LauncherScreen.java; the migration of the maintainer's `run\` (on a copy first) | The same list |
| 3. White | either | The White build in the same binary: the shared SDK files, White's table row and sha1, its PortGame table (w_bwgame). Linux, Windows, Android: the rename header (2026-11-05); macOS, iOS: one relocatable object, no rename header (2026-11-06); both "Step 3 as built" | Link with no duplicates; T1 for each game; switching Black to White to Black with T1 equal each time |
| 4. Black 2 / White 2 | later | pokebw2's tree as games 3 and 4 | as step 3 |

The smallest useful step is 1 on the Mac: the maintainer's own request (a real Restart to title on the phones). The
launcher's restart and game switching need exactly this, and it ships with no visible change elsewhere. Step 1-PC is a
scheduled follow-up, not optional (platform parity), and goes on the maintainers' notes's "Owed" list when step 1
merges.

## The maintainer's decisions (2026-11-04, answered)

| Decision | The maintainer's answer |
| --- | --- |
| What a start shows | **Configurable**: the last game straight away, or the launcher. A first start with no game shows the launcher. |
| The launcher's look | **A clean, minimalist, modern launcher, not one row per game.** Two layouts to build and show the maintainer: (a) a library of every imported game; (b) a top row with a tab per game, each tab styled a little for its game. A game's showcase may feature its box legendary in 3D: this comes only from the game's own data in the user's ROM at run time (for example its title screen's scene), never art we draw, make or ship. **Landscape and portrait** both. |
| Save slots | None. Backups with Restore. |
| Backups | **Tiered:** one a day for a week, then one a week for a month, then one a month for a year, none older. In addition: the last 20 (one before every import, restore or deletion) and the migration's copy, kept for good. |
| The migration | Yes: move Black's files into its folder with a verified copy (section 6). |
| Keep the ROM copy after extraction | Yes for now (simpler). Later, libntr's FS reading straight from the ROM would drop the extracted copy (about 196 MB a game). |
| DeSmuME .dsv | Accept it by cutting its footer. |
| Restart on the desktop | The in-process path everywhere; exec and CreateProcess removed once it passes there. |

## Not checked

- (Checked in step 1: a zero-fill custom section works in ld64; libsim's mixed globals hold almost only host state:
  "Step 1 as built".)
- libntr's 3D command processor's state: only the walled-off agent may look.
- How the 74 host-resource sites in the SDK libraries behave across a reset: each is read in step 1.
- White's code differences beyond the decompilation's (`#if GAME_VERSION`): none needed in the port's stand-ins ("Step 3 as built"); Black 2 and White 2's codes and sha1s need their dumps.
- (`-fno-common` across libntr's, libntrsystem's and the game's C: no duplicate, it links.)
