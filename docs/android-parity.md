# Android against iOS: the parity list

Owner 11-05: Android reaches the iOS build's feature level. This file is the audit of what iOS has (port/src/ios_settings.m, ios_main.c,
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
