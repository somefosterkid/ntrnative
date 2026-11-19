#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touch_hitbox_controller.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

struct TouchHitboxController;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;


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

void ov49_0225CAD4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, NARC *param3, u32 heapID);

BOOL ov49_0225C8D4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID);

BOOL ov49_0225C8D4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID) {
    BOOL v0 = 0;

    if (param0->unk_06 == 0) {
        TouchHitboxController_IsTriggered(param0->unk_1C);
    } else {
        param0->unk_03 = 1;
    }

    if (param0->unk_02 == 1) {
        if (param0->unk_00 == 0) {
            param0->unk_02 = 0;
            ov49_0225CAD4(param0, param1, 0, param2, heapID);
        }
    }

    if (param0->unk_03 != param0->unk_04) {
        param0->unk_04 = param0->unk_03;

        if (param0->unk_03 == 2) {
            ov49_0225CAD4(param0, param1, 1, param2, heapID);
        } else {
            if (param0->unk_02 == 0) {
                ov49_0225CAD4(param0, param1, 0, param2, heapID);
            } else {
                ov49_0225CAD4(param0, param1, 2, param2, heapID);
            }
        }

        if ((param0->unk_02 == 0) && (param0->unk_03 == 2)) {
            v0 = 1;
            param0->unk_02 = 1;
            param0->unk_00 = 1;
        }
    }

    return v0;
}
