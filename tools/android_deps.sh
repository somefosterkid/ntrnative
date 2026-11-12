#!/bin/bash
# Android toolchain and dependencies (the maintainers' notes, "Android"), outside the repo in $ANDROID_ROOT (default
# /opt/android): JDK 17 (Temurin), the Android SDK (command-line tools, platform-tools, a platform, build-tools, the NDK,
# the emulator and an x86_64 Google APIs system image), Gradle, an emulator AVD, and SDL2 + enet built with the NDK for
# arm64-v8a (phones) and x86_64 (the emulator on this host), plus SDL's Android Java glue (SDLActivity) from the same
# SDL release. Versions pinned below. Safe to run again: each piece is skipped when it is there at its version.
#   tools/android_deps.sh            install / check everything (about 4 GB of downloads the first time)
# RUNNING IT ACCEPTS THE ANDROID SDK LICENCES (sdkmanager --licenses, answered yes): read them with
# `/opt/android/sdk/cmdline-tools/latest/bin/sdkmanager --licenses` first if that has not been agreed.
# sdkmanager installs the newest revision of a package path: platform-tools, the emulator and the system image have no
# version in their path, so the script prints what it got and warns when it is not the revision tested here.
set -eu
A=${ANDROID_ROOT:-/opt/android}
JDK_VER=17.0.21+9; JDK_SHA=8b9b611d917e3dfd8419d3566b5949226f69b052fed7e109c1368add1dfe8711
CMDLINE_BUILD=16111833; CMDLINE_SHA1=e025545c62a8e64c7559119566a569fb1dec5f60    # command-line tools 23.0
GRADLE_VER=8.14.4; GRADLE_SHA=f1771298a70f6db5a29daf62378c4e18a17fc33c9ba6b14362e0cdf40610380d
NDK_VER=30.0.16248370      # r30, the 2026 LTS
PLATFORM=android-35; BUILD_TOOLS=35.0.0
SYSIMG="system-images;android-34;google_apis;x86_64"
TESTED="platform-tools 37.0.1, emulator 37.2.13, $SYSIMG 14"
SDL_VER=2.32.10; SDL_SHA=5f5993c530f084535c65a6879e9b26ad441169b3e25d789d83287040a9ca5165    # as tools/ios_deps.sh
ENET_VER=1.3.18; ENET_SHA=2a8a0c5360d68bb4fcd11f2e4c47c69976e8d2c85b109dd7d60b1181a4f85d36
MINSDK=24                  # port/machine/android-*.ini, tools/android/app/build.gradle
ABIS="arm64-v8a x86_64"
# the emulator needs one library Ubuntu does not have by default
dpkg -s libnss3 > /dev/null 2>&1 || DEBIAN_FRONTEND=noninteractive apt-get install -y -q libnss3 > /dev/null
AVD=bwport34

mkdir -p "$A/dl" "$A/deps/src"
# one installer at a time: mkdir is the lock (remove $A/.lock if a run was killed)
while ! mkdir "$A/.lock" 2>/dev/null; do echo "waiting for $A/.lock"; sleep 5; done
trap 'rmdir "$A/.lock"' EXIT

fetch() {   # file url sum (sha256, or sha1 when 40 hex digits)
    local f=$A/dl/$1 sum
    [ -f "$f" ] || { echo "download $1"; curl -sfL --retry 3 -o "$f.part" "$2" && mv "$f.part" "$f"; }
    if [ ${#3} = 40 ]; then sum=$(sha1sum "$f" | cut -d' ' -f1); else sum=$(sha256sum "$f" | cut -d' ' -f1); fi
    [ "$sum" = "$3" ] || { echo "$1: checksum mismatch ($sum)"; rm -f "$f"; exit 1; }
}

# JDK 17
if [ "$(cat "$A/jdk/.version" 2>/dev/null)" != "$JDK_VER" ]; then
    V=${JDK_VER/+/_}
    fetch jdk-$JDK_VER.tgz "https://github.com/adoptium/temurin17-binaries/releases/download/jdk-${JDK_VER/+/%2B}/OpenJDK17U-jdk_x64_linux_hotspot_$V.tar.gz" $JDK_SHA
    rm -rf "$A/jdk-$JDK_VER"; mkdir -p "$A/jdk-$JDK_VER"
    tar -xzf "$A/dl/jdk-$JDK_VER.tgz" -C "$A/jdk-$JDK_VER" --strip-components=1
    echo "$JDK_VER" > "$A/jdk-$JDK_VER/.version"
    ln -sfn "jdk-$JDK_VER" "$A/jdk"
fi
export JAVA_HOME=$A/jdk ANDROID_HOME=$A/sdk ANDROID_SDK_ROOT=$A/sdk ANDROID_AVD_HOME=$A/avd
export PATH=$JAVA_HOME/bin:$PATH

# the SDK's command-line tools, then the packages through them
SM=$A/sdk/cmdline-tools/latest/bin/sdkmanager
if [ ! -x "$SM" ] || ! grep -q "Pkg.Revision=23.0" "$A/sdk/cmdline-tools/latest/source.properties"; then
    fetch cmdline-tools-$CMDLINE_BUILD.zip "https://dl.google.com/android/repository/commandlinetools-linux-${CMDLINE_BUILD}_latest.zip" $CMDLINE_SHA1
    rm -rf "$A/sdk/cmdline-tools/latest" "$A/dl/cmdline-tools"
    mkdir -p "$A/sdk/cmdline-tools"
    unzip -q "$A/dl/cmdline-tools-$CMDLINE_BUILD.zip" -d "$A/dl"
    mv "$A/dl/cmdline-tools" "$A/sdk/cmdline-tools/latest"
fi
yes | "$SM" --licenses > /dev/null 2>&1 || true    # accepts every SDK licence (see the header)
PKGS=("platform-tools" "emulator" "platforms;$PLATFORM" "build-tools;$BUILD_TOOLS" "ndk;$NDK_VER" "$SYSIMG")
MISSING=()
for p in "${PKGS[@]}"; do
    [ -f "$A/sdk/${p//;//}/source.properties" ] || MISSING+=("$p")
done
if [ ${#MISSING[@]} -gt 0 ]; then
    echo "sdkmanager: ${MISSING[*]}"
    "$SM" --install "${MISSING[@]}" > "$A/dl/sdkmanager.log" 2>&1 || { tail -20 "$A/dl/sdkmanager.log"; exit 1; }
fi
GOT=""
for p in platform-tools emulator "${SYSIMG//;//}"; do
    GOT="$GOT${GOT:+, }${p//\//;} $(grep -m1 'Pkg.Revision=' "$A/sdk/$p/source.properties" | cut -d= -f2)"
done
echo "sdk: $GOT"
[ "$GOT" = "$TESTED" ] || echo "note: tested with $TESTED"

# Gradle (no wrapper in git: tools/android_package.sh runs this one)
if [ ! -x "$A/gradle-$GRADLE_VER/bin/gradle" ]; then
    fetch gradle-$GRADLE_VER.zip "https://services.gradle.org/distributions/gradle-$GRADLE_VER-bin.zip" $GRADLE_SHA
    rm -rf "$A/gradle-$GRADLE_VER"
    unzip -q "$A/dl/gradle-$GRADLE_VER.zip" -d "$A"
fi
ln -sfn "gradle-$GRADLE_VER" "$A/gradle"

# the emulator's device: a phone-sized Google APIs x86_64 image (tools/android_emu.sh runs it headless)
if [ ! -f "$A/avd/$AVD.avd/config.ini" ]; then
    mkdir -p "$A/avd"
    echo no | "$A/sdk/cmdline-tools/latest/bin/avdmanager" create avd -n $AVD -k "$SYSIMG" -d pixel_7 --force > /dev/null
fi
# no hardware keyboard: the emulator's "qwerty2" keyboard shows up in SDL as a joystick, and a joystick hides the touch controls
CFG=$A/avd/$AVD.avd/config.ini
for kv in hw.ramSize=4096 disk.dataPartition.size=8G hw.keyboard=no hw.dPad=no hw.audioInput=no hw.cpu.ncore=4; do
    sed -i "/^${kv%%=*}=/d" "$CFG"
    echo "$kv" >> "$CFG"
done

# SDL2 (shared: SDLActivity loads libSDL2.so, then libmain.so) and enet (static), per ABI, with the NDK's cmake toolchain
NDK=$A/sdk/ndk/$NDK_VER
STAMP="sdl2 $SDL_VER enet $ENET_VER ndk $NDK_VER minsdk $MINSDK"
if [ "$(cat "$A/deps/stamp" 2>/dev/null)" != "$STAMP" ]; then
    rm -rf "$A/deps"/b-* "$A/deps/sdl-java"
    for ABI in $ABIS; do rm -rf "$A/deps/$ABI"; done
    src() {   # name url sha
        fetch $1.tgz "$2" $3
        [ -d "$A/deps/src/$1" ] || { mkdir -p "$A/deps/src/$1" && tar -xzf "$A/dl/$1.tgz" -C "$A/deps/src/$1" --strip-components=1; }
    }
    src SDL2-$SDL_VER "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-$SDL_VER.tar.gz" $SDL_SHA
    src enet-$ENET_VER "http://enet.bespin.org/download/enet-$ENET_VER.tar.gz" $ENET_SHA
    for ABI in $ABIS; do
        P=$A/deps/$ABI
        COMMON="-DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-$MINSDK
                -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$P -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -G Ninja"
        cmake -S "$A/deps/src/SDL2-$SDL_VER" -B "$A/deps/b-sdl2-$ABI" $COMMON -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TEST=OFF > /dev/null
        cmake --build "$A/deps/b-sdl2-$ABI" -j "${BW_JOBS:-8}" > /dev/null
        cmake --install "$A/deps/b-sdl2-$ABI" > /dev/null
        cmake -S "$A/deps/src/enet-$ENET_VER" -B "$A/deps/b-enet-$ABI" $COMMON > /dev/null
        cmake --build "$A/deps/b-enet-$ABI" -j "${BW_JOBS:-8}" > /dev/null
        cmake --install "$A/deps/b-enet-$ABI" > /dev/null
        [ -f "$P/lib/libenet.a" ] || cp "$P/lib/static/libenet.a" "$P/lib/" 2>/dev/null || cp "$A/deps/b-enet-$ABI/libenet.a" "$P/lib/"
        [ -d "$P/include/enet" ] || cp -r "$A/deps/src/enet-$ENET_VER/include/enet" "$P/include/"
        echo "$ABI: $(ls "$P/lib" | tr '\n' ' ')"
    done
    # SDL's Java side, from the same release: tools/android_package.sh copies it into the Gradle project
    mkdir -p "$A/deps/sdl-java"
    cp -r "$A/deps/src/SDL2-$SDL_VER/android-project/app/src/main/java/org" "$A/deps/sdl-java/"
    echo "$STAMP" > "$A/deps/stamp"
fi
echo "android toolchain ready in $A (JDK $JDK_VER, NDK $NDK_VER, $PLATFORM, build-tools $BUILD_TOOLS, Gradle $GRADLE_VER, SDL2 $SDL_VER, enet $ENET_VER)"
