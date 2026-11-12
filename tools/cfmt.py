#!/usr/bin/env python3
"""Reformat a C function to the repo's style: tools/ref_port.py's way of writing natural C from a reference whose license
allows no copy (docs/plan.md, 2026-10-13: read it, write natural C).

    python3 tools/cfmt.py < function.c        # the function reformatted on stdout

What changes: the comments go; 4-space indentation, the opening brace of a function and of a statement on its line,
spaces around binary operators and after commas, `if (`, braces around every body (`if`, `else`, `for`, `while`,
`do`), `case` labels in line with their `switch`, labels at column 0, one statement a line, long lines wrapped at a
comma or a logical operator. What does not: the tokens. format() checks that the tokens it prints are the input's
(braces and empty statements aside) and raises ValueError if not, so the function that comes out compiles to what went
in. Reads the function's text only (no preprocessing): a `#if` inside it stays as a line of its own.
"""
import re
import sys

TOKEN = re.compile(r'''
   (?P<pp>^[ \t]*\#(?:[^\n\\]|\\\n|\\.)*)
 | (?P<lc>//[^\n]*)
 | (?P<bc>/\*.*?\*/)
 | (?P<ws>[ \t\r\f\v]+|\n)
 | (?P<str>L?"(?:\\.|[^"\\\n])*")
 | (?P<chr>L?'(?:\\.|[^'\\\n])+')
 | (?P<num>0[xX][0-9a-fA-F]+[uUlL]*|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?[fFuUlL]*)
 | (?P<id>[A-Za-z_]\w*)
 | (?P<op>>>=|<<=|\.\.\.|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=|&=|\|=|\^=|\#\#|[-+*/%&|^~!<>=?:;,.(){}\[\]\#])
''', re.X | re.S | re.M)

TYPE_WORDS = {"unsigned", "signed", "int", "char", "short", "long", "void", "float", "double", "const", "volatile",
              "struct", "union", "enum", "static", "register", "extern", "inline", "restrict"}
KEYWORDS = {"if", "else", "for", "while", "do", "switch", "case", "default", "return", "goto", "break", "continue",
            "sizeof", "typedef"}
OPEN_OPERAND = {"return", "case", "sizeof", "goto", "else", "do"}  # a word after which an operator is unary
BINARY = {"+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "&&", "||", "=", "+=",
          "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "?", ":"}
UNARY = {"-", "+", "*", "&", "!", "~"}
TYPENAME = re.compile(r"^(?:[us](?:8|16|32|64)|\w+_t|BOOL|f32|fx\d+|[A-Z][a-z]\w*)$")
WIDTH = 120


def tokens(text):
    """[(kind, text)] without whitespace and comments."""
    out, i = [], 0
    while i < len(text):
        m = TOKEN.match(text, i)
        if not m:
            raise ValueError(f"cannot tokenize at {text[i:i + 30]!r}")
        i = m.end()
        kind = m.lastgroup
        if kind in ("ws", "lc", "bc"):
            continue
        s = m.group()
        if kind == "pp":
            if re.match(r"#\s*pragma\s+unused", s):
                continue
            raise ValueError("preprocessor line " + s.split()[0])
            s = re.sub(r"[ \t]*\\\n[ \t]*", " \\\n    ", s.strip())
            s = re.sub(r"^#\s*", "#", s)
        out.append((kind, s))
    return out


def match_paren(toks, start):
    depth = 0
    for j in range(start, len(toks)):
        if toks[j][0] == "op":
            if toks[j][1] == "(":
                depth += 1
            elif toks[j][1] == ")":
                depth -= 1
                if depth == 0:
                    return j
    return None


def is_type(toks, typedefs):
    """toks (the inside of parentheses) read as a type name."""
    if not toks or any(k != "id" and s != "*" for k, s in toks):
        return False
    words = [s for k, s in toks if k == "id"]
    if not words or any(w in KEYWORDS for w in words):
        return False
    if any(w in TYPE_WORDS or w in typedefs for w in words):
        return True
    if toks[-1][1] == "*":
        return True  # (Foo *) is a cast; (a * b) does not end with a star
    return len(words) == 1 and TYPENAME.match(words[0]) is not None


def expr(toks, typedefs=(), decl=False, header=False):
    """toks as one line of text. decl: a declaration's `*` before the first `=` belongs to the name; header: a
    function's head, whose parameter list is declarations."""
    kinds = {}  # index of a "(" -> "call", "cast", "type" or "group"
    cast_close = set()
    for n, (k, s) in enumerate(toks):
        if k == "op" and s == "(":
            prev = toks[n - 1] if n else None
            j = match_paren(toks, n)
            if prev and prev[0] == "id" and prev[1] == "sizeof":
                kinds[n] = "type" if j is not None and is_type(toks[n + 1:j], typedefs) else "call"
            elif prev and (prev[0] == "id" and prev[1] not in KEYWORDS or prev[1] in (")", "]")):
                kinds[n] = "call"
            else:
                after = toks[j + 1] if j is not None and j + 1 < len(toks) else None
                if j is not None and after and is_type(toks[n + 1:j], typedefs) and (
                        after[0] in ("id", "num", "str", "chr") or after[1] in ("(", "~", "!", "&", "*", "-", "+",
                                                                                "++", "--", "{")):
                    kinds[n] = "cast"
                    cast_close.add(j)
                else:
                    kinds[n] = "group"
    out = []
    prev = None
    after_operand = False
    tight = True  # no space before the next token
    modes = [decl]  # for each open paren (and the top): does `*` after a name declare a pointer
    assigned = [False]  # per open paren: has an `=` come (after it a `*` multiplies)
    for n, (k, s) in enumerate(toks):
        space = not tight and prev is not None
        operand_end = False
        tight_next = False
        if k == "op":
            if s == "(":
                kind = kinds[n]
                if kind == "call":
                    space = False
                modes.append(True if kind in ("cast", "type") else header if kind == "call" else False)
                assigned.append(False)
                if kind == "type":
                    space = False
                tight_next = True
            elif s == ")":
                space = False
                if len(modes) > 1:
                    modes.pop()
                    assigned.pop()
                if n in cast_close:
                    tight_next = True
                else:
                    operand_end = True
            elif s == "[":
                space = False
                modes.append(False)
                assigned.append(False)
                tight_next = True
            elif s == "]":
                space = False
                if len(modes) > 1:
                    modes.pop()
                    assigned.pop()
                operand_end = True
            elif s == ",":
                space = False
                assigned[-1] = False
            elif s == ";":
                space = False
            elif s in (".", "->"):
                if after_operand:
                    space = False
                tight_next = True
            elif s in ("++", "--"):
                if after_operand:  # postfix
                    space = False
                    operand_end = True
                else:
                    tight_next = True
            elif s in ("!", "~"):
                tight_next = True
            elif s == "*" and modes[-1] and not assigned[-1] and prev is not None and (
                    prev[0] == "id" and prev[1] not in OPEN_OPERAND or prev[1] == "*"):
                space = prev[1] != "*"
                tight_next = True
            elif s in UNARY and not after_operand:
                tight_next = True
            elif s == "{":
                modes.append(False)
                assigned.append(False)
            elif s == "}":
                if len(modes) > 1:
                    modes.pop()
                    assigned.pop()
                operand_end = True
            elif s == "=":
                assigned[-1] = True
        else:
            operand_end = not (k == "id" and s in OPEN_OPERAND)
        out.append((" " if space else "") + s)
        prev = (k, s)
        after_operand = operand_end
        tight = tight_next
    return "".join(out)


class Fmt:
    def __init__(self, toks, typedefs=()):
        self.t = toks
        self.i = 0
        self.out = []
        self.typedefs = set(typedefs)

    def peek(self, k=0):
        j = self.i + k
        return self.t[j][1] if j < len(self.t) else ""

    def kind(self, k=0):
        j = self.i + k
        return self.t[j][0] if j < len(self.t) else ""

    def is_op(self, s):
        return self.kind() == "op" and self.peek() == s

    def is_word(self, s):
        return self.kind() == "id" and self.peek() == s

    def expr(self, toks, **kw):
        return expr(toks, self.typedefs, **kw)

    def line(self, indent, text):
        if text.startswith("#"):
            self.out.append(text)
            return
        pad = "    " * indent
        if len(pad) + len(text) <= WIDTH:
            self.out.append(pad + text)
            return
        self.out.extend(wrap(pad, text))

    def parens(self):
        """The tokens inside the parentheses at the cursor; the cursor moves past them."""
        j = match_paren(self.t, self.i)
        if j is None:
            raise ValueError("unbalanced parenthesis")
        inside = self.t[self.i + 1:j]
        self.i = j + 1
        return inside

    def body(self, indent, head):
        """head + ' {' and the statement at the cursor as the block's lines; the closing brace is the caller's."""
        self.line(indent, head + " {")
        self.contents(indent)

    def contents(self, indent):
        if self.is_op("{"):
            self.i += 1
            while not self.is_op("}"):
                self.stmt(indent + 1)
            self.i += 1
        else:
            self.stmt(indent + 1)

    def stmt(self, indent):
        k, s = self.t[self.i]
        if k == "pp":
            self.i += 1
            self.out.append(s)
            return
        if k == "op" and s == "{":
            self.line(indent, "{")
            self.i += 1
            while not self.is_op("}"):
                self.stmt(indent + 1)
            self.i += 1
            self.line(indent, "}")
            return
        if k == "op" and s == ";":
            self.i += 1
            return
        if k == "id" and s == "if":
            self.i += 1
            self.body(indent, f"if ({self.expr(self.parens())})")
            while self.is_word("else"):
                self.i += 1
                if self.is_word("if"):
                    self.i += 1
                    self.line(indent, f"}} else if ({self.expr(self.parens())}) {{")
                    self.contents(indent)
                    continue
                self.line(indent, "} else {")
                self.contents(indent)
                break
            self.line(indent, "}")
            return
        if k == "id" and s in ("for", "while"):
            self.i += 1
            inside = self.parens()
            if s == "for":
                parts, depth, cur = [], 0, []
                for tok in inside:
                    if tok[0] == "op" and tok[1] in ("(", "["):
                        depth += 1
                    elif tok[0] == "op" and tok[1] in (")", "]"):
                        depth -= 1
                    if tok[0] == "op" and tok[1] == ";" and depth == 0:
                        parts.append(cur)
                        cur = []
                    else:
                        cur.append(tok)
                parts.append(cur)
                if len(parts) != 3:
                    raise ValueError("for with " + str(len(parts)) + " parts")
                texts = [self.expr(p, decl=self.declaration(p)) for p in parts]
                head = "for (" + texts[0] + ";" + (" " + texts[1] if texts[1] else "") + ";" + \
                    (" " + texts[2] if texts[2] else "") + ")"
            else:
                head = f"while ({self.expr(inside)})"
            if self.is_op(";"):
                self.i += 1
                self.line(indent, head + ";")
                return
            self.body(indent, head)
            self.line(indent, "}")
            return
        if k == "id" and s == "do":
            self.i += 1
            self.body(indent, "do")
            if not self.is_word("while"):
                raise ValueError("do without while")
            self.i += 1
            cond = self.expr(self.parens())
            if self.is_op(";"):
                self.i += 1
            self.line(indent, f"}} while ({cond});")
            return
        if k == "id" and s == "switch":
            self.i += 1
            self.line(indent, f"switch ({self.expr(self.parens())}) {{")
            if not self.is_op("{"):
                raise ValueError("switch without a brace")
            self.i += 1
            while not self.is_op("}"):
                self.switch_item(indent)
            self.i += 1
            self.line(indent, "}")
            return
        if k == "id" and self.kind(1) == "op" and self.peek(1) == ":" and s not in ("default", "case"):
            self.i += 2
            self.out.append(f"{s}:")
            return
        self.simple(indent)

    def switch_item(self, indent):
        k, s = self.t[self.i]
        if k == "id" and s in ("case", "default"):
            j, depth = self.i + 1, 0
            while True:
                tk, ts = self.t[j]
                if tk == "op" and ts in ("(", "["):
                    depth += 1
                elif tk == "op" and ts in (")", "]"):
                    depth -= 1
                elif tk == "op" and ts == "?":
                    depth += 1  # a ternary's colon is not the label's
                elif tk == "op" and ts == ":":
                    if depth == 0:
                        break
                    depth -= 1
                j += 1
            label = self.expr(self.t[self.i:j])
            self.i = j + 1
            self.line(indent, label + ":")
            return
        self.stmt(indent + 1)

    def declaration(self, toks):
        """Does the statement read as a declaration: `Type *name`, `const Type name = ...`."""
        ids, j = [], 0
        while j < len(toks) and (toks[j][0] == "id" or toks[j][1] == "*" and toks[j][0] == "op"):
            if toks[j][0] == "id":
                if toks[j][1] in KEYWORDS - {"typedef"}:
                    return False
                ids.append(toks[j][1])
            j += 1
        if len(ids) < 2:
            return False
        return j == len(toks) or toks[j][1] in ("=", ";", ",", "[") and toks[j][0] == "op"

    def simple(self, indent):
        start, depth = self.i, 0
        while self.i < len(self.t):
            k, s = self.t[self.i]
            if k == "op" and s in ("(", "[", "{"):
                depth += 1
            elif k == "op" and s in (")", "]", "}"):
                depth -= 1
            elif k == "op" and s == ";" and depth == 0:
                break
            self.i += 1
        toks = self.t[start:self.i]
        self.i += 1  # the semicolon
        self.last_decl = indent == 1 and self.declaration(toks)
        self.line(indent, self.expr(toks, decl=self.declaration(toks)) + ";")

    def function(self):
        j = 0
        while not (self.t[j][0] == "op" and self.t[j][1] == "{"):
            j += 1
        text = self.expr(self.t[:j], decl=True, header=True)
        self.i = j + 1
        self.line(0, text + " {")
        self.last_decl = False
        while not self.is_op("}"):
            was = self.last_decl
            self.last_decl = False
            mark = len(self.out)
            self.stmt(1)
            if was and not self.last_decl and mark < len(self.out):
                self.out.insert(mark, "")
        self.line(0, "}")
        return self.out


def wrap(pad, text):
    """text (indented by pad) over WIDTH: split after a `, ` or before ` && ` / ` || ` outside literals, the rest 8
    columns further."""
    cont = pad + "        "
    cuts, quote, i = [], None, 0
    while i < len(text):
        c = text[i]
        if quote:
            if c == chr(92):
                i += 1
            elif c == quote:
                quote = None
        elif c in "\"'":
            quote = c
        elif text.startswith(", ", i):
            cuts.append(i + 2)
        elif text.startswith(" && ", i) or text.startswith(" || ", i):
            cuts.append(i + 1)
        i += 1
    out, line, last = [], pad, 0
    for cut in cuts + [len(text)]:
        piece = text[last:cut]
        if len(line) + len(piece) > WIDTH and line.strip():
            out.append(line.rstrip())
            line = cont
        line += piece.lstrip() if line == cont else piece
        last = cut
    out.append(line.rstrip())
    return out


def significant(toks):
    return [s for k, s in toks if s not in ("{", "}", ";") and k != "pp"]


def format(text, typedefs=()):
    toks = tokens(text)
    lines = Fmt(toks, typedefs).function()
    out = "\n".join(lines) + "\n"
    a, b = significant(toks), significant(tokens(out))
    if a != b:
        at = next((n for n in range(min(len(a), len(b))) if a[n] != b[n]), min(len(a), len(b)))
        raise ValueError(f"tokens differ at {a[max(0, at - 3):at + 3]} vs {b[max(0, at - 3):at + 3]}")
    return out


if __name__ == "__main__":
    sys.stdout.write(format(sys.stdin.read()))
