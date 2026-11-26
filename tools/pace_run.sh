#!/bin/bash
# tools/pace_run.sh: pacing A/B on Linux headless (a pretended display link) with BWPORT_PRESENTLOG + tools/pace_report.py.
#   bash tools/pace_run.sh [-o DIR] [-s SECONDS] [-p PLAY] [-r RENDERER] HZ...
# For each panel rate HZ: the new pacing (head) and BWPORT_INTERPLEAD=0 BWPORT_SLOTCHAIN=0 (old), alternating, one report line each.
OUT=${PACE_OUT:-build/pace}
PLAY=ivf-pace; REND=; EXTRA=; PASSES=2
while getopts "o:p:r:x:n:" o; do case $o in o) OUT=$OPTARG;; p) PLAY=$OPTARG;; r) REND=$OPTARG;; x) EXTRA=$OPTARG;; n) PASSES=$OPTARG;; esac; done
shift $((OPTIND-1))
HZS=${*:-"60 120 144"}
cd "$(git rev-parse --show-toplevel)"
mkdir -p "$OUT"; OUT=$(cd "$OUT" && pwd)   # absolute: the game runs in its own folder
run() { # tag hz extra-env
  local tag=$1 hz=$2; shift 2
  local log=$OUT/${tag}_$hz.log
  rm -f "$log"
  env BWPORT_PANELHZ=$hz BWPORT_SMOOTH=on BWPORT_FRAMERATE=monitor BWPORT_PRESENTLOG=$log ${REND:+BWPORT_RENDERER=$REND} $EXTRA "$@" \
    python3 tools/port_run.py --play $PLAY --realtime --skip 1 -t 110 2>&1 | tail -1 | cut -c1-140
  python3 tools/pace_report.py "$log"
}
for i in $(seq 1 $PASSES); do
  for hz in $HZS; do
    run new $hz
    run old $hz BWPORT_INTERPLEAD=0 BWPORT_SLOTCHAIN=0
  done
done
