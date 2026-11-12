#!/usr/bin/env python3
"""Markdown table from tools/mac_perf.py matrix's jsonl: tools/mac_perf_table.py matrix.jsonl
Per config and scene: the steadier of the two capped runs (lower p99 of the work), the uncapped fps, and both capped runs' work medians."""
import json, sys

rows = [json.loads(l) for l in open(sys.argv[1])]
rows = [r for r in rows if r]
keys = []
for r in rows:
    k = (r["renderer"], r["res"], r["scene"])
    if k not in keys:
        keys.append(k)
print("| renderer | res | scene | work ms med / p99 | game / render / present (med) | worker med | GPU busy med | cores | capped reps (work med) | uncapped fps | load before |")
print("|---|---|---|---|---|---|---|---|---|---|---|")
for k in sorted(keys, key=lambda k: (k[0], k[1], ["title", "dialog", "room"].index(k[2]))):
    cap = [r for r in rows if (r["renderer"], r["res"], r["scene"]) == k and r["mode"] == "capped"]
    unl = [r for r in rows if (r["renderer"], r["res"], r["scene"]) == k and r["mode"] == "unlimited"]
    if not cap:
        continue
    b = min(cap, key=lambda r: r["work"][1])
    f = lambda v: "%.1f" % v[0] if v else "-"
    print("| %s | %d | %s | %.1f / %.1f | %s / %s / %s | %s | %s | %.1f | %s | %s | %s |" % (
        k[0], k[1], k[2], b["work"][0], b["work"][1], f(b["game"]), f(b["render"]), f(b["present"]), f(b["worker"]), f(b["gpu"]),
        b["cores"], " , ".join("%.1f" % r["work"][0] for r in cap), " , ".join("%.0f" % r["fps"] for r in unl) or "-",
        " ".join("%.0f" % r["load0"] for r in cap + unl) + (" DISTURBED" if any(r.get("disturbed", r["load0"] >= 12) for r in cap + unl) else "")))
