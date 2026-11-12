#!/bin/bash
# Android packaging (the maintainers' notes, "Android"):  tools/android_package.sh [--build]
# Makes build/android/bwport.apk with both ABIs (arm64-v8a for phones, x86_64 for the emulator) from the meson builds
# build/port/android-arm64 and build/port/android-x86_64 (--build runs their ninja first; BWPORT_ANDROID_ABIS="x86_64" packs one).
# The Gradle project tools/android is copied to build/android/project with the game (stripped, as lib/<abi>/libmain.so), SDL's
# libSDL2.so and Java glue (tools/android_deps.sh built them from the SDL release) and the icon (tools/ios/AppIcon.png).
# Signed with the release key when its properties file is there (BWPORT_ANDROID_KEYS, default C:\stress\android-keys\
# signing.properties: storeFile, storePassword, keyAlias, keyPassword; outside git), else with the debug key (no install over a
# release-signed one). versionCode = the commit count, so each build installs over the last.
# The unstripped libraries are kept under build/android/symbols/<git hash>/<abi>/ (ndk-stack -sym DIR reads a crash from logcat).
# The APK holds the game's compiled code and bytes from the user's ROM: never commit or share it (tools/android_publish.sh).
set -eu
ROOT=$(cd "$(dirname "$0")/.." && pwd)
A=${ANDROID_ROOT:-/opt/android}
KEYS=${BWPORT_ANDROID_KEYS:-$HOME/.bwport-android/signing.properties}
ABIS=${BWPORT_ANDROID_ABIS:-"arm64-v8a x86_64"}
NDK=$A/sdk/$(grep -o "ndk/[0-9.]*" "$ROOT/port/machine/android-arm64.ini" | head -1)
export JAVA_HOME=$A/jdk ANDROID_HOME=$A/sdk GRADLE_USER_HOME=$A/gradle-home
export PATH=$JAVA_HOME/bin:$PATH
P=$ROOT/build/android/project
cd "$ROOT"

builddir() { case $1 in arm64-v8a) echo build/port/android-arm64 ;; x86_64) echo build/port/android-x86_64 ;; *) echo "unknown ABI $1" >&2; exit 2 ;; esac; }
if [ "${1:-}" = --build ]; then
    for ABI in $ABIS; do
        B=$(builddir $ABI)
        [ -f $B/build.ninja ] || meson setup $B port --native-file port/machine/${B#build/port/}.ini < /dev/null > /dev/null
        nice -n 5 ninja -C $B -j "${BW_JOBS:-8}" bwport < /dev/null | tail -1
    done
fi

HASH=$(git rev-parse --short=9 HEAD)
git diff --quiet HEAD 2> /dev/null || HASH=$HASH-dirty
rm -rf "$P"
mkdir -p "$P/app/src/main/java/org" "$P/app/src/main/res/mipmap-xxxhdpi"
cp -r tools/android/. "$P/"
cp -r "$A/deps/sdl-java/org/." "$P/app/src/main/java/org/"
cp tools/ios/AppIcon.png "$P/app/src/main/res/mipmap-xxxhdpi/ic_launcher.png"
# the notices (SDL, enet, libntr, ImGui, libc++ ...) ride in the APK as assets/licenses/ (THIRD_PARTY.md says what each covers)
mkdir -p "$P/app/src/main/assets/licenses"
cp licenses/* THIRD_PARTY.md LICENSE "$P/app/src/main/assets/licenses/"
echo "sdk.dir=$A/sdk" > "$P/local.properties"
for ABI in $ABIS; do
    SO=$(builddir $ABI)/bwport
    [ -f "$SO" ] || { echo "$SO: not built (ninja -C $(builddir $ABI) bwport, or --build)"; exit 1; }
    J=$P/app/src/main/jniLibs/$ABI
    S=build/android/symbols/$HASH/$ABI
    mkdir -p "$J" "$S"
    "$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" --strip-unneeded -o "$J/libmain.so" "$SO"
    "$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" --strip-unneeded -o "$J/libSDL2.so" "$A/deps/$ABI/lib/libSDL2.so"
    # libmain.so links the NDK's shared C++ runtime (-lc++ in meson's link line, despite -static-libstdc++): without it the app dies at
    # System.loadLibrary ("libc++_shared.so not found")
    TRIPLE=$([ "$ABI" = arm64-v8a ] && echo aarch64-linux-android || echo x86_64-linux-android)
    "$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" --strip-unneeded -o "$J/libc++_shared.so" "$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$TRIPLE/libc++_shared.so"
    cp "$SO" "$S/libmain.so"
    cp "$A/deps/$ABI/lib/libSDL2.so" "$S/"
done

PROPS=(-PbwVersionCode="$(git rev-list --count HEAD)" -PbwVersionName="0.1-$HASH")
if [ -f "$KEYS" ]; then PROPS+=(-PbwSigning="$KEYS"); else echo "no $KEYS: the APK is signed with the debug key"; fi
"$A/gradle/bin/gradle" -p "$P" --no-daemon --console=plain assembleRelease "${PROPS[@]}" < /dev/null > build/android/gradle.log 2>&1 \
    || { grep -v "^\s*at \|^$" build/android/gradle.log | tail -30; exit 1; }
cp "$P/app/build/outputs/apk/release/app-release.apk" build/android/bwport.apk
"$A/sdk/build-tools/35.0.0/apksigner" verify --print-certs build/android/bwport.apk | grep -m1 "SHA-256" || true
echo "build/android/bwport.apk ($(du -m build/android/bwport.apk | cut -f1) MB, $HASH, ABIs $ABIS); symbols in build/android/symbols/$HASH"
