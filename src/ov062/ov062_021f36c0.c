// Black's code at ov062 0x021f36c0-0x021f3754. No name is known: the module and address (Black's field gimmick 52, the
// only code of overlay 62; no assert or twin names it).

#include "field/ov062.h"
#include "types.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "struct_decls.h"
#include "field/field_map.h"
#include "gfl/g3d.h"
#include "system/game_data.h"
#include "system/game_system.h"

static const G3DSceneAnimationSetup data_ov062_021f3754[] = { { 1, 0 } };

static const G3DSceneResourceSetup data_ov062_021f3778[] = {
    { 0xd9, 0, 0, 0 },
    { 0xd9, 1, 0, 0 },
};

static const G3DSceneActorSetup data_ov062_021f3758[] = {
    { 0, 0, 0, 0, data_ov062_021f3754, NELEMS(data_ov062_021f3754) },
};

static const G3DSceneSetup data_ov062_021f3768 = { data_ov062_021f3778, NELEMS(data_ov062_021f3778),
                                                   data_ov062_021f3758, NELEMS(data_ov062_021f3758) };

void func_ov062_021f36c0(Field *field) {
    const VecFx32 pos = { FX32_CONST(5160), FX32_CONST(8), FX32_CONST(12280) };
    u32 *userData;
    FieldExpObjSystem *system;

    userData = GimmickState_GetUserData(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))), 52);
    system = Field_GetExpObjSystem(field);
    LoadFieldExpandObjData(system, &data_ov062_021f3768, 0);
    FieldExpObj_GetActorMatrixPtr(system, 0, 0)->translation = pos;
    func_ov021_021be4a4(system, 0, 0, TRUE);
    FieldExpObj_SetAnm(system, 0, 0, 0, TRUE);
    if (*userData != 0) {
        FieldExpObj_SetActorHidden(system, 0, 0, TRUE);
    }
}

void func_ov062_021f3738(Field *field) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(field);
    FieldExpObj_FreeScene(system, 0);
}

void func_ov062_021f3748(Field *field) {
    FieldExpObjSystem *system;

    system = Field_GetExpObjSystem(field);
    FieldExpObj_StepAllAnimations(system);
}
