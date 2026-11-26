#include "global.h"

#include "overlay_manager.h"
#include "pokemon.h"
#include "overlay_70_gtsapplication_state.h"

PlayerProfile *ov70_0224182C(GTSPokemonListing *);

s32 sub_0202DB64(void *, Pokemon *);

extern OverlayManagerTemplate ov70_02245D50;

typedef struct UnkStruct_ov70_022414A0 {
    u8 pad[0x11F0];
    Pokemon *mon;
} UnkStruct_ov70_022414A0;

s32 ov70_022414A0(GTSApplicationState *arg0);

s32 ov70_022414A0(GTSApplicationState *arg0) {
    u32 temp_r0;

    ((UnkStruct_ov70_022414A0 *)arg0)->mon = AllocMonZeroed(HEAP_ID_61);
    temp_r0 = arg0->screenArgument;
    switch (temp_r0) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    case 7:
        arg0->tradeAnimationConfig.sendingPokemon = Mon_GetBoxMon((Pokemon *)&arg0->receivedListing);
        arg0->tradeAnimationConfig.receivingPokemon = arg0->tradeAnimationConfig.sendingPokemon;
        arg0->receivingPokemonTrainer = ov70_0224182C(&arg0->receivedListing);
        arg0->tradeAnimationConfig.otherTrainer = arg0->receivingPokemonTrainer;
        arg0->tradeAnimationConfig.background = 3;
        arg0->tradeAnimationConfig.tradeType = 2;
        break;
    case 8:
        arg0->tradeAnimationConfig.receivingPokemon = Mon_GetBoxMon((Pokemon *)&arg0->receivedListing);
        arg0->tradeAnimationConfig.sendingPokemon = arg0->tradeAnimationConfig.receivingPokemon;
        arg0->receivingPokemonTrainer = ov70_0224182C(&arg0->receivedListing);
        arg0->tradeAnimationConfig.otherTrainer = arg0->receivingPokemonTrainer;
        arg0->tradeAnimationConfig.background = 3;
        arg0->tradeAnimationConfig.tradeType = 4;
        break;
    case 10:
        arg0->tradeAnimationConfig.receivingPokemon = Mon_GetBoxMon((Pokemon *)&arg0->receivedListing);
        sub_0202DB64(*(void **)arg0->playerData, ((UnkStruct_ov70_022414A0 *)arg0)->mon);
        arg0->tradeAnimationConfig.sendingPokemon = Mon_GetBoxMon(((UnkStruct_ov70_022414A0 *)arg0)->mon);
        arg0->receivingPokemonTrainer = ov70_0224182C(&arg0->receivedListing);
        arg0->tradeAnimationConfig.otherTrainer = arg0->receivingPokemonTrainer;
        arg0->tradeAnimationConfig.background = 3;
        arg0->tradeAnimationConfig.tradeType = 1;
        break;
    case 9:
        sub_0202DB64(*(void **)arg0->playerData, ((UnkStruct_ov70_022414A0 *)arg0)->mon);
        arg0->tradeAnimationConfig.sendingPokemon = Mon_GetBoxMon(((UnkStruct_ov70_022414A0 *)arg0)->mon);
        arg0->tradeAnimationConfig.receivingPokemon = Mon_GetBoxMon((Pokemon *)&arg0->searchResults[arg0->selectedSearchResult]);
        arg0->receivingPokemonTrainer = ov70_0224182C(&arg0->searchResults[arg0->selectedSearchResult]);
        arg0->tradeAnimationConfig.otherTrainer = arg0->receivingPokemonTrainer;
        arg0->tradeAnimationConfig.background = 3;
        arg0->tradeAnimationConfig.tradeType = 1;
        break;
    }
    arg0->tradeAnimationConfig.options = *(struct Options **)((u8 *)arg0->playerData + 0x24);
    arg0->appMan = OverlayManager_New(&ov70_02245D50, &arg0->tradeAnimationConfig, HEAP_ID_61);
    arg0->appManActive = 1;
    return 2;
}
