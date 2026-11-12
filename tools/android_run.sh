#!/bin/bash
# A timed game run in the Android emulator (the maintainers' notes, "Android"), the iOS Simulator's tools/ios_sim_run.sh for Android:
#   tools/android_run.sh [options] SECONDS [KEYS] [SHOTS] [TAPS]
#     KEYS   BWPORT_KEYS for the game (frames or seconds, as on the desktop; "" for none)
#     SHOTS  wall seconds after the start for screenshots, comma-separated ("12,20"): OUT/sNNN.png (adb exec-out screencap)
#     TAPS   "X,Y@S ..." taps at wall seconds S (adb shell input tap, screen pixels: real touches through Android's input);
#            "X,Y,MS@S" holds the point MS milliseconds (input swipe from it to itself)
#   options: --serial emulator-5554 (default)  --apk build/android/bwport.apk (default; installed with -r)  --out DIR
#            (default build/android/run)  --renderer vcore|gpu (BWPORT_RENDERER)  --save NAME (<saves>/NAME.bin as save.bin, and
#            the CONTINUE keys before KEYS)  --fresh (the app's files deleted first: the ROM is pushed again and extracted at start)
#            --env "K=V K=V" (more BWPORT_*/SDL_* variables; they reach the game as start-intent extras: BwportActivity.java)
# The emulator must be up: tools/android_emu.sh run (in the background) + wait. The ROM comes from BWPORT_ROM or the main checkout's
# orig/baserom_b_us.nds; it is pushed into the app's files only when none is there. Results in OUT: log.txt (the game's stdout and
# stderr), logcat.txt (the whole device log of the run), sNNN.png; the game is stopped at the end (am force-stop).
set -eu
ROOT=$(cd "$(dirname "$0")/.." && pwd)
A=${ANDROID_ROOT:-/opt/android}
GAME=black SERIAL=emulator-5554 APK=$ROOT/build/android/bwport.apk OUT=$ROOT/build/android/run RENDERER= SAVE= FRESH=0 ENVS=
while [ $# -gt 0 ]; do
    case $1 in
    --serial) SERIAL=$2; shift 2 ;;
    --apk) APK=$2; shift 2 ;;
    --out) OUT=$2; shift 2 ;;
    --renderer) RENDERER=$2; shift 2 ;;
    --save) SAVE=$2; shift 2 ;;
    --fresh) FRESH=1; shift ;;
    --env) ENVS=$2; shift 2 ;;
    --game) GAME=$2; shift 2 ;;
    *) break ;;
    esac
done
SECONDS_RUN=${1:?usage: $0 [options] SECONDS [KEYS] [SHOTS] [TAPS]}
KEYS=${2:-}
SHOTS=${3:-}
TAPS=${4:-}
ADB="$A/sdk/platform-tools/adb -s $SERIAL"
PKG=com.bwport.app
D=/sdcard/Android/data/$PKG/files
# --game b2|w2 (Black 2, White 2): its ROM is pushed as the drop-in "Pokemon Black 2.nds" and BWPORT_GAME picks it; a save goes into
# the game's folder (made by the first run's import: run once, then again with --save)
case $GAME in
    black) ROMFILE=baserom_b_us.nds; FOLDER="Pokemon Black"; CODE= ;;
    b2) ROMFILE=baserom_b2_us.nds; FOLDER="Pokemon Black 2"; CODE=IREO ;;
    w2) ROMFILE=baserom_w2_us.nds; FOLDER="Pokemon White 2"; CODE=IRDO ;;
    *) echo "--game black|b2|w2"; exit 1 ;;
esac
ROM=${BWPORT_ROM:-$ROOT/orig/$ROMFILE}; [ -f "$ROM" ] || ROM=/root/bw-port/orig/$ROMFILE
mkdir -p "$OUT"
rm -f "$OUT"/s*.png "$OUT/log.txt" "$OUT/logcat.txt"

$ADB get-state > /dev/null 2>&1 || { echo "no device $SERIAL: tools/android_emu.sh run + wait first"; exit 1; }
$ADB shell settings put secure immersive_mode_confirmations confirmed > /dev/null 2>&1 || true
if ! R=$($ADB install -r "$APK" 2>&1); then
    # signed with another key (a debug build, then a release one): in the emulator the app and its files may go
    case "$R/$SERIAL" in
    *UPDATE_INCOMPATIBLE*/emulator-*) $ADB uninstall $PKG > /dev/null; $ADB install "$APK" > /dev/null ;;
    *) echo "install failed: $R"; exit 1 ;;
    esac
fi
$ADB shell am force-stop $PKG
if [ $FRESH = 1 ]; then $ADB shell rm -rf "$D"; fi
$ADB shell mkdir -p "$D"
if [ -n "$CODE" ]; then
    if ! $ADB shell ls "$D/$FOLDER" 2> /dev/null | grep -q '^header.bin$'; then
        [ -f "$ROM" ] || { echo "no ROM at $ROM (BWPORT_ROM)"; exit 1; }
        $ADB push "$ROM" "$D/$FOLDER.nds" > /dev/null
    fi
    ENVS="BWPORT_GAME=$CODE $ENVS"
elif ! $ADB shell ls "$D" | grep -q '\.nds$\|^header.bin$'; then
    [ -f "$ROM" ] || { echo "no ROM at $ROM (BWPORT_ROM)"; exit 1; }
    $ADB push "$ROM" "$D/Pokemon Black.nds" > /dev/null
fi
if [ -n "$SAVE" ]; then
    [ -f "/root/saves/$SAVE.bin" ] || { echo "no save /root/saves/$SAVE.bin"; exit 1; }
    SD=$D; [ -n "$CODE" ] && SD="$D/$FOLDER"
    $ADB push "/root/saves/$SAVE.bin" "$SD/save.bin" > /dev/null
    $ADB shell chmod 666 "\"$SD/save.bin\"" > /dev/null 2>&1 || true   # (pushed as the shell user, 0644: the game could not write it, and showed its save error)
    if [ -n "$CODE" ]; then ENVS="BWPORT_SKIPMENU=1 $ENVS"; else KEYS="${BWPORT_CONTINUE_KEYS:-start@f600 start@f750 start@f900 a@f1000} $KEYS"; fi
fi
EXTRAS=()
[ -n "$KEYS" ] && EXTRAS+=(--es BWPORT_KEYS "'$KEYS'")
[ -n "$RENDERER" ] && EXTRAS+=(--es BWPORT_RENDERER "$RENDERER")
for kv in $ENVS; do EXTRAS+=(--es "${kv%%=*}" "'${kv#*=}'"); done

# the timeline: screenshots and taps at wall seconds, then the end
EV=$(mktemp)
for s in ${SHOTS//,/ }; do echo "$s shot" >> "$EV"; done
for t in $TAPS; do echo "${t#*@} tap ${t%@*}" >> "$EV"; done
echo "$SECONDS_RUN end" >> "$EV"
$ADB logcat -c
T0=$(date +%s.%N)
$ADB shell am start -n $PKG/.BwportActivity "${EXTRAS[@]}" > /dev/null
sort -n "$EV" | while read -r at what arg; do
    now=$(date +%s.%N)
    wait=$(awk -v a="$T0" -v b="$at" -v c="$now" 'BEGIN { print a + b - c }')
    case $wait in -*) ;; *) sleep "$wait" ;; esac
    case $what in
    shot) $ADB exec-out screencap -p > "$OUT/$(printf 's%03d' "${at%.*}").png" < /dev/null ;;
    tap) case $arg in *,*,*) a=(${arg//,/ }); $ADB shell input swipe ${a[0]} ${a[1]} ${a[0]} ${a[1]} ${a[2]} < /dev/null ;;
              *) $ADB shell input tap ${arg//,/ } < /dev/null ;; esac ;;
    end) ;;
    esac
done
rm -f "$EV"
$ADB shell cat "$D/log.txt" > "$OUT/log.txt" 2> /dev/null || true
$ADB logcat -d > "$OUT/logcat.txt"
$ADB shell am force-stop $PKG
grep -m3 "FATAL\|Fatal signal\|AndroidRuntime: \|backtrace:" "$OUT/logcat.txt" || true
echo "$OUT: $(ls "$OUT" | tr '\n' ' ')"
