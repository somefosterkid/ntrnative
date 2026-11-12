// Black's code at ov047 0x021f36c0-0x021f3840. No name is known: the module and address.

#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_map.h"
#include "field/field_player.h"
#include "nitro/fx/fx.h"
#include "nitro/mi.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/fp.h"
#include "gfl/math_util.h"
#include "gfl/random.h"

// The gimmick's work: the player, its actor and the actor's offset, whose height bobs on a sine wave
typedef struct {
    FieldPlayer *player;
    FieldActor *actor;
    VecFx32 offset;
} Ov047Work;

BOOL func_ov021_021ab44c(u32 tileClass);
BOOL func_ov047_021f381c(Ov047Work *work);

void func_ov047_021f36c0(Field *param_1) {
    u16 heapId;
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov047Work *gimmickWorkBlock;
    FieldPlayer *player;
    FieldActor *pFVar2;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 0x1f);
    heapId = Field_GetHeapID(param_1);
    Field_AllocGimmickWorkBlock(param_1, 1, heapId, sizeof(Ov047Work));
    gimmickWorkBlock = Field_GetGimmickWorkBlock(param_1, 1);
    player = Field_GetPlayer(param_1);
    gimmickWorkBlock->player = player;
    pFVar2 = FieldPlayer_GetActor(player);
    gimmickWorkBlock->actor = pFVar2;
    gimmickWorkBlock->offset.x = 0;
    gimmickWorkBlock->offset.y = 0;
    gimmickWorkBlock->offset.z = 0;
}

void func_ov047_021f370c(Field *param_1) {
    GameSystem *gsys;
    GameData *gameData;
    GimmickState *gimmick;
    Ov047Work *gimmickWorkBlock;

    gsys = Field_GetGameSystem(param_1);
    gameData = GSYS_GetGameData(gsys);
    gimmick = GameData_GetGimmickState(gameData);
    GimmickState_GetUserData(gimmick, 0x1f);
    gimmickWorkBlock = Field_GetGimmickWorkBlock(param_1, 1);
    func_ov010_0216d8e0(gimmickWorkBlock->actor, &gimmickWorkBlock->offset);
    Field_DeleteGimmickWorkBlock(param_1, 1);
}

static const u8 data_ov047_021f3840[4] = { 8, 8, 12, 14 };

void func_ov047_021f3740(Field *field) {
    GameSystem *gsys = Field_GetGameSystem(field);
    GameData *gameData = GSYS_GetGameData(gsys);
    GimmickState *gimmick = GameData_GetGimmickState(gameData);
    s16 *userData = GimmickState_GetUserData(gimmick, 0x1f);
    Ov047Work *work = Field_GetGimmickWorkBlock(field, 1);
    f32 f;
    s32 value;
    s64 product;
    fx16 scale;

    if (func_ov047_021f381c(work) == FALSE) {
        work->offset.y = 0;
        userData[0] = 0;
        func_ov010_0216d8e0(work->actor, &work->offset);
        return;
    }
    if (userData[0] == 0) {
        userData[1] = data_ov047_021f3840[GFL_RandomLCAlt(4)];
    }
    if (userData[1] > 0) {
        f = 0.5f + (f32)(userData[1] << 12);
    } else {
        f = (f32)(userData[1] << 12) - 0.5f;
    }
    value = (s32)f;
    scale = func_0203f684(userData[0]);
    product = (s64)scale * value;
    work->offset.y = (fx32)((product + 0x800) >> 12);
    func_ov010_0216d8e0(work->actor, &work->offset);
    userData[0] = ((s32)userData[0] + 8) % 360;
}


void func_ov047_021f3800(GameSystem *param_1) {
    GameData *gameData;
    GimmickState *gimmick;
    void *dest;

    gameData = GSYS_GetGameData(param_1);
    gimmick = GameData_GetGimmickState(gameData);
    dest = GimmickState_GetUserData(gimmick, 0x1f);
    MI_CpuFill8(dest, 0, 4);
}

BOOL func_ov047_021f381c(Ov047Work *work) {
    u32 tileType;
    BOOL BVar2;

    tileType = FieldPlayer_GetTileTypeUnder(work->player);
    BVar2 = MapTile_IsValid(tileType);
    if (BVar2 == 0) {
        return 0;
    }
    BVar2 = func_ov021_021ab44c((u32)GetTileClass(tileType));
    return BVar2;
}
