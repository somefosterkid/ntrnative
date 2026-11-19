#!/usr/bin/env python3
"""Replay a game's playthrough route, on Linux (the PC, headless in Xvfb) or the Mac (docs/port-worker.md, "Routes": read it
before writing a leg).

    tools/route.py GAME|all [LEG ...] [--no-cheat] [--fast-opts] [--quick] [-j N] [--store] [--replace NAME] [--chain] [--shot N] [--cap SECS] [--list] [--learn] [--repeat N] [--skip N] [--gpu]
    tools/route.py port SRC DST --from LEG [--to LEG] [--start SAVE] [--dry]   (b -> w, b2 -> w2: tools/route_port.py, docs/port-worker.md)

GAME is b, w, b2 or w2 (Gen 4: pt, hg, ss, from the same default build, or another dir: tools/port_run.py's GAME_BUILD): tools/routes/GAME.txt holds the legs, one a line, fields separated by " | ":

    name | start | pilot tokens | end | save | flags

start is a save in the saves library, $BWPORT_SAVES (Linux: ~/saves, whose README is the index; Mac: ~/bw-saves) (a name without .bin) or NEWGAME (the boot, the title); the pilot tokens are
BWPORT_PILOT's (port/src/route_test.c; E:on / E:off switch the wild encounters, C:ball catches a wild Pokemon with the first Poke Ball); end is where the leg must finish: "zone N x,z" (the grid position, the last "pos:"
line of the run); save is the name of the save the leg produces ("-" for none); flags: `encounters` (wild encounters on:
the leg fights), `noskip` (no frame skip), `realtime` (the game's own pace, not Max), `quick` (in --quick's set), `defaultopts`
(the game's own speeds even with the speed options: a leg not yet made to pass with them), `expect-hang` (a leg that is meant to end in
a hang or timeout: it passes as `look` then, any other verdict fails). '#' starts a comment. `(TOKENS)*N` in the pilot
field is TOKENS written N times (a cycle of K / T tokens: Platinum's new-look).

Each leg runs from its start save in its own copy of a per-game base root (the ROM imported once; Linux: build/route-cache of the worktree, $ROUTE_CACHE;
Mac: ~/bw-cache, an APFS clone), through tools/port_run.py on Linux (a fresh worktree builds its exe on the first run, 1-2 min) and tools/mac_run.sh on
the Mac, uncapped (cfgscript ff=1 from frame 0), pinned (BWPORT_BENCH_CLOCK and BWPORT_BENCH_SEED: the same keys always play the same
frames), with BWPORT_NOENCOUNTER=1 unless the leg says `encounters`, frame skip on (the picture 1 in 8; the logic runs every frame), and prints one line: ok, or crash,
hang, stuck, timeout (an event wait not done), save-failed, off-route with the position. Everything else is in RUN_WORK/GAME-LEG.log and GAME-LEG.game.log (RUN_WORK is $cache/route-work, or $ROUTE_WORK). Every run has
BWPORT_MAPDUMP=RUN_WORK/GAME-LEG-map/ (a map dump at its end): it stays with the game's log (game.log there) for any verdict but
ok, named on the leg's line (map=DIR/).

Verdicts: ok (ends at `end`); look (a look leg, end `zone 0 0,0`, finished and left its map dump: a pass, read the dump); off-route (ended
elsewhere: a scene differed, or a new leg: --learn); crash (the game died: a game bug, report the place, do
not work around it); hang (the pilot made no progress for 3000 frames, or the game stopped drawing: read the map dump in GAME-LEG-map/, its story
section, and --shot N); stuck / "pilot: no path" (a walk or target cannot be done: read the dump, fix the token); timeout (a W token never finished); save-failed.
Writing a leg: `look | SAVE | Md C | zone 0 0,0` with --map, read warps (D -> zone), actors (N uid), triggers (T) and the story section (LIVE scene, flags); write
G:warp:ZONE (or G:zone:N, below), G:npc:UID (talk or trainer, one token), G:item, G:trig (then Wf), G:bg, Wf, Ws:FLAG, S; --learn the end; --repeat 2 must say SAME; --store NEWNAME and
one README line. In a battle the pilot presses A and answers the forget-a-move and switch questions No (B).

-j N          legs run N at a time (default: half the cores; --chain runs in order). The end line is "N/M ok in S s; F game
              frames in L leg seconds (X frames/s)": a leg's line also ends with its wall seconds and v=N, the game frames (V-blanks
              since the boot: a leg from a save starts at the load's, about 220)
--quick       only the legs flagged `quick`; GAME `all` is the four games (the check every merge and push gets: tools/check.sh)
--chain       a leg's start save is the previous leg's produced one when this run made it (a route from NEWGAME in one go). It
              ends with the full-run report, a table of leg, frames, seconds and result with a total line, also in RUN_WORK/GAME-chain.tsv
              (without --chain too: a leg whose start save is not in the library but is made by an earlier leg of the run, continue-b2
              after new-b2-save, waits for it and starts from the save the run made)
--repeat N    each leg that passes runs N more times, from the same start, each exactly like the first run (a repeat takes no
              screenshot of its own: it never changes the run); prints "repeat xK SAME ..." or "DIFFERENT field values of each
              run" for the verdict, the end position, the end frame and the save bytes (the run's save.bin). With --shot the
              screen hash is printed too, as info only (animated screens make it flaky). The exit code is 1 for a DIFFERENT.
              (Legs with battles are not pinned: docs/port-worker.md)
--skip N      the picture one frame in N (default 8; a leg flagged `noskip` stays at 1). Linux only
--gpu         GALLIUM_DRIVER=d3d12: the game draws on the GPU through WSL's D3D12 (the default under Xvfb is llvmpipe, on the CPU). Linux only.
              11-13: the same wall time as llvmpipe (the 7 legs aspc-alder..gym-cheren at -j 1: 25-29 s both ways, -j 2: 15 s) with
              ~30% less CPU, ~0.5 s more start-up a leg; the 3x of 11-08 (71 s against 24 s) did not come back, with or without gdb
add           `route.py GAME add "name | start | tokens | end-or-? | save" [--desc TEXT] [--comment LINE] [--from-file SPEC]`: the leg written to
              the route file, learned, repeated twice, its save + README line stored when SAME; at most ~8 lines printed
              (tools/route_add.py, docs/route-writer.md)
state         `route.py GAME state SAVE`: what a save holds (zone + tile, badges, money, party, bag, key items/HMs, story counts), numbers
              only; `route.py GAME diff SAVE_A SAVE_B`: what changed (tools/route_state.py, docs/route-writer.md)
ROUTE_TRACE=1 (env, with -j 1) a table of each leg's phases and their seconds (root copy, Xvfb, gdb, exe, first frame, the
              field up, the pilot's end, teardown) after its line: where a slow leg's time went
--remote [M]  (any command above: look, add, legs, --chain) the same run on a free slot of another machine M (auto, laptop, bd,
              local) with this worktree's uncommitted changes; its output, route-file edits, work files, saves and README lines come
              back here, and a last line names the machine (tools/route_remote.py, docs/route-writer.md)
--store       keep a leg's produced save in the saves library, under a NEW name only; --replace NAME may overwrite
              NAME when it is a b2-* or w2-* test save. Nothing else is ever overwritten (n-before.bin never)
--shot N      a screenshot at V-blank N (also hashed in the line); N,N,...: one each, at least 9 frames apart (RUN_WORK/GAME-LEG-N.png)
--list        the legs
--no-cheat    the test cheat off. It is ON by default (BWPORT_TESTCHEAT=win: battles play in full, the player moves first,
              one-shots and takes no damage; docs/port-runtime.md "Test cheat"): off only for a test of a battle itself
--no-fast-opts  the game's own speeds. ON by default: the speed options (FAST_OPTS: Instant text and transitions, walk,
              run and battle speed 4x; BWPORT_CFGSCRIPT) except for a leg flagged `defaultopts`
--map         a map dump at the leg's end (or where it got stuck), printed after its line: the walkable grid around the
              player, warps, triggers, actors (port/src/route_map.c's header has the legend; BWPORT_MAPDUMP). The files stay
              in RUN_WORK/GAME-LEG-map/. The dump is short (about 60 lines: the grid around the player and the G targets,
              unloaded rows as one line, the entries in the window; the rest on a line of ids): --full-map the whole 64 x 64 and every entry
              (BWPORT_MAPDUMP_FULL=1). Walk with the pilot's G tokens: G:X,Z (a planned path to a tile, actors and walls
              avoided, across zone edges) and G:warp:N (to the warp leading to zone N, or the edge where the map goes on into
              zone N), and to what the dump lists: G:npc:UID (next to the actor, facing it, A, its event to the end; :nota
              no A), G:wall:UID (through a pass-through actor, Castelia Gym's honey walls: the cell before it, the key held until 2 past it; "wall N closed" when it does not open), G:item:UID, G:trig:N (onto trigger TN), G:bg:N (facing BG event bN from the side its direction reads it from, A); on a rail map (the dump's
              rail[line,front,side] places) the same tokens plan on the rail, and G:rail:L,F,S walks onto a rail place (a hybrid zone, grid and rails in one: the nearest hand-off, docs/port-worker.md); Md in
              the tokens dumps there too
G:zone:N      in a leg's tokens: expanded before the run into the G:warp:Z hops of the atlas path (tools/atlas.py GAME path A N,
              from the ROM: warps and the edges where the map goes on) from the zone the player is in to zone N; the expansion is
              in the leg's log. A scripted door (a script's MapChange*: tools/story.py GAME who-warps N) is a hop too: the token
              that runs it, G:npc:UID (talk), G:trig:N Wf (step on it) or G:bg:N; its conditions (work, flags) are in the log
              and are not checked: a closed one ends the leg off-route or stuck. The zone before a token: G:zone:A-N says it (an explicit start), else where the last G:zone, G:warp
              or Wz token of the leg left the player, else the start save's zone, read by a one-frame run from the save (NEWGAME
              has none: write A-N). After any other movement token (a door, a walk) write A-N.
--learn       a leg whose script finished but ended elsewhere gets the position it reached as its `end` in the file (a new leg:
              write `zone 0 0,0`, run, read the line, check it is where the keys should end)
"""
import argparse
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
import threading
import time
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LINUX = sys.platform.startswith("linux")   # Linux: each leg runs through tools/port_run.py (Xvfb; --no-gdb), saves in ~/saves
if LINUX:
    SAVES = Path(os.environ.get("BWPORT_SAVES", str(Path.home() / "saves")))
    CACHE = Path(os.environ.get("ROUTE_CACHE", os.environ.get("BWPORT_CACHE", str(ROOT / "build/route-cache"))))
else:
    SAVES = Path(os.environ.get("BWPORT_SAVES", str(Path.home() / "bw-saves")))
    CACHE = Path(os.environ.get("BWPORT_CACHE", str(Path.home() / "bw-cache")))
WORK = Path(os.environ.get("ROUTE_WORK", str(CACHE / "route-work")))
ORIG = Path(os.environ.get("BWPORT_ORIG", str(ROOT / "orig")))  # the ROMs: this checkout's orig/
GAMES = {  # code, game folder in a launcher root, ROM
    "b": ("IRBO", "Pokemon Black", "baserom_b_us.nds"),
    "w": ("IRAO", "Pokemon White", "baserom_w_us.nds"),
    "b2": ("IREO", "Pokemon Black 2", "baserom_b2_us.nds"),
    "w2": ("IRDO", "Pokemon White 2", "baserom_w2_us.nds"),
    # Gen 4 (docs/platinum.md, docs/hgss.md): from the default build like the others (port_run.GAME_BUILD; BWPORT_PLAT_BUILD,
    # BWPORT_HG_BUILD or BWPORT_BUILD name another dir). `all` stays the four above
    "pt": ("CPUE", "Pokemon Platinum", "baserom_plat_us.nds"),
    "hg": ("IPKE", "Pokemon HeartGold", "baserom_hg_us.nds"),
    "ss": ("IPGE", "Pokemon SoulSilver", "baserom_ss_us.nds"),
}
GEN5 = ("b", "w", "b2", "w2")   # `route.py all`
PORT_RUN_GAME = {"b": "black", "w": "white", "pt": "platinum", "hg": "heartgold", "ss": "soulsilver"}   # port_run.py --game
# A leg from a save: the pilot tokens that come before the leg's own. Black / White skip the title by themselves (a --save run
# starts at the field, ~f220); Platinum passes it as a player does: the opening to the title (pinned: the same V-blanks each run),
# START, A on CONTINUE (tools/plays/platinum/room-load's keys), then the field free. So no leg in tools/routes/pt.txt starts with it.
# HeartGold / SoulSilver the same way (tools/plays/heartgold/room-load's keys: START, A on CONTINUE, A twice through the save's
# summary), then nothing until the field is up (~V-blank 10500: the pulse key of a Wf during the load could reach the menus).
_HG_START = "K0:7700 Ks:10 K0:590 Ka:10 K0:390 Ka:10 K0:390 Ka:10 K0:1400 Wf"
SAVE_START = {"pt": "K0:6300 Ks:10 K0:390 Ka:10 Wf", "hg": _HG_START, "ss": _HG_START}
CLOCK = "2026-10-29T12:00:00"   # docs/port-worker.md: --pin's
SEED_FRAME = "100"
MAC_ADDR = "98e255000001"   # the DS MAC the game keeps in the save (block 0x25): fixed (pin_mac)
PROTECTED = {"n-before"}
# the speed options a leg runs with (docs/speed-options.md; --no-fast-opts or the leg flag `defaultopts`: the game's own)
FAST_OPTS = "f0:textspeed=3;f0:fadespeed=3;f0:walkspeed=4;f0:runspeed=4;f0:battlespeed=4"


def game_build(game):
    """port_run with GAME's build dir (Gen 4: its module build) chosen: the module"""
    if str(ROOT / "tools") not in sys.path:
        sys.path.insert(0, str(ROOT / "tools"))
    import port_run
    port_run.use_game_build(PORT_RUN_GAME.get(game, game))
    return port_run


def ensure_exe(game="b"):
    """Linux: the port's exe up to date before the legs run (a fresh worktree has none: tools/setup_worktree.sh first when
    configure.py never ran there, then port_run.py's build: meson setup with ccache, ninja). One line when it does work.
    GAME's build: port_run.GAME_BUILD (a build dir made with a Gen 4 module off is configured on: minutes cold)"""
    port_run = game_build(game)
    t0 = time.time()
    exe = Path(port_run.EXE)
    if not (ROOT / "config/w_us/assets.tsv").exists():
        print("route: a fresh worktree: bash tools/setup_worktree.sh (about a minute)", flush=True)
        r = subprocess.run(["bash", "tools/setup_worktree.sh"], cwd=ROOT, stdin=subprocess.DEVNULL, capture_output=True, text=True)
        if r.returncode:
            sys.exit(f"route: tools/setup_worktree.sh failed:\n{r.stdout[-2000:]}{r.stderr[-2000:]}")
    if not exe.exists():
        print(f"route: no {exe.relative_to(ROOT)}: building the port (meson setup, ninja; ccache: a few minutes cold, "
              "well under one warm)", flush=True)
    err = port_run.build()
    if err:
        sys.exit(f"route: the port's build failed:\n{err[-3000:]}")
    if time.time() - t0 > 3:
        print(f"route: port built in {time.time() - t0:.0f} s", flush=True)


def expand_repeats(pilot):
    """(TOKENS)*N in a leg's pilot field: the tokens N times (read_legs)"""
    return re.sub(r"\(([^()]*)\)\*(\d+)", lambda m: " ".join([m.group(1).strip()] * int(m.group(2))), pilot)


def read_legs(game):
    legs = []
    routes = Path(os.environ.get("BWPORT_ROUTES_DIR") or ROOT / "tools/routes")   # route_add.py's look: a temporary one-leg file
    for n, line in enumerate((routes / f"{game}.txt").read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        f = [x.strip() for x in line.split(" | ")]
        if len(f) < 4:
            sys.exit(f"{game}.txt:{n}: need name | start | pilot | end")
        f += ["-", ""][len(f) - 4:]
        legs.append({"name": f[0], "start": f[1], "pilot": expand_repeats(f[2]), "end": f[3], "save": f[4], "flags": f[5].split()})
    return legs


def base_root(game):
    code, folder, rom = GAMES[game]
    base = CACHE / f"routebase-{game}"
    if (base / folder / "header.bin").exists():
        return base
    if LINUX:   # the launcher's own importrom step (port_run.py's), no window
        # into a directory of this process's own, renamed into place when complete: several route.py processes started at
        # once (a sweep) all imported into the one base and the files they wrote over each other's broke the ROM's data
        # (Black's legs then stopped at boot, a heap allocation of a garbage size in msgdata.c; 2026-11-13)
        tmp = CACHE / f"routebase-{game}.tmp{os.getpid()}"
        shutil.rmtree(tmp, ignore_errors=True)
        tmp.mkdir(parents=True)
        env = dict(os.environ, BWPORT_LCTEST=f"importrom={ORIG / rom}", SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
        subprocess.run([game_build(game).EXE], cwd=tmp, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if not (tmp / folder / "header.bin").exists():
            shutil.rmtree(tmp, ignore_errors=True)
            sys.exit(f"route: the base root {base} has no {folder}: the ROM import failed")
        try:
            os.rename(tmp, base)   # fails when another process's base got there first (or an old incomplete one is there)
        except OSError:
            shutil.rmtree(tmp, ignore_errors=True)
            if not (base / folder / "header.bin").exists():
                sys.exit(f"route: {base} is there without {folder}/header.bin: remove it and run again")
        return base
    base.mkdir(parents=True, exist_ok=True)
    (base / "launcher.ini").write_text(f"LastGame={code}\nStart=last\n")
    env = dict(os.environ, BWPORT_ROM=str(ORIG / rom))
    # the first run imports the ROM and stops in the launcher: a few seconds, then it is killed
    subprocess.run([str(ROOT / "tools/mac_run.sh"), "--root", str(base), "40", "", "f60"], env=env,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    for p in list(base.glob("*/f60.png")) + list(base.glob("*/save.bin")) + [base / "log.txt"]:
        p.unlink(missing_ok=True)
    if not (base / folder / "header.bin").exists():
        sys.exit(f"route: the base root {base} has no {folder}: the ROM import failed")
    return base


GAME_WIDTH = {True: 512, False: 1016}   # LINUX: the Xvfb window is 992 wide, the two DS screens the left 512 (scale 2), the panel right of it


def game_width():
    return GAME_WIDTH[LINUX]


def game_hash(path, width=None):
    """A hash of the game's picture in a hidden run's screenshot: the left `width` pixels of every row (the debug panel and its
    changing statistics are on the right; default: the platform's game_width()). PNG rows only filter from the left and the row
    above, so each stops at `width`."""
    width = width or game_width()
    d = path.read_bytes()
    pos, idat, w, h, bpp = 8, b"", 0, 0, 4
    while pos < len(d):
        n, t = struct.unpack(">I4s", d[pos:pos + 8])
        if t == b"IHDR":
            w, h, _, ct = struct.unpack(">IIBB", d[pos + 8:pos + 18])
            bpp = {6: 4, 2: 3}[ct]
        elif t == b"IDAT":
            idat += d[pos + 8:pos + 8 + n]
        pos += 12 + n
    raw = zlib.decompress(idat)
    stride, keep = w * bpp, min(width, w) * bpp
    prev, md = bytearray(keep), hashlib.sha1()
    for y in range(h):
        f = raw[y * (stride + 1)]
        line = bytearray(raw[y * (stride + 1) + 1:y * (stride + 1) + 1 + keep])
        for i in range(keep):
            a = line[i - bpp] if i >= bpp else 0
            b = prev[i]
            if f == 1:
                line[i] = (line[i] + a) & 255
            elif f == 2:
                line[i] = (line[i] + b) & 255
            elif f == 3:
                line[i] = (line[i] + ((a + b) >> 1)) & 255
            elif f == 4:
                c = prev[i - bpp] if i >= bpp else 0
                pp = a + b - c
                pa, pb, pc = abs(pp - a), abs(pp - b), abs(pp - c)
                line[i] = (line[i] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        md.update(line)
        prev = line
    return md.hexdigest()[:10]


def compare_runs(runs):
    """(True, "") when every run of a leg ended alike (--repeat): the same verdict, end position, end frame (V-blank) and save
    bytes (a hash of the run's save.bin); else (False, what differs: the field and its value in each run). A field a run has no
    value for is left out of the comparison. The screen hash (png, with --shot) is not compared: animated screens make it
    flaky (png_info prints it)."""
    diffs = []
    for field in ("verdict", "pos", "v", "save"):
        if len({r.get(field) for r in runs if r.get(field) is not None}) > 1:
            diffs.append(f"{field} " + " / ".join(str(r.get(field, "-")) for r in runs))
    return not diffs, "; ".join(diffs)


def png_info(runs):
    """the screen hashes of --repeat's runs for its line (only with --shot), as info: they are not compared"""
    pngs = [r["png"] for r in runs if r.get("png")]
    if not pngs:
        return ""
    return f" png={pngs[0]}" if len(set(pngs)) == 1 else " png=" + "/".join(pngs) + " (info only)"


def chain_table(rows):
    """--chain's report: a header, a line a leg (name, frames, seconds, result) and a total, tab separated.
    rows: (leg, frames or None, seconds, verdict)"""
    lines = ["leg\tframes\tseconds\tresult"]
    lines += [f"{n}\t{'-' if v is None else v}\t{s:.1f}\t{r}" for n, v, s, r in rows]
    frames, secs = sum(v or 0 for _, v, _, _ in rows), sum(s for _, _, s, _ in rows)
    lines.append(f"total\t{frames}\t{secs:.1f}\t{sum(r in PASS for *_, r in rows)}/{len(rows)} ok")
    return "\n".join(lines) + "\n"


TRACE = os.environ.get("ROUTE_TRACE") == "1"
TRACES = []


def trace(what):
    """ROUTE_TRACE=1: note a phase's monotonic time (print_trace prints them with port_run.py's)"""
    if TRACE:
        TRACES.append((time.monotonic(), what))


def print_trace(log):
    """a leg's phases, each with the seconds since the one before: route.py's and port_run.py's (its `trace:` lines
    in the leg's log)"""
    rows = list(TRACES)
    TRACES.clear()
    for m in re.finditer(r"^trace: ([\d.]+) (.*)$", log.read_text(errors="replace"), re.M):
        rows.append((float(m.group(1)), m.group(2)))
    rows.sort()
    for i, (t, what) in enumerate(rows):
        print(f"  trace {t - rows[0][0]:8.2f} s  +{t - rows[max(i - 1, 0)][0]:7.2f}  {what}")


def pin_mac(run):
    """MacAddr= in the run root's sim_config.ini (the base root's has none: libntr adds a random one at a root's first
    launch, which the game keeps in the save, so two runs from one save would end with different save bytes)"""
    ini = run / "sim_config.ini"
    if not ini.exists():
        return
    text = ini.read_text()
    if re.search(r"^MacAddr=", text, re.M):
        text = re.sub(r"^MacAddr=.*$", f"MacAddr={MAC_ADDR}", text, flags=re.M)
    else:
        text += f"\n[General]\nMacAddr={MAC_ADDR}\n"
    ini.write_text(text)


LOOK = "zone 0 0,0"   # a look leg's end: nothing to match, the leg is there for its map dump
PASS = ("ok", "look")   # the verdicts that count as a pass (exit 0)
DONE = re.compile(r"^pilot: (script done|end token)", re.M)
# the exe's open-file guard (port/src/run_frames.c): `openfiles: now N peak P` at the end of a run, `openfiles: leak: ...` once when
# more than its bound of host files were open at once (the games keep ~120 NARCs open; a file opened and never closed grows)
OPENFILES = re.compile(r"^openfiles: now (\d+) peak (\d+)", re.M)
OPENFILES_LEAK = re.compile(r"^openfiles: leak: (.*)$", re.M)
ZONE_TOKEN = re.compile(r"G:zone:(?:(\d+)-)?(\d+)")   # G:zone:N, or G:zone:A-N with the start A
ZONE_NOW = re.compile(r"(?:G:warp|Wz):(\d+)")   # the tokens that leave the player in zone N
ATLAS_LOCK = threading.Lock()   # the atlas JSON is built on first use: one thread builds it
# Waypoints a zone hop needs: (game, zone, next zone) -> G tokens walked before the hop's G:warp. The planner's far hop (the
# reachable tile nearest the door) loops where the way goes AWAY from the door first: White 2's Reversal Mountain interior
# (539 -> 542: south along the corridor, then east; Black 2's 533 -> 536 has no such detour). Found from the Md dumps.
# (Black 2 / White 2's frozen Giant Chasm crater, 233 -> 234, needed waypoints until pz19: its far chunks were not loaded and
# the planner guessed through '?'; the adapter now reads unloaded chunks' terrain from the ROM and a plain G:warp:234 walks it.)
WAYPOINTS = {("w2", 539, 542): "G:7,52 G:40,57"}


def expand_zones(game, pilot, start_zone, data=None):
    """pilot with each G:zone:N token replaced by the G:warp:Z hops of the atlas path (tools/atlas.py GAME path A N: warps and
    matrix edges alike, the pilot's G:warp walks to either; a scripted door's hop is its runner's token, a trigger's with Wf) from the zone the player is in to zone N. That zone is the token's own
    start (G:zone:A-N), else where the last G:zone, G:warp or Wz token left the player, else start_zone() (the start save's zone,
    asked at most once). Returns (pilot, notes: a line per token for the leg's log). data: the atlas (default: atlas.load(game)).
    Raises ValueError: no start zone, no path."""
    import atlas
    toks, notes, here = [], [], None
    for tok in pilot.split():
        m = ZONE_TOKEN.fullmatch(tok)
        if not m:
            toks.append(tok)
            if ZONE_NOW.fullmatch(tok):
                here = int(tok.rsplit(":", 1)[1])
            continue
        a = int(m.group(1)) if m.group(1) else here if here is not None else start_zone()
        b = int(m.group(2))
        if data is None:
            with ATLAS_LOCK:
                data = atlas.load(game)
        steps = atlas.path(data, a, b)
        if steps is None:
            raise ValueError(f"{tok}: no atlas path from zone {a} to zone {b}")
        hops = []
        for z0, how, z in steps:
            tok = how.split()[1].rstrip(":") if how.startswith("script ") else f"G:warp:{z}"
            hops += WAYPOINTS.get((game, z0, z), "").split()
            hops.append(tok + " Wf" if tok.startswith("G:trig:") else tok)
        toks += hops
        notes.append(f"{tok} from zone {a}: {' '.join(hops) or '(already there)'}"
                     + "".join(f"\n    {x} -> {z}: {how}" for x, how, z in steps))
        here = b
    return " ".join(toks), notes


def warp_hints(game, pilot, data=None):
    """BWPORT_WARP_HINTS for the pilot's G:warp:Z tokens: "Z=S:x,z;..." the tile of each warp into Z that zone S's event data
    holds (tools/atlas.py; outdoors the tiles are the world's). The planner (port/src/route_map.c, WarpHint) walks toward the
    nearest one, using only those where the map matrix says zone S, when no warp to Z is in its window and no chunk of Z is
    on the matrix (an interior zone). "" when the pilot has no G:warp token. data: the atlas (default: atlas.load(game))."""
    zones = {int(z) for z in re.findall(r"(?:^|\s)G:warp:(\d+)(?=\s|$)", pilot)}
    if not zones:
        return ""
    if data is None:
        import atlas
        with ATLAS_LOCK:
            data = atlas.load(game)
    return ";".join(f"{w['to']}={s}:{w['x']},{w['z']}" for s, z in enumerate(data["zones"]) for w in z["warps"]
                    if w["to"] in zones and "rail" not in w and not w.get("dead"))


def zone_leg(game, leg, args, produced):
    """leg with its G:zone tokens expanded (leg["zones"]: the notes for the leg's log), or the leg itself when it has none.
    The start zone of a token without its own start is read off the start save by a one-frame run from it. Raises ValueError"""
    if "G:zone:" not in leg["pilot"]:
        return leg

    def start_zone():
        if leg["start"] == "NEWGAME":
            raise ValueError("G:zone: NEWGAME has no zone: write G:zone:A-N (the start A)")
        probe = dict(leg, name="zone-" + leg["name"], pilot="W1", end=LOOK, save="-", flags=[])
        pst = {}
        verdict, where, _ = run_leg(game, probe, args, produced, pst)
        shutil.rmtree(WORK / f"{game}-{probe['name']}-map", ignore_errors=True)
        m = re.match(r"zone (\d+) ", (pst.get("pos") or "") + " ")
        if not m:
            raise ValueError(f"G:zone: the zone of save {leg['start']} is unknown ({verdict} {where}): write G:zone:A-N (the start A)")
        return int(m.group(1))

    pilot, notes = expand_zones(game, leg["pilot"], start_zone)
    return dict(leg, pilot=pilot, zones=notes)


def judge(leg, text, pr_verdict, pos, saved, dumped):
    """judge_run's verdict, except that a leg flagged `expect-hang` (Platinum's new-look: the intro's stylus-only ball) passes as a
    look when it ends in a hang or timeout; a crash, an off-route end or any other verdict still fails it."""
    verdict, note = judge_run(leg, text, pr_verdict, pos, saved, dumped)
    if "expect-hang" in leg["flags"] and verdict in ("hang", "timeout"):
        return "look", "expected " + verdict + (": " + note if note else "")
    return verdict, note


def judge_run(leg, text, pr_verdict, pos, saved, dumped):
    """(verdict, note) of a finished run (the note goes in parentheses after the position). text: the game's log; pr_verdict:
    port_run.py's (crash, stub, hang in F); pos: the last position; saved: save.bin changed; dumped: the run left a map dump"""
    done = DONE.search(text)
    if "CRASH" in text:
        return "crash", ""
    if pr_verdict.startswith(("crash", "stub", "stopped", "heapcheck", "openfiles")):   # the game died (gdb's stop; BWPORT_HEAPCHECK's): say where, not "hang"
        return "crash", pr_verdict
    hang = re.search(r"^pilot: hang: (.*?) at V-blank", text, re.M)
    if hang:   # the pilot's own no-progress check (route_test.c)
        return "hang", hang.group(1)
    if not done and pr_verdict.startswith("hang"):   # no game frame for --hang seconds (port_run.py)
        return "hang", pr_verdict
    if "pilot: stuck" in text:
        return "stuck", ""
    fail = re.search(r"^pilot: (timeout: .*|save: .*)$", text, re.M)
    if fail and not done:   # an event wait or the save that did not finish (route_test.c: BWPORT_PILOT_TIMEOUT)
        return ("timeout" if fail.group(1).startswith("timeout") else "save-failed"), fail.group(1)
    if not done:
        if pr_verdict.startswith("reached-frame"):   # the wall cap (-t) ended a run whose frames were still advancing: a slow host, not a hang
            return "hang", "wall cap, frames still advancing: " + pr_verdict
        return "hang", ""
    leak = OPENFILES_LEAK.search(text)
    if leak:   # a file opened and never closed (EMFILE at ulimit -n on Linux, the CRT's 512 on win64): the leg got there, the build is not ok
        return "leak", leak.group(1)
    if pos != leg["end"]:
        if leg["end"] == LOOK and dumped:   # a look leg ends anywhere: its product is the map dump
            return "look", ""
        return "off-route", f"want {leg['end']}"
    if leg["save"] != "-" and not saved:
        return "save-failed", "save.bin unchanged"
    return "ok", ""


def again_under_gdb(verdict, text):
    """a verdict that wants gdb's backtrace: a crash, or a hang the pilot did not name itself (route_test.c's `pilot: hang:`)"""
    return verdict == "crash" or (verdict == "hang" and not re.search(r"^pilot: hang: ", text, re.M))


def save_waits(jobs, library):
    """{(game, leg name): the save it starts from} for each leg of the run whose start save is not in the library (the set of
    names in $BWPORT_SAVES) but is made by a leg before it in the same run (a leg a producer's earlier in the run, as
    continue-b2 is for new-b2-save's b2-newsave): such a leg waits for that leg and starts from the save the run made.
    Only an earlier leg counts: the pool starts the legs in order, so a wait never blocks a worker its producer needs"""
    made, waits = set(), {}
    for g, leg in jobs:
        start = leg["start"]
        if start != "NEWGAME" and start in made and start not in library:
            waits[(g, leg["name"])] = start
        if leg["save"] != "-":
            made.add(leg["save"])
    return waits


def run_leg(game, leg, args, produced, st=None, tag="", under_gdb=False):
    """Run one leg: (verdict, where, produced save or None). st (a dict) gets the numbers --repeat and --chain compare and
    report: secs, v (the end V-blank), pos, png, save (a hash of the run's save.bin). tag names the files of a repeat's run
    apart: a repeat is the same run (the same cfgscript, no screenshot of its own, --shot's if given). Linux: the exe runs
    itself (port_run.py --no-gdb: gdb's polling costs CPU); a leg that crashed or hung runs once more under gdb (under_gdb:
    --gdb does that for every leg), whose stop says where"""
    st = {} if st is None else st
    shot = args.shot
    code, folder, rom = GAMES[game]
    name = leg["name"] + tag
    # the run's directory has the same name for every run of a leg, --repeat's too (only the log, save and map files carry the
    # tag). Until 2026-11-13 the game's behaviour depended on the length of the strings in its environment and on the build's
    # layout (the leg to-floccesy ended at another V-blank when its directory name was 3 characters longer): buildSeed hashed
    # 20 bytes of host stack, pointers among them (port/extern/lib/gfl/src/gf_standard.c now zeroes them). The same name
    # stays: a cheap guard should another such read turn up
    run = WORK / f"run-{game}-{leg['name']}"
    log = WORK / f"{game}-{name}.log"
    WORK.mkdir(parents=True, exist_ok=True)
    shutil.rmtree(run, ignore_errors=True)
    trace("leg start")
    subprocess.run(["cp", "-a" if LINUX else "-cR", str(base_root(game)), str(run)], check=True)
    trace("root copied")
    pin_mac(run)
    start = leg["start"]
    saves = SAVES
    cmd = [str(ROOT / "tools/mac_run.sh")]
    if LINUX:
        cmd = [sys.executable, str(ROOT / "tools/port_run.py"), "--no-build", "--game", PORT_RUN_GAME.get(game, game), "--dir", str(run), "--out", str(run / "out"),
               "--shots", "0", "--skip", "1" if "noskip" in leg["flags"] else str(args.skip), "-t", str(args.cap), "--hang", str(args.hang), "--hang-stop"]
        if "realtime" in leg["flags"]:
            cmd.append("--realtime")
        if not (under_gdb or getattr(args, "gdb", False) or os.environ.get("ROUTE_GDB")):
            cmd.append("--no-gdb")
        # ROUTE_GDB="source FILE;;break F" (env): gdb commands before the run (port_run.py --gdb); a leg then runs under gdb (ptleg7)
        for g in filter(None, os.environ.get("ROUTE_GDB", "").split(";;")):
            cmd += ["--gdb", g]
    if not LINUX:   # mac_run.sh's own options (port_run.py has --skip N and --realtime, set above: it rejects --noskip)
        if "noskip" in leg["flags"]:
            cmd.append("--noskip")
        if "realtime" in leg["flags"]:
            cmd.append("--realtime")
        cmd.append("--root")
    if start != "NEWGAME":
        src = produced.get(start) if args.chain or leg.get("dep") else None
        if src is not None:   # this run made it: put it where mac_run.sh looks
            tmp = WORK / "chain-saves"
            tmp.mkdir(exist_ok=True)
            shutil.copy(src, tmp / f"{start}.bin")
            saves = tmp
        elif not (SAVES / f"{start}.bin").exists():
            return "no-save", f"no save {start}", None
        cmd += ["--save", str(saves / f"{start}.bin") if LINUX else start]
    if not LINUX:
        cmd += [str(run), str(args.cap)]
    pilot = f"{SAVE_START[game]} {leg['pilot']}" if start != "NEWGAME" and game in SAVE_START else leg["pilot"]
    env = dict(os.environ, BWPORT_SAVES=str(saves), BWPORT_PILOT=pilot, BWPORT_POSLOG="1", BWPORT_MUSICNORMAL="off",
               BWPORT_BENCH_CLOCK=CLOCK, BWPORT_BENCH_SEED=SEED_FRAME, BWPORT_ROM=str(ORIG / rom))
    # llvmpipe's raster threads: 2, not one per core (8 parallel legs x 16 threads): 8 alternating runs of the same 8 legs, user+sys
    # CPU 80.8 s (sd 4) against 93.6 s (sd 22), the same verdicts; set LP_NUM_THREADS to override. (An environment string of
    # another length no longer moves a leg's end V-blank: see "the run's directory" above.)
    env.setdefault("LP_NUM_THREADS", "2")
    # Max (ff=1) still presents up to 60 pictures a second (libntr-ios-pace.patch: at most one a display refresh), and each is
    # a full draw and swap on the frame loop's thread: ~4.4 ms on llvmpipe + Xvfb, so a third of a leg's wall time. Nobody
    # watches a leg (its shots force their frames, the pilot _exit()s), so Max presents 10 a second (BWPORT_PANELHZ: the panel
    # rate Max's present slots follow; at 100 ms a refresh no other pacing path takes it as a display link). The 7 legs
    # aspc-alder..gym-cheren, -j 1, alternating runs: 28/34 -> 22/24 s on a quiet pool, 82/68 -> 40/27 s at load ~20, the same
    # end V-blanks; --gpu 28/29 -> 25/27 s. Realtime legs keep their pace (60 fps capped: not Max). Set it to override.
    if LINUX and "realtime" not in leg["flags"]:
        env.setdefault("BWPORT_PANELHZ", "10")
    env.pop("BWPORT_WARP_HINTS", None)
    hints = warp_hints(game, leg["pilot"])
    if hints:
        env["BWPORT_WARP_HINTS"] = hints
    env.pop("BWPORT_NOENCOUNTER", None)
    if "encounters" not in leg["flags"]:
        env["BWPORT_NOENCOUNTER"] = "1"
    cfg = []
    if args.fast_opts and "defaultopts" not in leg["flags"]:
        cfg.append(FAST_OPTS + (";f0:surfspeed=4" if game in ("pt", "hg", "ss") else ""))   # Gen 4: Surf 4x too (spd4, 2026-11-19)
    if shot:
        # Max does not draw the shot frame itself (a stale picture): leave Max for the frames around the shot. Several
        # (--shot 2200,2400): route.png for one, routeN.png for each (the frames at least 9 apart: the Max steps between)
        shots = sorted({int(x) for x in str(shot).split(",") if x})
        nm = {s: "route" if len(shots) == 1 else f"route{s}" for s in shots}
        if "realtime" in leg["flags"]:
            cfg.append(";".join(f"f{s}:shot={nm[s]}" for s in shots))
        else:
            cfg.append("f0:ff=1;" + ";".join(f"f{max(s - 4, 1)}:ff=0;f{s}:shot={nm[s]};f{s + 4}:ff=1" for s in shots))
    elif LINUX and "realtime" not in leg["flags"]:
        cfg.insert(0, "f0:ff=1")   # Max from frame 0 (mac_run.sh adds it itself)
    env.pop("BWPORT_CFGSCRIPT", None)
    if cfg:
        env["BWPORT_CFGSCRIPT"] = ";".join(cfg)
    env["BWPORT_TESTCHEAT"] = "win" if args.cheat else ""
    if args.gpu:
        env["GALLIUM_DRIVER"] = "d3d12"   # the game's GL on the GPU through WSL's D3D12 (the default under Xvfb: llvmpipe, on the CPU)
    # every run has a map dump at its end (and at an Md token, a hang, no path): RUN_WORK/GAME-LEG-map/, kept below unless the leg is ok
    mapdir = WORK / f"{game}-{name}-map"
    envmap = WORK / f"{game}-{leg['name']}-map"   # what the game's environment says: the same for every run of a leg (see run above)
    shutil.rmtree(envmap, ignore_errors=True)
    envmap.mkdir(parents=True)
    env["BWPORT_MAPDUMP"] = str(envmap)
    env.pop("BWPORT_MAPDUMP_FULL", None)
    if getattr(args, "full_map", False):
        env["BWPORT_MAPDUMP_FULL"] = "1"
    t0 = time.time()
    with open(log, "w") as lf:
        subprocess.run(cmd, env=env, stdout=lf, stderr=subprocess.STDOUT)
    secs = time.time() - t0
    trace("port_run returned")
    if envmap != mapdir:
        shutil.rmtree(mapdir, ignore_errors=True)
        envmap.rename(mapdir)
    logf = run / "out/log.txt" if LINUX else run / "log.txt"
    text = logf.read_text(errors="replace") if logf.exists() else ""
    if logf.exists():   # the game's whole log (and gdb's backtraces) stays: RUN_WORK/GAME-LEG.game.log
        shutil.copy(logf, WORK / f"{game}-{name}.game.log")
    vfile = run / "out/verdict.txt"
    pr_verdict = vfile.read_text().strip() if LINUX and vfile.exists() else ""   # port_run.py's: crash, stub, hang in F
    with open(log, "a") as lf:
        if leg.get("zones"):   # G:zone tokens, expanded by zone_leg before the run
            lf.write("\n---- G:zone expanded (tools/atlas.py path) ----\n" + "\n".join(leg["zones"]) + "\n")
        lf.write("\n---- the game's log (pilot, pos, crash lines) ----\n")
        lf.writelines(l + "\n" for l in text.splitlines() if re.match(r"(pilot:|pos:|trace:|bwport: (field|NOENC|benchpin|wild|save state)|.*CRASH)", l))
    pos = None
    for m in re.finditer(r"^pos: (\S+) Z zone (\d+) (-?\d+),(-?\d+)", text, re.M):
        pos = f"zone {m.group(2)} {m.group(3)},{m.group(4)}"
    done = DONE.search(text)
    save_bin = run / folder / "save.bin"
    vb = re.search(r"^pilot: (?:script done|end token)[^\n]* at V-blank (\d+)", text, re.M)
    wild = len(re.findall(r"^bwport: wild encounter ", text, re.M))
    # every line ends with the wall seconds and the game frames (V-blanks since the boot: a leg from a save starts at the load's)
    extra = f" {secs:.0f}s" + (f" wild={wild}" if wild else "")
    st.update(secs=secs, pos=pos, v=int(vb.group(1)) if vb else None, png=None, save=None)
    of = OPENFILES.search(text)
    st["openfiles"] = int(of.group(2)) if of else None   # the most host files the game had open at once
    if save_bin.exists():   # what --repeat compares: the save bytes the run ended with
        st["save"] = hashlib.sha1(save_bin.read_bytes()).hexdigest()[:10]
        shutil.copy(save_bin, WORK / f"{game}-{name}.save.bin")   # kept beside the log: diff two runs' saves
    if vb:
        extra += f" v={vb.group(1)}"
    if shot:
        pngs = list(run.glob("*/route.png")) + list(run.glob("route.png"))
        if pngs:
            shutil.copy(pngs[0], WORK / f"{game}-{name}.png")
            st["png"] = game_hash(pngs[0])
            extra += " png=" + st["png"]
        for p in sorted(run.glob("*/route[0-9]*.png")) + sorted(run.glob("route[0-9]*.png")):   # a list of shots: a file each
            shutil.copy(p, WORK / f"{game}-{name}-{p.stem[5:]}.png")
            extra += f" shot{p.stem[5:]}"
    out = None
    # a leg that makes a save must change save.bin (the start save was copied in; a save the keys missed leaves it as it was)
    saved = save_bin.exists() and (start == "NEWGAME" or save_bin.read_bytes() != (saves / f"{start}.bin").read_bytes())
    if leg["save"] != "-" and saved and pos == leg["end"] and done:
        out = WORK / f"{game}-{name}.save.bin"
        shutil.copy(save_bin, out)
    verdict, note = judge(leg, text, pr_verdict, pos, saved, any(mapdir.glob("mapdump-*.txt")))
    if verdict == "ok" and not args.map:
        shutil.rmtree(mapdir, ignore_errors=True)
    else:   # the dumps and the game's log stay in RUN_WORK/GAME-LEG-map/ (--map reads them), named on the line unless ok
        if logf.exists():
            shutil.copy(logf, mapdir / "game.log")
        if verdict != "ok":
            extra += f" map={mapdir}/"
    shutil.rmtree(run, ignore_errors=True)
    trace("run dir removed")
    if TRACE:
        print_trace(log)
    if verdict != "ok":
        out = None
    elif out is not None:
        produced[leg["save"]] = out
    where = f"{pos}" + (f" ({note})" if note else "") + extra
    if LINUX and not (under_gdb or getattr(args, "gdb", False)) and again_under_gdb(verdict, text):
        # a crash or a hang without gdb has no backtrace: the same leg once more under gdb (the same run: the exe's
        # environment and ASLR are gdb's), its stop says where. The first run's logs stay as .nogdb
        for f in (log, WORK / f"{game}-{name}.game.log"):
            if f.exists():
                f.replace(f.with_name(f.name.replace(".log", ".nogdb.log")))
        v2, where2, out2 = run_leg(game, leg, args, produced, st, tag, under_gdb=True)
        if again_under_gdb(v2, ""):
            return v2, where2, out2
        return verdict, where + f" (not reproduced under gdb: {v2})", None
    return verdict, where, out


def store(save, out, replace):
    if save in PROTECTED or not re.match(r"^[A-Za-z0-9_-]+$", save):
        return f"not stored: {save} is protected"
    dst = SAVES / f"{save}.bin"
    if dst.exists():
        if replace != save or not re.match(r"^(b2|w2)-", save):
            return f"not stored: {save}.bin exists (--replace {save} works for b2-* and w2-* only)"
    shutil.copy(out, dst)
    return f"stored {dst}: add its line to the README"


def arg_parser():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("game", choices=sorted(GAMES) + ["all"])
    ap.add_argument("legs", nargs="*")
    ap.add_argument("--store", action="store_true")
    ap.add_argument("--replace", default="")
    ap.add_argument("--chain", action="store_true")
    ap.add_argument("--quick", action="store_true")
    ap.add_argument("-j", type=int, default=max(1, (os.cpu_count() or 2) // 2))
    ap.add_argument("--shot", default="", help="a V-blank, or a comma list of them")
    ap.add_argument("--cap", type=int, default=600)
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--no-build", action="store_true", help="Linux: run the exe as it is (default: build it first)")
    ap.add_argument("--gdb", action="store_true",
                    help="Linux: every leg under gdb (the old way; default: the exe itself, and a leg that crashes or hangs "
                         "runs again under gdb for the backtrace)")
    ap.add_argument("--hang", type=int, default=20, metavar="SECS",
                    help="Linux: no game frame for SECS wall seconds ends a leg as a hang (port_run.py --hang-stop)")
    ap.add_argument("--cheat", action=argparse.BooleanOptionalAction, default=True,
                    help="BWPORT_TESTCHEAT=win (docs/port-runtime.md, Test cheat): on by default, --no-cheat turns it off")
    ap.add_argument("--fast-opts", action=argparse.BooleanOptionalAction, default=True,
                    help="the speed options (FAST_OPTS) for every leg not flagged defaultopts: on by default "
                         "(--no-fast-opts: the game's own speeds)")
    ap.add_argument("--learn", action="store_true")
    ap.add_argument("--map", action="store_true")
    ap.add_argument("--full-map", action="store_true")
    ap.add_argument("--repeat", type=int, default=0, metavar="N",
                    help="after its run each leg runs N more times, the same run (no screenshot of its own); prints SAME or "
                         "DIFFERENT (verdict, end position, end frame, save bytes)")
    ap.add_argument("--skip", type=int, default=8, metavar="N",
                    help="frame skip: the picture one frame in N (default 8; a leg flagged noskip stays at 1). Linux")
    ap.add_argument("--gpu", action="store_true",
                    help="GALLIUM_DRIVER=d3d12: the game draws on the GPU through WSL's D3D12 (default: llvmpipe). Linux")
    return ap


def main():
    if any(a == "--remote" or a.startswith("--remote=") for a in sys.argv[1:]):   # the run on another machine's slot: tools/route_remote.py
        import route_remote
        machine, rest = route_remote.strip_remote(sys.argv[1:])
        sys.exit(route_remote.main(rest, ROOT, SAVES, WORK, machine))
    if len(sys.argv) > 2 and sys.argv[1] in GAMES and sys.argv[2] == "add":   # one command a leg: tools/route_add.py
        import route_add
        sys.exit(route_add.main(sys.argv[1], sys.argv[3:], ROOT, SAVES, WORK, store))
    if len(sys.argv) > 3 and sys.argv[1] in GAMES and sys.argv[2] == "look":   # a look leg without the routes file: tools/route_add.py
        import route_add
        sys.exit(route_add.look(sys.argv[1], sys.argv[3:], ROOT, SAVES, WORK, store))
    if len(sys.argv) > 3 and (sys.argv[1] in GAMES or sys.argv[1] == "pt") and sys.argv[2] in ("state", "diff"):   # a save's contents: tools/route_state.py
        import route_state
        sys.exit(route_state.main(sys.argv[1], sys.argv[2:]))
    if sys.argv[1:2] == ["port"]:   # route.py port SRC DST ...: the version port (tools/route_port.py)
        import route_port
        sys.exit(route_port.main(sys.argv[2:]))
    args = arg_parser().parse_args()
    if not LINUX and (args.gpu or args.skip != 8):
        sys.exit("route: --gpu and --skip are for Linux (the Mac runs through tools/mac_run.sh)")
    games = sorted(GEN5) if args.game == "all" else [args.game]
    if args.game == "all" and (args.legs or args.learn or args.store or args.replace or args.chain):
        sys.exit("route: GAME all takes no legs, --learn, --store or --chain")
    jobs = []   # (game, leg)
    for g in games:
        legs = read_legs(g)
        if args.list:
            for l in legs:
                print(f"{l['name']:16} {l['start']:14} -> {l['end']:22} {l['save']:14} {' '.join(l['flags'])}")
            continue
        pick = [l for l in legs if (not args.legs or l["name"] in args.legs) and (not args.quick or "quick" in l["flags"])]
        unknown = set(args.legs) - {l["name"] for l in legs}
        if unknown:
            sys.exit(f"route: no such leg: {' '.join(sorted(unknown))}")
        jobs += [(g, l) for l in pick]
    if args.list:
        return
    if LINUX and not args.no_build:
        ensure_exe(games[0])
    for g in sorted({g for g, _ in jobs}):
        base_root(g)   # serial: the first run of a game imports its ROM
    produced, bad, t0 = {}, 0, time.time()
    summary, peaks = {}, []
    waits = save_waits(jobs, {p.stem for p in SAVES.glob("*.bin")})
    done = {name: threading.Event() for name in set(waits.values())}   # a producer's save is made (or the leg failed)

    def one(job):
        g, leg = job
        try:
            if (g, leg["name"]) in waits:
                done[waits[(g, leg["name"])]].wait()
                leg = dict(leg, dep=True)   # run_leg starts it from the save this run made
            return run_one(g, leg)
        finally:
            if leg["save"] in done:
                done[leg["save"]].set()

    def run_one(g, leg):
        st = {}
        try:
            leg = zone_leg(g, leg, args, produced)   # G:zone:N tokens, expanded once for every run of the leg
        except ValueError as e:
            verdict, where, out = "no-path", str(e), None
        else:
            verdict, where, out = run_leg(g, leg, args, produced, st)
        st["verdict"] = verdict
        note = ""
        if out is not None and (args.store or args.replace):
            note = " " + store(leg["save"], out, args.replace)
        if verdict in ("off-route", "look") and args.learn:
            f = ROOT / "tools/routes" / f"{g}.txt"
            got = re.split(r" (?:\d+s|wild=|v=|png=)", where.split(" (want")[0])[0]
            lines = f.read_text().split("\n")
            for i, l in enumerate(lines):
                if l.startswith(leg["name"] + " | "):
                    lines[i] = l.replace(f" | {leg['end']}", f" | {got}", 1)
            f.write_text("\n".join(lines))
            note += f" (end learned: {got})"
        print(f"{g} {leg['name']}: {verdict} {where}{note}", flush=True)
        if verdict not in ("ok", "look"):   # why it failed and what to try, 1-3 lines (tools/route_triage.py)
            import route_triage
            print("\n".join(route_triage.triage(g, leg, verdict, route_triage.load_logs(WORK, g, leg["name"], where))), flush=True)
        if args.map:
            dumps = sorted((WORK / f"{g}-{leg['name']}-map").glob("mapdump-*.txt"))
            for d in dumps[-1:]:
                print(f"--- {d}\n" + d.read_text(), flush=True)
        same = None
        if args.repeat and verdict == "ok":
            runs = [st]   # each repeat is the first run again: same keys, same cfgscript, nothing taken mid-run
            for i in range(1, args.repeat + 1):
                s = {}
                s["verdict"] = run_leg(g, leg, args, dict(produced), s, tag=f"-r{i}")[0]
                runs.append(s)
            same, why = compare_runs(runs)
            last = runs[-1]
            print(f"{g} {leg['name']}: repeat x{len(runs)} " + (f"SAME {last['pos']} v={last['v']} save={last['save']}" if same else f"DIFFERENT {why}")
                  + png_info(runs), flush=True)
        peaks.append((st.get("openfiles"), f"{g} {leg['name']}"))
        return g, verdict, (leg["name"], st.get("v"), st.get("secs", 0.0), verdict), same

    if args.chain:
        results = [one(j) for j in jobs]
    else:
        from concurrent.futures import ThreadPoolExecutor
        with ThreadPoolExecutor(max_workers=max(1, args.j)) as ex:
            results = list(ex.map(one, jobs))
    for g, v, _, same in results:
        summary.setdefault(g, [0, 0])
        summary[g][1] += 1
        summary[g][0] += v in PASS
    ok = sum(a for a, _ in summary.values())
    n = sum(b for _, b in summary.values())
    rows = [r for _, _, r, _ in results]
    frames, legsecs = sum(r[1] or 0 for r in rows), sum(r[2] for r in rows)
    if args.chain:   # the full-run report: frames and seconds a leg, in WORK/GAME-chain.tsv too
        table = chain_table(rows)
        (WORK / f"{games[0]}-chain.tsv").write_text(table)
        print(table, end="")
    if args.game == "all":
        for g, (a, b) in sorted(summary.items()):
            print(f"{g}: {a}/{b} ok")
    if any(p is not None for p, _ in peaks):   # a leak shows here before it reaches the exe's bound: the usual peak is flat
        peak, where = max((p, w) for p, w in peaks if p is not None)
        print(f"open files: peak {peak} at once ({where}), {len([1 for p, _ in peaks if p is not None])} legs")
    print(f"{ok}/{n} ok in {time.time() - t0:.0f} s; {frames} game frames in {legsecs:.0f} leg seconds"
          + (f" ({frames / legsecs:.0f} frames/s)" if legsecs else ""))
    differ = [1 for _, _, _, same in results if same is False]
    if args.repeat:
        print(f"repeat x{args.repeat + 1}: {sum(1 for *_, same in results if same)}/{n} legs SAME" + (f", {len(differ)} DIFFERENT" if differ else ""))
    sys.exit(0 if ok == n and not differ else 1)


if __name__ == "__main__":
    main()
