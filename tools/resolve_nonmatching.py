"""docs/nonmatching.md of a conflicted merge: ours, with each row (keyed by function and file) that either side deleted
since the base gone, a row the other side changed taken from it, and the rows only theirs added appended at the end
of the table they were in (theirs' order)."""
import re
import subprocess

P = "docs/nonmatching.md"
show = lambda n: subprocess.run(["git", "show", f":{n}:{P}"], capture_output=True, text=True).stdout
ROW = re.compile(r"^\| `([^`]+)` \| `0x[0-9a-f]+` \| `([^`]+)` \|")
key = lambda l: (m[1], m[2]) if (m := ROW.match(l)) else None
base, ours, theirs = (show(n).splitlines(True) for n in (1, 2, 3))
rows = lambda ls: {k: l for l in ls if (k := key(l))}
b, o, t = rows(base), rows(ours), rows(theirs)
out = []
for l in ours:
    k = key(l)
    if not k:
        out.append(l)
    elif k in b and k not in t:  # theirs deleted it
        continue
    else:
        out.append(t[k] if k in t and o[k] == b.get(k) and t[k] != o[k] else l)
# rows only theirs added: after the last row of ours from the same table position (the end of the first table)
added = [l for k, l in t.items() if k not in b and k not in o]
if added:
    last = max(i for i, l in enumerate(out) if key(l))
    out[last + 1:last + 1] = added
import nonmatching_dedupe
text, dropped = nonmatching_dedupe.dedupe("".join(out))  # one row per function
open(P, "w").write(text)
out = text.splitlines(True)
print(f"ours {len(o)}, theirs {len(t)}, base {len(b)} rows -> {sum(1 for l in out if key(l))}; {len(added)} added from theirs")
