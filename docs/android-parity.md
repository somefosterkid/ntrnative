# Android against iOS: the parity list

The goal (11-05): Android reaches the iOS build's feature level. This file is the audit of what iOS has (port/src/ios_settings.m, ios_main.c,
ios_input.m, ios_safearea.m, ios_log.c, apple_thermal.m, the libntr-ios-*.patch chain) against what Android has, with the state after
and a ranked list of what is left. The launcher (LauncherScreen) is al90's, the in-process restart wr90's/machine.c's: not here.
Only the headless x86_64 emulator was available (no phone): what it cannot show is said in each row.

## Settings screen (tools/android/.../SettingsScreen.java against ios_settings.m)

Rows, keys, names, choices, order and greying rules were already the same as iOS's at the start of the round (Display: Renderer,
Resolution, Antialiasing, 3D renderer, Smooth motion; Look at 2x and above; Speed: Fast-forward speed, Battle speed, Text speed,
Transitions, Walk / Run / Surf / Save speed, Skip frames when slow, Music at normal speed; Sound; Controller; Performance overlay; About).
The Java compiles (Gradle, both ABIs) and the libntr chain compiles for arm64-v8a and x86_64 (the note "never compiled" in
the follow-up is closed). changes: the 3D renderer row's two subtitles and the performance test's alert text as on iOS.
Not on Android, on purpose: the Graphics API row (iOS: OpenGL ES or Metal; Android has ES only), Games (al90's launcher).

## Platform layer

| iOS | Android after |
|---|---|
| Log per run in Documents/logs, last 10 kept, 2 MB cap, head lines (build, device) | log.txt is this run's; the last run's moves to logs/bwport-<time>.log, 10 kept, 2 MB cap with the head written again (port/src/android_log.c, android_main.c); "bwport: app build 0.1-<hash> (<versionCode>)" first line |
| PORT_LogSettings: sim_config.ini [General] + BWPORT_ variables in the log | the same (android_log.c; ios_log.c's empty stub excludes Android) |
| Share latest log (UIActivityViewController) | the share sheet with log.txt and the newest earlier log (LogShare.java: a ContentProvider, no androidx; manifest provider) |
| Thermal state and Low Power Mode logged at start and each change; `thermal` in the sys: line | PowerManager thermal status (API 29+) and Power Saver, same log lines; `thermal none|light|...` in the sys: line (Platform.java, android_log.c, one branch in sysmetrics.c) |
| Panel rate: 120 only while Smooth motion is on, 60 otherwise, 30-60 with the settings sheet up (CADisplayLink preferredFrameRateRange) | Surface.setFrameRate on the game's surface, same three modes, from a Choreographer callback (Platform.java; API 30+; the log says `display: asking the panel for N Hz`). Untested on a high-refresh panel: the emulator's is 60 Hz, so only the vote is seen |
| Safe area | done in (display cutout + navigation bar) |
| Lifecycle (SDL app events, nothing held across the background) | shared code (libntr-ios-appfix.patch's Watch, SimTouchBackgroundWait); see the emulator check below |
| Backup of save.bin + sim_config.ini only | Auto Backup rules (res/xml/backup_rules.xml, extraction_rules.xml): save.bin, sim_config.ini, the launcher's `Pokemon Black/` copies and launcher.ini; never the extracted ROM |
| (packaging) | tools/android_package.sh now ships libc++_shared.so: libmain.so links it (meson's link line has -lc++ despite -static-libstdc++) and main's APK died at System.loadLibrary without it |
| (test tool) | tools/android_run.sh chmods the pushed save 666: pushed as the shell user (0644) the app could not write it and the game sat in its save-error screen, black |

## Checked in the emulator (x86_64, headless)

- Every Settings row through the dialog (uiautomator taps): Renderer, Resolution (its pick list carries the "512 x 768" subtitles), Smooth motion,
  Fast-forward / Battle / Text / Transitions / Walk / Run / Surf / Save speed, Skip frames when slow, Music at normal speed, Sound quality, Hide
  touch controls, Overlay: sim_config.ini afterwards held Renderer=vcore Resolution=2 SmoothMotion=on FastForwardSpeed=3 BattleSpeed=2
  TextSpeed=instant FadeSpeed=instant WalkSpeed/RunSpeed/SurfSpeed/SaveSpeed=4 FrameSkip=auto MusicNormalSpeed=off SoundQuality=ds
  Overlay=off HideTouchWithController=off (from the other values of the run before: screenshots as90_05, as90_06).
- Share latest log: the system share sheet lists "bwport-current.log + 1 more file" (as90_07); logs/ holds the earlier runs.
- HOME and back: `app event will enter background ... did enter foreground` in the log, the game continues (as90_08, as90_09).
- The log's head: app build, device, thermal none, low power mode off, the settings in effect; `sys:` lines end in `thermal none`.
- Panel votes: `display: asking the panel for 60 Hz (60 fps game)`, 30 Hz while the settings are open, 60 again after.
- Auto Backup: the manifest takes the rules and `dumpsys package` shows ALLOW_BACKUP, but `bmgr backupnow` on the emulator's local transport
  answers "Backup is not allowed" for the package (the cause was not found; no file reached the transport): UNTESTED, check on a phone
  (`adb shell bmgr backupnow com.bwport.app`, then a reinstall with the cloud backup on).

## Generation IV on Android (and5, 11-17)

Platinum, HeartGold and SoulSilver are in the APK by default, as on the PC, with no ROM at build time: the meson options
`platinum` and `hgss` were never off on Android (only `is_mac` turns them off), the generators (tools/pokeplat_gen.py,
tools/pokehg_gen.py) run on the build machine as every meson `run_command` does, and the NDK's lld takes the same overlay
script as the PC's (tools/port_overlays.py --target android = linux). The README's "PC only" was the state of the tests, not of the
build. The APK with both ABIs is 134 MB (11-14, before Generation IV and the ROM fill: 95 MB); libmain.so (stripped) is 62 MB for
x86_64 and 66 MB for arm64-v8a (43 and 46 MB before).
What did stop them (all found by running them in the emulator):
- The start-up ROM fill (port/src/asset_load.c, tools/port_romfill.py) makes the read-only groups writable and then read-only
  again. The NDK's lld puts .rodata, the `.bwrom_*` groups and .text in one R E segment, and SELinux's `execmod` denies making a
  page of the APK's mapping executable after it was written: "rom fill: can't restore the protection" in every game's log (Black's
  too), the data left writable and the fill reported as failed. `-Wl,--rosegment` (port/meson.build, is_android) gives the read-only
  data a segment of its own: the message is gone from all three games' logs.
- tools/android_run.sh knew only Black, Black 2 and White 2: `--game plat|hg|ss` (the ROM, the app folder, the game code) and, with
  `--save`, the route chain's pins and its START / A keys through the title and the save's summary; `--pilot TOKENS` runs a route
  leg's pilot (tools/routes/*.txt) inside the app.
- tools/pokehg_gen.py failed on its second run (`git rm` over pret's nested clone under subprojects/): the checkout is emptied with
  `git read-tree --empty` now.
The ABI: arm64's AAPCS64 does not widen narrow arguments (as win64's); tools/hg_abi_scan.py finds 2 declarations in HeartGold, both
in tools/hg_abi_exceptions.txt (a constant that the DS passes truncated: right as it is) and 0 in Platinum (build/pokeplatinum and
port/game): nothing to add to the generated zzabi patch. The arm64 build runs in the x86_64 emulator through its ARM translation
(next section: HeartGold's frame trace equal to Linux's); a phone is still the check for speed and the GPU. Before that: the arm64 build compiles, links and
is packaged (a phone is the check left).

Checked in the x86_64 emulator (headless, `--renderer vcore`, speed Max to the field, shots in the worker's folder):
- Platinum from a save (pt-poketch) reaches the field; one touch on the Poketch's red button changes its app (the watch, then the
  calculator, then the counter): the bottom screen's touch works.
- HeartGold from a save (hg-gear) reaches the field with the touch menu (trainer card, Pokemon, Bag, Save, Options); one touch on
  Bag opens the Bag. SoulSilver the same (ss-ecruteak: Bag opened, item list).
- A game's ROM is imported from the drop-in `Pokemon Platinum.nds` / `Pokemon HeartGold.nds` / `Pokemon SoulSilver.nds` in the
  app's files folder on the first start (the launcher's folder layout: `Pokemon Platinum/` with the extracted files and
  save.bin); a run that starts with the files folder in a half-made state (made by `adb shell` before the app ran) can fail the
  import with "The ROM couldn't be written": `tools/android_run.sh --fresh` removes the folder first.
- Route pilot inside the app (`android_run.sh --pilot`, the leg's tokens from tools/routes/hg.txt): HeartGold's `new-look` reaches
  zone 64 6,6 at V-blank 11588, the leg's end, in 13 s of wall time at speed Max. Platinum's `new-look` (route pt) did not match the
  PC's: with the game's own speed options the pilot hangs at V-blank 18667 (zone 65535, the intro) where the PC is in the room at
  19236; with tools/route.py's FAST_OPTS (textspeed, fadespeed ... in BWPORT_CFGSCRIPT) the app exited with status 0 after 13 s with no
  `pilot:` or `pos:` line. Not found yet; the save-start runs above are the proof that the game plays on Android.
- tools/release_check.sh on the APK with both ABIs: before the `hg` group of tools/port_romfill.py (merged from the Mac port's
  work) it REFUSED the APK, 228 KB of ROM bytes in 629 runs (HeartGold 185 KB, Platinum 45 KB, SoulSilver 3 KB, the rest
  matches in Black's and Black 2 / White 2's data); with it, 0 ROM bytes (only the listed public tables), and HeartGold boots and
  takes the touch tap as before (the log: `assets: IPKE: ... the image's data 150527 bytes in 4792 copies`, no fill message).
  APK 129 MB, both ABIs.

## arm64-v8a in the x86_64 emulator (and6, 11-18)

The Google APIs x86_64 image we already use (Android 14, r14) carries ARM translation: its build.prop has
`ro.dalvik.vm.native.bridge=libndk_translation.so` and `ro.product.cpu.abilist=x86_64,arm64-v8a`, so no arm64 system image is
needed. `tools/android_run.sh --abi arm64-v8a` installs the APK with `adb install --abi arm64-v8a` (`dumpsys package` then says
primaryCpuAbi=arm64-v8a; logcat `ndk_translation: Initialized NDK translation (aarch64)`; the game's log `ABI x86_64 (built for
arm64-v8a)`). `--abi x86_64` switches back over the same install.
- What was broken: the translated library has a libc of its own, so nothing that Java's Os.setenv wrote (the start intent's
  BWPORT_* extras, BWPORT_DENSITY ...) reached the game: `BWPORT_RENDERER=(unset)`, no BWPORT_KEYS presses, the title forever. Fixing it
  in SDL_main is too late (libmain's constructors read BWPORT_KEYS and others while the library loads). BwportActivity.loadLibraries
  now copies every BWPORT_* / SDL_* variable through libSDL2's nativeSetenv (libSDL2.so is translated too) just before libmain.so
  loads. Java only; a phone runs arm64 natively and gets the same values twice.
- A process wrapper (`setprop wrap.com.bwport.app:game SCRIPT`, which would give the process its environment at exec) does not work:
  the wrapped app_process starts without the native bridge and refuses the arm64 library ("instead of EM_X86_64").
- Black from the save `town` (CONTINUE keys, vcore, ff=1): the field in Nuvema Town on arm64 as on x86_64 (~30 fps against ~45 under a
  loaded host).
- HeartGold `new-look` by the pilot with route.py's whole environment (POSLOG, MUSICNORMAL=off, BENCH_CLOCK / BENCH_SEED, NOENCOUNTER,
  TESTCHEAT=win, FAST_OPTS + ff=1 in BWPORT_CFGSCRIPT) and `BWPORT_HGTRACE=/sdcard/Android/data/com.bwport.app/files/trace.txt`:
  `pos: HG Z zone 64 6,6 (V-blank 11588)` (and5's x86_64 number), `pilot: script done at V-blank 11664`, 9-12 min of wall time. The trace (11664 lines, one a V-blank) is
  byte for byte the Linux one of the same leg on a clang 21 Linux host (route.py hg new-look; the NDK is clang 21 too): sha1 of the first
  1000 / 5000 / 10000 / 11000 lines and the last line equal. So AAPCS64's unwidened narrow arguments change nothing on this leg, as
  tools/hg_abi_scan.py predicted. The x86_64 APK's trace matched the same lines as far as it ran.
- Remember when comparing: the emulator (and any APK run) is not a clang 18 host: compare HG traces with a clang 21 Linux host,
  as HG/SS frames differ between clang 18 and 21 machines.
- A loaded host makes the emulator's own system process late: its "Process system isn't responding" dialog sits over the
  screenshots (`settings put global hide_error_dialogs 1` does not hide it); the game runs on behind it.
- Speed: under translation the game runs ~1.5-3x slower than x86_64 in the same emulator (Black ~30 against ~45 fps, HeartGold ~16 against ~48 fps at Max); the vcore renderer is the one to use
  (gpu on SwiftShader under translation drew 7 fps).

## Open, ranked (size: S under 2 h, M a day, L more)

1. GLES shader portability: DONE in (ag1; docs/gpu-compat.md). g07_compose.frag#1+C_PLAN used `?:` on a struct, which ANGLE
   and SwiftShader refuse. It is now if/else, and `0xFFFFFFFFu` (a SwiftShader warning) is now `~0u`. The pictures are unchanged
   (pics_ab 672 / 672, 0 differ, plain and with GPU_COMPOSE=gpu; the regenerated MSL changed only in C_PLAN). The round-86
   "g00_raster.frag at 3x" failure did not reproduce: every pass R variant compiles on the emulator's SwiftShader and ANGLE.
   Emulator: Renderer=gpu draws at 1x, 2x and 3x ("renderer: gpu Nx ... on Android Emulator OpenGL ES Translator (Google
   SwiftShader)"), and Renderer=auto picks gpu and stays on it. Gate: `port/gpu/tools/gpu_glsl_check.py` (meson test `glsl strict`)
   checks all 24 ES programs with glslang, Mesa, the emulator's ANGLE and SwiftShader, and Arm's malioc for Mali-G52/G57/G78/G710/
   G715/G720: all pass. Adreno: no offline compiler without a Qualcomm login. A phone's view: tools/android_testlab.sh (Firebase
   Test Lab; the maintainer runs it).
2. Game controllers (S to test, M to fix). SDL's Android joystick path feeds the same pad code as the desktop (default AJoyKey=0 ... in sim_config.ini);
   the Controller > hide touch controls row is there. Not testable in the emulator (no way to attach a pad); needs a phone with a Bluetooth pad:
   check button mapping (SDL maps Android gamepads as joysticks with the same button numbers) and that touch controls hide.
3. System gesture edges (S). iOS's ios_input.m switches off the recognizers that delay touches near the edges; Android's gesture navigation takes
   back swipes from the left/right 20-30 dp where the D-pad and the A/B diamond sit. `View.setSystemGestureExclusionRects` over the controls
   (Java, from the touch layout's rectangles) would stop the back gesture eating a press. Needs the rectangles from sim_touch.cpp (a small libntr
   patch callback) and a gesture-navigation device to see the effect.
4. Fast on GL / framebuffer fetch / SMAA / 4-sample AA on Mali, Adreno (M, a phone). Compile side checked in: the fast 3D
   renderer's framebuffer-fetch programs (g10_fast*.frag +G10_FETCH, 1 and 4 samples, with and without Filter) and the SMAA and
   4-sample programs compile in malioc for all six Mali targets. The emulator offers no EXT_shader_framebuffer_fetch, so Fast stays
   untested there. Still to come from a phone (tools/android_testlab.sh, docs/gpu-compat.md "What only a phone can show"): whether
   the driver offers the extension, whether the pictures are right, the speed (the raster loop is at 50 % occupancy with small
   spills on Valhall, 180 bytes on Bifrost G52), and the Adreno side. The "Fast 3D" footer reads the driver's answer
   (SIM_Renderer3DFastState) and will say why when the GPU cannot.
5. Perf test: its table is in the log (shared); on Android the screen is the same dialog: untested on a phone for thermals.
6. Stuck-input checks (touch ends held by gesture recognizers): UIKit specific; Android sends ACTION_CANCEL to SDL, which libntr treats as a lift.
   No known issue; nothing to port until a phone shows one.
7. Vulkan or a Metal-like second API: not planned.
