#!/usr/bin/env python3
"""Tests for tools/interp_sweep.py with synthetic scorer output (no capture, no build needed):

    python3 tools/interp_sweep_tests.py        # prints "ok" per test, exits 1 on a failure
"""
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import interp_sweep as S  # noqa: E402

OUT = """PLAY heartgold cherry-door-east 10990-11360/1
EV A_MISPAIR 11100 11098 421 2893370c 4 120.0 90.0 16.0 16.20 12.15 8.10 partner=3 best=4
EV A_MISPAIR 11102 11100 422 2893370c 4 121.0 90.0 16.0 15.00 11.25 7.50 partner=3 best=4
EV A_MISPAIR 11400 11398 10 11111111 4 20.0 30.0 8.0 7.00 5.25 3.50 -
EV C_APPEAR 11200 11198 5 0db31a08 120 250.0 80.0 60.0 2.00 1.50 1.00 ref=2.0,0.0 nl=600/598
EV E_CUT 11300 11298 -1 00000000 0 0.0 0.0 0.0 0.00 0.00 0.00 cut
STAT keyframes 180 repeats 190 pairs 178 cuts 1 lists 100000 paired 98.40% gaps1 3 gaps2 175 gaps3+ 1
SUM A_MISPAIR 3 3 3 12.15 11100 421
SUM C_APPEAR 1 1 0 1.50 11200 5
SUM E_CUT 1 0 0 0.00 11300 -1
"""


def test_parse():
    head, evs, sums, stat = S.parse(OUT)
    assert head == {'game': 'heartgold', 'play': 'cherry-door-east', 'spec': '10990-11360/1'}
    assert len(evs) == 5 and evs[0].cls == 'A_MISPAIR' and evs[0].old == 11098 and evs[0].list == 421
    assert evs[0].v120 == 12.15 and evs[0].info == 'partner=3 best=4'
    assert sums['A_MISPAIR'] == (3, 3, 3, 12.15, 11100, 421)
    assert stat['keyframes'] == '180' and stat['paired'] == '98.40%'


def test_worst_one_per_neighbourhood():
    _, evs, _, _ = S.parse(OUT)
    w = S.worst(evs, 'A_MISPAIR', 5)
    # 11100 and 11102 are one artifact (same key, same 30-frame window): it takes one slot
    assert [e.frame for e in w] == [11100, 11400]


def test_read_set_shards():
    with tempfile.TemporaryDirectory() as d:
        p = os.path.join(d, 'set.txt')
        open(p, 'w').write('# c\na 1-2/1\n\nb 3-4/1\nc 5-6/1\n')
        assert S.read_set(p, None) == [['a', '1-2/1'], ['b', '3-4/1'], ['c', '5-6/1']]
        assert S.read_set(p, '1/2') == [['a', '1-2/1'], ['c', '5-6/1']]
        assert S.read_set(p, '2/2') == [['b', '3-4/1']]


def test_crop_box_clips_to_top_screen():
    assert S.crop_box(128, 96, 10, 3, 768, 1152) == (288, 201, 192, 174)
    x0, y0, cw, ch = S.crop_box(250, 190, 40, 3, 768, 1152)
    assert x0 + cw <= 768 and y0 + ch <= 576 and cw > 0 and ch > 0


def test_png_roundtrip_and_strip():
    with tempfile.TemporaryDirectory() as d:
        rows = [bytes(b for x in range(8) for b in (x * 10, y * 20, 7)) for y in range(6)]
        p = os.path.join(d, 'a.png')
        S.writepng(p, 8, 6, rows)
        w, h, back = S.readpng(p)
        assert (w, h) == (8, 6) and [bytes(r) for r in back] == rows
        out = os.path.join(d, 's.png')
        S.strip([p, p], (2, 1, 3, 2), out)
        w, h, back = S.readpng(out)
        assert (w, h) == (2 * (3 + 4), 2)
        assert bytes(back[0][:9]) == rows[1][6:15] and bytes(back[0][9:12]) == bytes([255, 0, 255])


def test_report_counts_and_ranks():
    with tempfile.TemporaryDirectory() as d:
        open(os.path.join(d, 'heartgold-cherry-door-east.txt'), 'w').write(OUT)
        open(os.path.join(d, 'black-x.txt'), 'w').write(
            'PLAY black x 1-2/1\nEV A_MISPAIR 50 48 7 abcdef01 4 10.0 10.0 4.0 30.00 22.50 15.00 -\n'
            'SUM A_MISPAIR 1 1 1 22.50 50 7\nSTAT keyframes 2 pairs 1\n')
        md = os.path.join(d, 'r.md')
        assert S.main(['report', '--out', d, '--md', md, '--top', '2']) == 0
        t = open(md).read()
        assert '| A_MISPAIR | 1 / 1 / 1 | 3 / 3 / 3 |' in t  # games sorted: black, heartgold
        sec = t.split('## A_MISPAIR: worst 2')[1]
        rows = [ln for ln in sec.splitlines() if ln.startswith('| black') or ln.startswith('| heartgold')]
        assert rows[0].startswith('| black | x | 50 (48)') and rows[1].startswith('| heartgold | cherry-door-east | 11100')
        assert '## C_APPEAR: worst 2' in t and '## B_HELD:' not in t


if __name__ == '__main__':
    bad = 0
    for name, fn in sorted(globals().items()):
        if name.startswith('test_') and callable(fn):
            try:
                fn()
                print('ok', name)
            except Exception as e:  # noqa: BLE001
                bad += 1
                print('FAIL', name, repr(e))
    sys.exit(1 if bad else 0)
