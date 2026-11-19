# The PC build (port phase)

The decompiled C built as a PC program on [libntr](https://github.com/cybervisi0n/libntr) (MIT, NitroSDK/TwlSDK for
PC) and libntrsystem (NitroSystem), for Linux x86_64 and win64 (mingw-w64, cross from WSL). docs/libntr/*.md has
how libntr itself builds and what it covers. Measured 2026-10-28 on main ea131eb9a plus this branch: every game file
the build takes compiles on both targets, both link (with generated stubs for what is still undefined), and both
start: libntr opens its window, calls the game's NitroMain, and SystemInit runs until its first undefined function.
That was the start; the sections below are dated. Today every game plays on Linux, Windows and Android (the README), and
the build needs no ROM ("A build with no ROM" below).

## Commands

From the repo root, in WSL (meson >= 1.3, ninja, clang, g++, SDL2 and enet: docs/libntr/build.md "Dependencies").
No ROM build is needed first (since 2026-11-16: "A build with no ROM" below); with the dumps in orig/ the link also runs the
ROM-fill step.

    meson setup build/port/linux port --native-file port/machine/linux.ini
    ninja -C build/port/linux bwport                    # 1525 steps, 35 s on the worker slice; links with lld when installed
                                                        # (apt install lld; -Dlld=disabled: GNU ld; the Windows build stays on GNU ld)
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
`python3 tools/run_tests.py [-j N] [NAME ...]` runs every tools/*_tests.py from the repo root (a test that needs the
ROMs, extract/ or a ~/ref checkout skips itself): one line per file, one summary line, exit 1 on any failure.

## Running

`tools/port_run.py` builds the Linux exe, runs it headless and says in one line how far it got (the last line it
prints, also in `<out>/verdict.txt`):

    python3 tools/port_run.py                               # build, run 30 s, a screenshot every 5 s
    python3 tools/port_run.py -t 120 --shots 2 --keys 'start@f400 a@f700 a*0.5@20'
    python3 tools/port_run.py --no-build --fresh -t 10      # the exe as it is; no save.bin, sim_config.ini, imgui.ini

| verdict | when |
|---|---|
| `exit-frame N in T s (F fps)` | `--frames N`: the exe ended itself, exit 0, at the game's V-blank N (BWPORT_EXIT_FRAME): a PASS, the same game state on a slow host or a fast one |
| `watchdog: frame F of N in T s (F fps)` | `--frames N`, but `-t` (then only the watchdog, default 60 + N/10 s) came first: a FAIL, no result |
| `reached-frame N in T s (F fps)` | still running at the wall cap `-t`, frames advancing (a slow host's result: use `--frames` for a check that must end on a game frame) |
| `hang in F (file:line) [... G (file:line)], frame N (no frame for S s)` | running, but no frame for `--hang` seconds (5): the main thread's innermost frame with source; `... before the first frame` |
| `stub X called from F (file:line), frame N` | an abort stub (build/port/undefined.txt) |
| `heapcheck: heap overrun after F (file:line) <- G, frame N (clean at M), heap H, KIND, ...` | `--heapcheck` / BWPORT_HEAPCHECK: the heap checker stopped the exe (docs/nnsys-port.md "Heap checker"): F allocated the block before the damage |
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

The unwind table (`.pdata`) must stay right: mingw's longjmp (the in-process restart, port/src/machine.c) and the crash report's
frames walk the game's frames by it. GNU ld sorts a `.pdata` output section by its raw bytes even in a `-r` link, which breaks
the relinked modules (pl.o, b2.o, w2.o: tools/port_relink.py works around it, docs/launcher.md "Platinum, HeartGold and
SoulSilver in the launcher"). Check a new exe with `x86_64-w64-mingw32-objdump -p bwport.exe | grep -c "same begin"` (0).

The exe holds all seven games (Black, White, Black 2, White 2, Platinum, HeartGold, SoulSilver), as Linux's does, whatever
is in orig/ at build time: port/meson.build's `white`, `b2w2`, `platinum` and `hgss` default to on, and a missing ROM only
means no whole-image fill (11-18, w64g: a default build and one with `-Drom_dir=` an empty folder both accept all seven ROMs).
The launcher's library lists a game only when its ROM or files are in the library (launcher_gui.cpp), so a run that imported
four ROMs shows four tabs. The table itself: `BWPORT_LCTEST="importrom=B.nds;importrom=W.nds;..."` over the seven ROMs ends
`0 failed` (each ROM is matched by its sha1 against the exe's table).

    tools/port_win64.sh                 # BUILD ONLY (build/port/win64/bwport.exe); nothing is deployed
    tools/port_win64.sh deploy          # build and fill C:\bwport, the deploy folder (--out DIR for another folder)
    # a build writes bwport.exe.sha (its commit) next to the exe; under BWPORT_WIN64_PREBUILT=1 (a runner shipped the exe)
    # build and deploy refuse an exe whose stamp is not HEAD
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

### win64 on Windows (tools/port_win64.py)

The same exe built on Windows itself, no WSL: any Windows shell with Git and a python.org Python 3. Clone with
`git -c core.autocrlf=false clone ...` (Git for Windows defaults to CRLF checkouts, which break the wrap patches and sh scripts).

    python tools/port_win64.py fetch                  # once: the toolchain into build/win64-tools (--tools DIR elsewhere)
    python tools/port_win64.py                        # build/port/win64/bwport.exe (Black and White)
    python tools/port_win64.py deploy --out DIR       # and the folder: exe, SDL2.dll, enet.dll, libwinpthread-1.dll, mesa\, bwport.bat

The toolchain is WinLibs' MinGW-w64 (GCC 13.2 posix, clang 18.1.3, MinGW-w64 11.0.1, MSVCRT: Ubuntu 24.04's versions, so the
objects match the cross build's), meson 1.12.1 + ninja in a venv, WinFlexBison (libntr's makelcf is configured even though
bwport.exe does not use it), SDL2 2.32.10 and enet 1.3.18 as docs/libntr/build.md, Mesa's DLLs (only with 7-Zip installed: the archive uses BCJ2,
which no pip package reads); every download is pinned
by sha256. port/machine/win64-native.ini is win64.ini with WinLibs' names and no Windows.h shim (Windows' names ignore case:
the shim would include itself). What the native build needed changed (Linux and WSL unchanged): port_sources.py prints
paths with / (meson compares them with its own: src/system/main.c was linked twice), port_assets.py's `#line` names
use / (a backslash there is an escape), asset.py's per-table SIGALRM limit is skipped where there is none, the linker
script's -T path uses / (port_undefined.py's response file), port_relink.py splits meson's tool paths without eating
their backslashes, and meson.build falls back to unprefixed objcopy and nm.

Not yet: (1) Platinum and HeartGold/SoulSilver: pokeplat_gen.py / pokehg_gen.py build pret's host tools with MinGW gcc, and
they are POSIX code (nitroarc includes fnmatch.h; enumproc's libexpr and nitroarc assume a 64-bit long under -Werror);
pret's tools/meson.build also wants arm-none-eabi-gcc (Arm's Windows toolchain zip works) and cp/true/sh (Git's usr\bin).
`fetch --gen4` gets MSYS2's libpng, zlib and pkgconf, which pret's meson setup then finds. (2) Black 2 and White 2
(-Db2w2=auto) are untested: port_relink.py's `-r` links need GNU ld, which WinLibs has. (3) Meson re-archives a thin
static library in place on Windows (no rm first): after a source list change GNU ar fails with "libgame.a: No such
file"; delete that .a and build again.

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
   **On the graphics card (2026-11-13 d3d12, 2026-11-14 zink first)**: the bundled Mesa picks its driver in this order
   (unless the user's GALLIUM_DRIVER or BWPORT_GL=mesa says otherwise), and a GPU driver that gives no usable context is
   unloaded (SDL_GL_UnloadLibrary, the pre-loaded DLL freed; libgallium_wgl.dll really leaves the process, so the next load
   reads GALLIUM_DRIVER and makes its screen afresh: tested, zink with no Vulkan falls to d3d12 or llvmpipe in the same
   process) and the next one tried; only llvmpipe failing is fatal:
   1. **zink** (Mesa's GL on Vulkan; MIT; needs only the graphics driver's vulkan-1.dll): when `FindVulkanGpu` finds a
      physical device that is a real GPU (vkCreateInstance 1.0 with no layers or extensions, vkEnumeratePhysicalDevices,
      vkGetPhysicalDeviceProperties: deviceType discrete or integrated; CPU / virtual / other are skipped; the Vulkan types are
      declared in gl_select.c, no SDK). Remote Desktop is the case: the session's GL is GDI Generic 1.1 while Vulkan still
      sees the card.
   2. **d3d12** (GL on Direct3D 12): when mesa\dxil.dll is there (Microsoft's DXIL signer, **not in the public zips**:
      `tools/port_win64.sh --dxil` copies it into a private folder; without it the d3d12 driver dies at its first draw, exit
      0x80070057) and DXGI has a hardware adapter that makes a D3D12 device. Mesa takes DXGI's first adapter, which in an RDP
      session is the Basic Render Driver (WARP, software): gl_select.c hands Mesa the first hardware adapter of DXGI's
      high-performance order through libgallium_wgl.dll's DrvSetCallbackProcs (the ICD callback pfnGetAdapterLuid), before
      SDL_GL_LoadLibrary (SDL makes a context there, and Mesa's screen keeps the adapter of its first context).
   3. **llvmpipe**, the software GL.
   A GPU driver whose renderer string says software (lavapipe, WARP) counts as no usable context. Per process: no policy or
   registry change; the card's memory is used only while the game runs. Log: `GL: Vulkan device: <card> (discrete GPU)`,
   `GL: the bundled Mesa on <card> through Vulkan (Zink)`, then `bundled Mesa (Zink, Vulkan): zink Vulkan 1.4(<card> ...): hardware`
   (Renderer=auto then picks gpu); skipped drivers say why (`no Vulkan (no vulkan-1.dll)`, `d3d12 skipped: no ...\dxil.dll`),
   a failed one `the bundled Mesa on zink gives no usable OpenGL (...): unloading it, trying the next driver`. The AMD desktop
   (RX 7900 GRE) in an RDP session, town save, uncapped, Renderer=gpu (machine under load, so a spread): zink 548-775 fps,
   d3d12 675-807, llvmpipe 79-100 (53 on d3d12's WARP); vcore 244 on any. `BWPORT_GL=mesa` or `GALLIUM_DRIVER=llvmpipe` keeps
   the software GL.
3. `BWPORT_GL=system` takes the system's GL whatever it is (a software one too; a GL that can't make a context at all still
   goes to Mesa, then the order above), `BWPORT_GL=mesa` the bundled one on the CPU (llvmpipe), `BWPORT_GL=zink` / `d3d12` the
   bundled one on the card through that driver first (the later ones of the order as the fallback; a forced zink is tried even
   if no Vulkan GPU was found). GALLIUM_DRIVER=zink / d3d12 / llvmpipe set by the user stands the same way (another value, e.g.
   softpipe: that driver only). Linux: always the system's (Mesa or the vendor driver); only the report and LP_NUM_THREADS apply.
   Testing "no Vulkan" on win64: VK_DRIVER_FILES / VK_LOADER_DRIVERS_DISABLE do not hide a card the loader finds through
   the registry / DXGI; an invalid file named vulkan-1.dll next to the exe does (LoadLibrary fails: remove it afterwards).
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
- **Windows test traps** (gp3, 2026-11-14): (1) Smart App Control blocks an exe PER HASH: a rebuilt bwport.exe (a new hash) can be
  refused to start while the build before it ran, with nothing wrong in the code. It is off on the dev desktop now; on any other Windows
  machine an exe that will not start at all (Windows' block message, or nothing) is checked against it first (Windows Security >
  App & browser control) before the build is blamed. (2) A PowerShell script (the bench scripts: a `powershell -File .../bench.ps1` call)
  started from Git Bash needs forward-slash paths, in the command line and in its arguments: Git Bash eats backslashes before PowerShell
  sees them, and the script then runs on a wrong or empty path without an error.
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
  256 bytes) for each datum (the ROM objects' nm output is kept in build/port/rom_definitions.json by mtime and size:
  6 s a relink, 30 s without). bwport links with those, so it runs until it reaches one. `-Dallow_undefined=false`
  links without them. (PE's ld can't leave a symbol unresolved, so the stubs are what makes win64 link.)
  **Function or datum** (11-15, ud1; the HG battle hung because a trap's code bytes were read as a table): a symbol an
  object CALLS is a function (ELF relocation R_X86_64_PLT32, aarch64 CALL26/JUMP26: tools/port_undefined.py reloc_uses,
  which reads the link's objects and archives, thin ones too, in under a second), else the ROM objects' kind of it, else
  its name (DATA_NAME: `s_ g_ [sg]Upper ...List Table`), else an object that only takes its address (GOT load, a
  pointer in a table) makes it a datum, else a function. COFF and Mach-O objects aren't read (a call and an address
  are one relocation type there): those builds use the kind and the name only. A function whose address is only taken
  and that has no ROM kind would be called a datum: it shows in that list, loudly, which is the point.
  **Undefined data is a bug**: undefined.txt's "UNDEFINED DATA" section lists every datum (known / NEW), the build
  prints one `UNDEFINED DATA: N ...` line, port_run.py prints `undefined data: N (see .../undefined.txt)` at the start,
  and the HGSS build (-Dhgss=true) fails on a datum port/undefined_data_allow.txt does not name (`--fail-on-data
  --allow-data`); delete a line there when its table gets defined. Black and Platinum only list theirs.

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
player's ROM before the game boots. The macOS binary and the IPA too since 2026-11-12 ("On Apple" below).

- **The design.** A matching decompilation's compiled data IS the ROM's data, however it is spelled in C, so the step works
  on the whole linked image, not table by table, and needs no knowledge of any C type. (1) The link (`bwport-full`, with
  `-Wl,-Map`) keeps the C as compiled; the linker script (tools/port_overlays.py romdata_script) puts each group's read-only
  data in an output section of its own, so the linker merges strings and constants only within a group: `.bwrom_b` (Black's
  game C: libgame.a, libentry.a, without port/src's and port/game's files), `.bwrom_w` (White's), `.bwrom_s` (what every game
  runs: the two game files both share, libntr's and libntrsystem's SDK libraries but libsim, port/acore's ARM7 tables; plain
  .rodata only). Writable data and const data with pointers stay where they were (.data, .ovldata, .machdata,
  .data.rel.ro); the map says which object each of their input sections is. (2) tools/port_romfill.py (port/meson.build's
  `bwport` target; 25 s cold, 3-5 s on a relink: romfill-cache/ in the build dir keeps each ROM's modules and the match
  answers by the ROM's sha1 and the tool's source; `--cache ''` or BWFILL_NOCACHE=1: none) reads the image's own relocations (ELF .rela.dyn/.relr.dyn, PE .reloc): those pointer slots
  stay as linked. The bytes between them are matched against the game's ROM modules as the DS loads them (ARM9 static
  module and overlays BLZ-decompressed, the ARM7), longest match first through a sorted 8-gram index, preferring the place
  the last match ended (a table's next element) and the DS object of the same C file (config/<version>/**/delinks.txt).
  Each match is a copy (module, offset, length); its bytes are zeroed in the image. A shared group's bytes must be in every
  built game's ROM (each game gets its own copies). Bytes in no module stay as compiled: host-only data (clang's jump tables,
  port-only code in game files), 57 KB a game. (3) The copies go into the image's `.bwfill` section (port/src/asset_load.c
  reserves 512 KB; 112 KB used), per game with an FNV-1a hash of the bytes they give. The step checks itself: it applies
  its table to the zeroed image and compares with the linked one, for each game.
- **At start.** asset_load.c's PORT_AssetsLoad (machine.c's SIM_RunGame, after the launcher chose the game; once per game
  per process, the rows again after another game's: below) finds the ROM as before (the launcher's copy, BWPORT_ROM, a .nds in the working directory: only with the
  game's code), makes the regions' pages writable (mprotect / VirtualProtect), copies, puts the protection back (RELRO
  included), and copies writable regions into ovl.c's and machine.c's snapshots (PortOvl_Keep, PORT_MachineKeep) so an
  overlay load or an in-process restart puts the ROM's values back. The shared group is copied for the first game that
  starts. A ROM whose bytes differ (another revision) fails the hash: a message (and a message box) and exit 1. Log:
  `assets: IRBO: 0 tables, 0 bytes, the image's data 221946 bytes in 9949 copies, from <rom> in 19.1 ms` (Linux; win64
  18.4 ms, the Android emulator 29.9 ms; rt1's 428 rows took 30.4 ms). A link without the step (`ninja bwport-full`)
  runs as before with the data compiled in.
- **The rows' revision check, the switch between HG and SS, a fill without a game (lcfix1, 2026-11-18).** The assets.tsv
  rows (every build, the no-ROM ones too) are checked like the image's copies: tools/port_assets.py `rows` writes each
  version's FNV-1a 64 of the bytes its rows read (from port/asset_rows_fnv.tsv: version, code, a sha1 of the rows' names
  and ranges, the hash), and PORT_AssetsLoad hashes the same bytes of the ROM before it fills them; another revision with the
  same game code stops with "the ROM's data is not the data this build was made for (its IRBO rows' bytes hash to ...)". A
  row added or moved changes the signature: `rows` (the build) stops until `python3 tools/port_assets.py fnv` runs with
  the ROMs (orig/; a version whose ROM is not there keeps its line). HeartGold and SoulSilver register the same tables (one
  module), each with its own rows, so the rows fill again whenever the game differs from the one whose rows filled them last
  (the image's copies and the hash stay once per game; the log line ends "(the rows again: ...)"). An image whose fill table
  has games but not this one (a fill made without that game's ROM) stops at start: port/meson.build's rom_fill needs every
  built game's ROM, HeartGold's and SoulSilver's both, and tools/rom_leak_check.py --package refuses (exit 2) a package that
  plays a game (the launcher table's ROM sha1 as a C string in a file) whose ROM is not on the machine.
- **What the image holds now** (Linux, both games): Black 221,946 bytes copied (.bwrom_b 175 KB, .data.rel.ro 6.8 KB,
  .ovldata 7.9 KB, .machdata 0.7 KB, shared 31.7 KB), White the same. Code: no run of 16 or more bytes of .text equals
  either game's modules (what the compiler folds into code is code).
- **A build with no ROM (2026-11-16, zr1).** Black and White build from a clone with no orig/, build/ or extract/ (no
  configure.py, no ROM build): `meson setup build/port/linux port --native-file port/machine/linux.ini && ninja -C
  build/port/linux bwport`. Off Apple `asset_loader` is on: tools/port_assets.py writes each assets.tsv row as a
  placeholder (zeros in the host's layout; the pointer fields' symbol names from config/b_us/asset_ptrs.tsv, names only,
  which `python3 tools/port_assets.py ptrs build` rewrites from a ROM build when tools/asset.py --move adds a row with a
  pointer field), the sources that include one are compiled from rewritten copies (const dropped; an automatic array
  becomes a static table and a copy; a constructor registers each table), and asset_load.c fills them from the ROM the
  app imported. version_config.py writes config/w_us at meson time, so White is always built. The whole-image step runs
  only when every built game's ROM is in orig/ (Black's, and White's: the shared group must match every game); without
  it bwport is the link itself (the C's own data compiled in: that is the repository's C). With both, asset_load.c
  applies the step's table first and the rows after it (a placeholder's zeros can be one of the step's copies). The
  same legs give the same frames from the no-ROM build as from the build with the ROMs. Black 2 / White 2 the same way
  (zr2): always built off Apple (`-Db2w2=disabled` leaves them out), their rows placeholders (config/b2_us/asset_ptrs.tsv,
  White 2's pointers named alike), pokebw2's sources rewritten with its include dirs (`port_assets.py list b2w2`; a
  header of its -idirafter include/ is left as written), the registration a block per version (`#if BLACK2 / WHITE2 /
  WHITE / else Black`, each its game code), and every overlay file through the rewrite in its order (the B2 overlay
  layout is the archive's member order between the markers, and meson puts a target's generated objects first). A C
  table whose plain type is not the row's layout takes SYM_LAYOUT (`n8`: a 4-byte row value in a 1-byte slot,
  kDriftveilPads); pokebw2's G3DSceneResourceSetup has no union (TYPE_LAYOUT_B2W2). Platinum (zr4) needs no
  placeholders: no source includes its config/plat_us/assets.tsv rows (docs/platinum.md "Asset rows": retired, pret's
  tables are its C again; pl_assets writes nothing without the ROM). Its overlays' DS layout: port_overlays.py --plat
  compiles zeros without the ROM, and ovl.c's PortOvl_SetGame takes the layout from the player's ROM's overlay table
  (asset_load.c PORT_AssetRomOverlays). The whole-image step needs its ROM too when Platinum is built. From a no-ROM
  clone (ROMs linked into orig/ after the build): the Platinum chain 218/218 with main's frame total, room-load / room-bag
  / room-save the same DS screens at fixed V-blanks as the build with the ROM, and the win64 build reaches the title. Still built from
  their ROM: nothing since 2026-11-17 (Apple too: "On Apple" below).
  HeartGold / SoulSilver
  (zr5 2026-11-16): `-Dhgss=true` links with no orig/ (no romfill step; the vendored tree's ROM data are blanks plus rows,
  docs/hgss.md "ROM data as blanks"), and hg / ss new-look end on the same frame (11664) as the build with the ROMs.
  Trap: route.py with BWPORT_BUILD set to another checkout's build dir reconfigures that dir against route.py's own
  checkout; run route.py from inside the no-ROM checkout (BWPORT_ROM / BWPORT_ORIG name the ROMs).
- **Black 2 / White 2.** port_romfill.GAMES has their rows (IREO, IRDO; archives libgame_b2.a/libentry_b2.a, _w2): a PC
  link of pokebw2's C as archives, as White's, gets the same treatment with `--game IREO:b2:<rom>:<pokebw2>/config/b2_us`.
  On macOS and iOS they link as b2.o, w2.o and are filled as their own groups (below).
- **Platinum (2026-11-14, pas1).** GAMES row `pl` (CPUE, pl.o: pret's C is one relocatable object, as b2.o), so
  `.bwrom_pl` holds its .rodata and the step fills it and pl.o's .data/.ovldata/.data.rel.ro: `--game CPUE:pl:<plat rom>:config/plat_us`
  (no delinks.txt there: no same-object preference). Linux, `-Dplatinum`: 301,124 bytes copied in 7,778 copies (.bwrom_pl 251 KB,
  .data.rel.ro 8.2 KB, .ovldata 10.5 KB, .machdata 0.6 KB, the shared group's 30.7 KB), table 306 KB of 512; package scan
  (`rom_leak_check.py --package` on the `--strip-debug`'ed exe) 214,850 bytes in 752 runs + ~6.5 KB of Black/B2/W2 runs -> 0
  (the unstripped build's debug info has one 50-byte chance run). Three traps found: (1) `ovl_ld` now depends on
  tools/port_romfill.py (its GAMES table makes the script's groups; a new row left the old script, no .bwrom_pl); (2) pret's battle
  AI script (pl_ai_script.o, 42,428 bytes) is assembled into `.text`, which the step never fills: meson's objcopy step renames it
  to .rodata (.rdata on win64); (3) the SDK's pitch table (libntr's snd_util.c, port/acore's k_pitch) is in Platinum's ROM but not
  Black's, and is round(65536*(2^(i/768)-1)) for all 768 entries, so it is listed in tools/release_allow.tsv (with two
  chance runs of the host's .data.rel.ro). A shared group's bytes must be in every built game's ROM: Platinum's SDK copies are
  not separate archives (pl.o has them in .bwrom_pl, filled from Platinum's own ROM). asset_load.c needs nothing: it takes the
  game by its ROM code. Apple: the same since 2026-11-17 (`__DATA,__bwrom_pl`, "On Apple" below).
- **On Apple (ld64, Mach-O; 2026-11-12).** No linker script, so the groups are compile-time sections: port/machine/
  macos_rom_section.h (included by macos_machine_section.h and macos_ovl_section.h, which the game's, White's, B2/W2's and
  lib/nnsys's compiles already get) puts each file's read-only data, const data with pointers too (`relro`), into
  `__DATA,__bwrom_b|w|b2|w2|s` by its defines (WHITE, BLACK2, WHITE2; BWPORT_ROM_SHARED for lib/nnsys);
  libntr-ios-romfill.patch does the SDK's (`__bwrom_s`; libsim back to the default). A writable section: iOS's signed
  __TEXT is never written and no protection changes. The link writes ld64's `-map`; tools/port_romfill.py reads the
  Mach-O (load commands, sections), its pointer slots from LC_DYLD_CHAINED_FIXUPS (or LC_DYLD_INFO's opcodes), and each
  atom's object from the map: an atom's group is its object's (port_romfill.group_of; Black's overlay archive
  libgame_ovl.a, white.o, b2.o, w2.o; port/src's stand-ins in libgame.a are the host's and stay), in those sections and
  __machdata/__ovldata/__data. The table is `__DATA,__bwfill` (asset_load.c); the step re-signs the binary ad hoc
  (`codesign -f -s -`). String literals stay in __TEXT,__cstring (ld64 merges them across objects).
  port/src/ovl.c's PortOvl_Keep now updates the chosen game's Mach-O snapshot too (it was a no-op there).
  2026-11-17 (gen4): the same build as the PC's. (1) The asset loader is on (port_assets.py's rewrite spells its section
  `BWASSET_DATA`, "__DATA,__data" on Mach-O, and has no alias there: the storage takes the global's assembler name), so the
  Mac's rom_assets step is gone and Apple builds Black, White, Black 2 and White 2 with or without their ROMs.
  `-Drom_dir=DIR` names the ROM folder the fill reads (default orig/, Apple tools/mac_rom.sh's); an empty folder is a build
  that sees no ROM (build/port/macos-norom: no ROM path in build.ninja). Without the ROMs the binary is the plain link (the
  C's own data compiled in, as on the PC): the release gate is for the build with the ROMs. (2) Platinum, HeartGold and
  SoulSilver link in (pl.o, hg.o by tools/port_relink.py --macho, whose `--ovl-tag` marker check reads `nm -m`; b2.o and
  w2.o now the same way). Mach-O needed: HG's asm data with the underscore names, Mach-O sections and 8-aligned .quad
  (tools/pokehg_asm.py), HG's blanks' section through BWPORT_DATA_SECTION (hg_assets.py writes the macro itself; pokeheartgold-zzmacho.patch
  bridged the vendoring until the re-vendor at 766902128), Platinum's AI script with assembler-local labels and its section named in the text, HG's 10,000 overlay files
  in archives of 2,500 (macOS's 1 MB argument limit), `#include_next` in include_plat/nitroWiFi/soc.h (a case-insensitive
  file system found the file itself). (3) HeartGold/SoulSilver is a ROM-fill group (GAMES "hg", on every platform: it had
  none, and 115 KB of its tables stayed in the Mac image, runs up to 13 KB); Mach-O fills __DATA,__const too (HG's asm
  data). Mac binary with all seven games, all ROMs: Black 140,985 bytes from the ROM, White 140,973, Black 2 190,275,
  White 2 190,251, Platinum 285,823, HG 148,476, SS 148,476 (table 294 KB of 512); release_check.sh 0 ROM bytes on it
  and on the IPA (eight host runs the Gen 4 ROMs bring are in release_allow.tsv). The no-ROM build (-Drom_dir=empty, the
  ROMs imported by route.py's roots): the same frames as the build with the ROMs (Gen 5 --quick 24,291; SoulSilver's first
  six legs 80,068; Platinum new-look 19,236), and its gate finds 744 KB (the C's own data: no fill without the ROMs). Black,
  town save, instructions a drawn frame: 12.06 M before (main 9f641177c), 12.07 M after. Pret's Platinum header generators
  need `brew install libpng arm-none-eabi-gcc` on a new Mac.
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
