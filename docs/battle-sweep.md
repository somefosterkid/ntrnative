# Battle sweep (bs1): every move from both sides, every species, many cases per battle

`tools/battle_sweep.py` (game-neutral core) + `tools/battle_sweep_<adapter>.py` (thin per-game adapter; `hg` = HeartGold/SoulSilver,
`pt` = Platinum: `--adapter pt`, game `platinum`, save `pt-pgrunt`; `bw` = Black/White: `--adapter bw [--game white]`, saves
`b-cas3` / `w-striaton-bros`; the same commands below).
Cheat code lives only in the port's test hooks (off unless the env var is set). Frames are game V-blanks, never wall time.

## Run
    python3 tools/battle_sweep.py run --adapter hg --what moves --ids all -j 8 --out DIR --isolate
    python3 tools/battle_sweep.py run --adapter hg --what species-foe|species-our --ids all -j 8 --out DIR
    python3 tools/battle_sweep.py submit --adapter hg --shards bd:3,laptop:1 --commit REF --out DIR   # shards over machines
Cases: `moves` = one turn per case (our move M vs the foe's partner(M): each move is used once by each side; `--isolate` re-runs a
failed pair as (M vs a harmless move) and (harmless vs F): culprit our/foe/our+foe/state); `species-foe` / `species-our` = one battle
per species, ended at the first move menu. Out: results.tsv (case, kind, outcome ok|ok-end|crash|hang|heap|no-start|no-battle,
V-blanks, top function, bt_id, top 3 frames, culprit, previous cases, wall s), summary.txt, runs/<case>/log.txt.gz of each failure.
Crash-and-continue: the case running at a crash/hang (last `sweep: case K ... start` without `done`) is recorded and the next
instance resumes after it. `--case-frames N` (default 8000 V-blanks) is the per-case hang cap, `--batch` the cases per battle (12).

Parallelism: `-j N` workers per job, each with a run dir `out/run/w<i>` that LINKS one imported copy of the game data
(`out/run/tmpl`, ~250 MB, imported once per job): a job's disk is one copy; `out/run` and the queue are removed when the job ends
(a failure keeps `runs/<case>/log.txt.gz` and `bt.txt`). Work is a DYNAMIC QUEUE: every worker takes the next unclaimed unit (a batch
of moves cases, one species case) by an atomic mkdir in `out/queue`; `--queue DIR` shares one queue between several jobs on one host,
so no straggler holds a fixed chunk and a job that starts late just takes less. `submit --shards bd:3,bd:3,bd:3,laptop:1` starts one
job per entry on a free slot of that machine (`--remote-out DIR`: where each job keeps its out dir there); the jobs of one machine
share a slice of the case list (their weights summed; the slices of two machines split by weight) and one queue. Keep the sum of all
`-j` at or under 80% of the machines' threads.
WATCHDOG (wall time is allowed here only): every instance's V-blank counter is read each second; no new V-blank for `--watchdog` s
(default 90; the boot gets 3x) or a log over `--max-log` MB (default 300) is a `hang`: SIGINT to the exe (the gdb under port_run
prints `thread apply all bt` into the log), then SIGKILL of the instance's own session; the row carries that backtrace and the
sweep goes on with the case after it (port_run's own stall rule, `--port-hang`, default 30 s, fires first when it works).
RESUME: `run` on an out dir that already has results.tsv skips the cases in it (`--redo` runs everything again), EXCEPT `harness` rows (the
sweep's own error: an exception, a dead exe; not a verdict on the case): those are dropped from the table and run again, so a stale out dir
of a run that failed everywhere never looks like finished results.
PROGRESS: every finished unit prints `[Ns] n/N cases (worker i)` and overwrites `$NTR_PROGRESS` (set by a queue job runner on the slot;
the job runner's progress viewer shows it). `submit --progress SECS` prints one `progress TAG on MACHINE: [Ns] n/N cases` line per live shard every
SECS s (the runner's `--peek RUN progress`; a runner without `--peek` just shows "(no progress yet)").
`--pairs OUR:FOE[@SPECIES/LEVEL],..` runs single turns against a chosen foe species (the long-animation cases: raise `--case-frames`);
`--isolate` accepts these keys too (`p433-153@161L5`): both halves keep the pair's foe species and level.
`report --out DIR --md FILE` writes the ranked markdown (crash, heap, hang, cap groups: names from the decomp's constants headers,
backtrace top 3, culprit side after `--isolate`, a repro command).
`--shard I/N --weights w1,..` takes the I-th contiguous slice (a 3x faster machine: weight 3); BATTLE_SWEEP_RUNNER (default
chain_slot.sh) runs each job (`--commit --hg --family hgss --no-port --on M --cmd ...`) and one `BSROWS` line (the table, gzip + base64: a runner shows only the last lines of a command) carries the rows back.

## Exe protocol (the adapter's contract with the game)
port/src/run_frames.c: `PORT_SweepCase(k, what)` -> `sweep: case K <what> start at V-blank N` (closes the previous case with
`sweep: case K-1 done at V-blank N (F frames)`); `PORT_SweepEnd(why, exitNow)` -> `sweep: end list|battle at V-blank N`;
BWPORT_SWEEP_CASE_FRAMES=F: a case over F V-blanks prints `sweep: case K cap F frames`, the state line, exits 0.

## Adapter interface (tools/battle_sweep_<name>.py)
NAME, PORT_GAME (port_run --game), SAVE (library save), MAX_MOVE, MAX_SPECIES, HARMLESS (a do-nothing move id), PULSE_FROM,
PULSE_STEP (the A-pulse), `configure(args)`, `reload_keys()`, `instance_env(kind, batch, args)` (the exe's env for one instance;
batch = [(key, kind, our, foe)]), `port_run_args(kind)`, `crash_info(log)` -> (top, text, id), `battle_started(log)`,
`species_ok(log)`, `species_frames(log)`; optional `names()` (constants names for the report) and `RUNNER_ARGS` (the job flags `submit`
gives the runner; default HG's `--hg --family hgss --no-port`, Platinum's `--family plat`: the slot builds the exe, which must carry
BWPORT_PT_TESTLIST: with `--no-port` a stale exe ignores the list and every battle runs to the frame cap as `no-start`).

## Black / White hooks (port/include/port_bw_sweep_field.h, port_bw_sweep_battle.h, port/extern/src/ov093/battle_event.c)
BWPORT_BW_TESTBATTLE=SPECIES:LEVEL: once the field has been free for 90 frames a scripted wild battle starts (the CallWildBattleEx
event, then the field's re-entry), `test: wild battle ...`; the field free again: `test: battle over` (+ `sweep: end battle`).
BWPORT_BW_TESTLIST=O:F,...: the btl_client.c stand-in routes the client's sends through PortSweep_Send; a turn is a send of the
client's actions array holding a fight action (acks, yes/no and switch picks are not turns: counting every send ended the list before
case 0). There our client's fight action becomes O, the AI's F (BattleAction_SetFightParam: the server runs a move the Pokemon does
not know; it has no PP slot, so no PP is used), and our client refills both sides' HP first and prints `sweep: case K`. Our client
also prints `sweep: exec battler B move M` for each move the server runs (its "used a move" command 0x59). battle_event.c: both
sides always hit, no PP, no hit over 1/8 of the defender's HP. Our lead fainting (Self-Destruct, Explosion, Memento, an OHKO: the cap
is on damage only) ends the instance at our next send with `sweep: end battle`, and so does the party list opening (our U-turn,
Volt Switch, Baton Pass: the A pulse cannot pick a Pokemon; BattleClientCmd_StartPokeList is routed through PortSweep_PokeList);
the foe fainting ends the battle (`test: battle over`), and so does the server's experience command (0x45, SC_EXP_GAIN in pokebw2: the
foe's Explosion / Memento made our lower-level lead level up, and the level-up / learn-move screens wait for a choice the A pulse
cannot make: PortSweep_ReadCmd ends the instance with `sweep: end battle` when it reads it); the harness resumes after that case either way.
Black 2 / White 2 (`--game b2|w2`, saves b2-floccesy2 / w2-floccesy2): the same two headers, compiled through the stand-ins
port/extern_b2w2/src/ov167/btl_client.c, ov167/battle_event.c and system/game_event.c; the game's own names come in as PSW_* macros
(PSW_MOVE_CMD = SC_MOVE_MSG, PSW_EXP_CMD = SC_EXP_GAIN, PSW_TARGET, PSW_WILD_EVENT = EventWildBattleCall_CreateStatic; the send is
BtlAdapter_SetReturnData, the read BtlServerCmdQueue_Read). `--commit REF` is resolved in the main repository, not the cwd: pass the
branch name (HEAD is main's). BWPORT_BW_TESTSLOT=stop: the first turn ends the run (`test: moves done`,
species-foe). Our Pokemon is the save's lead: no species-our pass. One case is ~550-900 V-blanks; the boot to the field ~f220.
White's build renames the game's functions (a w_ macro): the stand-in captures the real ones as function pointers before its
call-site macros, never #undef.
First Black/White moves passes (2026-11-25, `--isolate --watchdog 400`): Black 559 cases, ok 555, wall 1781 s over bd:3,bd:3,laptop:1,local:1;
White 559, ok 554, wall 1745 s over laptop:1,local:1,local:1. No battle-code crash. The hangs were our Baton Pass / U-turn / Volt Switch
at the party list (fixed: the 4 Black rows rerun ok) and, on White only, the foe's Explosion / Memento (433-153, 542-262: the foe
faints, our lower-level lead levels up and the move-to-learn screen waits; still open: checking the foe's HP at our next send did not
end them). The one crash (Black m37-316, SIM_Audio_Init, culprit state) is a boot crash, as on Platinum.

## Platinum hooks (the Platinum half of port/game/route_gen4.c)
Same switches as HG's with `PT` for `HG`: BWPORT_PT_TESTBATTLE (spec as HG's), BWPORT_PT_TESTLIST (Pr4_TestList: the BattleMons'
curHP/maxHP, status, statusVolatile, moves, ppCur), BWPORT_PT_TESTSLOT, BWPORT_PT_TESTEXIT. CheatSide / CheatDamage / CheatHpCalc
answer for the sweep (both sides hit, no PP, 1/8 HP cap, no last-HP loss) exactly as on HG. Platinum starts from the library save
`pt-pgrunt` through pt_sweep.RELOAD, the first free field frame is ~f1656 (PULSE_FROM 1700). tools/pt_battle_sweep.py is the older
species/move pass tool (4 moves per battle, no both-sides coverage): kept, the sweep core replaces it.
First Platinum run (2026-11-24, loaded pool, `--watchdog 400`): moves 467/467 cases ran (ok 465; the 2 crashes are frame-0 boot crashes in
SIM_RenderInit/SIM_Audio_Init that `--isolate` could not reproduce), wall 2695 s over bd:3,bd:3,laptop:1,laptop:1. A loaded host
starves the boot: raise `--watchdog` (400) or the boot looks like a hang. Two `submit`s in one second used to share an out dir.

## HG hooks (port/game/route_gen4.c)
BWPORT_HG_TESTBATTLE=OURSPECIES:LEVEL[:M..[:FOESPECIES/LEVEL[:F..]]], BWPORT_HG_TESTLIST=O:F,O:F,... (one pair per move menu: our
slot 1 = O, every foe slot = F, PP 35, HP full, status cleared; the wild AI picks after our answer, so F applies to the same turn),
BWPORT_HG_TESTSLOT=stop|cycle|N, BWPORT_HG_TESTEXIT=1. With TESTLIST both sides always hit and use no PP, a hit takes at most 1/8
of the defender's HP, a script loss (recoil, self-KO) never the last HP: nothing faints. A battle that ends (blown away) prints
`sweep: end battle`; the harness resumes after that case (`ok-end`).
