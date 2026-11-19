#include "global.h"
#include "constants/heap.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "bg_window.h"
#include "heap.h"
#include "palette.h"
#include "overlay_12_field_battle_dto.h"

s32 sub_02074F74(u32 param_1, s32 *param_2);

struct FieldBattleDTO;
struct SavePalPad;
typedef struct LinkBattleCommState {
    FieldBattleDTO *dto;
    BgConfig *bgConfig;
    Window *window;
    PaletteData *paletteData;
    SavePalPad *palPad[4];
    u8 sendBuffer[0x1000];
    u8 recvCount;
    u8 handshakeStep;
    u16 syncTimer;
    void *waitDial;
} LinkBattleCommState;
u8 *sub_02074F94(int param0, void *commState, int param2);
void sub_02074F9C(BattleSystem *battleSys, int param1, int param2, void *param3, u8 param4);
void sub_02075028(int param0, int param1, void *param2, void *param3);

s32 sub_02074F74(u32 param_1, s32 *param_2) {
    return *param_2 + 0x5c;
}

s32 sub_02074F7C(u32 param_1, s32 *param_2);

s32 sub_02074F7C(u32 param_1, s32 *param_2) {
    return *param_2 + 0xc4;
}

u32 sub_02074F84(u32 param_1, u8 **param_2);

u32 sub_02074F84(u32 param_1, u8 **param_2) {
    return *(u32 *)(*param_2 + 8);
}

u32 sub_02074F8C(u32 param_1, u8 **param_2);

u32 sub_02074F8C(u32 param_1, u8 **param_2) {
    return *(u32 *)(*param_2 + 0x10);
}

u8 *sub_02074F94(int param0, void *commState, int param2) {
    LinkBattleCommState *linkBattleCommState = commState;
    return (u8 *)linkBattleCommState->palPad[param0];
}

void sub_02074F9C(BattleSystem *battleSys, int param1, int param2, void *param3, u8 param4) {
    int v0;
    BattleMessageInfo *info;
    u8 *v2;
    u8 *v3;
    u16 *v4;
    u16 *v5;

    info = (BattleMessageInfo *)Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleMessageInfo));
    v3 = BattleSystem_GetServerMessage(battleSys);
    v4 = BattleSystem_GetServerWriteIndex(battleSys);
    v5 = BattleSystem_GetServerEndIndex(battleSys);

    if (v4[0] + sizeof(BattleMessageInfo) + param4 + 1 > 0x1000) {
        v5[0] = v4[0];
        v4[0] = 0;
    }

    info->recipient = param1;
    info->battler = param2;
    info->size = param4;

    v2 = (u8 *)info;

    for (v0 = 0; v0 < sizeof(BattleMessageInfo); v0++) {
        v3[v4[0]] = v2[v0];
        v4[0]++;
    }

    v2 = (u8 *)param3;

    for (v0 = 0; v0 < param4; v0++) {
        v3[v4[0]] = v2[v0];
        v4[0]++;
    }

    Heap_Free(info);
}

void sub_02075028(int param0, int param1, void *param2, void *param3) {
    BattleSystem *battleSys = (BattleSystem *)param3;
    int v1;
    u8 *v2 = (u8 *)param2;
    u8 *v3 = BattleSystem_GetClientMessage(battleSys);
    u16 *v4 = BattleSystem_GetClientWriteIndex(battleSys);
    u16 *v5 = BattleSystem_GetClientEndIndex(battleSys);

    if (v4[0] + param1 + 1 > 0x1000) {
        v5[0] = v4[0];
        v4[0] = 0;
    }

    for (v1 = 0; v1 < param1; v1++) {
        v3[v4[0]] = v2[v1];
        v4[0]++;
    }
}
