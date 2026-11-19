#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "sys_task.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "map_object.h"

s32 ov14_021F3488(void *, s32, s32);

s32 ov14_021F43F4(s32, s32);

static void ov14_021F2534(void *arg0);

static void ov14_021F2534(void *arg0) {
    u32 v0;

    v0 = *(u32 *)((u8 *)(*(void **)arg0) + 8);
    switch (v0) {
    case 0:
        ov14_021F3488(arg0, 2, 1);
        return;
    case 1:
    case 2:
    case 3:
        ov14_021F43F4(*((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->sys), 0);
        ov14_021F3488(arg0, 1, 1);
    default:
        return;
    }
}

s32 ov14_021F0254(void *, s16);

s32 ov14_021F2A18(u8 *, s32, s32);

s32 ov14_021F66E8(void *, s32, s32);

void ov14_021F2574(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F2574(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(*((void **)&arg0->sys), 9, 0);
    ov14_021F66E8(arg0, 0x25, 1);
    ov14_021F2534(arg0);
    ov14_021F0254(arg0, 3);
}

void ov14_021F259C(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F259C(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(*((void **)&arg0->sys), 9, 0);
    ov14_021F66E8(arg0, 0x25, 0);
    ov14_021F2534(arg0);
    ov14_021F0254(arg0, 4);
}

s32 ov14_021E76B8();

s32 ov14_021F0AD8(UnkStruct_ov14_boxapp *);

void ov14_021F25C4(void *arg0);

void ov14_021F25C4(void *arg0) {
    ov14_021E76B8();
    ov14_021F0AD8(arg0);
}

s32 ov14_021F0C88(UnkStruct_ov14_boxapp *);

void ov14_021F25D4(UnkStruct_ov14_boxapp *arg0);

void ov14_021F25D4(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E76B8();
    ov14_021F0C88(arg0);
}

void ov14_021F1100(UnkStruct_ov14_boxapp *param_1, u8 param_2);

void ov14_021F40E8(UnkStruct_ov14_boxapp *param_1, s32 param_2);

void ov14_021F25E4(struct UnkStruct_ov14_boxapp *param_1);

void ov14_021F25E4(UnkStruct_ov14_boxapp *param_1) {
    ov14_021F40E8(param_1, 0);
    *((u8 *)&param_1->unk23[1]) = 1;
    *((u8 *)&param_1->unk26[3]) = 0;
    ov14_021F2A18(*((void **)&param_1->sys), 9, 0);
    ov14_021F1100(param_1, 0x2f);
    return;
}

struct MapObject;

struct ObjectEventGfxRenderDetailsEntry;

typedef struct MapObject MapObject;

void ov14_021F027C(MapObject *mapObj, s8 param1);

void ov14_021F2610(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails);

void ov14_021F2610(MapObject *mapObj, u8 currTileBehavior, u8 prevTileBehavior, const ObjectEventGfxRenderDetailsEntry *renderDetails) {
    ov14_021F027C(mapObj, 0);
}

void ov14_021F0910(UnkStruct_ov14_boxapp *param_1);

void ov14_021F261C(UnkStruct_ov14_boxapp *param_1);

void ov14_021F261C(UnkStruct_ov14_boxapp *param_1) {
    ov14_021F0910(param_1);
}

void ov14_021F0A80(void *arg0);

void ov14_021F2624(void *arg0);

void ov14_021F2624(void *arg0) {
    ov14_021F0A80(arg0);
}

void ov14_021F09BC(void *arg0);

void ov14_021F262C(void *arg0);

void ov14_021F262C(void *arg0) {
    ov14_021F09BC(arg0);
}
