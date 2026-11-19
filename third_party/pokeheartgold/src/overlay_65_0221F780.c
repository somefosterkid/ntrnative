#include "global.h"
struct TradeRoom;

#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "unk_02037C94.h"
#include "overlay_65_trade_room.h"

void ov65_0221F780(struct TradeRoom *param_1);

void ov65_0221F850(struct TradeRoom *arg0);
void ov65_0221F864(struct TradeRoom *arg0);

void ov65_0221F780(TradeRoom *param_1) {
    if (*((WaitingIcon * *)&param_1->waitDialHandle) != NULL) {
        sub_0200F450(*((WaitingIcon * *)&param_1->waitDialHandle));
        *((u32 *)&param_1->waitDialHandle) = 0;
    }
    return;
}

u32 ov65_0221F79C(u8 *param_1);

u32 ov65_0221F79C(u8 *param_1) {
    s32 iVar1;
    s32 *piVar2;
    s32 iVar3;
    s32 iVar4;

    iVar4 = 0;
    iVar1 = Party_GetCount(*(Party **)(param_1 + 0x2224));
    iVar3 = 0;
    if (0 < iVar1) {
        piVar2 = (s32 *)(param_1 + 0x94);
        do {
            if (iVar3 != *piVar2) {
                iVar4 = iVar4 + (u32) * (u8 *)(param_1 + 0x6a1);
            }
            iVar3 = iVar3 + 1;
            param_1 = param_1 + 0x10;
        } while (iVar3 < iVar1);
    }
    if (iVar4 == iVar1 + -1) {
        return 0;
    }
    return 1;
}

u32 ov65_0221F7E0(u8 *param_1);

u32 ov65_0221F7E0(u8 *param_1) {
    s32 iVar1;
    Pokemon *pPVar2;
    u32 uVar3;
    s32 iVar4;

    iVar1 = Party_GetCount(*(Party **)(param_1 + 0x2224));
    iVar4 = 0;
    if (0 < iVar1) {
        do {
            pPVar2 = Party_GetMonByIndex(*(Party **)(param_1 + 0x2224), iVar4);
            uVar3 = GetMonData(pPVar2, 3, (void *)0x0);
            if (uVar3 != 0) {
                return 1;
            }
            iVar4 = iVar4 + 1;
        } while (iVar4 < iVar1);
    }
    iVar1 = Party_GetCount(*(Party **)(param_1 + 0x2228));
    iVar4 = 0;
    if (0 < iVar1) {
        do {
            pPVar2 = Party_GetMonByIndex(*(Party **)(param_1 + 0x2228), iVar4);
            uVar3 = GetMonData(pPVar2, 3, (void *)0x0);
            if (uVar3 != 0) {
                return 2;
            }
            iVar4 = iVar4 + 1;
        } while (iVar4 < iVar1);
    }
    return 0;
}

void ov65_0221F850(TradeRoom *arg0) {
    *((s32 *)&arg0->connectionTimeoutArmed) = 1;
    *((s32 *)&arg0->connectionTimeoutFrames) = 0;
}

void ov65_0221F864(TradeRoom *arg0) {
    if (*((s32 *)&arg0->connectionTimeoutArmed) != 0) {
        *((s32 *)&arg0->connectionTimeoutFrames) = (s32)(*((s32 *)&arg0->connectionTimeoutFrames) + 1);
        if ((s32) *((s32 *)&arg0->connectionTimeoutFrames) > 0x708) {
            sub_02039AD8(4);
        }
    }
}
