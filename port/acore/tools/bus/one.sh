#!/bin/bash
# one.sh NAME SAVE SECS TAG SPEED on|off [MOVES]: one music-alone run
NAME=$1; SAVE=$2; SECS=$3; tag=$4; s=$5; bus=$6; shift 6
TOOLS=$(cd "$(dirname "$0")" && pwd); OUT=${OUT:-/root/mt/bus}; mkdir -p $OUT
cd $TOOLS
if [ "$bus" = off ]; then export BWPORT_MUSICBUS=off; fi
SOLO=music NB=1 bash mrun.sh ${NAME}_$tag $s $SAVE $SECS "$@" > $OUT/${NAME}_$tag.out 2>&1
