# port/vcore: the DS video core (docs/vcore.md). Its own meson project, apart from the port's build:
#   meson setup build/vcore port/vcore && ninja -C build/vcore
#   build/vcore/vcore_tests [pNN ...]            the unit tests
#   build/vcore/vcore_render FILE.vcs OUT.png    one frame
# The sources are every src/*.c and tests/*.c (list_sources.py): a piece adds a file without editing this one, then
# `meson setup --reconfigure build/vcore port/vcore` (meson doesn't watch directories).
project('vcore', 'c', default_options: ['c_std=c11', 'warning_level=2', 'buildtype=debugoptimized'])

cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments(['-Wno-unused-parameter', '-Wno-unused-function']), language: 'c')

python = find_program('python3')
list = files('list_sources.py')
srcs = run_command(python, list, meson.current_source_dir() / 'src', check: true).stdout().strip().split('\n')
tsrcs = run_command(python, list, meson.current_source_dir() / 'tests', check: true).stdout().strip().split('\n')

inc = include_directories('include', 'src', 'tests')
m = cc.find_library('m', required: false)
thr = dependency('threads')
libvcore = static_library('vcore', srcs, include_directories: inc, dependencies: [m, thr])
vcore_dep = declare_dependency(link_with: libvcore, include_directories: inc, dependencies: [m, thr])

vcore_tests = executable('vcore_tests', tsrcs, dependencies: vcore_dep)
executable('vcore_render', 'tools/vcore_render.c', dependencies: vcore_dep)
executable('vcore_interp', 'tools/vcore_interp.c', dependencies: vcore_dep)
test('vcore unit tests', vcore_tests, args: ['-q'])
