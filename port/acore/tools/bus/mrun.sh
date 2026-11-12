#!/bin/bash
# usage: mrun.sh TAG SPEED SAVE SECS [MOVES...]   env: SOLO=music|rest|"" MN=on|off FAST=1 NB=1 (no build) SCRIPT=ticks:speed list
TAG=$1; S=$2; SAVE=$3; SECS=$4; shift; shift; shift; shift
TOOLS=$(cd "$(dirname "$0")" && pwd); OUT=${OUT:-/root/mt/bus}; mkdir -p $OUT
cd ${WT:-$(git -C $TOOLS rev-parse --show-toplevel)}
mkdir -p $OUT
D=build/port/run_$TAG
mkdir -p $D
W=$OUT/$TAG.wav
rm -f $W
MV=$(python3 $TOOLS/walk.py ${START:-2600} ${STEP:-48} "$@" 2>/dev/null)
KEYS="start@f600 start@f900 start@f1300 a@f1700 $MV $XKEYS"
export BWPORT_GAMESPEED=$S BWPORT_AUDIO_WAV=$W BWPORT_AUDIOLOG=1 BWPORT_MUSICNORMAL=${MN:-on} BWPORT_SNDLOG=1
[ -n "$SOLO" ] && export BWPORT_ACORE_SOLO=$SOLO
[ -n "$SCRIPT" ] && export BWPORT_SPEEDSCRIPT=$SCRIPT
export BWPORT_KEYS="$KEYS"
ARGS=""
[ -n "$NB" ] && ARGS="$ARGS --no-build"
[ -n "$FAST" ] && ARGS="$ARGS --fast"
python3 tools/port_run.py $ARGS --dir $D --shots ${SHOTS:-0} -t $SECS --save $SAVE $EXTRA > $OUT/run_${TAG}.log 2>&1 </dev/null
tail -1 $OUT/run_${TAG}.log
grep "^audio: frame" $D/out/log.txt | tail -1
grep "^snd:" $D/out/log.txt | head -5
ls -l $W | awk '{print $5}'
