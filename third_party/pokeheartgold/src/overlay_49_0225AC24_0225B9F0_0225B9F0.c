#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_02260744.h"
#include "overlay_49_unk_struct_ov70_0225fa84.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

struct TouchHitboxController;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov49_0225BA20(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1);

void ov49_0225CA30(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, NARC *param3, u32 heapID);

void ov49_0225B9F0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID, u32 param3);

void ov49_0225B9F0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID, u32 param3) {
    if (param0->unk_01 == 3) {
        ov49_0225BA20(param0, param1);
    }

    ov49_0225CA30(&param0->unk_114, param1, param3, param0->unk_14C, heapID);
    param0->unk_01 = 3;
}
