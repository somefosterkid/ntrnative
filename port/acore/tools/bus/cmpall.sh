#!/bin/bash
# cmpall.sh NAME [SECS] [MARKPLAYER]: an84 of each speed against the 1x file, each cut at its run's first start on nns-player MARKPLAYER (default 0)
NAME=$1; SECS=${2:-20}; MP=${3:-0}; NTH=${4:-1}
TOOLS=$(cd "$(dirname "$0")" && pwd); OUT=${OUT:-/root/mt/bus}; mkdir -p $OUT
cd $OUT
mark() { grep "^snd: start" ${WT:-$(git -C $TOOLS rev-parse --show-toplevel)}/build/port/run_${NAME}_$1/out/log.txt | grep "nns-player $MP wav" | sed -n ${NTH}p | sed 's/.* wav //'; }
for t in 1y b2 a2 b3 a3 b4 a4; do echo "=== $NAME $t (marks $(mark 1x) $(mark $t))"; python3 $TOOLS/an84.py ${NAME}_1x.wav@$(mark 1x) ${NAME}_$t.wav@$(mark $t) $SECS 2>&1 | tail -12; done
