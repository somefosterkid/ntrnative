#!/usr/bin/env python3
"""config_migrate_test.py (rn80): the ini migration of. Builds a tiny harness over libntr's own sim_config.c.o /
ini.c.o (from build/port/linux, so run a port build first), loads a series of old / new / mixed sim_config.ini files with
SIM_Config_LoadDefaults + SIM_Config_LoadConfigFile, checks the loaded Game speed / Frame rate / Frame rate restore /
Smooth motion, saves with SIM_Config_SaveConfigFile and checks the saved file (new keys only, nothing lost), then loads the
saved file again (the round trip must not change a value). Env vars: BWPORT_GAMESPEED / BWPORT_SPEED, BWPORT_FRAMERATE /
BWPORT_DISPLAYCAP, BWPORT_SMOOTH / BWPORT_INTERP over a file. Exit 0 = all pass.
  python3 tools/config_migrate_test.py"""
import os, re, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BLD = os.path.join(ROOT, 'build/port/linux')
RATE = {'60': 0, '120': 1, 'monitor': 2, 'unlimited': 3}

HARNESS = r'''
#include <stdio.h>
#include <unistd.h>
#include <simulator/config/sim_config.h>
int main(int argc, char **argv) {
    SIM_config_type c;
    if (argc > 1 && chdir(argv[1]) != 0) return 2;
    SIM_Config_LoadDefaults(&c);
    SIM_Config_LoadConfigFile(&c);
    printf("speed=%d rate=%d back=%d smooth=%d\n", c.speed, c.displayCap, c.displayCapBack, c.interpolation);
    SIM_Config_SaveConfigFile(&c);
    return 0;
}
'''
STUBS = r'''
unsigned char s_HW_MAIN_MEM_SYSTEM[1 << 16];
void MI_CpuCopy8(const void *a, void *b, unsigned c) { (void)a; (void)b; (void)c; }
'''


def build(tmp):
    ninja = open(os.path.join(BLD, 'build.ninja')).read()
    m = re.search(r'src_config_sim_config\.c\.o: .*?\n(?: .*\n)*? ARGS = (.*)\n', ninja)
    args = m.group(1).split()
    open(tmp + '/h.c', 'w').write(HARNESS)
    open(tmp + '/stubs.c', 'w').write(STUBS)
    subprocess.check_call(['clang'] + args + ['-c', tmp + '/h.c', '-o', tmp + '/h.o'], cwd=BLD)  # the ninja ARGS are relative to BLD
    subprocess.check_call(['clang', '-w', '-c', tmp + '/stubs.c', '-o', tmp + '/stubs.o'])
    objs = [os.path.join(BLD, 'subprojects/libntr/libraries/sim/libsim.a.p/src_config_%s.c.o' % n) for n in ('sim_config', 'ini')]
    subprocess.check_call(['clang', '-o', tmp + '/h', tmp + '/h.o', tmp + '/stubs.o'] + objs)
    return tmp + '/h'


def run(h, ini, env=None):
    d = tempfile.mkdtemp(prefix='cfg')
    if ini is not None:
        open(d + '/sim_config.ini', 'w').write(ini)
    e = {k: v for k, v in os.environ.items() if not k.startswith('BWPORT_')}
    e.update(env or {})
    out = subprocess.run([h, d], capture_output=True, text=True, env=e).stdout.strip()
    saved = open(d + '/sim_config.ini').read()
    v = dict(x.split('=') for x in out.split())
    return {k: int(x) for k, x in v.items()}, saved, d


def keys(text):
    return dict(l.split('=', 1) for l in text.splitlines() if '=' in l)


OLD_KEYS = ('Speed', 'DisplayCap', 'CapFrameRate', 'Interpolation')

# (name, ini, env, expected speed, rate, back, smooth)
CASES = [
    ('old: cap off + 120 + 3x + interp', '[General]\nCapFrameRate=false\nSpeed=3\nDisplayCap=120\nInterpolation=on\n', {}, 3, 'unlimited', '120', 1),
    ('old: cap on + monitor', '[General]\nCapFrameRate=true\nDisplayCap=monitor\nSpeed=2\nInterpolation=off\n', {}, 2, 'monitor', 'monitor', 0),
    ('old: cap off only (Tab user)', '[General]\nCapFrameRate=false\n', {}, 1, 'unlimited', '60', 0),
    ('old: cap on only', '[General]\nCapFrameRate=true\n', {}, 1, '60', '60', 0),
    ('old: DisplayCap=120 no cap key', '[General]\nDisplayCap=120\n', {}, 1, '120', '120', 0),
    ('old: cap off + monitor', '[General]\nCapFrameRate=false\nDisplayCap=monitor\n', {}, 1, 'unlimited', 'monitor', 0),
    ('old: Speed=9 clamps, DisplayCap=240', '[General]\nSpeed=9\nDisplayCap=240\n', {}, 4, '120', '120', 0),
    ('old: Interpolation=1', '[General]\nInterpolation=1\n', {}, 1, '60', '60', 1),
    ('old: keys in the other order', '[General]\nInterpolation=on\nDisplayCap=120\nSpeed=2\nCapFrameRate=false\n', {}, 2, 'unlimited', '120', 1),
    ('new: unlimited + restore monitor', '[General]\nGameSpeed=2\nFrameRate=unlimited\nFrameRateRestore=monitor\nSmoothMotion=off\n', {}, 2, 'unlimited', 'monitor', 0),
    ('new: 120 no restore', '[General]\nFrameRate=120\nSmoothMotion=on\n', {}, 1, '120', '120', 1),
    ('new: unlimited no restore + old DisplayCap', '[General]\nFrameRate=unlimited\nDisplayCap=120\n', {}, 1, 'unlimited', '120', 0),
    ('mixed: new keys win over old', '[General]\nCapFrameRate=false\nFrameRate=60\nSpeed=4\nGameSpeed=2\nInterpolation=on\nSmoothMotion=off\nDisplayCap=120\n', {}, 2, '60', '120', 0),
    ('no file', None, {}, 1, '60', '60', 0),
    ('empty file', '[General]\n', {}, 1, '60', '60', 0),
    ('env old names over a file', '[General]\nGameSpeed=1\nFrameRate=60\n', {'BWPORT_SPEED': '2', 'BWPORT_DISPLAYCAP': 'unlimited', 'BWPORT_INTERP': 'on'}, 2, 'unlimited', '60', 1),
    ('env new names over a file', '[General]\nGameSpeed=1\nFrameRate=unlimited\nFrameRateRestore=60\n', {'BWPORT_GAMESPEED': '3', 'BWPORT_FRAMERATE': 'monitor', 'BWPORT_SMOOTH': 'on'}, 3, 'monitor', 'monitor', 1),
    ('env DISPLAYCAP=120 over an old file', '[General]\nCapFrameRate=false\n', {'BWPORT_DISPLAYCAP': '120'}, 1, '120', '120', 0),
]


def main():
    tmp = tempfile.mkdtemp(prefix='cfgh')
    h = build(tmp)
    bad = 0
    for name, ini, env, sp, rate, back, sm in CASES:
        got, saved, d = run(h, ini, env)
        want = dict(speed=sp, rate=RATE[rate], back=RATE[back], smooth=sm)
        k = keys(saved)
        errs = []
        if got != want:
            errs.append('loaded %s want %s' % (got, want))
        for o in OLD_KEYS:
            if o in k:
                errs.append('saved file still has old key %s' % o)
        exp = {'GameSpeed': str(sp), 'FrameRate': rate, 'FrameRateRestore': back, 'SmoothMotion': 'on' if sm else 'off'}
        for key, val in exp.items():
            if k.get(key) != val:
                errs.append('saved %s=%s want %s' % (key, k.get(key), val))
        # the round trip: load the saved file again (no env): same values
        if not env:
            got2, saved2, _ = run(h, saved)
            if got2 != want:
                errs.append('round trip gave %s' % got2)
            k2 = keys(saved2)
            if any(k2.get(x) != k.get(x) for x in list(exp) + ['VsyncInterval']):  # (Owner_Nickname differs on every load: not ours)
                errs.append('round trip changed the file')
        print('%-4s %s%s' % ('FAIL' if errs else 'ok', name, ('  ' + '; '.join(errs)) if errs else ''))
        bad += bool(errs)
    print('config-migrate: %s (%d cases)' % ('FAIL' if bad else 'PASS', len(CASES)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
