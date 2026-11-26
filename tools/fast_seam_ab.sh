#!/bin/bash
# Fast renderer seam A/B, crack check and jitter sweep over the shared capture sets (docs/gpu-hwraster.md "Seams ... on GL").
#   bash tools/fast_seam_ab.sh ab     DRV      gpu_fd gaps/extras/specks, BWPORT_FAST_SEAM 0 vs 3, 3 sets x scales 1 2 3 x aa off/4 samples
#   bash tools/fast_seam_ab.sh jitter DRV      the same with BWPORT_FAST_JITTER D = 16 8 4 2 (1/D px), seam 0 vs 3
#   bash tools/fast_seam_ab.sh crack  N...     gpu_crack on Exact at N x over HG, Pt, Black, B2 and W2 walks
# Env: NAMES (set names), SCALES, AAS, EVERY (Pt/HG/B2/W2 thinning), OUT (default build/fst1), DRV llvmpipe|d3d12 (GALLIUM_DRIVER).
MODE=$1; shift
OUT=${OUT:-build/fst1}; mkdir -p $OUT
J=${NTR_SLOT_JOBS:-${BW_JOBS:-4}}
declare -A SETS=([black]="town-lab 300-760/8 1" [hg]="hg-door 10800-12600/16 1" [pt]="town-walk 7500-9200/8 ${EVERY:-4}"
                 [b2]="b2-r19 300-760/8 1" [w2]="w2-r19 300-760/1 1" [hgr]="hg-r29 11000-11500/1 1" [hugh]="hugh-fight 260-760/1 1")
[ -d build/gpu ] || meson setup build/gpu port/gpu > /dev/null 2>&1
ninja -C build/gpu gpu_fastdiff gpu_crack gpu_render > /dev/null || exit 2
capdir() { set -- ${SETS[$1]}; python3 tools/caps.py get $1 $2 | tail -1; }
case $MODE in
ab|jitter)
  DRV=$1; [ "$DRV" = d3d12 ] && export GALLIUM_DRIVER=d3d12
  JS=${JS:-0}; [ $MODE = jitter ] && JS=${JS_LIST:-16 8 4 2}
  for name in ${NAMES:-black hg pt}; do
    dir=$(capdir $name); ev=$(set -- ${SETS[$name]}; echo $3)
    for sc in ${SCALES:-1 2 3}; do for aa in ${AAS:-1 2}; do for jd in $JS; do for seam in 0 3; do
      f=$OUT/$MODE-$DRV-$name-s$sc-aa$aa-j$jd-seam$seam.json
      BWPORT_FAST_JITTER=$jd BWPORT_FAST_SEAM=$seam python3 tools/gpu_fd.py run $dir --scale $sc --aa $aa --every $ev --nobuild --save $f 2>&1 | tail -1 | sed "s/^/$MODE $DRV $name s$sc aa$aa j$jd seam$seam: /"
    done; done; done; done
  done ;;
crack)
  for name in ${NAMES:-hg hgr pt b2 w2 black}; do
    dir=$(capdir $name); ev=$(set -- ${SETS[$name]}; echo $3); case $name in hgr|b2|w2|hugh) ev=${EVERY2:-30};; esac
    for n in "$@"; do
      ls $dir/*.vcs | awk -v k=$ev 'NR%k==0' | xargs -P $J -I{} sh -c "xvfb-run -a build/gpu/gpu_crack {} $n --api gl 2>&1 | tail -1" > $OUT/crack-$name-n$n.txt
      awk -v nm=$name -v n=$n '{c++; for(i=1;i<=NF;i++) if($i=="cracks"){ok++; s+=$(i-8); b+=$(i+1); if($(i+1)>0) w++}} END{printf "crack %s N=%d: %d captures (%d failed), silhouette %d N x px, %d cracks, %d captures with cracks\n",nm,n,c,c-ok,s,b,w}' $OUT/crack-$name-n$n.txt
    done
  done ;;
esac
