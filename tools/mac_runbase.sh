#!/bin/bash
# Prints the run-folder template $HOME/bw-cache/runbase (the ROM extracted once; tools/mac_run.sh clones it with cp -cR), making it
# when missing or stale. It records what it was made from in runbase/.stamp: the ROM's size and mtime and the hash of the extraction code
# (port/src/main_start.c and launcher_core.c). Holds ROM bytes: outside git, never copy it elsewhere. Needs build/port/macos/bwport.
set -u
ROOT=$(cd "$(dirname "$0")/.." && pwd)
BASE=${BWPORT_RUNBASE:-$HOME/bw-cache/runbase}
ROM=${BWPORT_ROM:-$("$ROOT/tools/mac_rom.sh")}
STAMP="$(stat -f '%z %m' "$ROM") $(cat "$ROOT/port/src/main_start.c" "$ROOT/port/src/launcher_core.c" | shasum | cut -d' ' -f1)"
if [ -f "$BASE/.stamp" ] && [ "$(cat "$BASE/.stamp")" = "$STAMP" ]; then echo "$BASE"; exit 0; fi
# A worktree without a macOS build must not wipe the shared template (ios_sim_run.sh --noruntbase skips it).
BIN=$ROOT/build/port/macos/bwport
[ -x "$BIN" ] || { echo "mac_runbase: no $BIN, the template is left as it is (build bwport, or pass --noruntbase)" >&2; exit 1; }
# Built beside the template and swapped in with mv, so a run cloning the old one never sees it half deleted.
NEW=$BASE.new.$$; LOG=$BASE.$$.log
rm -rf "$NEW"; mkdir -p "$NEW"
BWPORT_DIR=$NEW BWPORT_ROM=$ROM BWPORT_HIDDEN=1 SDL_AUDIODRIVER=dummy "$BIN" > "$LOG" 2>&1 &
PID=$!
for i in $(seq 1 300); do
  grep -q "files extracted" "$LOG" && break
  kill -0 $PID 2>/dev/null || break
  sleep 0.5
done
kill -9 $PID 2>/dev/null; wait $PID 2>/dev/null
if [ ! -f "$NEW/a/0/0/0" ]; then echo "mac_runbase: extraction failed (see $LOG)" >&2; rm -rf "$NEW"; exit 1; fi
rm -f "$NEW/save.bin" "$NEW/imgui.ini" "$NEW/sim_config.ini" "$LOG"
echo "$STAMP" > "$NEW/.stamp"
OLD=$BASE.old.$$
[ -d "$BASE" ] && mv "$BASE" "$OLD"
mv "$NEW" "$BASE" && rm -rf "$OLD"
echo "$BASE"
