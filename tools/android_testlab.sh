#!/bin/bash
# tools/android_testlab.sh: the APK on real phones in Firebase Test Lab (Google's device farm), Renderer=gpu, no phone of our own
# (docs/gpu-compat.md, "Real phones: Firebase Test Lab"). NOT RUN BY AGENTS: only the maintainer starts it.
#
#   tools/android_testlab.sh models                          the physical devices Test Lab has now (GPU column: look the SoC up)
#   tools/android_testlab.sh [options] run                   one game loop per device, then the results downloaded
#   tools/android_testlab.sh --dry-run run                   prints the gcloud command, uploads nothing
#     --devices "model=oriole,version=33 model=..."          the devices (default: $DEVICES below; check them with `models`)
#     --renderer gpu|vcore|auto (default gpu)   --resolution N (default 1; runs one matrix per value: --resolution "1 3")
#     --seconds S (default 120: the game ends itself then)   --keys "start@f600 ..." (BWPORT_KEYS; default none: the title
#     screen's 3D Zekrom from ~10 s, then the attract loop)   --boygirl: the keys to the intro's boy/girl question (~f4300, the
#     round-86 Pixel 9 report: both characters should show), 150 s
#     --apk build/android/bwport.apk   --rom orig/baserom_b_us.nds   --project ID (default: gcloud's)   --out DIR
#
# WHAT IT UPLOADS. The APK holds the game's compiled code and bytes from the user's ROM, and the game needs the ROM file itself,
# so this pushes the ROM to each device too (--other-files). Running it puts both on Google's servers: the files go to a
# Cloud Storage bucket of the maintainer's Firebase project (private to that project; Test Lab deletes its devices after each run,
# the bucket keeps the uploads and results until deleted: `gcloud storage rm -r gs://BUCKET/DIR`). It refuses to run
# until BWPORT_TESTLAB_UPLOAD=yes is set, so the decision is the maintainer's each time.
#
# Setup, once (the maintainer, in a terminal: `! cmd` in the agent's prompt runs it in the maintainer's own session):
#   1. a Google account and a Firebase project (console.firebase.google.com, Add project; the free Spark plan has a daily quota
#      of physical-device minutes; the Blaze plan bills per device-minute past it)
#   2. the Google Cloud CLI (cloud.google.com/sdk/docs/install), then `gcloud auth login` and
#      `gcloud config set project PROJECT_ID`; `gcloud services enable testing.googleapis.com toolresults.googleapis.com`
#   3. tools/android_package.sh --build (the APK: arm64-v8a is what the phones run)
#
# What each run does: Test Lab installs the APK, pushes the ROM and testlab.env into the app's files
# (/sdcard/Android/data/com.bwport.app/files), starts BwportActivity with the game-loop intent (BwportActivity.testLoop reads
# testlab.env: BWPORT_RENDERER, BWPORT_RESOLUTION, BWPORT_KEYS, and ends the game after BWPORT_TESTLAB_SECONDS), records a video
# and logcat, and pulls files/testlab/log.txt. Results land in OUT/<device>/ (logcat, video.mp4, log.txt); the lines to read:
#   bwport: device: ...            the phone, Android, SoC
#   bwport: GL: system: ...         the GPU and driver (GL_RENDERER, GL_VERSION)
#   bwport: Renderer=gpu: ...       the GPU path's limits, or why it fell back (a shader's compile log is there in full)
#   bwport: renderer: gpu Nx ...    what drew; "renderer: vcore" after Renderer=gpu means the GPU path failed
#   Fast 3D / SMAA / AA lines       what the driver answered for those
set -eu
ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$ROOT"
# Mali (Pixel 6: Tensor G1 Mali-G78; Pixel 8: G3 Mali-G715; Pixel 9: G4 Mali-G715) and Adreno (Galaxy S23: Snapdragon 8 Gen 2
# Adreno 740; Pixel 5: Snapdragon 765G Adreno 620) phones; Test Lab's list changes: `tools/android_testlab.sh models`
DEVICES=${BWPORT_TESTLAB_DEVICES:-"model=oriole,version=33 model=shiba,version=34 model=tokay,version=34 model=dm1q,version=34 model=redfin,version=30"}
RENDERER=gpu RES=1 SECONDS_RUN=120 KEYS="" APK=build/android/bwport.apk ROM=orig/baserom_b_us.nds
PROJECT="" OUT=build/android/testlab DRY=0
while [ $# -gt 0 ]; do
  case $1 in
  --devices) DEVICES=$2; shift 2 ;;
  --renderer) RENDERER=$2; shift 2 ;;
  --resolution) RES=$2; shift 2 ;;
  --seconds) SECONDS_RUN=$2; shift 2 ;;
  --keys) KEYS=$2; shift 2 ;;
  --boygirl) KEYS="start@f600 start@f900 start@f1300 a@f1700$(for f in $(seq 2200 90 4300); do printf ' a@f%d' $f; done)"
             SECONDS_RUN=150; shift ;;
  --apk) APK=$2; shift 2 ;;
  --rom) ROM=$2; shift 2 ;;
  --project) PROJECT=$2; shift 2 ;;
  --out) OUT=$2; shift 2 ;;
  --dry-run) DRY=1; shift ;;
  models) exec gcloud firebase test android models list --filter="form=PHYSICAL" \
            --format="table(id,brand,name,supportedVersionIds.list(),formFactor)" ;;
  run) shift; break ;;
  *) sed -n 2,9p "$0"; exit 2 ;;
  esac
done
GC=(gcloud)
[ $DRY = 1 ] && GC=(echo gcloud)
if [ $DRY = 0 ]; then
  command -v gcloud > /dev/null || { echo "no gcloud: install the Google Cloud CLI (the header's setup)"; exit 1; }
  if [ "${BWPORT_TESTLAB_UPLOAD:-}" != yes ]; then
    echo "This uploads $APK (code and bytes from your ROM) and $ROM (the ROM itself) to Google's servers, into your Firebase"
    echo "project's storage bucket (private to the project). Set BWPORT_TESTLAB_UPLOAD=yes to go ahead; --dry-run shows the command."
    exit 1
  fi
fi
[ -f "$APK" ] || { echo "no $APK: tools/android_package.sh --build"; exit 1; }
[ -f "$ROM" ] || { echo "no ROM at $ROM (--rom)"; exit 1; }
D=/sdcard/Android/data/com.bwport.app/files
mkdir -p "$OUT"
for res in $RES; do
  ENV=$OUT/testlab-$RENDERER-${res}x.env
  printf 'BWPORT_RENDERER=%s\nBWPORT_RESOLUTION=%s\nBWPORT_KEYS=%s\nBWPORT_TESTLAB_SECONDS=%s\n' "$RENDERER" "$res" "$KEYS" \
    "$SECONDS_RUN" > "$ENV"
  DIR=bwport-$(git rev-parse --short HEAD)-$RENDERER-${res}x-$(date +%Y%m%d-%H%M%S)
  DEV=()
  for d in $DEVICES; do DEV+=(--device "$d,locale=en,orientation=portrait"); done
  echo "== $DIR: Renderer=$RENDERER Resolution=$res on: $DEVICES"
  "${GC[@]}" firebase test android run --type game-loop --app "$APK" --scenario-numbers 1 \
    "${DEV[@]}" --timeout "$((SECONDS_RUN + 120))s" \
    --other-files "$D/rom.nds=$ROM,$D/testlab.env=$ENV" \
    --directories-to-pull "$D/testlab" \
    --results-dir "$DIR" ${PROJECT:+--project "$PROJECT"} 2>&1 | tee "$OUT/$DIR.log" || true
  [ $DRY = 1 ] && continue
  # the results: the bucket Test Lab named ("Raw results will be stored in your GCS bucket at [.../storage/browser/BUCKET/DIR/]")
  B=$(grep -o 'storage/browser/[^/]*' "$OUT/$DIR.log" | head -1 | sed 's#storage/browser/##' || true)
  if [ -z "$B" ]; then
    echo "no results bucket in $OUT/$DIR.log: see the console link above; then gcloud storage cp -r gs://BUCKET/$DIR $OUT/"
    continue
  fi
  gcloud storage cp -r "gs://$B/$DIR" "$OUT/" > /dev/null
  for f in $(find "$OUT/$DIR" -name 'log.txt' -o -name 'logcat'); do
    echo "-- $f"
    grep -a -E "bwport: (device|GL: system|Renderer=|renderer: )|Fast 3D|SMAA|samples|FATAL|Fatal signal" "$f" | head -12 || true
  done
done
