#!/usr/bin/env python3
"""The order MWCC puts a file's data objects in (the .rodata/.data sections of its object), as a model.

    python3 tools/rodata_order.py ITEMS.json     # [["name", size, "kind"], ...] in creation order; prints the order

Found on ircbattlemenu.c (tdo, 10-20), where tools/local_init.py had turned data blobs into locals with initializers
and fit_data.py then saw relocations that disagreed on the start of .rodata/.data:

  * The compiler keeps one list of objects, newest first: each local initializer ( becomes a
     object in .rodata, one section each) when its function is compiled, each file-scope static or global when it is
    defined, all in source order (a static defined between two functions comes between their locals).
  * It sorts that list by size with MSL's qsort (lib/msl/src/qsort_msl.c, a heap sort, so not stable: equal sizes come
    out in an order the list decides) and emits the objects in that order.
  * Statics and globals under 64 bytes are merged: all the const ones into one .rodata section, all the others into one
    .data section, each placed where its first member (the smallest) falls in the sorted order, its members in sorted
    order. A 64 byte or bigger one is a section of its own. Locals are never merged.

So a local's place in .rodata depends on where the file defines its statics: to make the data complete after blobs became
locals, reorder the statics (and a proc table or other global const, defined last) until simulate() gives Black's
order; a search over the creation order (simulated annealing on the number of locals out of place) finds the
arrangements in a second. Globals are not modelled exactly (a global const in the middle of the list is not placed
like a static): define them first or last. Check with a compile: python3 -c "..." on the object's sections.
Also needed there: a struct copy  that the original wrote as a block-scope initializer
() must be spelled so, or the copy moves; and the blob bytes must be the ROM's (a table of
rects whose fields were written in the wrong order only shows in fit_data's "bytes differ" line).
"""
import json
import sys


def qsort(a, cmp):
    """MSL's qsort (lib/msl/src/qsort_msl.c) on a list."""
    a = list(a)
    count = len(a)
    if count < 2:
        return a
    root = count // 2 + 1
    first, last = root - 1, count - 1
    while True:
        if root > 1:
            root -= 1
            first -= 1
        else:
            a[first], a[last] = a[last], a[first]
            count -= 1
            if count == 1:
                return a
            last -= 1
        index, child = root, root - 1
        while count >= index * 2:
            parent = child
            index *= 2
            child = index - 1
            if count > index and cmp(a[child], a[child + 1]) < 0:
                index += 1
                child += 1
            if cmp(a[parent], a[child]) >= 0:
                break
            a[parent], a[child] = a[child], a[parent]


def simulate(items, merge_below=64):
    """items: [(name, size, kind)] in creation order, kind "local" (a @N object), "const" or "data" (a static or global
    in .rodata / .data). Returns the sections in the object's order, each a list of names."""
    srt = qsort(items[::-1], lambda x, y: x[1] - y[1])
    out, groups = [], {}
    for name, size, kind in srt:
        if kind in ("const", "data") and size < merge_below:
            if kind not in groups:
                groups[kind] = []
                out.append(groups[kind])
            groups[kind].append(name)
        else:
            out.append([name])
    return out


if __name__ == "__main__":
    for section in simulate([tuple(i) for i in json.load(open(sys.argv[1]))]):
        print(" ".join(section))
