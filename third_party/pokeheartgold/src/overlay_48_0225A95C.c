#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "vram_transfer_manager.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

void ov48_0225A95C(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1);

void ov48_0225AA38(u8 *arg0, u32 arg1, s8 arg2);
void ov48_0225AA50(void *arg0);

void ov48_0225AA5C(UnkStruct_ov69_0225E7DC *param0, u32 param1, UnkStruct_ov69_0225D35C *param2);

void ov48_0225A95C(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1) {
    int v0;
    u32 v1, v2;

    v2 = (param0->unk_00 * param0->unk_18) / param0->unk_02;
    v1 = (param0->unk_00 * param0->unk_24) / param0->unk_02;

    param0->unk_00 = (param0->unk_00 + 1) % param0->unk_02;

    if ((param0->unk_18 > 0) && (v2 != param0->unk_1A)) {
        param0->unk_1A = v2;

        for (v0 = 0; v0 < 3; v0++) {
            if (param0->unk_04[v0] == 1) {
                CopyToBgTilemapRect(param1->unk_00, 6, 0, 0 + (6 * v0), 32, 6, param0->unk_10[param0->unk_1A]->rawData, 0, 0 + (6 * v0), param0->unk_10[param0->unk_1A]->screenWidth / 8, param0->unk_10[param0->unk_1A]->screenHeight / 8);
                ScheduleBgTilemapBufferTransfer(param1->unk_00, 6);
            }
        }
    }

    if ((param0->unk_24 > 0) && (v1 != param0->unk_25)) {
        BOOL v3;
        u8 *v4;

        param0->unk_25 = v1;

        v4 = (u8 *)param0->unk_20->pRawData;
        v3 = GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, param0->unk_26 * 32, &v4[param0->unk_25 * 32], 32);
        GF_ASSERT(v3);
    }
}

void ov48_0225AA38(u8 *arg0, u32 arg1, s8 arg2) {
    if (arg1 >= 3U) {
        GF_AssertFail();
    }
    *(s8 *)((u8 *)(arg0 + arg1) + 4) = arg2;
}

void ov48_0225AA50(void *arg0) {
    *(s8 *)((u8 *)arg0 + 4) = 0;
    *(s8 *)((u8 *)arg0 + 5) = 0;
    *(s8 *)((u8 *)arg0 + 6) = 0;
    *(s8 *)((u8 *)arg0 + 7) = 0;
}

void ov48_0225AA5C(UnkStruct_ov69_0225E7DC *param0, u32 param1, UnkStruct_ov69_0225D35C *param2) {
    if (param0->unk_18 > 0) {
        CopyToBgTilemapRect(param2->unk_00, 6, 0, 0 + param1, 32, 1, param0->unk_10[param0->unk_1A]->rawData, 0, 0 + param1, param0->unk_10[param0->unk_1A]->screenWidth / 8, param0->unk_10[param0->unk_1A]->screenHeight / 8);
        ScheduleBgTilemapBufferTransfer(param2->unk_00, 6);
    }
}
