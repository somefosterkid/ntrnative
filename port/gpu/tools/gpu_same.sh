#!/bin/bash
# same.sh OLD NEW [SCALES...] : the N x layers of OLD and NEW gpu_render on the corpus, byte compare of the PNGs
OLD=$1; NEW=$2; shift; shift
SC=${@:-1 4}
D=/root/mt/g11/same; mkdir -p $D
for n in $SC; do
  bad=0; tot=0
  for f in /root/ref/corpus/*.vcs; do
    b=$(basename $f .vcs)
    $OLD $f $D/o.png --scale $n --hi $D/o.hi.png -q 2>/dev/null
    $NEW $f $D/n.png --scale $n --hi $D/n.hi.png -q 2>/dev/null
    tot=$((tot+1))
    if ! cmp -s $D/o.hi.png $D/n.hi.png || ! cmp -s $D/o.png $D/n.png; then bad=$((bad+1)); echo "DIFF $b scale $n"; fi
  done
  echo "scale $n: $tot frames, $bad differ"
done
