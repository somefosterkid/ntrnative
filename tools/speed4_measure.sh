#!/bin/bash
# Frames a tile for the Gen 4 Walk / Run speed options, from BWPORT_MOVELOG4 (port/game/speed_gen4.c PortSpeed4_MoveLog).
# usage: tools/speed4_measure.sh GAME SAVE DIR KIND SPEED [HOLD] [START]
#   GAME platinum|heartgold|soulsilver, SAVE a saves-library name, DIR up|down|left|right, KIND walk|run (run holds B too),
#   SPEED 1|2|4, HOLD frames the direction is held (default 120), START the V-blank the hold starts (default: the field is
#   up by then after CONTINUE: Platinum 8600, HG/SS 11000).
# Prints: the V-blanks between the player's tile changes, the player's tile < the follower's tile : follow state at each
# change, and the step ends the field saw (PLAYER_MOVE_STATE_END frames: wild encounters and step counters run on them).
# MODE=2 (BWPORT_MOVELOG4=2) also logs the pixel position each V-blank it moves. OUT=DIR keeps the logs (default
# ~/.cache/bwport/speed4); each run has its own run dir there, so several can run at once.
cd "$(git rev-parse --show-toplevel)" || exit 2
game=$1; save=$2; dir=$3; kind=$4; sp=$5; hold=${6:-120}
keep=${OUT:-$HOME/.cache/bwport/speed4}
if [ "$game" = platinum ]; then
    pre="start@f6300 a@f6700"; st=${7:-8600}
else
    pre="start@f7700 a@f8300 a@f8700 a@f9100 b@f10600"; st=${7:-11000}
fi
tag=${game}_${save}_${kind}${sp}
mkdir -p "$keep/run_$tag"
if [ "$kind" = run ]; then
    keys="$pre b*f$((hold + 20))@f$st $dir*f$hold@f$st"; export BWPORT_RUNSPEED=$sp BWPORT_WALKSPEED=1
else
    keys="$pre $dir*f$hold@f$st"; export BWPORT_WALKSPEED=$sp BWPORT_RUNSPEED=1
fi
export BWPORT_SURFSPEED=1 BWPORT_MOVELOG4=${MODE:-1}
python3 tools/port_run.py --no-build --dir "$keep/run_$tag" --game "$game" --save "$save" --keys "$keys" \
    --frames $((st + hold + 90)) --skip 1 -t 150 > "$keep/$tag.run" 2>&1
log=$(sed -n 's/^port_run: \(.*log\.txt\),.*/\1/p' "$keep/$tag.run" | tail -1)
grep -h "^move4" "$log" > "$keep/$tag.log"
# the tiles after the hold starts (the first dt counts from the last tile change before it)
awk -v st="$st" -v tag="$tag" -v verdict="$(tail -1 "$keep/$tag.run" | cut -c1-60)" '
/^move4:/ { split($2, v, "="); vb = v[2] + 0; if (vb < st) { next }
            split($3, t, "="); split($4, d, "="); split($5, f, "="); split($6, s, "="); split($7, e, "=")
            n++; dts = dts " " d[2]; fol = fol " " t[2] "<" f[2] ":" s[2]; if (n == 1) { e0 = e[2] } e1 = e[2] }
/^move4f:/ { split($2, v, "="); if (v[2] + 0 >= st) { split($3, f, "="); split($4, p, "="); lastf = f[2] " (player " p[2] ", vb " v[2] ")"; nf++ } }
END { printf "%s tiles=%d step-ends=%d dt:%s\n    player<follower:state%s\n    follower tiles=%d, last %s [%s]\n", tag, n, e1 - e0, dts, fol, nf, lastf, verdict }' "$keep/$tag.log"
