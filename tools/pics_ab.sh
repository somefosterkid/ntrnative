#!/bin/bash
# pics_ab.sh BEFORE_ROOT AFTER_ROOT TAG [CAPTURE_DIR...]
# Picture A/B of a change to port/gpu or port/vcore: builds gpu_render (port/gpu) and vcore_render (port/vcore) from two source
# trees (a worktree or checkout each; BEFORE = AFTER checks that a tree is stable), renders every capture (.vcs, from
# BWPORT_VCAPTURE; tools/vcap_corpus.sh records a corpus) and compares the md5 of every picture:
#   gpu_render   at 1x and 3x, AA off (--aa 1) / 4 samples (--aa 2) / SMAA, on each API (macOS: gl and metal; elsewhere gl and es,
#                BWPORT_APIS overrides), the picture and the --hi layer      (a line per capture, scale, api, aa)
#   vcore_render at 1x and 3x                                                 (a line per capture, scale)
# Capture dirs: arguments or BWPORT_CAPTURES (colon list). Work dir BWPORT_SCRATCH (default build/pics-ab); tools/ab_lib.sh lists
# the other knobs. GPU_COMPOSE=gpu in the environment checks the composed screens instead of vcore's. The PNG is removed before
# each render, so a failed render shows as an empty md5 and never as the last picture's.
# Prints (TAG-pics-{before,after}.txt and TAG-vc-{before,after}.txt in the work dir hold the md5 lines):
#   gpu: N / N lines, K differ      the two line counts must be equal (a short count = a render failed); K = lines whose md5 changed
#   vcore: N / N lines, K differ    the same for vcore_render
#   vcore empty md5 lines: N        renders that produced no picture; must be 0
#   (apis: ...; work dir ...)       what ran
# Read it: "gpu: 444 / 444 lines, 0 differ" is the exact-picture rule for a change that must not alter any pixel (Exact paths, a
# refactor). K > 0 on a change meant to alter pictures: diff the two txt files to see which captures/apis/aa moved.
set -u
. "$(dirname "$0")/ab_lib.sh"
[ $# -ge 3 ] || { sed -n 2,3p "$0"; exit 2; }
B=$(ab_abs "$1"); A=$(ab_abs "$2"); T=$3; shift 3
ab_captures "$@"
for side in before after; do
  src=$B
  [ $side = after ] && src=$A
  ab_build "$src" gpu "$AB_WORK/$T-gr-$side"
  ab_build "$src" vcore "$AB_WORK/$T-vc-$side"
done
PNG=$AB_WORK/$T-o.png
HI=$AB_WORK/$T-h.png
for side in before after; do
  G=$(ab_exe "$AB_WORK/$T-gr-$side" gpu_render)
  V=$(ab_exe "$AB_WORK/$T-vc-$side" vcore_render)
  : > "$AB_WORK/$T-pics-$side.txt"
  : > "$AB_WORK/$T-vc-$side.txt"
  while read -r f; do
    name=$(basename "$(dirname "$f")")/$(basename "$f" .vcs)
    for n in 1 3; do
      for api in $AB_APIS; do
        for aa in 1 2 smaa; do
          rm -f "$PNG" "$HI"
          "$G" "$f" "$PNG" --scale $n --aa $aa --api "$api" --hi "$HI" -q > /dev/null 2>&1
          echo "$name s$n $api $aa $(ab_md5 "$PNG") $(ab_md5 "$HI")" >> "$AB_WORK/$T-pics-$side.txt"
        done
      done
      rm -f "$PNG"
      "$V" "$f" "$PNG" --scale $n > /dev/null 2>&1
      echo "$name s$n $(ab_md5 "$PNG")" >> "$AB_WORK/$T-vc-$side.txt"
    done
  done < "$AB_WORK/captures.txt"
done
rm -f "$PNG" "$HI"
W=$AB_WORK/$T
echo "gpu: $(wc -l < "$W-pics-before.txt" | tr -d ' ') / $(wc -l < "$W-pics-after.txt" | tr -d ' ') lines, $(diff "$W-pics-before.txt" "$W-pics-after.txt" | grep -c '^<') differ"
echo "vcore: $(wc -l < "$W-vc-before.txt" | tr -d ' ') / $(wc -l < "$W-vc-after.txt" | tr -d ' ') lines, $(diff "$W-vc-before.txt" "$W-vc-after.txt" | grep -c '^<') differ"
echo "vcore empty md5 lines: $(grep -c ' $' "$W-vc-after.txt")"
echo "(apis: $AB_APIS; work dir $AB_WORK)"
