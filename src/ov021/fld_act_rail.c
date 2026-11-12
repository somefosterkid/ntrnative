// Black's code at ov021 0x0219e0a8-0x0219e2c0 (pokebw2's fld_act_rail.c): the rail actors' jump along the rail, and the
// tables of the functions of their action commands, by the command's state, which ACTOR_ACTION_COMMAND_TABLES_RAIL lists.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/field_g3dobj.h"
#include "field/field_map.h"
#include "field/field_nogrid_mapper.h"
#include "field/field_rail.h"
#include "field/field_effect.h"
#include "field/zone.h"
#include "gfl/random.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/sound.h"

u32 func_ov021_0219dc18(FieldActor *param_1);
u32 func_ov021_0219dc28(FieldActor *param_1);
u32 func_ov021_0219dc58(FieldActor *param_1);
u32 func_ov021_0219dc64(FieldActor *param_1);
u32 func_ov021_0219dc70(FieldActor *param_1);
u32 func_ov021_0219dc7c(FieldActor *param_1);
u32 func_ov021_0219dcc4(FieldActor *param_1);
u32 func_ov021_0219dd18(FieldActor *param_1);
u32 func_ov021_0219dd24(FieldActor *param_1);
u32 func_ov021_0219dd34(FieldActor *param_1);
u32 func_ov021_0219dd44(FieldActor *param_1);
u32 func_ov021_0219dd54(FieldActor *param_1);
u32 func_ov021_0219dd64(FieldActor *param_1);
u32 func_ov021_0219dd74(FieldActor *param_1);
u32 func_ov021_0219dd84(FieldActor *param_1);
u32 func_ov021_0219dd94(FieldActor *param_1);
u32 func_ov021_0219dda4(FieldActor *param_1);
u32 func_ov021_0219ddb4(FieldActor *param_1);
u32 func_ov021_0219ddc4(FieldActor *param_1);
u32 func_ov021_0219ddd4(FieldActor *param_1);
u32 func_ov021_0219dde4(FieldActor *param_1);
u32 func_ov021_0219ddf4(FieldActor *param_1);
u32 func_ov021_0219de04(FieldActor *param_1);
u32 func_ov021_0219de14(FieldActor *param_1);
u32 func_ov021_0219de24(FieldActor *param_1);
u32 func_ov021_0219de34(FieldActor *param_1);
u32 func_ov021_0219de44(FieldActor *param_1);
u32 func_ov021_0219de54(FieldActor *param_1);
u32 func_ov021_0219de98(FieldActor *param_1);
u32 func_ov021_0219decc(FieldActor *param_1);
u32 func_ov021_0219dedc(FieldActor *param_1);
u32 func_ov021_0219deec(FieldActor *param_1);
u32 func_ov021_0219defc(FieldActor *param_1);
u32 func_ov021_0219df0c(FieldActor *param_1);
u32 func_ov021_0219df1c(FieldActor *param_1);
u32 func_ov021_0219df2c(FieldActor *param_1);
u32 func_ov021_0219df3c(FieldActor *param_1);
u32 func_ov021_0219df4c(FieldActor *param_1);
u32 func_ov021_0219df5c(FieldActor *param_1);
u32 func_ov021_0219df6c(FieldActor *param_1);
u32 func_ov021_0219df7c(FieldActor *param_1);
u32 func_ov021_0219df8c(FieldActor *param_1);
u32 func_ov021_0219df9c(FieldActor *param_1);
u32 func_ov021_0219dfac(FieldActor *param_1);
u32 func_ov021_0219dfbc(FieldActor *param_1);
u32 func_ov021_0219dfcc(FieldActor *param_1);
u32 func_ov021_0219dfdc(FieldActor *param_1);
u32 func_ov021_0219dfec(FieldActor *param_1);
u32 func_ov021_0219dffc(FieldActor *param_1);
void func_ov021_0219e084(FieldActor *param_1, u16 param_2, s32 param_3, u8 param_4, u16 param_5, s16 param_6, u16 param_7);
void func_ov021_0219e354(FieldActor *param_1);
void func_ov021_0219e370(FieldActor *param_1, u16 param_2, s32 param_3);
void func_ov021_0219e6c8(FieldActor *param_1, u16 param_2);

// How high an actor is above the rail at each stage of its jump, by the kind of jump
const s32 data_ov021_021d835c[16] = {
#include "asset/fld_act_rail_data_ov021_021d835c.inc"
};
const s32 data_ov021_021d839c[16] = {
#include "asset/fld_act_rail_data_ov021_021d839c.inc"
};
const s32 data_ov021_021d83dc[16] = {
#include "asset/fld_act_rail_data_ov021_021d83dc.inc"
};

const s32 *data_ov021_021ddd94[3] = { data_ov021_021d835c, data_ov021_021d839c, data_ov021_021d83dc };

u32 func_ov021_0219e0a8(FieldActor *param_1) {
    ActorRailJumpWork *move;
    RailUnit *unit;
    VecFx32 pos;
    VecFx32 offset;
    s32 step;
    u16 idx;
    const s32 *row;

    move = func_ov010_0216d6d4(param_1);
    move->timer--;
    if (move->speed != 0) {
        unit = FldAct_GetRailUnit(param_1);
        func_ov021_0219e354(param_1);
        RailUnit_GetCalcPos(unit, &pos);
        SetActorWPosValue(param_1, &pos);
        if (move->progress >= 0x10000 && move->timer > 0) {
            move->progress = 0;
            func_ov021_0219e370(param_1, move->dir, 8);
            func_ov021_0219e6c8(param_1, move->dir);
            SetActorMovementFlag(param_1, 0x20);
        }
        step = move->speed;
        if (step < 0) {
            step = -step;
        }
        move->progress += step;
    }
    move->phase += move->step;
    if (move->phase > 0xf00) {
        move->phase = 0xf00;
    }
    idx = move->phase / 256;
    row = data_ov021_021ddd94[move->height];
    offset.x = 0;
    offset.y = row[idx];
    offset.z = 0;
    SetActorWPosOffset(param_1, &offset);
    if (move->timer > 0 || func_ov021_021b8040(unit) != 0) {
        return 0;
    }
    VecFx32 zero = { 0, 0, 0 };

    SetActorWPosOffset(param_1, &zero);
    SetActorMovementFlag(param_1, 0x540);
    SetActorInitialGPosToNowGPos(param_1);
    func_ov010_0216d674(param_1, 0);
    FieldActor_NextAcmdState(param_1);
    GFL_SndSEPlay(0x67b);
    return 1;
}



u32 func_ov021_0219e1b0(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 0, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_0219e1d0(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 1, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_0219e1f4(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 2, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_0219e218(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 3, 0x2000, 8, 3, 0, 0x200);
    return 1;
}

u32 func_ov021_0219e238(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 0, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_0219e258(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 1, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_0219e27c(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 2, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

u32 func_ov021_0219e29c(FieldActor *param_1) {
    func_ov021_0219e084(param_1, 3, 0x2000, 0x10, 3, 0, 0x100);
    return 1;
}

const FieldActorFunc data_ov021_021d8114[2] = { func_ov021_0219dc28, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8104[2] = { func_ov021_0219dc58, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d810c[2] = { func_ov021_0219dc64, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d80fc[2] = { func_ov021_0219dc70, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d80f4[2] = { func_ov021_0219dc7c, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8308[3] = { func_ov021_0219dd24, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82fc[3] = { func_ov021_0219dd34, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82f0[3] = { func_ov021_0219dd44, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82e4[3] = { func_ov021_0219dd54, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82d8[3] = { func_ov021_0219dd64, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82cc[3] = { func_ov021_0219dd74, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82c0[3] = { func_ov021_0219dd84, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82b4[3] = { func_ov021_0219dd94, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d82a8[3] = { func_ov021_0219dda4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d829c[3] = { func_ov021_0219ddb4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8290[3] = { func_ov021_0219ddc4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8284[3] = { func_ov021_0219ddd4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8278[3] = { func_ov021_0219dde4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8248[3] = { func_ov021_0219ddf4, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8254[3] = { func_ov021_0219de04, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d826c[3] = { func_ov021_0219de14, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8314[3] = { func_ov021_0219de24, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d823c[3] = { func_ov021_0219de34, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8230[3] = { func_ov021_0219de44, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8224[3] = { func_ov021_0219de54, func_ov021_0219dcc4, func_ov021_0219dd18 };
const FieldActorFunc data_ov021_021d8134[3] = { func_ov021_0219decc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8350[3] = { func_ov021_0219dedc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8200[3] = { func_ov021_0219deec, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81f4[3] = { func_ov021_0219defc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81e8[3] = { func_ov021_0219df0c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81dc[3] = { func_ov021_0219df1c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81d0[3] = { func_ov021_0219df2c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81c4[3] = { func_ov021_0219df3c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81b8[3] = { func_ov021_0219df4c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8194[3] = { func_ov021_0219df5c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81a0[3] = { func_ov021_0219df6c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8260[3] = { func_ov021_0219df7c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8320[3] = { func_ov021_0219dfcc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d817c[3] = { func_ov021_0219dfdc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d811c[3] = { func_ov021_0219dfec, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d820c[3] = { func_ov021_0219dffc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8158[3] = { func_ov021_0219df8c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8140[3] = { func_ov021_0219df9c, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d814c[3] = { func_ov021_0219dfac, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d81ac[3] = { func_ov021_0219dfbc, func_ov021_0219de98, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d832c[3] = { func_ov021_0219e1b0, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8218[3] = { func_ov021_0219e1d0, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8164[3] = { func_ov021_0219e1f4, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8188[3] = { func_ov021_0219e218, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8338[3] = { func_ov021_0219e238, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8128[3] = { func_ov021_0219e258, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8344[3] = { func_ov021_0219e27c, func_ov021_0219e0a8, func_ov021_0219dc18 };
const FieldActorFunc data_ov021_021d8170[3] = { func_ov021_0219e29c, func_ov021_0219e0a8, func_ov021_0219dc18 };
