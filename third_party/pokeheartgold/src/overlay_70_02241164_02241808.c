#include "global.h"
struct GTSApplicationState;

#include "heap.h"
#include "player_data.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238E58(void *);

s32 ov70_02241808(struct GTSApplicationState *arg0);

s32 ov70_02241808(GTSApplicationState *arg0) {
    Heap_Free(*((void * *)&arg0->tradeTempPokemon));
    Heap_Free(*((void * *)&arg0->receivingPokemonTrainer));
    ov70_02238E58(arg0);
    return 1;
}

PlayerProfile *ov70_0224182C(GTSPokemonListing *arg0);

PlayerProfile *ov70_0224182C(GTSPokemonListing *arg0) {
    PlayerProfile *profile;

    profile = PlayerProfile_New(HEAP_ID_61);
    PlayerProfile_Init(profile);
    Save_Profile_PlayerName_Set(profile, arg0->trainerNames);
    PlayerProfile_SetVersion(profile, arg0->gameVersion);
    PlayerProfile_SetLanguage(profile, arg0->trainerLanguage);
    return profile;
}
