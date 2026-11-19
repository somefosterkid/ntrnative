#!/bin/sh
# The launcher's HeartGold / SoulSilver save check (port/tests/launcher_hgss_save_test.c): builds it with the launcher core
# (port/src/launcher_core.c) alone, Platinum and HeartGold in the table, and runs it on synthetic saves and on the real saves
# given as arguments (.sav files the game wrote, e.g. a route test's save library); prints one line a case and PASS or FAIL.
set -e
here=$(cd "$(dirname "$0")" && pwd)
out=${TMPDIR:-/tmp}/launcher_hgss_save_test.$$
cc -std=gnu11 -O1 -w -DBWPORT_PLAT -DBWPORT_HG -I"$here/../src" -o "$out" "$here/launcher_hgss_save_test.c" "$here/../src/launcher_core.c"
status=0
"$out" "$@" || status=$?
rm -f "$out"
exit $status
