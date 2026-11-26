#!/bin/bash
# BEFORE/AFTER table of the Gen 4 Walk/Run speeds (docs/speed-options.md "HG/SS follower"): tools/speed4_measure.sh for walk/run x 1/2/4
# env: OUT (default: its own run dir ~/.cache/bwport/speed4/table.PID, so two tables on one machine never clobber each other; a
# caller's OUT is never emptied), HOLD frames, PAR parallel runs, PTSAVE/PTDIR the Platinum save and direction (default pt-floaroma up: it runs
# north from CONTINUE, pt-r202 stands still), TMO the per-run watchdog seconds (default 600, a loaded machine needs more than 150).
cd "$(git rev-parse --show-toplevel)" || exit 2
# OUT (the logs of the runs) is the caller's, passed on untouched and never deleted; unset, it is this table's own dir
[ -z "${OUT:-}" ] && bash tools/speed4_prune.sh "$HOME/.cache/bwport/speed4" 2>/dev/null   # older tables/runs of the default dir (never a caller's OUT)
export OUT=${OUT:-$HOME/.cache/bwport/speed4/table.$$}
mkdir -p "$OUT"
# the per-run result files live in OUR OWN temp dir (removed on exit)
R=$(mktemp -d "${TMPDIR:-/tmp}/speed4_table.XXXXXX") || exit 2; export R
trap 'rm -rf "$R"' EXIT
H=${HOLD:-60}
jobs=""
for g in "heartgold hg-ecruteak up" "soulsilver ss-ecruteak up" "platinum ${PTSAVE:-pt-floaroma} ${PTDIR:-up}"; do
    for k in walk run; do for s in 1 2 4; do jobs="$jobs$g $k $s $H
"; done; done
done
printf "%s" "$jobs" | xargs -P ${PAR:-6} -L1 bash -c 'bash tools/speed4_measure.sh $0 $1 $2 $3 $4 $5 > $R/res_$0_$3$4.txt 2>&1'
for g in heartgold soulsilver platinum; do for k in walk run; do for s in 1 2 4; do cat $R/res_${g}_$k$s.txt; done; done; done
