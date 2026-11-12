# GPU compatibility: the GPU renderer's shaders on phone drivers

The GPU renderer (port/gpu, docs/gpu.md) is GLSL ES 3.00 on Android and iOS-GL, GLSL 3.30 / 4.50 on desktop GL, and MSL made
from the same GLSL on Metal. Android phones run the vendor's own compiler: Qualcomm's for Adreno (Snapdragon), Arm's for Mali
(Pixel 6-9 Tensor, MediaTek Dimensity, older Exynos), and on some newer Android builds ANGLE on Vulkan. These compilers are
stricter than desktop GL and Apple's in places, so the check runs on this machine, without a GPU or a phone.

## What is checked

`python3 port/gpu/tools/gpu_glsl_check.py` (the meson test `glsl strict` in build/gpu; ~30 s). build/gpu/gpu_glsl writes every
program the cores can build: 24 ES and 32 GL, taken from the C sources' own list. Every `#K`, `#Kf`, `+DEF`, the
transform-feedback and fragment forms of pass S, the fast 3D renderer's modes. The text is the same assembly src/gpu_gl.c's
build_program compiles (gpu_gl_program_text). The checker then runs these:

| check | what | from |
|---|---|---|
| names | every shader name in port/gpu/src/*.c is in gpu_glsl's list | |
| lint | ES text: `precision highp` float/int/samplers before any code, no mediump/lowp, every opaque type highp | |
| glslang | Khronos' reference front end, each shader and each pair linked (ES and desktop) | glslang 16.6.0, tools/gpu_shader_deps.sh |
| mesa | the headless context's compiler (llvmpipe, GLES 3.2) | the system |
| angle | the Android emulator's ANGLE (GLES 3.0): Chrome's front end, and Android's system GLES on some builds | /opt/android (tools/android_deps.sh) |
| swiftshader | the emulator's SwiftShader GLES 3.0 (what `-gpu swiftshader_indirect` runs) | /opt/android |
| mali-G52 ... G720 | Arm's Mali Offline Compiler (driver r56p1): Mali-G52, G57 (cheap phones, Bifrost / early Valhall), G78 (Pixel 6), G710 (Pixel 7), G715 (Pixel 8, 9), G720 (Dimensity 9300) | Arm Performance Studio 2026.6, `tools/gpu_shader_deps.sh --mali` |
| adreno | Qualcomm's offline compiler, when `ADRENO_CMD` is set | not installed: needs a Qualcomm ID |

ANGLE and SwiftShader are loaded by LD_LIBRARY_PATH into gpu_glsl's EGL context, so `gpu_render` runs whole frames through them
the same way (`LD_LIBRARY_PATH=<links to gles_angle/libEGL.so as libEGL.so.1> build/gpu/gpu_render F.vcs out.png --compose gpu`).
That is how the round-86 emulator failure was reproduced here: the same message, on the CPU. With `-v`, malioc's line per program
gives the work registers, the occupancy, any spilling and the cycles.

Results:

| | lint | glslang | mesa | angle | swiftshader | Mali G52 G57 G78 G710 G715 G720 |
|---|---|---|---|---|---|---|
| before (2e210bce4) | 24 ok | 20 ok, 4 skip | 24 ok | 19 ok, C_PLAN FAIL, 4 skip | 19 ok, C_PLAN FAIL, 4 skip | 24 ok on all six |
| after | 24 ok | 20 ok, 4 skip | 24 ok | 20 ok, 4 skip | 20 ok, 4 skip | 24 ok on all six |

The skips are the fast 3D renderer's framebuffer-fetch programs. glslang has no EXT_shader_framebuffer_fetch, and the
emulator's GLES does not offer it. Mesa and malioc compile them.

## Rules for the shaders (what the strict compilers taught)

- No `?:` whose operands are a struct or an array: use if/else. GLSL ES 3.00 leaves it to the compiler, and ANGLE and SwiftShader
  refuse it ("ternary operator is not allowed for structures or arrays"). Desktop GL, Apple's GL, Mesa, glslang and Mali accept it.
- Write a full 32-bit mask as `~0u`, not `0xFFFFFFFFu`. That literal is valid, but SwiftShader warns "Integer overflow".
- Precision: the ES header (src/gpu_gl.c) declares highp for float, int, sampler2D, isampler2D and usampler2D. Any other opaque
  type is declared highp where it is used. Nothing is mediump. Mali and Adreno run mediump as fp16 / 16-bit integers, and the
  cores need 32 bits: 24-bit depth, the packed pixel state, texture coordinates and positions at 8x. ES 3.0 requires highp in
  fragment shaders, so every phone has it.
- Extensions are switched on only where the C side checked for them: the fast 3D renderer asks for
  EXT_shader_framebuffer_fetch on ES (src/g10_fast_gl.c) and otherwise reports why it cannot draw.

A new shader or a new variant needs one line in port/gpu/tools/gpu_glsl.c's list, or the name check fails. After a GLSL change,
`python3 port/gpu/tools/gpu_msl.py gen /opt/gpu-tools/glslang/bin/glslang /opt/gpu-tools/spirv-cross/build/spirv-cross`
regenerates port/gpu/msl on Linux too. Unchanged GLSL gives byte-identical files.

## What only a phone can show

A shader that compiles can still behave differently on a phone. That is runtime behaviour, and no offline compiler shows it:

- Float rounding: pass S builds the span records in float-float arithmetic (two floats per value). A driver that reassociates or
  fuses differently changes the low halves. SwiftShader already does: the self-check logged "0 integer fields differ, 875
  attribute lo beyond an ulp" at the intro. The low halves only refine the interpolation. A Mali/Adreno log line like that, with
  integer fields differing, would be a real fault.
- Performance: the raster loop uses all 64 work registers at 50 % occupancy on every Mali target, and spills 4-10 bytes on
  Valhall and 180 bytes on Bifrost (G52). It compiles, but cheap Bifrost phones may be slow on Renderer=gpu.
- Driver bugs and limits: integer render targets (RGBA32UI / RGBA8UI), texelFetch from R32UI / RGBA32F, transform feedback with 48
  components, 4 draw buffers, framebuffer fetch (Fast 3D), multisampling (4 samples) and SMAA.
- The round-86 Pixel 9 report (the boy/girl screen without the two characters): still open. All of the programs compile for
  Mali-G715 (malioc, driver r56p1; the program that ANGLE refused also compiled there before the fix), so the Pixel ran the
  GPU path. The fault is at runtime, or in arm64 code. the maintainers' notes "BOY/GIRL SCREEN" lists what to ask a tester for.

## Real phones: Firebase Test Lab

`tools/android_testlab.sh` runs the APK on Test Lab's physical phones as a game loop. BwportActivity takes the TEST_LOOP intent,
reads testlab.env (Renderer, Resolution, keys) and ends itself. Test Lab returns logcat, a video and files/testlab/log.txt. The
default devices are Pixel 6 (Mali-G78), Pixel 8 and Pixel 9 (Mali-G715), Galaxy S23 (Adreno 740) and Pixel 5 (Adreno 620). Check
them with `tools/android_testlab.sh models`, since Test Lab's list changes. `--resolution "1 3"` runs both scales, and
`--boygirl` takes the keys to the intro's boy/girl question.

- It uploads the APK (game code and bytes from the ROM) and the ROM itself to the maintainer's Firebase project's Cloud Storage
  bucket. The bucket is private to the project, and the files stay there until they are deleted. The script refuses to run
  without `BWPORT_TESTLAB_UPLOAD=yes`. `--dry-run` prints the gcloud command and uploads nothing.
- Setup (the maintainer, once): a Google account and a Firebase project; the Google Cloud CLI; `gcloud auth login`,
  `gcloud config set project ID`, `gcloud services enable testing.googleapis.com toolresults.googleapis.com`. The free plan has a
  daily quota of physical-device minutes.
- Checked in the emulator only: `adb shell am start -a com.google.intent.action.TEST_LOOP -t application/javascript -n
  com.bwport.app/.BwportActivity` read testlab.env, drew with gpu 2x, ended at the set time and wrote files/testlab/log.txt.
  No Test Lab run has been made.

Qualcomm's Adreno offline compiler (Qualcomm Software Center, "Adreno GPU Offline Compiler") needs a Qualcomm ID login, so
it is not installed here. With it, `ADRENO_CMD="<compiler> ... {file}"` (`{stage}` is vert or frag) adds an adreno column.
