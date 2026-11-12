#!/bin/bash
# vcore_ab.sh BEFORE_ROOT AFTER_ROOT TAG [CAPTURE_DIR...]
# The vcore check alone (the "74/74" check): builds vcore_render from both trees and renders every capture at 1x and 3x with
# --geo (the geometry vcore hands the GPU path), and compares the md5 of the picture and of the geometry file.
# Capture dirs, BWPORT_CAPTURES, BWPORT_SCRATCH: see pics_ab.sh and tools/ab_lib.sh.
# Prints:
#   vcore: N lines, K differ, M missing outputs   N = 2 x captures; K = lines whose md5s changed; M = lines with no picture or no
#                                                 geo file (a render failed). K and M must be 0 for a change that is a no-op.
#   (then up to 4 lines of the diff)
# Read it: "vcore: 148 lines, 0 differ, 0 missing outputs" = vcore renders the corpus bit-exactly as before.
set -u
. "$(dirname "$0")/ab_lib.sh"
[ $# -ge 3 ] || { sed -n 2,3p "$0"; exit 2; }
B=$(ab_abs "$1"); A=$(ab_abs "$2"); T=$3; shift 3
ab_captures "$@"
for side in before after; do
  src=$B
  [ $side = after ] && src=$A
  ab_build "$src" vcore "$AB_WORK/$T-vc-$side"
done
for side in before after; do
  V=$(ab_exe "$AB_WORK/$T-vc-$side" vcore_render)
  OUT=$AB_WORK/$T-vcab-$side.txt
  : > "$OUT"
  while read -r f; do
    for n in 1 3; do
      rm -f "$AB_WORK/$T-vc.png" "$AB_WORK/$T-vc.geo"
      "$V" "$f" "$AB_WORK/$T-vc.png" --scale $n --geo "$AB_WORK/$T-vc.geo" > /dev/null 2>&1
      echo "$(basename "$(dirname "$f")")/$(basename "$f") s$n $(ab_md5 "$AB_WORK/$T-vc.png") $(ab_md5 "$AB_WORK/$T-vc.geo")" >> "$OUT"
    done
  done < "$AB_WORK/captures.txt"
done
rm -f "$AB_WORK/$T-vc.png" "$AB_WORK/$T-vc.geo"
W=$AB_WORK/$T-vcab
echo "vcore: $(wc -l < "$W-before.txt" | tr -d ' ') lines, $(diff "$W-before.txt" "$W-after.txt" | grep -c '^<') differ, $(awk 'NF<4' "$W-after.txt" | wc -l | tr -d ' ') missing outputs"
diff "$W-before.txt" "$W-after.txt" | head -4
