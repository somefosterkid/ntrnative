#!/bin/bash
# The win64 exe, HIDDEN (no window, no taskbar button: BWPORT_HIDDEN=1, libntr-hidden-pc.patch), from a save for N seconds.
#
#   tools/win_smoke.sh [--save NAME|PATH] [--shots K] [--keys 'a@f300 ...'] [--no-build] [--out DIR] [SECONDS]
#
# SECONDS default 30; --save default room ($BWPORT_SAVES/NAME.bin, copied to save.bin; BWPORT_SKIPMENU=1 goes straight into
# CONTINUE); --shots K a picture every K s (default 10; 0 none). Builds (tools/port_win64.sh build) and fills its OWN folder
# (default C:\bwsmoke, never the deploy folder C:\bwport), starts the exe through tools/port_win64.sh run, which kills it when the
# time is up (by its folder's path, never another folder's game). While it runs, PowerShell reports its PID and MainWindowHandle
# (0 = no window). Prints: window check, the last frame the log reached ("fps" lines), the shot paths, the exit.
set -e
REPO=$(cd "$(dirname "$0")/.." && pwd)
SAVE=room; SHOTS=10; KEYS=; BUILD=1; OUT=${BWPORT_WIN64_OUT:-/mnt/c/bwsmoke}
while [ $# -gt 0 ]; do
    case "$1" in
        --save) SAVE=$2; shift 2 ;;
        --shots) SHOTS=$2; shift 2 ;;
        --keys) KEYS=$2; shift 2 ;;
        --no-build) BUILD=; shift ;;
        --out) OUT=$2; shift 2 ;;
        *) SECS=$1; shift ;;
    esac
done
SECS=${SECS:-30}
RUN=$OUT/run
case "$OUT" in /mnt/c/bwport|/mnt/c/bwport/) echo "win_smoke: not in the deploy folder $OUT"; exit 2 ;; esac
src=$SAVE; [[ $SAVE == */* ]] || src=${BWPORT_SAVES:-$HOME/saves}/$SAVE.bin
[ -f "$src" ] || { echo "win_smoke: no save $src"; exit 2; }

W=(env BWPORT_WIN64_OUT="$OUT" bash "$REPO/tools/port_win64.sh")
if [ -n "$BUILD" ] || [ ! -f "$OUT/bwport.exe" ] || [ "$REPO/build/port/win64/bwport.exe" -nt "$OUT/bwport.exe" ]; then
    # the folder's own deploy (never C:\bwport: --out above), not the deploy folder's
    "${W[@]}" deploy > "$REPO/build/win_smoke_deploy.log" 2>&1 || { tail -20 "$REPO/build/win_smoke_deploy.log"; echo "win_smoke: build failed"; exit 1; }
fi
mkdir -p "$RUN"
rm -rf "$RUN/shots"; cp "$src" "$RUN/save.bin"

# window check in the background: the exe's PID and whether any window handle shows up
CHK=$RUN/window_check.txt; rm -f "$CHK"
(
    sleep $((SECS < 12 ? SECS / 2 : 12))
    cd "$OUT" && powershell.exe -NoProfile -Command "Get-Process bwport -ErrorAction SilentlyContinue | Where-Object { \$_.Path -like '$(wslpath -w "$OUT")\\*' } | ForEach-Object { 'pid ' + \$_.Id + ' MainWindowHandle ' + \$_.MainWindowHandle + ' title [' + \$_.MainWindowTitle + ']' }" > "$CHK" 2>&1
) &
chk=$!

export BWPORT_HIDDEN=1 BWPORT_DIR="$RUN" BWPORT_SKIPMENU=1 SDL_AUDIODRIVER=${SDL_AUDIODRIVER:-dummy} BWPORT_FPSLOG=1
[ -n "$KEYS" ] && export BWPORT_KEYS="$KEYS"
args=(run "$SECS")
[ "$SHOTS" != 0 ] && args+=(--shots "$SHOTS")
"${W[@]}" "${args[@]}" > "$RUN/smoke_out.txt" 2>&1 || true
wait $chk 2> /dev/null || true

echo "win_smoke: window check (MainWindowHandle 0 = none):"; sed 's/^/  /' "$CHK" 2> /dev/null || echo "  (exe not found running)"
echo "win_smoke: log tail:"; grep -a -v '^ovl: ' "$RUN/bwport.log" | tail -4 | sed 's/^/  /'
echo "win_smoke: $(grep -a -c . "$RUN/bwport.log") log lines; last frame/fps: $(grep -a -i 'fps\|frame' "$RUN/bwport.log" | tail -1)"
tail -1 "$RUN/smoke_out.txt"
if [ -d "$RUN/shots" ]; then
    for f in "$RUN"/shots/*.png; do [ -f "$f" ] && echo "win_smoke: shot $f ($(stat -c %s "$f") bytes)"; done
fi
left=$(cd "$OUT" && powershell.exe -NoProfile -Command "(Get-Process bwport -ErrorAction SilentlyContinue | Where-Object { \$_.Path -like '$(wslpath -w "$OUT")\\*' } | Measure-Object).Count" | tr -d '\r')
echo "win_smoke: bwport processes left in $OUT: $left"
