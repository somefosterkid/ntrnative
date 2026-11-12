#!/bin/bash
# macOS run: tools/mac_run.sh [--save NAME] [--store NAME] RUNDIR SECONDS [KEYS] [SHOT_SECONDS,...]   (the maintainers' notes, "Mac")
# Runs build/port/macos/bwport in RUNDIR (its own folder: extracted ROM, save.bin, sim_config.ini) for SECONDS, then kills it by PID.
# A new RUNDIR is an APFS clone of the template $HOME/bw-cache/runbase (ROM extracted once; tools/mac_runbase.sh), so no ~15 s extraction.
# --save NAME   start from $HOME/bw-saves/NAME.bin (README lists them): copied to save.bin, and BWPORT_SKIPMENU=1 goes straight into
#               CONTINUE (no title or menu; the log's "field: up at V-blank N", printed at the end, is the first frame the player can move:
#               ~f220 from the room). BWPORT_CONTINUE_KEYS="start@f600 start@f750 start@f900 a@f1000" presses the title's keys instead
#               (a build without the skip; the field then comes up at ~f1250)
# --realtime    the game's own pace (battle speed, timing and audio checks need it). Default: BWPORT_FRAMERATE=unlimited and the Max
#               fast-forward path from frame 0 (cfgscript f0:ff=1; BWPORT_FFMAX=0 turns Max off), so a test does not wait in real
#               time; a run with seconds-based keys or shots runs in real time by itself (a note says so)
# --noskip      no frame skip. Default: BWPORT_FRAMESKIP=4 (logic every frame, the picture 1 frame in 4; screenshots are always drawn: libntr-ios-shotdraw.patch)
#               for runs that check what the game DOES; a run that checks a SCREEN or times anything uses --noskip (tools/mac_perf.py does)
# --root       RUNDIR is a launcher root (docs/launcher.md; a new one starts empty): no BWPORT_DIR, the exe starts in RUNDIR, BWPORT_ROM only when you
#               set it (the root's own .nds otherwise); --save (and --store) use RUNDIR/GAME/save.bin once that folder exists,
#               GAME by the save's name: b2-* Pokemon Black 2, w2-* Pokemon White 2, w-* and white-* Pokemon White, else Pokemon Black
# --store NAME  after the run keep RUNDIR/save.bin as $HOME/bw-saves/NAME.bin (add its line to the README)
# A crash prints a backtrace (port/src/crash_bt.c) and exits without macOS's dialog; lldb can't attach from a sandboxed shell.
# KEYS is BWPORT_KEYS ("start@f600 a@f1700"), frames counted from boot. SHOTS: fN entries (f1700) are frame-exact cfgscript shot= steps
# (RUNDIR/fN.png), seconds make RUNDIR/sNNN.png (BWPORT_SHOTS). When every key and shot is frame-counted and BWPORT_CFGSCRIPT is not
# set, the run ends itself (cfgscript quit=1) 30 frames after the last; SECONDS stays the cap. fN shots with your own BWPORT_CFGSCRIPT:
# put shot=NAME steps in it instead.
# BWPORT_EXE=PATH runs another build dir's exe (e.g. build/port/macos-o0/bwport). Env passes through: BWPORT_ROM (default orig/baserom_b_us.nds of the main checkout), BWPORT_FPSLOG=1, BWPORT_RENDERER, ...
set -u
ROOT=$(cd "$(dirname "$0")/.." && pwd)
SAVES=${BWPORT_SAVES:-$HOME/bw-saves}
SAVE=; STORE=; ROOTMODE=; SKIP=${BWPORT_FRAMESKIP-4}; REALTIME=
while [ $# -gt 0 ]; do
  case $1 in --save) SAVE=$2; shift 2;; --store) STORE=$2; shift 2;; --noskip) SKIP=0; shift;; --root) ROOTMODE=1; shift;;
    --realtime) REALTIME=1; shift;; *) break;; esac
done
RUN=$1; SECS=$2; KEYS=${3:-}; SHOTS=${4:-}
# a full disk stops every agent and the agent harness itself (2026-11-04: 707 forgotten run folders): refuse under 15 GB free
FREE_KB=$(df -k "$(dirname "$RUN")" 2> /dev/null | awk 'NR==2 {print $4}')
if [ -n "$FREE_KB" ] && [ "$FREE_KB" -lt 15728640 ]; then
  echo "mac_run: only $((FREE_KB / 1048576)) GB free: clean up first (tools/scratch_gc.sh DIR; docs/port-worker.md \"Disk\")"; exit 1
fi
[ -z "$ROOTMODE" ] && export BWPORT_ROM=${BWPORT_ROM:-$("$ROOT/tools/mac_rom.sh")}
if [ ! -d "$RUN" ] && [ -z "$ROOTMODE" ]; then   # a new root (--root) starts empty
  BASE=$("$ROOT/tools/mac_runbase.sh") || exit 1
  mkdir -p "$(dirname "$RUN")"; cp -cR "$BASE" "$RUN"
fi
mkdir -p "$RUN"; RUN=$(cd "$RUN" && pwd)
# a launcher root's game folder, by the save's name (README: b2-*, w2-* are Black 2's and White 2's)
case ${SAVE:-$STORE} in b2-*) GAMEDIR="Pokemon Black 2";; w2-*) GAMEDIR="Pokemon White 2";; white-*|w-*) GAMEDIR="Pokemon White";; *) GAMEDIR="Pokemon Black";; esac
SAVEBIN=$RUN/save.bin
[ -n "$ROOTMODE" ] && [ -d "$RUN/$GAMEDIR" ] && SAVEBIN=$RUN/$GAMEDIR/save.bin
if [ -n "$SAVE" ]; then
  [ -f "$SAVES/$SAVE.bin" ] || { echo "mac_run: no save $SAVES/$SAVE.bin"; exit 1; }
  cp "$SAVES/$SAVE.bin" "$SAVEBIN"
  if [ -n "${BWPORT_CONTINUE_KEYS:-}" ]; then   # the title (up by f600): START x3, A = CONTINUE; the field is up by ~f1250
    KEYS="$BWPORT_CONTINUE_KEYS $KEYS"; export BWPORT_SKIPMENU=0
  else
    export BWPORT_SKIPMENU=${BWPORT_SKIPMENU:-1}   # port/src/skip_menu.c
  fi
fi
# fN shots -> cfgscript steps; every key and shot frame-counted (and no cfgscript of the caller's) -> quit=1 after the last
MAXSHOTS=0; [ -z "$REALTIME" ] && [ "${BWPORT_FFMAX:-1}" != 0 ] && MAXSHOTS=1
PLAN=$(python3 -c '
import re, sys
keys, shots = sys.argv[1].split(), [s for s in sys.argv[2].split(",") if s]
frames, secs = [], 0
for k in keys:
    hold, _, when = k.rpartition("@")
    h = re.search(r"\*f(\d+)$", hold)
    for w in when.split("-"):
        if w.startswith("f") and w[1:].isdigit(): frames.append(int(w[1:]) + (int(h.group(1)) if h else 4))
        else: secs = 1
fshots = [s for s in shots if s.startswith("f") and s[1:].isdigit()]
frames += [int(s[1:]) for s in fshots]
ev = [(n, 1, "shot=f%d" % n) for n in sorted({int(s[1:]) for s in fshots})]
if sys.argv[3] == "1":   # Max does not draw the shot frame itself: leave Max (ff=0, the frame-skip path) 4 frames around each shot
    win = []
    for n, _, _ in ev:
        if win and n - 4 <= win[-1][1]: win[-1][1] = n + 4
        else: win.append([max(n - 4, 1), n + 4])
    ev += [(a, 0, "ff=0") for a, b in win] + [(b, 2, "ff=1") for a, b in win]
steps = ";".join("f%d:%s" % (n, t) for n, _, t in sorted(ev))
rest = ",".join(s for s in shots if s not in fshots)
print(bool(secs or rest), max(frames) + 30 if frames else 0, steps or "-", rest or "-")
' "$KEYS" "$SHOTS" "$MAXSHOTS")
read -r SECKEYS LAST FSTEPS SHOTS <<< "$PLAN"
[ "$FSTEPS" = - ] && FSTEPS=; [ "$SHOTS" = - ] && SHOTS=
if [ -n "${BWPORT_CFGSCRIPT:-}" ]; then
  [ -n "$FSTEPS" ] && { echo "mac_run: fN shots with BWPORT_CFGSCRIPT: put shot=NAME steps in the script"; exit 1; }
elif [ "$SECKEYS" = False ] && [ "$LAST" != 0 ]; then
  export BWPORT_CFGSCRIPT="${FSTEPS:+$FSTEPS;}f$LAST:quit=1"
fi
if [ -z "$REALTIME" ]; then
  if [ "$SECKEYS" = True ]; then echo "mac_run: keys or shots in seconds: the run is in real time (write them fN to run uncapped)"
  else
    export BWPORT_FRAMERATE=${BWPORT_FRAMERATE:-unlimited}
    # Max (cfgscript ff=1, the uncapped fast-forward path: ~20x the unlimited setting's ~240 frames/s) from frame 0, unless the
    # caller's script sets ff itself or BWPORT_FFMAX=0
    case ${BWPORT_CFGSCRIPT:-} in *ff=*) ;; *) [ "${BWPORT_FFMAX:-1}" != 0 ] && export BWPORT_CFGSCRIPT="f0:ff=1${BWPORT_CFGSCRIPT:+;$BWPORT_CFGSCRIPT}";; esac
  fi
fi
[ -z "$ROOTMODE" ] && export BWPORT_DIR=$RUN
# music bus joined: no "Music at normal speed" split, so every sound state the game reads (a jingle's end) advances by game frames,
# not wall time, and a run's V-blank counts do not depend on speed or load (docs/port-worker.md, "Routes")
export BWPORT_MUSICNORMAL=${BWPORT_MUSICNORMAL:-off}
export SDL_AUDIODRIVER=${SDL_AUDIODRIVER:-dummy}
[ -n "$KEYS" ] && export BWPORT_KEYS=$KEYS
[ -n "$SHOTS" ] && export BWPORT_SHOTS=$RUN,$SHOTS
export BWPORT_FRAMESKIP=$SKIP   # 0 = off (also against a FrameSkip= in the run folder's sim_config.ini)
export BWPORT_HIDDEN=${BWPORT_HIDDEN:-1}   # no window, no Dock icon (libntr-ios-hidden.patch); BWPORT_HIDDEN=0 for a visible run
EXE=${BWPORT_EXE:-$ROOT/build/port/macos/bwport}; case $EXE in /*) ;; *) EXE=$PWD/$EXE;; esac   # the exe starts in RUN
(cd "$RUN" && exec "$EXE") > "$RUN/log.txt" 2>&1 &
PID=$!
( sleep "$SECS"; kill -9 $PID 2>/dev/null ) > /dev/null 2>&1 &
KILLER=$!
wait $PID
echo "exit=$?"
# a cfgscript quit=1 ends the exe early: stop the timer, which would otherwise kill -9 the PID after SECS (it may be reused by
# then); its sleep runs out alone, with no hold on our output (a `| tail` of this script waited SECS for it)
{ kill $KILLER && wait $KILLER; } 2>/dev/null
if [ -n "$STORE" ]; then
  mkdir -p "$SAVES"; cp "$SAVEBIN" "$SAVES/$STORE.bin" && echo "mac_run: stored $SAVES/$STORE.bin: add its line to $SAVES/README"
fi

grep -A14 "CRASH" "$RUN/log.txt" | cut -c1-160
tail -n 2 "$RUN/log.txt" | cut -c1-200
grep -m1 "bwport: field: up at" "$RUN/log.txt" | sed 's/^bwport: /mac_run: /'
