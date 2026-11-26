#!/bin/bash
# GL context loss test (glr2): the window's GL context is destroyed and made new (BWPORT_GLRESET=<V-blank>[,<every>], desktop),
# the final picture of a static scene must equal the control run's. Usage: bash tools/gl_reset_test.sh [quick|full]
cd "$(dirname "$0")/.." || exit 2
mode=${1:-quick}
OUT=build/port/run/out
LOGD=${BWPORT_CACHE:-$HOME/.cache/bwport}/glreset
mkdir -p "$LOGD"
PRA=(--save downstairs)
run() { # name cfgscript frames [env VAR=val ...]
  local name=$1 cfg=$2 frames=$3
  shift 3
  rm -f $OUT/final.png
  find . build/port -maxdepth 5 -name "glr_*.png" -delete 2>/dev/null   # the exe writes it into its working dir: none may be left from an earlier run
  env BWPORT_CFGSCRIPT="f0:$cfg,metrics=off;f$((frames-30)):shot=glr_$name" "$@" python3 tools/port_run.py "${PRA[@]}" --skip 1 --frames "$frames" -t 280 --shots 0 \
     > $LOGD/glr_$name.log 2>&1
  local v lost err
  LOG=$(awk '/^port_run: /{sub(/^port_run: /,"");sub(/,.*/,"");l=$0} END{print l}' $LOGD/glr_$name.log)   # the run dir differs per game (run-platinum/out)
  [ -f "$LOG" ] || LOG=$OUT/log.txt
  cp "$LOG" $LOGD/glr_$name.txt 2>/dev/null
  v=$(grep -a "exit-frame\|crash\|hang\|stub" $LOGD/glr_$name.log | tail -1)
  lost=$(grep -ao "old objects ([0-9]*" "$LOG" 2>/dev/null | sort -u | wc -l)
  err=$(grep -aci "GL errors in 3\|RendererFailed\|GL error" "$LOG" 2>/dev/null)
  local png
  png=$(find . build/port -maxdepth 5 -name "glr_$name.png" 2>/dev/null | head -1)
  [ -n "$png" ] && cp "$png" "$LOGD/glr_$name.png"
  echo "$name: pic=$(md5sum "$png" 2>/dev/null | cut -c1-12) lost=$lost errors=$err | $v"
}
python3 tools/port_run.py --help > /dev/null 2>&1
if [ "$mode" = pt ]; then   # Gen 4 (the plat build): Platinum from pt-room, title + CONTINUE by keys (f6300/f6700), the room is up by f7000
  PRA=(--game platinum --save pt-room --keys "start@f6300 a@f6700")
  for cfgname in "gpu1:renderer=gpu,resolution=2,antialias=1" "vcore:renderer=vcore"; do
    n=${cfgname%%:*}
    c=${cfgname#*:}
    run pt-$n-ctl1 "$c" 7600
    run pt-$n-ctl2 "$c" 7600
    run pt-$n-reset "$c" 7600 BWPORT_GLRESET=7000,150
    run pt-$n-poison "$c" 7600 BWPORT_GLRESET=7000,150 BWPORT_GLRESET_POISON=1
  done
  exit 0
fi
for cfgname in "gpu1:renderer=gpu,resolution=2,antialias=1" "vcore:renderer=vcore"; do
  n=${cfgname%%:*}
  c=${cfgname#*:}
  run $n-ctl1 "$c" 1100
  run $n-ctl2 "$c" 1100
  run $n-reset "$c" 1100 BWPORT_GLRESET=400
  run $n-poison "$c" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_POISON=1
  run $n-mid "$c" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_MID=1   # glr4: the loss inside the frame, at the event poll (Android's order)
  run $n-midpoison "$c" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_MID=1 BWPORT_GLRESET_POISON=1
done
if [ "$mode" = full ]; then
  run gpusmaa-ctl "renderer=gpu,resolution=2,antialias=3" 1100
  run gpusmaa-reset "renderer=gpu,resolution=2,antialias=3" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_POISON=1
  run gpuedges-ctl "renderer=gpu,resolution=2,antialias=edges" 1100
  run gpuedges-reset "renderer=gpu,resolution=2,antialias=edges" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_POISON=1
  run gpusmooth-ctl "renderer=gpu,resolution=2,antialias=1,smooth=1" 1100
  run gpusmooth-reset "renderer=gpu,resolution=2,antialias=1,smooth=1" 1100 BWPORT_GLRESET=400 BWPORT_GLRESET_POISON=1
  run stress50 "renderer=gpu,resolution=2,antialias=1" 6600 BWPORT_GLRESET=300,120
  grep -a "GL context lost" "$LOG" | sort -u | sort -t'(' -k2 -n | sed -n '1p;10p;25p;40p;$p'
fi
