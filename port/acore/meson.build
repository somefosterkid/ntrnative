# port/acore: the DS sound core (docs/acore.md). Its own meson project, apart from the port's build:
#   meson setup build/acore port/acore && ninja -C build/acore
#   build/acore/acore_tests [pNN ...]            the unit tests
#   build/acore/acore_render FILE.acs OUT.wav    one stream
# The sources are every src/*.c and tests/*.c (list_sources.py): a piece adds a file without editing this one, then
# `meson setup --reconfigure build/acore port/acore` (meson doesn't watch directories).
project('acore', 'c', default_options: ['c_std=c11', 'warning_level=2', 'buildtype=debugoptimized'])

cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments(['-Wno-unused-parameter', '-Wno-unused-function']), language: 'c')
# the same three flags as the port's build of the core (port/meson.build core_cargs)
add_project_arguments(cc.get_supported_arguments(['-fno-strict-aliasing', '-fwrapv', '-ffp-contract=off']), language: 'c')

python = find_program('python3')
list = files('list_sources.py')
srcs = run_command(python, list, meson.current_source_dir() / 'src', check: true).stdout().strip().split('\n')
tsrcs = run_command(python, list, meson.current_source_dir() / 'tests', check: true).stdout().strip().split('\n')

inc = include_directories('include', 'src', 'tests')
m = cc.find_library('m', required: false)
libacore = static_library('acore', srcs, include_directories: inc, dependencies: m)
acore_dep = declare_dependency(link_with: libacore, include_directories: inc, dependencies: m)

acore_tests = executable('acore_tests', tsrcs, dependencies: acore_dep)
executable('acore_render', 'tools/acore_render.c', dependencies: acore_dep)
test('acore unit tests', acore_tests, args: ['-q'])
