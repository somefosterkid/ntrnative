#!/bin/bash
# tools/mac_package.sh [BUILD_DIR] : the macOS release app, zipped. BUILD_DIR (default build/port/macos-release) is set up with
#   tools/ios_deps.sh   (SDL2 and enet, static, for macOS too)
#   tools/port_wraps.py build/port/macos-release --native-file port/machine/macos-release.ini
#   ninja -C build/port/macos-release bwport
# and makes BUILD_DIR/bwport.app (Contents/MacOS/bwport stripped of its debug map, Info.plist from tools/mac/, the icon from
# tools/ios/AppIcon.png, licenses/ in Resources; ad-hoc signed, as the link's ROM fill leaves the exe) and
# build/bwport-macos-<rev>.zip. arm64, macOS 13.3 or later, nothing linked from outside the system (checked here). The app
# keeps its games, saves and settings in ~/Library/Application Support/bwport (port/src/main_start.c). The zip is a release
# package only from a build with every ROM present (the fill ran) and after tools/release_check.sh and tools/package_scan.sh,
# both run here.
set -eu
ROOT=$(cd "$(dirname "$0")/.." && pwd)
B=${1:-$ROOT/build/port/macos-release}
[ -x "$B/bwport" ] || { echo "$B/bwport: not built"; exit 1; }
APP=$B/bwport.app
rm -rf "$APP"; mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources/licenses"
cp "$ROOT/tools/mac/Info.plist" "$APP/Contents/Info.plist"
REV=$(git -C "$ROOT" rev-parse --short=9 HEAD)
plutil -replace CFBundleVersion -string "$REV" "$APP/Contents/Info.plist"
cp "$B/bwport" "$APP/Contents/MacOS/bwport"
strip -S "$APP/Contents/MacOS/bwport"   # the debug map (the .o paths); the symbols stay for the crash backtrace
cp "$ROOT"/licenses/* "$ROOT/THIRD_PARTY.md" "$ROOT/LICENSE" "$APP/Contents/Resources/licenses/"

# the icon: every size iconutil wants, from the 1024 PNG
IS=$B/AppIcon.iconset
rm -rf "$IS"; mkdir -p "$IS"
for s in 16 32 128 256 512; do
    sips -z $s $s "$ROOT/tools/ios/AppIcon.png" --out "$IS/icon_${s}x${s}.png" > /dev/null
    sips -z $((s * 2)) $((s * 2)) "$ROOT/tools/ios/AppIcon.png" --out "$IS/icon_${s}x${s}@2x.png" > /dev/null
done
iconutil -c icns "$IS" -o "$APP/Contents/Resources/AppIcon.icns"
rm -rf "$IS"

# nothing but the system: every dylib under /usr/lib or /System
bad=$(otool -L "$APP/Contents/MacOS/bwport" | tail -n +2 | awk '{print $1}' | grep -v -E '^/usr/lib/|^/System/' || true)
[ -z "$bad" ] || { echo "mac_package: REFUSED: links outside the system: $bad"; exit 1; }
minos=$(otool -l "$APP/Contents/MacOS/bwport" | awk '/LC_BUILD_VERSION/{f=1} f&&/minos/{print $2; exit}')
[ "$minos" = 13.3 ] || { echo "mac_package: REFUSED: minimum macOS $minos, not 13.3 (port/machine/macos-release.ini)"; exit 1; }

codesign --force -s - "$APP"
ZIP=$ROOT/build/bwport-macos-$REV.zip
rm -f "$ZIP"
(cd "$B" && ditto -c -k --keepParent bwport.app "$ZIP")
"$ROOT/tools/release_check.sh" "$ZIP"
"$ROOT/tools/package_scan.sh" "$ZIP"
ls -l "$ZIP"
