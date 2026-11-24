# Battle sweep (bs1): every move from both sides, every species, many cases per battle

`tools/battle_sweep.py` (game-neutral core) + `tools/battle_sweep_<adapter>.py` (thin per-game adapter; `hg` = HeartGold/SoulSilver).
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
`--pairs OUR:FOE[@SPECIES/LEVEL],..` runs single turns against a chosen foe species (the long-animation cases: raise `--case-frames`).
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
`species_ok(log)`, `species_frames(log)`. Platinum: BWPORT_PT_TESTLIST in the Platinum half of port/game/route_gen4.c (copy of
HG's PortRoute4_TestList: foe moves set on the BattleMons at the move menu, CheatSide/Damage/HpCalc clamps), then the adapter.

## HG hooks (port/game/route_gen4.c)
BWPORT_HG_TESTBATTLE=OURSPECIES:LEVEL[:M..[:FOESPECIES/LEVEL[:F..]]], BWPORT_HG_TESTLIST=O:F,O:F,... (one pair per move menu: our
slot 1 = O, every foe slot = F, PP 35, HP full, status cleared; the wild AI picks after our answer, so F applies to the same turn),
BWPORT_HG_TESTSLOT=stop|cycle|N, BWPORT_HG_TESTEXIT=1. With TESTLIST both sides always hit and use no PP, a hit takes at most 1/8
of the defender's HP, a script loss (recoil, self-KO) never the last HP: nothing faints. A battle that ends (blown away) prints
`sweep: end battle`; the harness resumes after that case (`ok-end`).
