// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/draw_system.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_DRAW_SYSTEM_H
#define BWPORT_APP_COMM_TVT_DRAW_SYSTEM_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// The pens, by their shapes
enum {
    DRAW_PEN_DOT,
    DRAW_PEN_SMALL,
    DRAW_PEN_ROUND,
    DRAW_PEN_HEART,
    DRAW_PEN_POKE_BALL,
    DRAW_PEN_FACE,
    DRAW_PEN_STAR,
    DRAW_PEN_DROP,
    DRAW_PEN_MAX,
};

// One stroke, from (x0, y0) to (x1, y1)
struct DrawCommand {
    u8 x0;
    u8 y0;
    u8 x1;
    u8 y1;
    u16 color;
    u8 pen;
};

struct DrawSystemParam {
    HeapID heapId;
    u8 unk2;
    u16 numCommands;
    u16 clipLeft;
    u16 clipRight;
    u16 clipTop;
    u16 clipBottom;
};

// pokebw2's DrawSystem_Create
DrawSystem *func_ov172_021b6be0(const DrawSystemParam *param);
// pokebw2's DrawSystem_Draw
// Draws the strokes added since the last call
void func_ov172_021b6c4c(DrawSystem *sys);
// pokebw2's DrawSystem_AddCommand
void func_ov172_021b6e88(DrawSystem *sys, const DrawCommand *cmd);
// pokebw2's DrawSystem_SetWriteIndex
void func_ov172_021b6ea8(DrawSystem *sys, u16 index);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DrawSystem_Delete
void func_ov172_021b6c34(DrawSystem *sys);
// pokebw2's DrawSystem_Update
void func_ov172_021b6c48(DrawSystem *sys);

#endif // BWPORT_APP_COMM_TVT_DRAW_SYSTEM_H
