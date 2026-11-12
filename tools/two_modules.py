#!/usr/bin/env python3
"""One source file whose sections go to two modules: TwlSDK's card_rom.c and card_utility.c have their code in main
and a twl/ltdmain_begin.h block in the LTD autoload's .ltdmain (and .ltdmain.bss), one translation unit, one object.
dsd gives a file name one module ("Delink file name 'card_utility.c' in LTD autoload 0 already used in ARM9 main"),
so the second module's delinks.txt names the file's part there by an alias beside it, <stem>@<tag>.<ext>
(lib/nitro/src/card/card_rom@ltd.c for card_rom.c), which no source is:

- while the alias isn't complete, the link takes dsd's delinked object of it, Black's bytes, as any file's;
- once it is (with the file it stands for, whose C then has the block), this rewrites what `dsd lcf` wrote: the
  linker script's <stem>@<tag>.o(<section>) lines name <stem>.o, the real file's object, and in an LTD autoload the
  alias's .bss is that object's .ltdmain.bss, the block's uninitialised data; the alias's object (which nothing
  builds) leaves the object list. The link then puts the object's .text in one module and its .ltdmain in the other.

    python3 tools/two_modules.py build/b_us/arm9.lcf build/b_us/objects.txt     # rewrites both in place

configure.py's lcf rule runs it after dsd, so tools reading either file see the real object; a delinks.txt range
of the alias is the file's range in that module (its .bss range holds the .ltdmain.bss).
"""
import os
import re
import sys

ALIAS = re.compile(r"^(?P<dir>.*/)?(?P<stem>(?!_dsd_gap@)[^/@]+)@[^/]*(?P<ext>\.\w+)$")


def real_of(name):
    """The file (or object) an alias names, lib/x/a.c for lib/x/a@ltd.c (build/b_us/lib/x/a.o for
    build/b_us/lib/x/a@ltd.o), else None: a plain name or dsd's gap objects (_dsd_gap@main_0.o)."""
    m = ALIAS.match(name)
    return f"{m['dir'] or ''}{m['stem']}{m['ext']}" if m else None


def compiled_alias(path):
    """Whether an object list's path is a complete alias's object (build/b_us/<dir>/<stem>@<tag>.o, which nothing
    builds), not dsd's delinked object of an incomplete one (under build/<version>/delinks/)."""
    return real_of(path) is not None and "/delinks/" not in path


def rewrite(lcf, objects):
    """(the linker script, the object paths) with the complete aliases' sections their real objects': see above.
    Stops on an alias complete before the file it stands for (its object is the delinked one, without the block)."""
    aliases = [p for p in objects if compiled_alias(p)]
    for p in aliases:
        if real_of(p) not in objects:
            sys.exit(f"two_modules.py: {p} is complete, but {real_of(p)} is not linked (is its file complete?)")
    names = {os.path.basename(p): os.path.basename(real_of(p)) for p in aliases}
    if not names:
        return lcf, objects

    def lines(block, ltd):
        def one(m):
            sec = ".ltdmain.bss" if ltd and m["sec"] == ".bss" else m["sec"]
            return f"{m['pad']}{names[m['obj']]}({sec})"
        return re.sub(r"^(?P<pad>\s*)(?P<obj>\S+\.o)\((?P<sec>\.[\w.]+)\)$",
                      lambda m: one(m) if m["obj"] in names else m[0], block, flags=re.M)

    # the LTD autoloads' blocks (as tools/dead_strip.py finds them), then the rest
    ltd = re.compile(r"^\s*\.ltd_\w+ : \{\n.*?^\s*\} > LTD_\w+$", re.M | re.S)
    out, at = [], 0
    for m in ltd.finditer(lcf):
        out += [lines(lcf[at:m.start()], False), lines(m[0], True)]
        at = m.end()
    out.append(lines(lcf[at:], False))
    return "".join(out), [p for p in objects if p not in aliases]


def dotted(lcf, objects):
    """The linker script with the object of a source of two dots named as dsd's object list names it: dsd lcf writes
    crt0.FLX.o(.text) for lib/nitro/src/init/crt0.FLX.c, while its object list and the delinked object are crt0.o
    (the name up to its first dot), so the link placed none of it (t36c, t37a). configure.py copies the compiled
    object to dsd's name."""
    have = {os.path.basename(p) for p in objects}

    def one(m):
        name = m["obj"]
        short = name.split(".")[0] + ".o"
        return m[0] if name in have or short not in have else f"{m['pad']}{short}({m['sec']})"
    return re.sub(r"^(?P<pad>\s*)(?P<obj>[^\s/]+\.[^\s/]+\.o)\((?P<sec>\.[\w.]+)\)$", one, lcf, flags=re.M)


def main():
    lcf_path, objects_path = sys.argv[1:3]
    objects = [line.strip().strip('"') for line in open(objects_path) if line.strip()]
    text = open(lcf_path).read()
    lcf, kept = rewrite(text, objects)
    lcf = dotted(lcf, kept)
    if kept != objects or lcf != text:
        open(lcf_path, "w").write(lcf)
        open(objects_path, "w").write("".join(f'"{p}"\n' for p in kept))


if __name__ == "__main__":
    main()
