#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "touch_hitbox_controller.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

typedef struct {
    s16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    Window unk_0C;
    TouchHitboxController *unk_1C;
    void *unk_20[3];
    NNSG2dScreenData *unk_2C[3];
} UnkStruct_ov70_02260744;

void ov49_0225BB14(UnkStruct_ov70_0225E4EC *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 heapID);

extern s32 ov49_022696EC;

extern s32 ov49_022696F0;

void ov49_0225CAD4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, NARC *param3, u32 heapID);

void ov49_0225CAD4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, NARC *param3, u32 heapID) {
    CopyToBgTilemapRect(param1->unk_00, 5U, 0U, 3U, 0x20U, 0x15U, param0->unk_2C[param2]->rawData, 0U, 3U, 0x20U, 0x20U);
    ScheduleBgTilemapBufferTransfer(param1->unk_00, 5U);
    if (param0->unk_08 == 1) {
        ov49_0225BB14(param1, param3, *(u8 *)(((u8 *)&ov49_022696EC) + param2) + (param0->unk_0A * 2) + 0x60, 6U, 0x140U, heapID);
    }
    ScheduleSetBgPosText(param1->unk_00, 6U, BG_POS_OP_SET_XSCALE, (s32) * (s8 *)(((u8 *)&ov49_022696F0) + param2));
}
