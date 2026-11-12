#!/bin/bash
# tools/release_check.sh PACKAGE... : the release gate. A package that leaves this machine (an APK, an IPA, the win64 zip)
# must hold no bytes of the game's ROM: the game's data comes from the player's own ROM at start (port/src/asset_load.c,
# docs/port-build.md "Game data from the ROM"). Runs tools/rom_leak_check.py --package over each package (an apk/ipa/zip's
# members, a folder's files, plain files), prints each file with ROM bytes and the total, and exits 1 if there are any
# (the run list in build/release_check.tsv: file, offset, length, ROM source, ROM offset; tools/rom_leak_check.py names no bytes).
# Needs the ROMs and their extracts (rom_leak_check.py's DEFAULT_ROMS); without them it refuses (exit 2), so a machine that
# can't check can't publish. Called by tools/android_publish.sh and tools/port_win64.sh zip (tools/ios_publish.sh: docs/ios-handoff.md).
set -u
ROOT=$(cd "$(dirname "$0")/.." && pwd)
[ $# -ge 1 ] || { echo "usage: tools/release_check.sh PACKAGE..."; exit 2; }
mkdir -p "$ROOT/build"
python3 "$ROOT/tools/rom_leak_check.py" --package "$@" --detail "$ROOT/build/release_check.tsv"
rc=$?
if [ $rc = 0 ]; then
    echo "release_check: no ROM bytes in $*"
elif [ $rc = 1 ]; then
    echo "release_check: REFUSED: the package holds bytes of the ROM (runs in build/release_check.tsv)"
else
    echo "release_check: REFUSED: the check could not run"
fi
exit $rc
