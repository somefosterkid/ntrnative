#!/usr/bin/env python3
"""gate_select.py: which merge-gate checks a change needs (the gate script calls it).
usage: gate_select.py [--full] [FILE...]   (no FILE: read names from stdin, one per line)
Prints one line per check: 'run NAME: why' or 'skip NAME: why', then 'TOOLTESTS f1 f2 ...' (the touched tools' own tests).
Checks: rom (ninja + both sha1s), port (Linux build + one --fast run), cores, switch, sweep, touch, apk, tooltests.
Tiers: decomp files (src lib include config asm extract configure.py) -> rom, and port when a header or lib changed;
port/ or libntr patches -> everything but apk; Android files or shared port code -> + apk; docs/tools only -> the
touched tools' own tests plus the check a tool belongs to (touch_*, lockstep/, port_run, android)."""
import os
import re
import sys

CHECKS = ['rom', 'port', 'cores', 'switch', 'sweep', 'touch', 'apk']
DECOMP = re.compile(r'^(src|lib|include|config|asm|extract)/|^(configure\.py|build\.ninja)$')
HEADERS = re.compile(r'^(include|lib)/|\.h$')
PORT = re.compile(r'^port/')
CORES = re.compile(r'^port/(vcore|gpu|acore)/')
SHARED = re.compile(r'^port/(src|subprojects|extern|extern_b2w2|include|include_b2w2|include_nnsys|bridge|machine|game)/')
ANDROID = re.compile(r'android|^port/src/.*(jni|sdl)', re.I)
TOOL_CHECK = [  # tools whose change needs the live check they belong to
    (re.compile(r'^tools/(touch_.*|port_run)\.py$'), 'touch'),
    (re.compile(r'^tools/(port_run|play_script)\.py$|^tools/plays/'), 'switch'),
    (re.compile(r'^tools/lockstep/|^tools/port_run\.py$'), 'sweep'),
    (re.compile(r'^tools/android'), 'apk'),
]


def select(files, full=False):
    why = {c: None for c in CHECKS}
    if full:
        return {c: 'FULL=1' for c in CHECKS}, []
    for f in files:
        if DECOMP.search(f):
            why['rom'] = why['rom'] or f'decomp file {f}'
            if HEADERS.search(f):
                why['port'] = why['port'] or f'header/lib {f} (port compile check)'
        if PORT.search(f):
            for c in ('port', 'cores', 'switch', 'sweep', 'touch'):
                why[c] = why[c] or f'port file {f}'
            if SHARED.search(f) or ANDROID.search(f):
                why['apk'] = why['apk'] or f'android/shared port file {f}'
        if ANDROID.search(f):
            why['apk'] = why['apk'] or f'android file {f}'
        for rx, c in TOOL_CHECK:
            if rx.search(f):
                why[c] = why[c] or f'tool {f}'
                if c in ('touch', 'switch', 'sweep'):
                    why['port'] = why['port'] or f'tool {f} needs the build'
    tests = []
    for f in files:
        m = re.match(r'^(tools/.*?)(_tests)?\.py$', f)
        if m:
            t = m.group(1) + '_tests.py'
            if (t in files or os.path.exists(t)) and t not in tests and (os.path.exists(t)):
                tests.append(t)
        if f == 'tools/lockstep/lstep_compare.py' or f == 'tools/lockstep/test_compare.py':
            tests.append('tools/lockstep/test_compare.py')
    return why, sorted(set(tests))


def main(argv):
    full = '--full' in argv
    files = [a for a in argv if not a.startswith('--')] or [l.strip() for l in sys.stdin if l.strip()]
    why, tests = select(files, full)
    for c in CHECKS:
        print(('run ' if why[c] else 'skip ') + c + ': ' + (why[c] or 'no file for it in the diff'))
    print(('run tooltests: ' + ' '.join(tests)) if tests else 'skip tooltests: no touched tool has tests')
    print('TOOLTESTS ' + ' '.join(tests))


if __name__ == '__main__':
    main(sys.argv[1:])
