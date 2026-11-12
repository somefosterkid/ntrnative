#!/bin/bash
# The Android emulator, headless (the maintainers' notes, "Android"). The AVD is tools/android_deps.sh's (bwport34: Pixel 7 size,
# Android 14, Google APIs x86_64; /dev/kvm makes it fast).
#   tools/android_emu.sh run [PORT]    runs the emulator in the FOREGROUND (start it in the background yourself: the Bash tool's
#                                      run_in_background; `nohup ... &` inside ~/bw dies with the call). Its PID goes to
#                                      /tmp/bwport-emu-PORT.pid, its output to /tmp/bwport-emu-PORT.log.
#   tools/android_emu.sh wait [PORT]   waits until it has booted (prints the adb serial, emulator-PORT)
#   tools/android_emu.sh stop [PORT]   kills it by its PID
# PORT: an even number 5554..5584 (default 5554); -read-only lets several run the same AVD at once, and every boot is a fresh
# device (nothing an earlier run installed or wrote is there). NEVER without -no-window: WSLg would put the window on the maintainer's
# desktop. Sound: -audio none (the guest keeps its audio device, so the game's sound path runs; nothing reaches the host's speakers).
# GL: swiftshader_indirect (software GLES 3 in the emulator process; the game's Renderer=gpu works on it, slowly).
set -eu
A=${ANDROID_ROOT:-/opt/android}
CMD=${1:-}
PORT=${2:-5554}
PIDF=/tmp/bwport-emu-$PORT.pid
S=emulator-$PORT
ADB=$A/sdk/platform-tools/adb
export ANDROID_HOME=$A/sdk ANDROID_SDK_ROOT=$A/sdk ANDROID_AVD_HOME=$A/avd
unset DISPLAY WAYLAND_DISPLAY PULSE_SERVER
alive() { [ -f "$PIDF" ] && kill -0 "$(cat "$PIDF")" 2> /dev/null; }
case $CMD in
run)
    alive && { echo "$S runs already (pid $(cat "$PIDF"))"; exit 1; }
    echo $$ > "$PIDF"
    exec "$A/sdk/emulator/emulator" -avd bwport34 -port "$PORT" -no-window -gpu swiftshader_indirect -audio none -read-only \
        -no-snapshot -no-boot-anim -no-metrics > "/tmp/bwport-emu-$PORT.log" 2>&1 < /dev/null
    ;;
wait)
    for i in $(seq 1 30); do alive && break; sleep 1; done
    alive || { echo "no emulator on $PORT"; exit 1; }
    "$ADB" start-server > /dev/null 2>&1 || true
    t=0
    until [ "$("$ADB" -s "$S" shell getprop sys.boot_completed 2> /dev/null | tr -d '\r')" = 1 ]; do
        alive || { echo "the emulator ended:"; tail -20 "/tmp/bwport-emu-$PORT.log"; exit 1; }
        t=$((t + 2)); [ $t -lt 600 ] || { echo "no boot after 600 s"; exit 1; }
        sleep 2
    done
    "$ADB" -s "$S" shell settings put global window_animation_scale 0 > /dev/null 2>&1 || true
    "$ADB" -s "$S" shell settings put global transition_animation_scale 0 > /dev/null 2>&1 || true
    echo "$S booted (${t} s)"
    ;;
stop)
    if alive; then kill "$(cat "$PIDF")"; sleep 3; alive && kill -9 "$(cat "$PIDF")"; fi
    rm -f "$PIDF"
    echo "$S stopped"
    ;;
*)
    echo "usage: $0 run|wait|stop [PORT]"; exit 2 ;;
esac
