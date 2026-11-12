# Building libntr for linux and win64 (port phase, first step)

Measured 2026-10-28 in WSL (Ubuntu 26.04, gcc 15.2, meson 1.10.1, ninja 1.13.2, 24 cores of a shared slice)
on libntr `1555d91` ("SIM: Add fullscreen support", a local clone of ~/ref/libntr). Everything lives outside the
repo in <scratch>: `libntr/` (clone and its build dirs), `hello/` (the test program), `mingw/` (win64 deps),
`mingw-native.ini`, `dl/` (downloads). Nothing in libntr itself was changed.

## Results

| target | libntr + standalone demo | tiny NitroMain (`hello`, `cube`) | runs |
|---|---|---|---|
| linux, NTR (sdk 4) | builds, 309 steps, 30 s | builds | Xvfb and WSLg: 300-600 frames, exit 0 |
| linux, TWL hybrid (sdk 5, twl_hyb) | builds, 23 s (adds camera dsp el scfg wxc) | builds (333 compiles with `-DSDK_TWLHYB`) | Xvfb: 120 frames at 59.8 fps, cube renders |
| win64, NTR, cross from WSL (mingw-w64 13 posix) | libraries + standalone `libntr.exe` build; full `meson compile` fails on one ARM7 target | builds `hello.exe`, `cube.exe` | on this Windows host: 300 frames at 33 fps with Mesa llvmpipe |
| win64, TWL hybrid | not tried separately | builds `hello.exe` | 120 frames at 32 fps |

Linux is the easy target: apt packages, one meson setup, one flag. win64 cross works with four workarounds,
listed below. Nothing in libntr was patched for either target.

## Dependencies (apt, Ubuntu 26.04)

    apt-get install -y --no-install-recommends g++ meson nasm flex bison libsdl2-dev libenet-dev xvfb xauth mesa-utils x11-apps netpbm gdb 7zip
    # gcc-mingw-w64 / g++-mingw-w64 (13.2, win32 and posix variants) were already installed.

What libntr needs: meson >= 1.3, a C/C++20 compiler, nasm (the project declares it; there are no .asm files),
flex + bison (tools/makelcf), python3 (gen/ scripts), SDL2 and enet (found with `find_library`, not pkg-config;
SDL2 2.32.10 and enet 1.3.18 here). It has no OpenGL link dependency: glad loads GL 4.2 core through
`SDL_GL_GetProcAddress`. Dear ImGui, ImGuiFileDialog, blip-buf and glad sources come bundled in libraries/sim/src.

## linux

    cd <scratch>
    meson setup build-linux-ntr -Dbuild_target=linux -Dstandalone=true -Dc_args=-U_FORTIFY_SOURCE -Dcpp_args=-U_FORTIFY_SOURCE
    meson compile -C build-linux-ntr           # 30 s wall, 2m37 user
    # TWL hybrid, the setting for Black:
    meson setup build-linux-twl -Dbuild_target=linux -Dsdk_version_major=5 -Dhardware_type=twl_hyb -Dstandalone=true -Dc_args=-U_FORTIFY_SOURCE -Dcpp_args=-U_FORTIFY_SOURCE
    meson compile -C build-linux-twl           # 23 s

You need `-U_FORTIFY_SOURCE` on Ubuntu. Ubuntu's gcc turns on `_FORTIFY_SOURCE=3` at -O2, and libsim is compiled at -O2
(its own `cpp_args`). `calculateClipMatrix()` (libraries/sim/src/g3_handler.cpp:334) copies a whole MtxFx44
(64 bytes) into the single 4-byte register variable `s_reg_G3X_CLIPMTX_RESULT_0`, so the first `G3_Identity()`
(called by `G3X_InitMtxStack`) aborts with "*** buffer overflow detected ***". A build without the flag runs
2D-only code and dies on the first 3D init. Upstream builds on Debian 13 in docker (standalone/docker/Dockerfile),
where fortify is not on by default. The real fix belongs in libntr (one struct or array for the CLIPMTX registers).

Outputs: 33 thin static archives (libsim.a holds the SDL/GL/ImGui simulator, libsim7.a the ARM7 side: PXI, WM,
sound), `standalone/libntr` (16.4 MB, -O0 -g, a spinning-cube demo), `components/ichneumon/ichneumon_sub`.
The executable links SDL2, enet and libstdc++ dynamically.

## The tiny NitroMain program (<scratch>)

libntr's `main()` (libraries/sim/src/sim_main.cpp) sets up the simulated hardware, opens the SDL window and then calls the
game's `NitroMain()`. A game gives NitroMain and nothing else. The frame happens inside `OS_WaitVBlankIntr()`:
the main and sub engines are drawn, then the game's HBlank and VBlank IRQ handlers run.

`meson.build` (libntr as a subproject: `subprojects/libntr` is a symlink to the clone):

    project('hello', 'c', 'cpp', default_options: {'default_library': 'static'})
    add_global_arguments('-U_FORTIFY_SOURCE', language: ['c', 'cpp'], native: true)
    nitrosdk_dep = dependency('NitroSDK', fallback: ['libntr', 'nitrosdk_dep'], native: true)
    executable('hello', 'main.c', dependencies: nitrosdk_dep, native: true)
    executable('cube', 'cube.c', dependencies: nitrosdk_dep, native: true)

`main.c`, the core of it (stdio and getenv are fine: it is a PC build):

    static volatile u32 sVBlanks;
    static void VBlankIntr(void) { sVBlanks++; OS_SetIrqCheckFlag(OS_IE_V_BLANK); }

    void NitroMain(void)
    {
        OS_Init(); OS_InitTick(); GX_Init();
        OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
        (void)OS_EnableIrqMask(OS_IE_V_BLANK); (void)OS_EnableIrq(); (void)GX_VBlankIntr(TRUE);
        GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D); GXS_SetGraphicsMode(GX_BGMODE_0);
        GX_DispOn(); GXS_DispOn();
        for (frame = 0; frame < maxFrames; frame++) {   /* HELLO_FRAMES, default 120 */
            u16 top = GX_RGB(31, frame & 31, 0), bottom = GX_RGB(0, 0, 31);
            GX_LoadBGPltt(&top, 0, 2); GXS_LoadBGPltt(&bottom, 0, 2);
            OS_WaitVBlankIntr();
            /* frame 30, if HELLO_SHOT=file.png: system("xwd -name 'libntr Application' | xwdtopnm | pnmtopng") */
        }
        /* print frames, VBlank IRQs, OS_GetTick milliseconds and fps; exit(0) */
    }

`cube.c` is libntr's standalone/main.c (DEMO init, a G3 cube) with the same frame counter, screenshot and exit.

    # hello must be a git repo with one commit first (see "Consumer must be a git repo" below)
    cd <scratch>
    meson setup build-linux-ntr -Dlibntr:build_target=linux
    meson setup build-linux-twl -Dlibntr:build_target=linux -Dlibntr:sdk_version_major=5 -Dlibntr:hardware_type=twl_hyb
    meson compile -C build-linux-ntr           # whole libntr + hello + cube: 17-22 s
    cd <scratch>                       # the program writes sim_config.ini, imgui.ini, DbgLog.txt in its cwd
    SDL_AUDIODRIVER=dummy HELLO_FRAMES=300 timeout -s KILL 60 xvfb-run -a <scratch>
    SDL_AUDIODRIVER=dummy HELLO_FRAMES=300 timeout -s KILL 60 <scratch>    # WSLg, DISPLAY=:0

Measured (frames, then OS_GetTick time; CapFrameRate=true and VsyncInterval=1 are the sim_config.ini defaults; the box was shared with other
workers, so fps varies run to run):

| run | result |
|---|---|
| NTR, Xvfb | 600 frames in 11.1-11.2 s (53.7-54.3 fps); 300 frames 47.9 fps |
| NTR, WSLg (XWayland, :0) | 600 frames in 14.7 s (40.7 fps); 300 frames 47.7 fps |
| TWL hybrid, Xvfb | 120 frames in 2.0 s (59.8 fps) |
| GL in both | llvmpipe (LLVM 21.1.8), OpenGL 4.5 Mesa 26.0.8: there is no GPU in this VM, so it is all software |

What a screenshot (xwd of the 512x768 window, default vertical layout at 2x) shows:
- cube (NTR and TWL): the 3D cube is drawn on the top screen over a blue backdrop. The OpenGL 4.2 3D path works.
- hello: the sub screen shows its backdrop exactly (0,0,255). The main screen is a flat (239,255,255) in every
  variant tried: palette loaded (reg read back 0x03bf), screens swapped (the colour follows the main engine),
  3D off, 3D on with a green clear colour, BG/OBJ VRAM mapped, OBJ on with all OAM entries hidden. So a main engine
  with no BG enabled does not show its backdrop. This is still open; the next step is to compare with
  DEMOInitDisplay*, which the cube uses and which looks right.
- Mesa reports one GL error per frame: `glUniform1fv(backdropColor, 4, ...)` on a vec4 (sim_main.cpp:604).
  It does no harm, because a `glProgramUniform4f` right after it sets the value.

Things to know when using libntr as a subproject:
- Consumer must be a git repo: gen/nitro/git_info/gen_git_info.py runs `git rev-parse --short HEAD` in
  the *consumer's* source dir and the build fails if it isn't one (bw-port is, so no problem there).
- Every libntr PC target is `native: true` (`native: build_target == 'linux' or ... 'win64'`), so the game's
  executable has to be `native: true` as well, and libntr's toolchain is the build-machine one.
- Passing `-Dlibntr:c_args=...` did nothing with meson 1.10 (the value stays `[]`). `add_global_arguments(...,
  native: true)` in the parent project does reach libntr (262 compile commands carry it).
- The build options `build_target`, `sdk_version_major` and `hardware_type` are `yield: true`, so the parent can
  declare the same options and own them. `-Dlibntr:...` at setup works too.
- Kill the program with `timeout -s KILL`. SIGTERM is turned into an SDL quit event that the loop never reads, and
  the stock demo kept running after a plain `timeout 6` for 4.5 minutes, with 74 threads.
- The window title comes from `header.bin` (first 0x160 bytes of a ROM) in the cwd if present, else
  "libntr Application". The game code and maker code can also be set with the `card_game_code`/`card_maker_code` options.

## win64, cross from WSL

The build works, but it is not a normal meson cross build. Because every PC target is `native: true`, a cross file
would give the *build* machine (linux gcc) to all of libntr. So mingw-w64 goes in as the build-machine compiler,
through a native file. Then meson cannot run the compiler's test executable: WSL interop fails under
`systemd-run` ("Invalid argument", even with WSL_INTEROP set), so the sanity check is skipped.

`<scratch>`:

    [binaries]
    c = 'x86_64-w64-mingw32-gcc-posix'          # posix threads: libntr uses pthreads
    cpp = 'x86_64-w64-mingw32-g++-posix'
    ar = 'x86_64-w64-mingw32-gcc-ar-posix'
    strip = 'x86_64-w64-mingw32-strip'
    windres = 'x86_64-w64-mingw32-windres'
    pkg-config = 'false'
    [built-in options]
    c_args = ['-I<scratch>', '-I<scratch>', '-U_FORTIFY_SOURCE']
    cpp_args = ['-I<scratch>', '-I<scratch>', '-U_FORTIFY_SOURCE']
    c_link_args = ['-L<scratch>', '-static-libgcc', '-static-libstdc++']
    cpp_link_args = ['-L<scratch>', '-static-libgcc', '-static-libstdc++']
    [properties]
    skip_sanity_check = true

Dependencies (Ubuntu ships no mingw SDL2 or enet):

    cd <scratch>
    curl -sSLO https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.tar.gz
    curl -sSLO http://enet.bespin.org/download/enet-1.3.18.tar.gz
    #   sha256 83a5d740...38ff2 (SDL2), 2a8a0c53...85d36 (enet)
    tar xzf SDL2-devel-2.32.10-mingw.tar.gz && cp -r SDL2-2.32.10/x86_64-w64-mingw32/* <scratch>
    tar xzf enet-1.3.18.tar.gz && cd enet-1.3.18 && mkdir -p obj
    for f in callbacks compress host list packet peer protocol unix win32; do x86_64-w64-mingw32-gcc-posix -O2 -Iinclude -c $f.c -o obj/$f.o; done
    x86_64-w64-mingw32-gcc-posix -shared -o <scratch> obj/*.o -lws2_32 -lwinmm -Wl,--out-implib,<scratch>
    cp -r include/enet <scratch>
    mkdir -p <scratch> && echo '#include <windows.h>' > <scratch>

Build:

    cd <scratch>
    meson setup build-win64-ntr --native-file <scratch> -Dbuild_target=win64 -Dstandalone=true
    meson compile -C build-win64-ntr standalone/libntr      # 24 s -> standalone/libntr.exe, 20.4 MB
    cd <scratch>
    meson setup build-win64-ntr --native-file <scratch> -Dlibntr:build_target=win64
    meson setup build-win64-twl --native-file <scratch> -Dlibntr:build_target=win64 -Dlibntr:sdk_version_major=5 -Dlibntr:hardware_type=twl_hyb
    meson compile -C build-win64-ntr hello cube              # 22 s; name the targets (see 1. below)

What it took, each found by a failing build:
1. `components/ichneumon/ichneumon_sub.lcf` (an ARM7 component) runs `tools/makelcf/makelcf`. That tool is
   `native: true` too, so here it is a PE file that can't run in WSL (code 127). A plain `meson compile` stops there.
   Building the targets you need by name avoids it: no PC executable depends on that component.
2. `#include <Windows.h>` (nanotime.h in sim and sim7, imgui.cpp): mingw's header is `windows.h`, and the Linux
   filesystem is case-sensitive. A one-line shim dir fixes it.
3. A static libenet.a leaves `timeGetTime`/`timeBeginPeriod` (winmm) unresolved, because the link line has
   `-lSDL2 -lenet -lws2_32` and no winmm. Building enet as a DLL (as MSYS2 packages it) fixes it.
4. The posix-thread mingw variant (`-posix`), because libntr uses pthreads.

`hello.exe` imports KERNEL32, USER32, SHELL32, WS2_32, msvcrt, SDL2.dll, enet.dll and libwinpthread-1.dll. Ship
the last three next to it (<scratch>, /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll).

Run on the Windows side of this host (Windows Server 2025 VM, "Microsoft Basic Display Adapter", no GPU):
- Plain: it segfaults during SDL/GL setup, because Windows' own opengl32 is GDI 1.1 and has no GL 4.2 core.
- With Mesa for Windows (pal1000/mesa-dist-win 26.2.4 release-mingw: `x64/opengl32.dll` + `x64/libgallium_wgl.dll`
  next to the exe), the default Gallium driver exits with code 87 in the first `OS_WaitVBlankIntr`.
- With `GALLIUM_DRIVER=llvmpipe` it works: hello.exe 300 frames in 9.1 s (33.0 fps), hello-twl.exe 120 frames
  (32.3 fps), cube.exe 60 frames, all exit 0. A machine with a real GPU driver needs none of this.

On a Windows machine with MSYS2, upstream's own route is Install_MSys2.ps1 and
standalone/scripts/setup_msys2.sh (ucrt64 gcc, SDL2, enet, nasm, meson), then ci_build_win64.sh or
ci_build_win64_twl.sh. That is the build its CI runs. It was not tried here.

## Not done here
- No apt mingw SDL2 or enet; the downloaded ones are not checked in anywhere.
- Sister libraries (libntrsystem, libntrwifi, libntrdwc) were not cloned or built.
- The main-engine backdrop question above, and audio (all runs used `SDL_AUDIODRIVER=dummy`).
