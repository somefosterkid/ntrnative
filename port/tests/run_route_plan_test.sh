#!/bin/sh
# The route planner's unit test (port/tests/route_plan_test.c): builds it with the planner (port/src/route_map.c) alone
# and runs it; prints one line a case and PASS or FAIL.
set -e
here=$(cd "$(dirname "$0")" && pwd)
out=${TMPDIR:-/tmp}/route_plan_test.$$
cc -std=gnu11 -O1 -Wall -I"$here/../include" -o "$out" "$here/route_plan_test.c" "$here/../src/route_map.c"
status=0
"$out" || status=$?
rm -f "$out"
exit $status
