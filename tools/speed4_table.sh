#!/bin/bash
# BEFORE/AFTER table of the Gen 4 Walk/Run speeds (docs/speed-options.md "HG/SS follower"): tools/speed4_measure.sh for walk/run x 1/2/4
cd "$(git rev-parse --show-toplevel)" || exit 2
export OUT=${OUT:-/tmp/s4}
rm -rf "$OUT"; mkdir -p "$OUT"
H=${HOLD:-60}
jobs=""
for g in "heartgold hg-ecruteak up" "soulsilver ss-ecruteak up" "platinum ${PTSAVE:-pt-r202} ${PTDIR:-up}"; do
    for k in walk run; do for s in 1 2 4; do jobs="$jobs$g $k $s $H
"; done; done
done
printf "%s" "$jobs" | xargs -P ${PAR:-6} -L1 bash -c 'bash tools/speed4_measure.sh $0 $1 $2 $3 $4 $5 > $OUT/res_$0_$3$4.txt 2>&1'
for g in heartgold soulsilver platinum; do for k in walk run; do for s in 1 2 4; do cat $OUT/res_${g}_$k$s.txt; done; done; done
