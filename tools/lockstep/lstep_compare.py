#!/usr/bin/env python3
"""Lockstep compare: the first divergent game frame between a melonDS dump and a port dump.

Contract: docs/lockstep.md. Usage:
    lstep_compare.py MELON.jsonl PORT.jsonl [options]
Exit status: 0 the dumps agree, 1 they diverge, 2 bad input.
The last line printed is always the one-line summary (-q prints only that).
"""
import argparse
import fnmatch
import json
import sys

# var name (fnmatch pattern) -> group, first match wins
GROUP_RULES = [
    ("rng.*", "rng"), ("battle.rng", "rng"),
    ("battle.*", "battle"),
    ("party*", "party"),
    ("eventwork", "flags/work"),
    ("script.*", "script"), ("event*", "script"), ("proc.*", "script"),
    ("player.*", "position"), ("state.*", "position"), ("field.*", "position"), ("field", "position"),
    ("game.frame", "frame"),
]
GROUP_ORDER = ["frame", "position", "script", "rng", "flags/work", "battle", "party", "other"]


def group_of(name):
    for pat, g in GROUP_RULES:
        if fnmatch.fnmatchcase(name, pat):
            return g
    return "other"


def load(path):
    """-> (header, {f: record}). Raises ValueError on a bad file."""
    header, recs = None, {}
    with open(path) as fh:
        for n, line in enumerate(fh, 1):
            line = line.strip()
            if not line:
                continue
            try:
                o = json.loads(line)
            except ValueError as e:
                raise ValueError("%s:%d: bad JSON: %s" % (path, n, e))
            if header is None and "lockstep" in o:
                header = o
                continue
            if "f" not in o:
                raise ValueError("%s:%d: record without f" % (path, n))
            if o["f"] in recs:
                raise ValueError("%s:%d: duplicate frame f=%s" % (path, n, o["f"]))
            recs[o["f"]] = o
    if header is None:
        raise ValueError("%s: no header line (\"lockstep\": 1)" % path)
    return header, recs


def grid_cells(s):
    """288 hex chars -> 48 (r,g,b) cells; None when malformed."""
    if not isinstance(s, str) or len(s) != 288:
        return None
    try:
        v = [int(s[i:i + 2], 16) for i in range(0, 288, 2)]
    except ValueError:
        return None
    return [tuple(v[i:i + 3]) for i in range(0, 144, 3)]


def screen_diff(a, b, tol):
    """-> ([(cell, maxdiff) beyond tol], worst diff); (None, None) without usable grids."""
    ca, cb = grid_cells(a), grid_cells(b)
    if ca is None or cb is None:
        return None, None
    bad, worst = [], 0
    for i, (p, q) in enumerate(zip(ca, cb)):
        d = max(abs(x - y) for x, y in zip(p, q))
        worst = max(worst, d)
        if d > tol:
            bad.append((i, d))
    return bad, worst


def compare_screens(ra, rb, tol, min_cells):
    """per screen {hash, bad, worst, cells, diverged}; None when a side has no screens in this record (the port's frame
    skip, LSTEP_SKIP: "top" is null on a frame it did not draw; melonDS always has them)"""
    if ra.get("top") is None or rb.get("top") is None or ra.get("bot") is None or rb.get("bot") is None:
        return None
    out = {}
    for name, gk in (("top", "gtop"), ("bot", "gbot")):
        same = ra.get(name) == rb.get(name)
        d = {"hash": same, "bad": 0, "worst": 0, "cells": [], "diverged": False}
        if not same:
            bad, worst = screen_diff(ra.get(gk), rb.get(gk), tol)
            if bad is None:        # no usable grid: the hash is all we have
                d["diverged"] = True
                d["worst"] = None
            else:
                d["bad"], d["worst"], d["cells"] = len(bad), worst, bad
                d["diverged"] = len(bad) >= min_cells
        out[name] = d
    return out


def var_diffs(ra, rb, names, ignore):
    va, vb = ra.get("vars", {}), rb.get("vars", {})
    diffs = []
    for n in names:
        if any(fnmatch.fnmatchcase(n, p) for p in ignore):
            continue
        x, y = va.get(n, "<absent>"), vb.get(n, "<absent>")
        if x != y and not alias_match(x, y):
            diffs.append((n, x, y))
    return diffs


def alias_match(x, y):
    """A DS overlay address several overlays share is dumped as "a|b|c" (vars.txt); it equals a side that names one of them."""
    if not (isinstance(x, str) and isinstance(y, str)):
        return False
    xs, ys = set(x.split("|")), set(y.split("|"))
    return bool(xs & ys) and ("|" in x or "|" in y)


def frame_state(a, ra, rb, names, ignore):
    """-> (var diffs, screen compare or None, screens diverged) of one frame pair, as the main loop judges it."""
    vd = [] if a.screens_only else var_diffs(ra, rb, names, ignore)
    if a.skip_lag and vd and (ra.get("lag") or rb.get("lag")):
        vd = []       # a melonDS lag frame (a load): the DS side's game state may sit a frame behind
    sd = None if a.ignore_screens else compare_screens(ra, rb, a.tol, a.min_cells)
    sdiv = bool(sd and (sd["top"]["diverged"] or sd["bot"]["diverged"]))
    return vd, sd, sdiv


BEAM_FIRST_CELL = 40      # the grid's bottom row (cells 40-47): 32 of the 192 lines, where the beam is not yet


def beam_race(a, f, sd, A, B, names, ignore):
    """True when frame f's difference is the one-frame beam race: frame f+1 (both sides present) has no var or screen
    difference, and every bad cell of the differing screens is either in the top screen's bottom row (the writes land
    below the beam) or a cell the port's own picture changes in between frames f-1 and f+1 and melonDS's f holds the
    state of one of them, or its whole picture is the port's f+1 and changed in f (the game's logic writes VRAM while the DS draws, so melonDS shows part or all of the next
    picture; the port renders a whole state at once, the one after f's V-blank work)."""
    scr = [(nm, gk) for nm, gk in (("top", "gtop"), ("bot", "gbot")) if sd[nm]["diverged"]]
    if not scr or any(sd[nm]["worst"] is None for nm, _ in scr):
        return False
    ra, rb = A.get(f + 1), B.get(f + 1)
    if ra is None or rb is None or (a.until is not None and f + 1 > a.until):
        return False
    vd, sd2, sdiv2 = frame_state(a, ra, rb, names, ignore)
    if vd or sdiv2:
        return False

    def d(p, q):
        return max(abs(x - y) for x, y in zip(p, q))
    for nm, gk in scr:
        mine = grid_cells(A[f].get(gk))
        before = grid_cells((B.get(f - 1) or {}).get(gk))
        after = grid_cells(rb.get(gk))
        if mine is not None and after is not None:
            # the whole picture is the port's next one, and melonDS's picture did change in f (a port blip would not)
            mprev = grid_cells((A.get(f - 1) or {}).get(gk))
            if (all(d(p, q) <= a.tol for p, q in zip(mine, after))
                    and mprev is not None and any(d(p, q) > a.tol for p, q in zip(mine, mprev))):
                continue
        for c, _ in sd[nm]["cells"]:
            if nm == "top" and c >= BEAM_FIRST_CELL:
                continue
            if mine is None or before is None or after is None:
                return False
            # the cell changes across the frame in the port, and melonDS holds the state from one side of the change
            if d(before[c], after[c]) <= a.tol or min(d(mine[c], before[c]), d(mine[c], after[c])) > a.tol:
                return False
    return True


def fmt(v):
    return "null" if v is None else (v if isinstance(v, str) else json.dumps(v))


def sdesc(s):
    if s["hash"]:
        return "same"
    if s["worst"] is None:
        return "hash-differs(no-grid)"
    return "%s(bad=%d,worst=%d)" % ("DIFF" if s["diverged"] else "renderer", s["bad"], s["worst"])


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("melon", help="the melonDS dump")
    ap.add_argument("port", help="the port dump")
    ap.add_argument("--after", type=int, default=-1, metavar="F", help="compare only frames f > F")
    ap.add_argument("--until", type=int, default=None, metavar="F", help="compare only frames f <= F")
    ap.add_argument("--ignore-var", action="append", default=[], metavar="PAT",
                    help="ignore vars matching the fnmatch pattern (repeatable; comma lists ok)")
    ap.add_argument("--ignore-group", action="append", default=[], metavar="G",
                    help="ignore a group: " + ", ".join(GROUP_ORDER))
    ap.add_argument("--compare-frame", action="store_true",
                    help="also compare game.frame (ignored by default: the boot lengths of the two sides differ)")
    ap.add_argument("--skip-lag", action="store_true",
                    help="ignore var differences on frames where either side lagged (melonDS lag = a load wait: "
                         "the field's phase and event state can sit one frame apart there and meet again)")
    ap.add_argument("--ignore-beam-race", action="store_true",
                    help="skip a one-frame top-screen difference confined to the grid's bottom row (below the beam) "
                         "when all vars are equal and the next frame agrees again (docs/lockstep.md)")
    ap.add_argument("--ignore-screens", action="store_true", help="compare vars only")
    ap.add_argument("--screens-only", action="store_true", help="compare screens only")
    ap.add_argument("--tol", type=int, default=3,
                    help="grid tolerance: a cell is bad when a 5-bit channel mean differs by more than this (3)")
    ap.add_argument("--min-cells", type=int, default=1,
                    help="bad cells on a screen for it to count as different (1)")
    ap.add_argument("--allow-missing", action="store_true",
                    help="a frame in only one dump (a lag frame) is not a divergence")
    ap.add_argument("--allow-short", action="store_true",
                    help="one dump ending before the other is not a divergence")
    ap.add_argument("-n", "--context", type=int, default=5, metavar="N", help="frames before the divergence to show (5)")
    ap.add_argument("-q", "--quiet", action="store_true", help="print only the one-line summary")
    ap.add_argument("--json", action="store_true", help="print the result as JSON after the summary line")
    a = ap.parse_args(argv)

    try:
        ha, A = load(a.melon)
        hb, B = load(a.port)
    except (OSError, ValueError) as e:
        print("lstep_compare: %s" % e, file=sys.stderr)
        print("ERROR input")
        return 2

    ignore = [p for s in a.ignore_var for p in s.split(",") if p]
    if not a.compare_frame:
        ignore.append("game.frame")
    groups = set(g for s in a.ignore_group for g in s.split(",") if g)
    names = []
    for rec in [A[k] for k in sorted(A)[:1]] + [B[k] for k in sorted(B)[:1]]:
        for n in rec.get("vars", {}):
            if n not in names:
                names.append(n)
    ignore += [n for n in names if group_of(n) in groups]

    notes = []
    for k in ("rom", "save", "inputs", "clock", "vars"):
        if ha.get(k) != hb.get(k):
            notes.append("header %s differs: %s vs %s" % (k, ha.get(k), hb.get(k)))

    frames = [f for f in sorted(set(A) | set(B)) if f > a.after and (a.until is None or f <= a.until)]
    lastA, lastB = (max(A) if A else -1), (max(B) if B else -1)

    div = None
    first_var = first_scr = first_hashonly = None
    n_cmp = n_hashonly = n_beam = n_scr = 0
    first_beam = None
    for f in frames:
        ra, rb = A.get(f), B.get(f)
        if ra is None or rb is None:
            if a.allow_missing or f > min(lastA, lastB):
                continue      # a lag frame, or a dump that ended: the length check below says which
            div = {"f": f, "kind": "missing", "side": "port" if rb is None else "melonds"}
            break
        n_cmp += 1
        vd, sd, sdiv = frame_state(a, ra, rb, names, ignore)
        if sd is not None:
            n_scr += 1
        if sd and not (sd["top"]["hash"] and sd["bot"]["hash"]) and not (sd["top"]["diverged"] or sd["bot"]["diverged"]):
            n_hashonly += 1
            if first_hashonly is None:
                first_hashonly = f
        if a.ignore_beam_race and not vd and sdiv and beam_race(a, f, sd, A, B, names, ignore):
            n_beam += 1
            if first_beam is None:
                first_beam = f
            continue
        if vd and first_var is None:
            first_var = f
        if sdiv and first_scr is None:
            first_scr = f
        if vd or sdiv:
            div = {"f": f, "kind": "vars" if vd else "screens", "vars": vd, "screens": sd, "ra": ra, "rb": rb}
            break

    if div is None and not a.allow_short and not a.allow_missing and lastA != lastB:
        div = {"f": min(lastA, lastB) + 1, "kind": "length", "side": "melonds" if lastA < lastB else "port",
               "lenA": len(A), "lenB": len(B)}

    # ---- the one-line summary
    if div is None:
        summary = "SAME frames=%d" % n_cmp
        if n_cmp == 0:
            summary = "SAME frames=0 (nothing compared)"
        if n_scr != n_cmp and not a.ignore_screens:
            summary += " screens-compared=%d" % n_scr
        if n_hashonly:
            summary += " renderer-only-frames=%d" % n_hashonly
        if n_beam:
            summary += " beam-race-frames=%d" % n_beam
    else:
        f = div["f"]
        g = (div.get("ra") or A.get(f) or B.get(f) or {}).get("g", "?")
        if div["kind"] == "vars":
            gs = sorted(set(group_of(n) for n, _, _ in div["vars"]), key=GROUP_ORDER.index)
            vs = [n for n, _, _ in div["vars"]]
            sd = div["screens"]
            if sd is None:
                scr = "screens=ignored" if a.ignore_screens else "screens=not-drawn"
            elif sd["top"]["diverged"] or sd["bot"]["diverged"]:
                scr = "screens=differ"
            else:
                scr = "screens=same" if sd["top"]["hash"] and sd["bot"]["hash"] else "screens=renderer-only"
            summary = "DIVERGED f=%d g=%s kind=vars groups=%s vars=%s%s %s" % (
                f, g, ",".join(gs), ",".join(vs[:6]), "+%d" % (len(vs) - 6) if len(vs) > 6 else "", scr)
        elif div["kind"] == "screens":
            summary = "DIVERGED f=%d g=%s kind=screens-only top=%s bot=%s" % (
                f, g, sdesc(div["screens"]["top"]), sdesc(div["screens"]["bot"]))
        elif div["kind"] == "missing":
            summary = "DIVERGED f=%d kind=missing-frame absent-in=%s" % (f, div["side"])
        else:
            summary = "DIVERGED f=%d kind=length shorter=%s melonds=%d port=%d" % (
                f, div["side"], div["lenA"], div["lenB"])

    if not a.quiet:
        print("melonds: %s (%d frames)   port: %s (%d frames)" % (a.melon, len(A), a.port, len(B)))
        for n in notes:
            print("note: " + n)
        if div is None:
            print("no divergence in %d compared frames" % n_cmp)
            if n_scr != n_cmp and not a.ignore_screens:
                print("screens compared on %d of them (the port drew only those: frame skip)" % n_scr)
            if n_beam:
                print("beam race: %d one-frame top-screen differences skipped, first at f=%d" % (n_beam, first_beam))
            if n_hashonly:
                print("screens: %d frames differ in hash only, within tolerance (renderer), first at f=%d"
                      % (n_hashonly, first_hashonly))
        else:
            report(div, a, A, B, names, first_var, first_scr, first_hashonly, n_hashonly)
    print(summary)
    if a.json:
        print(json.dumps({"same": div is None, "f": div and div["f"], "kind": div and div["kind"],
                          "vars": [v[0] for v in (div or {}).get("vars", [])]}))
    return 0 if div is None else 1


def report(div, a, A, B, names, first_var, first_scr, first_hashonly, n_hashonly):
    f = div["f"]
    print("FIRST DIVERGENT FRAME: f=%d" % f)
    if div["kind"] == "missing":
        print("  the frame is absent in the %s dump" % div["side"])
        context(f, a, A, B, names, [])
        return
    if div["kind"] == "length":
        print("  the %s dump ends first (melonds %d records, port %d)" % (div["side"], div["lenA"], div["lenB"]))
        context(f, a, A, B, names, [])
        return
    ra, rb = div["ra"], div["rb"]
    print("  g: melonds %s, port %s   vb: %s / %s   lag: %s / %s   keys: %r / %r" % (
        ra.get("g"), rb.get("g"), ra.get("vb"), rb.get("vb"), ra.get("lag"), rb.get("lag"),
        ra.get("keys"), rb.get("keys")))
    if div["vars"]:
        print("  VARS differing (%d), melonds -> port:" % len(div["vars"]))
        by = {}
        for n, x, y in div["vars"]:
            by.setdefault(group_of(n), []).append((n, x, y))
        for gr in GROUP_ORDER:
            for n, x, y in by.get(gr, []):
                print("    [%-10s] %-18s %s -> %s" % (gr, n, fmt(x), fmt(y)))
    else:
        print("  vars: all equal")
    sd = div["screens"]
    if sd is None:
        print("  screens: ignored")
    else:
        for nm in ("top", "bot"):
            s = sd[nm]
            if s["hash"]:
                print("  screen %s: same" % nm)
                continue
            if s["worst"] is None:
                print("  screen %s: hash differs, no usable grid in the dump" % nm)
                continue
            print("  screen %s: hash differs; %d/48 cells beyond tol %d, worst %d (5-bit) => %s" % (
                nm, s["bad"], a.tol, s["worst"],
                "PICTURE DIFFERS" if s["diverged"] else "renderer only"))
            if s["cells"]:
                mark = set(c for c, _ in s["cells"])
                for r in range(6):
                    print("      " + "".join("#" if r * 8 + c in mark else "." for c in range(8)))
        sdiff = sd["top"]["diverged"] or sd["bot"]["diverged"]
        if div["kind"] == "vars" and not sdiff:
            print("  => game state differs while the pictures agree (within tol)")
        elif div["kind"] == "screens":
            print("  => game state agrees; only the picture differs: read the PNGs (a draw or 2D-state difference)")
    if n_hashonly:
        print("  (%d frames differ in screen hash only, within tolerance; first f=%s)" % (n_hashonly, first_hashonly))
    context(f, a, A, B, names, [v[0] for v in div.get("vars", [])])


def context(f, a, A, B, names, focus):
    if a.context <= 0:
        return
    prev = [x for x in sorted(set(A) | set(B)) if a.after < x < f][-a.context:]
    if not prev:
        return
    cols = focus[:4] or [n for n in ("player.x", "player.z", "script.id", "event.cb") if n in names]
    print("  last %d frames before (values melonds|port, ! = differ):" % len(prev))
    print("    %5s %6s %-10s %s" % ("f", "g", "keys", "  ".join(cols)))
    for x in prev:
        ra, rb = A.get(x), B.get(x)
        if ra is None or rb is None:
            print("    %5d  (absent in %s)" % (x, "port" if rb is None else "melonds"))
            continue
        cells = []
        for c in cols:
            va, vb = ra.get("vars", {}).get(c), rb.get("vars", {}).get(c)
            cells.append("%s|%s%s" % (fmt(va), fmt(vb), "" if va == vb or alias_match(va, vb) else "!"))
        print("    %5d %6s %-10s %s" % (x, ra.get("g"), ra.get("keys", ""), "  ".join(cells)))


if __name__ == "__main__":
    sys.exit(main())
