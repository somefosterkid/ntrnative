#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "error_handling.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_90.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"

struct BufferManager;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov66_0222DFF8_t;

struct YesNoPrompt;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

extern const u8 ov90_0225C1EC[4];

void ov90_0225B2A8(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B330(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B2A8(UnkStruct_ov114_0225E854 *param0) {
    int v0;
    u32 v1;
    u8 *v2;
    BOOL v3;

    if (param0->unk_6C2 == 0) {
        return;
    }

    if ((param0->unk_6C0 == 0) || (param0->unk_6C0 == 8)) {
        for (v0 = 0; v0 < param0->unk_0C.unk_08; v0++) {
            if (param0->unk_1C.unk_10[v0] == 0) {
                v1 = (ov90_0225C1EC[v0] * 32);
                v2 = param0->unk_6BC->pRawData;

                if (param0->unk_6C0 == 0) {
                    v2 += ((3 + ov90_0225C1EC[v0]) * 32);
                } else {
                    v2 += ((7 + ov90_0225C1EC[v0]) * 32);
                }

                v3 = GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, v1, v2, 32);
                GF_ASSERT(v3);
            }
        }
    }

    param0->unk_6C0 = (param0->unk_6C0 + 1) % (8 * 2);
}

void ov90_0225B330(UnkStruct_ov114_0225E854 *param0) {
    Heap_Free(param0->unk_6B8);
}

void ov90_0225B340(UnkStruct_ov114_0225F270 *arg0, UnkStruct_ov114_0225CEF0 *arg1, s8 arg2, enum HeapID arg3) {
    memset(arg0, 0, sizeof(UnkStruct_ov114_0225F270));
    arg0->unk_08.bgConfig = arg1->unk_00;
    arg0->unk_08.bgId = 4;
    arg0->unk_08.tileStart = 0x8B;
    arg0->unk_08.plttSlot = 2;
    arg0->unk_08.x = 0x18;
    arg0->unk_08.y = 8;
    arg0->unk_08.shapeParam = 0;
    arg0->unk_04 = YesNoPrompt_Create(arg3);
    arg0->unk_01 = arg2;
    arg0->unk_1D = 1;
}
