#!/bin/sh
# The game data fill's unit tests (port/tests/asset_load_test.c, which includes port/src/asset_load.c): the assets.tsv rows
# and the image's .bwfill copies from synthetic ROMs. Builds it alone and runs it; prints one line a case, then PASS or FAIL.
set -e
here=$(cd "$(dirname "$0")" && pwd)
out=${TMPDIR:-/tmp}/asset_load_test.$$
cc -std=gnu11 -O1 -w -I"$here/../include" -o "$out" "$here/asset_load_test.c"
status=0
"$out" 2>/dev/null || status=$?
rm -f "$out"
exit $status
