#!/bin/bash
# Window-close cycles: the exe boots headless (port_run.py, its own Xvfb), the cfgscript step close=1 pushes the window's close
# event at game frame F, the real quit path (SIM_Quit) ends the process. Counts the runs that did not end with exit 0 / "exit".
# usage: tools/quit_cycles.sh N [FRAME] [OUTDIR]   (build first: port_run.py builds; this runs --no-build)
N=${1:-10}; F=${2:-600}; D=${3:-$HOME/.cache/bwport/quit_cycles}
mkdir -p "$D"; bad=0; ok=0
for i in $(seq 1 "$N"); do
    BWPORT_CFGSCRIPT="f1:ff=1;f$F:close=1" python3 tools/port_run.py --no-build --no-gdb -t 60 --shots 0 --out "$D/o$i" >"$D/r$i.txt" 2>&1
    v=$(tail -1 "$D/r$i.txt")
    rc=$(grep -c "AddressSanitizer\|ThreadSanitizer\|Assertion\|terminate called\|Aborted\|pure virtual\|SEGV\|mutex" "$D/o$i/log.txt" 2>/dev/null)
    case "$v" in *exit*|*"closed"*) ;; esac
    # (lfx3) "exit 127 at no frame count" (the exe was missing) matched "exit" and counted as a clean run: only exit 0 / exit-frame is one
    if [ "$rc" != 0 ] || ! echo "$v" | grep -q "exit" || echo "$v" | grep -q "exit [1-9]"; then bad=$((bad+1)); echo "run $i: BAD ($rc hits) $v"; else ok=$((ok+1)); fi
done
echo "quit_cycles: $N runs, $ok clean, $bad bad"
