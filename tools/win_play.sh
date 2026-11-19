#!/bin/bash
# A Gen 4 game's play script (tools/plays/<game>/NAME.txt) on the win64 exe, HIDDEN (BWPORT_HIDDEN=1: no window, no taskbar button), at
# maximum speed, in a folder of its own (default C:\bwsmoke-<game>, never the deploy folder C:\bwport):
#
#   tools/win_play.sh GAME NAME [--no-build] [--out DIR] [--secs N] [--save NAME|PATH]
#
# GAME = platinum (the module build, tools/port_win64.sh --game platinum). The folder is a launcher root: the exe imports the game's ROM
# (orig/baserom_*_us.nds) into run\<game folder>\ once (the launcher's own importrom step, no window), then runs with BWPORT_GAME, the play
# script's keys (BWPORT_KEYS), the clock and seed pinned as tools/port_run.py pins a Platinum run, cfgscript ff=1, frame skip 4, a shot
# of the window at the script's end frame (run\<game folder>\wfinal.png) and quit. A script `from: SAVE` starts from /root/saves/SAVE.bin.
# Prints the exit, the last frame/fps lines of the log, and the shots. The exe is killed after --secs (default 300) if it is still up.
set -e
REPO=$(cd "$(dirname "$0")/.." && pwd)
GAME=$1; NAME=$2; shift 2
BUILD=1; SECS=300; SAVE=; OUT=
while [ $# -gt 0 ]; do
    case "$1" in
        --no-build) BUILD=; shift ;;
        --out) OUT=$2; shift 2 ;;
        --secs) SECS=$2; shift 2 ;;
        --save) SAVE=$2; shift 2 ;;
        *) echo "win_play: unknown option $1"; exit 2 ;;
    esac
done
case "$GAME" in
    platinum) CODE=CPUE; FOLDER="Pokemon Platinum"; ROMF=baserom_plat_us.nds ;;
    *) echo "win_play: game: platinum"; exit 2 ;;
esac
OUT=${OUT:-/mnt/c/bwsmoke-$GAME}
case "$OUT" in /mnt/c/bwport|/mnt/c/bwport/) echo "win_play: not in the deploy folder $OUT"; exit 2 ;; esac
PLAYF=$(ls "$REPO"/tools/plays/*/"$NAME".txt 2> /dev/null | head -1)
[ -f "$PLAYF" ] || { echo "win_play: no play script tools/plays/*/$NAME.txt"; exit 2; }
MAIN=$(cd "$REPO" && dirname "$(git rev-parse --path-format=absolute --git-common-dir)")
ROM=$MAIN/orig/$ROMF

eval "$(python3 - "$PLAYF" <<'PY'
import sys, shlex
sys.path.insert(0, 'tools')
import play_script
p = play_script.parse(open(sys.argv[1]).read(), sys.argv[1])
print('FROM=%s END=%d KEYS=%s' % (shlex.quote(p['from']), p['end_frame'], shlex.quote(' '.join(p['keys']))))
PY
)"
[ -n "$SAVE" ] || { [ "$FROM" = boot ] || SAVE=$FROM; }

W=(env BWPORT_WIN64_OUT="$OUT" bash "$REPO/tools/port_win64.sh" --game "$GAME")
EXE=$REPO/build/port/win64-plat/bwport.exe
if [ -n "$BUILD" ] || [ ! -f "$OUT/bwport.exe" ] || [ "$EXE" -nt "$OUT/bwport.exe" ]; then
    "${W[@]}" deploy > "$REPO/build/win_play_deploy.log" 2>&1 || { tail -20 "$REPO/build/win_play_deploy.log"; echo "win_play: build failed"; exit 1; }
fi
RUN=$OUT/run
mkdir -p "$RUN"
export BWPORT_HIDDEN=1 SDL_AUDIODRIVER=dummy BWPORT_NOPAUSE=1
# the import: the launcher's own step, no window (SDL's dummy video driver)
if [ ! -f "$RUN/$FOLDER/header.bin" ]; then
    BWPORT_LCTEST="importrom=$(wslpath -m "$ROM")" SDL_VIDEODRIVER=dummy "${W[@]}" run 120 > "$RUN/import_out.txt" 2>&1 || true
    grep -a -q 'lctest: 0 failed' "$RUN/bwport.log" || { echo "win_play: import failed"; tail -5 "$RUN/bwport.log"; exit 1; }
fi
rm -f "$RUN/imgui.ini" "$RUN/$FOLDER/wfinal.png" "$RUN/$FOLDER/save.bin" "$RUN/save.bin"
if [ -n "$SAVE" ]; then
    src=$SAVE; [[ $SAVE == */* ]] || src=${BWPORT_SAVES:-$HOME/saves}/$SAVE.bin
    [ -f "$src" ] || { echo "win_play: no save $src"; exit 2; }
    cp "$src" "$RUN/$FOLDER/save.bin"
fi
export BWPORT_GAME=$CODE BWPORT_KEYS="$KEYS" BWPORT_FRAMESKIP=4 BWPORT_FPSLOG=1 BWPORT_MUSICNORMAL=off \
       BWPORT_BENCH_CLOCK=2026-10-29T12:00:00 BWPORT_BENCH_SEED=100 \
       BWPORT_CFGSCRIPT="f1:ff=1;f$END:shot=wfinal;f$((END + 8)):quit=1"
unset BWPORT_LCTEST BWPORT_SKIPMENU
echo "win_play: $GAME $NAME (from ${SAVE:-boot}), ends at f$END, kill after $SECS s, in $OUT"
"${W[@]}" run "$SECS" > "$RUN/play_out.txt" 2>&1 || true
tail -3 "$RUN/play_out.txt"
echo "win_play: log tail:"; grep -a -v '^ovl: ' "$RUN/bwport.log" | tail -6 | sed 's/^/  /'
echo "win_play: last fps line: $(grep -a -i 'fps' "$RUN/bwport.log" | tail -1)"
for f in "$RUN/$FOLDER"/wfinal.png; do [ -f "$f" ] && echo "win_play: shot $f ($(stat -c %s "$f") bytes)" || echo "win_play: NO final shot"; done
left=$(cd "$OUT" && powershell.exe -NoProfile -Command "(Get-Process bwport -ErrorAction SilentlyContinue | Where-Object { \$_.Path -like '$(wslpath -w "$OUT")\\*' } | Measure-Object).Count" | tr -d '\r')
echo "win_play: bwport processes left in $OUT: $left"
