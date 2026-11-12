#!/bin/bash
# The first sweep (the maintainers' notes): every save with its input script in tools/lockstep/inputs/ (frames 0-700),
# one summary line each. Dumps in build/lockstep/NAME/. Usage: tools/lockstep/lstep_sweep.sh [NAME...] (default: all)
here=$(cd "$(dirname "$0")" && pwd)
mkdir -p "$here/../../build/lockstep"
names=("$@"); [ ${#names[@]} -gt 0 ] || names=(room downstairs town bianca lab townmap route1 caught)
# Parallel: the first save builds the port as usual; the others run at once with a run dir each (a copy of
# build/port/run: the extracted ROM and a sim_config.ini, so --fast holds from their first run) and --no-build.
# LSTEP_JOBS=N caps how many run at once (default 8). LSTEP_SKIP=N (frame skip, docs/lockstep.md): the port draws one
# frame in N; N > 1 compares vars only (--ignore-screens: the beam-race check needs the next frame's screens too).
root=$(cd "$here/../.." && pwd)
one() {
  s=$1; shift
  FRAMES=${FRAMES:-0-700} LSTEP_PORT_ARGS="$*" bash "$here/lstep_run.sh" "$s" "$here/inputs/$s.txt" "$root/build/lockstep/$s" -- --skip-lag --ignore-beam-race ${LSTEP_SKIP:+$([ "$LSTEP_SKIP" -gt 1 ] && echo --ignore-screens)} > "$root/build/lockstep/$s.report" 2>&1
}
one "${names[0]}"
pids=()
for s in "${names[@]:1}"; do
  run="$root/build/lockstep/run-$s"
  [ -d "$run" ] || cp -a "$root/build/port/run" "$run"
  while [ "$(jobs -rp | wc -l)" -ge "${LSTEP_JOBS:-8}" ]; do sleep 1; done
  one "$s" --dir "$run" --no-build &
done
wait
for s in "${names[@]}"; do echo "$s: $(tail -1 "$root/build/lockstep/$s.report")"; done
