#!/usr/bin/env python3
"""route_triage: a failed route leg in 1-3 lines: what went wrong and what to try. tools/route.py prints them under a non-ok
verdict line, tools/route_add.py's FAILED output carries them. A writer reads these instead of the logs and dumps.

    triage(game, leg, verdict, logs) -> [line, ...]   (1-3 lines, "  > CLASS: evidence" then "  > try: what to do")
        leg: the leg (route.py's dict: "pilot" tokens; route_add's spec: "tokens")
        logs: dict, every key optional: where (the text after the verdict on route.py's line), log (RUN_WORK/GAME-LEG.log), game
              (RUN_WORK/GAME-LEG.game.log, the game's whole log), dump (the end dump's text)
    load_logs(work, game, name, where="") -> logs   (reads those files; the dump is the newest in RUN_WORK/GAME-LEG-map/)

Classes: port (a crash, a stub, the game stopped drawing), no-path (a G token cannot be done: an actor, a wall, a ledge, water, a
target not loaded, a rail), stuck (a walk that does not step), waiting (an unanswered yes/no, a scene that never ends, a Ws that never
comes true), off-route (ended elsewhere: a trigger that did not fire), battle, save, unclassified (the 2 last pilot lines).
Never quotes game text. Tests: python3 tools/route_triage_tests.py"""
import heapq
import re
from pathlib import Path

WIDTH = 230
HEX = r"0x[0-9a-f]+"
FREE = ".,@DTb=is"            # the dump's walkable cells (tools/../port/src/route_map.c legend)
LEDGE = {"^": (0, -1), "v": (0, 1), "<": (-1, 0), ">": (1, 0)}
COST = {"N": 30, "~": 200, "?": 2, "#": 1000}
NAMES = {"N": "an actor", "~": "water (Surf)", "#": "a wall", "?": "unloaded tiles", "L": "a one-way ledge"}


def cut(s, n=WIDTH):
    return s if len(s) <= n else s[:n - 3] + "..."


def load_logs(work, game, name, where=""):
    """the files of a run in RUN_WORK: {where, log, game, dump} (text; "" when there is none)"""
    work = Path(work)

    def read(p):
        return p.read_text(errors="replace") if p.exists() else ""
    dumps = sorted((work / f"{game}-{name}-map").glob("mapdump-*.txt"))
    return dict(where=where, log=read(work / f"{game}-{name}.log"), game=read(work / f"{game}-{name}.game.log"),
                dump=read(dumps[-1]) if dumps else "")


# ---------------------------------------------------------------- the end dump (port/src/route_map.c, short or full)
def parse_dump(text):
    """the dump's numbers: player, zone, win (x0, x1, z0, z1), grid {(x, z): char}, warps, trigs, actors, offwin (ids not in the
    window), changed {work/flag: value now}, live {T: (work, want, now)}, hidden {uid: flag}, due (the scene table's DUE line)"""
    d = dict(player=None, zone=None, win=None, grid={}, warps=[], trigs=[], actors=[], offwin=set(), changed={}, live={}, hidden={},
             due=None, rail=False)
    lines = text.splitlines()
    m = re.match(r"map: \S+ zone (\d+) player (-?\d+),(-?\d+) .*?tiles (-?\d+)\.\.(-?\d+) x (-?\d+)\.\.(-?\d+)", lines[0]) if lines else None
    if not m:
        return d
    d["zone"], d["player"] = int(m.group(1)), (int(m.group(2)), int(m.group(3)))
    x0, x1, z0, z1 = (int(m.group(i)) for i in range(4, 8))
    d["win"] = (x0, x1, z0, z1)
    story = False
    for l in lines[1:]:
        if l.startswith("story ("):
            story = True
        if l.startswith("rail: the player"):
            d["rail"] = True
        if not story and len(l) == 6 + x1 - x0 + 1 and l[5] == " " and l[:5].strip().lstrip("-").isdigit() and "not loaded" not in l:
            for i, ch in enumerate(l[6:]):
                d["grid"][(x0 + i, int(l[:5]))] = ch
            continue
        for m in re.finditer(r"\b(?:D(\d+)>z(\d+)|(uid) (\d+))\b", l) if "not shown:" in l else ():
            if m.group(3):
                d["offwin"].add(int(m.group(4)))
        m = re.match(r"\s+D(\d+) (?:(-?\d+),(-?\d+)|\S+) (\d+)x(\d+) -> zone (\d+)", l)
        if m and not story:
            d["warps"].append(dict(n=int(m.group(1)), x=m.group(2) and int(m.group(2)), z=m.group(3) and int(m.group(3)),
                                   w=int(m.group(4)), h=int(m.group(5)), zone=int(m.group(6)), camera="(camera" in l))
        m = re.match(r"\s+T(\d+) (?:(-?\d+),(-?\d+)|\S+) (\d+)x(\d+) script (\d+) \(work (" + HEX + r") = (\d+)", l)
        if m and not story:
            d["trigs"].append(dict(n=int(m.group(1)), x=m.group(2) and int(m.group(2)), z=m.group(3) and int(m.group(3)),
                                   w=int(m.group(4)), h=int(m.group(5)), work=m.group(7), want=int(m.group(8))))
        m = re.match(r"\s+N uid (\d+) at (-?\d+),(-?\d+) \(from .*?\) code (\d+) move (\d+)", l)
        if m and not story:
            d["actors"].append(dict(uid=int(m.group(1)), x=int(m.group(2)), z=int(m.group(3)), code=int(m.group(4))))
        m = re.match(r"\s+(work|flag) (" + HEX + r") = (\d+) \(was", l)
        if m and story:
            d["changed"][m.group(2)] = int(m.group(3))
        m = re.match(r"\s+T(\d+) .*?work (" + HEX + r")(?: \S+)? == (\d+): now (-?\d+)( LIVE)?", l)
        if m and story:
            d["live"][int(m.group(1))] = (m.group(2), int(m.group(3)), int(m.group(4)), bool(m.group(5)))
        m = re.match(r"\s+uid (\d+) .*?flag (" + HEX + r")(?: \S+)? (?:set|clear): hidden", l)
        if m and story:
            d["hidden"][int(m.group(1))] = m.group(2)
        if story and l.rstrip().endswith(" DUE") and "-> script" in l:
            d["due"] = l.strip()
    return d


def goal_tiles(d, tok):
    """the tiles the walk ends on for a G token, from the dump's lists: (set of tiles, is_target_known)"""
    m = re.fullmatch(r"G:(-?\d+),(-?\d+)", tok)
    if m:
        return {(int(m.group(1)), int(m.group(2)))}, True
    m = re.fullmatch(r"G:warp:(\d+)", tok)
    if m:
        ws = [w for w in d["warps"] if w["zone"] == int(m.group(1)) and w["x"] is not None]
        return {(w["x"] + i, w["z"] + j) for w in ws for i in range(w["w"] or 1) for j in range(w["h"] or 1)}, bool(ws)
    m = re.fullmatch(r"G:trig:(\d+)", tok)
    if m:
        ts = [t for t in d["trigs"] if t["n"] == int(m.group(1)) and t["x"] is not None]
        return {(t["x"] + i, t["z"] + j) for t in ts for i in range(t["w"] or 1) for j in range(t["h"] or 1)}, bool(ts)
    m = re.fullmatch(r"G:(?:npc|item|wall):(\d+)(?::nota)?", tok)
    if m:
        a = [a for a in d["actors"] if a["uid"] == int(m.group(1))]
        return {(a[0]["x"] + dx, a[0]["z"] + dz) for dx, dz in ((0, 1), (0, -1), (1, 0), (-1, 0))} if a else set(), bool(a)
    return set(), False


def blockers(d, goals):
    """the cheapest way from the player to a goal tile across everything: [(class, tile)] for each cell on it that is not free,
    or None when there is no way at all (or no grid). Class: N actor, ~ water, # wall, ? unloaded, L a ledge entered the wrong way"""
    grid, start = d["grid"], d["player"]
    if not grid or start is None or not goals:
        return None
    best, prev, heap = {start: 0}, {}, [(0, start)]
    while heap:
        c, p = heapq.heappop(heap)
        if c > best.get(p, 1 << 30):
            continue
        if p in goals:
            path = []
            while p in prev:
                if (cls := cell_class(grid, prev[p][1], p)) != ".":
                    path.append((cls, p))
                p = prev[p][0]
            return path[::-1]
        for dx, dz in ((0, 1), (0, -1), (1, 0), (-1, 0)):
            q = (p[0] + dx, p[1] + dz)
            if q not in grid:
                continue
            n = c + step_cost(grid, p, q, (dx, dz))
            if n < best.get(q, 1 << 30):
                best[q] = n
                prev[q] = (p, p)
                heapq.heappush(heap, (n, q))
    return None


def cell_class(grid, frm, q):
    ch = grid.get(q, "#")
    if ch in FREE:
        return "."
    if ch in LEDGE:
        return "." if (q[0] - frm[0], q[1] - frm[1]) == LEDGE[ch] else "L"
    return ch if ch in COST else "#"


def step_cost(grid, p, q, dirn):
    cls = cell_class(grid, p, q)
    return 1 if cls == "." else 300 if cls == "L" else COST[cls]


def blocker_text(d, path, game, zone):
    """(what blocks, the suggestion) for blockers()'s path"""
    kinds = []
    for cls, _ in path:
        if cls not in kinds:
            kinds.append(cls)
    cls = kinds[0]
    cells = [t for c, t in path if c == cls]
    if cls == "N":
        who = []
        for t in cells[:3]:
            a = [a for a in d["actors"] if (a["x"], a["z"]) == t]
            who.append(f"uid {a[0]['uid']} at {t[0]},{t[1]}" if a else f"an actor at {t[0]},{t[1]}")
        a0 = [a for a in d["actors"] if (a["x"], a["z"]) == cells[0]]
        if game in ("b", "w") and zone == 29 and a0 and a0[0]["code"] == 0:
            return "blocked by " + ", ".join(who) + " (a honey wall)", f"G:wall:{a0[0]['uid']} walks through it (the cell before it, key held)"
        uid = a0[0]["uid"] if a0 else None
        return ("blocked by " + ", ".join(who) + (f" (+{len(cells) - 3} more)" if len(cells) > 3 else ""),
                (f"talk to it first (G:npc:{uid} Wf) or " if uid is not None else "") + "find what moves it: story.py GAME zone Z (its script, its flag)")
    if cls == "L":
        return (f"only a one-way ledge leads there (entered the wrong way at {cells[0][0]},{cells[0][1]})",
                "a ledge is jumped one way only: go round it (another warp or side), or come from the other side first")
    if cls == "~":
        return f"water ({len(cells)} tiles) is in the way", "needs Surf: a TOOL GAP unless the leg has it (story.py GAME who-sets flag ... for the HM)"
    if cls == "?":
        return "the way goes through tiles not loaded yet", "walk closer with G:X,Z (a tile near the target) first, then the token"
    return (f"walled off: {len(cells)} wall tiles ('#') between {path[0][1][0]},{path[0][1][1]} and the target",
            "a gate/door the story opens (story.py GAME who-warps Z; a scripted door needs its talk or trigger first) or a Cut tree/boulder")


# ---------------------------------------------------------------- the pilot's log
def pilot_lines(logs):
    text = logs.get("game") or logs.get("log") or ""
    return [l for l in text.splitlines() if l.startswith("pilot:")]


def tokens_of(leg):
    return (leg.get("pilot") or leg.get("tokens") or "").split()


def waiting_token(leg, plines):
    """the token the run was on: the first one after the last that logged 'done' (Pa, Pb, Md, E: log nothing)"""
    toks, i = tokens_of(leg), 0
    for l in plines:
        m = re.match(r"pilot: (\S+) done", l)
        if m:
            while i < len(toks) and toks[i] != m.group(1).rstrip(":"):
                i += 1
            i += 1
    while i < len(toks) and (toks[i] in ("Pa", "Pb", "Md", "E:on", "E:off") or toks[i].startswith("K")):
        i += 1
    return toks[i] if i < len(toks) else None


def story_hint(game, d, work, want):
    """the story.py query that answers 'what sets work 0x.. to N', and the work's value now when the dump says it"""
    now = d["changed"].get(work)
    return (f"story.py {game} who-sets work {work} ={want}", now)


# ---------------------------------------------------------------- the classes
def classify_no_path(game, leg, why, tok, logs, d):
    """(evidence, suggestion) for a 'pilot: no path: WHY (G token TOK)' line"""
    zone = d["zone"]
    if re.search(r"no actor of uid (\d+) in zone", why):
        uid = int(re.search(r"uid (\d+)", why).group(1))
        if uid in d["hidden"]:
            return (f"{tok}: actor {uid} is hidden here (flag {d['hidden'][uid]})",
                    f"story.py {game} who-sets flag {d['hidden'][uid]} =0 shows what brings it; or you are in the wrong zone (Wz:N)")
        return f"{tok}: zone {zone} has no actor of uid {uid}", f"wrong zone or uid? the dump's actor list; hidden or not yet created: story.py {game} zone {zone}"
    if re.search(r"zone \d+ has \d+ (triggers|BG events)", why):
        return f"{tok}: " + re.search(r"has \d+ (?:triggers|BG events) \([^)]*\)", why).group(0), "the index is the dump's own (TN / bN of the zone the player is in)"
    if "not a pass-through actor" in why:
        return f"{tok}: that actor is not a honey wall", "G:wall passes the Castelia Gym walls only (B/W zone 29); anything else: G:npc or another way round"
    if "started no event" in why:
        uid = re.search(r"G:(?:npc|item):(\d+)", tok)
        return (f"{tok}: A facing it started no event (scenery, or its script waits on a flag)",
                f"story.py {game} script {zone} ID: the actor's script (uid {uid.group(1)})" if uid else "the actor's script: story.py GAME script Z ID")
    if "does not turn to face" in why:
        return f"{tok}: {why.split(': ')[-1][:120]}", "something holds the player: an event still running (Wf before the token) or a step the game refuses"
    if why.startswith("wall ") and "closed" in why:
        return f"{tok}: {why.split(' (')[0]}: its switches are not all pressed", "do the gym's earlier switches first (G:trig/G:npc in this leg), then the wall"
    if "rail" in why:
        return f"{tok}: {cut(why, 150)}", "rails: G:rail:L,F,S (a place) while on a rail, tiles and warps from the grid; a hybrid hand-off is planned by G:warp/G:X,Z itself; the dump's rail lines"
    if "no warp to zone" in why:
        return f"{tok}: {cut(why, 150)}", f"the door may be scripted: story.py {game} who-warps N; or G:zone:A-N for the atlas path"
    if "no field" in why:
        return f"{tok}: no field (a battle or scene is running)", "add Wf before the token"
    if "toward" in why:
        return f"{tok}: the target is not on the loaded map (no tile nearer reachable)", "walk closer with G:X,Z first (a tile near it), then the token"
    if re.search(r"want G:|not a G token|a number after|after the number", why):
        return f"{tok}: bad token ({cut(why, 140)})", "tokens: the header of port/src/route_test.c"
    m = re.search(r"\bride:\d+: (?!the target)(.*?)(?: \(G token .*)?$", why)
    if m:   # the adapter's own reason (another floor, closed, switch wrong, no such ride): not a window problem
        return f"{tok}: {cut(m.group(1), 170)}", "G:ride:N waits for the car at spot N; a switch (G:trig:T) or another ride on the way first; the ride table is the game's gimmick"
    goals, known = goal_tiles(d, tok)
    reach = ""
    m = re.search(r"warps reached lead to:(.*)$", why)
    if m:
        reach = " (reachable doors lead to:" + " ".join(re.findall(r"zone \d+", m.group(1))[:8]) + ")"
    walls = [a["uid"] for a in d["actors"] if game in ("b", "w") and zone == 29 and a["code"] == 0]
    if not known and walls:
        return (f"{tok}: the way is closed by honey walls (actors {' '.join(map(str, walls[:5]))}; the target is not in the window)",
                f"G:wall:UID passes one (the gym's switches first): {' '.join('G:wall:' + str(u) for u in walls[:3])}")
    m = re.search(r": (\d+) goal tiles, none reachable", why)
    if not known and m and int(m.group(1)) > 0:   # the planner's window is twice the dump's: it saw the target, no way leads there
        return (f"{tok}: unreachable: {m.group(1)} tiles to stand on next to it, none can be reached from here{reach}",
                "a wall, pit or closed gate between (the dump shows only 32 tiles each way: `route.py --full-map`; BWPORT_MAPDUMP_H "
                "lists heights); go round with G:X,Z or open it first (story.py " + game + f" who-warps {zone})")
    if not known:
        return (f"{tok}: the target is not in the dump's window{reach}",
                "walk closer with G:X,Z first (a tile near it), then the token; `route.py --full-map` lists every actor and warp")
    if "0 goal tiles" in why:
        return f"{tok}: no free tile to stand on next to the target{reach}", "it stands in a niche or behind a counter: G:X,Z next to it, or :nota; the dump grid around it"
    path = blockers(d, goals)
    if path:
        what, tip = blocker_text(d, path, game, zone)
        return f"{tok}: {what}{reach}", tip.replace("GAME", game).replace(" Z", f" {zone}")
    return f"{tok}: unreachable: no way to it in the loaded grid{reach}", "the dump's grid: where the walls are; a gate/door the story opens (story.py " + game + f" who-warps {zone})"


def stuck_text(game, leg, plines, d):
    m = None
    for l in plines:
        mm = re.match(r"pilot: stuck at (\S+) \(G: (?:(\d+) re-plans without a step|the warp does not take)\)", l)
        if mm:
            m = mm
    if m and m.group(2) is None:
        cam = [w for w in d["warps"] if w["camera"]]
        return (f"stuck at {m.group(1)}: the warp does not take",
                f"a scripted door ('camera' warp{' D' + str(cam[0]['n']) if cam else ''}) needs its talk/trigger first: story.py {game} who-warps {d['zone']}")
    if m:
        pos = tuple(int(v) for v in m.group(1).split(",")) if re.fullmatch(r"-?\d+,-?\d+", m.group(1)) else None
        wp = None
        for l in plines:
            mm = re.match(r"pilot: G: .*? -> waypoint (-?\d+),(-?\d+)", l)
            if mm:
                wp = (int(mm.group(1)), int(mm.group(2)))
        near = [a for a in d["actors"] if pos and abs(a["x"] - pos[0]) + abs(a["z"] - pos[1]) == 1]
        ev = f"stuck at {m.group(1)}: {m.group(2)} re-plans without a step" + (f" toward {wp[0]},{wp[1]}" if wp else "")
        return (ev + (f"; uid {near[0]['uid']} stands next to it" if near else ""),
                "a step the planner thinks is free is refused (a moving actor, a scripted block): wait with Wf:30, or an earlier token must clear it")
    last = [l for l in plines if l.startswith("pilot: stuck")]
    return (cut(last[-1][7:], 150) if last else "stuck", "the dump's grid around @ shows what is next to the player")


def waiting_text(game, leg, verdict, why, plines, d, logs):
    """hang / timeout: the pilot waited for something that did not come"""
    tok = waiting_token(leg, plines)
    m = re.match(r"(P[yn]): no yes/no list in (\d+) frames", why)
    if m:
        return f"{m.group(1)} waited {m.group(2)} frames: the script asked no yes/no question here", f"remove {m.group(1)}, or a talk token (G:npc:UID) before it must start the question"
    if re.match(r"P[yn]: the yes/no list takes no answer", why):
        return "a yes/no list is up and takes no answer (the pilot's key does nothing)", "port bug: report the leg and the game log"
    m = re.match(r"(\S+), event (on|battle), zone (\d+) (-?\d+,-?\d+), no progress in (\d+) frames(, a yes/no list is up)?", why)
    if m:
        if m.group(2) == "battle":
            lines = [l for l in plines if "battle" in l][-1:]
            return (f"battle hang at {m.group(1)} in zone {m.group(3)}: no progress in {m.group(5)} frames (the pilot presses A: a move with 0 PP, a menu it does not answer)" + (
                    f"; last: {cut(lines[0][7:], 80)}" if lines else ""), "run the battle by hand with --shot N to see the screen; report it if the port draws no menu")
        if m.group(6):
            return (f"waiting: an unanswered yes/no question at {m.group(1)} (zone {m.group(3)} {m.group(4)})",
                    "put Py (Yes) or Pn (No) before " + m.group(1) + " (a script's list needs the answer; Pa/Pb cannot)")
        on = [l for l in plines if l.startswith("pilot: event on")][-1:]
        ev = f"waiting: a scene never ended at {m.group(1)} (event on{' since ' + on[0].split('at ')[-1] if on else ''}, no event off, {m.group(5)} frames at zone {m.group(3)} {m.group(4)})"
        due = f"; a scene is DUE: {d['due']}" if d.get("due") else ""
        return cut(ev + due), "a text box/menu the pulse key does not close (Pa vs Pb?) or a scene waiting for a trigger: `route.py --shot N` shows it; story.py " + game + f" zone {d['zone']}"
    m = re.match(r"(Ws:(!?)(\S+?)(?:=(\d+))?) not done in (\d+) frames", why)
    if m:
        if re.fullmatch(HEX, m.group(3)) and m.group(4) is not None:
            return (f"{m.group(1)} never became true in {m.group(5)} frames" + (f" (work {m.group(3)} is {d['changed'][m.group(3)]} now)" if m.group(3) in d["changed"] else ""),
                    f"the scene that sets it has not run: story.py {game} who-sets work {m.group(3)} ={m.group(4)} (the trigger/script and its checks)")
        return f"{m.group(1)} never became true in {m.group(5)} frames", f"story.py {game} who-sets flag {m.group(3)}: what sets it, and is that scene reachable from here"
    m = re.match(r"(Wz:\d+) not done", why)
    if m:
        return f"{m.group(1)}: the player never reached that zone (now zone {d['zone']})", "the warp/token before it did not lead there: G:warp:N or G:zone:N"
    m = re.match(r"(Wf\S*) not done in (\d+) frames", why)
    if m:
        return f"{m.group(1)}: the field never became free ({m.group(2)} frames)", "a scene, text or question is still up: Pa before it, Py/Pn for a question"
    m = re.match(r"(\S+) not done in (\d+) frames", why)
    if m:
        return f"{m.group(1)} not done in {m.group(2)} frames", "the walk loops or overshoots (G:), or the scene never ends: the dump and --shot N"
    return None


def off_route_text(game, leg, where, plines, d):
    m = re.match(r"(zone -?\d+ \S+) .*?\(want (zone -?\d+ \S+)\)", where) or re.match(r"(\S+ \S+ \S+).*want (.*?)\)?$", where)
    got, want = (m.group(1), m.group(2)) if m else ("?", "?")
    notrig = [re.match(r"pilot: (G:trig:\d+) done at [^()]*\(V-blank \d+\)$", l) for l in plines]
    notrig = [x.group(1) for x in notrig if x]
    ev = f"off-route: ended {got}, wanted {want}"
    if notrig:
        t = int(notrig[-1].split(":")[-1])
        tr = [x for x in d["trigs"] if x["n"] == t]
        if tr:
            gate = f"T{t} needs work {tr[0]['work']} = {tr[0]['want']}" + (f", now {d['changed'][tr[0]['work']]}" if tr[0]["work"] in d["changed"] else "")
            return [cut(ev + f"; {notrig[-1]} started no scene: " + gate),
                    f"story.py {game} who-sets work {tr[0]['work']} ={tr[0]['want']}: the scene that sets it must run first"]
        return [cut(ev + f"; {notrig[-1]} started no scene (its work gate does not match)"), "the dump's story section lists the live triggers; story.py GAME zone Z"]
    return [ev, "if the end is right, learn it with `?`; else the end dump shows where the player is and the story section what the leg changed"]


def triage(game, leg, verdict, logs):
    """the 1-3 lines for a failed leg (see the module's docstring); [] for ok and look"""
    if verdict in ("ok", "look"):
        return []
    where = logs.get("where") or ""
    plines = pilot_lines(logs)
    d = parse_dump(logs.get("dump") or "")
    text = (logs.get("game") or "") + "\n" + (logs.get("log") or "")
    logf = f"RUN_WORK/{game}-{leg.get('name', 'LEG')}.game.log"

    def out(*a):
        return [cut("  > " + a[0])] + [cut("  > try: " + x) for x in a[1:]]

    note = re.search(r"\(((?:crash|stub|stopped|hang in)\b.*?)\)(?= \d+s|$)", where)
    if verdict == "no-path":
        return out(f"no atlas path: {cut(where, 150)}", f"python3 tools/atlas.py {game} path A B shows the hops; a scripted door has none: write its tokens by hand")
    if verdict == "no-save":
        return out(f"{where}", "the save is made by an earlier leg (run it first, or --chain), or check $BWPORT_SAVES/README")
    stub = re.search(r"bwport: undefined function (\S+) called", text)
    if verdict == "crash" or note and note.group(1).startswith(("crash", "stub", "hang in")) or stub:
        what = note.group(1) if note else f"stub {stub.group(1)} called" if stub else "CRASH in the game's log"
        kind = "stub: a function the port does not implement" if what.startswith("stub") or stub else "the game itself died or stopped drawing frames"
        return out(f"port bug ({kind}): {cut(what, 170)}", f"report it (leg line, verdict, {logf}); do not work around it")
    nop = re.search(r"^pilot: no path: (.*) \(G token (\S+)\)$", text, re.M)
    if nop:
        ev, tip = classify_no_path(game, leg, nop.group(1), nop.group(2), logs, d)
        return out(f"no path: {ev}", tip)
    if verdict == "stuck":
        ev, tip = stuck_text(game, leg, plines, d)
        return out(ev, tip)
    hang = re.search(r"^pilot: (?:hang|timeout): (.*?)(?: at V-blank \d+)?$", text, re.M)
    if verdict in ("hang", "timeout") and hang:
        w = waiting_text(game, leg, verdict, hang.group(1), plines, d, logs)
        if w:
            return out("waiting: " + w[0] if not w[0].startswith(("waiting", "battle")) else w[0], w[1])
    if verdict in ("hang", "timeout") and not hang:
        tok = waiting_token(leg, plines)
        return out(f"{verdict}: {cut(where, 120)}" + (f"; the run was waiting at {tok}" if tok else ""), "no pilot reason in the log: the game stopped drawing frames (port bug: report it) or a long wait; --shot N")
    if verdict == "off-route":
        return out(*off_route_text(game, leg, where, plines, d))
    if verdict == "save-failed":
        s = [l for l in plines if l.startswith("pilot: S:")][-2:]
        return out("the save did not finish: " + (cut("; ".join(l[7:] for l in s), 150) if s else where), "end with `Wf S W30` (the field free first); a message box on the save screen needs Pa")
    last = [l for l in plines if not re.match(r"pilot: (?:event|G: .*waypoint|map dump)", l)][-2:]
    return out(f"unclassified ({verdict}): " + cut(" | ".join(l[7:] for l in last) or where, 170), "the end dump and the game log: build/route-cache/route-work")
