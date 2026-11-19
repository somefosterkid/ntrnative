#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

struct G3DPipelineBuffers;

struct SpriteList;

struct SpriteResourceCollection;

struct TouchHitboxController;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov49_0225BA5C(SysTask *param0, void *param1);

void ov49_0225BA40(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, u32 heapID);

void ov49_0225BA40(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, u32 param2, u32 heapID) {
    param0->unk_154 = param2;
    param0->unk_156 = heapID;

    SysTask_CreateOnVWaitQueue(ov49_0225BA5C, param0, 0);
}
