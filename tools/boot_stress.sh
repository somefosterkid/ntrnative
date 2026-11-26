#!/bin/bash
# tools/boot_stress.sh [-n BOOTS] [-j PARALLEL] [-l BUSY_LOOPS] [-g GAME] [-f FRAME] [-t SECS] [-o DIR] [-b LABEL] [-x]
#   Cold-boot the Linux exe BOOTS times to game frame FRAME under host load and count how each boot ended: the start-up stall / abort
#   hunt (libntr's SIM_SdlInit / SIM_Audio_Init, 11-25). Every boot is a fresh process through tools/port_run.py --no-build (its own
#   Xvfb + gdb, the way the sweeps and chains boot), so a stall ends in port_run's `hang in F` verdict with gdb's backtrace and an abort
#   in `crash at X`, both kept in DIR/fail/. PARALLEL boots run at once, each worker in its own run dir (DIR/wK, made by the first boot
#   of the worker); BUSY_LOOPS extra `while :` shell loops add CPU load (the tool prints the load average it ran under).
#   -g black|white|platinum|heartgold|soulsilver (default black). -t = port_run's watchdog per boot (default 90). -b = label in the
#   summary (e.g. before / after). -x = no frame cap (the exe runs at 60 fps: slower, kinder to a loaded host; default uncapped).
#   Needs a built exe (python3 tools/port_run.py --game G --frames 60 once, or the slot's build) and orig/baserom_*.nds like port_run.
#   Result: DIR/results.tsv (boot, worker, verdict class, seconds, "did not start" parts, port_run's verdict line) and a summary:
#       ok / hang / crash / other counts, the retry lines with their reasons (`libntr: SDL's <part> did not start (<reason>)`).
#   Exit 0 when every boot reached the frame, 1 otherwise.
cd "$(dirname "$0")/.." || exit 2
N=500 J=8 LOAD=0 GAME=black FRAMES=300 WD=90 OUT=build/boot_stress LABEL=run CAP=
while getopts n:j:l:g:f:t:o:b:x o; do
    case $o in n) N=$OPTARG;; j) J=$OPTARG;; l) LOAD=$OPTARG;; g) GAME=$OPTARG;; f) FRAMES=$OPTARG;; t) WD=$OPTARG;; o) OUT=$OPTARG;; b) LABEL=$OPTARG;; x) CAP=--realtime;; *) sed -n 2,16p "$0"; exit 2;; esac
done
mkdir -p "$OUT/fail" || exit 2
: > "$OUT/results.tsv"
RUN="python3 tools/port_run.py --no-build --game $GAME --frames $FRAMES -t $WD --hang 25 --shots 0 $CAP"
LOADPIDS=()
for ((i = 0; i < LOAD; i++)); do
    bash -c 'while :; do :; done' &
    LOADPIDS+=($!)
done
trap 'kill "${LOADPIDS[@]}" 2>/dev/null' EXIT
worker() {   # worker K: boots K, K+J, K+2J, ... < N
    local k=$1 b dir out t0 verdict cls parts
    dir=$OUT/w$k
    for ((b = k; b < N; b += J)); do
        out=$dir/out
        t0=$SECONDS
        verdict=$($RUN --dir "$dir" --out "$out" 2>&1 < /dev/null | tail -n 1)
        case $verdict in
            *'exit-frame '$FRAMES*) cls=ok;;
            *hang*) cls=hang;;
            *crash*|*signal*|*abort*) cls=crash;;
            *) cls=other;;
        esac
        parts=$(grep -h -o "libntr: SDL's [a-z]* did not start ([^)]*)\|audio: SDL's audio did not start: .*" "$out/log.txt" 2> /dev/null | sort -u | tr '\n' ';')
        if [ $cls != ok ]; then cp -r "$out" "$OUT/fail/boot$b" 2> /dev/null; fi
        if [ -n "$parts" ]; then mkdir -p "$OUT/fail/boot$b"; cp "$out/log.txt" "$OUT/fail/boot$b/" 2> /dev/null; fi
        printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$b" "$k" "$cls" "$((SECONDS - t0))" "${parts:--}" "$verdict" >> "$OUT/results.tsv"
    done
}
T0=$SECONDS
echo "boot_stress[$LABEL]: $N boots of $GAME to frame $FRAMES, $J at once, $LOAD busy loops; load before: $(cut -d' ' -f1-3 /proc/loadavg)"
WPIDS=()
for ((k = 0; k < J; k++)); do worker $k & WPIDS+=($!); done
( while :; do sleep 5; cut -d. -f1 /proc/loadavg; done ) > "$OUT/load.txt" &
SAMPLER=$!
for p in "${WPIDS[@]}"; do wait "$p"; done
kill "$SAMPLER" "${LOADPIDS[@]}" 2> /dev/null
MAXLOAD=$(sort -n "$OUT/load.txt" | tail -n 1)
tot=$(wc -l < "$OUT/results.tsv")
echo "boot_stress[$LABEL]: $GAME $tot boots in $((SECONDS - T0)) s, peak 1-min load $MAXLOAD:" \
    "ok $(awk -F'\t' '$3=="ok"' "$OUT/results.tsv" | wc -l)," \
    "hang $(awk -F'\t' '$3=="hang"' "$OUT/results.tsv" | wc -l)," \
    "crash $(awk -F'\t' '$3=="crash"' "$OUT/results.tsv" | wc -l)," \
    "other $(awk -F'\t' '$3=="other"' "$OUT/results.tsv" | wc -l)," \
    "with a 'did not start' line $(awk -F'\t' '$5!="-"' "$OUT/results.tsv" | wc -l)"
awk -F'\t' '$5!="-" {print "  " $5}' "$OUT/results.tsv" | sort | uniq -c | sort -rn | head -10
awk -F'\t' '$3!="ok" {print "  boot " $1 ": " $3 " (" $4 " s) " $6}' "$OUT/results.tsv" | cut -c1-240 | head -15
[ "$(awk -F'\t' '$3!="ok"' "$OUT/results.tsv" | wc -l)" = 0 ]
