#include "global.h"

#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "pokemon_summary_app.h"
#include "pokemon_summary_args.h"
#include "pokemon_types_def.h"
#include "sav_chatot.h"
#include "unk_02088288.h"

typedef struct {
    char unk_00[51];
    u8 padding_33[1];
    int unk_34;
    u16 unk_38;
    u16 unk_3A;
    u32 unk_3C;
} UnkStruct_02030CEC;
u32 GetMonData(Pokemon *mon, int param, void *dest);
enum SummaryStatus {
    SUMMARY_CONDITION_POKERUS = 0,
    SUMMARY_CONDITION_PARALYSIS,
    SUMMARY_CONDITION_FREEZE,
    SUMMARY_CONDITION_SLEEP,
    SUMMARY_CONDITION_POISON,
    SUMMARY_CONDITION_BURN,
    SUMMARY_CONDITION_FAINTED,
    SUMMARY_CONDITION_NONE,
};

int sub_0208AD54(void);
u32 sub_0208AD58(void);
u32 sub_0208AD5C(void);
u32 sub_0208AD60(void);
u32 Pokemon_GetStatusIconId(Pokemon *mon);

void sub_0208AD34(PokemonSummaryArgs *summary, PlayerProfile *trainerInfo) {
    summary->OTName = PlayerProfile_GetNamePtr(trainerInfo);
    summary->OTID = PlayerProfile_GetTrainerID(trainerInfo);
    summary->OTGender = (u8)PlayerProfile_GetTrainerGender(trainerInfo);
}

int sub_0208AD54(void) {
    return sizeof(UnkStruct_02030CEC);
}

u32 sub_0208AD58(void) {
    return 65;
}

u32 sub_0208AD5C(void) {
    return 63;
}

u32 sub_0208AD60(void) {
    return 62;
}

u32 Pokemon_GetStatusIconId(Pokemon *mon) {
    u32 statusCondition = GetMonData(mon, MON_DATA_STATUS, 0L);

    if (GetMonData(mon, MON_DATA_HP, 0L) == 0) {
        return SUMMARY_CONDITION_FAINTED;
    } else if ((statusCondition & ((1 << 3) | (1 << 7))) != 0) {
        return SUMMARY_CONDITION_POISON;
    } else if ((statusCondition & ((1 << 0) | (1 << 1) | (1 << 2))) != 0) {
        return SUMMARY_CONDITION_SLEEP;
    } else if ((statusCondition & (1 << 4)) != 0) {
        return SUMMARY_CONDITION_BURN;
    } else if ((statusCondition & (1 << 5)) != 0) {
        return SUMMARY_CONDITION_FREEZE;
    } else if ((statusCondition & (1 << 6)) != 0) {
        return SUMMARY_CONDITION_PARALYSIS;
    }

    return SUMMARY_CONDITION_NONE;
}

void sub_0208ADB8(PokemonSummary *arg0, enum MenuInputState state);

void sub_0208ADB8(PokemonSummary *arg0, enum MenuInputState state) {
    MenuInputStateMgr *stateMgr;

    stateMgr = arg0->args->menuInputStatePtr;
    if (stateMgr != NULL) {
        MenuInputStateMgr_SetState(stateMgr, state);
    }
}
