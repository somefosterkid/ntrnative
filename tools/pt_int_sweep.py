#!/usr/bin/env python3
"""pt_int_sweep.py: the Platinum interaction sweep: every person, sign, hidden item and trigger tile of every map, once each, from
a library save (default pt-ver69). It is tools/int_sweep.py with game=platinum (the code is shared with HG/SS's twin); the hook
is port/game/route_gen4.c Pr4_Int* (BWPORT_PT_INT, BWPORT_PT_INT_LOG, off unless set). docs/platinum.md "Interaction sweep".

    python3 tools/pt_int_sweep.py --maps all -j 6 --out /tmp/ptint --heapcheck
    python3 tools/pt_int_sweep.py --maps 60-70,411 -j 3 --out DIR
    python3 tools/pt_int_sweep.py --summary --out DIR
"""
import int_sweep

if __name__ == '__main__':
    int_sweep.main('platinum')
