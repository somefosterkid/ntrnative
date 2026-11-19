#!/bin/sh
# The launcher's Platinum save check (port/tests/launcher_plat_save_test.c): builds it with the launcher core
# (port/src/launcher_core.c) alone, Platinum in the table, and runs it; prints one line a case and PASS or FAIL.
set -e
here=$(cd "$(dirname "$0")" && pwd)
out=${TMPDIR:-/tmp}/launcher_plat_save_test.$$
cc -std=gnu11 -O1 -w -DBWPORT_PLAT -DBWPORT_HG -I"$here/../src" -o "$out" "$here/launcher_plat_save_test.c" "$here/../src/launcher_core.c"
status=0
"$out" || status=$?
rm -f "$out"
exit $status
