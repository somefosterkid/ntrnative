"""tools/hg_abi_scan.py [--gate] [ROOT [DIR ...]] (default build/pokeheartgold): win64 ABI scan of the HG/SS tree (Platinum:
build/pokeplatinum; each extra DIR, e.g. port/game, is scanned too: the port's own declarations of the game's functions). A
function declared in one file with a narrow (8/16-bit) parameter or return where its definition has a 32-bit one (or the
reverse for returns): SysV clang extends narrow arguments at the caller and narrow returns in the callee, win64 does neither
(and clang 21 no longer extends a narrow return), so the 32-bit side reads garbage upper bits.
Prints: NAME  what  decl-file:line  def-file:line
The port's view of the tree is scanned: the DS-only branch of a `#if defined(SDK_PORT) || defined(NONMATCHING)` (or
`#ifdef SDK_PORT`) is skipped, so a declaration pokeheartgold-zzabi.patch (tools/pokehg_abi.py) has fixed is not a row.
--gate: exit 1 if any row is left that is not in tools/hg_abi_exceptions.txt (a re-vendor's new rows: remake the patch
with `python3 tools/pokehg_abi.py`, or list the function there with why).
A static function is matched only with the declarations of its own file (two files' static functions of one name are two
functions: pokeplatinum's UpdateSpritePositions)."""
import os, re, sys
NARROW = {'u8', 's8', 'u16', 's16', 'char', 'unsigned char', 'signed char', 'short', 'unsigned short', 'signed short',
          'bool', 'BOOL8', 'vu8', 'vu16', 'unsigned short int', 'short int', 'uint8_t', 'int8_t', 'uint16_t', 'int16_t',
          'vs8', 'vs16', 'fx16', 'GXRgb'}
# a parameter's last word that is its type, not its name ("const u8", "unsigned int", "volatile vu16"). rev2: every last
# word starting with u was taken for a type, so `u8 unused0` was the type "u8 unused0" ('other', never a row)
TYPEWORD = re.compile(r'^(int|char|short|long|unsigned|signed|float|double|v?[us](8|16|32|64)|fx(16|32|64)|u?int(8|16|32|64|ptr)_t)$')
QUAL = ('const', 'volatile', 'register')
WIDE = re.compile(r'^(const\s+)?(u32|s32|int|unsigned|unsigned int|signed int|BOOL|fx32|enum \w+|[A-Z]\w*ID|HeapID|long|unsigned long|uintptr_t|intptr_t)$')
FN = re.compile(r'(?m)^([A-Za-z_][\w \*]*?[\w\*])\s*\b(\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*([;{])')
KW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else'}
EXCEPTIONS = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hg_abi_exceptions.txt')
PC_IF = re.compile(r'#\s*(if\s+defined\s*\(?\s*SDK_PORT\s*\)?(\s*\|\|\s*defined\s*\(?\s*NONMATCHING\s*\)?)?|ifdef\s+SDK_PORT)\s*(//.*|/\*.*)?$')
DS_IF = re.compile(r'#\s*(ifndef\s+SDK_PORT|if\s+!\s*defined\s*\(?\s*SDK_PORT\s*\)?)\s*(//.*|/\*.*)?$')


def port_view(txt):
    """txt with the lines the port does not compile (the DS branch of an SDK_PORT guard) blanked; line numbers kept."""
    out, stack = [], []   # stack: 'pc' (keep), 'ds' (blank), None (another #if: as its parent)
    for line in txt.split('\n'):
        s = line.strip()
        if s.startswith('#'):
            d = re.sub(r'^#\s*', '#', s)
            if d.startswith('#if'):
                stack.append('pc' if PC_IF.match(d) else 'ds' if DS_IF.match(d) else None)
            elif d.startswith(('#else', '#elif')) and stack and stack[-1]:
                stack[-1] = 'ds' if stack[-1] == 'pc' else 'pc'
            elif d.startswith('#endif') and stack:
                stack.pop()
            out.append(line)
            continue
        out.append('' if 'ds' in stack else line)
    return '\n'.join(out)


def split(params):
    out, d, cur = [], 0, ''
    for ch in params:
        if ch == '(':
            d += 1
        elif ch == ')':
            d -= 1
        if ch == ',' and d == 0:
            out.append(cur); cur = ''
        else:
            cur += ch
    out.append(cur)
    return [p.strip() for p in out]


def ptype(p):
    p = re.sub(r'\s+', ' ', p.replace('\n', ' ')).strip()
    if '*' in p or '(' in p or '[' in p:
        return 'ptr'
    w = p.split(' ')
    # the last word is a name unless it is a type word or nothing but qualifiers / struct, enum, union come before it
    if len(w) > 1 and not TYPEWORD.match(w[-1]) and [x for x in w[:-1] if x not in QUAL + ('struct', 'enum', 'union')]:
        w = w[:-1]   # drop the parameter name
    t = ' '.join(x for x in w if x not in QUAL)
    return t


def kind(t):
    if t in NARROW:
        return 'narrow'
    if WIDE.match(t):
        return 'wide'
    return 'other'


def scan(root, extra=()):
    """[(name, [(slot, decl type, def type)], decl path, line, def path, line)]; slot: a parameter index or 'return'."""
    decls, defs = {}, {}
    dirs = [os.path.join(root, d) for d in ('src', 'port', 'include', 'lib/include', 'lib/src')] + list(extra)
    for top in dirs:
        for dp, _, fs in sorted(os.walk(top)):
            for f in sorted(fs):
                if not f.endswith(('.c', '.h')):
                    continue
                path = os.path.join(dp, f)
                txt = port_view(open(path, errors='replace').read())
                for m in FN.finditer(txt):
                    ret, name, params, end = m.group(1).strip(), m.group(2), m.group(3), m.group(4)
                    if name in KW or ret.split()[-1] in KW or ret.startswith(('return', 'else', '#', 'typedef')):
                        continue
                    line = txt.count('\n', 0, m.start()) + 1
                    static = re.match(r'^(static|inline|static inline)\s', ret) is not None
                    ret = re.sub(r'^(static|extern|inline|static inline)\s+', '', ret)
                    ps = [] if params.strip() in ('', 'void') else [ptype(p) for p in split(params)]
                    rel = path[len(root) + 1:] if path.startswith(root + '/') else path
                    rec = (rel, line, ret if '*' not in ret else 'ptr', ps, static)
                    (defs if end == '{' else decls).setdefault(name, []).append(rec)
    rows = []
    for name, dl in sorted(defs.items()):
        if name not in decls:
            continue
        for path, line, ret, ps, static in decls[name]:
            same = [d for d in dl if d[0] == path]
            other = [d for d in dl if not d[4]]
            if static or (same and same[0][4]):   # a file-local function: its own file's definition only
                if not same:
                    continue
                dpath, dline, dret, dps, _ = same[0]
            elif other:
                dpath, dline, dret, dps, _ = other[0]
            else:
                continue
            slots = [(i, a, b) for i, (a, b) in enumerate(zip(ps, dps)) if kind(a) == 'narrow' and kind(b) == 'wide']
            if kind(dret) == 'narrow' and kind(ret) == 'wide':
                slots.append(('return', ret, dret))
            if slots:
                rows.append((name, slots, path, line, dpath, dline))
    return rows


def what(slots):
    return ','.join('param%d %s->%s' % (i, a, b) if i != 'return' else 'return %s->%s' % (b, a) for i, a, b in slots)


def exceptions(path=None):
    """{function name: why} of the rows the generator leaves as they are (NAME<TAB>why; # comments)."""
    out, path = {}, path or EXCEPTIONS
    if os.path.exists(path):
        for line in open(path):
            if line.strip() and not line.startswith('#'):
                name, _, why = line.rstrip('\n').partition('\t')
                out[name.strip()] = why.strip()
    return out


def main(argv):
    gate = '--gate' in argv
    argv = [a for a in argv if a != '--gate']
    root = argv[0] if argv else 'build/pokeheartgold'
    rows = scan(root, argv[1:])
    exc = exceptions()
    left = 0
    for name, slots, path, line, dpath, dline in rows:
        tag = '' if name not in exc else '\t(exception: %s)' % exc[name]
        left += not tag
        print('%s\t%s\t%s:%d\t%s:%d%s' % (name, what(slots), path, line, dpath, dline, tag))
    print('# %d declarations (%d not in hg_abi_exceptions.txt)' % (len(rows), left), file=sys.stderr)
    if gate and left:
        print('hg_abi_scan --gate: %d declarations narrower than their definitions: remake '
              'port/subprojects/pokeheartgold-zzabi.patch (python3 tools/pokehg_abi.py) or list them in '
              'tools/hg_abi_exceptions.txt' % left, file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
