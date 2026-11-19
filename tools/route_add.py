#!/usr/bin/env python3
"""One command for a route leg: `tools/route.py GAME add "name | start | tokens | end | save [| flags]" [options]`
(tools/route.py dispatches here; docs/route-writer.md is the writer's quickstart).

    tools/route.py b add "gym-out | b-badge3 | Pa G:warp:31 Wf S W30 | ? | b-r4-0" --desc "Black: outside the gym" --comment "why"
    tools/route.py b add --from-file legs.txt          # a spec of legs, run in order, stops at the first failure

The leg is written to (or replaces the leg of that name in) tools/routes/GAME.txt (GAME.txt in $BWPORT_ROUTES_DIR when set, the
way route.py reads it: test legs never touch the committed files), with its comment lines; then it runs with
--learn when its end is `?` (the end is the position it reached: check that it is where the keys should end) or against the
given end otherwise, then --repeat 2; when the three runs are SAME and the leg makes a save, the save goes to the saves library
(never over an existing one: --replace SAVE for b2-*/w2-* test saves, route.py's rule) and its README line is added. Prints at most
about 8 lines: the verdict line, the story changes (the end dump's "changed since the snapshot"), the repeat line, stored or
why not; on a failure the pilot line that says why and the map dump's path. Exit 0 when the leg passed (and was stored or
needs no save), 1 otherwise.

A look leg (`Md C`, end `zone 0 0,0`) is run as it is: the verdict is `look`, its product is the map dump (the path is printed).

--from-file SPEC: the spec is route-file syntax, one leg a line; `#` lines above a leg are its comment lines and a line
`desc: README text` above it is its README text (--desc/--comment/--replace on the command line apply to every leg)."""
import os
import re
import subprocess
import sys
from pathlib import Path

NAME_RE = re.compile(r"^[A-Za-z0-9_.-]+$")
END_RE = re.compile(r"^zone \d+ -?\d+,-?\d+$")
PLACEHOLDER = "zone 0 0,0"   # route.py's LOOK: a leg whose end is not known yet (--learn replaces it)
SHOW_CHANGES = 5            # story changes printed (the rest as "+N")


def parse_spec(text):
    """"name | start | tokens | end | save [| flags]" -> dict(name, start, tokens, end, save, flags). end "?" = learn it.
    Raises ValueError with what is wrong."""
    f = [x.strip() for x in text.strip().split("|")]
    if len(f) == 5 or len(f) == 6:
        f += [""] * (6 - len(f))
    else:
        raise ValueError(f"need name | start | tokens | end | save [| flags]: got {len(f)} fields")
    name, start, tokens, end, save, flags = f
    if not NAME_RE.match(name):
        raise ValueError(f"bad leg name {name!r}")
    if not (start == "NEWGAME" or NAME_RE.match(start)):
        raise ValueError(f"bad start {start!r} (a save name or NEWGAME)")
    if not tokens:
        raise ValueError("no pilot tokens")
    if end != "?" and not END_RE.match(end):
        raise ValueError(f"bad end {end!r} (zone N x,z, or ? to learn it)")
    if save != "-" and not NAME_RE.match(save):
        raise ValueError(f"bad save {save!r} (a new name, or - for none)")
    return dict(name=name, start=start, tokens=tokens, end=end, save=save, flags=flags)


def parse_spec_file(text):
    """a spec file -> [(spec text, comment lines, README text or None)]: `#` lines above a leg are its comments, `desc:` its
    README text; blank lines are skipped, a comment block separated from a leg by a blank line still counts"""
    legs, comments, desc = [], [], None
    for line in text.splitlines():
        s = line.strip()
        if not s:
            continue
        if s.startswith("#"):
            comments.append(s)
        elif s.startswith("desc:"):
            desc = s[5:].strip()
        else:
            legs.append((s, comments, desc))
            comments, desc = [], None
    return legs


def leg_line(spec):
    end = PLACEHOLDER if spec["end"] == "?" else spec["end"]
    f = [spec["name"], spec["start"], spec["tokens"], end, spec["save"]] + ([spec["flags"]] if spec["flags"] else [])
    return " | ".join(f)


def routes_file(root, game):
    """the route file a leg goes to: tools/routes/GAME.txt, or GAME.txt in BWPORT_ROUTES_DIR (route.py reads the same one), so a
    test leg never touches the committed route files"""
    return Path(os.environ.get("BWPORT_ROUTES_DIR") or Path(root) / "tools/routes") / f"{game}.txt"


def write_leg(path, spec, comments):
    """put the leg in the route file: a leg of that name is replaced (its comment lines too when `comments` is given, else they
    stay), a new one goes after the last leg line (a comment block at the end of the file stays the file's last lines).
    Returns "added" or "replaced"."""
    lines = path.read_text().split("\n") if path.exists() else []
    if lines and lines[-1] == "":
        lines.pop()
    new = [c if c.startswith("#") else "# " + c for c in comments]
    prefix = spec["name"] + " |"
    old = [i for i, l in enumerate(lines) if l.startswith(prefix)]
    if old:
        i = old[0]
        j = i
        while new and j > 0 and lines[j - 1].startswith("#"):
            j -= 1
        lines[j:i + 1] = new + [leg_line(spec)]
        res = "replaced"
    else:
        last = max([i for i, l in enumerate(lines) if l.strip() and not l.startswith("#")], default=-1)
        lines[last + 1:last + 1] = new + [leg_line(spec)]
        res = "added"
    path.write_text("\n".join(lines) + "\n")
    return res


def readme_line(save, desc, spec, game):
    return f"{save:<14} {desc}   (route {game} {spec['name']} from {spec['start']})"


def readme_add(saves, save, line):
    """append the README line of a stored save unless the README has one for it: True when added"""
    p = Path(saves) / "README"
    text = p.read_text() if p.exists() else ""
    if re.search(rf"^{re.escape(save)}\s", text, re.M):
        return False
    p.write_text(text + ("" if text.endswith("\n") or not text else "\n") + line + "\n")
    return True


def story_changes(dump_text):
    """the lines of the dump's "changed since the snapshot (N):" section as "flag 0x0123 = 1 NAME" / "work 0x4092 = 4 (was 3)
    NAME" (each change's V-blank and zone dropped), the likely story ones first: named, then flags set, work values, flags
    cleared"""
    out, on = [], False
    for l in dump_text.splitlines():
        if l.startswith("changed since the snapshot"):
            on = True
        elif on and l.startswith("  "):
            m = re.match(r"\s+(flag|work) (0x[0-9a-f]+) = (\d+) \(was (\d+); [^)]*\)\s*(.*)$", l)
            if m:
                kind, num, now, was, nm = m.groups()
                rank = (0 if nm else 1, 0 if kind == "flag" and now != "0" else 1 if kind == "work" else 2)
                out.append((rank, len(out), f"{kind} {num} = {now}" + (f" (was {was})" if kind == "work" else "") + (f" {nm}" if nm else "")))
        elif on:
            break
    return [x[2] for x in sorted(out)]


def final_dump(work, game, name, dump):
    """the map dump of the leg's end that exists after --repeat (a repeat renames the first run's directory to its own): the
    printed path if it is still there, else the last repeat's"""
    if dump and Path(dump).exists():
        return dump
    found = sorted(Path(work).glob(f"{game}-{name}-r*-map/mapdump-*.txt"))
    return str(found[-1]) if found else dump


def parse_output(text, game, name):
    """route.py's printed output for one leg -> dict: line (the leg's verdict line), verdict, repeat (the repeat line or None),
    same (True/False/None), dump (path of the printed map dump or None), dump_text, learned (the end it learned or None)"""
    r = dict(line=None, verdict=None, repeat=None, same=None, dump=None, dump_text="", learned=None)
    head = f"{game} {name}: "
    lines = text.splitlines()
    for i, l in enumerate(lines):
        if l.startswith(head + "repeat x"):
            r["repeat"] = l
            r["same"] = " SAME " in l + " "
        elif l.startswith(head) and r["line"] is None:
            r["line"] = l
            r["verdict"] = l[len(head):].split(" ", 1)[0]
            m = re.search(r"\(end learned: ([^)]*)\)", l)
            r["learned"] = m.group(1) if m else None
        elif l.startswith("--- ") and r["dump"] is None:
            r["dump"] = l[4:].strip()
            body = []
            for d in lines[i + 1:]:
                if d.startswith(head) or re.match(r"^\d+/\d+ ok in ", d):
                    break
                body.append(d)
            r["dump_text"] = "\n".join(body)
    return r


def why(work, game, name):
    """the one line that says why a leg failed: the pilot's own reason (no path, hang, timeout, save), else a crash line"""
    log = Path(work) / f"{game}-{name}.log"
    text = log.read_text(errors="replace") if log.exists() else ""
    for pat in (r"^pilot: no path: .*", r"^pilot: hang: .*", r"^pilot: timeout: .*", r"^pilot: save: .*", r"^.*CRASH.*",
                r"^pilot: stuck.*", r"^pilot: .*"):
        found = re.findall(pat, text, re.M)
        if found:
            return found[-1] if pat == r"^pilot: .*" else found[0]
    return ""


def cut(s, n=220):
    return s if len(s) <= n else s[:n - 3] + "..."


def run_route(root, game, argv):
    p = subprocess.run([sys.executable, str(Path(root) / "tools/route.py"), game] + argv, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, text=True, errors="replace", stdin=subprocess.DEVNULL)
    return p.stdout


def add_leg(game, spec, comments, desc, replace, ctx, build=True, runner=None):
    """add one leg and run it: (ok, [the lines to print]). ctx: root, saves, work, store (route.py's)"""
    runner = runner or (lambda argv: run_route(ctx["root"], game, argv))
    name = spec["name"]
    out = []
    path = routes_file(ctx["root"], game)
    res = write_leg(path, spec, comments)
    nb = [] if build else ["--no-build"]
    learn = spec["end"] == "?"
    text = runner([name, "--map"] + nb + (["--learn"] if learn else ["--repeat", "2"]))
    r = parse_output(text, game, name)
    learned = r["learned"]
    if learn and learned:   # the end is in the file now: the same run again, with the repeats
        text = runner([name, "--map", "--no-build", "--repeat", "2"])
        r = parse_output(text, game, name)
    if r["line"] is None:
        out.append("no verdict from route.py: " + cut(" | ".join(text.strip().splitlines()[-3:])))
        return False, out
    out.append(cut(re.sub(r" map=\S+", "", r["line"]) + (f" ({res}{', end learned' if learned else ''})")))
    ch = story_changes(r["dump_text"])
    if ch:
        out.append(cut(f"story: {len(ch)} changed: " + "; ".join(ch[:SHOW_CHANGES]) + (f"; +{len(ch) - SHOW_CHANGES} more" if len(ch) > SHOW_CHANGES else "")))
    if r["verdict"] == "look":
        out.append(f"look leg: the map dump is {final_dump(ctx['work'], game, name, r['dump'])}")
        return True, out
    if r["verdict"] != "ok":
        import route_triage
        lines = route_triage.triage(game, spec, r["verdict"], route_triage.load_logs(ctx["work"], game, name, r["line"].split(": ", 1)[1].split(" ", 1)[-1]))
        reason = why(ctx["work"], game, name)
        if lines:
            out.extend(lines)
        elif reason:
            out.append(cut(reason))
        dump = r["dump"] or str(Path(ctx["work"]) / f"{game}-{name}-map")
        out.append(f"FAILED ({r['verdict']}): dump {final_dump(ctx['work'], game, name, dump)}")
        return False, out
    if r["repeat"] is None:
        out.append("FAILED: no repeat line from route.py")
        return False, out
    out.append(cut(r["repeat"].split(": ", 1)[1]))
    if not r["same"]:
        out.append("NOT STORED: the repeat is not SAME (a game bug or a timing dependence: report it)")
        return False, out
    save = spec["save"]
    dump = final_dump(ctx["work"], game, name, r["dump"])
    if save == "-":
        out.append("no save (save is -); end dump: " + str(dump))
        return True, out
    msg = ctx["store"](save, Path(ctx["work"]) / f"{game}-{name}.save.bin", save if replace else "")
    if msg.startswith("stored"):
        pos = re.search(r"(zone \d+ -?\d+,-?\d+)", r["line"])
        text = desc or (f"{pos.group(1)} after " if pos else "") + spec["tokens"]
        added = readme_add(ctx["saves"], save, readme_line(save, text, spec, game))
        out.append(f"stored {save}.bin" + (" + README line" if added else " (README has a line for it)"))
    else:
        out.append(cut(msg.replace("not stored", "NOT STORED", 1)))
    out.append("end dump: " + str(dump))
    return True, out


STORY_NEXT_LINES = 30   # lines of `story.py next` a look prints (the rest as a count)


def look_pilot(tokens, save_end):
    """the pilot string of a look: the writer's tokens, `S` when the look saves at its end, then the dump and the end"""
    toks = list(tokens) + (["S"] if save_end else [])
    if toks[-2:] != ["Md", "C"]:
        toks += ["Md", "C"]
    return " ".join(toks)


def look(game, argv, root, saves, work, store):
    """route.py GAME look SAVE [TOKENS ...] [--save-end] [--no-build]  (also: route.py GAME add --look SAVE [TOKENS ...])

    A look leg without touching the routes file: starts from SAVE (a library name or a .bin path), plays TOKENS (none: just
    the start), writes the map dump where it ends (`Md C`) and prints it (the short dump: the window around the player, warps,
    triggers, BG events, actors, the matrix's zones, the story section), then `story.py GAME next` for the zone it stopped in.
    story.py reads a save's flags and works, so it reads SAVE's: after tokens that change the story use --save-end (the look
    ends with the field menu's `S`, in the run's own copy of the save: the library is never touched) to read the end state."""
    import argparse
    import os
    import tempfile
    ap = argparse.ArgumentParser(prog=f"route.py {game} look", description=look.__doc__.split("\n\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("save", help="the start save: a name in the library or a .bin path")
    ap.add_argument("tokens", nargs="*", help="pilot tokens to play first (quote them as one argument or give them one by one)")
    ap.add_argument("--save-end", action="store_true", help="save at the end (S) so story.py next reads the end state")
    ap.add_argument("--full-map", action="store_true", help="the whole 64 x 64 map and every entry, not the short window (route.py --full-map)")
    ap.add_argument("--no-build", action="store_true")
    a = ap.parse_args(argv)
    start = Path(a.save)
    if start.suffix == ".bin" or "/" in a.save:   # a path: the library needs a name (route.py starts from SAVES/NAME.bin)
        if not start.exists():
            print(f"route look: no such save file {a.save}")
            return 2
        tmp = Path(tempfile.mkdtemp(prefix="look-saves-"))
        name = "lookstart"
        (tmp / f"{name}.bin").write_bytes(start.read_bytes())
        env_saves = str(tmp)
    else:
        name, env_saves = a.save, None
        if not (Path(saves) / f"{name}.bin").exists():
            print(f"route look: no save {name} in {saves}")
            return 2
    tokens = [t for x in a.tokens for t in x.split()]
    routes = Path(tempfile.mkdtemp(prefix="look-routes-"))
    (routes / f"{game}.txt").write_text(f"lookleg | {name} | {look_pilot(tokens, a.save_end)} | {PLACEHOLDER} | -\n")
    env = dict(os.environ, BWPORT_ROUTES_DIR=str(routes))
    if env_saves:
        env["BWPORT_SAVES"] = env_saves
    p = subprocess.run([sys.executable, str(Path(root) / "tools/route.py"), game, "lookleg", "--map"] + (["--full-map"] if a.full_map else []) +
                       (["--no-build"] if a.no_build else []),
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace", stdin=subprocess.DEVNULL, env=env)
    text = p.stdout
    r = parse_output(text, game, "lookleg")
    if r["line"] is None:
        print("no verdict from route.py: " + cut(" | ".join(text.strip().splitlines()[-3:])))
        return 1
    print(cut(re.sub(r" map=\S+", "", r["line"])))
    if r["verdict"] != "look":
        import route_triage
        lines = route_triage.triage(game, dict(name="lookleg", start=name, pilot=look_pilot(tokens, a.save_end), end=PLACEHOLDER, save="-", flags=[]),
                                    r["verdict"], route_triage.load_logs(work, game, "lookleg", r["line"].split(": ", 1)[1].split(" ", 1)[-1]))
        print("\n".join(lines) or cut(why(work, game, "lookleg")))
        print(f"FAILED ({r['verdict']}): dump {final_dump(work, game, 'lookleg', r['dump'] or str(Path(work) / f'{game}-lookleg-map'))}")
        return 1
    print(r["dump_text"].rstrip())
    zone = re.search(r"^map: \S+ zone (\d+)", r["dump_text"], re.M)
    saved = Path(work) / f"{game}-lookleg.save.bin"
    src = a.save if env_saves is None else str(Path(env_saves) / f"{name}.bin")
    note = f"the start save {a.save}'s flags" if not tokens else f"the START save {a.save}'s flags: --save-end reads the end state"
    if a.save_end and saved.exists():
        src, note = str(saved), "the look's own save at its end"
    cmd = [sys.executable, str(Path(root) / "tools/story.py"), game, "next", src] + (["--zone", zone.group(1)] if zone else [])
    q = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace", stdin=subprocess.DEVNULL)
    rows = q.stdout.rstrip().splitlines()
    print(f"story.py {game} next ({note}):")
    print("\n".join(rows[:STORY_NEXT_LINES]) + (f"\n  ... {len(rows) - STORY_NEXT_LINES} more lines" if len(rows) > STORY_NEXT_LINES else ""))
    print(f"dump: {final_dump(work, game, 'lookleg', r['dump'])}")
    return 0


def main(game, argv, root, saves, work, store):
    import argparse
    if "--look" in argv:   # add --look SAVE [TOKENS]: the same as `route.py GAME look SAVE [TOKENS]`
        return look(game, [x for x in argv if x != "--look"], root, saves, work, store)
    ap = argparse.ArgumentParser(prog=f"route.py {game} add", description=__doc__.split("\n\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("spec", nargs="?", help='"name | start | tokens | end-or-? | save [| flags]"')
    ap.add_argument("--desc", default="", help="the README line's text")
    ap.add_argument("--comment", action="append", default=[], help="a comment line above the leg (repeat for more)")
    ap.add_argument("--from-file", metavar="SPEC", help="legs, one a line, run in order; stops at the first failure")
    ap.add_argument("--replace", action="store_true", help="may overwrite the leg's b2-*/w2-* test save (never another)")
    ap.add_argument("--no-build", action="store_true")
    a = ap.parse_args(argv)
    if bool(a.spec) == bool(a.from_file):
        ap.error("give one leg line or --from-file SPEC")
    try:
        if a.from_file:
            legs = [(parse_spec(s), c + a.comment, d or a.desc) for s, c, d in parse_spec_file(Path(a.from_file).read_text())]
        else:
            legs = [(parse_spec(a.spec), a.comment, a.desc)]
    except ValueError as e:
        print(f"route add: {e}")
        return 2
    if not legs:
        print("route add: no legs")
        return 2
    ctx = dict(root=root, saves=saves, work=work, store=store)
    for n, (spec, comments, desc) in enumerate(legs):
        ok, out = add_leg(game, spec, comments, desc, a.replace, ctx, build=not a.no_build and n == 0)
        print("\n".join(out), flush=True)
        if not ok:
            if len(legs) > 1:
                print(f"stopped at leg {n + 1} of {len(legs)} ({spec['name']})")
            return 1
    if len(legs) > 1:
        print(f"{len(legs)} legs ok")
    return 0
