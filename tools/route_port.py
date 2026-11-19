#!/usr/bin/env python3
"""route.py port SRC DST --from LEG [--to LEG] [--start SAVE] [--dry] [--repeat N] [route.py options]

Port a route to the other version of the game: b -> w, b2 -> w2 (and back). White/White 2 are the same games as Black/Black 2
with small differences, so each SRC leg from LEG on (to LEG --to, in the file's order) is tried in DST as it is: the same line, the
save names mapped (b-X and black-X -> w-X, b2-X -> w2-X; any other name gets the prefix), run from DST's saves library.

  --start SAVE  the DST save the first leg starts from (default: the mapped name of its start, when that save exists)
  --dry         print the legs that would be ported, their mapped lines, and run nothing
  --repeat N    the repeats of a leg that passes (default 2: route.py's recipe, "repeat x3 SAME")

A leg is in the chain when its start save is --start's twin or made by an earlier leg of the run (a leg from any other save, as
b2's gap1 from b2-town, is skipped); a leg DST already has by name is skipped too (so a re-run continues where the last stopped).
One line per leg:
  ported   ends at SRC's end tile; the repeats are SAME: the line is appended to tools/routes/DST.txt, the save stored in
           the saves library and a line added to its README (SRC's text, the game name changed)
  learned  ends elsewhere in the SAME zone, and the SRC leg's run (made here, for its map dump) has the same story changes (the
           flags and work values the map dump's story section lists): the learned end is taken, with a comment line saying so
  STOP     anything else (another zone, other story changes, crash, hang, stuck, a repeat that ends elsewhere, a save name
           that exists): nothing is written; the report says the leg, the verdict line, how the story changes differ, the map
           dump (read it like any new leg's) and the line a writer starts from. Exit 1.
A repeat that differs only in the end frame or the save bytes (the pokestar legs do in b2 too) is accepted with a comment line.
"""
import argparse
import copy
import re
import sys
from pathlib import Path

import route as R

ROUTES = R.ROOT / "tools/routes"
NAME = {"b": "Black", "w": "White", "b2": "Black 2", "w2": "White 2"}
ALIAS = {"b": ("b", "black"), "w": ("w", "white"), "b2": ("b2",), "w2": ("w2",)}   # the save-name prefixes of each game
PAIRS = {("b", "w"), ("w", "b"), ("b2", "w2"), ("w2", "b2")}
CHANGE = re.compile(r"^\s+(flag|work) (0x[0-9a-f]+) = (\d+) \(was ", re.M)


def map_save(name, src, dst):
    """a SRC save name as DST's: b-X, black-X -> w-X, b2-X -> w2-X, any other name gets the prefix (caught -> w-caught); - stays"""
    if name == "-":
        return name
    m = re.match(r"^(?:%s)-(.+)$" % "|".join(ALIAS[src]), name)
    return f"{dst}-{m.group(1) if m else name}"


def swap_game(text, src, dst):
    """a README text of SRC's save as DST's: the game name, the save names (b2-v7) and `route b2` changed"""
    text = re.sub(r"\b%s\b%s" % (NAME[src], r"(?! 2)" if len(src) == 1 else ""), NAME[dst], text)
    text = re.sub(r"\b(?:%s)-(?=\w)" % "|".join(ALIAS[src]), dst + "-", text)
    return re.sub(r"\broute %s\b" % src, f"route {dst}", text)


def dst_leg(leg, src, dst, have):
    """SRC's leg as DST's (a dict like read_legs): start and save mapped, the `quick` flag dropped (the quick set is a choice for
    each game); None when its start save is not in the chain (have: SRC save name -> DST save name)"""
    if leg["start"] not in have:
        return None
    return dict(leg, start=have[leg["start"]], save=map_save(leg["save"], src, dst), flags=[f for f in leg["flags"] if f != "quick"])


def line_of(leg):
    """the routes file's line of a leg: the trailing fields that are empty or - are left out (flags, then the save)"""
    f = [leg["name"], leg["start"], leg["pilot"], leg["end"], leg["save"], " ".join(leg["flags"])]
    while len(f) > 4 and f[-1] in ("", "-"):
        f.pop()
    return " | ".join(f)


def same_zone(a, b):
    za, zb = (a or "").split()[:2], (b or "").split()[:2]
    return za[:1] == ["zone"] and za == zb


def story_changes(text):
    """{(flag or work, id): value} of a map dump's story section ("changed since the snapshot"), without the V-blank"""
    return {(k, i): int(v) for k, i, v in CHANGE.findall(text)}


def story_diff(a, b):
    """"" when the two story changes are the same, else what differs (a: SRC's, b: DST's)"""
    out = []
    for key in sorted(set(a) | set(b)):
        if a.get(key) != b.get(key):
            out.append(f"{key[0]} {key[1]}: SRC {'-' if key not in a else a[key]}, DST {'-' if key not in b else b[key]}")
    return "; ".join(out)


def dump_text(game, leg):
    dumps = sorted((R.WORK / f"{game}-{leg}-map").glob("mapdump-*.txt"))
    return dumps[-1].read_text() if dumps else ""


def readme_line(save, src_save, src, dst, how):
    """the saves README's line for DST's save: SRC's text (game name changed) when it has one"""
    text = ""
    readme = R.SAVES / "README"
    if readme.exists():
        m = re.search(r"^%s:?\s+(.*)$" % re.escape(src_save), readme.read_text(errors="replace"), re.M)
        text = swap_game(m.group(1), src, dst) if m else ""
    return f"{save:<14} {text or NAME[dst] + ': ' + how}   [ported from {src_save} by route.py port: {how}]"


def append(path, text):
    old = path.read_bytes() if path.exists() else b""
    with open(path, "ab") as f:
        f.write((b"\n" if old and not old.endswith(b"\n") else b"") + text.encode())


def stop(a, leg, d, why, verdict_line, diff="", dump=""):
    """the STOP report (a list of lines)"""
    out = [f"{a.dst} {leg['name']}: STOP {why}", f"  verdict: {verdict_line}"]
    if diff:
        out.append(f"  story changes differ: {diff}")
    if dump:
        out.append(f"  map dump: {dump}")
    out.append(f"  line (not written): {line_of(d)}")
    return out


def port_one(a, args, leg, d):
    """Run SRC's leg `leg` as DST's `d`: (ok, lines to print). ok: written (routes file, saves library, README)"""
    mapdir = R.WORK / f"{a.dst}-{d['name']}-map/"
    st, comments, learned = {}, [], False
    try:
        z = R.zone_leg(a.dst, d, args, {})
    except ValueError as e:
        return False, stop(a, leg, d, f"no-path {e}", "the G:zone path")
    verdict, where, out = R.run_leg(a.dst, z, args, {}, st)
    st["verdict"] = verdict
    vline = f"{verdict} {where}"
    if verdict == "off-route":
        got = st["pos"]
        if not same_zone(got, leg["end"]):
            return False, stop(a, leg, d, f"ends in another zone ({got}; {a.src} ends at {leg['end']})", vline, dump=mapdir)
        sargs = copy.copy(args)
        sargs.map = True   # the SRC run's dump stays: its story changes are what DST's are compared with
        try:
            sz = R.zone_leg(a.src, leg, sargs, {})
        except ValueError as e:
            return False, stop(a, leg, d, f"the {a.src} leg itself has no path: {e}", vline, dump=mapdir)
        sv, sw, _ = R.run_leg(a.src, sz, sargs, {}, {})
        if sv not in R.PASS:
            return False, stop(a, leg, d, f"the {a.src} leg is {sv} itself", vline + f"   [{a.src}: {sv} {sw}]", dump=mapdir)
        diff = story_diff(story_changes(dump_text(a.src, leg["name"])), story_changes(dump_text(a.dst, d["name"])))
        if diff:
            return False, stop(a, leg, d, f"ends at {got} ({a.src}: {leg['end']}) with other story changes", vline, diff, mapdir)
        comments.append(f"# {NAME[a.dst]}: ends at {got}, {NAME[a.src]} at {leg['end']} (the same zone, the same story changes)")
        learned = True
        d = dict(d, end=got)
        z = dict(z, end=got)
        st = {}
        verdict, where, out = R.run_leg(a.dst, z, args, {}, st)   # the end taken: ok, and the save of the leg
        st["verdict"] = verdict
        vline = f"{verdict} {where}"
    if verdict not in R.PASS:
        return False, stop(a, leg, d, verdict, vline, dump=mapdir)
    runs = [st]
    for i in range(1, a.repeat + 1):
        s = {}
        s["verdict"] = R.run_leg(a.dst, z, args, {}, s, tag=f"-r{i}")[0]
        runs.append(s)
    same, why = R.compare_runs(runs)
    if not same:
        fields = [w.split()[0] for w in why.split("; ")]
        if "verdict" in fields or "pos" in fields:
            return False, stop(a, leg, d, f"the repeats differ: {why}", vline, dump=mapdir)
        comments.append(f"# {NAME[a.dst]}: --repeat differs in {', '.join(fields)} ({why})")
    note = ""
    if d["save"] != "-":
        if out is None:
            return False, stop(a, leg, d, "no save produced", vline, dump=mapdir)
        res = R.store(d["save"], out, "")
        if not res.startswith("stored"):
            return False, stop(a, leg, d, res, vline)
        how = f"route {a.dst} {d['name']} from {d['start']}, end {st['pos']}"
        append(R.SAVES / "README", readme_line(d["save"], leg["save"], a.src, a.dst, how) + "\n")
        note = f" stored {d['save']}"
    append(ROUTES / f"{a.dst}.txt", "".join(c + "\n" for c in comments) + line_of(d) + "\n")
    return True, [f"{a.dst} {d['name']}: {'learned' if learned else 'ported'} {st['pos']} " + ("(repeat " + ("SAME" if same else "differs") + ")") + note + f"   [{a.src} {leg['end']}]"]


def main(argv):
    ap = argparse.ArgumentParser(prog="route.py port", description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("src", choices=sorted(R.GAMES))
    ap.add_argument("dst", choices=sorted(R.GAMES))
    ap.add_argument("--from", dest="first", required=True, metavar="LEG")
    ap.add_argument("--to", metavar="LEG")
    ap.add_argument("--start", metavar="SAVE")
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--repeat", type=int, default=2, metavar="N")
    a, rest = ap.parse_known_args(argv)
    if (a.src, a.dst) not in PAIRS:
        sys.exit(f"route port: {a.src} -> {a.dst}: the pairs are b -> w and b2 -> w2 (and back)")
    args = R.arg_parser().parse_args([a.dst] + rest)
    legs = R.read_legs(a.src)
    names = [l["name"] for l in legs]
    for n in (a.first, a.to):
        if n and n not in names:
            sys.exit(f"route port: no leg {n} in {a.src}.txt")
    first = legs[names.index(a.first)]
    start = a.start or map_save(first["start"], a.src, a.dst)
    if first["start"] != "NEWGAME" and not (R.SAVES / f"{start}.bin").exists():
        sys.exit(f"route port: the first leg starts from {start}, which is not in the saves library: --start SAVE")
    have = {first["start"]: start}
    if not a.dry and R.LINUX and not args.no_build:
        R.ensure_exe()
    n_ok = 0
    for leg in legs[names.index(a.first):(names.index(a.to) if a.to else len(legs) - 1) + 1]:
        mine = {l["name"]: l for l in R.read_legs(a.dst)}
        if leg["name"] in mine:
            if leg["save"] != "-":
                have[leg["save"]] = mine[leg["name"]]["save"]
            print(f"{a.dst} {leg['name']}: already in {a.dst}.txt", flush=True)
            continue
        d = dst_leg(leg, a.src, a.dst, have)
        if d is None:
            print(f"{a.dst} {leg['name']}: skipped (its start {leg['start']} is not in the chain)", flush=True)
            continue
        exists = d["save"] != "-" and (R.SAVES / f"{d['save']}.bin").exists()
        if a.dry:
            print(f"{a.dst} {leg['name']}: would port: {line_of(d)}" + (f"   [STOP: {d['save']}.bin exists in the saves library]" if exists else ""), flush=True)
        elif exists:
            print("\n".join(stop(a, leg, d, f"{d['save']}.bin exists in the saves library (rename or remove it)", "not run")), flush=True)
            return 1
        else:
            ok, lines = port_one(a, args, leg, d)
            print("\n".join(lines), flush=True)
            if not ok:
                return 1
            n_ok += 1
        if leg["save"] != "-":
            have[leg["save"]] = d["save"]
    print(f"{a.dst}: {n_ok} legs ported" if not a.dry else f"{a.dst}: dry run")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
