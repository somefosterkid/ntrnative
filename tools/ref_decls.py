#!/usr/bin/env python3
"""The declarations of the references' headers: the types, constants, variables and prototypes a ported function needs.

    python3 tools/ref_decls.py FSResult CARD_TARGET_ROM            # what the references define under those names
    python3 tools/ref_decls.py --check      # every ref_decls block in the tree against the references' values

tools/ref_port.py puts a reference's function into Black's C; the types, enumerators, macros, externs and prototypes
its body names are in the reference's headers, and where Black's headers lack them the function does not compile. This
indexes the headers of the checkouts tools/lib_twins.py knows (the preprocessor's choices for a final ROM made,
port_lib.unifdef) and gives, for a set of names, the definitions Black lacks in the order a C file needs them
(closure()). The definitions are written in the repo's style (declaration()): the comments go, one member a line.
No license: read, not copied, as for the functions (docs/plan.md, 2026-10-13).
"""
import glob
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cfmt  # noqa: E402
import lib_twins  # noqa: E402
import port_lib as pl  # noqa: E402
from file_ranges import ROOT  # noqa: E402

REF = lib_twins.REF
QUOTES = (chr(34), chr(39))
STRUCTURAL = {"typedef", "struct", "union", "enum", "static", "extern", "const", "volatile", "unsigned", "signed",
              "int", "char", "short", "long", "void", "float", "double", "inline", "sizeof", "if", "else", "for",
              "while", "do", "switch", "case", "default", "return", "goto", "break", "continue", "register"}
_index, _black = {}, {}


class Def:
    """One top-level declaration of a reference header: kind macro, type, var or proto; its text, the names it
    defines and the words it uses."""

    def __init__(self, kind, text, names, source, rel):
        self.kind, self.text, self.names, self.source, self.rel = kind, text, names, source, rel
        self.members = []  # the enumerators of an enum, among names
        self.uses = set(re.findall(r"[A-Za-z_]\w*", re.sub(r"^#\s*define\s+\w+", "", text))) - STRUCTURAL - set(names)


def strip_comments(text):
    return re.sub(r"/@*.*?@*/|//[^@n]*".replace("@", chr(92)), " ", text, flags=re.S)


def chunks(text):
    """The top-level statements of header text without comments: preprocessor lines and declarations ending at `;`
    (function bodies are skipped)."""
    out, cur, depth, i, n = [], "", 0, 0, len(text)
    at_line = True
    while i < n:
        c = text[i]
        if at_line and depth == 0 and not cur.strip() and re.match(r"[ @t]*#".replace("@", chr(92)), text[i:i + 40]):
            j = i
            while True:
                e = text.find(chr(10), j)
                e = n if e < 0 else e
                if e > j and text[e - 1] == chr(92) and e < n:
                    j = e + 1
                    continue
                break
            out.append(text[i:e].strip())
            i = e + 1
            continue
        at_line = c == chr(10) or (at_line and c in " @t".replace("@", chr(92)))
        if c in QUOTES:
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == chr(92) else 1
            cur += text[i:j + 1]
            i = j + 1
            continue
        if c in "({[":
            depth += 1
        elif c in ")}]":
            depth -= 1
        cur += c
        i += 1
        if depth == 0 and (c == ";" or c == "}" and re.search(r"@)[ @t@n]*@{".replace("@", chr(92)), cur) and
                           not re.match(r"[ @t@n]*(typedef|struct|union|enum)@b".replace("@", chr(92)), cur)):
            if c == ";":
                out.append(cur.strip())
            cur = ""
    return out


def enumerators(body):
    out, depth, cur = [], 0, ""
    for ch in body:
        depth += (ch == "(") - (ch == ")")
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    out.append(cur)
    return [m[1] for e in out for m in [re.match(r"s*([A-Za-z_]w*)".replace("s", chr(92) + "s").replace("w", chr(92) + "w"), e)] if m]


def classify(chunk, source, rel):
    """A Def for a chunk of a header, or None."""
    one = " ".join(chunk.replace(chr(92) + chr(10), " ").split())
    m = re.match(r"#\s*define\s+(\w+)(\([^)]*\))?\s*(.*)$", one)
    if m:
        params = set(re.findall(r"[A-Za-z_]\w*", m[2] or ""))
        d = Def("macro", f"#define {m[1]}{m[2] or ''} {m[3]}".rstrip(), [m[1]], source, rel)
        d.uses -= params
        return d
    if chunk.startswith("#") or not chunk.endswith(";"):
        return None
    flat = one
    body = re.match(r"(typedef\s+)?(enum|struct|union)\s*(\w*)\s*\{(.*)\}\s*([^;{}]*);$", flat, re.S)
    if body:
        td, kind, tag, inner, tail = body.groups()
        names = [n for n in re.findall(r"[A-Za-z_]\w*", tail)] if td else []
        if tag:
            names.append(tag)
        members = enumerators(inner) if kind == "enum" else []
        d = Def("type", chunk, names + members, source, rel) if names + members else None
        if d:
            d.members = members
        return d
    if flat.startswith("typedef"):
        fp = re.search(r"\(\s*\*\s*(\w+)\s*\)", flat)
        name = fp[1] if fp else (re.findall(r"[A-Za-z_]\w*", re.sub(r"\[[^\]]*\]", "", flat[:-1])) or [None])[-1]
        return Def("type", chunk, [name], source, rel) if name else None
    if flat.startswith("extern") and "(" not in flat.split("=")[0]:
        name = re.findall(r"[A-Za-z_]\w*", re.sub(r"\[[^\]]*\]", "", flat[:-1]))[-1]
        return Def("var", chunk, [name], source, rel)
    proto = re.match(r"(?:extern\s+)?[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(", flat)
    if proto and flat.endswith(");") and proto[1] not in STRUCTURAL:
        return Def("proto", chunk, [proto[1]], source, rel)
    return None


def resolve(raw):
    """A header with the preprocessor's choices for a final ROM made, and its other conditionals taken as undefined (a
    macro the SDK does not set), as tools/ref_port.py does for a function."""
    saved = set(pl.UNDEFINED)
    try:
        text = pl.unifdef(raw)
        for _ in range(3):
            names = {w for l in text.split(chr(10)) if re.match(r"s*#s*(?:if|ifdef|ifndef|elif)b".replace("s", chr(92) + "s")
                                                              .replace("b", chr(92) + "b"), l)
                     for w in re.findall(r"[A-Za-z_]w*".replace("w", chr(92) + "w"), re.sub(r"^s*#s*w+".replace("s", chr(92) + "s")
                                                                        .replace("w", chr(92) + "w"), "", l)) if w != "defined"}
            if not names:
                break
            pl.UNDEFINED.update(names)
            text = pl.unifdef(raw)
        return text
    finally:
        pl.UNDEFINED.clear()
        pl.UNDEFINED.update(saved)


def headers_of(source):
    out = []
    for inc in lib_twins.SOURCES[source][2]:
        if not inc.startswith(source):
            continue
        for d in sorted(glob.glob(f"{REF}/{inc.removesuffix('/**')}")):
            out += sorted(glob.glob(f"{d}/**/*.h", recursive=True))
    return list(dict.fromkeys(out))


def files_source(paths):
    """A source for index(), lookup() and closure() that is a list of files instead of a checkout's headers: a pokebw2
    file and the headers it includes (tools/ref_port.py's bw2_source), whose types and macros are then found too."""
    return ("files", tuple(paths))


def index(source):
    """{name: Def} of the headers of one checkout (or of the files of a files_source)."""
    if source not in _index:
        found = {}
        for f in (source[1] if isinstance(source, tuple) else headers_of(source)):
            try:
                text = strip_comments(resolve(pl.read_text(f)))
            except (OSError, UnicodeError):
                continue
            for c in chunks(text):
                d = classify(c, source, os.path.relpath(f, REF))
                if d:
                    for n in d.names:
                        old = found.get(n)
                        # typedef struct X X; and struct X {...}; apart (pokebw2's headers): the typedef is the
                        # name's, and needs the body, found under "struct X"
                        if old is not None and old is not d and ("{" in d.text) != ("{" in old.text):
                            fwd, body = (old, d) if "{" in d.text else (d, old)
                            if fwd.text.lstrip().startswith("typedef"):
                                found[n], found["struct " + n] = fwd, body
                                fwd.uses.add("struct " + n)
                                continue
                        found.setdefault(n, d)
        _index[source] = found
    return _index[source]


def lookup(name, sources):
    for s in sources:
        d = index(s).get(name)
        if d:
            return d
    return None


def declaration(d):
    """The definition in the repo's style: a macro as one line, a type with one member (or enumerator) a line."""
    if d.kind == "macro":
        return d.text
    toks = cfmt.tokens(d.text)
    open_at = next((i for i, (k, s) in enumerate(toks) if k == "op" and s == "{"), None)
    if open_at is None:
        return cfmt.expr(toks[:-1], decl=True) + ";"
    depth, close_at = 0, None
    for i in range(open_at, len(toks)):
        if toks[i][0] == "op" and toks[i][1] in ("{", "("):
            depth += 1
        elif toks[i][0] == "op" and toks[i][1] in ("}", ")"):
            depth -= 1
            if depth == 0:
                close_at = i
                break
    head = cfmt.expr(toks[:open_at], decl=False)
    is_enum = any(k == "id" and s == "enum" for k, s in toks[:open_at])
    members, cur, depth = [], [], 0
    for tok in toks[open_at + 1:close_at]:
        if tok[0] == "op" and tok[1] in ("{", "("):
            depth += 1
        elif tok[0] == "op" and tok[1] in ("}", ")"):
            depth -= 1
        if depth == 0 and tok[0] == "op" and tok[1] == ("," if is_enum else ";"):
            members.append(cur)
            cur = []
        else:
            cur.append(tok)
    if cur:
        members.append(cur)
    lines = [cfmt.expr(m, decl=not is_enum) + ("," if is_enum else ";") for m in members if m]
    tail = cfmt.expr(toks[close_at + 1:-1], decl=True)
    return (head + " {" + chr(10) + chr(10).join("    " + l for l in lines) + chr(10) + "}" +
            (" " + tail if tail else "") + ";")


UNRESOLVED = []  # (enumerator, header, why) of each constant partial_enum left out, as it was told


def partial_enum(d, known, sources=None):
    """A Def of an anonymous enum with the enumerators of d that known() lacks, at the values d gives them. The
    values come from ref_values (a name in an expression is read from the headers); an enumerator whose value cannot
    be worked out is left out, noted in UNRESOLVED and on stderr, never written at a guessed value."""
    import ref_values as rv
    if "black" not in _black:
        _black["black"] = rv.Black()
    srcs = [s for s in sources or () if isinstance(s, str)] or [
        "ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-NitroDWC", "libntr"]
    why = {}
    got = rv.enum_values(d.text, rv.Values(srcs, _black["black"]), lambda n, w: why.setdefault(n, w))
    out = []
    for name, expr in rv.enum_items(d.text):
        if known(name):
            continue
        if name not in got:
            UNRESOLVED.append((name, d.rel, why.get(name, "no value")))
            print(f"ref_decls: {name} ({d.rel}) left out: {why.get(name, 'no value')}", file=sys.stderr)
            continue
        out.append(f"{name} = {got[name]:#x}" if expr and "0x" in expr.lower() else f"{name} = {got[name]}")
    tail = re.search(r"\}\s*(\w+)\s*;$", " ".join(d.text.split()))
    typedef = tail[1] if tail and d.text.lstrip().startswith("typedef") and not known(tail[1]) else None
    if not out and not typedef:
        return None
    text = ("typedef enum { " + ", ".join(out) + " } " + typedef + ";") if out and typedef else \
        f"typedef int {typedef};" if typedef else "enum { " + ", ".join(out) + " };"
    new = Def("type", text, list(d.members), d.source, d.rel)
    new.members = list(d.members)
    return new


def black_words():
    """Every word of Black's headers: a name in it is declared (or defined) somewhere a C file can see."""
    if not _black:
        words = set()
        for g in ("lib/*/include/**/*.h", "include/**/*.h", "lib/*/sdk/**/*.h", "src/**/*.h"):
            for f in glob.glob(f"{ROOT}/{g}", recursive=True):
                words |= set(re.findall(r"[A-Za-z_]w*".replace("w", chr(92) + "w"), open(f, errors="replace").read()))
        _black["words"] = words
    return _black["words"]


def closure(names, sources, known=None):
    """The Defs a C file needs for `names` (those known(word) does not say the file has already), uses first."""
    known = known or (lambda w: False)
    seen, out = set(), []

    def visit(name):
        d = lookup(name, sources)
        if not d or id(d) in seen or known(name):
            return
        seen.add(id(d))
        if d.members and any(known(n) for n in d.names if n != name):
            d = partial_enum(d, known, sources)  # some of its names are Black's (a macro, the type): only the others
            if d is None:
                return
        for w in sorted(d.uses):
            visit(w)
        out.append(d)
    for n in names:
        visit(n)
    return out


def block_of(text):
    """The text of the declarations of a file's ref_decls block (after its two comment lines), or None."""
    marker = "// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they"
    if marker not in text:
        return None
    at = text.index(marker)
    end = text.find(chr(10) + chr(10), at)
    block = text[at:end if end >= 0 else len(text)].split(chr(10))[2:]
    return chr(10).join(block)


def check_file(path, sources, values, rv):
    """[problem] for the constants of the file's ref_decls block that differ from the references' headers: an
    enumerator or a macro whose value is not the one the reference gives (or has no reference)."""
    text = Path(path).read_text(errors="replace")
    block = block_of(text)
    if block is None:
        return []
    local = rv.Black(files=[path])

    def resolve(name):
        try:
            return local(name)
        except rv.Unresolved:
            return values(name)
    problems = []
    for c in chunks(strip_comments(block)):
        d = classify(c, "block", str(path))
        if d is None or not (d.kind == "macro" or d.members):
            continue
        if d.kind == "macro":
            mine, ref = rv.macro_body(d.text), lookup(d.names[0], sources)
            if ref is None or ref.kind != "macro" or mine is None:
                continue
            try:
                a = rv.Evaluator(mine, resolve).value()
                b = rv.Evaluator(rv.macro_body(ref.text), values).value()
            except rv.Unresolved:
                continue
            if a != b:
                problems.append(f"{path}: {d.names[0]} is {a:#x}, the reference's {ref.rel} has {b:#x}")
            continue
        mine = rv.enum_values(d.text, resolve)
        for name in d.members:
            ref = lookup(name, sources)
            if ref is None or name not in ref.members:
                problems.append(f"{path}: {name} is in no reference header")
                continue
            try:
                theirs = rv.enum_values(ref.text, values)
            except rv.Unresolved:
                continue
            if name not in theirs:
                problems.append(f"{path}: {name} has no value in {ref.rel}")
            elif mine.get(name) != theirs[name]:
                got = f"{mine[name]:#x}" if name in mine else "no value"
                problems.append(f"{path}: {name} is {got}, the reference's {ref.rel} has {theirs[name]:#x}")
    return problems


def check_tree(sources):
    """Every ref_decls block in the tree against the references: [problem]."""
    import ref_values as rv
    values = rv.Values(sources, rv.Black())
    out = []
    for g in ("src/**/*.c", "lib/**/*.c", "include/**/*.h", "lib/**/*.h"):
        for f in sorted(glob.glob(f"{ROOT}/{g}", recursive=True)):
            try:
                if "tools/ref_decls.py" in open(f, errors="replace").read():
                    out += check_file(f, sources, values, rv)
            except OSError:
                pass
    return out


def main():
    srcs = ["ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-NitroDWC", "libntr"]
    if sys.argv[1:] == ["--check"]:
        problems = check_tree(srcs)
        print(chr(10).join(problems) if problems else "every ref_decls block matches the references")
        sys.exit(1 if problems else 0)
    for d in closure(sys.argv[1:], srcs):
        print(f"// {d.source}: {d.rel}")
        print(declaration(d))


if __name__ == "__main__":
    main()
