#!/bin/bash
# vcap_corpus.sh OUTDIR [SET...]
# Records a capture corpus (.vcs frame-state files) for the picture A/B tools (pics_ab.sh, vcore_ab.sh, fast_ab.sh) with the port
# app and the maintainer's saves: BWPORT_VCAPTURE=DIR,FRAMES makes the app write the 3D and 2D state of those frames (works under the
# Metal present too, with the shot chunk zeroed). Each set is one short run from a save (the title is skipped: CONTINUE is pressed):
#   town    save "town"    f1401 (Nuvema Town, the scene the older corpora used)       -> OUTDIR/town/f001401.vcs
#   room    save "room"    f1401 (the wrecked room)                                    -> OUTDIR/room/
#   lab     save "lab"     f1401 (Juniper's lab)                                       -> OUTDIR/lab/
#   battle  save "caught"  Route 1 in the tall grass: up/down every 30 frames from f1150 until a wild battle starts; a capture every
#                          100 frames f1500..f2900 (the transition, the battle) -> OUTDIR/battle/ (15 files; the battle may
#                          start earlier or later: the encounter is random, so look at the files' sizes or render a few)
# No SET = all four. BWPORT_BATTLE_SAVE=route1 starts the battle set from Route 1's south end instead.
# Where the app comes from: macOS: tools/mac_run.sh (build/port/macos/bwport, or BWPORT_EXE); elsewhere: tools/port_run.py (Linux
# exe, headless in its own Xvfb) -- the Linux path is written from the maintainers' notes and UNTESTED. Saves: BWPORT_SAVES (default
# $HOME/bw-saves on macOS, ~/bw-saves elsewhere, <saves> where that exists; the library README lists them).
# Knobs: BWPORT_CORPUS_CFG = the cfgscript step at f1100 (default renderer=gpu,resolution=3: what the captures were taken under);
# BWPORT_BENCH_CLOCK / BWPORT_BENCH_SEED pin the clock and the random seed (defaults 2026-10-29T12:00:00 / 990).
# CAPTURES HOLD DATA EXTRACTED FROM THE ROM: never commit them. This script refuses an OUTDIR inside the repo unless it is under
# build/ (git-ignored); put it there or outside the repo. Run folders (the extracted ROM) are made under OUTDIR/.run and removed
# after each set. Prints one line per set: the file count, or "FAILED" with the log's tail.
# Use: tools/vcap_corpus.sh build/corpus; BWPORT_CAPTURES=$PWD/build/corpus tools/pics_ab.sh OLD NEW tag
set -u
REPO=$(cd "$(dirname "$0")/.." && pwd -P)
[ $# -ge 1 ] || { sed -n 2,3p "$0"; exit 2; }
OUT=$1; shift
case $OUT in /*) ;; *) OUT=$PWD/$OUT ;; esac
# resolve through the deepest existing ancestor (symlinks, ..) without creating anything inside the repo before the check
rest=; probe=$OUT
while [ ! -d "$probe" ]; do rest=/$(basename "$probe")$rest; probe=$(dirname "$probe"); done
OUT=$(cd "$probe" && pwd -P)$rest
case $OUT/ in
  "$REPO"/build/*) ;;
  "$REPO"/*) echo "vcap_corpus: refusing $OUT: inside the repo and not under build/ (captures hold ROM data and must never be committed)"; exit 2 ;;
esac
mkdir -p "$OUT"
SETS=${*:-town room lab battle}

if [ "$(uname)" = Darwin ]; then
  SAVES=${BWPORT_SAVES:-$HOME/bw-saves}
else
  SAVES=${BWPORT_SAVES:-$([ -d /root/saves ] && echo /root/saves || echo "$HOME/bw-saves")}
fi
export BWPORT_BENCH_CLOCK=${BWPORT_BENCH_CLOCK:-2026-10-29T12:00:00} BWPORT_BENCH_SEED=${BWPORT_BENCH_SEED:-990}
export BWPORT_FRAMERATE=unlimited
export BWPORT_CONTINUE_KEYS=${BWPORT_CONTINUE_KEYS:-start@f600 start@f750 start@f900 a@f1000}   # the corpus's scenes: the title's CONTINUE (mac_run.sh --save skips the menu otherwise)
CFG=${BWPORT_CORPUS_CFG:-renderer=gpu,resolution=3}

# record NAME SAVE CAPSPEC LASTFRAME KEYS: one run; the app quits itself at LASTFRAME+10 (cfgscript quit=1), the run tool's time is the backstop
record() {
  local name=$1 save=$2 spec=$3 last=$4 keys=$5 run=$OUT/.run secs=300
  local dir=$OUT/$name
  rm -rf "${run:?}" "${dir:?}"
  mkdir -p "$dir"
  export BWPORT_VCAPTURE=$dir,$spec
  export BWPORT_CFGSCRIPT="f1100:$CFG;f$((last + 10)):quit=1"
  if [ "$(uname)" = Darwin ]; then
    "$REPO/tools/mac_run.sh" --save "$save" "$run" $secs "$keys" > "$OUT/$name.log" 2>&1
  else
    # untested: port_run.py --save takes a path; it builds, runs under gdb in Xvfb, --keys are frame-exact (fN)
    python3 "$REPO/tools/port_run.py" --dir "$run" --save "$SAVES/$save.bin" -t $secs --shots 0 \
      --keys "start@f600 start@f750 start@f900 a@f1000 $keys" > "$OUT/$name.log" 2>&1
  fi
  local n
  n=$(find "$dir" -name '*.vcs' | wc -l | tr -d ' ')
  if [ "$n" -gt 0 ]; then echo "$name: $n captures in $dir"; else echo "$name: FAILED (no captures); $(tail -n 3 "$OUT/$name.log" | cut -c1-160)"; fi
  rm -rf "${run:?}"
}

for s in $SETS; do
  case $s in
    town) record town town 1401 1401 "" ;;
    room) record room room 1401 1401 "" ;;
    lab) record lab lab 1401 1401 "" ;;
    battle)
      K=
      for i in $(seq 0 60); do
        if [ $((i % 2)) = 0 ]; then K="$K up@f$((1150 + i * 30))"; else K="$K down@f$((1150 + i * 30))"; fi
      done
      record battle "${BWPORT_BATTLE_SAVE:-caught}" 1500-2900/100 2900 "$K" ;;
    *) echo "vcap_corpus: unknown set $s (town room lab battle)"; exit 2 ;;
  esac
done
