#!/bin/bash
# pt_sweep_job.sh NAME STEP...: run Platinum sweep steps in a slot (offload.sh job) and tar the small result files.
#   steps: maps:LO-HI (tools/pt_sweep.py, heap checker on), int:LO-HI (tools/pt_int_sweep.py), plays (menu/PC/mart/room plays, heap checker on),
#          battleA, battleB (tools/pt_battle_sweep.py)
# result: build/ptsweep/NAME/<step>/ (sweep.tsv, crash_groups.tsv, summary.txt, bt.txt per crash) and build/ptsweep_NAME.tgz
set -u
NAME=$1; shift
J=${NTR_SLOT_JOBS:-6}
O=$PWD/build/ptsweep/$NAME
mkdir -p $O
echo "ptsweep $NAME: jobs $J, $(git log --oneline -1), host $(hostname), load $(cut -d' ' -f1-3 /proc/loadavg)"
[ -x build/port/linux/bwport ] || { echo "NO EXE build/port/linux/bwport"; exit 1; }
ls -la build/port/linux/bwport | cut -c1-120
tarit() { (cd $O && tar czf $O/../../ptsweep_$NAME.tgz --exclude='*/run/*' --exclude='*.png' --exclude='*/run' . 2>/dev/null); }
run() { local n=$1; shift; local s=$(date +%s); "$@" > $O/$n.log 2>&1; echo "== $n rc=$? $(( $(date +%s) - s )) s"
  grep -E "^(ok|crash|hang|heap|cap|stuck|no-|enter|exit|timeout|verdicts|  ?[0-9]+ )" $O/$n/summary.txt 2>/dev/null | head -14 | cut -c1-200; tarit; }
for st in "$@"; do
  case $st in
    maps:*) r=${st#maps:}; run maps_$r python3 tools/pt_sweep.py --maps $r --heapcheck -j $J --out $O/maps_$r;;
    int:*)  r=${st#int:};  run int_$r python3 tools/pt_int_sweep.py --maps $r --heapcheck -j $J --out $O/int_$r;;
    battleA) run battleA python3 tools/pt_battle_sweep.py --pass A --ids all -j $J --out $O/battleA --heapcheck;;
    battleB) run battleB python3 tools/pt_battle_sweep.py --pass B --ids all -j $J --out $O/battleB --heapcheck;;
    plays)
      mkdir -p $O/plays
      for p in menu-party menu-bag menu-dex menu-card menu-opts menu-poketch pc-deposit pc-withdraw pc-move pc-items mart-common mart-special room-load room-bag room-save softreset; do
        echo "play $p: $(python3 tools/port_run.py --game platinum --no-build --play platinum/$p --heapcheck -t 900 2>&1 | tee $O/plays/$p.txt | tail -1 | cut -c1-200)"
      done | tee $O/plays.txt;;
    *) echo "unknown step $st";;
  esac
done
tarit; ls -la build/ptsweep_$NAME.tgz
