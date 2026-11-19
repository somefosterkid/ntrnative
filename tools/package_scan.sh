#!/bin/bash
# tools/package_scan.sh PACKAGE... : what tools/release_check.sh does not look for in a release package (a zip, an IPA, an
# APK or a folder): a player's files (a save, a settings or launcher file, a ROM, a backup folder) and local paths (a home
# folder, the build machine's name). Prints one line per finding and a verdict; exit 1 when there are any.
set -u
[ $# -ge 1 ] || { echo "usage: tools/package_scan.sh PACKAGE..."; exit 2; }
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT
n=0
# the machine's names (macOS's Computer Name and local host name; elsewhere the host name), when longer than a common word
host='/Users/|/home/|/private/var/|/private/tmp/'
for h in "$(scutil --get ComputerName 2> /dev/null)" "$(scutil --get LocalHostName 2> /dev/null)" "$(hostname -s)"; do
    [ ${#h} -ge 8 ] && host="$host|$h"
done
for P in "$@"; do
    D=$T/$(basename "$P")
    mkdir -p "$D"
    if [ -d "$P" ]; then cp -R "$P" "$D/"; else unzip -qq "$P" -d "$D" || { echo "$P: not a zip"; n=$((n + 1)); continue; }; fi
    while IFS= read -r f; do
        echo "$P: player file: ${f#$D/}"; n=$((n + 1))
    done < <(find "$D" \( -iname '*.sav' -o -iname 'save*.bin' -o -iname 'sim_config.ini' -o -iname 'launcher.ini' -o -iname 'imgui.ini' \
        -o -iname '*.nds' -o -iname 'header.bin' -o -name backups \) -print)
    while IFS= read -r f; do
        hits=$(strings -a "$f" | grep -E -c "$host")
        [ "$hits" -gt 0 ] && { echo "$P: ${f#$D/}: $hits local path(s), e.g. $(strings -a "$f" | grep -E -m1 "$host" | cut -c1-80)"; n=$((n + 1)); }
    done < <(find "$D" -type f)
done
if [ $n = 0 ]; then echo "package_scan: no player files or local paths in $*"; else echo "package_scan: REFUSED: $n finding(s)"; exit 1; fi
