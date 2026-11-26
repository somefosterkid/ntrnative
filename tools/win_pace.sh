#!/bin/bash
# the pacing A/B on the Windows exe, hidden (tools/win_smoke.sh): bash tools/win_pace.sh [-n PASSES] [-s SECS] [-e EXEDIR] HZ...
#   HZ 0 = no pretended link (the real vsync of the exe's own window). Logs: $OUTD/win_<new|old>_<hz>_<pass>.log + a report line each.
# The exe folder is a private copy of an existing win64 folder (EXEDIR, PACE_EXE, never the deploy folder.
PASSES=1; SECS=75; EXEDIR=${PACE_EXE:-}; SAVE=town; REND=
while getopts "n:s:e:r:" o; do case $o in n) PASSES=$OPTARG;; s) SECS=$OPTARG;; e) EXEDIR=$OPTARG;; r) REND=$OPTARG;; esac; done
shift $((OPTIND-1))
HZS=${*:-"60 120 144"}
cd "$(git rev-parse --show-toplevel)"
OUTD=${PACE_OUT:-build/pace}; mkdir -p "$OUTD"; OUTD=$(cd "$OUTD" && pwd)
[ -n "$EXEDIR" ] || { echo "win_pace: set PACE_EXE (or -e) to a win64 exe folder, e.g. the one tools/win_smoke.sh --out fills"; exit 2; }
WD=${PACE_WIN:-${EXEDIR}_pace}
if [ ! -f "$WD/bwport.exe" ]; then
  mkdir -p "$WD"
  for f in "$EXEDIR"/*; do [ "$(basename "$f")" = run ] || cp -r "$f" "$WD/"; done
fi
KEYS=$(python3 tools/play_script.py 300 40 left5 up8 right5 down8 left5 up8 right5 down8 left5 up8 right5 down8)
one() { # tag hz pass extra-env...
  local tag=$1 hz=$2 pass=$3; shift 3
  local log=$OUTD/win_${tag}_${hz}_$pass.log
  rm -f "$log" "$WD/pl.log"
  env BWPORT_SMOOTH=on BWPORT_FRAMERATE=monitor BWPORT_PRESENTLOG="$WD/pl.log" ${hz:+BWPORT_PANELHZ=$hz} ${REND:+BWPORT_RENDERER=$REND} "$@" \
    bash tools/win_smoke.sh --no-build --out "$WD" --save $SAVE --keys "$KEYS" --shots 0 $SECS 2>&1 | grep -a -E "window check|MainWindow|fps" | tail -2 | cut -c1-160
  cp "$WD/pl.log" "$log" 2>/dev/null   # (the exe writes on the Windows side: a C: path)
  python3 tools/pace_report.py "$log"
}
for p in $(seq 1 $PASSES); do
  for hz in $HZS; do
    one new $hz $p
    one old $hz $p BWPORT_INTERPLEAD=0 BWPORT_SLOTCHAIN=0
  done
done
echo "win folder: $WD"
