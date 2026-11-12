#!/bin/bash
# The win64 exe in a Windows folder, from WSL (docs/port-build.md, "win64"). From the repo root:
#
#   tools/port_win64.sh                   BUILD ONLY (build/port/win64/bwport.exe): touches nothing outside the build dir.
#                                         (Until 11-07 this form deployed to the deploy folder, C:\bwport: it no longer does.)
#   tools/port_win64.sh deploy            build bwport.exe AND fill the folder (default C:\bwport, the OWNER's: his exe is replaced):
#                                         the exe, its DLLs, Mesa's software GL
#                                         (in mesa\, used only when the system has no usable GL), bwport.bat, the ROM
#                                         (copied once), licenses\ (THIRD_PARTY.md and the notices). Then double-click bwport.bat on Windows.
#                                         Only the main session / an explicit deploy job does this; tests use --out DIR (tools/win_smoke.sh).
#   tools/port_win64.sh run [SECONDS]     run the folder's bwport.bat from WSL (Windows interop) for SECONDS (default
#                                         60, then killed); the log in <folder>/run/bwport.log, crash frames symbolized
#                                         ALWAYS HIDDEN: run and gdb set BWPORT_HIDDEN=1 (libntr-hidden-pc.patch: the window is created
#                                         hidden, never shown, no taskbar button); BWPORT_HIDDEN=0 only when a visible window is wanted
#       --shots K                         a screenshot every K seconds: <folder>/run/shots/sNNN.png (BWPORT_SHOTS: the composed frame
#                                         read back by the exe, so a hidden window is fine; with BWPORT_HIDDEN=0 a desktop capture
#                                         instead, tools/port_win64_shot.ps1: <N>s.png, the window must be in view)
#       --fresh                           the run dir emptied first (the ROM extracted again, no save.bin)
#   tools/port_win64.sh symbolize [LOG]   bwport.exe+0x... frames (port/src/win64_console.c's) -> function file:line
#   tools/port_win64.sh gdb [GDB ARGS]    the exe under gdb: Windows' gdbserver in the run dir, gdb here
#                                         (e.g. gdb -batch -ex c -ex bt)
#   tools/port_win64.sh layout            struct layouts of the linux and win64 builds compared (tools/port_layout.py)
#   tools/port_win64.sh zip [ZIP]         after a deploy: the folder's exe, DLLs, mesa\, bwport.bat and licenses\ (never run\ or a
#                                         ROM) as ZIP (default build/bwport-win64-<rev>.zip), only if tools/release_check.sh
#                                         finds no ROM bytes in it: the package one may publish
#
# Options and environment:
#   --out DIR   the folder (default /mnt/c/bwport, C:\bwport); env BWPORT_WIN64_OUT
#   --rom FILE  the ROM to copy (default the main checkout's orig/baserom_b_us.nds); --no-rom copies none
#   --also DIR  deploy to a second folder too (repeatable): the exe, DLLs, mesa\ and bwport.bat only, never its run\ folder
#               or its ROM, and nothing is deleted there but the old top-level opengl32.dll / libgallium_wgl.dll
#   --no-mesa   leave the mesa\ folder out (a machine with a GPU driver; without it a missing system GL is a message box)
#   MINGW       SDL2 and enet for mingw (default <scratch>, as port/machine/win64.ini), MESA the Mesa
#               x64 DLLs (default <scratch>): docs/libntr/build.md says how to get both
#   run passes SDL_AUDIODRIVER, GALLIUM_DRIVER, BWPORT_GL (system|mesa), BWPORT_ROM, BWPORT_DIR (paths translated), BWPORT_OVL_LOG,
#   BWPORT_OVL_POISON, BWPORT_TRACE_EXIT, BWPORT_FPSLOG (libntr: fps and ms per frame each second), BWPORT_GAME (IRAO: White), BWPORT_CFGSCRIPT (shot=NAME needs BWPORT_HIDDEN=1), BWPORT_SKIPMENU, BWPORT_KEYS (port/src/keys_script.c: a key script by time,
#   e.g. BWPORT_KEYS="start@115 a@125") to the exe when they are set
set -e
REPO=$(cd "$(dirname "$0")/.." && pwd)
BUILD=$REPO/build/port/win64
EXE=$BUILD/bwport.exe
OUT=${BWPORT_WIN64_OUT:-/mnt/c/bwport}
MINGW=${MINGW:-/root/pc/t40u/mingw}
MESA=${MESA:-/root/pc/t40u/dl/mesa/x64}
PTHREAD=/usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll
MAIN=$(cd "$REPO" && dirname "$(git rev-parse --path-format=absolute --git-common-dir)")
ROM=$MAIN/orig/baserom_b_us.nds
MESA_ON=1
ALSO=()
GDB_PORT=${GDB_PORT:-2345}

cmd=
args=()
while [ $# -gt 0 ]; do
    case "$1" in
        build|deploy|run|symbolize|gdb|layout|zip) if [ -z "$cmd" ]; then cmd=$1; else args+=("$1"); fi; shift ;;   # options may come before it
        --out) OUT=$2; shift 2 ;;
        --rom) ROM=$2; shift 2 ;;
        --no-rom) ROM=; shift ;;
        --no-mesa) MESA_ON=; shift ;;
        --also) ALSO+=("$2"); shift 2 ;;
        --shots) SHOTS=$2; shift 2 ;;
        --fresh) FRESH=1; shift ;;
        *) args+=("$1"); shift ;;
    esac
done
cmd=${cmd:-build}
RUN=$OUT/run

# the worker slice when there is one (bw-port's WSL); interop can't start Windows programs inside it
slice() {
    if systemctl is-active -q bwworkers.slice 2>/dev/null; then
        systemd-run --scope --slice=bwworkers.slice -q "$@"
    else
        "$@"
    fi
}

# a Windows program from a Windows working directory (from a Linux one, WSL's interop fails: "Invalid argument")
kill_win() {
    if [ "$1" = bwport.exe ]; then
        # only the exe running from THIS folder: another folder's game (the maintainer plays from his own) is left alone
        local w
        w=$(wslpath -w "$OUT")
        (cd "$OUT" && powershell.exe -NoProfile -Command "Get-Process bwport -ErrorAction SilentlyContinue | Where-Object { \$_.Path -like '${w}\\*' } | Stop-Process -Force" > /dev/null 2>&1) || true
        return
    fi
    (cd "$OUT" && taskkill.exe /F /IM "$1" > /dev/null 2>&1) || true
}

build() {
    # meson setup, or the wraps' patches again and a reconfigure when they changed (tools/port_wraps.py)
    python3 "$REPO/tools/port_wraps.py" "$BUILD" --native-file "$REPO/port/machine/win64.ini" -Dbuild_target=win64
    # name the target: a plain ninja stops at libntr's ARM7 component (docs/libntr/build.md, win64)
    slice ninja -C "$BUILD" bwport.exe
}

write_bat() {
    # CRLF: cmd.exe reads a batch file's lines by them
    sed 's/$/\r/' > "$1/bwport.bat" <<'BAT'
@echo off
rem Pokemon Black on PC: bw-port's win64 build (docs/port-build.md, win64; tools/port_win64.sh made this folder).
rem   bwport.bat [ROM.nds]   the first start imports the ROM into run\Pokemon Black\ and extracts its files (a few seconds); the
rem                          saves (save.bin, backups\) are there, the settings (sim_config.ini: keys, scale) in run\
rem The ROM: the argument, else BWPORT_ROM, else the first .nds in this folder. Keys (sim_config.ini): A=X B=Z X=S
rem Y=A L=Q R=W Start=Enter Select=right Shift, the mouse is the touch screen, ` the debug window, Tab the frame cap.
setlocal
rem pushd, not cd: started from a network share (\\server\...) it maps a drive letter (cd /d cannot)
pushd "%~dp0"
if not "%~1"=="" set "BWPORT_ROM=%~f1"
if not defined BWPORT_ROM for %%f in ("%~dp0*.nds") do if not defined BWPORT_ROM set "BWPORT_ROM=%%~ff"
rem No BWPORT_DIR: the exe works in run\ next to itself (it makes it) and shows the launcher (Games window, ROM library, saves);
rem set BWPORT_DIR to run one game in a flat folder with no launcher, as the test tools do.
rem OpenGL: the exe uses the system's (a graphics driver's) and falls back to the Mesa software GL in mesa\ when the
rem system has none that works; it says which in the log. BWPORT_GL=system or mesa forces one.
"%~dp0bwport.exe"
set RC=%ERRORLEVEL%
if not "%RC%"=="0" (
    echo bwport.exe ended with code %RC%
    if not defined BWPORT_NOPAUSE pause
)
popd
exit /b %RC%
BAT
}

# the exe, its DLLs, mesa\, licenses\ and bwport.bat into one folder (not its run\ folder, not the ROM)
fill_folder() {
    local dst=$1
    mkdir -p "$dst"
    cp "$EXE" "$dst/"
    cp "$MINGW/bin/SDL2.dll" "$MINGW/bin/enet.dll" "$PTHREAD" "$dst/"
    # Mesa's software GL sits in a subfolder: next to the exe it would be loaded instead of the system's GL (a real
    # graphics driver's); the exe loads it itself (port/src/gl_select.c) when the system's GL is missing or unusable.
    # Older folders had both DLLs beside the exe: they must go.
    rm -f "$dst/opengl32.dll" "$dst/libgallium_wgl.dll"
    if [ -n "$MESA_ON" ]; then
        mkdir -p "$dst/mesa"
        cp "$MESA/opengl32.dll" "$MESA/libgallium_wgl.dll" "$dst/mesa/"
    else
        rm -rf "$dst/mesa"
    fi
    # the notices of what the folder ships (SDL, enet, Mesa, winpthreads: MIT, zlib, Apache-2.0 with LLVM exceptions) and ours
    rm -rf "$dst/licenses"
    mkdir -p "$dst/licenses"
    cp "$REPO"/licenses/* "$REPO/THIRD_PARTY.md" "$REPO/LICENSE" "$dst/licenses/"
    write_bat "$dst"
}

deploy() {
    build
    mkdir -p "$OUT" "$RUN"
    kill_win bwport.exe   # a running one holds its exe
    fill_folder "$OUT"
    if [ -n "$ROM" ]; then
        dest=$OUT/$(basename "$ROM")
        if [ ! -f "$dest" ] || [ "$(stat -c %s "$dest")" != "$(stat -c %s "$ROM")" ]; then
            cp "$ROM" "$dest"
        fi
    fi
    echo "$OUT: $(cd "$OUT" && ls | tr '\n' ' ')"
    local also
    for also in "${ALSO[@]}"; do
        fill_folder "$also"
        echo "$also: $(cd "$also" && ls | tr '\n' ' ')"
    done
}

# bwport.exe+0xRVA -> the function and line at the exe's ImageBase + RVA
symbolize() {
    local base
    base=$(x86_64-w64-mingw32-objdump -p "$EXE" | awk '/^ImageBase/ {print $2}')
    while IFS= read -r line; do
        if [[ $line =~ bwport\.exe\+0x([0-9a-fA-F]+) ]]; then
            addr=$(printf '0x%x' $((0x$base + 0x${BASH_REMATCH[1]})))
            where=$(x86_64-w64-mingw32-addr2line -f -i -p -e "$EXE" "$addr" | sed "s#$REPO/##g; s#[^ ]*/\.\./\.\./\.\./##g" | tr '\n' ';')
            echo "$line  $where"
        else
            echo "$line"
        fi
    done
}

# the variables the exe takes, through WSLENV (/p: a path, translated)
wslenv() {
    local v e=
    for v in SDL_AUDIODRIVER GALLIUM_DRIVER BWPORT_OVL_LOG BWPORT_OVL_POISON BWPORT_TRACE_EXIT BWPORT_NOPAUSE \
        BWPORT_GL BWPORT_KEYS BWPORT_FPSLOG LP_NUM_THREADS BWPORT_VCORE_THREADS BWPORT_VCORE_PIPE BWPORT_VCORE_BANDS BWPORT_SWITCH_TEST BWPORT_SWITCH_START BWPORT_RENDERER BWPORT_RESOLUTION BWPORT_FRAMESKIP BWPORT_AUDIOLOG BWPORT_VCORE_POOL BWPORT_NOPRESENT BWPORT_BREAKDOWN BWPORT_PERFLOG BWPORT_GAME BWPORT_CFGSCRIPT BWPORT_HIDDEN BWPORT_SHOTS BWPORT_SKIPMENU; do
        if [ -n "${!v+x}" ]; then e=$e:$v; fi
    done
    for v in BWPORT_ROM BWPORT_DIR BWPORT_FRAMELOG; do
        if [ -n "${!v+x}" ]; then e=$e:$v/p; fi
    done
    echo "${e#:}${WSLENV:+:$WSLENV}"
}

run() {
    local secs=${args[0]:-60} rc shooter= t hidden
    [ -n "$FRESH" ] && rm -rf "$RUN"
    mkdir -p "$RUN" "$OUT/tools"
    export BWPORT_NOPAUSE=1
    export BWPORT_HIDDEN=${BWPORT_HIDDEN:-1}   # no window on the desktop (libntr-hidden-pc.patch)
    hidden=1; [ "$BWPORT_HIDDEN" = 0 ] && hidden=
    echo "bwport: $OUT/bwport.bat for $secs s, log $RUN/bwport.log"
    set +e
    if [ -n "$SHOTS" ] && [ -n "$hidden" ]; then
        # the exe writes them itself (BWPORT_SHOTS=DIR,S,S,...: DIR/sNNN.png), a Windows path with forward slashes
        rm -rf "$RUN/shots" && mkdir -p "$RUN/shots"
        local list=
        for ((t = SHOTS; t < secs; t += SHOTS)); do list=$list,$t; done
        export BWPORT_SHOTS="$(wslpath -m "$RUN/shots")$list"
    elif [ -n "$SHOTS" ]; then
        rm -rf "$RUN/shots" && mkdir -p "$RUN/shots"
        cp "$REPO/tools/port_win64_shot.ps1" "$OUT/tools/"
        # Windows programs started from WSL want a Windows working directory
        (cd "$RUN" && for ((t = SHOTS; t < secs; t += SHOTS)); do
            sleep "$SHOTS"
            powershell.exe -NoProfile -ExecutionPolicy Bypass -File "$(wslpath -w "$OUT/tools/port_win64_shot.ps1")" \
                -Out "$(wslpath -w "$RUN/shots")\\${t}s.png" > /dev/null 2>&1
        done) &
        shooter=$!
    fi
    (cd "$OUT" && WSLENV=$(wslenv) timeout -s KILL "$secs" cmd.exe /c "$(wslpath -w "$OUT/bwport.bat")") > "$RUN/bwport.log" 2>&1
    rc=$?
    # the timeout ends WSL's side only
    kill_win bwport.exe
    [ -n "$shooter" ] && kill "$shooter" 2> /dev/null
    set -e
    grep -v '^ovl: ' "$RUN/bwport.log" | symbolize | tail -40
    if [ $rc = 124 ] || [ $rc = 137 ]; then
        echo "bwport: still running after $secs s (killed)"
    else
        echo "bwport: exit code $rc"
    fi
    if [ -n "$SHOTS" ]; then
        echo "bwport: screenshots $RUN/shots: $(cd "$RUN/shots" && ls | sort -n | tr '\n' ' ')"
    fi
}

gdb_run() {
    local tools=$OUT/tools
    mkdir -p "$tools" "$RUN"
    # gdbserver for Windows (apt: gdb-mingw-w64-target) serves on a port only, and WSL's NAT network doesn't reach
    # the Windows host's 127.0.0.1: gdb talks to it through port_win64_relay.exe's stdio
    [ -f "$tools/gdbserver.exe" ] || cp /usr/share/win64/gdbserver.exe "$tools/"
    if [ ! -f "$tools/port_win64_relay.exe" ] || [ "$REPO/tools/port_win64_relay.c" -nt "$tools/port_win64_relay.exe" ]; then
        x86_64-w64-mingw32-gcc -O2 -o "$tools/port_win64_relay.exe" "$REPO/tools/port_win64_relay.c" -lws2_32
    fi
    kill_win gdbserver.exe
    # Windows programs started from WSL need a Windows working directory (a Linux one: "Invalid argument")
    cd "$RUN"
    export BWPORT_HIDDEN=${BWPORT_HIDDEN:-1}
    WSLENV=$(wslenv) "$tools/gdbserver.exe" --once "127.0.0.1:$GDB_PORT" "$(wslpath -m "$OUT/bwport.exe")" \
        > "$RUN/gdbserver.log" 2>&1 < /dev/null &
    echo "bwport: the game's output in $RUN/gdbserver.log" >&2
    set +e
    x86_64-w64-mingw32-gdb -q -ex 'set pagination off' -ex 'set confirm off' \
        -ex "target remote | $tools/port_win64_relay.exe $GDB_PORT" "${args[@]}" "$EXE"
    kill_win gdbserver.exe
    kill_win bwport.exe
    wait
    return 0
}

# The release package: the deployed folder's files a player needs, checked by the release gate before it exists
release_zip() {
    local zip=${args[0]:-$REPO/build/bwport-win64-$(git -C "$REPO" rev-parse --short=9 HEAD).zip} stage
    [ -f "$OUT/bwport.exe" ] || { echo "no $OUT/bwport.exe: deploy first"; exit 1; }
    stage=$(mktemp -d)
    mkdir -p "$stage/bwport"
    (cd "$OUT" && cp -r bwport.exe ./*.dll bwport.bat licenses "$stage/bwport/" && { [ ! -d mesa ] || cp -r mesa "$stage/bwport/"; })
    # without the debug info (its strings name the SDK's enums, as the ROM's Wi-Fi code does); symbolize reads the build's exe
    x86_64-w64-mingw32-strip --strip-debug "$stage/bwport/bwport.exe"
    rm -f "$zip"
    (cd "$stage" && python3 -m zipfile -c "$zip" bwport)
    rm -rf "$stage"
    if ! bash "$REPO/tools/release_check.sh" "$zip"; then
        rm -f "$zip"
        echo "no zip: the release gate refused it"
        exit 1
    fi
    echo "$zip"
}

case $cmd in
    build) build; echo "bwport: built $EXE (not deployed: 'deploy' fills $OUT)" ;;
    deploy) deploy ;;
    zip) release_zip ;;
    run) run ;;
    symbolize) if [ ${#args[@]} -gt 0 ]; then symbolize < "${args[0]}"; else symbolize; fi ;;
    gdb) gdb_run ;;
    layout) python3 "$REPO/tools/port_layout.py" "$REPO/build/port/linux/bwport" "$EXE" ;;
esac
