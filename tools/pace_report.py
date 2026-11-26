#!/usr/bin/env python3
"""Frame pacing report from a BWPORT_PRESENTLOG file: how evenly the game's motion reaches the panel, and how late.

    python3 tools/pace_report.py RUN.log [RUN2.log ...] [--skip-s 3] [--tail-s 0] [--table] [--json]

One line per log: presents/s, the even-step share, repeats and jumps, drops, refresh skips, the lag. Two logs given as
`A.log B.log` print each (compare them by eye or with --table, which prints a markdown row per log).

The log (libntr sim_main.cpp; times in us on the pacing clock):
  P now tick r mid hash slot call serial game due early   one present: `game` = the game time its picture shows (-1: not said)
  S serial submit done at vs per                          the panel model's refresh `at` for present `serial`
  D now waiting                                           a present dropped (the panel two pictures behind)
Other lines (T U W I G X) are ignored.

Definitions (what docs/interp.md "Pacing" and docs/perf.md "Smooth motion" call them):
  step      the game time the picture shows, minus the one before's, divided by the refreshes between the two (ms per refresh).
  mode      the commonest step (0.2 ms bins); with Smooth on it is the refresh period x the game's speed, 8.33 ms at 120 Hz.
  even      steps within 0.6 ms of the mode, as a share of all steps (100% = the motion on the glass is as even as it can be).
  repeat    step < a quarter of the mode (the same game time shown twice); jump: step > 1.8 x mode (a picture skipped).
  skip      two presents on the glass more than 1.5 refreshes apart (a refresh with nothing new); dbl: less than half a refresh.
  drop      D lines: presents the model threw away because the panel was two pictures behind.
  lag       the model's glass time `at` minus the game time the picture shows (picture to glass, ms): mean / p50 / p90.
  dlag      `at` minus the submit time (the swap call's start): the display lag the overlay shows.
Without S lines (no display link: a real vsync on a desktop) the present's own time stands in for `at` and the refresh period is
taken from the median gap ("link=no": skips, dbl and lag are then the pacing's, not the glass's).
"""
import argparse, json, statistics, sys


def parse(path):
    P, S, D = [], {}, []
    for l in open(path, errors="replace"):
        t = l.split()
        if not t:
            continue
        try:
            if t[0] == "P" and len(t) >= 12:
                P.append(dict(now=int(t[1]), tick=int(t[2]), r=int(t[3]), mid=int(t[4]), slot=int(t[6]), call=int(t[7]),
                              serial=int(t[8]), game=int(t[9]), due=int(t[10]), early=int(t[11])))
            elif t[0] == "S" and len(t) >= 7:
                S[int(t[1])] = dict(submit=int(t[2]), done=int(t[3]), at=int(t[4]), vs=int(t[5]), per=int(t[6]) // 1000)
            elif t[0] == "D" and len(t) >= 3:
                D.append(int(t[1]))
        except ValueError:
            pass  # a line cut at the end of the file
    return P, S, D


def pct(v, p):
    if not v:
        return float("nan")
    v = sorted(v)
    return v[min(len(v) - 1, int(p / 100.0 * len(v)))]


def analyse(path, skip_s=3.0, tail_s=0.0):
    P, S, D = parse(path)
    if not P:
        return dict(file=path, error="no P lines (BWPORT_PRESENTLOG not set, or the run never presented)")
    t0 = P[0]["now"] + skip_s * 1e6
    t1 = P[-1]["now"] - tail_s * 1e6
    link = any(p["serial"] in S for p in P)
    rows = []
    at1s = []
    for p in P:
        if p["now"] < t0 or p["now"] > t1:
            continue
        s = S.get(p["serial"])
        if s is not None:
            rows.append((s["at"], p["game"], s["submit"], p))
            if s["done"] > s["vs"] > 0:  # the first refresh after the GPU was done: the glass with no standing queue behind
                at1 = s["vs"] + -(-(s["done"] - s["vs"]) // s["per"]) * s["per"]
                at1s.append((s["at"] - at1, p["game"] >= 0 and at1 - p["game"]))
        elif not link:
            rows.append((p["now"], p["game"], p["now"], p))
    rows.sort(key=lambda r: r[0])
    if len(rows) < 20:
        return dict(file=path, error="too few presents (%d) after the %.0f s warm-up" % (len(rows), skip_s))
    dur = (rows[-1][0] - rows[0][0]) / 1e6
    ats = [r[0] for r in rows]
    gaps = [ats[i + 1] - ats[i] for i in range(len(ats) - 1)]
    per = statistics.median(S[r[3]["serial"]]["per"] for r in rows[:200]) if link else statistics.median(gaps)
    has_game = [r for r in rows if r[1] >= 0]
    steps = []
    for i in range(1, len(rows)):
        if rows[i][1] < 0 or rows[i - 1][1] < 0:
            continue
        n = max(1, round((rows[i][0] - rows[i - 1][0]) / per))
        steps.append((rows[i][1] - rows[i - 1][1]) / 1000.0 / n)
    out = dict(file=path, link="yes" if link else "no", presents=len(rows), secs=round(dur, 1), per_ms=round(per / 1000.0, 3),
               fps=round((len(rows) - 1) / dur, 1) if dur > 0 else 0.0,
               drops=sum(1 for d in D if t0 <= d <= t1),
               skips=sum(1 for g in gaps if g > 1.5 * per), dbl=sum(1 for g in gaps if g < 0.5 * per))
    if steps:
        bins = {}
        for s in steps:
            bins[round(s / 0.2)] = bins.get(round(s / 0.2), 0) + 1
        mode = max(bins, key=lambda b: (bins[b], -b)) * 0.2
        # refine the mode: the median of the steps in the winning bin
        near = [s for s in steps if abs(s - mode) <= 0.3]
        mode = statistics.median(near) if near else mode
        out.update(mode_ms=round(mode, 2), steps=len(steps),
                   even=round(100.0 * sum(1 for s in steps if abs(s - mode) <= 0.6) / len(steps), 1),
                   repeats=sum(1 for s in steps if s < 0.25 * mode) if mode > 1 else sum(1 for s in steps if abs(s) < 0.1),
                   jumps=sum(1 for s in steps if mode > 1 and s > 1.8 * mode),
                   sd_ms=round(statistics.pstdev(steps), 2))
        lag = [(r[0] - r[1]) / 1000.0 for r in has_game]
        out.update(lag_mean=round(statistics.mean(lag), 1), lag_p50=round(pct(lag, 50), 1), lag_p90=round(pct(lag, 90), 1))
        if at1s:  # the model's FIFO keeps a standing queue (a startup double present never drains: the swap does not block on a pretended link)
            out.update(queue_ms=round(statistics.mean(a for a, _ in at1s) / 1000.0, 1),
                       lag1_mean=round(statistics.mean(g / 1000.0 for _, g in at1s if g), 1))
    dl = [(r[0] - r[2]) / 1000.0 for r in rows]
    out.update(dlag_mean=round(statistics.mean(dl), 1), dlag_p90=round(pct(dl, 90), 1))
    out["mid_pct"] = round(100.0 * sum(1 for r in rows if r[3]["mid"]) / len(rows), 1)
    return out


def line(o):
    if "error" in o:
        return "%s: %s" % (o["file"], o["error"])
    return ("%s: presents/s=%s (%d in %ss, link=%s, refresh %s ms) even=%s%% (mode %s ms, sd %s) repeats=%s jumps=%s drops=%d "
            "skips=%d dbl=%d lag(game->glass)=%s/%s/%s ms dlag=%s/%s ms mid=%s%%" % (
                o["file"], o["fps"], o["presents"], o["secs"], o["link"], o["per_ms"], o.get("even", "?"), o.get("mode_ms", "?"),
                o.get("sd_ms", "?"), o.get("repeats", "?"), o.get("jumps", "?"), o["drops"], o["skips"], o["dbl"],
                o.get("lag_mean", "?"), o.get("lag_p50", "?"), o.get("lag_p90", "?"), o["dlag_mean"], o["dlag_p90"], o["mid_pct"])
            ) + ((" queue=%sms lag1=%sms" % (o["queue_ms"], o["lag1_mean"])) if "queue_ms" in o else "")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("logs", nargs="+")
    ap.add_argument("--skip-s", type=float, default=3.0, help="warm-up to leave out (s, default 3)")
    ap.add_argument("--tail-s", type=float, default=0.0, help="seconds to leave out at the end")
    ap.add_argument("--table", action="store_true", help="markdown rows")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    res = [analyse(f, a.skip_s, a.tail_s) for f in a.logs]
    if a.json:
        print(json.dumps(res, indent=1))
    elif a.table:
        print("| log | presents/s | even % | repeats | jumps | drops | skips | lag mean/p90 ms | dlag ms |")
        print("| --- | --- | --- | --- | --- | --- | --- | --- | --- |")
        for o in res:
            if "error" in o:
                print("| %s | %s |" % (o["file"], o["error"]))
            else:
                print("| %s | %s | %s | %s | %s | %d | %d | %s / %s | %s |" % (
                    o["file"].rsplit("/", 1)[-1], o["fps"], o.get("even", "?"), o.get("repeats", "?"), o.get("jumps", "?"),
                    o["drops"], o["skips"], o.get("lag_mean", "?"), o.get("lag_p90", "?"), o["dlag_mean"]))
    else:
        for o in res:
            print(line(o))
    return 0


if __name__ == "__main__":
    sys.exit(main())
