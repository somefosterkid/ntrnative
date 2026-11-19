#!/usr/bin/env python3
"""A compact index of the route pilot's code, to read first (then only the line ranges you need):

    python3 tools/route_index.py                 # every file below, about 230 lines
    python3 tools/route_index.py route_map.c     # only that file (a name as listed, a glob under the repo, or any C file or
                                                 # header's path); route_test.c brings the token tables
    python3 tools/route_index.py --token W       # only the pilot tokens starting with W (then G: for the goal words), each
                                                 # with where its called functions are defined: FILE LINE

For each port/src/route*.c and route*.h and port/include/port_route*.h (route_test.c first; every such file is listed, found
by glob so a new one needs no change here): its section dividers, functions, types and variables (definitions; prototypes only in headers) with
the line they start on and the first line of their comment (the comment above, else one after the signature, else the first
one in the body). Then the pilot's token table: each token of route_test.c's Pilot() dispatch with its line and the
functions its branch calls, and the G: goal words of PortRoute_ParseGoal with the places each GOAL_ kind is used.
A line is "LINE kind name  comment" (kind: fn, static fn, decl, type, var).
"""
import argparse
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FILES = ["port/src/route_test.c", "port/src/route*.c", "port/src/route*.h", "port/include/port_route*.h"]
WIDTH = 118
HELPERS = {"NextTok", "PressEvent", "SetKeys", "fprintf", "atoi", "strncmp", "sizeof", "OS_GetVBlankCount"}


def mask(text):
    """(code, comments): the text with comments, strings, character literals and preprocessor lines blanked (newlines kept),
    and {line: comment text} for each comment's first line."""
    out, comments = [], {}
    i, n, line = 0, len(text), 1
    bol = True   # only blanks since the line start (a '#' there starts a preprocessor line)
    while i < n:
        c = text[i]
        two = text[i:i + 2]
        if two == "//" or two == "/*":
            end = text.find("\n", i) if two == "//" else text.find("*/", i + 2)
            end = n if end < 0 else (end if two == "//" else end + 2)
            body = text[i + 2:end - (2 if two == "/*" and text[end - 2:end] == "*/" else 0)]
            first = next((x.strip(" *\t") for x in body.split("\n") if x.strip(" *\t")), "")
            comments.setdefault(line, first)
            out.append(re.sub(r"[^\n]", " ", text[i:end]))
            line += text.count("\n", i, end)
            i = end
            continue
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            out.append(c + " " * (j - i - 1) + (c if j < n and text[j] == c else ""))
            i = j + 1
            bol = False
            continue
        if c == "#" and bol:
            j = i
            while j < n and text[j] != "\n":
                j += 2 if text[j] == "\\" and text[j + 1:j + 2] == "\n" else 1
            seg = text[i:j]
            out.append(re.sub(r"[^\n]", " ", seg))
            line += seg.count("\n")
            i = j
            continue
        out.append(c)
        if c == "\n":
            line += 1
            bol = True
        elif not c.isspace():
            bol = False
        i += 1
    return "".join(out), comments


def line_of(code, idx):
    return code.count("\n", 0, idx) + 1


def items(path, text, is_header):
    """[(line, kind, name, comment)] of the top level of a C file."""
    code, comments = mask(text)
    lines = text.split("\n")
    res = []
    depth, start, i, n = 0, 0, 0, len(code)
    pending = None   # a type definition whose declared name follows its closing brace
    body_open = 0

    def comment_for(first, body_line):
        if first in comments and comments[first] and not lines[first - 1].lstrip().startswith("//"):
            return comments[first]
        k = first - 1   # the // block just above: its first line
        top = None
        while k >= 1 and lines[k - 1].lstrip().startswith("//") and not re.match(r"^\s*// -{3,}", lines[k - 1]):
            top = k
            k -= 1
        if top:
            return comments.get(top, "")
        for ln in range(first, body_line + 1):   # trailing, on a line of the signature
            if ln in comments and not lines[ln - 1].lstrip().startswith("//"):
                return comments[ln]
        nxt = body_line + 1   # the body's first line, when it is a comment
        if "{" in lines[body_line - 1] and nxt in comments and lines[nxt - 1].lstrip().startswith("//"):
            return comments[nxt]
        return ""

    def first_code_line(a):
        while a < n and code[a].isspace():
            a += 1
        return a

    while i < n:
        c = code[i]
        if c == "{":
            if depth == 0:
                head = code[start:i]
                body_open = i
                pending = None
                h = " ".join(head.split())
                m = re.search(r"([A-Za-z_]\w*)\s*\(", h) if "(" in h and "=" not in h.split("(")[0] else None
                if m and not re.match(r"^(typedef\s+)?(struct|union|enum)\b[^()]*$", h):
                    a = first_code_line(start)
                    nm = m.group(1)
                    nm_idx = code.find(nm, a)
                    kind = "static fn" if re.match(r"^(static|inline)\b", h) else "fn"
                    res.append([line_of(code, nm_idx), kind, nm, comment_for(line_of(code, a), line_of(code, i)), line_of(code, i)])
                elif re.match(r"^(typedef\s+)?(struct|union|enum)\b", h) and "=" not in h:
                    tag = re.match(r"^(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)", h)
                    a = first_code_line(start)
                    pending = [line_of(code, a), "type", tag.group(1) if tag else "", comment_for(line_of(code, a), line_of(code, i)),
                               h.startswith("typedef")]
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                if pending is None:   # a function body or an initialiser: the next item starts after it (an initialiser's ';' too)
                    j = i + 1
                    if res and res[-1][1] in ("fn", "static fn") and res[-1][4] == line_of(code, body_open):
                        res[-1].append(line_of(code, i))   # [5]: the closing line
                    if "=" in " ".join(code[start:body_open].split()):
                        while j < n and code[j] != ";":
                            j += 1
                        a = first_code_line(start)
                        mm = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*=", " ".join(code[start:body_open].split()))
                        if mm and not re.match(r"^static\b", " ".join(code[start:body_open].split())):
                            res.append([line_of(code, code.find(mm.group(1), a)), "var", mm.group(1),
                                        comment_for(line_of(code, a), line_of(code, a)), line_of(code, a)])
                        j += 1
                    start = j
                    i = j
                    continue
                j = i + 1   # a type: up to the ';', which may name a typedef
                while j < n and code[j] != ";":
                    j += 1
                tail = code[i + 1:j].strip()
                if pending[4] and tail:
                    pending[2] = re.findall(r"\w+", tail)[-1]
                if pending[2]:
                    res.append(pending[:4])
                pending = None
                start = j + 1
                i = j + 1
                continue
        elif c == ";" and depth == 0:
            h = " ".join(code[start:i].split())
            a = first_code_line(start)
            ln = line_of(code, a)
            if h:
                if re.match(r"^typedef\b", h):
                    mm = re.search(r"\(\s*\*\s*(\w+)\s*\)", h) or re.search(r"(\w+)\s*(?:\[[^\]]*\])?$", h)
                    if mm and not re.match(r"^typedef\s+(struct|union|enum)\s+\w+\s+\w+$", h):
                        res.append([ln, "type", mm.group(1), comment_for(ln, ln)])
                elif "(" in h and "=" not in h.split("(")[0]:
                    if is_header and not re.match(r"^static\b", h):
                        fp = re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", h)   # a function pointer variable
                        mm = fp or re.search(r"([A-Za-z_]\w*)\s*\(", h)
                        if mm and mm.group(1) not in ("__attribute__",):
                            res.append([ln, "var" if fp else "decl", mm.group(1), comment_for(ln, ln)])
                elif not re.match(r"^(struct|union|enum)\s+\w+$", h):
                    mm = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:=.*)?$", h)
                    if mm and not re.match(r"^static\b", h):   # (a file's own statics are not its interface)
                        res.append([ln, "var", mm.group(1), comment_for(ln, ln)])
            start = i + 1
        i += 1
    for ln in sorted(comments):   # section dividers: // ---- name ----
        t = lines[ln - 1].strip()
        if re.match(r"^// -{3,}", t):
            res.append([ln, "sect", re.sub(r"^//\s*-+\s*|\s*-+\s*$", "", t), ""])
    res.sort(key=lambda r: (r[0], r[1] != "sect"))
    # (a short static function with no comment says nothing its name does not: left out)
    return [tuple(r[:4]) for r in res if not (r[1] == "static fn" and not r[3] and len(r) > 5 and r[5] - r[0] < 10)]


def header_comment(text):
    """(last line, first line of text) of a file's leading // comment block, or None."""
    ls = text.split("\n")
    k = 0
    while k < len(ls) and ls[k].startswith("//"):
        k += 1
    return (k, ls[0].lstrip("/ ")) if k >= 5 else None


def function_span(text, name):
    """(first line, last line) of the top-level function `name`, by brace counting on the masked text."""
    code, _ = mask(text)
    m = re.search(r"^[A-Za-z_][^;{}\n]*\b" + name + r"\s*\([^;{]*\)\s*\{", code, re.M)
    if not m:
        return None
    depth, i = 0, m.end() - 1
    while i < len(code):
        depth += code[i] == "{"
        depth -= code[i] == "}"
        if depth == 0:
            return line_of(code, m.start()), line_of(code, i)
        i += 1
    return None


def token_table(text, funcs):
    """[(line, token, called functions)] of route_test.c's Pilot(): `if (*sTok == 'X' ...)` and `case 'X':` branches."""
    span = function_span(text, "Pilot")
    if not span:
        return [], []
    code, comments = mask(text)
    cl = code.split("\n")
    raw = text.split("\n")   # (the masked lines have the character literals blanked)
    rows, notes = [], []
    lo, hi = span
    for ln in range(lo, hi + 1):
        s = raw[ln - 1]
        m = re.match(r"^    if \(\*sTok == '(.)'\s*(.*?)\) \{\s*(//.*)?$", s) or re.match(r"^    case '(.)':\s*(//.*)?", s)
        if not m:
            if ln in comments and re.match(r"^    //", s) and not re.match(r"^\s*//", raw[ln - 2]):
                notes.append((ln, comments[ln]))
            continue
        cond = m.group(2) if m.re.pattern.startswith("^    if") else ""
        subs = "".join(re.findall(r"sTok\[1\] == '(.)'", cond))
        tok = m.group(1) + (":" if ":" == subs else (f"[{subs}]" if len(subs) > 1 else subs))
        if m.re.pattern.startswith("^    if"):   # the branch: up to the matching close at depth 1
            end = ln
            while end <= hi and not re.match(r"^    \}", cl[end - 1]):
                end += 1
        else:   # a case: up to the next case or the switch's end
            end = ln + 1
            while end <= hi and not re.match(r"^    (case |\})", cl[end - 1]):
                end += 1
        called = []
        for x in re.findall(r"\b([A-Za-z_]\w*)\s*\(", "\n".join(cl[ln - 1:end])):
            if x in funcs and x not in HELPERS and x not in called and x != "Pilot":
                called.append(x)
        rows.append((ln, tok, called, (m.group(3) if m.re.pattern.startswith("^    if") else m.group(2)) or ""))
    return rows, notes


def goal_table(files):
    """[(word, GOAL_ kind)] from PortRoute_ParseGoal's kWords, and {GOAL_ kind: [(file, line)]} where each is used."""
    words, uses = [], {}
    for rel, text in files.items():
        if rel.endswith("route_map.c"):
            m = re.search(r"kWords\[\]\s*=\s*\{(.*?)\};", text, re.S)
            if m:
                words = re.findall(r'\{"(\w+:)",\s*(GOAL_\w+)', m.group(1))
            if "G:rail:" in text:
                words.append(("rail:", "GOAL_RAIL"))
            words.append(("X,Z", "GOAL_TILE"))
    for rel, text in files.items():
        for ln, s in enumerate(text.split("\n"), 1):
            for k in re.findall(r"\bGOAL_\w+\b", s):
                uses.setdefault(k, []).append((os.path.basename(rel), ln))
    return words, uses


def route_files():
    """The files the index is about, as paths relative to the repo: FILES' globs, each file once, route_test.c first."""
    paths = []
    for p in FILES:
        for g in sorted(glob.glob(os.path.join(ROOT, p))):
            rel = os.path.relpath(g, ROOT).replace(os.sep, "/")
            if rel not in paths:
                paths.append(rel)
    return paths


def resolve(arg, known):
    """One file argument: a route file by name (route_map.c), a glob under the repo, or any path; a list of paths."""
    hit = [k for k in known if k == arg or os.path.basename(k) == arg]
    if not hit:
        hit = [os.path.relpath(g, ROOT) for g in sorted(glob.glob(os.path.join(ROOT, arg)))]
    if not hit and os.path.exists(arg):
        a = os.path.abspath(arg)
        hit = [os.path.relpath(a, ROOT) if a.startswith(ROOT + os.sep) else a]
    if not hit:
        sys.exit(f"route_index: no file {arg} (the route files: {', '.join(os.path.basename(k) for k in known)})")
    return hit


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="show only these files (names, globs or paths); default: all")
    ap.add_argument("--token", metavar="X", help="show only the pilot tokens that start with X (not the file listings)")
    args = ap.parse_args(argv[1:])
    known = route_files()
    # everything is indexed (the token table needs the functions of every file); the arguments choose what is printed
    shown = [p for a in args.files for p in resolve(a, known)] if args.files else list(known)
    out, files, funcs, where = [], {}, set(), {}
    sections = {}
    for rel in dict.fromkeys(known + shown):
        text = open(os.path.join(ROOT, rel) if not os.path.isabs(rel) else rel, encoding="utf-8", errors="replace").read()
        files[rel] = text
        its = items(rel, text, rel.endswith(".h"))
        funcs |= {nm for _, k, nm, _ in its if k in ("fn", "static fn")}
        for ln, k, nm, _ in its:
            if k in ("fn", "static fn"):
                where.setdefault(nm, (os.path.basename(rel), ln))
        sec = [f"== {rel} ({text.count(chr(10)) + 1} lines)"]
        hc = header_comment(text)
        if hc:
            sec.append(f"{1:5} header comment, lines 1-{hc[0]}: {hc[1]}"[:WIDTH])
        for ln, kind, nm, cm in its:
            if kind == "sect":
                sec.append(f"{ln:5} -- {nm}"[:WIDTH])
            else:
                sec.append((f"{ln:5} {kind:9} {nm}" + (f"  {cm}" if cm else ""))[:WIDTH])
        sections[rel] = sec
    pilot = "port/src/route_test.c"
    if not args.token:
        for rel in shown:
            out += sections[rel]
    if pilot in files and (args.token or pilot in shown):
        rows, notes = token_table(files[pilot], funcs)
        if args.token:
            rows = [r for r in rows if r[1].startswith(args.token)]
            notes = []
            out.append(f"== pilot tokens starting with {args.token} (route_test.c Pilot()); called functions as name (file line)")
        else:
            out.append("== pilot tokens (route_test.c Pilot(), BWPORT_PILOT; the syntax is in the header comment above)")
        for ln, tok, called, cm in rows:
            names = [f"{c} ({where[c][0]} {where[c][1]})" for c in called] if args.token else called
            out.append(f"{ln:5} {tok:8} -> {', '.join(names) if names else '(inline)'}  {cm.lstrip('/ ')}"[:WIDTH + (60 if args.token else 0)])
        for ln, cm in notes:
            out.append(f"{ln:5} Pilot stage: {cm}"[:WIDTH])
        words, uses = goal_table(files)
        if words and (not args.token or args.token == "G"):
            out.append("== G: goals (PortRoute_ParseGoal; GoStep walks them): word -> kind, where the kind is used")
            for w, k in words:
                at = {}
                for f, ln in uses.get(k, []):
                    at.setdefault(f, []).append(str(ln))
                out.append(f"      G:{w:6} {k:10} " + "  ".join(f"{f} {','.join(v[:5])}" for f, v in at.items()))
        if args.token and not rows and args.token != "G":
            sys.exit(f"route_index: no pilot token starts with {args.token}")
    print("\n".join(out))


if __name__ == "__main__":
    main(sys.argv)
