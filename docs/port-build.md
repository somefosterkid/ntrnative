# The PC build (port phase)

The decompiled C built as a PC program on [libntr](https://github.com/cybervisi0n/libntr) (MIT, NitroSDK/TwlSDK for
PC) and libntrsystem (NitroSystem), for Linux x86_64 and win64 (mingw-w64, cross from WSL). docs/libntr/*.md has
how libntr itself builds and what it covers. Measured 2026-10-28 on main ea131eb9a plus this branch: every game file
the build takes compiles on both targets, both link (with generated stubs for what is still undefined), and both
start: libntr opens its window, calls the game's NitroMain, and SystemInit runs until its first undefined function.

## Commands

From the repo root, in WSL (meson >= 1.3, ninja, clang, g++, SDL2 and enet: docs/libntr/build.md "Dependencies").
Run the ROM build's `ninja` first: the C includes `asset/<name>.inc`, which it writes to build/asset from the base ROM.

    meson setup build/port/linux port --native-file port/machine/linux.ini
    ninja -C build/port/linux bwport                    # 1525 steps, 35 s on the worker slice
    ninja -C build/port/linux undefined                 # build/port/undefined.txt
    cd <scratch> && SDL_AUDIODRIVER=dummy timeout -s KILL 60 xvfb-run -a <repo>/build/port/linux/bwport
                                                        # or without xvfb-run for WSLg; libntr writes sim_config.ini,
                                                        # imgui.ini, DbgLog.txt in the cwd

    meson setup build/port/win64 port --native-file port/machine/win64.ini -Dbuild_target=win64
    ninja -C build/port/win64 bwport.exe                # name the target: a plain `ninja` stops at libntr's ARM7
                                                        # component (makelcf is a PE file there; build.md "win64")

To run bwport.exe on Windows: tools/port_win64.sh (below, "win64").

After adding or removing a C file, `meson setup --reconfigure build/port/<target> port` (meson doesn't watch
directories; tools/port_run.py does it for Linux).
`python3 tools/port_errors.py <ninja -k 0 log>` groups a build's compile errors by cause.

## Running

`tools/port_run.py` builds the Linux exe, runs it headless and says in one line how far it got (the last line it
prints, also in `<out>/verdict.txt`):

    python3 tools/port_run.py                               # build, run 30 s, a screenshot every 5 s
    python3 tools/port_run.py -t 120 --shots 2 --keys 'start@f400 a@f700 a*0.5@20'
    python3 tools/port_run.py --no-build --fresh -t 10      # the exe as it is; no save.bin, sim_config.ini, imgui.ini

| verdict | when |
|---|---|
| `reached-frame N in T s (F fps)` | still running at the end, frames advancing |
| `hang in F (file:line) [... G (file:line)], frame N (no frame for S s)` | running, but no frame for `--hang` seconds (5): the main thread's innermost frame with source; `... before the first frame` |
| `stub X called from F (file:line), frame N` | an abort stub (build/port/undefined.txt) |
| `crash SIGSEGV in F (file:line) <- G <- H [... I (file:line)], frame N` | a signal: the innermost three frames with source (game, port/src, libntr) |
| `exit N at frame M` | the exe ended by itself |
| `build failed` | meson's or ninja's error (its tail is printed) |

`... I (file:line)` ends a hang's or a crash's place when the innermost game frame (src/, lib/) isn't one of those:
whose call it was, when the place is in libntr or port/src.

- **Build**: `ninja -C build/port/linux bwport`; first `meson setup` when there is no build dir, `meson setup
  --reconfigure` when port/src or port/exclude.txt changed (a file added or taken out), and `meson subprojects update
  --reset` before that when a wrap or a packagefiles patch changed (meson applies a wrap's diff_files to a new
  checkout only; this applies them again, and everything rebuilds: about 1.5 min). On the worker slice. tools/port_wraps.py does
  this for both port_run.py and `port_win64.sh` (the two build dirs share the checkouts in port/subprojects/).
- **Run**: an Xvfb display of its own (the first free one from :300; the harness needs it for the screenshots and
  keys), the exe under gdb in the run dir (`--dir`, default build/port/run, kept between runs: the ROM is extracted
  once, save.bin stays; `--fresh` removes save.bin, sim_config.ini and imgui.ini), `BWPORT_ROM` (the environment's,
  else orig/baserom_b_us.nds of the worktree or of the main checkout), `SDL_AUDIODRIVER=dummy`. For `-t` seconds (30)
  or until it stops; at the end gdb stops it (SIGINT; SIGSTOP if that doesn't arrive) for the backtraces.
- **Output** (`--out`, default `<run dir>/out`, emptied each run): log.txt (the exe's output, the keys sent, gdb's
  stop, `bt` and `thread apply all bt`), shots/`<seconds>s_f<frame>.png` every `--shots` seconds (5; 0: none) and
  final.png (the window when it stopped: both screens as libntr draws them, 512x768 at the scale 2 of main_start.c's
  sim_config.ini), verdict.txt.
- **Frames**: the V-blank count, OS_GetVBlankCount's word, which libntr's frame adds 1 to after the V-blank handler,
  read from outside through /proc/<pid>/mem: port/src/run_frames.c exports its address (`bwport_vblank_count`). No
  breakpoint, nothing in the frame. A frame is one `OS_WaitIrq(TRUE, OS_IE_V_BLANK)` of the game's loop, 60 a second
  at most (libntr's cap).
- **Keys** (`--keys`, xdotool): `KEY[*HOLD]@WHEN`, separated by spaces or commas. KEY: a b x y l r start select up
  down left right, sent as the run dir's sim_config.ini [PAD] keys (main_start.c's: A X, B Z, X S, Y A, L Q, R W,
  Start Enter, Select right Shift, the arrows). WHEN: seconds after the exe started, or `fN` at frame N (better: it
  doesn't depend on how long the start takes on a loaded machine). HOLD: seconds down (0.1: libntr reads the keys once
  a frame). The window gets the focus first (there is no window manager). The touch screen isn't scripted.
- **--fast**: the run dir's sim_config.ini `FrameRate=unlimited` (60 without it; the old `CapFrameRate=false` still loads), so libntr doesn't hold a frame to
  1/60 s: about 210 frames a second here, the field in under 2 minutes. Give key times in frames (`fN`); keys are
  held 0.03 s by default. `--fresh` keeps sim_config.ini with it (main_start.c writes a capped one when there is none).
- **--gdb CMD** (repeatable): a gdb command before `run`, e.g. `--gdb 'break exit'` or `--gdb 'source FILE'` with
  `dprintf file.c:LINE,"fmt
",args` or a breakpoint whose commands print and `continue` (`catch signal SIGSEGV` with
  commands that print the faulting frame's locals); the output is in log.txt before `== port_run: stop`.
- Exit code 0 when the exe was still running at the end (or exited 0), else 1. Runs in different run dirs can go at
  the same time. Needs Xvfb, xdotool, xwd (x11-apps) and gdb; the PNGs are Python's (stdlib) from xwd's dump. Linux
  only (bwport.exe: "win64" below, tools/port_win64.sh run, BWPORT_KEYS).

Measured 2026-10-28 (a loaded host, load average 25-29):

- main 9cf985898 with this tool: `crash SIGSEGV in OS_CreateExtraHeap (os_alloc.c:709) <- GFLAppInit (gfl_use.c:112)
  <- NitroMain (main.c:59), frame 0`, 1.7 s for the whole run when the build is up to date.
- A trial merge of b44a d4c4b9fc3 (the extra heap in s_HW_MAIN_MEM, the overlay model), b44c 71d54f16f and b44d
  1a1ba7a01 (the heaps twice their size on PC): `crash SIGSEGV in NNSi_FndInitHeapHead (heapcommon.c:62) <-
  InitExpHeap (expheap.c:213) <- NNS_FndCreateExpHeapEx (expheap.c:627) ... GFL_HeapAdd (heapsys.c:159), frame 0`:
  gfl_use.c's OS_AllocFromHeap of WORK_AREA_SIZE(EXTRA_HEAP_SIZE), 0x7e00 on PC, from an extra heap of
  HW_MAIN_MEM_PARAMETER_BUF_SIZE (0x4000) returns NULL, and heap 9 is made at address 0.
- A test loop in NitroMain's place (scratch only: the backdrop colour from A and Start, a spin on Select, an undefined
  function on B, exit(3) on X): `reached-frame 464 in 8 s (59 fps)`; the keys at their frames, their colours in the
  next screenshot; `hang in HarnessTest (main.c:75), frame 604 (no frame for 5 s)`; `stub BackupMail called from
  HarnessTest (main.c:76), frame 124`; `exit 3 at frame 69`.

## win64: bwport.exe in a Windows folder (tools/port_win64.sh)

    tools/port_win64.sh                 # BUILD ONLY (build/port/win64/bwport.exe); nothing is deployed
    tools/port_win64.sh deploy          # build and fill C:\bwport, the deploy folder (--out DIR for another folder)
    tools/win_smoke.sh --save room 30   # the exe HIDDEN from a save for 30 s in C:\bwsmoke: window check, shots, killed (below)
    tools/port_win64.sh run 200 --shots 10    # run it from WSL for 200 s, a screenshot every 10 s
    SDL_AUDIODRIVER=dummy BWPORT_KEYS="start@112 a@124" tools/port_win64.sh run 200 --shots 10
    tools/port_win64.sh gdb -batch -ex c -ex bt   # under gdb (Windows' gdbserver, gdb in WSL)
    tools/port_win64.sh symbolize LOG   # a crash report's bwport.exe+0x... frames as function and line
    tools/port_win64.sh layout          # the linux and win64 builds' struct layouts compared (tools/port_layout.py)

The folder (C:\bwport) gets bwport.exe; SDL2.dll and enet.dll (<scratch>, docs/libntr/build.md "win64"
says how they were made) and libwinpthread-1.dll (/usr/x86_64-w64-mingw32/lib); Mesa's software OpenGL in the subfolder
**mesa\** (opengl32.dll and libgallium_wgl.dll from <scratch>; `--no-mesa` leaves it out; "Which OpenGL"
below says when the exe loads it); bwport.bat; and the base ROM (copied once from the main checkout's
orig/). `--also DIR` (repeatable) deploys the exe, DLLs, mesa\ and bwport.bat to a second folder too, never its run\ or
its ROM (the maintainer's own folder is only ever filled when he asks; stop the game there first, a running exe is locked);
a deploy also removes an older top-level opengl32.dll / libgallium_wgl.dll (they would override the system's GL) and
stops only the bwport.exe running from the deployed folder. On Windows: double-click **bwport.bat**, or
`bwport.bat D:\path\to\rom.nds`. It takes the ROM from its
argument, else BWPORT_ROM, else the first .nds in its folder; extracts the ROM's files into run\ at the first start (a
few seconds); runs the exe in run\ (save.bin, sim_config.ini with the keys, DbgLog.txt are there), and on a
non-zero exit shows the code and waits for a key. Keys: A=X B=Z X=S Y=A L=Q R=W, Start=Enter, Select=right Shift, the
arrows; the mouse on the bottom screen is the touch screen; ` the debug window, Tab Frame rate Unlimited and back.

### Which OpenGL (port/src/gl_select.c, libntr-glselect.patch)

libntr needs a 4.2 core context for its window and Renderer=gpu runs on it. The exe uses the **system's** GL (the graphics
driver's opengl32.dll) and loads the bundled Mesa (mesa\opengl32.dll, llvmpipe, software) only when it must:

1. Before the window is created, SIM_RenderInit calls the hook: a probe (a hidden window, a 4.2 core context with libntr's
   attributes, GL_RENDERER / GL_VERSION, 26 GL functions port/gpu loads) judges the system's GL.
2. Usable and hardware: the system's GL stays. Not usable (no 4.2 core context: Windows' own "GDI Generic" 1.1; GL below
   4.2; a function missing) or software (llvmpipe, softpipe, "Microsoft Basic Render Driver", GDI Generic ...) while mesa\
   exists: `SDL_GL_LoadLibrary("<exe folder>\mesa\opengl32.dll")` before libntr's window (the folder is added to the DLL
   search path for libgallium_wgl.dll), GALLIUM_DRIVER=llvmpipe and, for Renderer=vcore, LP_NUM_THREADS=min(4, CPUs) (a user's
   own value stands) set then: the llvmpipe tuning is Mesa's software GL's only. With no mesa\ and no usable GL: a message box
   and exit 1, never a crash or a black window.
3. `BWPORT_GL=system` takes the system's GL whatever it is (a software one too; a GL that can't make a context at all still
   goes to Mesa), `BWPORT_GL=mesa` the bundled one. Linux: always the system's (Mesa or the vendor driver); only the report
   and LP_NUM_THREADS apply.
4. `int SIM_GLIsHardware(void)` (1 = a real GPU driver; 0 = software GL or none) and `const char *SIM_GLRendererName(void)`
   (GL_RENDERER) answer for the Renderer=auto choice and the status line; valid from the first call on (they probe when
   asked early), from the live context after the window exists.

What the log (run\bwport.log, stdout) says, at the start:

    bwport: GL: system: NVIDIA GeForce RTX 3060/PCIe/SSE2, 4.6.0 NVIDIA 552.22 (NVIDIA Corporation)     <- the system's probe
    bwport: GL: system: NVIDIA GeForce RTX 3060/PCIe/SSE2, 4.6.0 NVIDIA 552.22 (NVIDIA Corporation): hardware   <- the live context

and on a machine without a GPU driver (this host: the VM's GDI Generic):

    bwport: GL: the system's GL is not usable: GL 1.1.0 (4.2 core needed) (GDI Generic, 1.1.0)
    bwport: GL: loading the bundled Mesa (C:\...\mesa\opengl32.dll)
    bwport: GL: bundled Mesa: llvmpipe (LLVM 22.1.8, 256 bits), 4.6 (Core Profile) Mesa 26.2.4 (git-9a0169f2a7) (Mesa)
    bwport: GL: bundled Mesa: llvmpipe (...), 4.6 (Core Profile) Mesa 26.2.4 (git-9a0169f2a7) (Mesa): software

**Maintainer's test on a machine with a GPU**: copy the folder `tools/port_win64.sh` makes (C:\bwport, with its mesa\ subfolder),
double-click bwport.bat. (1) The log's first GL lines name the GPU and say `hardware`, no "bundled Mesa" line; Task Manager's
GPU engine shows 3D load; no GALLIUM_DRIVER / LP_NUM_THREADS in the exe's environment. (2) Renderer=gpu with Resolution 4:
the log's `Renderer=gpu: <your GPU>, <GL version>`, the picture at 4x. (3) `set BWPORT_GL=mesa` then the bat: the log says
BWPORT_GL=mesa and llvmpipe, and the game is slower. (4) If the first lines say the system's GL is not usable, the reason
is on that line (an old driver: GL below 4.2; a remote desktop session: GDI Generic): update the driver or run in a local
session; the game still runs on Mesa meanwhile.

What is win64's own (the rest is the same C and libntr as Linux):
- **port/src/win64_console.c**: stdout and stderr unbuffered (Windows buffers a stream that goes to a file or a pipe:
  a crash or a stub's abort() lost the log, and the first runs printed nothing); a crash report on stderr in place of
  gdb (the exception, the address read or written, the stack walked with x64's unwind tables as module+offset;
  `symbolize`, which `run` applies, adds function and line); an undefined function's stub prints its caller the same
  way; BWPORT_TRACE_EXIT=1 prints the stack of whoever calls exit() (GFL_HeapDumpOnFailure's exit(1) printed nothing).
- **gdb**: `apt install gdb-mingw-w64 gdb-mingw-w64-target` (x86_64-w64-mingw32-gdb, /usr/share/win64/gdbserver.exe).
  Windows' gdbserver serves on a TCP port only, and WSL's NAT network reaches neither the host's 127.0.0.1 nor its
  other addresses through the firewall, so gdb talks to it through tools/port_win64_relay.c (a stdio-to-TCP relay
  that runs on Windows: `target remote | port_win64_relay.exe 2345`). The game's output goes to run\gdbserver.log.
- **Interop details** (WSL starting Windows programs): a Windows program started from a Linux working directory fails
  with "Invalid argument" (the script runs them from the folder); not inside `systemd-run` either (build there, run
  outside); environment only through WSLENV (`run` passes SDL_AUDIODRIVER, GALLIUM_DRIVER, BWPORT_* with paths
  translated); `timeout` ends WSL's side only (taskkill ends the exe); cmd.exe here doesn't look in the current
  directory for a program (NoDefaultCurrentDirectoryInExePath), so the bat and the exe go by full path.
- **Hidden** (libntr-hidden-pc.patch): `run` and `gdb` set BWPORT_HIDDEN=1 unless it is set: the SDL window is created
  SDL_WINDOW_HIDDEN (never shown, no taskbar button, no focus; GL and the present run as before), and BWPORT_SHOTS=DIR,S,S,...
  / cfgscript `shot=NAME` read the composed frame back before the swap and write PNGs (same code as the Mac). `run --shots K`
  uses BWPORT_SHOTS then (DIR/sNNN.png). tools/win_smoke.sh is the whole test.
- **Screenshots of a visible window** (BWPORT_HIDDEN=0, only when a visible window is wanted; tools/port_win64_shot.ps1) copy the window's client area from the screen: the window must be in view
  in a connected desktop session (a covered window gives what covers it). PrintWindow would read a covered window but
  hung: it waits for the window's thread, which is the game's.
- **Keys**: port/src/keys_script.c, BWPORT_KEYS="KEY[*HOLD]@WHEN ..." (a b select start right left up down r l; WHEN
  seconds or `fN` for frame N, as port_run.py's --keys: "start@f600 start@f900"; HOLD seconds, default 0.1, 4 frames
  for a frame key), pressed by the exe itself on libntr's key register (any target; Linux also has port_run.py's
  xdotool keys). Frames come from OS_GetVBlankCount(), which read 0 until (s46d): libntr's twl/os inline
  read HW_VBLANK_COUNT_BUF as the TWL header's offset into main memory (past its 8 MB), not the word the simulator
  counts in (s_HW_MAIN_MEM_SYSTEM + 0x3c), by include order; libntr-vblankcount.patch. So the game's own
  OS_GetVBlankCount() callers (battle_select, p_sta_sys, demo3d, RNG seeds) saw 0 on the PC before.
- **Layout**: the game's types are the same on both targets with `-mno-ms-bitfields` (tools/port_layout.py over both
  exes' DWARF: 5228 named types compared). The differences it found: RivalDataSave (Linux's `unsigned long`, fixed by
  sdk_u32 in) and libntr's DGTHash1Context and MATHMD5Context, 0xa8 bytes on Linux and 0x58 (the DS's) on
  win64: libntr's `unsigned long` fields, consistent within each build. The win64 s32/u32 are long/unsigned long
  (libntr's types.h), 32 bits as on the DS: the int/long question is the compiler's pointer-type warning only.

Measured 2026-10-28 on this host (Windows Server 2025 VM, no GPU, Mesa 26.2.4 llvmpipe): win64 stops where Linux
stops at every step tried.'s main: the same SIGSEGV in OS_CreateExtraHeap (os_alloc.c:709), then on p43c's
fixes the same exit(1) from GFL_NetErrInit (heap 8 full). On main e61f9b6ba plus these commits (`run 250 --shots 10
--fresh` with BWPORT_KEYS="start@112 a@124 ..."): the ROM extracted, the copyright, the Game Freak logo, the opening
movie (to about 80 s), the title screen (90-110 s), Start: the start menu (four items), A on the first one (new game): the intro's background gradient and nothing more, still running at
250 s: docs/port-boot.md's open item 13 (the intro's script waits), as on Linux. On b44a's branch before its
particle-work fix (port-boot.md 10) the intro crashed at 136 s in GFL_G3DCameraFree <- particle.c:479 <-
intro_particle.c:44, which the crash report gave with lines, as Linux did.

## The design

- **port/** holds everything PC-only: port/meson.build (the meson project), port/meson.options, port/machine/*.ini,
  port/subprojects/ (libntr.wrap, libntrsystem.wrap pinned to f6b3a69 and 97fa995; meson clones them at setup into
  the git-ignored checkouts; packagefiles/ holds our patches), port/include/ (headers), port/src/*.c (each agent's
  own files), port/exclude.txt.
- **The sources** are tools/port_sources.py's: every C file under src/, lib/gfl, lib/poke_net and lib/spl (SPL, the
  particle library, isn't in libntrsystem) minus port/exclude.txt, plus every port/src/*.c. lib/nitro, lib/twl and
  lib/nnsys are libntr's and libntrsystem's on PC; lib/dwc, lib/dwcutil, lib/nitrowifi, lib/dpw (online), lib/dsprot
  (anti-piracy) and lib/msl (the host's libc in its place) are not compiled. A new port/src file needs no build edit.
- **port/extern/<game path>** stands in for the game file of that path (port_sources.py lists it instead): it includes
  the file and adds `extern __typeof__(F) F;` for each plain `inline` function F, which under C99 (gnu99) is an inline
  definition only and emits no symbol, so every call the compiler doesn't inline (all of them at -O0) is undefined.
  The object name still ends in the game file's mangled path, so the overlay layout keeps it in its overlay.
  tools/port_undefined.py stops the build (`error: F is undefined, but src/...:N defines it`) when this is needed.
- **port/exclude.txt**: `<path or glob> <category> <reason>`; categories dsi, hw, online, sdk, todo
  (`python3 tools/port_sources.py --excluded`, `--check` for stale lines).
- **The compiler** is clang for C on both targets (port/machine/linux.ini, win64.ini: `clang
  --target=x86_64-w64-mingw32`), g++ (mingw g++ -posix) for libntr's C++ and the link. Only clang takes a function
  prototyped extern and defined static (`-fms-extensions`: MWCC accepts it, gcc can't be told to). libntr's C builds
  with clang after port/subprojects/packagefiles/libntr-clang.patch (two lines gcc folds and clang refuses).
- **The game's flags** (port/meson.build game_args): libntr's linux/win64 defines from its dependency (TwlSDK 5,
  twl_hyb: `SDK_TWL SDK_TWLHYB SDK_PORT SDK_X86 SDK_64BIT ...`, the set tools/libntr_compile.py uses), `-std=gnu99
  -fms-extensions -fsigned-char -fcommon -fno-strict-aliasing -fwrapv -w`; on win64 also
  `-Wno-error=incompatible-function-pointer-types` (s32 is long there) and, global so libntr has it too,
  `-mno-ms-bitfields` (the DS's struct layout for bitfields next to 64-bit integers).
- **Include order**: port/include, include, lib/{dpw,dsprot,dwc,dwcutil,nitrowifi,poke_net,spl}/include, the ROM
  build dir (option `rom_build_dir`, default <repo>/build: asset/*.inc), then libntr's and libntrsystem's (from the
  dependencies), and lib/msl/include last with `-idirafter`: libntr's thread.h includes SDL2, which includes the host's
  libc, and MSL's stdio.h, stdarg.h ... must not answer for it.
- **port/include/port_prefix.h**, included first in every file (`-include`): libntr's `<nitro.h>` and `<nnsys.h>`
  (as libntr's own nitro_pch.h; the decomp's files use names their includes bring with our headers), port's
  types.h, the game's struct_decls.h (our nitro/os.h includes it), port_compat.h, and `main` undone (SDL_main.h makes
  it SDL_main on Windows).
- **port/include/types.h** stands in for include/types.h (same guard): the integer types are libntr's, whose win64
  u32/s32 are unsigned long/long; the game's `typedef unsigned int u32` contradicted them in every file.
- **port/include/port_compat.h**: the pokebw2 SDK spellings the C still uses that libntr spells otherwise or lacks
  (GX_PLANEMASK_ALL, DEG_TO_IDX, reg_G3X_RAM_COUNT, NNS_G2D_ANIMELEM_*, the NNS_G3D_SIGNATURE_*, the TWL MIC
  limited-sampling declarations ...). Names only: a function port/src defines must not be a macro there.
- **The entry**: src/system/main.c's main() is the DS's NitroMain, which libntr's main() (sim_main.cpp) calls after
  setting up the simulated hardware. That file is built apart (static_library entry, `-DBWPORT_ENTRY`: port_prefix.h
  then defines main as NitroMain).
- **The link**: the game's objects are static libraries linked whole (game, entry) with libntr's. A second exe,
  bwport-nostubs, is never built: tools/port_undefined.py runs its link command from build.ninja with the undefined
  references as warnings, groups the symbols (undefined.txt) and writes undefined_stubs.c, a trap for each undefined
  function (prints "bwport: undefined function X called" and aborts) and a zeroed block (the ROM's size, at least
  256 bytes) for each datum. bwport links with those, so it runs until it reaches one. `-Dallow_undefined=false`
  links without them. (PE's ld can't leave a symbol unresolved, so the stubs are what makes win64 link.)

## Counts (this branch alone, without the other port agents' port/src files)

| | linux | win64 |
|---|---|---|
| game C files | 1089 (src 1023, lib/gfl 55, lib/poke_net 3, lib/spl 8) | same |
| compiled | 1064 + port/src | 1064 |
| excluded | 25: 11 dsi (src/ov172/*.c, src/system/dsi.c), 2 hw (IR: irc_spi.c, ir_check.c), 1 online (dwc_rap_vchat.c), 4 sdk (SDK copies in src/main: MI DMA, DeliverArg, GX texture loads, OS mutex), 7 todo | same |
| compile errors | 0 | 0 |
| undefined symbols | 345 | 345 |
| exe | 37 MB, -O0 -g | 43 MB; imports SDL2, enet, libwinpthread-1, KERNEL32, msvcrt, SHELL32, USER32, WS2_32 |

build/port/undefined.txt (`group symbol ROM-object users`), by where a definition would come from:

| group | n | what |
|---|---|---|
| online | 153 | lib/dwc (101), lib/dpw (29), lib/nitrowifi (11), dwcutil (2), the excluded voice chat (10): port/src/stub_online.c |
| excluded | 129 | defined by the todo files (g3d_system.c 58, field_g3d_map.c 35, btlv_effvm.c 12, game_beacon_search.c 8, gimmick_roller_coaster.c 7, gf_overlay.c 3, dma_vblank.c 1) and the IR files (5) |
| game | 26 | a compiled file's ROM object has it and its C doesn't: the incomplete files' assembly (main_020584e0.c 9, main_0205a16c.c 11, ov071_021b7234.c 4 data, main_02090f18.c, c_gear_sw.c) |
| dsi | 8 | src/system/dsi.c's (func_0202eb0c, canPlayerExchangePhotos ...), lib/twl's SNDEX (func_027047c0, func_02704364), one ov172 datum |
| sdk | 7 | Black's mi_memory.c helpers (MIi_CpuClearFast, sys_memcpy_ex, sys_memcpy_fast), the PM exits func_027314f0 ... |
| libntr | 8 | libntr's own objects reference them and nothing defines them on PC: RTCEXi_* (6), OSi_CountryCodeList(NumEntries) |
| dsprot | 6 | func_ov230_021882a0 ... 021883cc: report "check passed" |
| linker | 4 | in no ROM object: BackupMail, BackupRecords, PStaInfo_LoadSpeciesName, TownMapMap_SetLimits (called from code the ROM's linker dropped) |
| nnsys | 3 | func_02065ce4, func_02065d4c, func_02066748 (NNS_G2dDrawCell, ...DrawCellAnimation, ...SetRendererImageProxy) |
| msl | 1 | _s32_div_f (MWCC's division helper, called by name) |
| gap | 0 | |

Merged with p43b's stubs and p43d's SDK files (a trial merge of the three branches; libntr-sdk-gaps.patch is in this
wrap already): no conflict, it builds, and 134 are left: excluded 124, game 5, linker 4, msl 1 (online, dsi, dsprot,
sdk, nnsys and libntr at 0). That exe gets through SystemInit and faults in GFLAppInit (src/system/gfl_use.c:112):
libntr's OS_CreateExtraHeap(OS_ARENA_MAIN) writes its first heap cell at os_alloc.c:709, the arena of the DS's extra
main memory, which nothing gives a PC address.

## The decomp's C changed for the PC build (ROM byte-identical: `ninja` passes)

- NitroSystem's and the SDK's field names where pokebw2 had its own (NNSG2dPaletteData, NNSG2dCharacterData,
  NNSG2dScreenData, NNSG2dMultiCellInstance, NNSG2dCellBoundingRectS16, NNSG2dMultiCellHierarchyData, WMParentParam,
  WMGameInfo, MICAutoParam, SNDWaveArc), in our headers and every use (main had them by the rebase).
- src/ov107/mbp.c: its own MB_RegisterFile prototype as the SDK's (`const void *buf`).
- 12 definitions spelled int, s32 or BOOL where their prototype spells another (one type on the DS and on Linux;
  long and int on win64): poke_net_gds.c, event_mapchange.c, field_msgbg.c, weather_task.c, plist_item.c,
  trcard_bmp.c, pokemontrade_demo_common.c, ov135_021f4580.c, game_data.c.

## What's left

- **The 7 todo files compile now**: field_g3d_map.c, btlv_effvm.c and
  gf_overlay.c compiled as they were (MtxFx43 and FSOverlayInfo had become the SDK's in). g3d_system.c and
  lib/nnsys's res_struct_accessor.c read the file header's signature as NitroSystem's `sigVal` (our
  NNSG3dResFileHeader is its union of `char signature[4]` and `u32 sigVal`); gimmick_roller_coaster.c has
  NitroSystem's NNS_G3D_JNTANM_TINFO_FX16ARRAY (pokebw2's FX16ARR was a second name for it); game_beacon_search.c
  and dsi.c read TwlSDK's OSTWLParentalControl `flags.isSetParentalControl`, `flags.photoExchange` (our nitro/os.h's
  struct by TwlSDK's names); dma_vblank.c's MI_DMA_REGS is port_compat.h's array over libntr's DMA register variables
  (laid out as the DS's: libntr's own MI_DMA_REGADDR reads them so). The ROM is byte-identical (`ninja`).
  gf_overlay.c alone pulls libntr's fs_overlay.o into the link, whose 5 linker-script names (SDK_OVERLAY_NUMBER ...)
  nothing defines on PC; p43c's port/src/ovl.c defines FS_LoadOverlayInfo, FS_LoadOverlay and FS_UnloadOverlay, so
  with it fs_overlay.o and those names leave the link.
- **Semantic notes found on the way**: src/ov103/irc_sync_check.c calls the decomp's NNS_G3dMdlSetMdlDiffAll(mdl,
  polygonId), which in Black is NitroSystem's NNS_G3dMdlSetMdlPolygonIDAll; on PC that call reaches libntrsystem's
  SetMdlDiffAll (the diffuse colour) unless the decomp renames it. SNDWaveArc and other ROM-file structs with a
  pointer member (topLink) are laid out with 8-byte pointers on PC, in libntr's headers as in ours.
- The undefined groups after the other agents' files are merged; then run the exe and take the stubs it reaches in
  turn (`gdb -batch -ex run -ex bt`: the stub's name and its caller).
# The PC build

## Data: what the ROM build takes from the base ROM

The ROM build links three kinds of bytes that no compiled C holds: the assets the C includes (config/b_us/assets.tsv),
the gaps between the files' ranges (the `_dsd_gap@*` objects) and the incomplete files (delinks.txt without
`complete`: Black's link takes their delinked objects, not their C). The PC build compiles the C only. What each
means for it, measured 2026-10-28 on the ROM build's objects with `python3 tools/port_data.py` (its `gaps` and
`incomplete` read build/b_us/, so run `ninja` first):

    python3 tools/port_data.py assets                 # the assets the PC files include, with their assets.tsv rows
    python3 tools/port_data.py assets --out DIR       # ... written to DIR/asset/<name>.inc from extract/b_us
    python3 tools/port_data.py gaps                   # what the PC files use that only ROM-only objects define (exit 1: data)
    python3 tools/port_data.py gap-c OUT.c            # that data as byte arrays from the base ROM (not needed today)
    python3 tools/port_data.py incomplete [FILE...]   # what the incomplete PC files' C lacks against the ROM's bytes

The PC files are tools/port_sources.py's (src/, lib/gfl, lib/poke_net and lib/spl less port/exclude.txt).

### Assets

- assets.tsv has 4 rows; the PC files include one: src/system/bmp_cursor.c's `asset/bmp_cursor_pixels.inc` (main
  0x0209e8cc-0x0209e8ec, u8). The other three are lib/nitro's (fs_overlay_digest.c: overlay_table_digest,
  overlay_digests) and lib/twl's (dsp_g711_component.c), which the PC build doesn't compile.
- The PC build takes ninja's: port/meson.build puts the ROM build dir (`-Drom_build_dir`, default `<repo>/build`) last
  in the include path, as configure.py does, so `asset/<name>.inc` is `build/asset/<name>.inc`. Checked: the PC
  object of bmp_cursor.c holds the ROM's 32 bytes. `ninja build/asset/bmp_cursor_pixels.inc` writes the one file;
  without a ROM build, `python3 tools/port_data.py assets --out DIR` writes the same bytes (compared) and
  `-Drom_build_dir=DIR` points the PC build there.
- No dsd extraction (macOS, iOS: dsd is a Linux x86_64 program): port/meson.build's `rom_assets` step writes
  build/asset itself with `tools/rom_modules.py --assets`, which reads the .nds directly (no dsd) into dsd's extract/
  layout and writes every main and overlay row (the maintainers' notes, "Mac").
- The values are C numbers of the row's type, so the host's byte order doesn't matter. An asset that
  `tools/asset.py --move` adds later reaches the PC build with no change to it.

### Game data from the ROM (2026-11-07: the packages ship without the game's data)

The Linux exe, the win64 exe and the APK's libraries hold none of the game's data: every table, string and constant of
the game's C that the DS build puts into the ROM's .data/.rodata is zero in the shipped image and is copied from the
player's ROM before the game boots. macOS and iOS do not do it yet (docs/ios-handoff.md "Game data from the ROM").

- **The design.** A matching decompilation's compiled data IS the ROM's data, however it is spelled in C, so the step works
  on the whole linked image, not table by table, and needs no knowledge of any C type. (1) The link (`bwport-full`, with
  `-Wl,-Map`) keeps the C as compiled; the linker script (tools/port_overlays.py romdata_script) puts each group's read-only
  data in an output section of its own, so the linker merges strings and constants only within a group: `.bwrom_b` (Black's
  game C: libgame.a, libentry.a, without port/src's and port/game's files), `.bwrom_w` (White's), `.bwrom_s` (what every game
  runs: the two game files both share, libntr's and libntrsystem's SDK libraries but libsim, port/acore's ARM7 tables; plain
  .rodata only). Writable data and const data with pointers stay where they were (.data, .ovldata, .machdata,
  .data.rel.ro); the map says which object each of their input sections is. (2) tools/port_romfill.py (port/meson.build's
  `bwport` target; about 5 s) reads the image's own relocations (ELF .rela.dyn/.relr.dyn, PE .reloc): those pointer slots
  stay as linked. The bytes between them are matched against the game's ROM modules as the DS loads them (ARM9 static
  module and overlays BLZ-decompressed, the ARM7), longest match first through a sorted 8-gram index, preferring the place
  the last match ended (a table's next element) and the DS object of the same C file (config/<version>/**/delinks.txt).
  Each match is a copy (module, offset, length); its bytes are zeroed in the image. A shared group's bytes must be in every
  built game's ROM (each game gets its own copies). Bytes in no module stay as compiled: host-only data (clang's jump tables,
  port-only code in game files), 57 KB a game. (3) The copies go into the image's `.bwfill` section (port/src/asset_load.c
  reserves 512 KB; 112 KB used), per game with an FNV-1a hash of the bytes they give. The step checks itself: it applies
  its table to the zeroed image and compares with the linked one, for each game.
- **At start.** asset_load.c's PORT_AssetsLoad (machine.c's SIM_RunGame, after the launcher chose the game; once per game
  per process) finds the ROM as before (the launcher's copy, BWPORT_ROM, a .nds in the working directory: only with the
  game's code), makes the regions' pages writable (mprotect / VirtualProtect), copies, puts the protection back (RELRO
  included), and copies writable regions into ovl.c's and machine.c's snapshots (PortOvl_Keep, PORT_MachineKeep) so an
  overlay load or an in-process restart puts the ROM's values back. The shared group is copied for the first game that
  starts. A ROM whose bytes differ (another revision) fails the hash: a message (and a message box) and exit 1. Log:
  `assets: IRBO: 0 tables, 0 bytes, the image's data 221946 bytes in 9949 copies, from <rom> in 19.1 ms` (Linux; win64
  18.4 ms, the Android emulator 29.9 ms; rt1's 428 rows took 30.4 ms). A link without the step (`ninja bwport-full`)
  runs as before with the data compiled in.
- **What the image holds now** (Linux, both games): Black 221,946 bytes copied (.bwrom_b 175 KB, .data.rel.ro 6.8 KB,
  .ovldata 7.9 KB, .machdata 0.7 KB, shared 31.7 KB), White the same. Code: no run of 16 or more bytes of .text equals
  either game's modules (what the compiler folds into code is code). tools/port_assets.py's per-table placeholders
  (rt1's rows) are off (`asset_loader = false`): the whole-image step covers their rows; its loader, ROM reading and hooks
  are the ones above.
- **Black 2 / White 2.** port_romfill.GAMES has their rows (IREO, IRDO; archives libgame_b2.a/libentry_b2.a, _w2): a PC
  link of pokebw2's C as archives, as White's, gets the same treatment with `--game IREO:b2:<rom>:<pokebw2>/config/b2_us`.
  Today they link on macOS/iOS only (b2.o, w2.o), where the step does not run; nothing of it is checked without their ROMs.
- **The release gate.** `tools/release_check.sh PACKAGE...` runs `tools/rom_leak_check.py --package` (an apk/ipa/zip's
  members, a folder, plain files; every run of >= 48 ROM bytes counts) and exits 1 on any run that is not trivial (a short
  period, or since 11-07 a ramp of 8/16/32-bit elements: code point tables, arm64 jump tables of equal-sized cases) or
  listed in tools/release_allow.tsv: public tables by the sha1 of the run (stb_image's zlib and JPEG Annex K tables, the
  IMA ADPCM step table, ImGui's font and SMAA's area table equal to ROM file bytes by chance, libntr's JIS maps, mingw's
  printf jump table, a dex bit-mask table) and third-party files by the sha256 of the file (SDL2, enet, winpthreads, Mesa,
  the APK's SDL2 and libc++), each named. A new library build has another hash and is looked at again. Release forms:
  the Linux exe and the win64 exe without DWARF (`strip --strip-debug`; `tools/port_win64.sh zip` does it: the debug strings
  name SDK enums the ROM's Wi-Fi code prints), the APK as tools/android_package.sh makes it (stripped). 2026-11-07: all
  three pass (Linux 1606 bytes in 19 listed runs, win64 zip 40655 in 419, APK 4292 in 25); before, 185 KB in 1159 runs.
  tools/android_publish.sh refuses an APK the gate refuses (BWPORT_RELEASE_CHECK=private-only skips it for the private
  prerelease alone), `tools/port_win64.sh zip [ZIP]` makes the Windows release zip only if the gate passes.

### Gaps: no gap_data.c

- The 11 gap objects (244 bytes of code, 391 of data) define 20 symbols. The C that uses them is lib/nitro's
  (CARDi_BackupSignature, SDK_OVERLAY_DIGEST_END), lib/nitrowifi's and lib/dwc's (the `*i_Signature` strings, gstats.c's
  and dwc_init.c's tables): none is compiled for PC. data_020a18c4, data_ov013_021b60f5 and the `_keep_*` copies
  are used by no C object.
- The PC files use one gap symbol, SDK_AUTOLOAD_DTCM_START (_dsd_gap@dtcm_0.o; gf_overlay.c, heap_dtcm.c,
  system/main.c and 14 overlays' procs), and that is the linker script's (port/src/linker_syms.c), not data.
- So the design's generated port/src/gap_data.c isn't needed: the PC build's C uses no gap's bytes. `gaps` checks it
  (it exits 1 when a PC file uses data that only a ROM-only object defines). `gap-c OUT.c` writes such data from
  extract/ into the build dir if it ever comes back, and leaves out data that holds pointers: their bytes are Black's
  addresses (the two ov071 proc tables below were such), and these need C.

### Incomplete files

12 of the 25 aren't compiled for PC: lib/nitro/src/syscall/secure_area.c, lib/nitrowifi's cps.c and ssl.c, lib/dwc's
gsPlatformUtil.c, largeint.c, dwc_netcheck.c and AuthService.c, lib/dwcutil's atermset.c, sceneSetError.c and
sceneSetList.c, lib/twl's ssp_jpegdec.c and ssp_jpegenc.c. The game calls 7 of their functions, which stubs must give:
GetCRC16 (secure_area.c; src/main/main_02087f60.c's OS_EncodeDeliverArg, OS_DecodeDeliverArg), func_ov009_0215c910
(cps.c) and func_ov009_02162248 (ssl.c) from lib/poke_net, Calloc (atermset.c) from src/ov018/intrude_field.c, and
SSP_StartJpegDecoder, SSP_StartJpegEncoder and SSP_GetJpegEncoderBufferSize from src/ov172/ctvt_comm.c.

The other 13 are compiled for PC, and their C was never linked before. Against their delinked objects (symbols.txt's
names; data the C names otherwise, such as string literals, found by its bytes), the C lacked no function. It lacked
5 data symbols that other C uses (100 bytes, the PC link's only ROM-only data) and had 2 tables with other bytes than
Black's; all 7 are fixed in C (the ROM is unchanged: these files' bytes are the ROM's in its link, and brief.py's counts
are the same before and after). What is left is the one function in each of 10 files that doesn't match: its C runs
on PC as written, never compared with Black's code but by its diff (brief.py's class, below). Registers, order and
stack are the same code; "an instruction more or less" is mostly a width, a reload or a mov; the 3 structural ones
(the bag's draw, two of the Entralink monolith's) are where the PC can do otherwise than the DS. In the PC exe before
these fixes (p43a's link, undefined symbols left at 0) the 5 data symbols were address 0: the C-Gear's fade and the
monolith's screen procs would have read or called through 0.

| file | functions matching | the C lacked (fixed) | left for PC |
|---|---|---|---|
| lib/gfl/src/backup_system.c | 51 of 52 | | func_0203335c (18 lines of diff, registers only) |
| src/ov015/irc_link.c | 18 of 19 | | func_ov015_02179fc4 (5, an instruction more or less) |
| src/ov021/field_g3d_mapper.c | 58 of 59 | GND_ANIME_WK's archive IDs were pokebw2's 0x44, 0x45; Black's 0x45, 0x46 (the field's terrain animations) | func_ov021_0218cd20 (26, an instruction more or less: loads in another order, a nop) |
| src/ov021/ov021_02193f34.c | 11 of 12 | | func_ov021_02194128 (32, stack only) |
| src/ov029/c_gear_sw.c | 84 of 85 | data_ov029_021f3224 (c_gear.c's fade table, 6 words); the lamps' steps: two arrays 4 bytes short of Black's, the second read past the first's 4 bytes, now one `[5][9]` table | func_ov029_021f1290 (57, an instruction more or less); data_ov029_021f3520 is `[8]` where Black's is 6 bytes (harmless) |
| src/ov046/ov046_021f36c0.c | 20 of 21 | | func_ov046_021f36c0 (122, an instruction more or less) |
| src/ov070/itemmenu_disp.c | 54 of 55 | | func_ov070_021b9280 (380, structural: a draft; the bag's draw) |
| src/ov071/ov071_021b7234.c | 31 of 32 | data_ov071_021ba8d0 and data_ov071_021ba91c (two GameProcFunctions that ov071_021b6100.c's screen table points to), data_ov071_021ba948 (the six level actors' positions), data_ov071_021babf4 (the asserts' "") | func_ov071_021b739c (59, structural) |
| src/ov071/monolith_tool.c | 37 of 38 | | func_ov071_021b6e1c (32, structural; its best was 29: `brief.py ... --restore`) |
| src/ov091/plist_plate.c | 30 of 30 | | nothing (Black's .rodata order only) |
| lib/poke_net/src/poke_net.c | 30 of 31 | | func_ov125_021d7fa0 (15, registers and order) |
| src/ov175/wifibattlematch_net_fields.c | 5 of 5 | | nothing (the strings pooled in another order) |
| src/ov175/wifibattlematch_net.c | 96 of 96 | | nothing (Black's .data order only) |

- `incomplete` still names a few data as "all zeros, not looked for": 4-byte "" objects, words Black relocates
  (pointer tables the C defines under other names: wifibattlematch_net.c's) and tables of one non-zero byte. The
  matching functions read the C's own, so on PC these are as the ROM has them; only the 10 functions above are open.
- A file that becomes complete later leaves this list; a new incomplete file's C gets `incomplete FILE` first.
