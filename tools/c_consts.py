#!/usr/bin/env python3
"""The values of the constant names (#define, enum) a C file of the vendored Black 2 / White 2 tree sees, for
tools/asset_table.py: a table of `SPECIES_MEW, MOVE_TACKLE | FLAG, 10 * 60` is inferred from the ROM's bytes only when
the names have values.

A small preprocessor: the file and what it #includes (found in the tree's include directories; a header that is not
there, such as the SDK's, is skipped), with #if/#ifdef/#else evaluated for the version's define. Object-like macros
and enums are read, and the SDK's function-like macros the tables use are known (FUNCS: FX32_CONST(1.5) as the
compiler rounds it, GX_RGB, GX_RGBA, GX_BG_SCRBASE, GX_BG_CHARBASE, NELEMS of an array of the file). Names that can not
be evaluated (another function-like macro, sizeof, a cast to a struct, a plain float) give None.

    python3 tools/c_consts.py --version b2_us FILE NAME...        # prints NAME = value
"""
import re
import sys
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VENDORED = ROOT / "third_party/pokebw2"
VERSION_DEFINE = {"b2_us": "BLACK2", "w2_us": "WHITE2"}
TYPES = {"u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64", "int", "long", "short", "char", "unsigned", "signed",
         "fx16", "fx32", "BOOL", "u_char", "size_t"}
FLOAT = re.compile(r"\s*((?:\d+\.\d*|\.\d+)(?:[eE][-+]?\d+)?|\d+[eE][-+]?\d+)[fF]?|\s*(\d+)[fF]\b")
FUNCS = re.compile(r"\b(FX32_CONST|FX16_CONST|GX_RGBA|GX_RGB|GX_BG_SCRBASE|GX_BG_CHARBASE|NELEMS)\s*\(")
TOKEN = re.compile(r"\s*(0[xX][0-9a-fA-F]+|\d+)[uUlL]*|\s*'(\\?.)'|\s*([A-Za-z_]\w*)|\s*(<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^~!<>()?:])")


def include_dirs():
    return [VENDORED / "include", VENDORED / "generated", *sorted((VENDORED / "lib").glob("*/include")),
            ROOT / "include", *sorted((ROOT / "lib").glob("*/include"))]


def strip_comments(text):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", text, flags=re.S)


class Consts:
    """name -> value for one C file."""

    def __init__(self, c_file, version):
        self.macros, self.enums, self.code = {}, {}, []
        self.defines = {VERSION_DEFINE.get(version, "BLACK2")}
        self.macros.update({d: "1" for d in self.defines})
        self.seen = set()
        self.cache = {}
        self.version, self.text = version, (ROOT / c_file).read_text(errors="replace") if (ROOT / c_file).exists() else ""
        self.read(ROOT / c_file)
        for body in re.findall(r"\benum\b[^{;]*\{([^{}]*)\}", "\n".join(self.code)):
            self.read_enum(body)

    def read(self, path):
        if path in self.seen or not path.exists():
            return
        self.seen.add(path)
        text = strip_comments(path.read_text(errors="replace")).replace("\\\n", " ")
        stack = []   # per open #if: [this branch is live, some branch was taken, the outer is live]
        live = lambda: all(s[0] for s in stack)
        for line in text.split("\n"):
            m = re.match(r"\s*#\s*(\w+)\s*(.*)", line)
            if not m:
                if live():
                    self.code.append(line)
                continue
            d, rest = m[1], m[2].strip()
            if d in ("if", "ifdef", "ifndef"):
                outer = live()
                v = (rest.split()[0] in self.macros if d == "ifdef" else rest.split()[0] not in self.macros
                     if d == "ifndef" else self.condition(rest)) if outer and rest else False
                stack.append([v, v, outer])
            elif d == "elif" and stack:
                s = stack[-1]
                s[0] = (not s[1]) and all(x[0] for x in stack[:-1]) and self.condition(rest)
                s[1] = s[1] or s[0]
            elif d == "else" and stack:
                s = stack[-1]
                s[0] = not s[1] and all(x[0] for x in stack[:-1])
                s[1] = True
            elif d == "endif" and stack:
                stack.pop()
            elif not live():
                continue
            elif d == "define":
                dm = re.match(r"(\w+)(\(?)\s*(.*)", rest)
                if dm:
                    self.macros[dm[1]] = None if dm[2] else dm[3]
            elif d == "undef":
                self.macros.pop(rest.split()[0], None)
            elif d == "include":
                im = re.match(r'["<]([^">]+)[">]', rest)
                if im and not im[1].startswith("asset/"):
                    for base in [path.parent, *include_dirs()]:
                        if (base / im[1]).exists():
                            self.read(base / im[1])
                            break

    def condition(self, expr):
        expr = re.sub(r"defined\s*\(\s*(\w+)\s*\)|defined\s+(\w+)", lambda m: "1" if (m[1] or m[2]) in self.macros else "0", expr)
        try:
            return bool(self.evaluate(expr, undefined=0))
        except (ValueError, KeyError, RecursionError):
            return False

    def read_enum(self, body):
        nxt = 0
        for item in (i.strip() for i in body.split(",")):
            if not item:
                continue
            name, _, expr = (p.strip() for p in item.partition("="))
            if expr:
                try:
                    nxt = self.evaluate(expr)
                except (ValueError, KeyError, RecursionError):
                    nxt = None
            if nxt is not None and re.fullmatch(r"[A-Za-z_]\w*", name):
                self.enums.setdefault(name, nxt)
                nxt += 1

    def lookup(self, name, depth, floats=False):
        if name in self.enums:
            return self.enums[name]
        if name in self.macros and self.macros[name] is not None:
            if depth > 20:
                raise RecursionError(name)
            return self.evaluate(self.macros[name], depth + 1, floats=floats)
        raise KeyError(name)

    def nelems(self, name):
        """NELEMS(name): the first dimension of an array the file defines: as declared, else its initialiser's elements
        (or, moved, its asset row's size)."""
        text = strip_comments(self.text)
        m = re.search(rf"\b{name}\s*((?:\[[^\]]*\]\s*)+)(?:ATTRIBUTE_ALIGN\(\d+\)\s*)?=\s*\{{", text)
        if not m:
            raise KeyError(name)
        dims = [d.strip() for d in re.findall(r"\[([^\]]*)\]", m[1])]
        if dims[0]:
            return self.evaluate(dims[0])
        depth, i, count, item = 1, m.end(), 0, False
        while depth:
            c = text[i]
            depth += (c in "{(") - (c in "})")
            if depth == 1 and c == ",":
                count, item = count + item, False
            elif depth and not c.isspace():
                item = True
            i += 1
        inc = re.search(r'#\s*include\s+"asset/(\w+)\.inc"', text[m.end():i])
        if inc:   # moved: the row's bytes over its record's (u16, or a layout such as u8,u16)
            import asset_table
            for line in (ROOT / "config" / self.version / "assets.tsv").read_text().splitlines():
                f = line.split("\t")
                if f[0] == inc[1]:
                    cols = asset_table.parse_layout(f[4])
                    inner = 1
                    for d in dims[1:] if len(cols) == 1 else []:   # a plain row holds every element of the inner arrays
                        inner *= self.evaluate(d)
                    return (int(f[3], 16) - int(f[2], 16)) // asset_table.offsets(cols)[1] // inner
            raise KeyError(name)
        return count + item

    def call(self, fn, args, depth):
        """The value of one of FUNCS."""
        if fn in ("FX32_CONST", "FX16_CONST"):
            x = self.evaluate(args[0], depth + 1, floats=True)
            return int(x * 4096 + (0.5 if x > 0 else -0.5))   # (fx32)(x * FX32_ONE +- 0.5f): truncated toward zero
        v = [self.evaluate(a, depth + 1) for a in args] if fn != "NELEMS" else []
        if fn == "GX_RGB":
            return v[0] | v[1] << 5 | v[2] << 10
        if fn == "GX_RGBA":
            return v[0] | v[1] << 5 | v[2] << 10 | v[3] << 15
        if fn == "GX_BG_SCRBASE":
            return v[0] // 0x800
        if fn == "GX_BG_CHARBASE":
            return v[0] // 0x4000
        return self.nelems(args[0].strip())

    def expand(self, expr, depth):
        """expr with each call of FUNCS replaced by its value."""
        while (m := FUNCS.search(expr)):
            level, i, args, arg = 1, m.end(), [], ""
            while level:
                if i >= len(expr):
                    raise ValueError(expr)
                c = expr[i]
                level += (c == "(") - (c == ")")
                if level == 1 and c == ",":
                    args, arg = args + [arg], ""
                elif level:
                    arg += c
                i += 1
            expr = f"{expr[:m.start()]}({self.call(m[1], args + [arg], depth)}){expr[i:]}"
        return expr

    def evaluate(self, expr, depth=0, undefined=None, floats=False):
        """The integer value of a constant expression, or ValueError/KeyError; floats: a float expression (the argument
        of FX32_CONST) and its float value."""
        if depth > 20:
            raise RecursionError(expr)
        out, pos, expr = [], 0, self.expand(expr.strip(), depth)
        while pos < len(expr):
            f = FLOAT.match(expr, pos)
            if f and floats:
                pos = f.end()
                out.append(repr(float(f[1] or f[2])))
                continue
            m = TOKEN.match(expr, pos)
            if not m or m.end() == pos or f and f.end() > m.end():
                raise ValueError(expr)
            pos = m.end()
            if m[1]:
                out.append(str(int(m[1], 0)))
            elif m[2]:
                out.append(str(ord(m[2][-1])))
            elif m[3]:
                if m[3] in TYPES and out[-1:] == ["("] and expr[pos:].lstrip().startswith(")"):
                    out.append(m[3])   # a cast: dropped below
                else:
                    try:
                        out.append(str(self.lookup(m[3], depth, floats)))
                    except KeyError:
                        if undefined is None:
                            raise
                        out.append(str(undefined))
            else:
                out.append({"&&": " and ", "||": " or ", "/": "/" if floats else "//", "!": " not "}.get(m[4], m[4]))
        s = " ".join(out)
        s = re.sub(r"\(\s*(?:%s)\s*\)" % "|".join(TYPES), "", s)   # casts
        if "?" in s or ":" in s or re.search(r"[A-Za-z_]", re.sub(r"\b(and|or|not)\b|\d[eE][-+]?\d", "", s)):
            raise ValueError(expr)
        try:
            v = eval(s, {"__builtins__": {}})
        except (SyntaxError, ZeroDivisionError, TypeError) as e:
            raise ValueError(f"{expr}: {e}")
        return float(v) if floats else int(v)

    def value(self, expr):
        """The value of a constant expression or None."""
        try:
            return self.evaluate(expr)
        except (ValueError, KeyError, RecursionError):
            return None


@lru_cache(maxsize=8)
def consts(c_file, version):
    return Consts(c_file, version)


if __name__ == "__main__":
    ver = sys.argv[2] if sys.argv[1] == "--version" else "b2_us"
    args = sys.argv[3:] if sys.argv[1] == "--version" else sys.argv[1:]
    c = Consts(args[0], ver)
    for n in args[1:]:
        print(n, "=", c.value(n))
