#!/usr/bin/env python3
"""The values of C constant expressions in the references' headers: what tools/ref_decls.py writes for an enumerator.

    python3 tools/ref_values.py 'NNS_G3D_TEXSRTANM_ELEM_LAST_INTERP_MASK' '(1 << 4) | 3'

An expression is read by a small recursive-descent parser (Evaluator) with C's operators, casts to the integer types
and the names resolved by a callback. Values (names) resolves them from an enumerator before the one being worked out,
then Black's headers (an object-like #define or an enumerator with a constant value), then the references' headers
(#defines, other enums). A name or form it cannot work out raises Unresolved: the caller leaves the constant out and
says so, it never writes a guessed value.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from file_ranges import ROOT  # noqa: E402

BS = chr(92)
CTYPES = {"u8": (8, False), "s8": (8, True), "u16": (16, False), "s16": (16, True), "u32": (32, False),
          "s32": (32, True), "u64": (64, False), "s64": (64, True), "int": (32, True), "unsigned": (32, False),
          "short": (16, True), "char": (8, True), "long": (32, True), "BOOL": (32, True), "fx16": (16, True),
          "fx32": (32, True), "vu8": (8, False), "vu16": (16, False), "vu32": (32, False), "vs8": (8, True),
          "vs16": (16, True), "vs32": (32, True)}
_TOK = re.compile(r"\s*(?:(?P<num>0[xX][0-9a-fA-F]+[uUlL]*|\d+[uUlL]*)|(?P<id>[A-Za-z_]\w*)|"
                  r"(?P<chr>'(?:" + BS * 2 + r"?.)')|(?P<op><<|>>|<=|>=|==|!=|&&|" + BS + "|" + BS + r"||[-+*/%&|^~!<>?:()]))")
_ESC = {"n": 10, "t": 9, "r": 13, "0": 0, BS: 92, "'": 39}


class Unresolved(Exception):
    """A constant expression with a name or a form that cannot be given a value."""


def _wrap(v, bits, signed):
    v &= (1 << bits) - 1
    return v - (1 << bits) if signed and v >> (bits - 1) else v


class Evaluator:
    """A C integer constant expression, names resolved by resolve(name) (an int, or raises Unresolved).

    Values are Python ints: a cast wraps to its type, a literal is as written (0x80000000 stays 2147483648) and no
    other integer promotion is modelled; enumerator values and masks do not need it."""

    LEVELS = [("||",), ("&&",), ("|",), ("^",), ("&",), ("==", "!="), ("<", ">", "<=", ">="), ("<<", ">>"),
              ("+", "-"), ("*", "/", "%")]

    def __init__(self, text, resolve):
        self.toks, self.i, self.resolve = [], 0, resolve
        pos, text = 0, text.strip()
        while pos < len(text):
            m = _TOK.match(text, pos)
            if not m:
                raise Unresolved(f"unreadable: {text[pos:pos + 12]!r}")
            pos = m.end()
            self.toks.append((m.lastgroup, m.group(m.lastgroup)))
        if not self.toks:
            raise Unresolved("empty")

    def peek(self):
        return self.toks[self.i] if self.i < len(self.toks) else (None, None)

    def take(self, op=None):
        t = self.peek()
        if t[0] is None or (op is not None and t[1] != op):
            raise Unresolved(f"expected {op or 'more'}")
        self.i += 1
        return t

    def value(self):
        v = self.ternary()
        if self.i != len(self.toks):
            raise Unresolved("trailing " + self.peek()[1])
        return v

    def ternary(self):
        c = self.binary(0)
        if self.peek() == ("op", "?"):
            self.take()
            a = self.ternary()
            self.take(":")
            b = self.ternary()
            return a if c else b
        return c

    def binary(self, level):
        if level == len(self.LEVELS):
            return self.unary()
        v = self.binary(level + 1)
        while self.peek()[0] == "op" and self.peek()[1] in self.LEVELS[level]:
            op = self.take()[1]
            w = self.binary(level + 1)
            if op in ("/", "%") and w == 0:
                raise Unresolved("division by zero")
            q = abs(v) // abs(w) * (1 if (v < 0) == (w < 0) else -1) if op in ("/", "%") else 0
            v = {"||": int(bool(v or w)), "&&": int(bool(v and w)), "|": v | w, "^": v ^ w, "&": v & w,
                 "==": int(v == w), "!=": int(v != w), "<": int(v < w), ">": int(v > w), "<=": int(v <= w),
                 ">=": int(v >= w), "<<": v << w if 0 <= w < 128 else 0, ">>": v >> w if 0 <= w < 128 else 0,
                 "+": v + w, "-": v - w, "*": v * w, "/": q, "%": v - w * q}[op]
        return v

    def cast_type(self):
        """After an open paren: a type name and the close paren as (bits, signed), consumed; None, nothing consumed,
        when it is not one."""
        j, words = self.i, []
        while j < len(self.toks) and self.toks[j][0] == "id":
            words.append(self.toks[j][1])
            j += 1
        if not words or j >= len(self.toks) or self.toks[j] != ("op", ")"):
            return None
        words = [w for w in words if w not in ("const", "volatile")]
        unsigned, signed = "unsigned" in words, "signed" in words
        words = [w for w in words if w not in ("unsigned", "signed")]
        base = words[-1] if words else "int"
        if base not in CTYPES:
            return None
        bits, sgn = CTYPES[base]
        if base in ("char", "short", "int", "long") and (unsigned or signed):
            sgn = not unsigned
        self.i = j + 1
        return bits, sgn

    def unary(self):
        k, t = self.peek()
        if k == "op" and t in ("-", "+", "~", "!"):
            self.take()
            v = self.unary()
            return {"-": -v, "+": v, "~": ~v, "!": int(not v)}[t]
        if k == "op" and t == "(":
            self.take()
            ty = self.cast_type()
            if ty is not None:
                return _wrap(self.unary(), *ty)
            v = self.ternary()
            self.take(")")
            return v
        if k == "num":
            self.take()
            n = t.rstrip("uUlL")
            return int(n, 8) if len(n) > 1 and n[0] == "0" and n[1] not in "xX" else int(n, 0)
        if k == "chr":
            self.take()
            body = t[1:-1]
            return ord(body) if len(body) == 1 else _ESC[body[1]]
        if k == "id":
            self.take()
            if t == "sizeof":
                raise Unresolved("sizeof")
            if self.peek() == ("op", "("):
                raise Unresolved(f"{t}(...): a function-like macro or a call")
            return self.resolve(t)
        raise Unresolved(f"unexpected {t}")


def enum_items(text):
    """[(name, expression text or None)] of the enumerators of an enum's declaration text, in order."""
    flat = " ".join(text.split())
    inner = re.search(r"\{(.*)\}", flat, re.S)[1]
    depth, cur, items = 0, "", []
    for ch in inner:
        depth += (ch == "(") - (ch == ")")
        if ch == "," and depth == 0:
            items.append(cur)
            cur = ""
        else:
            cur += ch
    items.append(cur)
    out = []
    for item in items:
        m = re.match(r"\s*([A-Za-z_]\w*)\s*(?:=\s*(.*?))?\s*$", item)
        if m:
            out.append((m[1], m[2]))
    return out


def enum_values(text, resolve, why=None):
    """{enumerator: value} of an enum's declaration text, those that cannot be worked out absent (why(name, reason)
    is told); a name in an expression is an earlier enumerator or resolve(name)."""
    out, nxt, local = {}, 0, {}

    def name_of(w):
        if w in local:
            if local[w] is None:
                raise Unresolved(f"{w}, an earlier enumerator that is itself unresolved")
            return local[w]
        return resolve(w)
    for name, expr in enum_items(text):
        if expr is not None:
            try:
                nxt = Evaluator(expr, name_of).value()
            except Unresolved as e:
                nxt = None
                if why:
                    why(name, str(e))
        elif nxt is None and why:
            why(name, "follows an enumerator with no value")
        local[name] = nxt
        if nxt is not None:
            out[name] = nxt
            nxt += 1
    return out


def macro_body(text):
    """The value text of an object-like define (backslash-newlines joined), else None."""
    m = re.match(r"#\s*define\s+\w+(\(.*?\))?\s*(.*)$", " ".join(text.replace(BS + chr(10), " ").split()))
    return None if not m or m[1] else m[2]


class Black:
    """Resolves a name from Black's own headers: an object-like #define or an enumerator with a constant value."""

    def __init__(self, files=None):
        import ref_decls as rd
        self.macros, self.enums, self.state = {}, [], {}
        for f in files if files is not None else self.headers():
            try:
                text = rd.strip_comments(open(f, errors="replace").read())
            except OSError:
                continue
            for c in rd.chunks(text):
                one = " ".join(c.replace(BS + chr(10), " ").split())
                m = re.match(r"#\s*define\s+(\w+)(\(?)", one)
                if m:
                    body = macro_body(one) if not m[2] else None
                    if body is not None:
                        self.macros.setdefault(m[1], body)
                elif re.search(r"\benum\b[^;{}]*\{", c):
                    d = rd.classify(c, "black", f)
                    if d and d.members:
                        self.enums.append(d)

    @staticmethod
    def headers():
        out = []
        for g in ("lib/*/include/**/*.h", "include/**/*.h", "lib/*/sdk/**/*.h", "src/**/*.h"):
            out += glob.glob(f"{ROOT}/{g}", recursive=True)
        return out

    def __call__(self, name, busy=()):
        if name in self.state:
            if self.state[name] is None:
                raise Unresolved(name)
            return self.state[name]
        if name in busy:
            raise Unresolved(f"{name}: defined by itself")
        inner = busy + (name,)
        try:
            if name in self.macros:
                v = Evaluator(self.macros[name], lambda w: self(w, inner)).value()
            else:
                d = next((d for d in self.enums if name in d.members), None)
                if d is None:
                    raise Unresolved(f"{name}: not in Black headers")
                v = enum_values(d.text, lambda w: self(w, inner))
                if name not in v:
                    raise Unresolved(name)
                v = v[name]
        except Unresolved:
            self.state[name] = None
            raise
        self.state[name] = v
        return v


class Values:
    """What a name means in an enumerator's expression: Black's constant of that name, else a #define or an
    enumerator of the references' headers (sources: tools/ref_decls.py's checkouts)."""

    def __init__(self, sources, black=None):
        self.sources, self.black, self.cache, self.busy = sources, black, {}, set()

    def __call__(self, name):
        import ref_decls as rd
        if name in self.cache:
            if self.cache[name] is None:
                raise Unresolved(name)
            return self.cache[name]
        if name in self.busy:
            raise Unresolved(f"{name}: defined by itself")
        self.busy.add(name)
        try:
            v = None
            if self.black is not None:
                try:
                    v = self.black(name)
                except Unresolved:
                    pass
            if v is None:
                d = rd.lookup(name, self.sources)
                if d is None:
                    raise Unresolved(f"{name}: in no header")
                if d.kind == "macro":
                    body = macro_body(d.text)
                    if body is None:
                        raise Unresolved(f"{name}: a function-like macro")
                    v = Evaluator(body, self).value()
                elif name in d.members:
                    got = enum_values(d.text, self)
                    if name not in got:
                        raise Unresolved(f"{name}: no value")
                    v = got[name]
                else:
                    raise Unresolved(f"{name}: not a constant")
        except Unresolved:
            self.cache[name] = None
            raise
        finally:
            self.busy.discard(name)
        self.cache[name] = v
        return v


def main():
    srcs = ["ntrtwl-NitroSDK", "ntrtwl-NitroSystem", "ntrtwl-NitroWiFi", "ntrtwl-NitroDWC", "libntr"]
    values = Values(srcs, Black())
    for expr in sys.argv[1:]:
        try:
            print(f"{expr} = {Evaluator(expr, values).value():#x}")
        except Unresolved as e:
            print(f"{expr}: unresolved: {e}")


if __name__ == "__main__":
    main()
