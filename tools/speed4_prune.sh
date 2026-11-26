#!/bin/bash
# usage: tools/speed4_prune.sh DIR [KEEP=5] [DAYS=2]
# Prunes the default run-output dir of tools/speed4_measure.sh / speed4_table.sh (~/.cache/bwport/speed4: a run_<tag> dir per run, a
# table.PID dir per table, ~100-300 MB each, 8.5 GB found never pruned). Keeps the newest KEEP of them and drops the rest, and anything
# untouched for more than DAYS days (whichever keeps less); the tag's <tag>.run / <tag>.log go with their run dir, and loose .run/.log
# files older than DAYS days go too. An entry touched in the last hour is a run in progress: never removed. Never call it on a
# caller's explicit OUT (the two scripts do it only when OUT is unset).
dir=$1; keep=${2:-5}; days=${3:-2}
[ -d "$dir" ] || exit 0
now=$(date +%s); n=0
for e in $(ls -dt "$dir"/run_* "$dir"/table.* 2>/dev/null); do   # newest first
    age=$((now - $(stat -c %Y "$e" 2>/dev/null || echo "$now"))); n=$((n + 1))
    [ "$age" -lt 3600 ] && continue
    if [ "$n" -gt "$keep" ] || [ "$age" -gt $((days * 86400)) ]; then
        b=$(basename "$e"); rm -rf -- "$e"; rm -f -- "$dir/${b#run_}.run" "$dir/${b#run_}.log"
    fi
done
find "$dir" -maxdepth 1 -type f \( -name '*.run' -o -name '*.log' \) -mtime +"$days" -delete 2>/dev/null
exit 0
