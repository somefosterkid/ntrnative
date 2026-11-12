#!/bin/bash
# fast_ab.sh BEFORE_ROOT AFTER_ROOT TAG [CAPTURE_DIR...]
# The Fast 3D renderer against the Exact one (gpu_fastdiff, port/gpu), before and after a change. Builds gpu_fastdiff from both trees
# and runs every capture at 3x with AA off (--aa 1) and 4 samples (--aa 2) on one API: metal on macOS, gl elsewhere
# (BWPORT_FASTDIFF_API overrides). Capture dirs, BWPORT_CAPTURES, BWPORT_SCRATCH: see pics_ab.sh and tools/ab_lib.sh.
# Prints:
#   fast: N lines, K differ, M missing    md5 of the Fast and the Exact picture per capture and aa, before vs after: K = lines that
#                                         changed (a change to Exact alone must keep both md5s; a Fast change moves the Fast one),
#                                         M = lines with a picture missing (the render failed)
#   fastdiff before|after: N runs, differ mean X%, max Y% (capture aaN)
#                                         gpu_fastdiff's own statistic: the share of pixels where Fast's picture differs from
#                                         Exact's. TAG-fastdiff-{before,after}.txt keep its whole lines (edge/trans/poly/interp
#                                         classes and the size of the colour step).
# Read it: Exact's md5 never moves unless the change meant it to; for Fast compare the after mean/max against the before's, and read
# the classes in the txt before calling a rise harmless.
set -u
. "$(dirname "$0")/ab_lib.sh"
[ $# -ge 3 ] || { sed -n 2,3p "$0"; exit 2; }
B=$(ab_abs "$1"); A=$(ab_abs "$2"); T=$3; shift 3
ab_captures "$@"
if [ -n "${BWPORT_FASTDIFF_API:-}" ]; then
  API=$BWPORT_FASTDIFF_API
elif [ "$(uname)" = Darwin ]; then
  API=metal
else
  API=gl
fi
for side in before after; do
  src=$B
  [ $side = after ] && src=$A
  ab_build "$src" gpu "$AB_WORK/$T-gr-$side"
done
for side in before after; do
  F=$(ab_exe "$AB_WORK/$T-gr-$side" gpu_fastdiff)
  OUT=$AB_WORK/$T-fast-$side.txt
  STAT=$AB_WORK/$T-fastdiff-$side.txt
  : > "$OUT"
  : > "$STAT"
  while read -r f; do
    for aa in 1 2; do
      rm -f "$AB_WORK/$T-fd_"*.png
      "$F" "$f" "$AB_WORK/$T-fd" --scale 3 --aa $aa --api "$API" 2> /dev/null | grep '^gpu_fastdiff: .*differ' | sed "s|^|$(basename "$f") |" >> "$STAT"
      echo "$(basename "$(dirname "$f")")/$(basename "$f") aa$aa $(ab_md5 "$AB_WORK/$T-fd_fast.png") $(ab_md5 "$AB_WORK/$T-fd_exact.png")" >> "$OUT"
    done
  done < "$AB_WORK/captures.txt"
done
rm -f "$AB_WORK/$T-fd_"*.png
W=$AB_WORK/$T
echo "fast: $(wc -l < "$W-fast-before.txt" | tr -d ' ') lines, $(diff "$W-fast-before.txt" "$W-fast-after.txt" | grep -c '^<') differ, $(awk 'NF<4' "$W-fast-after.txt" | wc -l | tr -d ' ') missing"
for side in before after; do
  awk -v side=$side '
    { aa = ""; for (i = 1; i <= NF; i++) if ($i == "aa") { aa = $(i+1); gsub(/[^0-9]/, "", aa) }
      for (i = 1; i <= NF; i++) if ($i == "differ") { p = $(i+2); gsub(/[^0-9.]/, "", p); s += p; n++; if (p + 0 > m) { m = p + 0; w = $1 " aa" aa } } }
    END { if (n) printf "fastdiff %s: %d runs, differ mean %.2f%%, max %.2f%% (%s)\n", side, n, s / n, m, w }' "$W-fastdiff-$side.txt"
done
