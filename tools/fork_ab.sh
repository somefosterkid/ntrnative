#!/bin/bash
# The title snapshot (port/src/title_fork.c) against a full boot: the same legs per arm, then each leg's end V-blank (v=) and
# end save. Arms: full (a boot per leg), fork (BWPORT_FORK_AT: forks inside its own boot), snap (ROUTE_SNAPSHOT=1: the fork server).
# GAME=pt LEGS="out barry" ARMS="full snap" J=1 bash tools/fork_ab.sh
# DIAG=1: also each leg's game log tail (fork:/pilot: lines) and the servers' logs (RUN_WORK/snap-*.log)
cd "$(git rev-parse --show-toplevel)"
GAME=${GAME:-pt}
LEGS=${LEGS:-out barry}
for arm in ${ARMS:-full fork snap}; do
  W=/tmp/forkab-$$-$arm; rm -rf $W; mkdir -p $W
  unset BWPORT_FORK_AT ROUTE_SNAPSHOT
  [ $arm = fork ] && export BWPORT_FORK_AT=${FORK_AT:-6290}
  [ $arm = snap ] && export ROUTE_SNAPSHOT=1
  t0=$(date +%s.%N)
  ROUTE_WORK=$W python3 tools/route.py $GAME $LEGS -j ${J:-1} $ROUTE_ARGS > $W.txt 2>&1
  t1=$(date +%s.%N)
  echo "=== $arm wall $(echo "$t1 - $t0" | bc)"
  grep -E "^$GAME [a-z0-9-]+: " $W.txt | cut -c1-200
  grep -v "^ *$" $W.txt | tail -3 | cut -c1-260
  for f in $(find $W -maxdepth 1 -name "*.save.bin" | sort); do echo "$(sha1sum < $f | cut -c1-12) ${f#$W/}"; done
  grep -h "^fork:" $W/*.game.log 2>/dev/null | sed 's/[0-9]\{3,\}/N/g' | sort | uniq -c | sort -rn | head -4 | cut -c1-200
  for l in $W/snap-*.log; do [ -f $l ] && echo "server log ${l#$W/}: $(grep '^fork: server' $l | head -1 | cut -c1-600)"; done
  for l in $(grep -l "^bwport: HANG" $W/*.game.nogdb.log $W/*.game.log 2>/dev/null); do echo "--- hang ${l#$W/}:"; grep -a -A14 "^bwport: HANG" $l | grep -v "libc.so" | head -40 | cut -c1-200; done
  if [ -n "$DIAG" ]; then
    for l in $W/*.game.log; do echo "--- ${l#$W/}"; grep -a -E "^fork:|^pilot:|rror|hang|verdict" $l | tail -${DIAG_N:-12} | cut -c1-220; done
    for l in $W/snap-*.log; do echo "--- ${l#$W/}"; grep -a -v "^pos " $l | tail -${DIAG_N:-12} | cut -c1-220; done
    set -- $W/snap-*.key; [ $# -ge 2 ] && { echo "--- keys differ:"; diff $1 $2 | cut -c1-300 | head -20; }
    ls $W | head -30
  fi
  [ -n "$KEEP" ] && { mkdir -p $KEEP/$arm; cp $W.txt $W/*.log $W/*.key $KEEP/$arm/ 2>/dev/null; }   # KEEP=build/forkab (offload --get)
  rm -rf $W $W.txt
done
