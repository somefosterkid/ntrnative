#include "global.h"

#include "party.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02240CE4(SaveWiFiHistory *arg0, GTSPokemonListing *arg1);

void ov70_02240CE4(SaveWiFiHistory *arg0, GTSPokemonListing *arg1) {
    sub_02039FB8(arg0, arg1->trainerCountry, arg1->trainerRegion, arg1->trainerLanguage);
}

s32 ov70_0223E76C(Pokemon *);

s32 ov70_02240D00(GTSApplicationState *arg0, void *arg1);

s32 ov70_02240D00(GTSApplicationState *arg0, void *arg1) {
    if ((ov70_0223E76C(arg1) != 0) && (Party_GetCount(*(Party **)((u8 *)(arg0->playerData) + 8)) == 6)) {
        return 2;
    }
    if ((arg0->deferredBoxPokemonCount == 0x21C) && (Party_GetCount(*(Party **)((u8 *)(arg0->playerData) + 8)) == 6)) {
        return 1;
    }
    return 0;
}

void ov70_02240D44(GTSApplicationState *arg0, s16 arg1, s16 arg2);

void ov70_02240D44(GTSApplicationState *arg0, s16 arg1, s16 arg2) {
    arg0->unk_11D4 = arg1;
    arg0->unk_11D6 = arg2;
}

u16 sub_0202DB54(void *);

BOOL ov70_02240D54(GTSApplicationState *arg0);

BOOL ov70_02240D54(GTSApplicationState *arg0) {
    if ((sub_0202DB54(*(void **)arg0->playerData) == 0) && (arg0->isPokemonListed != 0)) {
        return 1;
    }
    return 0;
}
