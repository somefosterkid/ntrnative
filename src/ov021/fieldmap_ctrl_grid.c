// Black's code at ov021 0x021a2d88-0x021a2e50. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_controller.h"
#include "field/field_player.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/key.h"

void func_ov021_021a2d88(Field *param_1, VecFx32 *pos, u16 param_3);
void func_ov021_021a2de4(Field *param_1);
void func_ov021_021a2df0(Field *field, VecFx32 *pos);
VecFx32 *func_ov021_021a2e34(Field *param_1);

// pokebw2's FieldmapCtrlGrid: the grid controller's work, Black's layout the same
typedef struct FieldmapCtrlGrid {
    u16 heapId;
    Field *field;
    u32 unk08;
    BOOL updateDisable;
} FieldmapCtrlGrid;

// The controller of the maps where the player moves on the grid, which fieldmap.c's table of map kinds lists
const FieldmapCtrlVTable data_ov021_021d88f8 = {
    0, func_ov021_021a2d88, func_ov021_021a2df0, func_ov021_021a2de4, func_ov021_021a2e34,
};

void func_ov021_021a2d88(Field *param_1, VecFx32 *pos, u16 param_3) {
    u16 heapId;
    FieldmapCtrlGrid *controller;
    FieldPlayer *player;
    FieldActor *actor;
    VecFx32 *position;

    heapId = Field_GetHeapID(param_1);
    controller = GFL_HeapAllocate(heapId, sizeof(FieldmapCtrlGrid), 1, "fieldmap_ctrl_grid.c", 0x68);
    controller->heapId = heapId;
    controller->field = param_1;
    player = Field_GetPlayer(param_1);
    actor = FieldPlayer_GetActor(player);
    CheckSetActorFaceDir(actor, param_3);
    FieldPlayer_InitGrid(player, heapId);
    position = GetMModelWPosPtr(actor);
    Field_SetPlayerPosPtr(param_1, position);
    Field_SetController(param_1, controller);
}

void func_ov021_021a2de4(Field *param_1) {
    void *ptr;

    ptr = Field_GetController(param_1);
    GFL_HeapFree(ptr);
}

void func_ov021_021a2df0(Field *field, VecFx32 *pos) {
    FieldmapCtrlGrid *controller;
    FieldPlayer *player;
    u32 pressedKeys;
    u32 heldKeys;
    u32 flags;

    controller = Field_GetController(field);
    player = Field_GetPlayer(field);
    if (controller->updateDisable == FALSE) {
        pressedKeys = GCTX_HIDGetPressedKeys();
        heldKeys = GCTX_HIDGetHeldKeys();
        flags = 0;
        if (FieldPlayer_CheckRunningShoesFlag(player) == TRUE) {
            flags |= 1;
        }
        FieldPlayer_UpdateGridMovement(player, pressedKeys, heldKeys, flags);
    }
}


VecFx32 *func_ov021_021a2e34(Field *param_1) {
    FieldPlayer *player;
    FieldActor *actor;

    player = Field_GetPlayer(param_1);
    actor = FieldPlayer_GetActor(player);
    return GetMModelWPosPtr(actor);
}

void FieldmapCtrlGrid_SetUpdateDisable(Field *field, BOOL disable) {
    FieldmapCtrlGrid *controller = Field_GetController(field);

    controller->updateDisable = disable;
}
