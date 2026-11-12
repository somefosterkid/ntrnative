// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/dendou_demo.h: the lines in use so far.
#ifndef BWPORT_DEMO_DENDOU_DEMO_H
#define BWPORT_DEMO_DENDOU_DEMO_H

#include "types.h"
#include "struct_decls.h"

// The demo's heap, a child of HEAPID_USER
#define HEAPID_DENDOU_DEMO 0x8d

// Black's, three more than Black 2's 213 (ddemo_main.c's code)
// The demo's graphics and particles
#define ARCID_DENDOU_DEMO 216

struct DendouDemoParam {
    PokeParty *party;
    PlayerInfo *playerInfo;
    PlayTime *playTime;
};

#endif // BWPORT_DEMO_DENDOU_DEMO_H
