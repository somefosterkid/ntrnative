#!/usr/bin/env python3
"""Tests for perf_frame.py: the per-frame difference of two runs, and the counters on a tiny program (skipped without a PMU)."""
import json
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import perf_frame as pf


def t_bench_env():
    e = pf.bench_env({'BWPORT_MUSICBUS': 'on', 'BWPORT_RENDERER': 'gpu'})
    assert e['BWPORT_MUSICBUS'] == 'on' and e['BWPORT_RENDERER'] == 'gpu', e
    assert e['BWPORT_SYSBURST'] == 'frames' and e['BWPORT_CFGSCRIPT'] == 'f1:ff=0,side=0' and e['BWPORT_PRESENTDROP'] == '0', e


def t_warm_up():
    seq = [131.5, 106.7, 100.3, 100.3, 100.3]
    made = []

    def run(i):
        made.append(i)
        return {'tids': {'1': {'instructions': seq[i], 'main': True}, '2': {'instructions': 5.0, 'main': False}}}
    assert pf.warm_up(run) == 4 and made == [0, 1, 2, 3], made
    made.clear()
    assert pf.warm_up(run, most=2) == 2 and made == [0, 1], made


def t_diff():
    z = {'instructions': 0, 'cycles': 0, 'task_ns': 0}
    a = {'total': {'instructions': 100e6, 'cycles': 50e6, 'task_ns': 20e6}, 'threads': {'game': dict(z, instructions=60e6)}}
    b = {'total': {'instructions': 400e6, 'cycles': 150e6, 'task_ns': 50e6},
         'threads': {'game': dict(z, instructions=300e6, cycles=100e6, task_ns=40e6), 'render': dict(z, instructions=30e6)}}
    d = pf.diff(a, b, 100)
    assert abs(d['total']['M_instr'] - 3.0) < 1e-9, d
    assert abs(d['game']['M_instr'] - 2.4) < 1e-9 and abs(d['render']['M_instr'] - 0.3) < 1e-9, d
    assert abs(d['(other: short threads)']['M_instr'] - 0.3) < 1e-9, d


def t_wrap_counts_a_child():
    if pf.counters_available():
        return
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, 'o.json')
        env = dict(os.environ, BWPORT_PERF_OUT=out)
        r = subprocess.run([sys.executable, pf.__file__, '--wrap', '--', sys.executable, '-c', 'sum(range(300000))'], env=env)
        assert r.returncode == 0
        d = json.load(open(out))
        assert d['exit'] == 0 and d['total']['instructions'] > 1e6 and d['total']['cycles'] > 1e5, d


if __name__ == '__main__':
    n = 0
    for k, v in sorted(globals().items()):
        if k.startswith('t_'):
            v()
            n += 1
    print(f'perf_frame_tests: {n} ok')
