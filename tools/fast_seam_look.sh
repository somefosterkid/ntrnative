# Smooth edges look: Exact/Fast x off/edges x Edge ds/fine at 3x for one capture of each set
OUT=build/fst1/look; mkdir -p $OUT
pick() { ls $1/*.vcs | sed -n "$2p"; }
d1=$(python3 tools/caps.py get town-lab 300-760/8 | tail -1)
d2=$(python3 tools/caps.py get hg-door 10800-12600/16 | tail -1)
d3=$(python3 tools/caps.py get town-walk 7500-9200/8 | tail -1)
for t in "black $(pick $d1 30)" "hg $(pick $d2 30)" "pt $(pick $d3 100)"; do
  set -- $t; n=$1; f=$2
  for r in exact fast; do for e in ds fine; do for a in 1 edges 2; do
    GALLIUM_DRIVER=llvmpipe BWPORT_RENDERER3D=$r BWPORT_GPU_EDGE=$e xvfb-run -a build/gpu/gpu_render $f $OUT/x.png --scale 3 --aa $a --api gl --hi $OUT/$n-$r-$e-aa$a.png -q 2>&1 | tail -1
  done; done; done
  echo $n $f
done
rm -f $OUT/x.png; ls $OUT | wc -l
