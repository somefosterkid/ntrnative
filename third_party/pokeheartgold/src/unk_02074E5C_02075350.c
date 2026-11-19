#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "party.h"
#include "pokemon_types_def.h"
#include "trainer_data.h"
#include "unk_020379A0.h"
#include "battle/battle_system.h"
#include "heap.h"
#include "sys_task_api.h"
#include "unk_02035900.h"
#include "battle/battle_controller.h"
#include "overlay_12_field_battle_dto.h"

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

BOOL sub_02036FD8(int cmd, const void *data, int size);

int sub_02037190(void);

BOOL sub_02075350(LinkBattleCommState *linkBattleCommState, int param1, int param2);

BOOL sub_020753D4(LinkBattleCommState *linkBattleCommState, int param1, int param2);
void sub_02075424(int param0, int param1, void *param2, void *commState);
typedef struct {
    BattleSystem *battleSys;
    u8 unk_04;
} UnkStruct_0207ACB4;
void sub_02075434(SysTask *param0, void *param1);

typedef struct {
    BattleSystem *battleSys;
    u8 unk_04;
} UnkStruct_0207AD40;
void sub_020754C0(SysTask *param0, void *param1);

void sub_02075534(int param0, int param1, void *param2, void *param3);

BOOL sub_02075350(LinkBattleCommState *linkBattleCommState, int param1, int param2) {
    if (sub_02037190() != 264) {
        return 0;
    }

    if (sub_02037B38(param2) == 0) {
        return 0;
    }

    if (param1 == 1) {
        return sub_02036FD8(29, (void *)&linkBattleCommState->sendBuffer[0], sizeof(Trainer));
    } else {
        return sub_02036FD8(30, (void *)&linkBattleCommState->sendBuffer[0], sizeof(Trainer));
    }
}

void sub_02075398(int param0, int param1, void *param2, void *commState);

void sub_02075398(int param0, int param1, void *param2, void *commState) {
    LinkBattleCommState *linkBattleCommState = (LinkBattleCommState *)commState;
    linkBattleCommState->recvCount++;
}

u32 sub_020753A8(u8 **param_1, s32 param_2);

u32 sub_020753A8(u8 **param_1, s32 param_2) {
    s32 iVar1;

    iVar1 = sub_02037190();
    if (iVar1 != 0x108) {
        return 0;
    }
    Party_Copy(*(Party **)(*param_1 + param_2 * 4 + 4), (Party *)(param_1 + 8));
    return 1;
}

BOOL sub_020753D4(LinkBattleCommState *linkBattleCommState, int param1, int param2) {
    if (sub_02037190() != 264) {
        return 0;
    }

    if (sub_02037B38(param2) == 0) {
        return 0;
    }

    if (param1 == 1) {
        return sub_02036FD8(31, (void *)&linkBattleCommState->sendBuffer[0], PartyCore_sizeof());
    } else {
        return sub_02036FD8(32, (void *)&linkBattleCommState->sendBuffer[0], PartyCore_sizeof());
    }
}

void sub_02075424(int param0, int param1, void *param2, void *commState) {
    LinkBattleCommState *linkBattleCommState = (LinkBattleCommState *)commState;
    linkBattleCommState->recvCount++;
}

void sub_02075434(SysTask *param0, void *param1) {
    UnkStruct_0207ACB4 *v0 = (UnkStruct_0207ACB4 *)param1;
    u8 *v1;
    u16 *v2;
    u16 *v3;
    u16 *v4;
    int v5;

    v1 = BattleSystem_GetServerMessage(v0->battleSys);
    v2 = BattleSystem_GetServerReadIndex(v0->battleSys);
    v3 = BattleSystem_GetServerWriteIndex(v0->battleSys);
    v4 = BattleSystem_GetServerEndIndex(v0->battleSys);

    switch (v0->unk_04) {
    case 0:
        if (sub_02037190() != 264) {
            break;
        }

        if (v2[0] == v3[0]) {
            break;
        }

        if (v2[0] == v4[0]) {
            v2[0] = 0;
            v4[0] = 0;
        }

        v5 = sizeof(BattleMessageInfo) + (v1[v2[0] + 2] | (v1[v2[0] + 3] << 8));

        if (sub_02037030(23, (void *)&v1[v2[0]], v5) == 1) {
            v2[0] += v5;
        }
        break;
    default:
    case 255:
        Heap_Free(param1);
        SysTask_Destroy(param0);
        break;
    }
}

void sub_020754C0(SysTask *param0, void *param1) {
    UnkStruct_0207AD40 *v0 = (UnkStruct_0207AD40 *)param1;
    u8 *v1;
    u16 *v2;
    u16 *v3;
    u16 *v4;
    int v5;

    v1 = BattleSystem_GetClientMessage(v0->battleSys);
    v2 = BattleSystem_GetClientReadIndex(v0->battleSys);
    v3 = BattleSystem_GetClientWriteIndex(v0->battleSys);
    v4 = BattleSystem_GetClientEndIndex(v0->battleSys);

    switch (v0->unk_04) {
    case 0:
        if (v2[0] == v3[0]) {
            break;
        }

        if (v2[0] == v4[0]) {
            v2[0] = 0;
            v4[0] = 0;
        }

        if (BattleController_RecvCommMessage(v0->battleSys, (void *)&v1[v2[0]]) == 1) {
            v5 = sizeof(BattleMessageInfo) + (v1[v2[0] + 2] | (v1[v2[0] + 3] << 8));
            v2[0] += v5;
        }
        break;
    default:
    case 255:
        Heap_Free(param1);
        SysTask_Destroy(param0);
        break;
    }
}

void sub_02075534(int param0, int param1, void *param2, void *param3) {
    BattleSystem *battleSys = (BattleSystem *)param3;

    ov12_0223BC14(battleSys, 255);
    ov12_0223BC20(battleSys, 255);
    ov12_0223BC2C(battleSys, 1);
}
