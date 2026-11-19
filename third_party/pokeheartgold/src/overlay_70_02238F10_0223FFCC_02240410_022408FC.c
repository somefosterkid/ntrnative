#include "global.h"
struct GTSApplicationState;

#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sav_chatot.h"
#include "save.h"
#include "screen_fade.h"
#include "text.h"
#include "unk_0203A3B0.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238D84(void *, s32, s32);

s32 ov70_02238E50(void *, s32, s32);

s32 ov70_02238F80(void *);

s32 ov70_02244FA4(void *, void *, s32, s32, s32);

s32 ov70_022408FC(GTSApplicationState *arg0);

s32 ov70_022408FC(GTSApplicationState *arg0) {
    if (Save_WriteFileAsync(*(SaveData **)((u8 *)(arg0->playerData) + 0x20)) == 2) {
        ov70_02238E50(arg0, 1, 0);
        ov70_02238F80(arg0);
        ov70_02244FA4(arg0, arg0->gtsMessageLoader, arg0->depositReturnError, 1, 0xF0F);
        ov70_02238D84(arg0, 0x25, 0x1C);
    }
    return 3;
}

s32 ov70_02240948(struct GTSApplicationState *arg0);

s32 ov70_02240948(GTSApplicationState *arg0) {
    ov70_02238F80(arg0);
    sub_0203A914();
    if (*((s32 *)&arg0->fadeBothScreens) == 1) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    } else {
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_61);
    }
    *((s32 *)&arg0->currentScreenInstruction) = 0;
    return 4;
}

s32 ov70_022409A0(GTSApplicationState *arg0);

s32 ov70_022409A0(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32) * &arg0->nextScreenInstruction;
    }
    return 3;
}

s32 sub_0202DB5C(void *, s16);

s32 sub_0202DB70(void *, Pokemon *, int);

void ov70_022409C0(GTSApplicationState *arg0, s32 arg1);

void ov70_022409C0(GTSApplicationState *arg0, s32 arg1) {
    Pokemon *mon;
    Pokemon *pokemon;

    if (arg0->selectedBoxId != 0x12) {
        pokemon = AllocMonZeroed(HEAP_ID_61);
        CopyBoxPokemonToPokemon(PCStorage_GetMonByIndexPair(*(PCStorage **)((u8 *)(arg0->playerData) + 0xC), (u32) arg0->selectedBoxId, (u32) arg0->partySlotIndex), pokemon);
        sub_0202DB70(*(void **)(arg0->playerData), pokemon, arg0->selectedBoxId);
        PCStorage_DeleteBoxMonByIndexPair(*(PCStorage **)((u8 *)(arg0->playerData) + 0xC), (u32) arg0->selectedBoxId, (u32) arg0->partySlotIndex);
        Heap_Free(pokemon);
    } else {
        mon = Party_GetMonByIndex(*(Party **)((u8 *)(arg0->playerData) + 8), (s32) arg0->partySlotIndex);
        Pokemon_RemoveCapsule(mon);
        sub_0202DB70(*(void **)(arg0->playerData), mon, arg0->selectedBoxId);
        Party_RemoveMon(*(Party **)((u8 *)(arg0->playerData) + 8), (s32) arg0->partySlotIndex);
        if (Party_HasMon(*(Party **)((u8 *)(arg0->playerData) + 8), 0x1B9U) == 0) {
            Chatot_Invalidate(Save_Chatot_Get(*(SaveData **)((u8 *)(arg0->playerData) + 0x20)));
        }
    }
    if (arg1 != 0) {
        sub_0202DB5C(*(void **)(arg0->playerData), 1);
    }
}
