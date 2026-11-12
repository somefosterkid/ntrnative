// Black's code at ov021 0x021bddc0-0x021be2a8. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021bddc0.h"
#include "field/city_state.h"
#include "field/field.h"
#include "field/field_map.h"
#include "field/event_entrance_effect.h"
#include "gfl/arc.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "gfl/overlay.h"
#include "field/field_camera.h"
#include "nitro/fx/fx.h"

void func_ov038_021f36c0(Field *field);
void func_ov038_021f39b4(Field *field);
void func_ov038_021f39d8(Field *field);
void func_ov039_021f36fc(Field *field);
void func_ov039_021f3cd8(Field *field);
void func_ov039_021f3d1c(Field *field);
void GymNacrene_Init(Field *field);
void GymNacrene_End(Field *field);
void GymNacrene_Update(Field *field);
void func_ov041_021f36c0(Field *field);
void func_ov041_021f37b8(Field *field);
void func_ov041_021f37d4(Field *field);
void func_ov042_021f36c0(Field *field);
void func_ov042_021f380c(Field *field);
void func_ov042_021f3828(Field *field);
void func_ov043_021f36c0(Field *field);
void func_ov043_021f3938(Field *field);
void func_ov043_021f395c(Field *field);
void func_ov044_021f36c0(Field *field);
void func_ov044_021f3740(Field *field);
void func_ov044_021f3764(Field *field);
void func_ov045_021f36c0(Field *field);
void func_ov045_021f37f8(Field *field);
void func_ov045_021f381c(Field *field);
void func_ov046_021f36c0(Field *field);
void func_ov046_021f3a04(Field *field);
void func_ov046_021f3a28(Field *field);
void func_ov047_021f36c0(Field *field);
void func_ov047_021f370c(Field *field);
void func_ov047_021f3740(Field *field);
void func_ov048_021f36c0(Field *field);
void func_ov048_021f374c(Field *field);
void func_ov048_021f376c(Field *field);
void func_ov049_021f36c0(Field *field);
void func_ov049_021f36ec(Field *field);
void func_ov049_021f3708(Field *field);
void func_ov050_021f36c0(Field *field);
void func_ov050_021f36ec(Field *field);
void func_ov050_021f3708(Field *field);
void func_ov051_021f3730(Field *field);
void func_ov051_021f3868(Field *field);
void func_ov051_021f38a0(Field *field);
void func_ov052_021f36c0(Field *field);
void func_ov052_021f3724(Field *field);
void func_ov052_021f3770(Field *field);
void func_ov053_021f36c0(Field *field);
void func_ov053_021f3734(Field *field);
void func_ov053_021f3778(Field *field);
void func_ov054_021f36c0(Field *field);
void func_ov054_021f3704(Field *field);
void func_ov054_021f373c(Field *field);
void func_ov058_021f36c0(Field *field);
void func_ov058_021f3714(Field *field);
void func_ov058_021f373c(Field *field);
void func_ov059_021f36c0(Field *field);
void func_ov059_021f376c(Field *field);
void func_ov059_021f3788(Field *field);
void func_ov059_021f3794(Field *field);
void func_ov059_021f3840(Field *field);
void func_ov059_021f385c(Field *field);
void func_ov059_021f3868(Field *field);
void func_ov059_021f3980(Field *field);
void func_ov059_021f39ac(Field *field);
void func_ov060_021f36c0(Field *field);
void func_ov060_021f3724(Field *field);
void func_ov060_021f374c(Field *field);
void func_ov061_021f36c0(Field *field);
void func_ov061_021f3754(Field *field);
void func_ov061_021f376c(Field *field);
void func_ov062_021f36c0(Field *field);
void func_ov062_021f3738(Field *field);
void func_ov062_021f3748(Field *field);
void func_ov064_021f36c0(Field *field);
void func_ov064_021f3790(Field *field);
void func_ov064_021f37ac(Field *field);
void func_ov065_021f36c0(Field *field);
void func_ov065_021f37c4(Field *field);
void func_ov065_021f37dc(Field *field);
static const u32 data_ov021_021d9d98[55] = {
#include "asset/ov021_021bddc0_data_ov021_021d9d98.inc"
};

static void (*const data_ov021_021d9cbc[55])(Field *) = {
    NULL, func_ov053_021f36c0, func_ov039_021f36fc,
    GymNacrene_Init, func_ov042_021f36c0, func_ov041_021f36c0,
    func_ov038_021f36c0, func_ov043_021f36c0, func_ov044_021f36c0,
    func_ov045_021f36c0, func_ov046_021f36c0, func_ov054_021f36c0,
    func_ov048_021f36c0, func_ov048_021f36c0, func_ov048_021f36c0,
    func_ov048_021f36c0, func_ov048_021f36c0, func_ov048_021f36c0,
    func_ov048_021f36c0, func_ov048_021f36c0, func_ov048_021f36c0,
    func_ov048_021f36c0, func_ov048_021f36c0, func_ov048_021f36c0,
    func_ov048_021f36c0, func_ov051_021f3730, func_ov051_021f3730,
    func_ov051_021f3730, func_ov049_021f36c0, func_ov050_021f36c0,
    func_ov048_021f36c0, func_ov047_021f36c0, func_ov052_021f36c0,
    func_ov052_021f36c0, func_ov052_021f36c0, func_ov052_021f36c0,
    func_ov052_021f36c0, func_ov052_021f36c0, func_ov052_021f36c0,
    func_ov052_021f36c0, func_ov052_021f36c0, func_ov048_021f36c0,
    func_ov048_021f36c0, func_ov048_021f36c0, func_ov048_021f36c0,
    func_ov058_021f36c0, func_ov059_021f36c0, func_ov059_021f3868,
    func_ov059_021f3794, func_ov060_021f36c0, func_ov061_021f36c0,
    func_ov052_021f36c0, func_ov062_021f36c0, func_ov064_021f36c0,
    func_ov065_021f36c0,
};

static void (*const data_ov021_021d9be0[55])(Field *) = {
    NULL, func_ov053_021f3734, func_ov039_021f3cd8,
    GymNacrene_End, func_ov042_021f380c, func_ov041_021f37b8,
    func_ov038_021f39b4, func_ov043_021f3938, func_ov044_021f3740,
    func_ov045_021f37f8, func_ov046_021f3a04, func_ov054_021f3704,
    func_ov048_021f374c, func_ov048_021f374c, func_ov048_021f374c,
    func_ov048_021f374c, func_ov048_021f374c, func_ov048_021f374c,
    func_ov048_021f374c, func_ov048_021f374c, func_ov048_021f374c,
    func_ov048_021f374c, func_ov048_021f374c, func_ov048_021f374c,
    func_ov048_021f374c, func_ov051_021f3868, func_ov051_021f3868,
    func_ov051_021f3868, func_ov049_021f36ec, func_ov050_021f36ec,
    func_ov048_021f374c, func_ov047_021f370c, func_ov052_021f3724,
    func_ov052_021f3724, func_ov052_021f3724, func_ov052_021f3724,
    func_ov052_021f3724, func_ov052_021f3724, func_ov052_021f3724,
    func_ov052_021f3724, func_ov052_021f3724, func_ov048_021f374c,
    func_ov048_021f374c, func_ov048_021f374c, func_ov048_021f374c,
    func_ov058_021f3714, func_ov059_021f376c, func_ov059_021f3980,
    func_ov059_021f3840, func_ov060_021f3724, func_ov061_021f3754,
    func_ov052_021f3724, func_ov062_021f3738, func_ov064_021f3790,
    func_ov065_021f37c4,
};



static void (*const data_ov021_021d9e74[55])(Field *) = {
    NULL, func_ov053_021f3778, func_ov039_021f3d1c,
    GymNacrene_Update, func_ov042_021f3828, func_ov041_021f37d4,
    func_ov038_021f39d8, func_ov043_021f395c, func_ov044_021f3764,
    func_ov045_021f381c, func_ov046_021f3a28, func_ov054_021f373c,
    func_ov048_021f376c, func_ov048_021f376c, func_ov048_021f376c,
    func_ov048_021f376c, func_ov048_021f376c, func_ov048_021f376c,
    func_ov048_021f376c, func_ov048_021f376c, func_ov048_021f376c,
    func_ov048_021f376c, func_ov048_021f376c, func_ov048_021f376c,
    func_ov048_021f376c, func_ov051_021f38a0, func_ov051_021f38a0,
    func_ov051_021f38a0, func_ov049_021f3708, func_ov050_021f3708,
    func_ov048_021f376c, func_ov047_021f3740, func_ov052_021f3770,
    func_ov052_021f3770, func_ov052_021f3770, func_ov052_021f3770,
    func_ov052_021f3770, func_ov052_021f3770, func_ov052_021f3770,
    func_ov052_021f3770, func_ov052_021f3770, func_ov048_021f376c,
    func_ov048_021f376c, func_ov048_021f376c, func_ov048_021f376c,
    func_ov058_021f373c, func_ov059_021f3788, func_ov059_021f39ac,
    func_ov059_021f385c, func_ov060_021f374c, func_ov061_021f376c,
    func_ov052_021f3770, func_ov062_021f3748, func_ov064_021f37ac,
    func_ov065_021f37dc,
};

FieldCamera *func_ov021_021be17c(EntranceCamera *camera);
u32 func_ov021_021be194(EntranceCamera *camera);
u32 func_ov021_021be1b0(EntranceCamera *camera);
void func_ov021_021be148(EntranceCameraRecord *record, s32 param_2);
void FieldCamera_ChangeRefPos(FieldCamera *param_1, VecFx32 *param_2);




void func_ov021_021bddc0(EntranceCamera *camera) {
    FieldCameraAnimation *animation = &camera->animation;
    FieldCameraCoords *dst = &animation->dst;
    FieldPlayer *player;
    u16 dir;
    VecFx32 pos;

    func_ov021_021be17c(camera);
    player = Field_GetPlayer(camera->field);
    dir = FieldPlayer_GetFaceDir(player);
    switch (dir) {
    case 2:
    case 3:
        camera->unk_68 = 1;
        camera->unk_6C = 0;
        break;
    case 0:
    case 1:
        camera->unk_68 = 0;
        camera->unk_6C = 0;
        break;
    }
    if (camera->unk_68 == 0) {
        return;
    }
    animation->unk4C = 0;
    animation->unk48 = 0;
    if (func_ov021_021be1b0(camera)) {
        func_ov021_021a7b08(camera->field, &pos);
    } else {
        FieldPlayer_GetWPos(player, &pos);
    }
    FieldCameraAnimationController_PrepareCamera(camera->controller);
    FieldCamera_GetAnimationCoords(camera->camera, &animation->src);
    {
        // A leftover, zeroed and never read, as in pokebw2's twin (entrance_camera.c)
        VecFx32 zero = { 0, 0, 0 };

        FieldCamera_GetAnimationCoords(camera->camera, dst);
    }
    switch (dir) {
    case 0:
        dst->distance = 180 << 12;
        break;
    case 1:
        break;
    case 2:
        dst->yaw = 0xa09;
        break;
    case 3:
        dst->yaw = 0xf5f7;
        break;
    }
    switch (dir) {
    case 0:
        animation->frames = 0;
        return;
    case 1:
        animation->frames = 0;
        return;
    case 2:
        animation->frames = 10;
        return;
    case 3:
        animation->frames = 10;
        return;
    }
}

void func_ov021_021bdeb4(EntranceCamera *camera) {
    FieldCameraAnimation *animation = &camera->animation;
    FieldCameraCoords *src = &animation->src;
    FieldCameraCoords *dst = &animation->dst;
    FieldCamera *fieldCamera;
    FieldPlayer *player;
    u16 dir;
    VecFx32 pos;

    fieldCamera = func_ov021_021be17c(camera);
    player = Field_GetPlayer(camera->field);
    dir = FieldPlayer_GetFaceDir(player);
    switch (dir) {
    case 1:
    case 2:
    case 3:
        camera->unk_68 = 0;
        camera->unk_6C = 1;
        break;
    case 0:
        camera->unk_68 = 0;
        camera->unk_6C = 0;
        break;
    }
    if (camera->unk_6C == 0) {
        return;
    }
    animation->unk4C = 0;
    animation->unk48 = 0;
    if (func_ov021_021be1b0(camera)) {
        func_ov021_021a7b08(camera->field, &pos);
    } else {
        FieldPlayer_GetWPos(player, &pos);
    }
    switch (dir) {
    case 0:
        animation->frames = 0;
        break;
    case 1:
        animation->frames = 10;
        break;
    case 2:
        animation->frames = 10;
        break;
    case 3:
        animation->frames = 10;
        break;
    }
    if (func_ov021_021be1b0(camera)) {
        FieldCamera_ChangeRefPos(fieldCamera, &pos);
    }
    FieldCameraAnimationController_PrepareCamera(camera->controller);
    FieldCamera_GetAnimationCoords(camera->camera, src);
    switch (dir) {
    case 0:
        break;
    case 1:
        src->distance = 180 << 12;
        break;
    case 2:
        src->yaw = 0xf5f7;
        break;
    case 3:
        src->yaw = 0xa09;
        break;
    }
    FieldCamera_GetAnimationCoords(camera->camera, dst);
    if (camera->unk_6C) {
        FieldCameraAnimationController_CommitSrcCoords(camera->controller);
    }
}

void func_ov021_021bdfb0(EntranceCamera *camera) {
    FieldCameraAnimation *animation = &camera->animation;
    FieldCameraCoords *dst = &animation->dst;
    FieldPlayer *player;
    VecFx32 pos;
    EntranceCameraRecord record;

    func_ov021_021be17c(camera);
    player = Field_GetPlayer(camera->field);
    FieldPlayer_GetFaceDir(player);
    func_ov021_021be148(&record, func_ov021_021be194(camera));
    animation->unk4C = record.unk_3C;
    animation->unk48 = record.unk_38;
    camera->unk_68 = record.unk_30;
    camera->unk_6C = record.unk_34;
    if (camera->unk_68 != 0) {
        if (func_ov021_021be1b0(camera)) {
            func_ov021_021a7b08(camera->field, &pos);
        } else {
            FieldPlayer_GetWPos(player, &pos);
        }
        FieldCameraAnimationController_PrepareCamera(camera->controller);
        FieldCamera_GetAnimationCoords(camera->camera, &animation->src);
        animation->frames = record.frames;
        dst->pitch = record.pitch;
        dst->yaw = record.yaw;
        dst->distance = record.distance << 12;
        {
            s32 z = record.offset.z << 12;
            s32 y = record.offset.y << 12;
            s32 x = record.offset.x << 12;
            dst->offset.x = x;
            dst->offset.y = y;
            dst->offset.z = z;
        }
        if (record.hasTarget) {
            s32 z = record.target.z << 12;
            s32 y = record.target.y << 12;
            s32 x = record.target.x << 12;
            dst->target.x = x;
            dst->target.y = y;
            dst->target.z = z;
        } else {
            dst->target = pos;
        }
    }
}

void func_ov021_021be068(EntranceCamera *camera) {
    FieldCameraAnimation *animation = &camera->animation;
    FieldCameraCoords *src = &animation->src;
    FieldCameraCoords *dst = &animation->dst;
    FieldCamera *fieldCamera;
    FieldPlayer *player;
    VecFx32 pos;
    EntranceCameraRecord record;

    fieldCamera = func_ov021_021be17c(camera);
    player = Field_GetPlayer(camera->field);
    FieldPlayer_GetFaceDir(player);
    func_ov021_021be148(&record, func_ov021_021be194(camera));
    animation->unk4C = record.unk_3C;
    animation->unk48 = record.unk_38;
    camera->unk_68 = record.unk_30;
    camera->unk_6C = record.unk_34;
    if (camera->unk_6C != 0) {
        if (func_ov021_021be1b0(camera)) {
            func_ov021_021a7b08(camera->field, &pos);
        } else {
            FieldPlayer_GetWPos(player, &pos);
        }
        if (animation->unk48 != 0 && func_ov021_021be1b0(camera)) {
            FieldCamera_ChangeRefPos(fieldCamera, &pos);
        }
        FieldCameraAnimationController_PrepareCamera(camera->controller);
        FieldCamera_GetAnimationCoords(camera->camera, dst);
        animation->frames = record.frames;
        src->pitch = record.pitch;
        src->yaw = record.yaw;
        src->distance = record.distance << 12;
        {
            s32 z = record.offset.z << 12;
            s32 y = record.offset.y << 12;
            s32 x = record.offset.x << 12;
            src->offset.x = x;
            src->offset.y = y;
            src->offset.z = z;
        }
        if (record.hasTarget) {
            s32 z = record.target.z << 12;
            s32 y = record.target.y << 12;
            s32 x = record.target.x << 12;
            src->target.x = x;
            src->target.y = y;
            src->target.z = z;
        } else {
            src->target = pos;
        }
        if (camera->unk_6C) {
            FieldCameraAnimationController_CommitSrcCoords(camera->controller);
        }
    }
}




void func_ov021_021be148(EntranceCameraRecord *param_1, s32 param_2) {
    GFL_ArcSysRead(param_1, 0xb3, param_2 - 7);
}

void FieldCamera_ChangeRefPos(FieldCamera *param_1, VecFx32 *param_2) {
    FieldCamera_ClearBind(param_1);
    FieldCamera_CoordsSetTarget(param_1, param_2);
    FieldCamera_CalcTransform(param_1, 0);
    FieldCamera_ResetBind(param_1);
}

FieldCamera *func_ov021_021be17c(EntranceCamera *camera) {
    return camera->camera;
}

EntranceCameraParam *func_ov021_021be180(EntranceCamera *camera) {
    return &camera->param;
}

void func_ov021_021be184(EntranceCamera *camera, const EntranceCameraParam *param) {
    camera->param = *param;
}


u32 func_ov021_021be194(EntranceCamera *camera) {
    return func_ov021_021be180(camera)->transitionType;
}

BOOL func_ov021_021be1a0(EntranceCamera *camera) {
    return func_ov010_02162370(func_ov021_021be180(camera)->transitionType);
}

u32 func_ov021_021be1b0(EntranceCamera *camera) {
    if (func_ov021_021be180(camera)->isDeparture == 0) {
        return 1;
    }
    return 0;
}

u32 func_ov021_021be1c4(EntranceCamera *camera) {
    if (func_ov021_021be180(camera)->isArrival == 0) {
        return camera->unk_68;
    }
    return camera->unk_6C;
}

void func_ov021_021be1dc(Field *field) {
    u32 id = GimmickState_GetID(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))));

    if (id != 0xffffffff) {
        if (data_ov021_021d9d98[id] != 0) {
            GFL_OvlLoad(data_ov021_021d9d98[id]);
        }
        data_ov021_021d9cbc[id](field);
    }
}

void func_ov021_021be218(Field *field) {
    u32 id = GimmickState_GetID(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))));

    if (id != 0xffffffff) {
        if (data_ov021_021d9be0[id] != NULL) {
            data_ov021_021d9be0[id](field);
        }
        if (data_ov021_021d9d98[id] != 0) {
            GFL_OvlUnload(data_ov021_021d9d98[id]);
        }
    }
}

void func_ov021_021be258(Field *field) {
    u32 id = GimmickState_GetID(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))));

    if (id != 0xffffffff && data_ov021_021d9e74[id] != NULL) {
        data_ov021_021d9e74[id](field);
    }
}

u32 func_ov021_021be288(Field *field, u32 id) {
    if (id == GimmickState_GetID(GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field))))) {
        return TRUE;
    }
    return FALSE;
}
