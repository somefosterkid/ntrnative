"""battle_sweep_pt.py: the Platinum adapter of tools/battle_sweep.py (docs/battle-sweep.md "Adapter interface").

The exe's side is the Platinum half of port/game/route_gen4.c (BWPORT_PT_TESTBATTLE, BWPORT_PT_TESTLIST, BWPORT_PT_TESTSLOT,
BWPORT_PT_TESTEXIT; the twin of HeartGold's hooks) and port/src/run_frames.c (the `sweep:` lines, BWPORT_SWEEP_CASE_FRAMES);
a run starts from a library save (the room: pt_sweep.RELOAD walks the title menu to the free field) with an A pulse
(messages, the action menu's FIGHT, the move menu).
"""
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
import pt_sweep  # noqa: E402  (RELOAD, crash_info)

NAME = 'pt'
PORT_GAME = 'platinum'         # port_run.py --game
SAVE = 'pt-pgrunt'             # the library save the run starts from (pt_sweep.RELOAD's target: a field with a free player)
RUNNER_ARGS = ['--family', 'plat']   # chain_slot.sh: a Platinum job (the slot builds its Platinum exe: no --no-port, the exe must carry BWPORT_PT_TESTLIST)
MAX_MOVE, MAX_SPECIES = 467, 493
HARMLESS = 150                 # Splash: the move that does nothing (isolating a crash: our move vs it, it vs the foe's)
PULSE_FROM, PULSE_STEP = 1700, 24   # the first free field frame is ~f1656: no A in the field before the battle starts
MEW, BLISSEY = 151, 242
# our Pokemon and the foe in a move sweep: Mew L50 (a slot-1 move the list overwrites each turn), the foe Mew L50 too
TURNS_SPEC = f'{MEW}:50:{HARMLESS},0,0,0:{MEW}/50:{HARMLESS},{HARMLESS},{HARMLESS},{HARMLESS}'
A_MOVES = '150,33,247,94'      # a species case: Splash, Tackle, Shadow Ball (Ghost: Spiritomb, Shedinja), Psychic


def configure(a):
    """called once with the parsed arguments: the game and the save of this adapter (the core's --game default is HG's)"""
    a.game = PORT_GAME


def reload_keys():
    return pt_sweep.RELOAD


def instance_env(kind, batch, a):
    """the exe's env for one instance: `batch` = [(key, kind, our, foe)] (moves: one turn each; species: one case)"""
    env = {'BWPORT_PT_TESTEXIT': '1', 'BWPORT_BENCH_CLOCK': '2026-10-29T12:00:00', 'BWPORT_BENCH_SEED': '212'}
    if kind == 'moves':
        spec = TURNS_SPEC.replace(f':{HARMLESS},{HARMLESS},{HARMLESS},{HARMLESS}',
                                  f':{batch[0][3]},{batch[0][3]},{batch[0][3]},{batch[0][3]}')
        if len(batch[0]) > 4 and batch[0][4]:   # --pairs: the foe is another species (SPECIES/LEVEL)
            spec = spec.replace(f'{MEW}/50', batch[0][4])
        env['BWPORT_PT_TESTBATTLE'] = spec
        env['BWPORT_PT_TESTLIST'] = ','.join(f'{c[2]}:{c[3]}' for c in batch)
    elif kind == 'species-foe':   # the wild opponent is species N: our Mew L100 with four known moves, the run ends at the first menu
        env['BWPORT_PT_TESTBATTLE'] = f'{MEW}:100:{A_MOVES}:{batch[0][2]}/50'
        env['BWPORT_PT_TESTSLOT'] = 'stop'
    else:                          # our Pokemon is species N against Blissey L100
        env['BWPORT_PT_TESTBATTLE'] = f'{batch[0][2]}:50::{BLISSEY}/100:{HARMLESS}'
        env['BWPORT_PT_TESTSLOT'] = 'stop'
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
    """({move number: NAME}, {species number: NAME}) from the decomp's generated lists (one identifier per line, the line = the number)"""
    out = []
    for fn, pre in (('moves.txt', 'MOVE_'), ('species.txt', 'SPECIES_')):
        d = {}
        try:
            lines = open(os.path.join(REPO, 'third_party/pokeplatinum/generated', fn)).read().split()
        except OSError:
            lines = []
        for i, w in enumerate(lines):
            if w.startswith(pre):
                d.setdefault(i, w[len(pre):])
        out.append(d)
    return tuple(out)
