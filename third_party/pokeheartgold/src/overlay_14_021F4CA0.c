#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02013534.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F4CA0(struct UnkStruct_ov14_boxapp *arg0);

struct UnkStruct_ov14_boxsys;

void ov14_021F4CA0(UnkStruct_ov14_boxapp *arg0) {
    u32 v0;

    ov14_021F2A18(*((void * *)&arg0->sys), 4, 0);
    ov14_021F2A18(*((void * *)&arg0->sys), 5, 0);
    ov14_021F2A18(*((void * *)&arg0->sys), 6, 0);
    ov14_021F2A18(*((void * *)&arg0->sys), 7, 0);
    ov14_021F2A18(*((void * *)&arg0->sys), 8, 0);
    v0 = 0;
    do {
        ov14_021F2A18(*((void * *)&arg0->sys), v0 + 0xF, 0);
        v0 += 1;
    } while (v0 < 6U);
    TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)(*((void * *)&arg0->sys)) + 0x414), 0);
    TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)(*((void * *)&arg0->sys)) + 0x424), 0);
}
