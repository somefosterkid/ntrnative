# port/gpu: the DS 3D at N x on the GPU (docs/gpu.md). Its own meson project, apart from the port's build:
#   meson setup build/gpu port/gpu && ninja -C build/gpu
#   build/gpu/gpu_info                              the headless GL context (EGL, Mesa surfaceless)
#   build/gpu/gpu_render FILE.vcs OUT.png [--3d L.png] [--scale N] [--hi H.png]
# vcore's sources (port/vcore, the reference and the CPU half: geometry, spans, 2D) are compiled in from ../vcore; the
# sources are every src/*.c here (vcore's list_sources.py), the shaders every shaders/*.glsl|vert|frag, embedded into a
# C table at build time (tools/embed_glsl.py). `meson setup --reconfigure build/gpu port/gpu` after adding a file.
project('gpu', 'c', default_options: ['c_std=c11', 'warning_level=2', 'buildtype=debugoptimized'])

cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments(['-Wno-unused-parameter', '-Wno-unused-function']), language: 'c')

python = find_program('python3')
fs = import('fs')
vcore_dir = fs.parent(meson.current_source_dir()) / 'vcore'
list = vcore_dir / 'list_sources.py'
vsrcs = run_command(python, list, vcore_dir / 'src', check: true).stdout().strip().split('\n')
gsrcs = run_command(python, list, meson.current_source_dir() / 'src', check: true).stdout().strip().split('\n')

embed = files('tools/embed_glsl.py')
glsl = run_command(python, embed, '--list', meson.current_source_dir() / 'shaders', check: true).stdout().strip().split('\n')
shaders_c = custom_target('gpu_shaders',
  input: glsl,
  output: 'gpu_shaders.c',
  command: [python, embed, '@OUTPUT@', '@INPUT@'])

m = cc.find_library('m', required: false)
thr = dependency('threads')
# the context: EGL (Mesa surfaceless, device, default display) and/or an SDL hidden window; the GL functions are loaded
# by name, so libgpu itself links neither GLESv2 nor GL. A host with neither (the mingw check) still compiles.
egl = dependency('egl', required: false)
sdl = dependency('sdl2', required: false)
gpu_defs = []
if egl.found()
  gpu_defs += '-DGPU_HAVE_EGL'
endif
if sdl.found()
  gpu_defs += '-DGPU_HAVE_SDL'
endif
# macOS / iOS: the Metal core (src/gpu_mtl.m, gpu_create(GPU_API_METAL)) and its MSL, generated from the GLSL into msl/ and
# committed (tools/gpu_msl.py gen); the build only embeds it, and fails when a GLSL input changed since (the command is printed)
metal = host_machine.system() == 'darwin' and add_languages('objc', required: false, native: false)
mtl_srcs = []
mtl_deps = []
if metal
  gpu_defs += '-DGPU_HAVE_METAL'
  msl = run_command(python, files('tools/gpu_msl.py'), '--list', check: true).stdout().strip().split('\n')
  mtl_srcs = ['src/gpu_mtl.m', custom_target('gpu_msl',
    input: msl + glsl + ['tools/gpu_msl.py', 'src/gpu_uni.h'],
    output: 'gpu_msl.c',
    command: [python, files('tools/gpu_msl.py'), 'embed', '@OUTPUT@'])]
  mtl_deps = [dependency('appleframeworks', modules: ['Metal', 'Foundation'])]
  # iOS (the Simulator): the tools' headless context is EAGL's GLES (src/gpu_eagl.m; run them with xcrun simctl spawn)
  if cc.get_define('TARGET_OS_IOS', prefix: '#include <TargetConditionals.h>') == '1'
    gpu_defs += '-DGPU_HAVE_EAGL'
    mtl_srcs += 'src/gpu_eagl.m'
    mtl_deps += dependency('appleframeworks', modules: ['OpenGLES'])
  endif
  add_project_arguments(gpu_defs, language: 'objc')
  add_project_arguments('-fobjc-arc', language: 'objc')
endif
add_project_arguments(gpu_defs, language: 'c')

vinc = include_directories(vcore_dir / 'include', vcore_dir / 'src')
ginc = include_directories('include', 'src')
libvcore = static_library('vcore', vsrcs, include_directories: vinc, dependencies: [m, thr],
  override_options: ['warning_level=0', 'optimization=2'])
libgpu = static_library('gpu', gsrcs, shaders_c, mtl_srcs, include_directories: [ginc, vinc], dependencies: [m, thr, egl, sdl] + mtl_deps)
gpu_dep = declare_dependency(link_with: [libgpu, libvcore], include_directories: [ginc, vinc],
  dependencies: [m, thr, egl, sdl] + mtl_deps)

executable('gpu_render', 'tools/gpu_render.c', dependencies: gpu_dep)
executable('gpu_info', 'tools/gpu_info.c', dependencies: gpu_dep)
# every program the GL cores build, as text, through strict GLSL ES compilers (glslang, Mesa, the Android emulator's ANGLE and
# SwiftShader, Arm's malioc when installed): tools/gpu_glsl_check.py, docs/gpu-compat.md
gpu_glsl = executable('gpu_glsl', 'tools/gpu_glsl.c', dependencies: gpu_dep)
test('glsl strict', python, args: [files('tools/gpu_glsl_check.py'), '--gpu-glsl', gpu_glsl], timeout: 300)
executable('gpu_cover', 'tools/gpu_cover.c', dependencies: gpu_dep)
gpu_tests = executable('gpu_tests', 'tests/t02_spans.c', dependencies: gpu_dep)
test('g02 spans', gpu_tests)
executable('gpu_crack', 'tools/gpu_crack.c', dependencies: gpu_dep)
executable('gpu_pxdiff', 'tools/gpu_pxdiff.c', dependencies: gpu_dep)
executable('gpu_fastdiff', 'tools/gpu_fastdiff.c', dependencies: gpu_dep)
