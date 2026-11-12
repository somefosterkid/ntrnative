#!/usr/bin/env python3
"""Header prototypes that disagree with the function's definition.

A worker who adds or changes a prototype in a header the defining file doesn't include gets no compiler error: the
header says `u32 f(Field *)`, the C defines `void f(Field *, s32)`, and every caller that includes the header calls it
wrong (weather_task.h, field_wfbc.h, ov021_0219d524.h in batch 50). This compares each function a header under
include/ or lib/*/include/ declares with its definition in src/ or lib/*/src/: the return type and each parameter's
type (parameter names, `const` on a value parameter, `struct`, `extern` and whitespace aside; `(void)` and `()` are
one). A function defined in no C file, a static one, or one whose parameter list has a function pointer in it is
skipped.

    python3 tools/proto_check.py                       # every mismatch
    python3 tools/proto_check.py include/field/weather_task.h ...   # the functions these headers declare
    python3 tools/proto_check.py --files src/ov021/weather_task.c   # the functions these C files define
    python3 tools/proto_check.py --locals                  # prototypes a C file declares itself that a header's disagrees with
    python3 tools/proto_check.py --locals src/ov009/x.c    # only those files'

--knr: a `()` declaration (a header's, or a C file's own) of a function a C file defines with parameters. The compiler
takes any call of it and passes the arguments unconverted (a u8 argument is not narrowed; a missing one is whatever
the register held), and finish.py refuses the header. --knr lists them with the calls that pass fewer arguments than
the definition has parameters (those need their arguments by hand; tools/outside_tweaks.py fills the ones the diff
shows); --knr --apply writes the definition's parameter list into each declaration, without a judge (tools/proto_fix.py
judges its own). Paths after --knr restrict it to the declarations in those files.

--locals: a C file's own prototype of a function a header declares differently (a draft's guess left in the file:
dwc_login.c and ov016_021b93f0.c in the o287 wave, `u32 f(u32 *)` against the header's `int f(DWCHttp *)`). The file's
callers and tools/draft_fill.py take its prototype, not the header's; the header's is the one to use.

Prototypes written inside C files are checked too (against the definition, else the header that declares the function).

tools/finish.py runs it on the headers a worker changed and the files it commits.
"""
import argparse, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
# a declaration or definition head: type, name, (params); a definition is followed by {
HEAD = re.compile(r"(?:^|(?<=[;}\n]))[ \t]*((?:(?:static|extern|inline|const|volatile|unsigned|signed|struct|union|enum)\s+)*"
                  r"[A-Za-z_]\w*(?:\s*\*+\s*|\s+)(?:const\s*\**\s*)?)([A-Za-z_]\w*)\s*\(([^()]*)\)\s*([;{])", re.M)
SKIP_WORDS = {"if", "while", "for", "switch", "return", "sizeof", "else", "do", "case", "typedef"}


def norm_type(t):
    t = re.sub(r"\b(extern|inline|struct|union|enum|register|volatile)\b", " ", t)
    t = re.sub(r"\s*\*\s*", "*", t.strip())
    return re.sub(r"\s+", " ", t).strip()


def norm_params(p):
    p = p.strip()
    if p in ("", "void"):
        return ()
    out = []
    for a in p.split(","):
        a = a.strip()
        if a == "...":
            out.append("...")
            continue
        a = re.sub(r"(\w+)\s*\[[^\]]*\]", r"* \1", a)  # an array parameter is a pointer
        m = re.match(r"(.*?[\s*])([A-Za-z_]\w*)$", a)
        t = m[1] if m and m[1].strip() and m[1].strip() not in ("struct", "union", "enum", "unsigned", "signed") else a
        t = norm_type(t)
        if "*" not in t:  # a const value parameter is the same parameter
            t = re.sub(r"\bconst\b", "", t).strip()
        out.append(re.sub(r"\s+", " ", t))
    return tuple(out)


def items(path, defs, spans=False):
    """(name, return type, parameter types, line) of the declarations (defs False) or definitions (defs True) of a
    file; with spans also (start of the return type, end of the parameter list) in the text."""
    blank = lambda m: re.sub(r"[^\n]", " ", m[0])  # same length, same lines
    text = COMMENT.sub(blank, path.read_text(errors="replace"))
    text = re.sub(r"^[ \t]*#.*$", blank, text, flags=re.M)
    for m in HEAD.finditer(text):
        ret, name, params, end = m[1], m[2], m[3], m[4]
        if name in SKIP_WORDS or (end == "{") != defs or "(" in params:
            continue
        if re.search(r"\b(typedef|return|else)\b", ret) or (defs and re.search(r"\bstatic\b", ret)):
            continue
        got = name, norm_type(re.sub(r"\b(static|const)\b(?=.*\*)?", lambda x: x[0] if x[0] == "const" else "", ret)), \
            norm_params(params), text.count("\n", 0, m.start(2)) + 1
        yield got + ((m.start(1), m.end(3)),) if spans else got


def headers():
    return [p for p in list((ROOT / "include").rglob("*.h")) + list(ROOT.glob("lib/*/include/**/*.h"))
            if "/sdk/" not in str(p)]


def c_files():
    return list((ROOT / "src").rglob("*.c")) + list(ROOT.glob("lib/*/src/**/*.c"))


def line_of(path, pos):
    return COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), path.read_text(errors="replace"))[:pos].count("\n") + 1


BASE = {"char": "s8", "signed char": "s8", "unsigned char": "u8", "short": "s16", "signed short": "s16",
        "unsigned short": "u16", "int": "s32", "signed int": "s32", "signed": "s32", "unsigned int": "u32",
        "unsigned": "u32", "long": "s32", "unsigned long": "u32", "long long": "s64", "unsigned long long": "u64",
        "s8": "s8", "u8": "u8", "s16": "s16", "u16": "u16", "s32": "s32", "u32": "u32", "s64": "s64", "u64": "u64",
        "void": "void", "float": "f32", "double": "f64", "bool": "u8"}
TYPEDEFS = {}


def typedefs():
    """name -> what it stands for, from every `typedef X Y;` and `typedef enum ... Y;` of the headers."""
    if not TYPEDEFS:
        for h in headers() + list(ROOT.glob("lib/*/sdk/**/*.h")):  # the SDK's enums (RTCResult, MICResult) too
            text = COMMENT.sub(" ", h.read_text(errors="replace"))
            for x, y in re.findall(r"\btypedef\s+((?:(?:unsigned|signed|const|volatile|struct|union|enum)\s+)*\w+"
                                   r"(?:\s*\*+)?)\s*(\w+)\s*;", text):
                TYPEDEFS.setdefault(y, x)
            for y in re.findall(r"\btypedef\s+enum\b[^;{]*\{[^}]*\}\s*(\w+)\s*;", text):
                TYPEDEFS.setdefault(y, "int")
    return TYPEDEFS


def abi(t):
    """What the compiler passes: a word (a pointer, a 32-bit integer), a narrower integer's width and sign, void, or a
    struct's name (by value). A u8 parameter where the definition takes a u32 changes every caller's code; an s32
    where it takes a pointer is a wrong type but the same code (--exact shows those)."""
    w = _abi(t)
    return "word" if w in ("ptr", "s32", "u32") else w


def _abi(t):
    t = re.sub(r"\b(const|volatile)\b", "", t).strip()
    for _ in range(10):
        if "*" in t:
            return "ptr"
        if t in BASE:
            return BASE[t]
        if t.startswith("enum") or t in ("BOOL",):
            return "s32"
        n = typedefs().get(t)
        if not n:
            return t
        t = norm_type(re.sub(r"\b(const|volatile)\b", "", n))
    return t


SPELL = {"signed int": "int", "signed": "int", "unsigned": "unsigned int", "signed short": "short",
         "signed long": "long", "short int": "short", "long int": "long", "unsigned short int": "unsigned short",
         "unsigned long int": "unsigned long"}


def canon(t, top=True):
    """The type as the compiler compares a redeclaration: typedefs resolved, a value parameter's const dropped, a
    pointer's target kept (void * and a struct pointer are two types: the defining file that includes the header
    fails to compile), an enum typedef as int (-enum int)."""
    t = norm_type(t)
    stars = t.count("*")
    base = t.replace("*", " ").strip()
    if top and not stars:
        base = re.sub(r"\bconst\b", "", base).strip()
    for _ in range(10):
        b = re.sub(r"\bconst\b", "", base).strip()
        n = typedefs().get(b)
        if not n:
            break
        if n == "int" and b != "int":  # an enum's typedef
            base = "int"
            break
        inner = norm_type(re.sub(r"\b(volatile)\b", "", n))
        stars += inner.count("*")
        base = ("const " if "const" in base.split() else "") + inner.replace("*", " ").strip()
    base = " ".join(base.split())
    const = "const " if "const" in base.split() and stars else ""
    base = re.sub(r"\bconst\b", "", base).strip()
    base = SPELL.get(base, base)
    return f"{const}{base}{'*' * stars}"


def check(hdrs, cfiles, only=None, exact=False):
    defs = {}
    for c in cfiles:
        for name, ret, params, pos in items(c, True):
            defs.setdefault(name, (c, ret, params, pos))
    bad = []
    for h in hdrs:
        for name, ret, params, pos in items(h, False):
            if name not in defs or (only is not None and name not in only):
                continue
            c, dret, dparams, dpos = defs[name]
            key = (lambda r, p: (canon(r), tuple(canon(x) for x in p))) if exact else \
                (lambda r, p: (abi(r), tuple(map(abi, p))))
            if key(ret, params) != key(dret, dparams):
                bad.append((h, pos, name, f"{ret} ({', '.join(params) or 'void'})", c,
                            f"{dret} ({', '.join(dparams) or 'void'})"))
    return bad


def declared_names(added_lines):
    """The function names finish.py checks of a header's added lines: the first `name(` of each (a `#` line gives
    none). finish.py's header_mismatches and proto_width.py's refused() both take them from here."""
    names = set()
    for line in added_lines:
        found = re.findall(r"\b([A-Za-z_]\w*)\s*\(", line)
        if found and not re.match(r"\s*#", line):
            names.add(found[0])
    return names


def refused(header, new_text, old_text, texts):
    """The mismatches finish.py would refuse (check(), exact) if header's text went from old_text to new_text: the
    declarations the new lines add against the definitions in `texts` ({C file path: text}, any superset of the files
    that define them). [] when it accepts."""
    import tempfile
    old = set(old_text.splitlines())
    names = declared_names(l for l in new_text.splitlines() if l not in old)
    if not names:
        return []
    mine = [p for p, t in texts.items() if p.suffix == ".c" and any(n in t for n in names)]
    if not mine:
        return []
    with tempfile.TemporaryDirectory() as d:
        tmp = Path(d) / header.name
        tmp.write_text(new_text)
        copies = []
        for i, c in enumerate(mine):
            copies.append(Path(d) / f"c{i}.c")
            copies[-1].write_text(texts[c])
        bad = check([tmp], copies, names, exact=True)
    return [(header,) + x[1:] for x in bad]


def check_locals(hdrs, cfiles, only=None, exact=False):
    """The prototypes written inside C files (a forward declaration or an `extern`) that disagree with the
    definition (any C file's) or, where nothing defines the function in C, with the header that declares it: the local
    prototype the compiler accepts because the header isn't included, and the callers' code is wrong (o287's
    dwc_login.c, o292's ov021_021a0138.h)."""
    defs, decls = {}, {}
    for c in cfiles:
        for name, ret, params, pos in items(c, True):
            defs.setdefault(name, []).append((c, ret, params, pos))
    for h in hdrs:
        for name, ret, params, pos in items(h, False):
            decls.setdefault(name, []).append((h, ret, params, pos))
    key = (lambda r, p: (canon(r), tuple(canon(x) for x in p))) if exact else \
        (lambda r, p: (abi(r), tuple(map(abi, p))))
    bad = []
    for c in cfiles:
        if only is not None and c not in only:
            continue
        for name, ret, params, pos in items(c, False):
            truth = defs.get(name) or decls.get(name)
            if not truth or any(key(ret, params) == key(r, p) for _, r, p, _ in truth):
                continue
            what = "defines" if name in defs else "declares"
            f, r, p, _ = truth[0]
            bad.append((c, pos, name, f"{ret} ({', '.join(params) or 'void'})", f,
                        f"{r} ({', '.join(p) or 'void'})", what))
    return bad


def blank(m):
    return re.sub(r"[^\n]", " ", m[0])


def call_args(text, pos):
    """The number of arguments of the call whose ( is text[pos]."""
    depth, args, seen = 0, 1, False
    for i in range(pos, len(text)):
        ch = text[i]
        depth += ch in "([{"
        depth -= ch in ")]}"
        if depth == 0:
            break
        if depth == 1 and ch == ",":
            args += 1
        seen = seen or (i > pos and not ch.isspace())
    return args if seen else 0


def knr(files=None):
    """[(path, line, name, (start, end) of the parameter list, the definition's parameters, [(file, line) of the calls
    with fewer arguments])] of the `()` declarations of functions a C file defines with parameters. files: only the
    declarations in these paths."""
    defs, texts = {}, {}
    for c in c_files():
        texts[c] = COMMENT.sub(blank, c.read_text(errors="replace"))
        for m in HEAD.finditer(texts[c]):
            if m[4] == "{" and m[2] not in SKIP_WORDS and "(" not in m[3] and m[3].strip() not in ("", "void") \
                    and not re.search(r"\b(typedef|return|else|static)\b", m[1]):
                defs.setdefault(m[2], (c, " ".join(m[3].split())))
    for h in headers():
        texts[h] = COMMENT.sub(blank, h.read_text(errors="replace"))
    out = []
    for f in headers() + c_files():
        if files is not None and f not in files:
            continue
        for m in HEAD.finditer(texts[f]):
            if m[4] != ";" or m[2] not in defs or m[3].strip() or re.search(r"\b(typedef|return|else)\b", m[1]):
                continue
            n = len(defs[m[2]][1].split(","))
            short = []
            for g, gt in texts.items():
                if m[2] not in gt:
                    continue
                for call in re.finditer(rf"(?<![\w.>]){m[2]}\s*\(", gt):
                    before = gt[gt.rfind("\n", 0, call.start()) + 1:call.start()]
                    if re.fullmatch(r"[ \t]*(?:(?:static|extern|inline|const)\s+)*\w+[\s*]*", before) and not \
                            re.search(r"\b(?:return|else)\s*$", before):
                        continue    # a declaration or definition: a type before the name
                    if call_args(gt, call.end() - 1) < n:
                        short.append((g, gt.count("\n", 0, call.start()) + 1))
            out.append((f, texts[f].count("\n", 0, m.start(2)) + 1, m[2], (m.start(3), m.end(3)), defs[m[2]][1], short))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--locals", action="store_true", help="the C files' own prototypes against the headers'")
    ap.add_argument("--knr", action="store_true", help="`()` declarations of functions defined with parameters")
    ap.add_argument("--apply", action="store_true", help="with --knr: write the definition's parameters there")
    ap.add_argument("headers", nargs="*")
    ap.add_argument("--files", nargs="+", default=[])
    ap.add_argument("--exact", action="store_true", help="every redeclaration the compiler would reject (a void * "
                                                         "for a struct pointer, an s32 for a pointer) if the defining "
                                                         "file included the header, not only those that change code")
    a = ap.parse_args()
    if a.knr:
        only = {ROOT / f for f in a.headers + a.files} or None
        rows = knr(only)
        edits = {}
        for f, line, name, span, params, short in rows:
            print(f"{f.relative_to(ROOT)}:{line}: {name}() is defined with ({params})" +
                  (f"; calls with fewer arguments: {', '.join(f'{g.relative_to(ROOT)}:{n}' for g, n in short[:6])}" if short else ""))
            edits.setdefault(f, []).append((span[0], span[1], params))
        if a.apply:
            for f, es in edits.items():
                text = f.read_text(errors="replace")
                for x, y, new in sorted(es, reverse=True):
                    text = text[:x] + new + text[y:]
                f.write_text(text)
                print(f"{f.relative_to(ROOT)}: {len(es)} prototype(s) written")
        return 1 if rows and not a.apply else 0
    if a.locals:  # the positional arguments are C files here
        only = {ROOT / f for f in a.headers + a.files} or None
        bad = check_locals(headers(), c_files(), only, a.exact)
        for c, n, name, decl, f, dfn, what in bad:
            print(f"{c.relative_to(ROOT)}:{n}: local prototype of {name} is {decl}; {f.relative_to(ROOT)} {what} {dfn}")
        return 1 if bad else 0
    hdrs = [ROOT / h for h in a.headers] or headers()
    only = None
    if a.files:
        only = {n for f in a.files for n, *_ in items(ROOT / f, True)}
        hdrs = a.headers and hdrs or headers()
    bad = check(hdrs, c_files(), only, a.exact)
    for h, n, name, decl, c, dfn in bad:
        print(f"{h.relative_to(ROOT)}:{n}: {name} declared {decl}; {c.relative_to(ROOT)} defines {dfn}")
    local = check_locals(headers(), c_files(), {ROOT / f for f in a.files} or None, a.exact)
    for c, n, name, decl, f, dfn, what in local:
        print(f"{c.relative_to(ROOT)}:{n}: local prototype of {name} is {decl}; {f.relative_to(ROOT)} {what} {dfn}")
    return 1 if bad or local else 0


if __name__ == "__main__":
    sys.exit(main())
