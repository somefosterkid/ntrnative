#include "global.h"

#include "battle/battle_022378C0.h"
#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "battle/trainer_ai.h"

#include "heap.h"
#include "overlay_12_battle_controller_opponent.h"
#include "palette.h"
#include "sys_task_api.h"
#include "pokepic.h"
#include "overlay_12_set_mosaic_data.h"
#include "battle/battle_party_context.h"

enum BattlePartyMode {
    BATTLE_PARTY_MODE_SELECT_POKEMON = 0,
    BATTLE_PARTY_MODE_SELECT_POKEMON_NO_CANCEL,
    BATTLE_PARTY_MODE_USE_ITEM,
    BATTLE_PARTY_MODE_LEARN_MOVE,
};

void ov08_0221BE20(BattlePartyContext *context);

typedef struct ForgetMoveData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 unused_0B;
    u16 move;
    u8 slot;
    u8 unused_0F;
} ForgetMoveData;

void ov12_02260B30(SysTask *arg0, void *arg1);

void ov12_022609F8(SysTask *task, void *data) {
    ForgetMoveData *forgetMoveData = data;
    PaletteData *paletteData = BattleSystem_GetPaletteData(forgetMoveData->battleSys);

    switch (forgetMoveData->state) {
    case 0:
        PaletteData_BeginPaletteFade(paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -8, 0, 16, 0);
        forgetMoveData->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            ov12_02237B0C(forgetMoveData->battleSys);

            forgetMoveData->battlePartyCtx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyContext));
            forgetMoveData->battlePartyCtx->party = BattleSystem_GetParty(forgetMoveData->battleSys, forgetMoveData->battler);
            forgetMoveData->battlePartyCtx->battleSys = forgetMoveData->battleSys;
            forgetMoveData->battlePartyCtx->heapID = HEAP_ID_BATTLE;
            forgetMoveData->battlePartyCtx->selectedPartyIndex = forgetMoveData->slot;
            forgetMoveData->battlePartyCtx->moveToLearn = forgetMoveData->move;
            forgetMoveData->battlePartyCtx->battlePartyExited = 0;
            forgetMoveData->battlePartyCtx->doubleBattleFirstSelectedPartySlot = 0;
            forgetMoveData->battlePartyCtx->battlePartyMode = BATTLE_PARTY_MODE_LEARN_MOVE;
            forgetMoveData->battlePartyCtx->selectedBattleBagItem = 0;
            forgetMoveData->battlePartyCtx->battler = forgetMoveData->battler;
            forgetMoveData->battlePartyCtx->isCursorEnabled = 0;

            ov08_0221BE20(forgetMoveData->battlePartyCtx);
            forgetMoveData->state++;
        }
        break;
    case 2:
        if (forgetMoveData->battlePartyCtx->battlePartyExited) {
            ov12_02237BB8(forgetMoveData->battleSys);
            PaletteData_BeginPaletteFade(paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -8, 16, 0, 0);
            forgetMoveData->state++;
        }
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            if (forgetMoveData->battlePartyCtx->selectedMoveSlot == 4) {
                BattleController_EmitPartyMenuResult(forgetMoveData->battleSys, forgetMoveData->battler, 0xFF);
            } else {
                BattleController_EmitPartyMenuResult(forgetMoveData->battleSys, forgetMoveData->battler, forgetMoveData->battlePartyCtx->selectedMoveSlot + 1);
            }

            BattleController_EmitClearCommand(forgetMoveData->battleSys, forgetMoveData->battler, forgetMoveData->command);

            Heap_Free(forgetMoveData->battlePartyCtx);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}

void ov12_02260B30(SysTask *task, void *ptr) {
    s32 value;
    u8 v0;
    u8 v1;
    u8 v2;

    v0 = ((SetMosaicData *)ptr)->state;
    switch (v0) {
    case 0:
        v2 = ((SetMosaicData *)ptr)->counter;
        if (v2 == ((SetMosaicData *)ptr)->wait) {
            ((SetMosaicData *)ptr)->counter = 0U;
            value = Pokepic_GetAttr(((Pokepic *)((SetMosaicData *)ptr)->monSprite), 0x28);
            v1 = ((SetMosaicData *)ptr)->intensity;
            if (value > (s32)v1) {
                value -= 1;
            } else if (value < (s32)v1) {
                value += 1;
            }
            Pokepic_SetAttr(((Pokepic *)((SetMosaicData *)ptr)->monSprite), 0x28, value);
            if (value == ((SetMosaicData *)ptr)->intensity) {
                ((SetMosaicData *)ptr)->state = (u8)(((SetMosaicData *)ptr)->state + 1);
                return;
            }
            return;
        }
        ((SetMosaicData *)ptr)->counter = (u8)(v2 + 1);
        return;
    case 1:
        BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((SetMosaicData *)ptr)->battler, (s32) ((SetMosaicData *)ptr)->command);
        Heap_Free(ptr);
        SysTask_Destroy(task);
        break;
    }
}
