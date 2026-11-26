"""battle_sweep_bw.py: the Black / White adapter of tools/battle_sweep.py (docs/battle-sweep.md "Adapter interface").

The exe's side is port/include/port_bw_sweep_field.h (BWPORT_BW_TESTBATTLE: a scripted wild battle from a free field),
port/include/port_bw_sweep_battle.h (BWPORT_BW_TESTLIST, BWPORT_BW_TESTSLOT, BWPORT_BW_TESTEXIT: the actions of each turn are
rewritten to the case's pair at the send to the server) and port/extern/src/ov093/battle_event.c (both sides always hit, no PP,
no hit over 1/8 of the defender's HP) with port/src/run_frames.c (the `sweep:` lines). A run starts from a library save (it skips
the title: the field is up at ~f220) with an A pulse (messages, the action menu's FIGHT, the move menu). Our Pokemon is the save's
lead (no species-our pass); `--game white` runs White (save w-...).
"""
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import pt_sweep  # noqa: E402  (crash_info: port_run's gdb backtrace)

NAME = 'bw'
PORT_GAME = 'black'            # port_run.py --game (a.game overrides: white)
SAVE = 'b-cas3'                # the library save the run starts from (a free field, a lead with moves); White: SAVE_WHITE
SAVE_WHITE = 'w-striaton-bros'
SAVE_B2 = 'b2-floccesy2'      # Black 2: a town (free field), Tepig Lv10; White 2: SAVE_W2
SAVE_W2 = 'w2-floccesy2'
RUNNER_ARGS = ['--family', 'gen5']   # chain_slot.sh: the slot builds the exe, which must carry BWPORT_BW_TESTLIST (no --no-port)
MAX_MOVE, MAX_SPECIES = 559, 649
HARMLESS = 150                 # Splash
PULSE_FROM, PULSE_STEP = 420, 24     # the field is up at ~f220, the wild battle starts 90 free frames later
MEW = 151
FOE_LEVEL = 50


def configure(a):
    """called once with the parsed arguments: the game and the save of this adapter"""
    if a.game not in ("black", "white", "b2", "w2"):   # the core's --game default is HG's
        a.game = PORT_GAME
    globals()['PORT_GAME'] = a.game
    if not a.save:
        a.save = {'white': SAVE_WHITE, 'b2': SAVE_B2, 'w2': SAVE_W2}.get(a.game, SAVE)
    if a.what == 'species-our':
        raise SystemExit('battle_sweep_bw: no species-our pass (our Pokemon is the save lead)')


def reload_keys():
    return ''


def instance_env(kind, batch, a):
    """the exe's env for one instance: `batch` = [(key, kind, our, foe)] (moves: one turn each; species: one case)"""
    env = {'BWPORT_BW_TESTEXIT': '1', 'BWPORT_BENCH_CLOCK': '2026-10-29T12:00:00', 'BWPORT_BENCH_SEED': '212'}
    if kind == 'moves':
        foe = batch[0][4] if len(batch[0]) > 4 and batch[0][4] else f'{MEW}/{FOE_LEVEL}'   # --pairs: SPECIES/LEVEL
        env['BWPORT_BW_TESTBATTLE'] = foe.replace('/', ':')
        env['BWPORT_BW_TESTLIST'] = ','.join(f'{c[2]}:{c[3]}' for c in batch)
    else:                          # species-foe: the wild opponent is species N, the run ends at the first move menu
        env['BWPORT_BW_TESTBATTLE'] = f'{batch[0][2]}:{FOE_LEVEL}'
        env['BWPORT_BW_TESTSLOT'] = 'stop'
    return env


def port_run_args(kind):
    return ['--no-cheat']


def crash_info(log):
    return pt_sweep.crash_info(log)


def battle_started(log):
    return re.search(r'^test: wild battle', log, re.M) is not None


def species_ok(log):
    """a species case is ok when the first move menu was reached (`test: moves done`) or the battle ended"""
    return re.search(r'^test: (?:moves done|battle over)', log, re.M) is not None


def species_frames(log):
    m = re.findall(r'^test: (?:moves done|battle over)[^\n]*V-blank (\d+)', log, re.M)
    return m[-1] if m else ''


def names():
    """({move number: NAME}, {species number: NAME}) from the decomp's constants headers (identifiers, for the report)"""
    out = []
    for fn, pre in (('moves.h', 'MOVE_'), ('species.h', 'SPECIES_')):
        d = {}
        try:
            text = open(os.path.join(REPO, 'include/constants', fn)).read()
        except OSError:
            text = ''
        for line in text.splitlines():
            w = line.split()
            if len(w) >= 3 and w[0] == '#define' and w[1].startswith(pre) and w[2].isdigit():
                d.setdefault(int(w[2]), w[1][len(pre):])
        out.append(d)
    return tuple(out)
