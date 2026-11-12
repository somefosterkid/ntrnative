#!/bin/bash
# mx.sh NAME SAVE SECS [MOVES...]: the music alone (SOLO=music) of one scenario at 1x and, with the bus (after) and
# BWPORT_MUSICBUS=off (before = main's tempo division), at 2x 3x 4x. WAVs: $OUT/NAME_{1x,b2,b3,b4,a2,a3,a4}.wav
NAME=$1; SAVE=$2; SECS=$3; shift; shift; shift
cd $TOOLS
job() {
  tag=$1; s=$2; bus=$3; shift; shift; shift
  if [ "$bus" = off ]; then export BWPORT_MUSICBUS=off; fi
  SOLO=music NB=1 bash mrun.sh ${NAME}_$tag $s $SAVE $SECS "$@" > $OUT/${NAME}_$tag.out 2>&1
}
export -f job; export NAME SAVE SECS
printf '%s\n' ${JOBS:-"1x 1 on" "1y 1 on" "b2 2 off" "a2 2 on" "b3 3 off" "a3 3 on" "b4 4 off" "a4 4 on"} | \
  xargs -P ${PAR:-4} -L1 bash -c 'job $0 $1 $2 '"$*"
for t in 1x 1y b2 a2 b3 a3 b4 a4; do echo $t $(grep -h "^audio" $OUT/${NAME}_$t.out | tail -1) $(grep -h reached $OUT/${NAME}_$t.out); done
