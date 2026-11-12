#!/bin/bash
# libntr_chain.sh [REPO] [CHECKOUT_DIR]
# Checks the libntr patch chain: applies the wrap's diff_files (REPO/port/subprojects/libntr.wrap, packagefiles/ in that order) to a
# fresh libntr checkout at the wrap's revision, as meson would, so a new patch that only applies on top of your own checkout (or in
# the wrong place in the list) is caught. REPO defaults to the repo this script is in; CHECKOUT_DIR to REPO/build/libntr-chain
# (git-ignored). The checkout is fetched from the wrap's url (one commit, `git fetch --depth 1`) when the dir is missing or lacks
# the revision, and reset and cleaned before every run; it is a scratch copy: never edit it.
# Prints "ok N patches at REVISION", or "FAIL PATCH" (the first that does not apply) and exits 1.
# Run it after adding or changing a libntr-*.patch: the new patch is the LAST name in diff_files (the maintainers' notes).
# LIB=NAME checks NAME.wrap's chain instead (checkout dir REPO/build/NAME-chain; libntr is the one wrap left).
set -u
REPO=$(cd "${1:-$(dirname "$0")/..}" && pwd)
LIB=${LIB:-libntr}
C=${2:-$REPO/build/$LIB-chain}
WRAP=$REPO/port/subprojects/$LIB.wrap
rev=$(grep '^revision' "$WRAP" | sed 's/.*= *//')
url=$(grep '^url' "$WRAP" | sed 's/.*= *//')
[ -n "$rev" ] && [ -n "$url" ] || { echo "no url/revision in $WRAP"; exit 2; }
if ! git -C "$C" cat-file -e "$rev^{commit}" 2> /dev/null; then
  mkdir -p "$C"
  git -C "$C" init -q 2> /dev/null
  git -C "$C" fetch -q --depth 1 "$url" "$rev" || { echo "cannot fetch $rev from $url"; exit 2; }
fi
git -C "$C" reset -q --hard "$rev" && git -C "$C" clean -qfd
n=0
for p in $(grep '^diff_files' "$WRAP" | sed 's/diff_files *= *//; s/,/ /g'); do
  git -C "$C" apply --whitespace=nowarn "$REPO/port/subprojects/packagefiles/$p" || { echo "FAIL $p"; exit 1; }
  n=$((n + 1))
done
echo "ok $n patches at $rev"
