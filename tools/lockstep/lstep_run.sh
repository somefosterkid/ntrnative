#!/bin/bash
# Lockstep run: both producers for one save + input script, then the compare.
#
#   tools/lockstep/lstep_run.sh SAVE INPUTS [OUTDIR] [-- compare options]
#     SAVE    a name in <saves> (town -> town.bin)
#     INPUTS  a BWPORT_KEYS script file (docs/lockstep.md, "Inputs"), frames counted from the anchor
#     OUTDIR  default build/lockstep/SAVE-INPUTSNAME
#   Produces OUTDIR/melon.jsonl, OUTDIR/port.jsonl (and OUTDIR/png/ if PNG=1), prints the compare report.
#   Exit status: the compare's (0 same, 1 diverged, 2 bad input); 3 a producer is missing or failed.
#
# The producers (interfaces fixed by docs/lockstep.md; both are written by other agents and may not exist yet):
#   melonDS: ${LSTEP_MELON:-<ref>/lockstep/lstep_melon} ROM SAVE.bin INPUTS OUT.jsonl [--png DIR] [--frames A-B]
#   port:    BWPORT_LSTEP=OUT.jsonl[,RECORDS] BWPORT_LSTEP_INPUTS=FILE via tools/port_run.py --save NAME
#            [--fast unless LSTEP_FAST=0] (docs/lockstep.md "The port side"; override the whole command with LSTEP_PORT_CMD: $SAVE $INPUTS $OUT).
# Environment: FRAMES=A-B limits both, PNG=1 writes PNGs of both sides, ROM=path overrides the ROM, LSTEP_FAST=0 runs
# the port at 60 fps (default uncapped: since the frame-driven sound clock, the dumps are byte-identical
# either way), LSTEP_SKIP=N: the port draws and dumps screens only on frames f % N == 0 (default 1: all; the compare
# then looks at screens on those frames only; vars-only hunts: docs/lockstep.md "Frame skip"), LSTEP_PORT_ARGS adds port_run.py options (lstep_sweep.sh: --dir RUN --no-build for parallel runs).
set -u
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
[ $# -ge 2 ] || { sed -n '2,17p' "$0"; exit 3; }
save=$1; inputs=$2; shift 2
out=""
if [ $# -gt 0 ] && [ "$1" != "--" ]; then out=$1; shift; fi
[ "${1:-}" = "--" ] && shift
cmp_opts=("$@")
[ -n "$out" ] || out="$root/build/lockstep/$save-$(basename "$inputs" | sed 's/\.[^.]*$//')"
# absolute paths
[ -f "$inputs" ] || { echo "lstep_run: no inputs file $inputs" >&2; exit 3; }
inputs=$(realpath "$inputs"); out=$(realpath -m "$out")
mkdir -p "$out"
rom=${ROM:-/root/bw-port/orig/baserom_b_us.nds}
melon=${LSTEP_MELON:-/root/ref/lockstep/lstep_melon}
frames=${FRAMES:+--frames $FRAMES}
png_m=""; [ "${PNG:-0}" = 1 ] && png_m="--png $out/png/melon"

rc=0
# The melonDS side is a pure function of (ROM, save, inputs, producer, vars, symbols, frames): cached under
# LSTEP_CACHE (default /root/mt/lstep_cache; LSTEP_CACHE=off disables), so a rerun only runs the port side.
cache=${LSTEP_CACHE:-/root/mt/lstep_cache}
if [ ! -x "$melon" ]; then
  echo "lstep_run: melonDS producer not found: $melon (docs/lockstep.md)" >&2; rc=3
else
  ckey=""
  if [ "$cache" != off ] && [ -z "$png_m" ]; then
    ckey=$(cat "$rom" "/root/saves/$save.bin" "$inputs" "$melon" "$root/tools/lockstep/vars.txt" "$root/config/b_us/arm9" 2>/dev/null | sha1sum | cut -c1-16)-${FRAMES:-all}
  fi
  if [ -n "$ckey" ] && [ -s "$cache/$ckey.jsonl" ]; then
    cp "$cache/$ckey.jsonl" "$out/melon.jsonl"; echo "lstep_run: melonDS side cached ($ckey)" >&2
  else
    "$melon" "$rom" "/root/saves/$save.bin" "$inputs" "$out/melon.jsonl" --vars "$root/tools/lockstep/vars.txt" --symbols "$root/config/b_us/arm9" $frames $png_m || { echo "lstep_run: lstep_melon failed" >&2; rc=3; }
    if [ $rc = 0 ] && [ -n "$ckey" ] && [ -s "$out/melon.jsonl" ]; then mkdir -p "$cache"; cp "$out/melon.jsonl" "$cache/$ckey.tmp$" && mv "$cache/$ckey.tmp$" "$cache/$ckey.jsonl"; fi
  fi
fi

[ $rc = 0 ] || exit $rc   # no point running the port against nothing

keys=$(sed 's/#.*//' "$inputs" | tr '\n' ' ')
if [ -n "${LSTEP_PORT_CMD:-}" ]; then
  SAVE=$save INPUTS=$inputs OUT="$out/port.jsonl" bash -c "$LSTEP_PORT_CMD" || { echo "lstep_run: port run failed" >&2; rc=3; }
else
  # the port side (docs/lockstep.md "The port side"): BWPORT_LSTEP=OUT[,RECORDS], inputs from BWPORT_LSTEP_INPUTS
  n=""; [ -n "${FRAMES:-}" ] && n=",$(( ${FRAMES#*-} + 1 ))"
  (cd "$root" && BWPORT_SKIPMENU=0 BWPORT_LSTEP="$out/port.jsonl$n" BWPORT_LSTEP_INPUTS="$inputs" BWPORT_LSTEP_SAVE="$save.bin" \
     BWPORT_LSTEP_SKIP="${LSTEP_SKIP:-1}" BWPORT_LSTEP_PNG="${PNG:+$out/png/port${FRAMES:+,$FRAMES}}" \
     python3 tools/port_run.py --save "$save" $([ "${LSTEP_FAST:-1}" = 1 ] && echo --fast) ${LSTEP_PORT_ARGS:-} -t "${PORT_TIMEOUT:-600}") || { echo "lstep_run: port run failed" >&2; rc=3; }
fi
[ -s "$out/port.jsonl" ] || rc=3
[ $rc = 0 ] || exit $rc

# FRAMES=A-B: lstep_melon records only f=A..B (the same f numbers as the port's: frames from the anchor), the port
# runs from the anchor to f=B and dumps 0..B (its PNGs only A-B): the compare is limited to A-B (--after A-1 --until B)
fr_opts=()
[ -n "${FRAMES:-}" ] && fr_opts=(--after $(( ${FRAMES%-*} - 1 )) --until "${FRAMES#*-}")
python3 "$here/lstep_compare.py" "$out/melon.jsonl" "$out/port.jsonl" "${fr_opts[@]}" "${cmp_opts[@]}"
