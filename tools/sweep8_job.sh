#!/bin/bash
# sweep8_job.sh [--maps A-B] NAME STEP...: build the HG/SS coverage exe in the slot, run the named sweep steps, tar the small result files.
#   --maps A-B   (or 60-70,33): the maps_hg / maps_ss steps sweep only these map ids (hg_sweep.py --maps) instead of all (+ the X-menu apps):
#                split the 541 / 700 maps over machines: one job per slice with its own NAME (sweep8_NAME.tgz), e.g.
#                  sweep8_job.sh --maps 0-270 hgA maps_hg   |   sweep8_job.sh --maps 271-700 hgB maps_hg
# Progress: every 30 s one line (step i/N, rows done in the step's sweep.tsv, seconds) into $NTR_PROGRESS (what the job runner's progress viewer shows), else build/sweep8/NAME/progress.txt.
set -u
MAPS=
if [ "${1:-}" = --maps ]; then MAPS=${2:?--maps needs A-B}; shift 2; fi
NAME=$1; shift
NSTEPS=$#; NDONE=0
J=${NTR_SLOT_JOBS:-8}
C=build/port/linux-hgss-cov
O=$PWD/build/sweep8/$NAME
mkdir -p $O
echo "sweep8 $NAME: jobs $J, $(git log --oneline -1), host $(hostname), load $(cut -d' ' -f1-3 /proc/loadavg)"
t0=$(date +%s)
if [ -z "${SWEEP8_DRY:-}" ]; then   # SWEEP8_DRY=1: the steps are printed, not run (tools/sweep8_job_tests.py)
if [ ! -x $C/bwport ] || ! grep -q '"hgss_cov".*' $C/meson-info/intro-buildoptions.json 2>/dev/null; then
python3 - <<'PY'
import os, sys
os.environ['BWPORT_BUILD'] = 'build/port/linux-hgss-cov'
os.environ['BWPORT_BUILD_GAMES'] = 'hgss'
sys.path.insert(0, 'tools'); sys.argv = ['x']
import port_run
print('refresh:', port_run.port_wraps.refresh(port_run.BUILD, port_run.setup_args()))
PY
fi
meson configure $C -Dhgss_cov=true > $O/conf.log 2>&1; tail -2 $O/conf.log
nice ninja -C $C -j $J bwport > $O/build.log 2>&1; echo "cov build rc=$? $(( $(date +%s) - t0 )) s"; tail -2 $O/build.log
[ -x $C/bwport ] || { echo "NO EXE"; exit 1; }
fi
progress() {   # progress STEP: a line every SWEEP8_PROGRESS_S (30) s into $NTR_PROGRESS (else $O/progress.txt): rows done of the step's sweep.tsv
  while sleep ${SWEEP8_PROGRESS_S:-30}; do
    local rows=0; [ -f $O/$1/sweep.tsv ] && rows=$(( $(wc -l < $O/$1/sweep.tsv) - 1 )); [ $rows -lt 0 ] && rows=0
    local msg="sweep8 $NAME: step $((NDONE + 1))/$NSTEPS $1: $rows rows done, $(( $(date +%s) - t0 )) s since start"
    echo "$msg" > ${NTR_PROGRESS:-$O/progress.txt}
  done
}
run() { local n=$1; shift; [ -n "${SWEEP8_DRY:-}" ] && { echo "DRY $n: $*"; return; }; local s=$(date +%s) pp; progress $n > /dev/null 2>&1 & pp=$!; "$@" > $O/$n.log 2>&1; local rc=$?; kill $pp 2>/dev/null; wait $pp 2>/dev/null; NDONE=$((NDONE + 1)); echo "== $n rc=$rc $(( $(date +%s) - s )) s ($NDONE/$NSTEPS)"; grep -E "^(ok|crash|hang|heap|cap|stuck|no-|enter|exit|timeout|unreached|verdicts|  ?[0-9]+ )" $O/$n/summary.txt 2>/dev/null | head -12 | cut -c1-200
  (cd $O && tar czf $O/../../sweep8_$NAME.tgz --exclude='*/run/*' --exclude='*.png' --exclude='cov.txt.gz' --exclude='*/run' . 2>/dev/null); }
APPS2=fr-,pa-,safari,bug-,link-,palpark,trade-house,gamecorner   # hg_app_sweep.plays2 (hgx1): the facilities the first 47 plays never open
for st in "$@"; do
  case $st in
    apps_hg)  run apps_hg python3 tools/hg_app_sweep.py --plays all --heapcheck -j $J --build $C --out $O/apps_hg;;
    apps_ss)  run apps_ss python3 tools/hg_app_sweep.py --plays all --game soulsilver --heapcheck -j $J --build $C --out $O/apps_ss;;
    apps2_hg) run apps2_hg python3 tools/hg_app_sweep.py --plays $APPS2 --heapcheck -j $J --build $C --out $O/apps2_hg;;
    apps2_ss) run apps2_ss python3 tools/hg_app_sweep.py --plays $APPS2 --game soulsilver --heapcheck -j $J --build $C --out $O/apps2_ss;;
    maps_hg)  if [ -n "$MAPS" ]; then run maps_hg python3 tools/hg_sweep.py --maps $MAPS --heapcheck -j $J --build $C --out $O/maps_hg
              else run maps_hg python3 tools/hg_sweep.py --maps all,apps --apps-tap --heapcheck -j $J --build $C --out $O/maps_hg; fi;;
    party_hg) run party_hg python3 tools/hg_sweep.py --maps apps --apps-tap --heapcheck -j $J --build $C --save hg-r30c --out $O/party_hg;;
    party_ss) run party_ss python3 tools/hg_sweep.py --maps apps --apps-tap --heapcheck -j $J --build $C --game soulsilver --save ss-r30c --out $O/party_ss;;
    maps_ss_a) run maps_ss_a python3 tools/hg_sweep.py --maps 0-270 --heapcheck -j $J --build $C --game soulsilver --save ss-room1 --out $O/maps_ss_a;;
    maps_ss_b) run maps_ss_b python3 tools/hg_sweep.py --maps 271-700 --heapcheck -j $J --build $C --game soulsilver --save ss-room1 --out $O/maps_ss_b;;
    maps_ss)  run maps_ss python3 tools/hg_sweep.py --maps ${MAPS:-all} --heapcheck -j $J --build $C --game soulsilver --save ss-room1 --out $O/maps_ss;;
    battleB_hg) run battleB_hg python3 tools/hg_battle_sweep.py --pass B --ids all --heapcheck -j $J --build $C --out $O/battleB_hg;;
    battleA_hg) run battleA_hg python3 tools/hg_battle_sweep.py --pass A --ids all --heapcheck -j $J --build $C --out $O/battleA_hg;;
    battleB_ss) run battleB_ss python3 tools/hg_battle_sweep.py --pass B --ids all --heapcheck -j $J --build $C --game soulsilver --save ss-room1 --out $O/battleB_ss;;
    *) echo "unknown step $st";;
  esac
done
ls -la build/sweep8_$NAME.tgz
