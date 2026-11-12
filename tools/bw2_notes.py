#!/usr/bin/env python3
"""pokebw2's own notes on a function: the rows of its docs/nonmatching-functions.md and the paragraphs of its
docs/matching.md that name it in backticks, cut short, for brief.py to show beside the twin's C. s755 matched
ica_anime.c's GFL_G3DCurvePreloadFrames (380 lines of diff) from the lead of its row there, and s768 found two leads
for mystery_album.c's function in matching.md, both by hand.

    python3 tools/bw2_notes.py NAME [NAME...]     # the block brief.py shows for these names (Black's, the twin's)

A paragraph is a bullet ("- " and its indented lines), a table row or a plain paragraph. One longer than CAP keeps its
first sentence (the rule, mostly) and the sentences that name the function with the one after, to CAP.
"""
import re
import sys
from pathlib import Path

DOCS = Path.home() / "ref" / "pokebw2" / "docs"
FILES = ("nonmatching-functions.md", "matching.md")  # the rows of the function's own first
CAP = 450  # characters of a note
ROW = 700  # of a nonmatching row, all about the function
MOST = 3  # notes shown
SENTENCE = re.compile(r"(?<=[.!?])\s+(?=[A-Z`(\"])")
_BLOCKS = {}


def blocks(name):
    """[(heading, line, text)] of a pokebw2 doc's paragraphs (line: its first's number), read once."""
    if name not in _BLOCKS:
        p = DOCS / name
        lines = p.read_text(errors="replace").splitlines() if p.exists() else []
        out, cur, heading, at = [], [], "", [0]

        def end():
            if cur:
                out.append((heading, at[0], " ".join(s.strip() for s in cur)))
                cur.clear()
        for no, line in enumerate(lines, 1):
            at[0] = at[0] if cur else no
            if line.startswith("#"):
                end()
                heading = line.lstrip("#").strip()
            elif not line.strip():
                end()
            elif line.startswith(("- ", "|", "* ")):
                end()
                at[0] = no
                cur.append(line)
            else:
                cur.append(line)
        end()
        _BLOCKS[name] = [(h, n, t) for h, n, t in out if not re.fullmatch(r"\|[\s|:-]*", t)]
    return _BLOCKS[name]


def cut(text, pattern, cap=None):
    """text, or where longer than cap the sentences that match pattern, then the one after each, then the first (the
    rule, mostly), as many as fit, in the text's order; a row (pattern None) is about its function whole: its start."""
    cap = cap or CAP
    if len(text) <= cap:
        return text
    ss = SENTENCE.split(text)
    named = [i for i, s in enumerate(ss) if pattern and pattern.search(s)]
    order = named + [i + 1 for i in named if i + 1 < len(ss)] + [0] if named else list(range(len(ss)))
    keep, size = set(), 0
    for i in dict.fromkeys(order):
        if size + len(ss[i]) + 5 <= cap:
            keep.add(i)
            size += len(ss[i]) + 5
        elif not keep:  # one sentence longer than cap: its start
            return ss[i][:ss[i].rfind(" ", 0, cap)] + " ..."
        elif not named:
            break
    out, last = "", -1
    for i in sorted(keep):
        out += ("" if not out else " " if i == last + 1 else " ... ") + ss[i]
        last = i
    return ("... " if min(keep) else "") + out + (" ..." if last < len(ss) - 1 else "")


def notes(names):
    """[(doc, line, heading, note)] that name one of names (Black's and the twin's), the function's own rows first."""
    names = [n for n in dict.fromkeys(names) if n and len(n) > 3]
    if not names:
        return []
    pat = re.compile(r"`(?:%s)(?:\(\))?`" % "|".join(map(re.escape, names)))
    out = []
    for doc in FILES:
        for heading, line, text in blocks(doc):
            if not pat.search(text):
                continue
            if text.startswith("|"):  # a row: | `file` | `function` | address | difference |
                cells = [c.strip() for c in text.strip("|").split(" | ")]
                if len(cells) >= 4 and pat.fullmatch(cells[1]):
                    out.append((doc, line, heading, cut(f"{cells[0]}: " + " | ".join(cells[3:]), None, ROW)))
                    continue
            elif text.startswith("- "):
                text = text[2:]
            out.append((doc, line, heading, cut(text, pat)))
            if len(out) >= MOST:
                return out
    return out


def brief_block(names):
    """The brief's block of pokebw2's notes on the function; "" where its docs name it nowhere."""
    found = notes(names)
    if not found:
        return ""
    shown = ", ".join(f"`{n}`" for n in dict.fromkeys(n for n in names if n))
    lines = [f"pokebw2's notes naming {shown} (~/ref/pokebw2/docs/, cut short; addresses there are Black 2's; a lead "
             "to try, not a rule):"]
    for doc, line, heading, text in found:
        lines.append(f"- {doc}:{line}" + (f" ({heading})" if heading and doc == "matching.md" else "") + f": {text}")
    return "\n".join(lines)


if __name__ == "__main__":
    print(brief_block(sys.argv[1:]) or "(no notes)")
