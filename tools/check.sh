#!/bin/bash
# The quick check, for every merge and push (about a minute; docs/port-worker.md, "Routes"): builds build/port/macos's bwport, then
# replays the legs flagged `quick` in tools/routes/*.txt for all four games, in parallel, in Max from saves. Ends with PASS or FAIL.
# The full route (tools/route.py all, or one GAME) is for changes to game code, pokebw2 patches or the port's game hooks, and before an IPA.
# tools/check.sh [route.py options, e.g. -j 6]
ROOT=$(cd "$(dirname "$0")/.." && pwd)
LOG=${TMPDIR:-/tmp}/check-build.log
if [ -d "$ROOT/build/port/macos" ]; then
  nice -n 10 ninja -C "$ROOT/build/port/macos" bwport > "$LOG" 2>&1 || { tail -5 "$LOG"; echo "check: FAIL (build)"; exit 1; }
fi
if "$ROOT/tools/route.py" all --quick "$@"; then echo "check: PASS"; else echo "check: FAIL"; exit 1; fi
